#include <vcl.h>
#pragma hdrstop
#include "MainForm.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
#pragma comment(lib,"..\MySQL\libMySql.lib")
//---------------------------------------------------------------------------
TMForm*MForm;
//---------------------------------------------------------------------------
//MySQL
static char*server_args[]=
{ "this_program", //this string is not used
  "--datadir=.",
  "--key_buffer_size=32M"
};
static char*server_groups[] =
{ "embedded",
  "server",
  "this_program_SERVER",
  (char*)NULL
};
MYSQL DB;
//---------------------------------------------------------------------------
__fastcall TMForm::TMForm(TComponent*Owner):TForm(Owner)
{ //�������������� MySQL library
  if(mysql_library_init(3,server_args,server_groups))
  { Status->Text="mysql_library_init() error!";
    return;
  }
  //�������������� MySQL
  if(!mysql_init(&DB))
  { Status->Text="mysql_init() error!";
    return;
  }
  //������ �������
  if(!mysql_real_connect(&DB,0,"root","root",0,0,0,0))
  { Status->Text=mysql_error(&DB);
    return;
  }
  //������������� ��������� �������
  if(mysql_query(&DB,"set names cp1251"))
  { Status->Text=mysql_error(&DB);
    return;
  }
  //------------------------------------------
  //�������� ���������� ����������
  CurrWord = "";
  WordListSQL = "WHERE word.name >= '$' AND word.status>0";
  //������� ������ ���
  if(RefreshBasesList()) Status->Text="Ok!";
  RefreshCaption();
  //------------------------------------------
}
//---------------------------------------------------------------------------
void __fastcall TMForm::FormDestroy(TObject *Sender)
{ //��������� MySQL
  mysql_close(&DB);
  mysql_library_end();
}
//---------------------------------------------------------------------------
void __fastcall TMForm::RefreshCaption(void)
{ //��������� ��������� ����
  AnsiString Cap = "mySQL TOM2: ";
  if(!CurrDB.IsEmpty())
  { Cap+=CurrDB;
    Cap+="     ";
    if(!CurrWord.IsEmpty())
    { Cap+=CurrWord;
      if(!CurrMeaning.IsEmpty())
      { Cap+="     ";
        Cap+=MeaningsTypes[CurrMeaType];
        Cap+=" ";
        Cap+=CurrMeaning;
        if(!CurrSynonym.IsEmpty())
        { Cap+="     ";
          Cap+=CurrSynonym;
        }
/*
        if(!CurrObject.IsEmpty())
        { Cap+="     ";
          Cap+=CurrObject;
          Cap+=".";
          Cap+=CurrProperty;
        }
*/
      }
    }
  }
  Caption = Cap;
}
//---------------------------------------------------------------------------
bool __fastcall TMForm::RefreshBasesList(void)
{
  //��������� ����������� ��� ���������� ������!

  //������� ������ ���
  if(mysql_query(&DB,"show databases"))
  { Status->Text=mysql_error(&DB);
    return false;
  }
  //������� ������ ���
  MYSQL_RES*Result=mysql_use_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return false;
  }
  bool Selected=false;
  MYSQL_ROW row;
  for(int r=0; bool(row=mysql_fetch_row(Result)); r++)
  { if
    ( strcmp(row[0],"information_schema")==0 ||
      strcmp(row[0],"mysql")==0
    ) continue; //��������� ����

    if(strcmp(row[0],"tomdic")==0 && CurrDB.IsEmpty())
    { CurrDB = row[0];
      Selected=true;
    }

    DBList->Items->Add(row[0]);
  }
  mysql_free_result(Result);

  //��������� ���� �� ���������
  if(Selected)
  { if(mysql_query(&DB,"use tomdic"))
    { Status->Text=mysql_error(&DB);
      return false;
    }
    DBList->Text="tomdic";
    return RefreshWordList();
  }
  else return true;
}
//---------------------------------------------------------------------------
bool __fastcall TMForm::RefreshWordList(void)
{ //�������� ������ ����
  AnsiString SQL =
  "SELECT word.name, meaning.type, meaning.name, "        //0, 1, 2
  "synword.name, m2.name, synonym.add_name, s2.add_name " //3, 4, 5, 6
  "FROM word "
  "LEFT JOIN meaning ON meaning.word_id = word.id "
  "LEFT JOIN synonym ON synonym.object_id = meaning.id "
  "LEFT JOIN word AS synword ON synword.id = synonym.word_id "
  "LEFT JOIN synonym AS s2 ON s2.word_id = word.id "
  "LEFT JOIN meaning AS m2 ON m2.id = s2.object_id ";

  if(!FindWord->Text.IsEmpty() && FindWord->Text[1]=='#')
    SQL+="AND LOCATE('$', m2.hashtag)>0 "
         "WHERE LOCATE('$', meaning.hashtag)>0";
  else SQL+=WordListSQL;

  SQL+=" ORDER BY word.name, meaning.name, synword.name";

  if(FindWord->Text!="$")
  { AnsiString SearchText = FindWord->Text.IsEmpty()?"�":FindWord->Text.c_str();
    int p=SQL.Pos("'$'");
    while(p)
    { SQL=SQL.Delete(p+1,1);
      SQL.Insert(SearchText,p+1);
      p=SQL.Pos("'$'");
    }
  }

  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return false;
  }
  MYSQL_RES*Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return false;
  }
  //������� ������
  WordTree->Items->BeginUpdate();
  WordTree->Items->Clear();
  AnsiString W, M, T, S;
  TTreeNode *WNode=0, *MNode, *SNode;
  MYSQL_ROW row;
  for(int r=0; r<300 && bool(row=mysql_fetch_row(Result)); r++)
  { if(!row[0]||!row[0][0]) continue; //��� �����?!
    if(W!=row[0])
    { //�������� ������ �����
      if(WNode && WNode->Text==CurrWord)
      { if(CurrMeaning.IsEmpty())
          WordTree->Select(WNode,TShiftState());
        WNode->Expanded = true;
        //�������� ������ ��������
        if(MNode && MNode->Text==CurrMeaning)
          MNode->Expanded = true;
      }
      //����� �����
      W=row[0]; M=""; S="";
      WNode=WordTree->Items->Add(0, row[0]);
      WNode->ImageIndex    = 11; //W
      WNode->SelectedIndex = 11; //W

      if(CurrWord.IsEmpty())
      { CurrWord=row[0];
        MeaningPanel->Visible = false;
        WordPanel->Visible = true;
        WordPanel->Align = alClient;
        RefreshCurrWord();
      }
    }
    if(row[1]&&row[2]&&row[2][0])
    { //���� ��������
      if(M!=row[2] || T!=row[1])
      { //����� ��������
        T=row[1];
        M=row[2];
        MNode=ClassTree->Items->AddChild(WNode,row[2]);
        for(int i=0; MeaningsTypes[i]; i++)
          if(strcmp(MeaningsTypes[i], row[1])==0)
          { MNode->ImageIndex = i;
            MNode->SelectedIndex = i;
            break;
          }
      }
      if(row[3] && row[3][0])
      { //���� �������
        SNode=ClassTree->Items->AddChild(MNode,row[3]);
        SNode->ImageIndex    = (row[5][0]=='0')?12:13; // S|N
        SNode->SelectedIndex = (row[5][0]=='0')?12:13; // S|N
      }
    }
    if(row[4] && row[4][0] && S!=row[4])
    { //���� ���������� ��������
      S=row[4];
      SNode=ClassTree->Items->AddChild(WNode,row[4]);
      SNode->ImageIndex    = (row[6][0]=='0')?12:13; // S|N
      SNode->SelectedIndex = (row[6][0]=='0')?12:13; // S|N
    }
  }
  mysql_free_result(Result);
  WordTree->Items->EndUpdate();

//RefreshDBChart();
  RefreshCaption();

  return true;
}

//---------------------------------------------------------------------------
void __fastcall TMForm::SetWordList(TObject*Sender)
{ //�������� ������, ������� ����� ������
  AnsiString SQL=WordListSQL;
  AnsiString OldSQL=WordListSQL;
  if(InputTxt(SQL,"�������� ������ ��� ������ ����",0))
  { WordListSQL=SQL;
    if(RefreshWordList())
      Status->Text = "������� ����� ������ ����";
  }
}

void __fastcall TMForm::SetWordListAsList(TObject*Sender)
{ //������ ���� �� ��������
  WordListSQL = "WHERE word.name >= '$' AND word.status>0";
  if(RefreshWordList())
    Status->Text = "���������� ����� ���� �������";
}

void __fastcall TMForm::SetWordList4Meanings(TObject*Sender)
{ //����� ���� �� ���������� �� �������
  WordListSQL = "WHERE word.name >= '$' AND meaning.status > 0";
  if(RefreshWordList())
    Status->Text = "���������� ����� �������� �� �������";
}

void __fastcall TMForm::SetWordListAsFind(TObject*Sender)
{ //������� ���� � ������� � ����
  WordListSQL=
  "LEFT JOIN (SELECT word_id FROM association\r\n"
  "WHERE LOCATE('$', code)>0\r\n"
  "GROUP BY word_id\r\n"
  ") AS ass ON word.id = ass.word_id\r\n"

  "WHERE LOCATE('$',word.glb_code)>0 OR LOCATE('$',word.obj_code)>0\r\n"
  "OR LOCATE('$',meaning.code)>0 OR LOCATE('$',meaning.description)>0\r\n"
  "OR NOT ass.word_id IS NULL";
  if(RefreshWordList())
    Status->Text = "���������� ����� ���� �� ����";
}
//---------------------------------------------------------------------------

void __fastcall TMForm::SQLTabShEnter(TObject *Sender)
{ //������ �� SQL-tools ��������
  TableListBox->Items->Clear();
  //������� ������ ������
  if(mysql_query(&DB,"show tables"))
  { Status->Text=mysql_error(&DB);
    return;
  }
  //������� ������ ������
  MYSQL_RES*Result=mysql_use_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_ROW row;
  for(int r=0; bool(row=mysql_fetch_row(Result)); r++)
    TableListBox->Items->Add(row[0]);
  mysql_free_result(Result);

  Status->Text="Ok!";
}
//---------------------------------------------------------------------------
void __fastcall TMForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  StopFlag=true;
}
//---------------------------------------------------------------------------

void __fastcall TMForm::FindWordChange(TObject*Sender)
{
  RefreshWordList();
}
//---------------------------------------------------------------------------
void __fastcall TMForm::WordListRefreshClick(TObject *Sender)
{
  RefreshWordList();
}
//---------------------------------------------------------------------------

void __fastcall TMForm::WordTreeClick(TObject *Sender)
{ //��������� ����
  if(!WordTree->Selected) return;
  else if(!WordTree->Selected->Parent)
  { //���� �� �����
    CurrWord = WordTree->Selected->Text;
    CurrMeaning = "";
    CurrMeaType = -1;
    CurrSynonym = "";
    MeaningPanel->Visible = false;
    WordPanel->Visible = true;
    WordPanel->Align = alClient;
    RefreshCurrWord();
  }
  else
  { if(!WordTree->Selected->Parent->Parent)
    { //���� �� ��������
      CurrWord = WordTree->Selected->Parent->Text;
      CurrMeaning = WordTree->Selected->Text;
      CurrMeaType = WordTree->Selected->ImageIndex;
      CurrSynonym = "";
      //����� ��� ��������
    }
    else
    { //���� �� ��������
      CurrWord = WordTree->Selected->Parent->Parent->Text;
      CurrMeaning = WordTree->Selected->Parent->Text;
      CurrMeaType = WordTree->Selected->Parent->ImageIndex;
      CurrSynonym = WordTree->Selected->Text;
    }
    WordPanel->Visible = false;
    MeaningPanel->Visible = true;
    MeaningPanel->Align = alClient;
    RefreshCurrMeaning();
  }
  RefreshCaption();
}
//---------------------------------------------------------------------------
void __fastcall TMForm::TagsMemoDblClick(TObject *Sender)
{ FindWord->Text = TagsMemo->Text;
  RefreshWordList();
}
//---------------------------------------------------------------------------

