#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>
#pragma hdrstop
#include "MainForm.h"
#include "DBaction.h"

#define add_StrCode
#include "..\DicStruct.h" //���� � ��������� �������� �������
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ������ �������� ��������� ��� ������ ����� �������
//---------------------------------------------------------------------------
bool ErrorMake;
void __fastcall TMForm::MakeAll(TObject*Sender)
{ //��������� ��� �������� �� ������
  ErrorMake=false;
  ProgressBar->Max = 5000; //���� ��������� ������� � ����������������!
  ProgressBar->Position=0;
  //�����-������
  MakeAssociation(Sender);
  if(ErrorMake) return;
  //������
  MakeEntries(Sender);
  if(ErrorMake) return;
  //hash-�������
  MakeHashTab(Sender);
  if(ErrorMake) return;
  //��������� ������
  MakeDic(Sender);
  ProgressBar->Position=0;
}
//---------------------------------------------------------------------------
dic_header Hdr;
//----------------
bool __fastcall AttachFile(int File1, int File2)
{ //������������ ���� 2 � ����� 1
  if(File1<0 || File2<0) return false;
  int L = filelength(File2);
  char*Buf = new char[L];
  read(File2,Buf,L);
  write(File1,Buf,L);
  delete Buf;
  return true;
}

//---------------------------------------------------------------------------
void __fastcall TMForm::MakeDic(TObject*Sender)
{ Status->Text="�������� ���� �������";
  Application->ProcessMessages();
  //������� ��������� �����
  int hHashTmp=open("hash_tab.tmp",O_RDONLY|O_BINARY);
  if(hHashTmp==-1)
  { Status->Text="�� ������ ���� hash_tab.tmp";
    ErrorMake=true;
    return;
  }

  int hStrArrayVerb=open("StrArray.verb",O_RDONLY|O_BINARY);
  if(hStrArrayVerb==-1)
  { Status->Text="�� ������ ���� StrArray.verb";
    ErrorMake=true;
    return;
  }

  int hStrArrayNoun=open("StrArray.noun",O_RDONLY|O_BINARY);
  if(hStrArrayNoun==-1)
  { Status->Text="�� ������ ���� StrArray.noun";
    ErrorMake=true;
    return;
  }

  int hEntrTmp=open("entries.tmp",O_RDONLY|O_BINARY);
  if(hEntrTmp==-1)
  { Status->Text="�� ������ ���� entries.tmp";
    ErrorMake=true;
    return;
  }

  //������ ���� �������
  int hDic=open("TOM.dic",O_WRONLY|O_CREAT|O_TRUNC|O_BINARY|O_APPEND,
                          S_IREAD|S_IWRITE );
  if(hDic==-1)
  { Status->Text="�� ������� ������� ���� TOM.dic";
    ErrorMake=true;
    return;
  }

  //����� ������ � ������ ����������� �����
  memcpy(Hdr.Version,"2.a.4.9",8);  //������

  Hdr.ShiftNoun = sizeof(Hdr)
                + filelength(hHashTmp)
                + filelength(hStrArrayVerb);

  Hdr.ShiftEntr = Hdr.ShiftNoun
                + filelength(hStrArrayNoun);

  write(hDic,&Hdr,sizeof(Hdr));

  //������������ ����� �������������� �����
  AttachFile(hDic,hHashTmp);
  AttachFile(hDic,hStrArrayVerb);
  AttachFile(hDic,hStrArrayNoun);
  AttachFile(hDic,hEntrTmp);

  //��������� �����
  close(hHashTmp);
  close(hStrArrayVerb);
  close(hStrArrayNoun);
  close(hEntrTmp);
  close(hDic);

  ProgressBar->Position+=100;
  Application->ProcessMessages();

  Status->Text="���� ������� ������!";
}

//==============================================================================
char*MeaningFormat[][2] = //������� ��� ������������ ��������
{ //�����
  {"word","$%N%{\n%C%\n%A%\n%B%\n}\n"},
  //��������
  //%F% - �����
  {"prep","preposition %N%\n{%V%%B%\n}\n"},
  //�������
  {"action",    "\naction(%A%)\n{%B%\n}\n"},
  {"fact",      "\nfact(%A%)\n{%B%\n}\n"},
  {"expression","\nexpression(%A%)\n{%B%\n}\n"},
  //�������� �������
  {"act_syn",   "\n%W%\naction(%A%) = action(%P%)\n"},
  {"fact_syn",  "\n%W%\nfact(%A%) = fact(%P%)\n"},
  {"exp_syn",   "\n%W%\nexpression(%A%) = expression(%P%)\n"},
  //����������
  {"variable",  "variable %N%=%A%\n"},
  {"prp",       "property %N%=%A%\n"},
  {"tmp",       "template %N%=%A%\n"},
  {"atr",       "%N%\n"},
  //��������
  {"synonym",   "%W%\n%N%.synonym=$%A%\n"},
  //������ � �������
  {"class",     "\n@%N%\n{%C%\n%V%\n%B%\n}\n"},
  {"location",  "\nlocation %N%\n{%C%\n%V%\n%B%\n}\n"},
  {0,           "\n%T% %N%\n{%C%\n%V%\n%B%\n}\n"},
};
AnsiString __fastcall GetObjectCode( const char*T,   //%T% - ��� �������
                                     const char*N,   //%N% - ��� �������
                                     const char*C,   //%C% - ����� �������
                                     const char*A,   //%A% - ���������
                                     const char*B=0, //%B% - ���� �������
                                     const char*P=0, //%P% - ��������� ��������
                                     const char*W=0  //%W% - �������� �����
                                   )
{ //��������� ��� ������� �� ������� �� �������� ����������
  //��� ���������� ������������� ��������� ���������� ������ ������
  if(!T||!T[0]) return ""; //��� ������ ����!
  //������ ������
  AnsiString Text;
  for(int i=0;;i++)
    if(!MeaningFormat[i][0] || strcmp(MeaningFormat[i][0],T)==0)
    { Text = MeaningFormat[i][1];
      break;
    }
  //������� ���������
  int p;
  if((bool)(p=Text.Pos("%T%"))) Text=Text.Delete(p,3).Insert(T,p);
  if((bool)(p=Text.Pos("%N%"))) Text=Text.Delete(p,3).Insert(N,p);
  if((bool)(p=Text.Pos("%C%"))) Text=Text.Delete(p,3).Insert(C,p);
  //-------------------------------------------
  if((bool)(p=Text.Pos("%A%")))
  { if(!A||!A[0]) return ""; //��������� ������ ����!
    Text=Text.Delete(p,3).Insert(A,p);
  }
  //variables - �������������� ��������� � ��������
  if((bool)(p=Text.Pos("%V%"))) Text=Text.Delete(p,3).Insert(A,p);
  //-------------------------------------------
  if((bool)(p=Text.Pos("%B%"))) Text=Text.Delete(p,3).Insert(B,p);
  if((bool)(p=Text.Pos("%P%")))
  { if(!P||!P[0]) return ""; //��������� ������ ����!
    Text=Text.Delete(p,3).Insert(P,p);
  }
  if((bool)(p=Text.Pos("%W%"))) Text=Text.Delete(p,3).Insert(W,p);

  while((bool)(p=Text.Pos("\n\n"))) Text=Text.Delete(p,1);
  return Text;
}
//---------------------------------------------------------------------
void __fastcall TMForm::MakeEntries(TObject*Sender)
{ AnsiString SQL;
  Status->Text="�������� ��������� ������";
  Application->ProcessMessages();
  //--------------------------------------------------
  //������� �������
  if(mysql_query(&DB,"DROP TABLE IF EXISTS entries"))
  { Status->Text=mysql_error(&DB);
    ErrorMake=true;
    return;
  }
  //������ �������
  if(mysql_query(&DB,
    "CREATE TABLE entries("
    "cluster INT NOT NULL PRIMARY KEY," //������ �� �����
    "addr INT," //����� ����� � �����
    "code TEXT,"//���������� ������
    "len INT"   //����� ������
    ") default charset=cp1251" //��������� �� ���������
  ))
  { Status->Text=mysql_error(&DB);
    ErrorMake=true;
    return;
  }
  //--------------------------------------------------
  //������� ������ ���������
  if(mysql_query(&DB,"SELECT cluster, name "
                     "FROM word "
                     "WHERE status>=100 AND cluster=id" ))
  { Status->Text=mysql_error(&DB);
    ErrorMake=true;
    return;
  }
  MYSQL_RES*Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    ErrorMake=true;
    return;
  }

  MYSQL_ROW row;
  while(bool(row=mysql_fetch_row(Result))) //���� �� ���������
  { AnsiString Text; //����� ������
    AnsiString Cluster=row[0]; //id ��������
    Status->Text=row[1];
    Application->ProcessMessages();
    //������ ����� ���� � ��������
    SQL =
    "SELECT DISTINCT form, accentuation "
    "FROM form "
    "JOIN word ON word_id = word.id "
    "WHERE word.status >=100 AND word.Cluster = ";
    SQL+=row[0];
    SQL+=" ORDER BY form";

    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      ErrorMake=true;
      return;
    }
    MYSQL_RES*Word=mysql_store_result(&DB);
    if(!Word)
    { Status->Text=mysql_error(&DB);
      ErrorMake=true;
      return;
    }
    AnsiString FormList[256];//������ ���������
    int FormCnt = 1; //���������� ���������
    MYSQL_ROW row2;
    for(; bool(row2=mysql_fetch_row(Word)); FormCnt++) //���� �� ����������
    { if(FormCnt>127)
      { AnsiString Err="��������� ���������� ���������� ��������� ��� �������� ";
        Err+=row[1];
        Status->Text = Err;
        ErrorMake=true;
        return;
      }
      if(FormCnt=='\'' || FormCnt=='\r') //��� ���� ������ ������������!
      { FormList[FormCnt++]="-";
        Text+="-;";
      }
      FormList[FormCnt]=row2[0];
      //��������� ��������
      if(row2[1] && row2[1][0]=='1')
        Text+=row2[0]; //�������� ��������
      else
      { //������ ��������
        AnsiString TmpTx = row2[0];
        int p = TmpTx.Pos("`");
        if(p) Text+=TmpTx.Delete(p,1);
        else Text+=row2[0];
      }
      Text+=";";
    }
    mysql_free_result(Word);
    Text+=";";

    //������ ��������� ����� ������
    SQL = "SELECT word.id, word.name, word.type, word.fkey, "     //0,1,2,3,
          "word.defkey, 'deleted', word.obj_code, meaning.name "  //4,6,7,
          "FROM word "
          "LEFT JOIN meaning ON word.class_id = meaning.id "
          "WHERE word.status>=100 AND word.cluster = ";
    SQL+= Cluster;
    sql qWord(SQL,ErrorMake);
    while(qWord.Next()) //���� �� ������
    { if(ErrorMake) return; //������
      bool ItsPrep = (qWord[2] && strcmp(qWord[2],"prep")==0);
      //������ ����� -----------------------------------------
      AnsiString Forms;
      SQL = "SELECT form, fkey FROM form WHERE word_id = ";
      SQL+= qWord[0];
      sql Form(SQL,ErrorMake);
      char Code[3];
      Code[0] = 4; //form
      Code[2] = 0;
      while(Form.Next())
      { if(Form[1][0]=='-') continue;
        if(ItsPrep)
        { Forms+=Form[0];
          Forms+=" ";
          Forms+=Form[1];
          Forms+="\n";
        }
        else
        { //������ ������� ����������
          bool Find=false;
          for(int f=1; !FormList[f].IsEmpty(); f++)
            if(FormList[f]==Form[0])
            { Code[1] = f;
              Forms+=Code;
              Find=true;
              break;
            }
          if(!Find)
          { Status->Text="�� ������� ����� ����� � ������ ��������!";
            ErrorMake=true;
            return;
          }
          Forms+=Form[1]; //����
        }
      }
      //������ ����� � ����� ---------------------------------
      AnsiString Class;
      if(qWord[7]&&qWord[7][0])
      { Class+="\n@";
        Class+=qWord[7];
      }
      //������� ����
      if(qWord[3] && qWord[3][0])
      { Class+="\nkey = #";
        Class+=qWord[3];
      }
      //������� ����������� ����
      if(qWord[4]&&qWord[4][0])
      { Class+="\ndefkey = #";
        Class+=qWord[4];
      }
      //������� ����� -----------------------------------------
      Text+=GetObjectCode( ItsPrep?"prep":"word", qWord[1], Class.c_str(),
                           Forms.c_str(), qWord[6] );
    }

    //����� � ������� �������� ����
    SQL="SELECT meaning.id, meaning.type, meaning.name, "   //0,1,2,
        "class.type,  class.name, "                         //3,4,
        "class2.type, class2.name, "                        //5,6,
        "class3.type, class3.name, "                        //7,8,
        "meaning.description, meaning.code, "               //9,10
        "class.description, verb.name, part_own.name "      //11,12,13
        "FROM word "
        "JOIN meaning ON meaning.word_id = word.id "
        "LEFT JOIN meaning AS class  ON class.id  = meaning.class_id "
        "LEFT JOIN meaning AS class2 ON class2.id = meaning.class2_id "
        "LEFT JOIN meaning AS class3 ON class3.id = meaning.class3_id "
        "LEFT JOIN word AS verb ON verb.id = class.word_id "
        "LEFT JOIN meaning AS part_own ON part_own.id = meaning.part_id "
        "WHERE word.cluster = ";
    SQL+=Cluster;
    SQL+=" ORDER BY meaning.id";
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      ErrorMake=true;
      return;
    }
    MYSQL_RES*Meaning=mysql_store_result(&DB);
    if(!Meaning)
    { Status->Text=mysql_error(&DB);
      ErrorMake=true;
      return;
    }
    MYSQL_ROW row3;
    while(bool(row3=mysql_fetch_row(Meaning)))
    { //���� �� ���������
      if(ErrorMake) return; //��������� ������
      //������ ----------------------------------
      AnsiString ClassList;
      for(int c=3; c<7; c+=2)
        if(row3[c]) //���� �����
        { if(strcmp(row3[c],"class")==0) ClassList+="@";
          else ClassList+="is ";
          ClassList+=row3[c+1];
          ClassList+="\n";
        }
      //������� ----------------------------------
      if(row3[7]) //���� �������
      { ClassList+="like ";
        if(strcmp(row3[7],"class")==0) ClassList+="@";
        ClassList+=row3[8];
        ClassList+="\n";
      }
      //�������������� ����� --------------------
      SQL = "SELECT word.name "
            "FROM synonym "
            "JOIN word ON word_id = word.id "
            "WHERE status>=100 AND (add_name OR cluster = ";
      SQL+=Cluster+") AND object_id = ";
      SQL+= row3[0];
      sql Q(SQL,ErrorMake);
      while(Q.Next()) //���� �� ������
      { ClassList+="synonym=$";
        ClassList+=Q[0]; //�����
        ClassList+="\n";
      }
      //���� ������� ----------------------------
      AnsiString Body, Methods;
      SQL = "SELECT type, name, code "
            "FROM association "
            "WHERE word_id IS NULL AND object_id = ";
      SQL+= row3[0];
      sql Prp(SQL,ErrorMake);
      while(Prp.Next()) //���� �� ���������
        if
        ( strcmp(Prp[0],"prp")==0 ||
          strcmp(Prp[0],"tmp")==0 ||
          strcmp(Prp[0],"atr")==0
        ) Body+=GetObjectCode(Prp[0],Prp[1],0,Prp[2]); //�������� ��� �������
        else
        { //�����
          Methods+=Prp[0];
          Methods+=Prp[1];
          Methods+=Prp[0];
          Methods+="\n";
          Methods+=Prp[2];
          Methods+="\n";
        }
      Body+=row3[10]; //������
      Body+="\n";
      Body+=Methods;
      //-----------------------------------------------------------------
      Text+=GetObjectCode( row3[1], row3[2], ClassList.c_str(),
                           row3[9], Body.c_str(), row3[11], row3[12] );
      //-----------------------------------------------------------------
      if(row3[13])
      { //�������� ����� ��� �����
        Text+=row3[13];
        Text+="\n";
      }
    }
    mysql_free_result(Meaning);

    //������� �������� --------------------------------------------------
    SQL =
    "SELECT word.name, meaning.type, meaning.name, synw.name " //0, 1, 2, 3
    "FROM synonym "
    "JOIN meaning ON meaning.id = synonym.object_id "
    "JOIN word ON word.id = meaning.word_id AND word.status >=100 "
    "JOIN word AS synw ON synw.id = synonym.word_id AND synw.status >=100 "
    "WHERE NOT add_name AND word.cluster <> synw.cluster AND synw.cluster = ";
    SQL+=Cluster;
    sql S(SQL);
    while(S.Next()) //���� �� ���������
    { AnsiString Name;
      if(strcmp(S[1],"class")==0) Name="@";
      Name+=S[2];
      Text+=GetObjectCode("synonym",Name.c_str(),0,S[3],0,0,S[0]);
    }

    //���-����� ---------------------------------------------------------
    SQL =
    "SELECT word.name, synw.name "
    "FROM synonym "
    "JOIN meaning ON meaning.id = synonym.object_id "
    "JOIN word ON word.id = meaning.word_id AND word.status >=100 "
    "JOIN word AS synw ON synw.id = synonym.word_id AND synw.status >=100 "
    "WHERE add_name AND word.cluster <> synw.cluster AND synw.cluster = ";
    SQL+=Cluster;
    sql D(SQL);
    while(D.Next()) //���� �� ���������
    { Text+="$";
      Text+=D[0];
      Text+="\n";
    }

    //-------------------------------------------------------------------
    //������� ���������� ��� ����
    SQL =
    "SELECT glb_code FROM word "
    "WHERE status>=100 AND cluster = ";
    SQL+= Cluster;

    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      ErrorMake=true;
      return;
    }
    Word=mysql_store_result(&DB);
    if(!Word)
    { Status->Text=mysql_error(&DB);
      ErrorMake=true;
      return;
    }
    for(int i=0; bool(row2=mysql_fetch_row(Word)); i++) //���� �� ������
    { if(!i) Text+="\n";
      Text+=row2[0];
    }
    mysql_free_result(Word);
    //----------------------------------------------------
    //������ ������
    int p;
    //������� ��� �������� ������� � ������ �������� �����
    while(bool(p=Text.Pos("\r")))  Text=Text.Delete(p,1);
    while(bool(p=Text.Pos("\n\n")))Text=Text.Delete(p,1);
    //�������� ������
    for(int i=0; StrCode[i].Code; i++) //���� �� ������� �����
    { if(!StrCode[i].Str || !StrCode[i].Str[0]) continue;
      int l = strlen(StrCode[i].Str) - 1;
      while(bool(p=Text.Pos(StrCode[i].Str)))
      { Text[p]=StrCode[i].Code;
        Text=Text.Delete(p+1,l);
      }
    }
    //----------------------------------------------------
    //��������� ��������� ����� �����-������ ��� ����� ��������
    int CrossLen = 4;
    SQL = "SELECT SUM(len) FROM cross_entries WHERE cluster_min = ";
    SQL+=Cluster;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      ErrorMake=true;
      return;
    }
    Word=mysql_store_result(&DB);
    if(!Word)
    { Status->Text=mysql_error(&DB);
      ErrorMake=true;
      return;
    }
    if(bool(row2=mysql_fetch_row(Word)) && row2[0])
      CrossLen+=AnsiString(row2[0]).ToInt();
    mysql_free_result(Word);

    SQL = "SELECT COUNT(*) FROM cross_entries WHERE cluster_max =";
    SQL+=Cluster;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      ErrorMake=true;
      return;
    }
    Word=mysql_store_result(&DB);
    if(!Word)
    { Status->Text=mysql_error(&DB);
      ErrorMake=true;
      return;
    }
    if(bool(row2=mysql_fetch_row(Word)) && row2[0])
      CrossLen+=AnsiString(row2[0]).ToInt()*2;
    mysql_free_result(Word);
    //----------------------------------------------------
    //������� ������
    SQL ="INSERT INTO entries SET cluster = ";
    SQL+=Cluster;
    SQL+=", code ='";
    SQL+=Text + "', len = ";
    SQL+=Text.Length() + 1 + CrossLen;

    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      ErrorMake=true;
      return;
    }
    ProgressBar->Position++;
    Application->ProcessMessages();
  }
  mysql_free_result(Result);

  //=======================================================================
  //�������� �������� ��� ��� ��������� ������
  if(mysql_query(&DB,
      "SELECT SUM(len), COUNT(cluster) "
      "FROM entries"
  ))
  { Status->Text=mysql_error(&DB);
    ErrorMake=true;
    return;
  }
  Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    ErrorMake=true;
    return;
  }
  if(bool(row=mysql_fetch_row(Result)))
  { int SUMlen= AnsiString(row[0]).ToInt(); //c������� ����� ���� ������
    int COUNT = AnsiString(row[1]).ToInt(); //���������� ������
    Hdr.EntrStep = (SUMlen+COUNT) / (0xFFFF-COUNT) + 1;
  }
  else
  { Status->Text="�� ������� ��������� SUM(len) � COUNT(cluster)";
    ErrorMake=true;
    return;
  }
  mysql_free_result(Result);
  //--------------------------------------------------
  //����������� ������ ������
  Status->Text="������������ ������ ��������� ������";
  Application->ProcessMessages();
  if(mysql_query(&DB,"SELECT cluster, len FROM entries"))
  { Status->Text=mysql_error(&DB);
    ErrorMake=true;
    return;
  }
  Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  unsigned short Addr=0;
  while(bool(row=mysql_fetch_row(Result)))
  { //if(Addr>=0xC000)
    if(Addr>=0xFFFF)
    { Status->Text="��������� ������ ��������� 0xFFFF!!!";
      ErrorMake=true;
      break;
    }
    const char*clst= row[0]; //������� �������
    int len = AnsiString(row[1]).ToInt();
    int A = len/Hdr.EntrStep + 1; //���������� �������� � ������
    //������� ������ � ������
    SQL="UPDATE entries SET addr = ";
    SQL+=Addr;
    SQL+=" WHERE cluster = ";
    SQL+=clst;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      ErrorMake=true;
      return;
    }
    //������� ������ � �����-������
    SQL="UPDATE cross_entries SET addr_min = ";
    SQL+=Addr;
    SQL+=" WHERE cluster_min = ";
    SQL+=clst;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      ErrorMake=true;
      return;
    }
    SQL="UPDATE cross_entries SET addr_max = ";
    SQL+=Addr;
    SQL+=" WHERE cluster_max = ";
    SQL+=clst;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      ErrorMake=true;
      return;
    }
    //�������� ����� �� ���������� �������� � ������
    Addr+=A;
  }
  mysql_free_result(Result);
  //--------------------------------------------------
  //��������� ������ �� ��������� ����
  Status->Text="��������� ��������� ������ � ����";
  Application->ProcessMessages();
  int hEntr=open("entries.tmp",O_WRONLY|O_CREAT|O_TRUNC|O_BINARY|O_APPEND,
                               S_IREAD|S_IWRITE );
  if(hEntr==-1)
  { Status->Text="�� ������� ������� ���� entries.tmp";
    ErrorMake=true;
    return;
  }
  if(mysql_query(&DB,"SELECT cluster, addr, len, code FROM entries"))
  { Status->Text=mysql_error(&DB);
    ErrorMake=true;
    return;
  }
  Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  Addr=0;
  while(bool(row=mysql_fetch_row(Result)))
  {
    const char*clst= row[0]; //������� �������
    if(AnsiString(row[1]).ToInt()!=Addr)
    { Status->Text="����� ������ �� ��������� � �����������!";
      ErrorMake=true;
      break;
    }
    //����� ������
    int EntLen = strlen(row[3]) + 1; //����� ������ ������� 0
    write(hEntr, row[3], EntLen);
    //����������� �����-������
    SQL = "SELECT addr_max, code FROM cross_entries WHERE cluster_min = ";
    SQL+=clst;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      ErrorMake=true;
      return;
    }
    MYSQL_RES*Cross=mysql_store_result(&DB);
    if(!Cross)
    { Status->Text=mysql_error(&DB);
      ErrorMake=true;
      return;
    }
    unsigned short CrossAddr;
    MYSQL_ROW row2;
    while(bool(row2=mysql_fetch_row(Cross))) //���� �� �����-�������
    { //������� �����
      CrossAddr = AnsiString(row2[0]).ToInt();
      write(hEntr, &CrossAddr, 2);
      write(hEntr, row2[1], StrLen(row2[1])+1);
    }
    mysql_free_result(Cross);
    CrossAddr = 0xFFFF;
    write(hEntr, &CrossAddr, 2);

    //����������� ������ �� �����-������
    SQL = "SELECT addr_min FROM cross_entries WHERE cluster_max = ";
    SQL+=clst;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      ErrorMake=true;
      return;
    }
    Cross=mysql_store_result(&DB);
    if(!Cross)
    { Status->Text=mysql_error(&DB);
      ErrorMake=true;
      return;
    }
    while(bool(row2=mysql_fetch_row(Cross))) //���� �� �����-�������
    { //������� �����
      CrossAddr = AnsiString(row2[0]).ToInt();
      write(hEntr, &CrossAddr, 2);
    }
    mysql_free_result(Cross);
    CrossAddr = 0xFFFF;
    write(hEntr, &CrossAddr, 2);

    //�������� ��������� ������ ������
    int len = AnsiString(row[2]).ToInt();
    int A = len/Hdr.EntrStep + 1; //���������� �������� � ������
    for(int i=A*Hdr.EntrStep-len; i>0; i--) write(hEntr,"\0",1);

    //�������� ����� �� ����������� �������� � ������
    Addr+=A;
  }
  mysql_free_result(Result);
  close(hEntr);

  //--------------------------------------------------
  if(!ErrorMake) Status->Text="���� ��������� ������ ������";
  ProgressBar->Position+=100;
  Application->ProcessMessages();
  //--------------------------------------------------
}

//==============================================================================
void __fastcall TMForm::MakeAssociation(TObject*Sender)
{ AnsiString SQL;
  Status->Text="�������� ��������� ����������";
  Application->ProcessMessages();
  //--------------------------------------------------
  //������� �������
  if(mysql_query(&DB,"DROP TABLE IF EXISTS cross_entries"))
  { Status->Text=mysql_error(&DB);
    ErrorMake=true;
    return;
  }
  //������ �������
  if(mysql_query(&DB,
    "CREATE TABLE cross_entries("
    "cluster_min INT NOT NULL," //
    "cluster_max INT NOT NULL," //
    "addr_min INT,"  //����� ��������� ������
    "addr_max INT,"  //����� ��������� ������
    "code TEXT," //���������� ������
    "len INT,"   //����� ������
    "UNIQUE KEY(cluster_min,cluster_max)," //���������� ������ �� ���������
    "KEY(cluster_min),"        //������ �� ��������
    "KEY(cluster_max)"         //������ �� ��������
    ") default charset=cp1251" //��������� �� ���������
  ))
  { Status->Text=mysql_error(&DB);
    ErrorMake=true;
    return;
  }
  //--------------------------------------------------
  //������� ���������� �����-������
  if(mysql_query(&DB,
    "SELECT word1.cluster AS cluster1, word2.cluster AS cluster2, "   //0, 1,
    "meaning.type AS objtype, meaning.name AS objname, "              //2, 3,
    "association.type AS prptype, association.name, association.code "//4, 5, 6
    "FROM association "
    "JOIN word AS word1 ON word_id = word1.id "
    "JOIN meaning ON object_id = meaning.id "
    "JOIN word AS word2 ON meaning.word_id = word2.id "
    "WHERE word1.status >=100 AND word2.status >=100 "
    "ORDER BY cluster1, cluster2, objtype, objname, prptype DESC" )
  )
  { Status->Text=mysql_error(&DB);
    ErrorMake=true;
    return;
  }
  MYSQL_RES*Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    ErrorMake=true;
    return;
  }

  AnsiString Text; //����� ������
  int ClusterMin, ClusterMax;
  AnsiString strClust0, strClust1, ObjType, ObjName;

  MYSQL_ROW row;
  for(;;)
  {
    bool IsRow = (row = mysql_fetch_row(Result));

    if
    ( !strClust0.IsEmpty() && !strClust1.IsEmpty() &&
      ( !IsRow || strClust0!=row[0] || strClust1!=row[1] )
    )
    { //������� ��������� ������
      Text+="}";
      ObjType = "";
      ObjName = "";

      //������� �������� �������
      int p;
      while(bool(p=Text.Pos("\r"))) Text=Text.Delete(p,1);
      //�������� ������
      for(int i=0; StrCode[i].Code; i++) //���� �� ������������ �������
      { if(!StrCode[i].Str || !StrCode[i].Str[0]) continue;
        int l = strlen(StrCode[i].Str) - 1;
        while(bool(p=Text.Pos(StrCode[i].Str)))
        { Text[p]=StrCode[i].Code;
          Text=Text.Delete(p+1,l);
        }
      }

      //������� ������
      SQL ="INSERT INTO cross_entries SET cluster_min = ";
      SQL+=ClusterMin;
      SQL+=", cluster_max = ";
      SQL+=ClusterMax;
      SQL+=", code ='";
      SQL+=Text + "', len = ";
      SQL+=Text.Length()+3;

      if(mysql_query(&DB,SQL.c_str()))
      { Status->Text=mysql_error(&DB);
        ErrorMake=true;
        return;
      }
      ProgressBar->Position++;
      Application->ProcessMessages();
      //������ �����
      Text="";
    }

    if(!IsRow) break; //������ �����������

    //������� ��������
    strClust0=row[0];
    strClust1=row[1];
    ClusterMin=strClust0.ToInt();
    ClusterMax=strClust1.ToInt();
    if(ClusterMin>ClusterMax)
    { //������ �������� �������
      int X = ClusterMin;
      ClusterMin = ClusterMax;
      ClusterMax = X;
    }
    //��������� ����� ������
    if(ObjType!=row[2] || ObjName!=row[3])
    { //������� ���������� ������
      if(!Text.IsEmpty()) Text+="}\n";
      //�������� ��� � ���
      ObjType = row[2];
      ObjName = row[3];
      //�����������
      if     (ObjType=="class")  Text+="@";
      else if(ObjType=="word")   Text+="$";
      else if(ObjType!="object")
      { Text+=ObjType;
        Text+=" ";
      }
      //��������� ����
      Text+=ObjName+"{\n";
    }
    //������ ������������
    if(strcmp(row[4],"syn")==0)
    { //������� �������
      Text+="synonym = $";
      Text+=row[5];
    }
    else if(strcmp(row[4],"atr")==0)
    { //������� �������
      Text+=row[5];
    }
    else if(strcmp(row[4],"prp")==0)
    { //������� ��������
      Text+="property ";
      Text+=row[5];
      Text+="=";
      Text+=row[6];
    }
    else if(strcmp(row[4],"tmp")==0)
    { //������� ��������
      Text+="template ";
      Text+=row[5];
      Text+="=";
      Text+=row[6];
    }
    else
    { //������� ������
      Text+=row[4];
      Text+=row[5];
      Text+=row[4];
      Text+="\n";
      Text+=row[6];
    }
    Text+="\n";
  }
  mysql_free_result(Result);
  //--------------------------------------------------
  if(!ErrorMake) Status->Text="������� ��������� ���������� �������";
  ProgressBar->Position+=100;
  Application->ProcessMessages();
  //--------------------------------------------------
}
//==============================================================================
AnsiString Report;
bool __fastcall MakeEndsFile(char*FileExt)
{ //-------------------------------------------
  //��������� ����
  AnsiString FileName = "test_ends.";
  FileName+=FileExt;
  MForm->Status->Text=L"��������� ����...";
  int handle=open(FileName.c_str(), O_RDONLY|O_BINARY);
  if(handle==-1)
  { MForm->Status->Text=L"���� �� ������!";
    return true;
  }
  long size=filelength(handle);
  if(size<0||size>1000000)
  { MForm->Status->Text=L"�� ������� ���������� ������ �����!";
    return true;
  }
  char*buf=new char[size+1];
  if(read(handle,buf,size)==-1)
  { delete buf;
    MForm->Status->Text=L"������ ����� ����������!";
    return true;
  }
  buf[size]=0;  //null terminator
  close(handle);
  TStringList*EndsList = new TStringList;
  EndsList->Text = buf;
  close(handle);
  delete buf;
  //----------------------------------------------------------
  //�������� ������ ����� � �������
  struct
  { AnsiString DelEnd, AddEnd;   //��������� � ������������ ���������
  }EndsArray[1000];
  int EndCount=0;
  for(int i=0; i<EndsList->Count; i++)
  { AnsiString DelEnd, AddEnd;
    int p=EndsList->Strings[i].Pos("\t"); //����������� ������
    if(p)
    { DelEnd = EndsList->Strings[i].SubString(1,p-1);
      AddEnd = EndsList->Strings[i].SubString(p+1,1000);
    }
    else DelEnd = EndsList->Strings[i];

    int DelLen = DelEnd.Length();
    if(DelLen==0 || DelEnd[1]=='/' || DelEnd[1]=='=') continue;

    EndsArray[EndCount].DelEnd = DelEnd;
    EndsArray[EndCount].AddEnd = AddEnd;
    EndCount++;
  }
  delete EndsList;
  //----------------------------------------------------------
  //����� ������ � ����
  FileName = "StrArray.";
  FileName+=FileExt;
  int hStr=open(FileName.c_str(),O_WRONLY|O_CREAT|O_TRUNC|O_BINARY|O_APPEND,
                          S_IREAD|S_IWRITE );
  if(hStr==-1)
  { MForm->Status->Text="�� ������� ������� ���� StrArray!";
    return true;
  }
  for(int i=0; i<EndCount; i++)
  { int DelL = EndsArray[i].DelEnd.Length();
    int AddL = EndsArray[i].AddEnd.Length();
    //����� DelEnd
    write(hStr,EndsArray[i].DelEnd.c_str(),DelL);
    write(hStr,"\0",1); //����� ������
    //����� AddEnd
    write(hStr,EndsArray[i].AddEnd.c_str(),AddL);
    write(hStr,"\0",1); //����� ������
  }
  write(hStr,"\0",1); //����������� ����
  close(hStr);
  //----------------------------------------------------------
  //������� Report
  Report += " ����� - ";
  Report += EndCount;
  Report += "\r\n";
  for(int i=0; i<EndCount; i++)
    Report+=EndsArray[i].DelEnd + "; " + EndsArray[i].AddEnd + "\r\n";

  return false;
}

/*
bool __fastcall MakeEndsFile(char*FileExt)
{ //-------------------------------------------
  //��������� ����
  AnsiString FileName = "test_ends.";
  FileName+=FileExt;
  MForm->Status->Text=L"��������� ����...";
  int handle=open(FileName.c_str(), O_RDONLY|O_BINARY);
  if(handle==-1)
  { MForm->Status->Text=L"���� �� ������!";
    return true;
  }
  long size=filelength(handle);
  if(size<0||size>1000000)
  { MForm->Status->Text=L"�� ������� ���������� ������ �����!";
    return true;
  }
  char*buf=new char[size+1];
  if(read(handle,buf,size)==-1)
  { delete buf;
    MForm->Status->Text=L"������ ����� ����������!";
    return true;
  }
  buf[size]=0;  //null terminator
  close(handle);
  TStringList*EndsList = new TStringList;
  EndsList->Text = buf;
  close(handle);
  delete buf;
  //----------------------------------------------------------
  //�������� ������ ����� � �������
  struct
  { AnsiString DelEnd, AddEnd;   //��������� � ������������ ���������
    char Ret;                    //����� ������������ ��������
    AnsiString TreeEnd, UniqEnd; //��������� � ���������� ���������
    unsigned char Code;
  }EndsArray[1000];
  int EndCount=0;
  for(int i=0; i<EndsList->Count; i++)
  { AnsiString DelEnd, AddEnd;
    int p=EndsList->Strings[i].Pos("\t"); //����������� ������
    if(p)
    { DelEnd = EndsList->Strings[i].SubString(1,p-1);
      AddEnd = EndsList->Strings[i].SubString(p+1,1000);
    }
    else DelEnd = EndsList->Strings[i];

    int DelLen = DelEnd.Length();
    if(DelLen==0 || DelEnd[1]=='/' || DelEnd[1]=='=') continue;

    EndsArray[EndCount].Ret = 0;
    for(int j=1; j<=DelEnd.Length() && j<=AddEnd.Length(); j++)
      if(DelEnd[j]==AddEnd[j])
        EndsArray[EndCount].Ret++;
      else break;

    EndsArray[EndCount].DelEnd = DelEnd;
    EndsArray[EndCount].AddEnd = AddEnd.SubString(1+EndsArray[EndCount].Ret,100);
    EndCount++;
  }
  delete EndsList;
  //----------------------------------------------------------
  //�������� ��������� ��������� �� ��������� � ����������
  for(int i=0; i<EndCount; i++)
  { int Tree=0;
    for(int j=0; j<EndCount; j++)
    { if(i==j) continue;
      int l=0;
      int k=EndsArray[i].DelEnd.Length();
      int m=EndsArray[j].DelEnd.Length();
      while(k>0 && m>0)
        if(EndsArray[i].DelEnd[k]==EndsArray[j].DelEnd[m])
        { l++; k--; m--; }
        else break;
      if(Tree<l) Tree=l;
    }
    Tree++;

    int Uniq=EndsArray[i].DelEnd.Length() - Tree;
    EndsArray[i].UniqEnd = EndsArray[i].DelEnd.SubString(1,Uniq);
    EndsArray[i].TreeEnd = EndsArray[i].DelEnd.SubString(Uniq+1,100);
  }
  //----------------------------------------------------------
  //��������� ������� ����� � ��������� ����
  char UniqLen=0, AddLen=0;
  unsigned char CurCode=0;
  for(int i=0; i<EndCount; i++)
  { EndsArray[i].Code = 0xFF; //��� ����
    int UniqL = EndsArray[i].UniqEnd.Length();
    int AddL  = EndsArray[i].AddEnd.Length();
    if(UniqL || AddL)
    { //������ ������� � ����
      if(UniqLen<UniqL) UniqLen=UniqL;
      if(AddLen <AddL ) AddLen =AddL;
      //�������� ��� ������ ����������� �����
      for(int j=0; j<i; j++)
        if
        ( EndsArray[j].Code   !=0xFF &&
          EndsArray[i].UniqEnd==EndsArray[j].UniqEnd &&
          EndsArray[i].AddEnd ==EndsArray[j].AddEnd  &&
          EndsArray[i].Ret    ==EndsArray[j].Ret
        )
        { //�������� ��������� ���
          EndsArray[i].Code = EndsArray[j].Code;
          break;
        }

      if(EndsArray[i].Code==0xFF)
      { //�������� ������ ����� ���
        EndsArray[i].Code=CurCode++;
        if(CurCode>64)
        { MForm->Status->Text=L"���������� ����� ��������� 64!";
          return true;
        }
      }
    }
  }
  //----------------------------------------------------------
  //����� ������ � ����
  FileName = "StrArray.";
  FileName+=FileExt;
  int hStr=open(FileName.c_str(),O_WRONLY|O_CREAT|O_TRUNC|O_BINARY|O_APPEND,
                          S_IREAD|S_IWRITE );
  if(hStr==-1)
  { MForm->Status->Text="�� ������� ������� ���� StrArray!";
    return true;
  }
  CurCode=0;
  write(hStr,&UniqLen,1); //����� ������ ���������� �����
  write(hStr,&AddLen,1);  //����� ������ ����������� �����
  for(int i=0; i<EndCount; i++)
  { if(EndsArray[i].Code!=CurCode) continue;
    CurCode++;
    int UniqL = EndsArray[i].UniqEnd.Length();
    int AddL  = EndsArray[i].AddEnd.Length();
    //UniqEnd ����� � ����������
    for(int s=UniqL; s>0; s--)
      write(hStr,&EndsArray[i].UniqEnd[s],1); //����� UniqEnd
    for(int s=UniqL; s<UniqLen; s++)
      write(hStr,"\0",1); //�������� ������
    //����� �����
    write(hStr,&EndsArray[i].Ret,1);
    //����� AddEnd
    write(hStr,EndsArray[i].AddEnd.c_str(),AddL);
    for(int s=AddL; s<AddLen; s++)
      write(hStr,"\0",1); //�������� ������
  }
  close(hStr);
  //----------------------------------------------------------
  //������ �������
  struct
  { AnsiString Addr;       //����� �������
    unsigned char Tab[32]; //�������
  }Tabs[128];              //��������� ��������� ����� �� ������ 128 ������
  int TabCount=1;
  //�������� �������
  for(int i=0; i<128; i++) memset(Tabs[i].Tab, 0, 32);
  //��������� �������
  for(int i=0; i<EndCount; i++)
  { int TreeL = EndsArray[i].TreeEnd.Length();
    int t0=-1;
    for(int s=1; s<=TreeL; s++)
    { //����� �������
      int t=0;
      if(s<TreeL)
      { //������� �����
        AnsiString Addr = EndsArray[i].TreeEnd.SubString(s+1,100);
        for(int j=1; j<TabCount; j++)
          if(Tabs[j].Addr==Addr){ t=j; break; } //����� �������
        if(!t)
        { //��������� �������
          t=TabCount++;
          if(TabCount>128)
          { MForm->Status->Text=L"���������� ������ ��������� 128!";
            return true;
          }
          Tabs[t].Addr=Addr;
        }
      }
      //������ ������ �������
      int a=EndsArray[i].TreeEnd[s]-'�';
      if(a<0 || a>31)
      { MForm->Status->Text=L"������������ ������ � ���������!";
        return true;
      }
      //�������� ������ �������
      if(t0<0)
        if(Tabs[t].Tab[a]==0)
          Tabs[t].Tab[a] = 'x'; //����� �����
        else
          Tabs[t].Tab[a] = 'X'; //�������� �����...
      else Tabs[t].Tab[a] = t0;

      t0=t;
    }
  }
  //----------------------------------------------------------
  //������� �������
  FileName = "TreeTabs.";
  FileName+=FileExt;
  int hTab=open(FileName.c_str(),O_WRONLY|O_CREAT|O_TRUNC|O_BINARY|O_APPEND,
                          S_IREAD|S_IWRITE );
  if(hTab==-1)
  { MForm->Status->Text="�� ������� ������� ���� TreeTabs!";
    return true;
  }

  for(int t=0; t<TabCount; t++) write(hTab, Tabs[t].Tab, 32);

  close(hTab);
  //----------------------------------------------------------
  //������� Report
  Report += " ����� - ";
  Report += CurCode;
  Report += ", ������ - ";
  Report += TabCount;
  Report += "\r\n";
  for(int i=0; i<EndCount; i++)
  { Report+=EndsArray[i].UniqEnd+"|"+EndsArray[i].TreeEnd+"; "
        + (int)EndsArray[i].Ret + "; " + EndsArray[i].AddEnd + "\r\n";
  }

  return false;
}
*/
void __fastcall TMForm::MakeEnds(TObject*Sender)
{ //���������� �����
  Report = "verb:";
  ErrorMake = MakeEndsFile("verb");
  if(ErrorMake) return;
  Report+= "\r\n\r\nnoun:";
  ErrorMake = MakeEndsFile("noun");
  if(ErrorMake) return;
  Status->Text="����� ��������� ���������";
  //������� Report
  InputTxt(Report,"������ ������ ���������","�������");
}
//==============================================================================
void __fastcall TMForm::MakeHashTab(TObject*Sender)
{ Status->Text="�������� Hash-�������";
  //--------------------------------------------------------
  //������� ���������� Hash-�����
  if(mysql_query(&DB,
    "SELECT COUNT(DISTINCT hash16) FROM form "
    "JOIN word ON word.id = word_id "
    "WHERE status >=100" )
  )
  { Status->Text=mysql_error(&DB);
    ErrorMake=true;
    return;
  }
  MYSQL_RES*Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    ErrorMake=true;
    return;
  }
  MYSQL_ROW row;
  if(bool(row = mysql_fetch_row(Result)))
    Hdr.HashCount = AnsiString(row[0]).ToInt();
  else
  { Status->Text="�� ������� �������� ���������� Hash-�����";
    ErrorMake=true;
    return;
  }
  mysql_free_result(Result);
  //--------------------------------------------------------
  //���������� �����
  unsigned short*Buf = new unsigned short[Hdr.HashCount*4];
  memset(Buf, 0xFF, sizeof(unsigned short)*Hdr.HashCount*4);
  //--------------------------------------------------------
  //�������� �����
  if(mysql_query(&DB,
    "SELECT hash16, addr FROM form "
    "JOIN word ON word.id = word_id "
    "JOIN entries ON entries.cluster = word.cluster "
    "WHERE status >=100 "
    "GROUP BY hash16, addr "
    "ORDER BY hash16, addr" )
  )
  { Status->Text=mysql_error(&DB);
    ErrorMake=true;
    return;
  }
  Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    ErrorMake=true;
    return;
  }
  int Hash, Addr, i=0, j;
  while(bool(row = mysql_fetch_row(Result)))
  { if(!row[0] || !row[1])
    { Status->Text="�� ��������� Hash ��� Addr";
      ErrorMake=true;
      return;
    }
    Hash = AnsiString(row[0]).ToInt();
    Addr = AnsiString(row[1]).ToInt();
    if(Buf[i]!=Hash)
    { //����� ���
      j = 1;
      if(Buf[i]!=0xFFFF) i+=4;
      if(i>=Hdr.HashCount*4)
      { Status->Text="������������ Hash-�������";
        ErrorMake=true;
        return;
      }
      Buf[i] = Hash;
    }
    else
    { //����������� ������
      j++;
      if(j>3)
      { Status->Text="������������ ������ Hash-�������";
        ErrorMake=true;
        return;
      }
    }
    Buf[i+j] = Addr;
  }
  mysql_free_result(Result);
  //--------------------------------------------------------
  //������� ������� � ����
  int hTab=open("hash_tab.tmp", O_WRONLY|O_CREAT|O_TRUNC|O_BINARY|O_APPEND,
                                S_IREAD|S_IWRITE );
  if(hTab==-1)
  { Status->Text="�� ������� ������� ���� hash_tab.tmp";
    ErrorMake=true;
    return;
  }

  write(hTab,Buf,sizeof(unsigned short)*Hdr.HashCount*4);

  close(hTab);
  //--------------------------------------------------------
  delete Buf;
  Status->Text="Hash-������� �������";
}
//==============================================================================

