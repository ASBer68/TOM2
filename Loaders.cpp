#include <io.h>
#include <fcntl.h>
#pragma hdrstop
#include "MainForm.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ���������� �� ������ ��������� ��������
//---------------------------------------------------------------------------
char* __fastcall TMForm::LoadFile(void)
{ //��������� ����, ���������� ������ �� �����
  Status->Text="��������� ����";
  if(OpenDialog->Execute())
  { //������� ����
    int handle=open(OpenDialog->FileName.c_str(),O_RDONLY|O_BINARY);
    if(handle==-1)
    { Status->Text="������ �������� �����";
      return 0;
    };
    //������� ������ �����
    long size=filelength(handle);
    if(size==-1||size>1000000000)
    { Status->Text="���� �� ��������!";
      return 0;
    };
    char*buf=new char[size+1];
    //������ ����
    if(read(handle,buf,size)==-1)
    if(size==-1||size>1000000000)
    { Status->Text="������ ������ �����";
      delete buf;
      return 0;
    };
    buf[size]=0;//null terminator
    close(handle);
    ProgressBar->Max = size;
    ProgressBar->Position=0;
    return buf;
  }
  return 0;
}

//---------------------------------------------------------------------------
void __fastcall TMForm::Load5000lemma(TObject*Sender)
{
  char*buf=LoadFile();
  if(!buf) return;
  Status->Text="������ ����� �� �����";
  //-----------------------------------------
  //������ �����
  char*S=buf;
  AnsiString Word, MainForm, F, Type, SQL;
  int Field=0; //����� ����
  StopFlag=false;
  while(*S&&!StopFlag)
  { switch(*S)
    { case 0: StopFlag=true;
      case'\n':case'\r': //����� ������
        Application->ProcessMessages(); //���� �� ��������
        if(!Word.IsEmpty())
        { Status->Text=Word;
          ProgressBar->Position=S-buf;
          //Sleep(10);
          //����������� �������� �����
          MainForm = MainForm.LowerCase();
          //������� �����
          AnsiString SQL="INSERT INTO word SET name = '";
          SQL+=Word + "', main_form = '";
          SQL+=MainForm + "', type = '";
          SQL+=Type+ "', status = 10";
          if(mysql_query(&DB,SQL.c_str()))
          { //Status->Text=mysql_error(&DB);
            //return;

            // ��� ������ � ��������?

          }


        }
        //������� ���������� ��� ����� ������
        Word=""; MainForm=""; F=""; Type="";
        Field=0;
        break;

      case ';': case ',': Field++; break; //����������� �����
      case ' ': break; //���������� �������
      case '�':
        if(Field==2)
        { Word+="�";
          MainForm+="�";
        }
        break; //���������� �������

      //���������� ����
      default: switch(Field)
      { case 0: F+=*S; break;    //�������������
        case 2: Word+=*S; MainForm+=*S; break; //�����
        case 3: Type+=*S; break; //����� ����
      }
    }
    S++;
  }
  //-----------------------------------------
  Status->Text="����� ���������";
  ProgressBar->Position=0;
  delete buf;
}
//---------------------------------------------------------------------------
void __fastcall TMForm::LoadAAZaliznyak(TObject*Sender)
{ char*buf=LoadFile();
  if(!buf) return;
  Status->Text="��������� ���������� �� �����";
  //-----------------------------------------
  //������ �����
  char*S=buf;
  AnsiString Word, Id, Type, Form, Key, SQL;
  int Field=0, p; //����� ����
  bool StrEnd=false;
  StopFlag=false;
  while(*S&&!StopFlag)
  { switch(*S)
    { case 0: StopFlag=true; //����� �����
      case'\n':case'\r': //����� ������
        StrEnd=true;
        if(!Id.IsEmpty())
        { //������� ������ �����
          SQL="UPDATE word SET status = 20 WHERE id = " + Id;
          mysql_query(&DB,SQL.c_str());
        }
      case',':       //����� ����
        if
        ( !Id.IsEmpty() && !Form.IsEmpty() && //���� ����� � �����
          (Field<=15 || Type!="verb") //��� �������� ���� ������ 15 ����
        )
        { //������� �����
          Key=Field; //���� �� ������ ����
          //�������� ���� ��������
          while((bool)(p=Form.Pos("'"))) Form[p]='`';
          //����������� �����
          AnsiString SForm = Form.LowerCase(); //������ �������
          while((bool)(p=SForm.Pos("�"))) SForm[p]='�'; //� -> �
          while((bool)(p=SForm.Pos("`"))) SForm=SForm.Delete(p,1);//��� ��������

          SQL="INSERT INTO form SET word_id = ";
          SQL+=Id + ", form = '";
          SQL+=Form + "', search_form = '";
          SQL+=SForm+ "', fkey = '";
          SQL+=Key  + "'";
          mysql_query(&DB,SQL.c_str());


          Form="";
          Field++;
        }
        if(StrEnd)
        { //������� ���������� ��� ����� ������
          StrEnd=false;
          Word=""; Id=""; Form="";
          Field=0;
        }
        break;

      case'#': //����� �����
        Field=1;
        Application->ProcessMessages(); //���� �� ��������
        ProgressBar->Position=S-buf;
        if(!Word.IsEmpty()) //���� �����
        { //�������� ����� � ������������ ����
          Word = Word.LowerCase();
          int p;
          while((bool)(p=Word.Pos("�"))) Word[p]='�'; //� -> �
          //������ �����, ������� Id
          SQL="SELECT id, type FROM word WHERE main_form = '";
          SQL+=Word+"' AND status>0 AND status<=10";
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
          { Id = row[0];
            Type = row[1];
            Status->Text=Word;
          }
          else Word=""; //��� ������ �����
          mysql_free_result(Result);
        }
        break;

      case ' ': break; //���������� �������

      //���������� ����
      default:
        if(Field) Form+=*S;
        else Word+=*S;
    }
    S++;
  }
  //-----------------------------------------
  Status->Text="�������� ���������";
  ProgressBar->Position=0;
  delete buf;
}

//---------------------------------------------------------------------------
char*HagenNoun[][2]=
{ " ��",  "��",
  " ��",  "��",
  " ��",  "��",
  " ���", "��",
  " ���", "��",
  " ���", "��",
  " ��",  "��",
  " ��",  "��",
  0,0
};
char*HagenVerb[][2]=
{ " ���", "����",
  " ���", "��2���",
  " ����","��",
  " ����","��",
  " ���", "��",
  " ���", "��",
  " ���", "��",
  " ��",  "��",
  " 1-�", "1�",
  " 2-�", "2�",
  " 3-�", "3�",
  " ��",  "����",
  " ��",  "��",
  0,0
};
char*HagenAdj[][2]= //��������������
{ " ��",  "��",
  " ��",  "��",
  " ��",  "��",
  " ���", "��",
  " ���", "��",
  " ���", "��",
  " ��",  "��",
  " ��",  "��",
  " ���", "��",
  " ���", "��",
  " ��",  "��",
  " ����","����",
  0,0
};
char*HagenPart[][2]= //���������
{ " 1���",  "1�",
  " 2���",  "2�",
  " �����", "��",
  " �����", "��",
  " ���",   "��",
  " �����", "��",
  " ���",   "��",
  " ����",  "��",
  " ����",  "��",
  " ���",   "��",

  " ��",  "��",
  " ��",  "��",
  " ��",  "��",
  " ���", "��",
  " ���", "��",
  " ���", "��",
  " ��",  "��",
  " ��",  "��",
  " ���", "��",
  " ���", "��",
  " ��",  "��",
  " ����","����",
  0,0
};


void __fastcall TMForm::LoadHagen(TObject*Sender)
{ char*buf=LoadFile();
  if(!buf) return;
  Status->Text="��������� ���������� �� �����";
  //-----------------------------------------
  //������ �����
  AnsiString SQL, Word, Id, Type, Key, DefKey;
  AnsiString FormRp; //����� ������������ ������
  char** HagenMorph;
  char*Str=buf;//������ ������
  char*Sign;
  int L=0;
  StopFlag=false;
  while(Str[0]&&!StopFlag)
  {
    Str+=L; //������� ������
    if(Str[0]=='*') Str++;

    char*N=strchr(Str,'\n');//������� ������
    char*R=strchr(Str,'\r');//������� �������
    if(!N && !R)
      L=strlen(Str); //����� ��������� ������
    else
    { if(R&&R<N) N=R;
      L=N-Str+2; //����� ������
    }

    if(L<=5)
    { //��� ����������� ����
      Id = "";
      continue;
    }

    if(N) N[0] = 0;

    //�������� �� ���������
    char*Tab=strchr(Str,'\t'); //����� ������� tab ��� �������� �����
    if(Tab)
    { Tab[0] = 0;
      Tab++;
    }
    //�������� ����� � ������������ ����
    Word = Str;
    Word = Word.LowerCase();
    int p;
    while((bool)(p=Word.Pos("�"))) Word[p]='�'; //� -> �

    bool FirstForm = false;
    if(strstr(Tab,"��� ") && strstr(Tab," �� ") && strstr(Tab," ��"))
    { //������ �������� ������ ����������������
      FirstForm = true;
      Sign      = "��� ";
      Type      = "noun";
      HagenMorph= &HagenNoun[0][0];
      DefKey    = "����";
      //������ ���� �����
      Key="";
      if     (strstr(Tab," ��� ")) Key = "��";
      else if(strstr(Tab," ��� ")) Key = "��";
      else if(strstr(Tab," �� "))  Key = "��";
      if     (strstr(Tab," ���� "))Key+= "��";
      else if(strstr(Tab," ���� "))Key+= "��";
    }
    else if(strstr(Tab,"�� ") && strstr(Tab," ���"))
    { //������ �������� ������ �������
      FirstForm = true;
      Sign      = "�� ";
      Type      = "verb";
      HagenMorph= &HagenVerb[0][0];
    }
    else if
    ( Id.IsEmpty() && strstr(Tab,"��� ") &&
      ( strstr(Tab," �� ��� ��") ||
        strstr(Tab,"���� ") && strstr(Tab," �� ���")
      )
    )
    { //������ �������� ������ ���������
      FirstForm = true;
      Sign      = "��� ";
      Type      = "part";
      HagenMorph= &HagenPart[0][0];
      Key       = "";
      DefKey    = "��������";
    }

    if(FirstForm)
    { //������� ��������
      ProgressBar->Position=Str-buf;
      Application->ProcessMessages();
      //������ �����, ������� Id
      SQL="SELECT id FROM word WHERE main_form = '";
      SQL+=Word+"' AND type='";
      SQL+=Type+"' AND status>0 AND status<=10";
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
      if(bool(row=mysql_fetch_row(Result))) Id = row[0];
      else Id=""; //��� ������ �����
      mysql_free_result(Result);

      if(Id.IsEmpty()) continue; //��� ������������ �����

      if(Type=="verb")
      { //����������� �������������
        AnsiString Q="����� ";
        Q+=Word;
        Q+=" - ��� ���������?";
        switch
        ( Application->MessageBoxA( Q.c_str(),"������������� �������",
                                    MB_YESNOCANCEL|MB_ICONQUESTION)
        )
        { case IDYES: //�������������
            Key = "��";
            DefKey = "����3���";
            break;
          case IDNO: //�����������
            Key = "��";
            DefKey = "����3���";
            break;
          default:
            Id = "";
            continue; //���������� ������
        }
      }

      //������� ������ �����
      SQL="UPDATE word SET status = 25, fkey = '";
      SQL+=Key + "', defkey = '";
      SQL+=DefKey + "' WHERE id = ";
      SQL+=Id;
      mysql_query(&DB,SQL.c_str());
      //������� ������ ��������
      Status->Text=Word;
      Application->ProcessMessages();
    }

    if(Id.IsEmpty()) continue;      //��� ������������ �����
    if(!strstr(Tab,Sign))
    { Id=="";
      continue; //��������� �� ������������� ���� �����
    }

    //������� ���� ������� �����
    Key="";
    for(int i=0; HagenMorph[i]; i+=2)
      if(strstr(Tab,HagenMorph[i]))
        Key+=HagenMorph[i+1];
    //2-� ����� ��.������
    if(strstr(Tab," �� ��� ��") && Word.SubString(Word.Length()-1,2)=="��")
      Key+="��";
    //������������� ��.������
    if((Type=="adj"||Type=="part") && (strstr(Tab," ���")||strstr(Tab," ��")))
      if(strstr(Tab," ���")) FormRp = Word; //�������� ���.�����
      else if(strstr(Tab," ���")) //��������� ������������� ������������
        Key+=(Word==FormRp)?"��":"��";


    //������� �����
    SQL="INSERT INTO form SET word_id = ";
    SQL+=Id + ", form = '";
    SQL+=Str;
    SQL+="', search_form = '";
    SQL+=Word+ "', fkey = '";
    SQL+=Key + "'";
    mysql_query(&DB,SQL.c_str());

  }

  Status->Text="�������� ���������";
  ProgressBar->Position=0;
  delete buf;
}

//---------------------------------------------------------------------------
//�������� ���� �� ���������� �����
void __fastcall TMForm::LoadText(TObject*Sender)
{ char*buf=LoadFile();
  if(!buf) return;
  Status->Text="������ ����� �� �����";
  //-----------------------------------------
  //������ �����
  char*S=buf;
  StopFlag=false;
/*
  while(!StopFlag)
  { if(


  }
*/
  //-----------------------------------------
  Status->Text="����� ���������";
  ProgressBar->Position=0;
  delete buf;
}
//---------------------------------------------------------------------------

