#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>
#pragma hdrstop
#include "MainForm.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
//������� ��������
void __fastcall TMForm::Clustering(TObject*Sender)
{ Status->Text="���������� �������� ����";
  Application->ProcessMessages();
  ProgressBar->Max = 4;
  ProgressBar->Position=0;
  //������� ��� ��������
  AnsiString SQL="UPDATE word SET cluster = NULL";
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  ProgressBar->Position++;

  //��������� � ������� ���������� ���� ��� ������ ����������
  if(mysql_query(&DB,
    "UPDATE form, "
    "( SELECT search_form, COUNT(DISTINCT word_id) AS cnt"
    "  FROM form"
    "  JOIN word ON form.word_id = word.id"
    "  WHERE form.search_form <> '-' AND word.status >= 100"
    "  GROUP BY search_form"
    ") AS sel "
    "SET word_cnt = cnt "
    "WHERE form.search_form = sel.search_form"
  ))
  { Status->Text=mysql_error(&DB);
    return;
  }
  ProgressBar->Position++;


  //����������� ����� �� ���������
  //--------------------------------------------------------
  //������ ���������� ������������� ��������� ������
  SQL="SELECT DISTINCT search_form FROM form "
      "WHERE form.word_cnt > 1 ";
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
  while(bool(row=mysql_fetch_row(Result))) //���� �� �����������
  { //--------------------------------------------------------
    //������ ����� ���������� ��� ����������
    SQL="SELECT DISTINCT word.id, cluster, cluster IS NULL AS isnull "
        "FROM form "
        "LEFT JOIN word ON form.word_id = word.id "
        "WHERE status>=100 AND form.search_form = '";
    SQL+=row[0];
    SQL+="' ORDER BY isnull, cluster, id";
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    MYSQL_RES*Words=mysql_store_result(&DB);
    if(!Words)
    { Status->Text=mysql_error(&DB);
      return;
    }
    AnsiString Cluster;
    for(int r=0; bool(row=mysql_fetch_row(Words)); r++) //���� �� ������
    { if(row[1]) //���� �������
      { if(r)
        { if(Cluster!=row[1]) //�������� ����������
          { //������� ��������

            //==============================================
            // ��������
            Status->Text="��������� ��������� ���������!!!";
            return;
            //==============================================

          }
        }
        else Cluster=row[1]; //����� ������� ������ ������
      }
      else //��� ��������
      { if(r==0) Cluster=row[0]; //id ������� ����� ���������� ���������
        SQL="UPDATE word SET cluster = ";
        SQL+=Cluster;
        SQL+=" WHERE id = ";
        SQL+=row[0];
        if(mysql_query(&DB,SQL.c_str()))
        { Status->Text=mysql_error(&DB);
          return;
        }
      }
    }
    mysql_free_result(Words);
    //--------------------------------------------------------
  }
  mysql_free_result(Result);
  ProgressBar->Position++;
  //--------------------------------------------------------
  //����� �� �������� � ������� �������� ������� �� ������ �����
  if(mysql_query(&DB,
    "UPDATE word SET cluster = id "
    "WHERE cluster IS NULL AND status >= 100"
  ))
  { Status->Text=mysql_error(&DB);
    return;
  }
  //--------------------------------------------------------
  Status->Text="Ok, �������� ���������";
  ProgressBar->Position=0;
}
//==============================================================================
void __fastcall TMForm::SetAccentuations(TObject*Sender)
{ //������ ��������
  //����� ��������
  AnsiString SQL = "UPDATE form SET accentuation = 0";
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  //����������� ��������
  SQL="UPDATE form, form AS form2 "
      "SET form.accentuation = 1 "
      "WHERE form.search_form = form2.search_form AND form.form <> form2.form";
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  Status->Text="Ok, �������� �����������";
}

//==============================================================================
char*MrSrPds[12] = //������� �������������� ������� ��� ���. �� � ��
{ "����", "����",
  "����", "����",
  "����", "����",
  "����", "����",
  "����", "����",
  "����", "����"
};
char*ZHrPds[13] = //������� �������������� ������� ��� ���. ��
{ "����", "����",
  "����", "����",
  "����", "����",
  "����", "����",
  "����", "������", "����",
  "����", "����"
};
char*AdjPds[37] = //������� �������������� ������� ��� ��������������
{ "������",     "������",     "������",     "����",
  "������",     "������",     "������",     "����",
  "������",     "������",     "������",     "����",
  "��������",   "������",     "������",     "������",
  "��������",   "del",        "del_2",      "������",
  "������",     "������",
                "��������",   "������",     "����",
  "������",     "������",     "������",     "����",
  "����������", "����������", "����������", "������",
  "del_3",      "del_4",      "del_5",      "del_6"
};
void __fastcall TMForm::WordKeyProcessing(TObject*Sender)
{ Status->Text="";
  //������� ��������� ����������
  //==================================================
  //��������������� �������� � �������� ����
  AnsiString SQL=
    "SELECT word.id, name, COUNT(form) AS form_cnt "
    "FROM form LEFT JOIN word "
    "ON word.id = form.word_id "
    "WHERE word.status = 20 " //���������� ����� ��������� �����
    "GROUP BY id, name "
    "HAVING form_cnt = 12";

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
  while(bool(row=mysql_fetch_row(Result)))
  { Status->Text=row[1];
    Application->ProcessMessages(); //���� �� ��������

    AnsiString Key, Word = row[1];
    switch(Word[Word.Length()])
    { case '�': //���������� ���������� ���
        Key="";
        break;
      case '�': case '�': case '�': case '�': //������� ���
        Key="��";
        break;
      default: Key="��";  //������� ���
        break;
    }
    //�������� �����
    SQL="UPDATE word SET fkey = '";
    SQL+=Key+"', defkey = '����', status = 25 ";
    SQL+=" WHERE id = ";
    SQL+=row[0];
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    //�������� �����
    for(int i=1; i<=12; i++)
    { SQL="UPDATE form SET fkey = '";
      SQL+=MrSrPds[i-1];
      SQL+="' WHERE word_id = ";
      SQL+=row[0];
      SQL+=" AND fkey = '";
      SQL+=i;
      SQL+="'";
      if(mysql_query(&DB,SQL.c_str()))
      { Status->Text=mysql_error(&DB);
        return;
      }
    }
  }
  mysql_free_result(Result);
  //==================================================
  //��������������� �������� ����
  SQL=
    "SELECT word.id, word.name "
    "FROM form LEFT JOIN word "
    "ON word.id = form.word_id "
    "WHERE form.fkey = '10' AND RIGHT(form,1) = '�' AND word.status = 20";

  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  while(bool(row=mysql_fetch_row(Result)))
  { Status->Text=row[1];
    Application->ProcessMessages(); //���� �� ��������
    //�������� �����
    SQL="UPDATE word SET fkey = '��', defkey = '����', status = 25 "
        "WHERE id = ";
    SQL+=row[0];
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    //�������� �����
    for(int i=1; i<=13; i++)
    { SQL="UPDATE form SET fkey = '";
      SQL+=ZHrPds[i-1];
      SQL+="' WHERE word_id = ";
      SQL+=row[0];
      SQL+=" AND fkey = '";
      SQL+=i;
      SQL+="'";
      if(mysql_query(&DB,SQL.c_str()))
      { Status->Text=mysql_error(&DB);
        return;
      }
    }
  }
  mysql_free_result(Result);

  //==================================================
  //��������������
  SQL=
    "SELECT word.id, name, COUNT(form) AS form_cnt "
    "FROM form LEFT JOIN word "
    "ON word.id = form.word_id "
    "WHERE word.status = 20  AND (type = 'adj' OR type = 'part')"
    "GROUP BY id, name "
    "HAVING form_cnt = 29 OR form_cnt = 33 OR form_cnt = 37";

  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  while(bool(row=mysql_fetch_row(Result)))
  { Status->Text=row[1];
    Application->ProcessMessages(); //���� �� ��������
    //�������� �����
    SQL="UPDATE word SET fkey = '', defkey = '��������', status = 25 "
        "WHERE id = ";
    SQL+=row[0];
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    //�������� �����
    for(int i=1; i<=37; i++)
    { SQL="UPDATE form SET fkey = '";
      SQL+=AdjPds[i-1];
      SQL+="' WHERE word_id = ";
      SQL+=row[0];
      SQL+=" AND fkey = '";
      SQL+=i;
      SQL+="'";
      if(mysql_query(&DB,SQL.c_str()))
      { Status->Text=mysql_error(&DB);
        return;
      }
    }
  }
  mysql_free_result(Result);

  Status->Text="Ok, ��������� ���� ���������";
}

//---------------------------------------------------------------------------
char*WerbSpr[14][4] = //������� �������������� ��������� ��� ��������
{ "����",     "����",     "����",     "����",   //���������
  "��1�����", "��1�����", "��1�����", "��1�����",
  "��1���",   "��1���",   "��1���",   "��1���",
  "��2�����", "��2�����", "��2�����", "��2�����",
  "��2���",   "��2���",   "��2���",   "��2���",
  "��3�����", "��3�����", "��3�����", "��3�����",
  "��3���",   "��3���",   "��3���",   "��3���",

  "del",      "��������", "del",      "��������",
  "��������", "��������", "��������", "��������",
  "��������", "��������", "��������", "��������",
  "��������", "����",     "��������", "����",

  "����",     "��2�����", "����",     "��2�����",

  "��2�����", "��2�����", "��2�����", "��2�����",
  "��2�����", "del",      "��2�����", "del",
};

void __fastcall TMForm::VerbKeyProcessing(TObject*Sender)
{ //������ ����� � �������
  //������������� ����������� ������������.
  Status->Text="";

  AnsiString SQL=
    "SELECT word.id, name, COUNT(form) AS form_cnt "
    "FROM form JOIN word "
    "ON word.id = form.word_id "
    "WHERE word.status = 20  AND type = 'verb' "
    "GROUP BY id, name "
    "HAVING form_cnt = 13 OR form_cnt = 14";

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
  AnsiString Key, DefKey;
  StopFlag=false;
  while(!StopFlag && bool(row=mysql_fetch_row(Result)))
  { Status->Text=row[1];
    int col=(strcmp(row[2],"14")==0)?0:1;
    //����������� �������������
    AnsiString Q="����� ";
    Q+=row[1];
    Q+=" - ��� ���������?";
    switch
    ( Application->MessageBoxA( Q.c_str(),"������������� �������",
                                MB_YESNOCANCEL|MB_ICONQUESTION))
    { case IDYES: //�������������
        Key = "��";
        DefKey = "����3���";
        break;
      case IDNO: //�����������
        Key = "��";
        DefKey = "����3���";
        col+=2;
        break;
      default: StopFlag=true; continue; //������������� ���������
    }
    //�������� �����
    SQL= "UPDATE word SET status = 25, fkey = '";
    SQL+=Key+"', defkey = '";
    SQL+=DefKey + "' WHERE id = ";
    SQL+=row[0];
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    //�������� �����
    for(int i=1; i<=14; i++)
    { SQL="UPDATE form SET fkey = '";
      SQL+=WerbSpr[i-1][col];
      SQL+="' WHERE word_id = ";
      SQL+=row[0];
      SQL+=" AND fkey = '";
      SQL+=i;
      SQL+="'";
      if(mysql_query(&DB,SQL.c_str()))
      { Status->Text=mysql_error(&DB);
        return;
      }
    }
  }
  mysql_free_result(Result);
  //������ ������ �����
  if(mysql_query(&DB,"DELETE FROM form WHERE fkey = 'del'"))
  { Status->Text=mysql_error(&DB);
    return;
  }


  Status->Text="Ok, ��������� �������� ���������";
/* � ���� �������� ����
Beep(247, 500);
Beep(417, 500);
Beep(417, 500);
Beep(370, 500);
Beep(417, 500);
Beep(329, 500);
Beep(247, 500);
Beep(247, 500);
*/
}
//---------------------------------------------------------------------------
void __fastcall TMForm::AddVerbKeys(TObject*Sender)
{ //��������� ��������� ���������� ���� � �������� �����
  Status->Text="";
  if(WordId.IsEmpty()||CurrWord.IsEmpty())
  { Status->Text="�������� ������!";
    return;
  }
  int Type = 0; //�������������
  if(Sender==Verb2Menu) Type=2; //�����������

  for(int i=0; i<14; i++)
  { if(strcmp(WerbSpr[i][Type],"del")==0) continue;

    AnsiString SQL="INSERT INTO form SET word_id = ";
    SQL+=WordId + ", fkey = '";
    SQL+=WerbSpr[i][Type];
    SQL+="', form = '";
    SQL+=CurrWord;
    SQL+="', search_form = '";
    SQL+=CurrWord;
    SQL+="'";

    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
  }

  Status->Text="Ok, ����� �������� ���������.";
}
//---------------------------------------------------------------------------
void __fastcall TMForm::AnalysisEndsFile(TObject*Sender)
{ //����������� ���� � �����������
  char*FileName;
//if(Sender==(TObject*)&NVerbEnds)
    FileName = "test_ends.verb";
//else if(Sender==(TObject*)&NWordEnds)
//  FileName = "test_ends.noun";
//else
//{ Status->Text=L"��� ����� �� ��������!";
//  return;
//}
  //------------------------------------------
  Status->Text=L"��������� ����...";
  int handle=open(FileName, O_RDONLY|O_BINARY);
  if(handle==-1)
  { Status->Text=L"���� �� ������!";
    return;
  }
  long size=filelength(handle);
  if(size<0||size>1000000)
  { Status->Text=L"�� ������� ���������� ������ �����!";
    return;
  }
  char*buf=new char[size+1];
  if(read(handle,buf,size)==-1)
  { delete buf;
    Status->Text=L"������ ����� ����������!";
    return;
  }
  buf[size]=0;  //null terminator
  TStringList*EndsList = new TStringList;
  EndsList->Text = buf;
  close(handle);
  delete buf;
  //------------------------------------------
  struct
  { AnsiString Str;
    int Count;
  }Index[1000];
  //------------------------------------------
  Status->Text=L"����������� ����...";
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

    for(int i=DelLen; i>0; i--)
    {
      AnsiString Result = DelEnd.SubString(i,100);
      //�������� ������
      for(int j=0; j<1000; j++)
        if(Index[j].Str.IsEmpty())
        { Index[j].Str = Result;
          Index[j].Count = 1;
          break;
        }
        else if(Index[j].Str==Result)
        { Index[j].Count++;
          break;
        }
    }
  }
  delete EndsList;
  //------------------------------------------
  Status->Text=L"������� ���������...";
  AnsiString Text;
  for(int j=0; j<1000; j++)
  { if(Index[j].Str.IsEmpty()) break;
    if(Index[j].Count==1) continue;
    Text+=Index[j].Str+"\t";
    Text+=Index[j].Count;
    Text+="\r\n";
  }
  InputTxt(Text,"������ ����� ���������","�������");
  Status->Text=L"Ok!";
}
//---------------------------------------------------------------------------

