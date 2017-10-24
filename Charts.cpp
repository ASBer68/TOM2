#pragma hdrstop
#include "MainForm.h"
#include "DBaction.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
void __fastcall TMForm::RefreshDBChart(TObject *Sender)
{ //диаграмма словаря
  Series1->Clear();
  Series2->Clear();
  Series3->Clear();
  Series4->Clear();
  Series5->Clear();
  Series6->Clear();
  Series7->Clear();
  Series8->Clear();

  AnsiString SQL=

    "SELECT type, status, 'xxx' AS mstatus, COUNT(name) "
    "FROM word "
    "GROUP BY type, status "
/*
    "UNION "
    "SELECT type, 700, 'Констр.', COUNT(name) "
    "FROM word "
    "WHERE word.status >=100 AND NOT obj_code IS NULL "
    "GROUP BY type "
*/
    "UNION "

    "SELECT type, 800, 'Гл.код', COUNT(name) "
    "FROM word "
    "WHERE word.status >=100 AND NOT glb_code IS NULL "
    "GROUP BY type "

    "UNION "

    "SELECT word.type, 900, 'Синонимы', COUNT(object_id) "
    "FROM synonym "
    "JOIN word ON word_id = word.id "
    "WHERE word.status >=100 "
    "GROUP BY type "
/*
    "UNION "
    "SELECT word.type, 999, meaning.type, COUNT(meaning.name) "
    "FROM meaning "
    "JOIN word ON word_id = word.id "
    "WHERE word.status >=100 "
    "GROUP BY word.type, meaning.type "
*/
    "ORDER BY status, mstatus, type";


  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_RES*Result=mysql_use_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  AnsiString Count, Status="0000000000", ColNames[100];
  int NoDef = 0; //слова без типа или статуса
  TBarSeries*Series;
  int Coll=0; //Status=-1000; //номер актуального столбика и статуса
  MYSQL_ROW row;
  while(bool(row=mysql_fetch_row(Result)))
  { Count=row[3];
    if(!row[0] || !row[1])
    { //не определён тип или статус слова
      NoDef+=Count.ToInt();
      continue;
    }
    if(strcmp(row[0],"noun")==0)      Series=Series1;
    else if(strcmp(row[0],"verb")==0) Series=Series2;
    else if(strcmp(row[0],"adj")==0)  Series=Series3;
    else if(strcmp(row[0],"adv")==0)  Series=Series4;
    else if(strcmp(row[0],"prep")==0) Series=Series5;
    else if(strcmp(row[0],"pron")==0) Series=Series6;
    else if(strcmp(row[0],"part")==0) Series=Series7;
    else
    { NoDef+=Count.ToInt(); //неучтенный тип
      continue;
    }
    AnsiString sStatus = "0000000000";
    if(strcmp(row[1],"999")==0)     sStatus+=row[2];
    else if(strcmp(row[1],"-1")!=0) sStatus+=row[1];
    sStatus = sStatus.SubString(sStatus.Length()-9,10);

    while(Series->Count()+1<Coll)
      Series->AddNull(ColNames[Series->Count()+1]); //догоняем серию
    if(sStatus>Status && Series->Count()<Coll)
      Series->AddNull(ColNames[Series->Count()+1]); //догоняем серию

    if(sStatus>Status)
    { //добавим прочее в предыдущую колонку
      if(NoDef) Series8->Add(NoDef,"");
      else Series8->AddNull("");
      NoDef=0;
    }

    AnsiString ColName;
    if(strcmp(row[1],"-1")==0)          ColName = "Мусор";
    else if(strcmp(row[1],"10")==0)     ColName = "5000 слов";
    else if(strcmp(row[1],"20")==0)     ColName = "Зализняк";
    else if(strcmp(row[1],"100")==0)    ColName = "Cловарь";
    else if(strcmp(row[1],"700")==0 ||
            strcmp(row[1],"800")==0 ||
            strcmp(row[1],"900")==0 ||
            strcmp(row[1],"999")==0 )   ColName = row[2];
    else                                ColName = row[1];
    Series->Add(Count.ToInt(),ColName);

    if(Series->Count()>Coll)
    { Coll=Series->Count();   //актуальный столбец
      Status=sStatus; //актуальный статус
      ColNames[Coll]=ColName; //имя столбца
    }
  }
  //добавим прочее в последнюю колонку
  if(NoDef) Series8->Add(NoDef,"");
  mysql_free_result(Result);
}
//---------------------------------------------------------------------------
void __fastcall TMForm::RefreshHashtagChart(TObject *Sender)
{ //диаграмма хештегов
  SeriesAct->Clear();
  SeriesAct2->Clear();
  SeriesAction->Clear();
  SeriesAction2->Clear();
  SeriesActionSyn->Clear();
  SeriesActionSyn2->Clear();
  SeriesFact->Clear();
  SeriesFact2->Clear();
  SeriesState->Clear();
  SeriesState2->Clear();
  SeriesClass->Clear();
  SeriesClass2->Clear();
  SeriesAttribute->Clear();
  SeriesAttribute2->Clear();

  AnsiString SQL=
    "SELECT hashtag, type, COUNT(id), status "
    "FROM meaning "
    "WHERE NOT hashtag IS NULL "
    "GROUP BY hashtag, type, status "
/*
    "UNION "

    "SELECT hashtag, 'synonym', COUNT(synonym.id) "
    "FROM meaning "
    "JOIN synonym ON synonym.object_id = meaning.id "
    "WHERE type='act' AND NOT hashtag IS NULL "
    "GROUP BY hashtag "

    "UNION "

    "SELECT hashtag, 'not ready', COUNT(id) "
    "FROM meaning "
    "WHERE status IS NULL AND NOT hashtag IS NULL "
    "GROUP BY hashtag "
*/
    "ORDER BY hashtag, type";

  sql Q(SQL);
  if(Q.Error) return;

  AnsiString Count, RowNames[100];
  THorizBarSeries*Series;
  int Row=0; //номер актуальной строки
  while(Q.Next())
  { Count=Q[2];
    if(!Q[1]) continue;
    else if(strcmp(Q[1],"act")==0)       Series=Q[3]?SeriesAct:SeriesAct2;
    else if(strcmp(Q[1],"action")==0)    Series=Q[3]?SeriesAction:SeriesAction2;
    else if(strcmp(Q[1],"act_syn")==0)   Series=Q[3]?SeriesActionSyn:SeriesActionSyn2;
    else if(strcmp(Q[1],"fact")==0)      Series=Q[3]?SeriesFact:SeriesFact2;
    else if(strcmp(Q[1],"expression")==0)Series=Q[3]?SeriesState:SeriesState2;
    else if(strcmp(Q[1],"class")==0)     Series=Q[3]?SeriesClass:SeriesClass2;
    else if(strcmp(Q[1],"attribute")==0) Series=Q[3]?SeriesAttribute:SeriesAttribute2;
    else continue;

    AnsiString RowName=Q[0];
    while(Series->Count()+1<Row)
      Series->AddNull(RowNames[Series->Count()+1]); //догоняем серию
    if(RowNames[Row]!=Q[0] && Series->Count()<Row)
      Series->AddNull(RowNames[Series->Count()+1]); //догоняем серию

    int Cnt=Count.ToInt();
    if(strcmp(Q[1],"synonym")==0) Cnt/=3; //синонимов обычно больше
    Series->Add(Cnt, RowName);

    if(Series->Count()>Row)
    { Row = Series->Count(); //актуальная строка
      RowNames[Row] = Q[0];  //имя строки
    }
  }
}
//---------------------------------------------------------------------------
