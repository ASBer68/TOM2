#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>
#pragma hdrstop
#include "MainForm.h"
#pragma package(smart_init)
//===========================================================================
// ��������� ������� �����
//--------------------------------------------------
bool __fastcall TMForm::RefreshCurrWord(void)
{ //�� ��� ����� ����� � �����
  AssocList->Items->Clear();
  AnsiString SQL=
    "SELECT word.id, word.cluster, "           // 0, 1
    "  word.type, meaning.name, word.status, " // 2, 3, 4
    "  word.name, word.fkey, word.defkey, "    // 5, 6, 7
    "  word.obj_code, word.glb_code "          // 8, 9
    "FROM word "
    "LEFT JOIN word AS clust ON clust.id = word.cluster "
    "LEFT JOIN meaning ON meaning.id = word.class_id "
    "WHERE word.name = '";
  SQL+=CurrWord;
  SQL+="'";
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return false;
  }
  MYSQL_RES*Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return false;
  }
  AnsiString ClusterId, Text;
  bool Prep;
  MYSQL_ROW row;
  if(bool(row=mysql_fetch_row(Result)))
  { //id
    WordId = row[0];
    ClusterId= row[1];
    //������
    Prep = strcmp(row[2],"prep") == 0;
    WordTypeButton->Caption = (row[2])?row[2]:"NULL";
    WordClassButton->Caption= (row[3])?row[3]:"�����...";
    StatusButton->Caption=AnsiString("Status = ") + row[4];
    //�����
    Text = "$";
    Text+= row[5];
    Text+= "\r\n\r\nkey = #";
    Text+= row[6];
    Text+= "\r\ndefkey = #";
    Text+= row[7];
    Text+= "\r\n";
    //���
    ObjCode ->Text = row[8];
    GlobCode->Text = row[9];
  }
  else
  { Status->Text="����� �� �������!";
    return false;
  }
  mysql_free_result(Result);
  //--------------------------------------------------
  //��������� ����������
  SQL="SELECT fkey, form FROM form WHERE fkey<>'-' AND word_id = ";
  SQL+=WordId;
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return false;
  }
  Result=mysql_use_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return false;
  }
  while(bool(row=mysql_fetch_row(Result)))
  { if(Prep)
    { //�������
      Text+="\r\n";
      Text+=row[1];
      Text+=" ";
      Text+=row[0];
    }
    else if(row[0] && row[1])
    { //�����
      Text+="\r\n#";
      Text+=row[0];
      Text+=" = ";
      Text+=row[1];
    }
  }
  mysql_free_result(Result);
  //--------------------------------------------------
  //��������� ������� �����
  if(!ClusterId.IsEmpty())
  { SQL="SELECT name FROM word WHERE cluster = ";
    SQL+=ClusterId;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return false;
    }
    Result=mysql_use_result(&DB);
    if(!Result)
    { Status->Text=mysql_error(&DB);
      return false;
    }
    Text+="\r\n\r\n������� �����:\r\n";
    while(bool(row=mysql_fetch_row(Result)))
    { Text+=row[0];
      Text+="\r\n";
    }
    mysql_free_result(Result);
  }
  //--------------------------------------------------
  //��������� ��������� �������
  SQL="SELECT meaning.type, meaning.name "
      "FROM association "
      "JOIN meaning ON meaning.id = object_id "
      "WHERE association.word_id = ";
  SQL+=WordId;
  SQL+=" GROUP BY meaning.id";
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return false;
  }
  Result=mysql_use_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return false;
  }
  bool H=true;
  while(bool(row=mysql_fetch_row(Result)))
  { if(H)
    { H = false;
      Text+="\r\n��� �������� ������������ �������:";
    }
    Text+="\r\n";
    Text+=row[0];
    Text+=" ";
    Text+=row[1];
  }
  mysql_free_result(Result);
  //--------------------------------------------------
  WordCode->Text = Text;
  //--------------------------------------------------
  return true;
}

//---------------------------------------------------------------------------
//������������ ����� (��������)
//����� ��� ����� ������������ ������������� ����� � ���������
AnsiString __fastcall GetTranscription(const char*Form)
{ AnsiString Res;
  bool PrGl=true; //������ ����� ������������ ����������� ��������
  bool PrSf=false;//���������� "������" �������
  for(const char*s=Form; *s; s++)
  { switch(*s)
    { case '`': break; //���������� ��������
      case '�': Res+=(PrGl?"��":(PrSf?"�":"��")); break;
      case '�': Res+=(PrGl?"��":(PrSf?"�":"��")); break;
      case '�'://Res+=(PrGl?"��":(PrSf?"�":"��"));break; ��� � (���������)
      case '�': Res+=(PrGl?"��":(PrSf?"�":"��")); break;
      case '�': Res+=(PrGl?"��":(PrSf?"�":"��")); break;
      case '�': Res+='�'; break;
      case '�': Res+=(PrGl?"?":(PrSf?"":"�")); break;
      case '�': break; //������� �� �����
      default:  Res+=*s;  break;
    }
    PrGl=(bool) strchr("�����������",*s);
    PrSf=(bool) strchr("������",*s);
  }
  //������ ��������� �������� ���������� �������
  int Pos;
  while((bool)(Pos=Res.Pos("�")))
  { Res=Res.Delete(Pos,1);
    Pos--;
    switch(Res[Pos])
    { case '�': Res[Pos]='b'; break;
      case '�': Res[Pos]='v'; break;
      case '�': Res[Pos]='g'; break;
      case '�': Res[Pos]='d'; break;
      case '�': Res[Pos]='z'; break;
      case '�': Res[Pos]='k'; break;
      case '�': Res[Pos]='l'; break;
      case '�': Res[Pos]='m'; break;
      case '�': Res[Pos]='n'; break;
      case '�': Res[Pos]='p'; break;
      case '�': Res[Pos]='r'; break;
      case '�': Res[Pos]='s'; break;
      case '�': Res[Pos]='t'; break;
    }
  }
  return Res;
}

//---------------------------------------------------------------------------
void __fastcall TMForm::TestTranscription(TObject*Sender)
{ AnsiString WordForm;
  if(InputStr(WordForm,"������� �����","������������"))
  {
    Status->Text=GetTranscription(WordForm.c_str());
  }
}

void __fastcall TMForm::SetTranscription(TObject*Sender)
{ Status->Text="������������ ����";
  if(mysql_query(&DB,"SELECT id, form FROM form"))
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_RES*Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_ROW row;
  AnsiString SQL;
  while(bool(row=mysql_fetch_row(Result)))
  { Status->Text=row[1];
    Application->ProcessMessages();

    SQL="UPDATE form SET search_form = '";
    SQL+=GetTranscription(row[1]);
    SQL+="' WHERE id = ";
    SQL+=row[0];
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      mysql_free_result(Result);
      return;
    }
  }
  mysql_free_result(Result);
  Status->Text="������������ ���������";
}

//---------------------------------------------------------------------------
void __fastcall TMForm::SetTraditional(TObject*Sender)
{ //��������� ��� search_form �� ��������� ���� ����
  Status->Text="��������� ����";
  if(mysql_query(&DB,"SELECT id, form FROM form"))
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_RES*Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  int p;
  AnsiString SQL, Form;
  MYSQL_ROW row;
  while(bool(row=mysql_fetch_row(Result)))
  { Status->Text=row[1];
    Application->ProcessMessages();
    Form = row[1];
    Form = Form.LowerCase();
    while((bool)(p=Form.Pos("�"))) Form[p]='�'; //�->�
    while((bool)(p=Form.Pos("`"))) Form = Form.Delete(p,1); //������� ��������

    SQL="UPDATE form SET search_form = '";
    SQL+=Form;
    SQL+="' WHERE id = ";
    SQL+=row[0];
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      mysql_free_result(Result);
      return;
    }
  }
  mysql_free_result(Result);
  Status->Text="��������� ���������";
}

//---------------------------------------------------------------------------
const char*SuffixList[]=   //������ ��������� � ������ �������
{ "��", "��", "��", "��", "��", "��", "��", "��", "��", 0 };

const char*VerbSuffixList[][2]= //������ ��������� ��� ��������
{ "���", "��",   //����� - �����
  "���", "��",   //���� - ����
  "���", "���",  //����� - ����
  "���", "���",  //���� - ������
  "���", "�",    //���� - ���
  "���", "��",   //������ - ����
  "���", "���",  //������ - �����
  "���", "��",   //���� - ����
  "���", "��",   //���� - ����
  "���", "����", //������ - �������
  "���", "���",  //����� - ���
  "���", "���",  //���� - ������
  "���", "�",    //������ - �����, ������ - �����
  "���", "���",  //���� - �����
  "���", "����", //������ - �������
  "���", "�",
  "���", "����", //������ - �������
  "���", "�",    //������� - �����
  "���", "���",  //������� - ��������

  "���", "��",   //����� - ����� (3 ����� �� ��������������!)
  "���", "��",   //����� - �����
  "���", "��",   //������ - ������
  "���", "��",   //���� - ����
  "���", "��",   //������� - �������

  0
};

void __fastcall TMForm::FormWithoutEnds(char*FName)
{ //���������� ���� ���� � ����������� (��� ���������)
  Status->Text="��������� ����";
  //������ ����� ���������
  TStringList*NounList = new TStringList;
  TStringList*VerbList = new TStringList;
  //������ ���� ��������������� � ��������������
  AnsiString FileName = FName;
  FileName += ".noun";
  int handle=open(FileName.c_str(), O_RDONLY|O_BINARY);
  if(handle==-1)
  { Status->Text=L"���� .noun �� ������!";
    return;
  }
  long size=filelength(handle);
  if(size<0||size>1000000)
  { Status->Text=L"�� ������� ���������� ������ ����� .noun!";
    delete NounList;
    delete VerbList;
    return;
  }
  char*buf=new char[size+1];
  if(read(handle,buf,size)==-1)
  { delete buf;
    delete NounList;
    delete VerbList;
    Status->Text=L"������ ����� .noun ����������!";
    return;
  }
  buf[size]=0;  //null terminator
  NounList->Text = buf;
  close(handle);
  delete buf;
  //������ ���� ��������� ��������
  FileName = FName;
  FileName += ".verb";
  handle=open(FileName.c_str(), O_RDONLY|O_BINARY);
  if(handle==-1)
  { Status->Text=L"���� .verb �� ������!";
    delete NounList;
    delete VerbList;
    return;
  }
  size=filelength(handle);
  if(size<0||size>1000000)
  { Status->Text=L"�� ������� ���������� ������ ����� .verb!";
    delete NounList;
    delete VerbList;
    return;
  }
  buf=new char[size+1];
  if(read(handle,buf,size)==-1)
  { delete buf;
    delete NounList;
    delete VerbList;
    Status->Text=L"������ ����� .verb ����������!";
    return;
  }
  buf[size]=0;  //null terminator
  VerbList->Text = buf;
  close(handle);
  delete buf;

  //���������� ��� ����� �����
  if
  ( mysql_query(&DB, "SELECT form.id, search_form, type "
                     "FROM form "
                     "JOIN word ON word_id = word.id")
  )
  { Status->Text=mysql_error(&DB);
    delete NounList;
    delete VerbList;
    return;
  }
  MYSQL_RES*Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    delete NounList;
    delete VerbList;
    return;
  }
  int p;
  AnsiString SQL, Form, End, Type;
  MYSQL_ROW row;
  while(bool(row=mysql_fetch_row(Result)))
  { Status->Text=row[1];
    Application->ProcessMessages();
    Form = row[1];
    Type = row[2];
    End = "";
    //����������� �� ����� ���������
    int MinBas = 3;
    if(Form.SubString(1,3)=="���") MinBas = 4;
/*
    if(Form.SubString(1,3)=="���") MinBas = 4;
    if(Form.SubString(1,4)=="����")MinBas = 5;
*/
    if(Type=="verb")
    { //������ ����� ���� ����������
      int l=Form.Length();
      bool Reflexive = l>2 && Form[l-1]=='�' && (Form[l]=='�' || Form[l]=='�');
      AnsiString BaseForm = Form;
      if(Reflexive) Form = Form.Delete(l-1,2); //��� ��
      //������� ��������� �������
      for(int i=0; i<VerbList->Count; i++)
      { AnsiString DelEnd, AddEnd;
        int p=VerbList->Strings[i].Pos("\t"); //����������� ������
        if(p)
        { DelEnd = VerbList->Strings[i].SubString(1,p-1);
          AddEnd = VerbList->Strings[i].SubString(p+1,1000);
        }
        else DelEnd = VerbList->Strings[i];

        int l = DelEnd.Length();
        int f = Form.Length() - l + 1;
        if(f+AddEnd.Length() > MinBas && Form.SubString(f,l)==DelEnd)
        { Form = Form.Delete(f,l) + AddEnd; //������ ���������
          End = DelEnd;
          break;
        }
        if(Reflexive && DelEnd!="�" && DelEnd!="�")
        { //�������� ��������� �������� � ������� �����
          f = BaseForm.Length() - l + 1;
          if(f+AddEnd.Length() > MinBas && BaseForm.SubString(f,l)==DelEnd)
          { Form = BaseForm.Delete(f,l) + AddEnd; //������ ���������
            End = DelEnd;
            Reflexive = false;
            break;
          }
        }
      }
      //��������� ���������
      int f = Form.Length()-2;
      for(int i=0; f>0 && VerbSuffixList[i][0]; i++)
      { AnsiString Suf = Form.SubString(f,3);
        if(Suf==VerbSuffixList[i][0])
        { Form = Form.Delete(f,3) + VerbSuffixList[i][1];
          f = Form.Length()-2;
        }
      }
      //���������� ������
      if(Reflexive) Form += "�";
    }
    else
    { //������� ��������� �� ��������
      for(int i=0; i<NounList->Count; i++)
      { int l = NounList->Strings[i].Length();
        int f = Form.Length() - l + 1;
        if(f > MinBas && Form.SubString(f,l)==NounList->Strings[i])
        { Form = Form.Delete(f,l); //������� ���������
          End = NounList->Strings[i];
          break;
        }
      }
      //������� ������������� �����
      AnsiString Form2=Form.SubString(1,1);
      for(int i=2; i<=Form.Length(); i++)
        if(Form2[Form2.Length()] != Form[i]) Form2+=Form[i];
      Form=Form2;
      //������� ���
      int L=Form.Length();
      if(L>6 && Form.SubString(L-2,4)=="���") //�������, �����
      { Form = Form.SubString(1,L-2);
        L-=2;
      }
      //������� ������ ������� � �������
      if(L>5)
      { bool Suffix = false;
        for(int i=0; !Suffix && SuffixList[i]; i++)
          Suffix = Form.SubString(L-1,2)==SuffixList[i];
        if(Suffix && strchr("���������������",Form[L-2])) Form=Form.Delete(L-1,1);
      }
      //������� ������������� �����
      Form2=Form.SubString(1,1);
      for(int i=2; i<=Form.Length(); i++)
        if(Form2[Form2.Length()] != Form[i]) Form2+=Form[i];
      Form=Form2;
    }

    //������� ���
    unsigned int Hash32 = 0x67452301;
    if(Type=="verb") Hash32 = 0xEFCDAB89;
    for(int p=1; p<=Form.Length(); p++)
    { unsigned int HBit = Hash32 >> 27; //������� ���� �������� � �������
      Hash32 = (Hash32 << 5) | HBit;
      Hash32 ^= Form[p] - '�'; //��������� ������� ������
    }

    unsigned short Hash16 = 0x6745;
    if(Type=="verb") Hash16 = 0xAB89;
    for(int p=1; p<=Form.Length(); p++)
    {
      //unsigned short HBit = Hash16 >> 11; //������� ���� �������� � �������
      //Hash16 = (Hash16 << 5) | HBit;
      //Hash16 ^= Form[p] - '�'; //��������� ������� ������
      unsigned short Ch = (Form[p] - '�')^031;
      unsigned short HBit;
      if(Ch&0x16)
      { HBit = Hash16 >> 11; //������� ���� �������� � �������
        Hash16 = (Hash16 << 5) | HBit;
      }
      else if(Ch&0x8)
      { HBit = Hash16 >> 12; //������� ���� �������� � �������
        Hash16 = (Hash16 << 4) | HBit;
      }
      else if(Ch&0x4)
      { HBit = Hash16 >> 13; //������� ���� �������� � �������
        Hash16 = (Hash16 << 3) | HBit;
      }
      else
      { HBit = Hash16 >> 14; //������� ���� �������� � �������
        Hash16 = (Hash16 << 2) | HBit;
      }
      Hash16 ^= Ch; //��������� ������� ������
    }

    SQL="UPDATE form SET work_form = '";
    SQL+=Form;
    SQL+="', work_end = '";
    SQL+=End;
    SQL+="', hash32 = ";
    SQL+=Hash32;
    SQL+=", hash16 = ";
    SQL+=Hash16;
    SQL+=" WHERE id = ";
    SQL+=row[0];
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      mysql_free_result(Result);
      delete NounList;
      delete VerbList;
      return;
    }
  }
  mysql_free_result(Result);
  delete NounList;
  delete VerbList;

  Status->Text="��������� ���������";
}

void __fastcall TMForm::SetWithoutEnd(TObject*Sender)
{ //���������� ���� ���� � ����������� (��� ���������)
  FormWithoutEnds("test_ends");
}


//---------------------------------------------------------------------------
//����� ����� � ����
bool __fastcall TMForm::WordToFile(const AnsiString&Id, int FileH)
{
  if(FileH==-1)
  { Status->Text="���� �� ������";
    return false; //��� �� ������
  }
  //������� ��� ����� �� ����
  AnsiString SQL="SELECT code FROM word WHERE id = ";
  SQL+=Id;
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return false;
  }
  MYSQL_RES*Result=mysql_use_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return false;
  }
  MYSQL_ROW row;
  bool Ok=true;
  if(bool(row=mysql_fetch_row(Result)))
  { if(row[0] && row[0][0]) //���� �������� ���
    { write(FileH,"\n",1);
      write(FileH,row[0],StrLen(row[0]));
      write(FileH,"\n",1);
    }
    else
    { Ok=false;
      Status->Text="��� ����� �� ����������� ���!";
    }
  }
  else
  { Ok=false;
    Status->Text="����� �� �������!";
  }
  mysql_free_result(Result);
  return Ok;
}
//===========================================================================
AnsiString WordName;
void __fastcall TMForm::NewWord(TObject *Sender)
{ //������� ����� �����
  WordName="";
  AnsiString MainForm, Type="noun";
  Status->Text="������� ����� �����...";
  if(InputStr(WordName,"������� �����","��������"))
  { //�������� ������������� �����
    AnsiString SQL="SELECT name FROM word WHERE name = '";
    SQL+=WordName + "'";
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    MYSQL_RES*Result=mysql_use_result(&DB);
    if(!Result)
    { Status->Text=mysql_error(&DB);
      return;
    }
    MYSQL_ROW row;
    bool Ex = (row=mysql_fetch_row(Result));
    mysql_free_result(Result);
    if(Ex)
    { Status->Text = "����� ����������";
      return;
    }
    //�������� �������� �����
    MainForm = WordName;
    InputStr(MainForm,"������� �������� �����",0);
    //����������� �������� �����
    MainForm = MainForm.LowerCase();
    int Pos;
    while((bool)(Pos=MainForm.Pos("�"))) MainForm[Pos]='�'; //� -> �
    //�������� ���
    InputStr(Type,"������� ��� �����",0);
    //������� �����
    SQL="INSERT INTO word SET name = '";
    SQL+=WordName + "', main_form = '";
    SQL+=MainForm + "', type = '";
    SQL+=Type + "', status = 1";
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      WordName="";
      return;
    }

    RefreshWordList();
    Status->Text="Ok, ����� ���������";
  }
  else Status->Text="";
}
//---------------------------------------------------------------------------
//����������� �����
void __fastcall TMForm::EditWord(TObject*Sender)
{ if(WordId.IsEmpty())
  { Status->Text="�� ������� �����!";
    return;
  }
  AnsiString WordName, MainForm;
  Status->Text="������� ����� �����...";
  if(InputStr(WordName,"������� �����","��������"))
  { //�������� �������� �����
    MainForm = WordName;
    InputStr(MainForm,"������� �������� �����","��������");
    //����������� �������� �����
    MainForm = MainForm.LowerCase();
    int Pos;
    while((bool)(Pos=MainForm.Pos("�"))) MainForm[Pos]='�'; //� -> �
    //������� �����
    AnsiString SQL="UPDATE word SET name = '";
    SQL+=WordName + "', main_form = '";
    SQL+=MainForm + "' WHERE id = " + WordId;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    CurrWord = WordName;
    RefreshCaption();
    RefreshCurrWord();
    Status->Text="Ok, ����� ��������";
  }
  else Status->Text="";
}
//---------------------------------------------------------------------------
/*
void __fastcall TMForm::SaveWord(TObject*Sender)
{ if(WordId.IsEmpty())
  { Status->Text="�� ������� �����!";
    return;
  }
  Status->Text="��������� ����� � ����...";
  SaveDialog->FileName=CurrWord->Caption+".word";
  if(SaveDialog->Execute())
  { int handle=open(SaveDialog->FileName.c_str(),O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
    if(WordToFile(WordId,handle)) Status->Text="C���� ���������";
    close(handle);
  }
  else Status->Text="";
}
*/
//---------------------------------------------------------------------------
/*
void __fastcall TMForm::SelectWord(TObject *Sender)
{
  //���� �� ������ ����
  int Item=-1;
  for(int i=0; i<WordList->Items->Count; i++)
    if(WordList->Selected[i]) Item=i;
  if(Item==-1) return;
  //������� �����
  CurrWord = WordList->Items->operator[](Item);
  if(RefreshCurrWord())
    Status->Text=CurrWord;
  //--------------------------------------------------
  //��������� �������� �����
  RefreshMeaningList();
  //--------------------------------------------------
  RefreshCaption();
}
*/
//---------------------------------------------------------------------------
void __fastcall TMForm::EditKey(TObject *Sender)
{ if(WordId.IsEmpty())
  { Status->Text="�� ������� �����!";
    return;
  }
  //---------------------------------------------------
  AnsiString SQL="SELECT fkey FROM word WHERE id = ";
  SQL+=WordId;
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_RES*Result=mysql_use_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  AnsiString Key;
  MYSQL_ROW row;
  if(bool(row=mysql_fetch_row(Result))) Key=row[0];
  mysql_free_result(Result);
  //---------------------------------------------------
  if(InputStr(Key,"������� ����","���������"))
  { SQL="UPDATE word SET fkey = '";
    SQL+=Key + "' WHERE id = " + WordId;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    RefreshCurrWord();
    Status->Text="Ok, ���� ��������";
  }
}

//---------------------------------------------------------------------------
void __fastcall TMForm::EditDefKey(TObject *Sender)
{ if(WordId.IsEmpty())
  { Status->Text="�� ������� �����!";
    return;
  }
  //---------------------------------------------------
  AnsiString SQL="SELECT defkey FROM word WHERE id = ";
  SQL+=WordId;
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_RES*Result=mysql_use_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  AnsiString Key;
  MYSQL_ROW row;
  if(bool(row=mysql_fetch_row(Result))) Key=row[0];
  mysql_free_result(Result);
  //---------------------------------------------------
  if(InputStr( Key,"������� ���� �� ���������","���������"))
  { SQL="UPDATE word SET defkey = '";
    SQL+=Key + "' WHERE id = " + WordId;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    RefreshCurrWord();
    Status->Text="Ok, ���� ��������";
  }
}
//---------------------------------------------------------------------------
void __fastcall TMForm::SetStatus(TObject*Sender)
{ if(WordId.IsEmpty())
  { Status->Text="�� ������� �����!";
    return;
  }
  AnsiString Stat;
  if(InputStr( Stat,"������� ������","����������"))
  { AnsiString SQL="UPDATE word SET status = '";
    SQL+=Stat + "' WHERE id = " + WordId;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    if(RefreshCurrWord()) //������� ��� �����
      Status->Text="Ok, ������ ����������";
  }
}

//---------------------------------------------------------------------------
void __fastcall TMForm::NewForm(TObject*Sender)
{ Status->Text="��������� ���� ����������...";
  if(WordId.IsEmpty())
  { Status->Text="�� ������� �����!";
    return;
  }
  //������� �����
  AnsiString Key, Form;
  if(InputForm(Key,Form,"��������"))
  { //����������� �����
    int Pos;
    AnsiString SForm = Form.LowerCase(); //������ �������
    while((bool)(Pos=SForm.Pos("�"))) SForm[Pos]='�'; //� -> �
    while((bool)(Pos=SForm.Pos("`"))) SForm=SForm.Delete(Pos,1);//��� ��������

    AnsiString SQL="INSERT INTO form SET word_id = ";
    SQL+=WordId + ", form = '";
    SQL+=Form + "', search_form = '";
    SQL+=SForm+ "', fkey = '";
    SQL+=Key  + "'";
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    RefreshCurrWord();
    Status->Text="Ok, ����� ���������";
  }
  else Status->Text="";
}
//---------------------------------------------------------------------------
// ����������� ������������ �����
void __fastcall TMForm::EditForm(TObject*Sender)
{ if(WordId.IsEmpty())
  { Status->Text="�� ������� �����!";
    return;
  }
  Status->Text="";
  AnsiString Key;
  if(!InputStr( Key,"������� ���� ���������� �����","�������������"))
    return;
  //������� ���������� �� �����
  AnsiString Id, Form;
  AnsiString SQL="SELECT id, form FROM form WHERE word_id = ";
  SQL+=WordId + " and fkey = '";
  SQL+=Key  + "'";
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_RES*Result=mysql_use_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_ROW row;
  if(bool(row=mysql_fetch_row(Result)))
  { Id=row[0];
    Form=row[1];
  }
  mysql_free_result(Result);

  if(Form.IsEmpty())
  { Status->Text="��� ���������� � ����� ������!";
    return;
  }
  //����������� ���������� � ����
  AnsiString NewKey = Key;
  if(InputForm(NewKey,Form,"��������"))
  { //����������� �����
    int Pos;
    AnsiString SForm = Form.LowerCase(); //������ �������
    while((bool)(Pos=SForm.Pos("�"))) SForm[Pos]='�'; //� -> �
    while((bool)(Pos=SForm.Pos("`"))) SForm=SForm.Delete(Pos,1);//��� ��������
    //������� ����������
    SQL="UPDATE form SET fkey = '";
    SQL+=NewKey + "', form = '";
    SQL+=Form + "', search_form = '";
    SQL+=SForm + "' WHERE id = " + Id;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    RefreshCurrWord();
    Status->Text="Ok, ����� ����������";
  }
}
//---------------------------------------------------------------------------
// ������� ����������
void __fastcall TMForm::DeleteWordForm(TObject*Sender)
{ if(WordId.IsEmpty())
  { Status->Text="�� ������� �����!";
    return;
  }
  Status->Text="";
  AnsiString Key1, Key2;
  if(!InputKeyRange(Key1, Key2, "�������"))
    return;
  //������ ���������� �� id � �����
  AnsiString SQL="DELETE FROM form WHERE word_id = ";
  SQL+=WordId;
  if(Key2.IsEmpty()) //������� ���� �����
  { SQL+=" and fkey = '";
    SQL+=Key1 + "'";
  }
  else //������� �������� ����
  { SQL+=" and fkey >= '";
    SQL+=Key1 + "' and fkey <= '";
    SQL+=Key2 + "'";
  }
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  RefreshCurrWord();
  Status->Text="Ok, ����� �������";
}

//---------------------------------------------------------------------------
void __fastcall TMForm::SetWordClass(TObject*Sender)
{ if(WordId.IsEmpty()) //�������� ������ ���� �������
  { Status->Text="�� ������� �����!";
    return;
  }
  //������� ��� ������� ������
  TToolButton*Btn = (TToolButton*)Sender;
  AnsiString ClassName, OldClassName;
  if(Btn->Caption.Pos("...")==0) OldClassName = ClassName = Btn->Caption;
  //������� ��� ������ ������
  Status->Text="������� ����� ����� ������...";
  if(InputStr(ClassName,"������� ��� ������",0) && ClassName!=OldClassName)
  { //������ Id ������
    AnsiString ClsId;
    AnsiString SQL =
      "SELECT meaning.id FROM meaning "
      "WHERE name = '";
    SQL+=ClassName + "'";
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    MYSQL_RES*Result=mysql_store_result(&DB);
    if(!Result)
    { Status->Text=mysql_error(&DB);
      return;
    }
    MYSQL_ROW row;
    if(bool(row=mysql_fetch_row(Result)))
      ClsId = row[0];
    else
    { MessageBox(0,"������� ����� �����?","����� �� ������!",MB_YESNO|MB_ICONQUESTION);

      //...

      Status->Text="����� �� ������!";
      mysql_free_result(Result);
      return;
    }
    mysql_free_result(Result);
    //��������� �����
    SQL="UPDATE word SET class_id = ";
    SQL+=ClsId + " WHERE id = " + WordId;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    Btn->Caption = ClassName;
    Status->Text="Ok, ����� ����������.";
  }
  else Status->Text="����� �� ������...";
}
//---------------------------------------------------------------------------
//������������ ����������
void __fastcall TMForm::ReNumberClick(TObject *Sender)
{ if(WordId.IsEmpty())
  { Status->Text="�� ������� �����!";
    return;
  }
  Status->Text="";
  AnsiString SQL="SELECT id, fkey FROM form WHERE word_id = ";
  SQL+=WordId;
  SQL+=" ORDER BY id";
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_RES*Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  AnsiString Id, Key;
  int i=1;
  MYSQL_ROW row;
  while(bool(row=mysql_fetch_row(Result)))
  { Id = row[0];
    Key= row[1];
    //������� ����������
    SQL="UPDATE form SET fkey = '";
    SQL+=i;
    SQL+="' WHERE id = ";
    SQL+= Id;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }

    i++;
  }
  mysql_free_result(Result);

  RefreshCurrWord();
  Status->Text="Ok, ���������� ��������������";
}

//---------------------------------------------------------------------------
//�������� ��������� ���������� � ��������� �����
void __fastcall TMForm::SplitClick(TObject *Sender)
{ if(WordId.IsEmpty())
  { Status->Text="�� ������� �����!";
    return;
  }
  //�������� �������� ���������
  AnsiString Key1, Key2;
  if(!InputKeyRange(Key1, Key2, "���������")) return;
  if(Key2.IsEmpty()) return;
  //������� ����� ��� ��������
  NewWord(0);
  if(WordName.IsEmpty()) return; //����� �� ���������
  //������� word_id
  AnsiString SQL="SELECT id FROM word WHERE name = '";
  SQL+=WordName+"'";
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_RES*Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  AnsiString NewId;
  MYSQL_ROW row;
  if(bool(row=mysql_fetch_row(Result))) NewId = row[0];
  else
  { Status->Text="���-�� �� ���...";
    return;
  }
  mysql_free_result(Result);
  //�������� ����������
  SQL="UPDATE form SET word_id = ";
  SQL+=NewId + " WHERE word_id = ";
  SQL+= WordId;
  SQL+=" and fkey >= '";
  SQL+=Key1 + "' and fkey <= '";
  SQL+=Key2 + "'";

  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  RefreshCurrWord();
  Status->Text="Ok, ���������� ��������������";
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TMForm::SaveObjCode(TObject*Sender)
{ if(WordId.IsEmpty())
  { Status->Text="�� ������� �����!";
    return;
  }
  AnsiString SQL;
  //�������� ����������� �����
  if(ObjCode->Text.IsEmpty())
    SQL="UPDATE word SET obj_code = NULL WHERE id = ";
  else
  { SQL="UPDATE word SET obj_code = '";
    SQL+=ObjCode->Text+"' WHERE id = ";
  }
  SQL+=WordId;
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  //�������� ���������� ���
  if(GlobCode->Text.IsEmpty())
    SQL="UPDATE word SET glb_code = NULL WHERE id = ";
  else
  { SQL="UPDATE word SET glb_code = '";
    SQL+=GlobCode->Text+"' WHERE id = ";
  }
  SQL+=WordId;
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  Status->Text="Ok, ��� ��������";
}
//---------------------------------------------------------------------------
void __fastcall TMForm::ShowSynonym(TObject*Sender)
{ //������� �������
  if(CurrSynonym.IsEmpty()) return;
  //������������� ������ ����
  FindWord->Text = CurrSynonym;
  RefreshWordList();
}
//---------------------------------------------------------------------------


