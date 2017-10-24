#pragma hdrstop
#include "MainForm.h"
#include "SQLaction.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// произвольный SQL-запрос
void __fastcall TMForm::ExecuteSQL(TObject*Sender)
{ AnsiString Query, Text;
  //-----------------------------------------------
  if(Sender==TableListBox)
  { //покажем колонки таблицы
    Query = "show columns from ";
    int i;
    for(i=0; i<TableListBox->Items->Count; i++)
      if(TableListBox->Selected[i])
      { Query+=TableListBox->Items->operator[](i);
        if(SQLText->Text.IsEmpty())
        { AnsiString TableQuery="SELECT * FROM ";
          TableQuery+=TableListBox->Items->operator[](i);
          SQLText->Text=TableQuery;
        }
        break;
      }
  }
  else switch(QueryPgCntr->TabIndex) //произвольный SQL-запрос
  { case 0: Query = SQLText->Text;  break;
    case 1: Query = SQLText2->Text; break;
    case 2: Query = SQLText3->Text; break;
  }
  //-----------------------------------------------
  //выполним SQL-запрос
  if(mysql_query(&DB,Query.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  SQLResult->Lines->Clear();
  //-----------------------------------------------
  //покажем результат
  MYSQL_RES*Result=mysql_use_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_ROW row;
  int num_fields = mysql_num_fields(Result);
  for(int r=0; r<200 && bool(row=mysql_fetch_row(Result)); r++)
  { AnsiString sRow;
    for(int i=0; i<num_fields; i++)
    { sRow+=row[i]?row[i]:"NULL";
      sRow+="; ";
    }
    SQLResult->Lines->Add(sRow);
  }
  mysql_free_result(Result);
  //-----------------------------------------------
  Status->Text="Ok!";
}


//---------------------------------------------------------------------------
