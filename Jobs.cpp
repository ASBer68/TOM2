#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>
#pragma hdrstop
#include "MainForm.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
//обновим кластеры
void __fastcall TMForm::Clustering(TObject*Sender)
{ Status->Text="—оставл€ем кластеры слов";
  Application->ProcessMessages();
  ProgressBar->Max = 4;
  ProgressBar->Position=0;
  //очистим все кластеры
  AnsiString SQL="UPDATE word SET cluster = NULL";
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  ProgressBar->Position++;

  //посчитаем и запишем количество слов дл€ каждой словоформы
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


  //распределим слова по кластерам
  //--------------------------------------------------------
  //отберЄм словоформы принадлежащие различным словам
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
  while(bool(row=mysql_fetch_row(Result))) //цикл по словоформам
  { //--------------------------------------------------------
    //отберЄм слова содержащие эту словоформу
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
    for(int r=0; bool(row=mysql_fetch_row(Words)); r++) //цикл по словам
    { if(row[1]) //есть кластер
      { if(r)
        { if(Cluster!=row[1]) //кластеры отличаютс€
          { //сливаем кластеры

            //==============================================
            // заглушка
            Status->Text="“ребуетс€ доработка кластеров!!!";
            return;
            //==============================================

          }
        }
        else Cluster=row[1]; //берем кластер первой строки
      }
      else //нет кластера
      { if(r==0) Cluster=row[0]; //id первого слова становитс€ кластером
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
  //слова не вход€щие в кластер образуют кластер из одного слова
  if(mysql_query(&DB,
    "UPDATE word SET cluster = id "
    "WHERE cluster IS NULL AND status >= 100"
  ))
  { Status->Text=mysql_error(&DB);
    return;
  }
  //--------------------------------------------------------
  Status->Text="Ok, кластеры обновлены";
  ProgressBar->Position=0;
}
//==============================================================================
void __fastcall TMForm::SetAccentuations(TObject*Sender)
{ //анализ ударений
  //сброс ударений
  AnsiString SQL = "UPDATE form SET accentuation = 0";
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  //расстановка ударений
  SQL="UPDATE form, form AS form2 "
      "SET form.accentuation = 1 "
      "WHERE form.search_form = form2.search_form AND form.form <> form2.form";
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  Status->Text="Ok, ударени€ расставлены";
}

//==============================================================================
char*MrSrPds[12] = //таблица преобразовани€ падежей дл€ сущ. ћр и —р
{ "»п≈ч", "»пћч",
  "–п≈ч", "–пћч",
  "ƒп≈ч", "ƒпћч",
  "¬п≈ч", "¬пћч",
  "“п≈ч", "“пћч",
  "ѕп≈ч", "ѕпћч"
};
char*ZHrPds[13] = //таблица преобразовани€ падежей дл€ сущ. ∆р
{ "»п≈ч", "»пћч",
  "–п≈ч", "–пћч",
  "ƒп≈ч", "ƒпћч",
  "¬п≈ч", "¬пћч",
  "“п≈ч", "“п≈чѕс", "“пћч",
  "ѕп≈ч", "ѕпћч"
};
char*AdjPds[37] = //таблица преобразовани€ падежей дл€ прилагательных
{ "ћр»п≈ч",     "∆р»п≈ч",     "—р»п≈ч",     "»пћч",
  "ћр–п≈ч",     "∆р–п≈ч",     "—р–п≈ч",     "–пћч",
  "ћрƒп≈ч",     "∆рƒп≈ч",     "—рƒп≈ч",     "ƒпћч",
  "ћр¬п≈чЌд",   "∆р¬п≈ч",     "—р¬п≈ч",     "¬пћчЌд",
  "ћр¬п≈чќд",   "del",        "del_2",      "¬пћчќд",
  "ћр“п≈ч",     "∆р“п≈ч",
                "∆р“п≈чѕс",   "—р“п≈ч",     "“пћч",
  "ћрѕп≈ч",     "∆рѕп≈ч",     "—рѕп≈ч",     "ѕпћч",
  "ћр»п≈ч ф“ы", "∆р»п≈ч ф“ы", "—р»п≈ч ф“ы", "»пћч ф",
  "del_3",      "del_4",      "del_5",      "del_6"
};
void __fastcall TMForm::WordKeyProcessing(TObject*Sender)
{ Status->Text="";
  //попытка расписать словоформы
  //==================================================
  //существительные мужского и среднего рода
  AnsiString SQL=
    "SELECT word.id, name, COUNT(form) AS form_cnt "
    "FROM form LEFT JOIN word "
    "ON word.id = form.word_id "
    "WHERE word.status = 20 " //нетронутые после «ализн€ка слова
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
    Application->ProcessMessages(); //чтоб не вешалась

    AnsiString Key, Word = row[1];
    switch(Word[Word.Length()])
    { case 'ь': //невозможно определить род
        Key="";
        break;
      case 'о': case 'е': case 'Є': case '€': //средний род
        Key="—р";
        break;
      default: Key="ћр";  //мужской род
        break;
    }
    //апдейтим слово
    SQL="UPDATE word SET fkey = '";
    SQL+=Key+"', defkey = '»п≈ч', status = 25 ";
    SQL+=" WHERE id = ";
    SQL+=row[0];
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    //апдейтим формы
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
  //существительные женского рода
  SQL=
    "SELECT word.id, word.name "
    "FROM form LEFT JOIN word "
    "ON word.id = form.word_id "
    "WHERE form.fkey = '10' AND RIGHT(form,1) = 'ю' AND word.status = 20";

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
    Application->ProcessMessages(); //чтоб не вешалась
    //апдейтим слово
    SQL="UPDATE word SET fkey = '∆р', defkey = '»п≈ч', status = 25 "
        "WHERE id = ";
    SQL+=row[0];
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    //апдейтим формы
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
  //прилагательные
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
    Application->ProcessMessages(); //чтоб не вешалась
    //апдейтим слово
    SQL="UPDATE word SET fkey = '', defkey = '—р»п≈чЌд', status = 25 "
        "WHERE id = ";
    SQL+=row[0];
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    //апдейтим формы
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

  Status->Text="Ok, обработка слов завершена";
}

//---------------------------------------------------------------------------
char*WerbSpr[14][4] = //таблица преобразовани€ спр€жени€ дл€ глаголов
{ "»вѕф",     "»вѕф",     "»вѕф",     "»вѕф",   //инфинитив
  "Ќв1л≈ч“ы", "Ќв1л≈ч“ы", "Ѕв1л≈ч“ы", "Ѕв1л≈ч“ы",
  "Ќв1лћч",   "Ќв1лћч",   "Ѕв1лћч",   "Ѕв1лћч",
  "Ќв2л≈ч“ы", "Ќв2л≈ч“ы", "Ѕв2л≈ч“ы", "Ѕв2л≈ч“ы",
  "Ќв2лћч",   "Ќв2лћч",   "Ѕв2лћч",   "Ѕв2лћч",
  "Ќв3л≈ч“ы", "Ќв3л≈ч“ы", "Ѕв3л≈ч“ы", "Ѕв3л≈ч“ы",
  "Ќв3лћч",   "Ќв3лћч",   "Ѕв3лћч",   "Ѕв3лћч",

  "del",      "ѕвћр≈ч“ы", "del",      "ѕвћр≈ч“ы",
  "ѕвћр≈ч“ы", "ѕв∆р≈ч“ы", "ѕвћр≈ч“ы", "ѕв∆р≈ч“ы",
  "ѕв∆р≈ч“ы", "ѕв—р≈ч“ы", "ѕв∆р≈ч“ы", "ѕв—р≈ч“ы",
  "ѕв—р≈ч“ы", "ѕвћч",     "ѕв—р≈ч“ы", "ѕвћч",

  "ѕвћч",     "Ќв2л≈чѕф", "ѕвћч",     "Ќв2л≈чѕф",

  "Ќв2л≈чѕф", "Ќв2лћчѕф", "Ќв2л≈чѕф", "Ќв2лћчѕф",
  "Ќв2лћчѕф", "del",      "Ќв2лћчѕф", "del",
};

void __fastcall TMForm::VerbKeyProcessing(TObject*Sender)
{ //ѕодбор кодов к глаголу
  //—овершенность запрашиваем интерактивно.
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
    //запрашиваем совершЄнность
    AnsiString Q="будет ";
    Q+=row[1];
    Q+=" - это правильно?";
    switch
    ( Application->MessageBoxA( Q.c_str(),"—овершЄнность глагола",
                                MB_YESNOCANCEL|MB_ICONQUESTION))
    { case IDYES: //несовершенное
        Key = "Ќш";
        DefKey = "Ќвћр3л≈ч";
        break;
      case IDNO: //совершенное
        Key = "—ш";
        DefKey = "ѕвћр3л≈ч";
        col+=2;
        break;
      default: StopFlag=true; continue; //останавливаем обработку
    }
    //апдейтим слово
    SQL= "UPDATE word SET status = 25, fkey = '";
    SQL+=Key+"', defkey = '";
    SQL+=DefKey + "' WHERE id = ";
    SQL+=row[0];
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    //апдейтим формы
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
  //удалим лишние формы
  if(mysql_query(&DB,"DELETE FROM form WHERE fkey = 'del'"))
  { Status->Text=mysql_error(&DB);
    return;
  }


  Status->Text="Ok, обработка глаголов завершена";
/* в лесу родилась Єлка
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
{ //добавл€ем заготовки глагольных форм к текущему слову
  Status->Text="";
  if(WordId.IsEmpty()||CurrWord.IsEmpty())
  { Status->Text="¬ыберите глагол!";
    return;
  }
  int Type = 0; //несовершенное
  if(Sender==Verb2Menu) Type=2; //совершенное

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

  Status->Text="Ok, формы глаголов добавлены.";
}
//---------------------------------------------------------------------------
void __fastcall TMForm::AnalysisEndsFile(TObject*Sender)
{ //анализируем файл с окончани€ми
  char*FileName;
//if(Sender==(TObject*)&NVerbEnds)
    FileName = "test_ends.verb";
//else if(Sender==(TObject*)&NWordEnds)
//  FileName = "test_ends.noun";
//else
//{ Status->Text=L"тип файла не определЄн!";
//  return;
//}
  //------------------------------------------
  Status->Text=L"загружаем файл...";
  int handle=open(FileName, O_RDONLY|O_BINARY);
  if(handle==-1)
  { Status->Text=L"файл не найден!";
    return;
  }
  long size=filelength(handle);
  if(size<0||size>1000000)
  { Status->Text=L"не удалось определить размер файла!";
    return;
  }
  char*buf=new char[size+1];
  if(read(handle,buf,size)==-1)
  { delete buf;
    Status->Text=L"чтение файла невозможно!";
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
  Status->Text=L"анализируем файл...";
  for(int i=0; i<EndsList->Count; i++)
  { AnsiString DelEnd, AddEnd;
    int p=EndsList->Strings[i].Pos("\t"); //разделитель строки
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
      //заполним индекс
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
  Status->Text=L"выводим результат...";
  AnsiString Text;
  for(int j=0; j<1000; j++)
  { if(Index[j].Str.IsEmpty()) break;
    if(Index[j].Count==1) continue;
    Text+=Index[j].Str+"\t";
    Text+=Index[j].Count;
    Text+="\r\n";
  }
  InputTxt(Text,"јнализ файла окончаний","«акрыть");
  Status->Text=L"Ok!";
}
//---------------------------------------------------------------------------

