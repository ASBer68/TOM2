#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>
#pragma hdrstop
#include "MainForm.h"
#include "DBaction.h"

#define add_StrCode
#include "..\DicStruct.h" //файл с описанием структур словаря
#pragma package(smart_init)
//---------------------------------------------------------------------------
// модуль содержит процедуры для сборки файла словаря
//---------------------------------------------------------------------------
bool ErrorMake;
void __fastcall TMForm::MakeAll(TObject*Sender)
{ //выполняем все действия по сборке
  ErrorMake=false;
  ProgressBar->Max = 5000; //надо посчитать сколько в действительности!
  ProgressBar->Position=0;
  //кросс-статьи
  MakeAssociation(Sender);
  if(ErrorMake) return;
  //статьи
  MakeEntries(Sender);
  if(ErrorMake) return;
  //hash-таблица
  MakeHashTab(Sender);
  if(ErrorMake) return;
  //финальная сборка
  MakeDic(Sender);
  ProgressBar->Position=0;
}
//---------------------------------------------------------------------------
dic_header Hdr;
//----------------
bool __fastcall AttachFile(int File1, int File2)
{ //присоединяем файл 2 к файлу 1
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
{ Status->Text="Собираем файл словаря";
  Application->ProcessMessages();
  //откроем временные файлы
  int hHashTmp=open("hash_tab.tmp",O_RDONLY|O_BINARY);
  if(hHashTmp==-1)
  { Status->Text="Не найден файл hash_tab.tmp";
    ErrorMake=true;
    return;
  }

  int hStrArrayVerb=open("StrArray.verb",O_RDONLY|O_BINARY);
  if(hStrArrayVerb==-1)
  { Status->Text="Не найден файл StrArray.verb";
    ErrorMake=true;
    return;
  }

  int hStrArrayNoun=open("StrArray.noun",O_RDONLY|O_BINARY);
  if(hStrArrayNoun==-1)
  { Status->Text="Не найден файл StrArray.noun";
    ErrorMake=true;
    return;
  }

  int hEntrTmp=open("entries.tmp",O_RDONLY|O_BINARY);
  if(hEntrTmp==-1)
  { Status->Text="Не найден файл entries.tmp";
    ErrorMake=true;
    return;
  }

  //создаём файл словаря
  int hDic=open("TOM.dic",O_WRONLY|O_CREAT|O_TRUNC|O_BINARY|O_APPEND,
                          S_IREAD|S_IWRITE );
  if(hDic==-1)
  { Status->Text="Не удалось открыть файл TOM.dic";
    ErrorMake=true;
    return;
  }

  //пишем версию и размер загружаемой части
  memcpy(Hdr.Version,"2.a.4.9",8);  //версия

  Hdr.ShiftNoun = sizeof(Hdr)
                + filelength(hHashTmp)
                + filelength(hStrArrayVerb);

  Hdr.ShiftEntr = Hdr.ShiftNoun
                + filelength(hStrArrayNoun);

  write(hDic,&Hdr,sizeof(Hdr));

  //присоединяем ранее сформированные части
  AttachFile(hDic,hHashTmp);
  AttachFile(hDic,hStrArrayVerb);
  AttachFile(hDic,hStrArrayNoun);
  AttachFile(hDic,hEntrTmp);

  //закрываем файлы
  close(hHashTmp);
  close(hStrArrayVerb);
  close(hStrArrayNoun);
  close(hEntrTmp);
  close(hDic);

  ProgressBar->Position+=100;
  Application->ProcessMessages();

  Status->Text="Файл словаря собран!";
}

//==============================================================================
char*MeaningFormat[][2] = //шаблоны для формирования объектов
{ //слова
  {"word","$%N%{\n%C%\n%A%\n%B%\n}\n"},
  //предлоги
  //%F% - формы
  {"prep","preposition %N%\n{%V%%B%\n}\n"},
  //функции
  {"action",    "\naction(%A%)\n{%B%\n}\n"},
  {"fact",      "\nfact(%A%)\n{%B%\n}\n"},
  {"expression","\nexpression(%A%)\n{%B%\n}\n"},
  //синонимы функций
  {"act_syn",   "\n%W%\naction(%A%) = action(%P%)\n"},
  {"fact_syn",  "\n%W%\nfact(%A%) = fact(%P%)\n"},
  {"exp_syn",   "\n%W%\nexpression(%A%) = expression(%P%)\n"},
  //переменные
  {"variable",  "variable %N%=%A%\n"},
  {"prp",       "property %N%=%A%\n"},
  {"tmp",       "template %N%=%A%\n"},
  {"atr",       "%N%\n"},
  //синонимы
  {"synonym",   "%W%\n%N%.synonym=$%A%\n"},
  //классы и объекты
  {"class",     "\n@%N%\n{%C%\n%V%\n%B%\n}\n"},
  {"location",  "\nlocation %N%\n{%C%\n%V%\n%B%\n}\n"},
  {0,           "\n%T% %N%\n{%C%\n%V%\n%B%\n}\n"},
};
AnsiString __fastcall GetObjectCode( const char*T,   //%T% - тип объекта
                                     const char*N,   //%N% - имя объекта
                                     const char*C,   //%C% - класс объекта
                                     const char*A,   //%A% - аргументы
                                     const char*B=0, //%B% - тело объекта
                                     const char*P=0, //%P% - аргументы родителя
                                     const char*W=0  //%W% - основное слово
                                   )
{ //формируем код объекта по шаблону из заданных аргументов
  //при отсутствии обязательного аргумента возвращаем пустую строку
  if(!T||!T[0]) return ""; //тип должен быть!
  //найдем шаблон
  AnsiString Text;
  for(int i=0;;i++)
    if(!MeaningFormat[i][0] || strcmp(MeaningFormat[i][0],T)==0)
    { Text = MeaningFormat[i][1];
      break;
    }
  //вставим аргументы
  int p;
  if((bool)(p=Text.Pos("%T%"))) Text=Text.Delete(p,3).Insert(T,p);
  if((bool)(p=Text.Pos("%N%"))) Text=Text.Delete(p,3).Insert(N,p);
  if((bool)(p=Text.Pos("%C%"))) Text=Text.Delete(p,3).Insert(C,p);
  //-------------------------------------------
  if((bool)(p=Text.Pos("%A%")))
  { if(!A||!A[0]) return ""; //аргументы должны быть!
    Text=Text.Delete(p,3).Insert(A,p);
  }
  //variables - необязательные аргументы у объектов
  if((bool)(p=Text.Pos("%V%"))) Text=Text.Delete(p,3).Insert(A,p);
  //-------------------------------------------
  if((bool)(p=Text.Pos("%B%"))) Text=Text.Delete(p,3).Insert(B,p);
  if((bool)(p=Text.Pos("%P%")))
  { if(!P||!P[0]) return ""; //аргументы должны быть!
    Text=Text.Delete(p,3).Insert(P,p);
  }
  if((bool)(p=Text.Pos("%W%"))) Text=Text.Delete(p,3).Insert(W,p);

  while((bool)(p=Text.Pos("\n\n"))) Text=Text.Delete(p,1);
  return Text;
}
//---------------------------------------------------------------------
void __fastcall TMForm::MakeEntries(TObject*Sender)
{ AnsiString SQL;
  Status->Text="Собираем словарные статьи";
  Application->ProcessMessages();
  //--------------------------------------------------
  //удаляем таблицу
  if(mysql_query(&DB,"DROP TABLE IF EXISTS entries"))
  { Status->Text=mysql_error(&DB);
    ErrorMake=true;
    return;
  }
  //создаём таблицу
  if(mysql_query(&DB,
    "CREATE TABLE entries("
    "cluster INT NOT NULL PRIMARY KEY," //ссылка на слово
    "addr INT," //адрес слова в файле
    "code TEXT,"//содержание статьи
    "len INT"   //длина статьи
    ") default charset=cp1251" //кодировка по умолчанию
  ))
  { Status->Text=mysql_error(&DB);
    ErrorMake=true;
    return;
  }
  //--------------------------------------------------
  //получим список кластеров
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
  while(bool(row=mysql_fetch_row(Result))) //цикл по кластерам
  { AnsiString Text; //текст статьи
    AnsiString Cluster=row[0]; //id кластера
    Status->Text=row[1];
    Application->ProcessMessages();
    //соберём формы слов к кластеру
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
    AnsiString FormList[256];//список словоформ
    int FormCnt = 1; //количество словоформ
    MYSQL_ROW row2;
    for(; bool(row2=mysql_fetch_row(Word)); FormCnt++) //цикл по основаниям
    { if(FormCnt>127)
      { AnsiString Err="Превышено допустимое количество словоформ для кластера ";
        Err+=row[1];
        Status->Text = Err;
        ErrorMake=true;
        return;
      }
      if(FormCnt=='\'' || FormCnt=='\r') //эти коды нельзя использовать!
      { FormList[FormCnt++]="-";
        Text+="-;";
      }
      FormList[FormCnt]=row2[0];
      //обработка ударений
      if(row2[1] && row2[1][0]=='1')
        Text+=row2[0]; //сохраним ударение
      else
      { //удалим ударение
        AnsiString TmpTx = row2[0];
        int p = TmpTx.Pos("`");
        if(p) Text+=TmpTx.Delete(p,1);
        else Text+=row2[0];
      }
      Text+=";";
    }
    mysql_free_result(Word);
    Text+=";";

    //соберём словарную часть статьи
    SQL = "SELECT word.id, word.name, word.type, word.fkey, "     //0,1,2,3,
          "word.defkey, 'deleted', word.obj_code, meaning.name "  //4,6,7,
          "FROM word "
          "LEFT JOIN meaning ON word.class_id = meaning.id "
          "WHERE word.status>=100 AND word.cluster = ";
    SQL+= Cluster;
    sql qWord(SQL,ErrorMake);
    while(qWord.Next()) //цикл по словам
    { if(ErrorMake) return; //ошибка
      bool ItsPrep = (qWord[2] && strcmp(qWord[2],"prep")==0);
      //соберём формы -----------------------------------------
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
        { //найдем текущую словоформу
          bool Find=false;
          for(int f=1; !FormList[f].IsEmpty(); f++)
            if(FormList[f]==Form[0])
            { Code[1] = f;
              Forms+=Code;
              Find=true;
              break;
            }
          if(!Find)
          { Status->Text="Не найдена форма слова в списке кластера!";
            ErrorMake=true;
            return;
          }
          Forms+=Form[1]; //ключ
        }
      }
      //соберём класс и ключи ---------------------------------
      AnsiString Class;
      if(qWord[7]&&qWord[7][0])
      { Class+="\n@";
        Class+=qWord[7];
      }
      //добавим ключ
      if(qWord[3] && qWord[3][0])
      { Class+="\nkey = #";
        Class+=qWord[3];
      }
      //добавим умолчальный ключ
      if(qWord[4]&&qWord[4][0])
      { Class+="\ndefkey = #";
        Class+=qWord[4];
      }
      //добавим слово -----------------------------------------
      Text+=GetObjectCode( ItsPrep?"prep":"word", qWord[1], Class.c_str(),
                           Forms.c_str(), qWord[6] );
    }

    //найдём и добавим значения слов
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
    { //цикл по значениям
      if(ErrorMake) return; //произошла ошибка
      //классы ----------------------------------
      AnsiString ClassList;
      for(int c=3; c<7; c+=2)
        if(row3[c]) //есть класс
        { if(strcmp(row3[c],"class")==0) ClassList+="@";
          else ClassList+="is ";
          ClassList+=row3[c+1];
          ClassList+="\n";
        }
      //подобие ----------------------------------
      if(row3[7]) //есть подобие
      { ClassList+="like ";
        if(strcmp(row3[7],"class")==0) ClassList+="@";
        ClassList+=row3[8];
        ClassList+="\n";
      }
      //дополнительные имена --------------------
      SQL = "SELECT word.name "
            "FROM synonym "
            "JOIN word ON word_id = word.id "
            "WHERE status>=100 AND (add_name OR cluster = ";
      SQL+=Cluster+") AND object_id = ";
      SQL+= row3[0];
      sql Q(SQL,ErrorMake);
      while(Q.Next()) //цикл по именам
      { ClassList+="synonym=$";
        ClassList+=Q[0]; //слово
        ClassList+="\n";
      }
      //тело объекта ----------------------------
      AnsiString Body, Methods;
      SQL = "SELECT type, name, code "
            "FROM association "
            "WHERE word_id IS NULL AND object_id = ";
      SQL+= row3[0];
      sql Prp(SQL,ErrorMake);
      while(Prp.Next()) //цикл по свойствам
        if
        ( strcmp(Prp[0],"prp")==0 ||
          strcmp(Prp[0],"tmp")==0 ||
          strcmp(Prp[0],"atr")==0
        ) Body+=GetObjectCode(Prp[0],Prp[1],0,Prp[2]); //свойство или признак
        else
        { //метод
          Methods+=Prp[0];
          Methods+=Prp[1];
          Methods+=Prp[0];
          Methods+="\n";
          Methods+=Prp[2];
          Methods+="\n";
        }
      Body+=row3[10]; //секции
      Body+="\n";
      Body+=Methods;
      //-----------------------------------------------------------------
      Text+=GetObjectCode( row3[1], row3[2], ClassList.c_str(),
                           row3[9], Body.c_str(), row3[11], row3[12] );
      //-----------------------------------------------------------------
      if(row3[13])
      { //упомянем целое для части
        Text+=row3[13];
        Text+="\n";
      }
    }
    mysql_free_result(Meaning);

    //добавим синонимы --------------------------------------------------
    SQL =
    "SELECT word.name, meaning.type, meaning.name, synw.name " //0, 1, 2, 3
    "FROM synonym "
    "JOIN meaning ON meaning.id = synonym.object_id "
    "JOIN word ON word.id = meaning.word_id AND word.status >=100 "
    "JOIN word AS synw ON synw.id = synonym.word_id AND synw.status >=100 "
    "WHERE NOT add_name AND word.cluster <> synw.cluster AND synw.cluster = ";
    SQL+=Cluster;
    sql S(SQL);
    while(S.Next()) //цикл по синонимам
    { AnsiString Name;
      if(strcmp(S[1],"class")==0) Name="@";
      Name+=S[2];
      Text+=GetObjectCode("synonym",Name.c_str(),0,S[3],0,0,S[0]);
    }

    //доп-имена ---------------------------------------------------------
    SQL =
    "SELECT word.name, synw.name "
    "FROM synonym "
    "JOIN meaning ON meaning.id = synonym.object_id "
    "JOIN word ON word.id = meaning.word_id AND word.status >=100 "
    "JOIN word AS synw ON synw.id = synonym.word_id AND synw.status >=100 "
    "WHERE add_name AND word.cluster <> synw.cluster AND synw.cluster = ";
    SQL+=Cluster;
    sql D(SQL);
    while(D.Next()) //цикл по синонимам
    { Text+="$";
      Text+=D[0];
      Text+="\n";
    }

    //-------------------------------------------------------------------
    //добавим глобальный код слов
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
    for(int i=0; bool(row2=mysql_fetch_row(Word)); i++) //цикл по словам
    { if(!i) Text+="\n";
      Text+=row2[0];
    }
    mysql_free_result(Word);
    //----------------------------------------------------
    //сжатие текста
    int p;
    //удаляем все переводы каретки и лишние переводы строк
    while(bool(p=Text.Pos("\r")))  Text=Text.Delete(p,1);
    while(bool(p=Text.Pos("\n\n")))Text=Text.Delete(p,1);
    //кодируем строки
    for(int i=0; StrCode[i].Code; i++) //цикл по таблице строк
    { if(!StrCode[i].Str || !StrCode[i].Str[0]) continue;
      int l = strlen(StrCode[i].Str) - 1;
      while(bool(p=Text.Pos(StrCode[i].Str)))
      { Text[p]=StrCode[i].Code;
        Text=Text.Delete(p+1,l);
      }
    }
    //----------------------------------------------------
    //посчитаем суммарную длину кросс-статей для этого кластера
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
    //добавим статью
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
  //вычислим адресный шаг для словарных статей
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
  { int SUMlen= AnsiString(row[0]).ToInt(); //cумарный объём всех статей
    int COUNT = AnsiString(row[1]).ToInt(); //количество статей
    Hdr.EntrStep = (SUMlen+COUNT) / (0xFFFF-COUNT) + 1;
  }
  else
  { Status->Text="Не удалось прочитать SUM(len) и COUNT(cluster)";
    ErrorMake=true;
    return;
  }
  mysql_free_result(Result);
  //--------------------------------------------------
  //распределим адреса статей
  Status->Text="Распределяем адреса словарных статей";
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
    { Status->Text="Адресация статей превысила 0xFFFF!!!";
      ErrorMake=true;
      break;
    }
    const char*clst= row[0]; //текущий кластер
    int len = AnsiString(row[1]).ToInt();
    int A = len/Hdr.EntrStep + 1; //количество секторов в статье
    //запишем адресс в статьи
    SQL="UPDATE entries SET addr = ";
    SQL+=Addr;
    SQL+=" WHERE cluster = ";
    SQL+=clst;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      ErrorMake=true;
      return;
    }
    //запишем адресс в кросс-статьи
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
    //сдвигаем адрес на количество секторов в статье
    Addr+=A;
  }
  mysql_free_result(Result);
  //--------------------------------------------------
  //сохраняем статьи во временный файл
  Status->Text="Сохраняем словарные статьи в файл";
  Application->ProcessMessages();
  int hEntr=open("entries.tmp",O_WRONLY|O_CREAT|O_TRUNC|O_BINARY|O_APPEND,
                               S_IREAD|S_IWRITE );
  if(hEntr==-1)
  { Status->Text="Не удалось открыть файл entries.tmp";
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
    const char*clst= row[0]; //текущий кластер
    if(AnsiString(row[1]).ToInt()!=Addr)
    { Status->Text="Адрес статьи не совпадает с фактическим!";
      ErrorMake=true;
      break;
    }
    //пишем статью
    int EntLen = strlen(row[3]) + 1; //длина статьи включая 0
    write(hEntr, row[3], EntLen);
    //запрашиваем кросс-статьи
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
    while(bool(row2=mysql_fetch_row(Cross))) //цикл по кросс-статьям
    { //запишем адрес
      CrossAddr = AnsiString(row2[0]).ToInt();
      write(hEntr, &CrossAddr, 2);
      write(hEntr, row2[1], StrLen(row2[1])+1);
    }
    mysql_free_result(Cross);
    CrossAddr = 0xFFFF;
    write(hEntr, &CrossAddr, 2);

    //запрашиваем ссылки на кросс-статьи
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
    while(bool(row2=mysql_fetch_row(Cross))) //цикл по кросс-статьям
    { //запишем адрес
      CrossAddr = AnsiString(row2[0]).ToInt();
      write(hEntr, &CrossAddr, 2);
    }
    mysql_free_result(Cross);
    CrossAddr = 0xFFFF;
    write(hEntr, &CrossAddr, 2);

    //добиваем последний сектор нулями
    int len = AnsiString(row[2]).ToInt();
    int A = len/Hdr.EntrStep + 1; //количество секторов в статье
    for(int i=A*Hdr.EntrStep-len; i>0; i--) write(hEntr,"\0",1);

    //сдвигаем адрес на количестово секторов в статье
    Addr+=A;
  }
  mysql_free_result(Result);
  close(hEntr);

  //--------------------------------------------------
  if(!ErrorMake) Status->Text="Файл словарных статей собран";
  ProgressBar->Position+=100;
  Application->ProcessMessages();
  //--------------------------------------------------
}

//==============================================================================
void __fastcall TMForm::MakeAssociation(TObject*Sender)
{ AnsiString SQL;
  Status->Text="Собираем словарные ассоциации";
  Application->ProcessMessages();
  //--------------------------------------------------
  //удаляем таблицу
  if(mysql_query(&DB,"DROP TABLE IF EXISTS cross_entries"))
  { Status->Text=mysql_error(&DB);
    ErrorMake=true;
    return;
  }
  //создаём таблицу
  if(mysql_query(&DB,
    "CREATE TABLE cross_entries("
    "cluster_min INT NOT NULL," //
    "cluster_max INT NOT NULL," //
    "addr_min INT,"  //адрес связанной статьи
    "addr_max INT,"  //адрес связанной статьи
    "code TEXT," //содержание статьи
    "len INT,"   //длина статьи
    "UNIQUE KEY(cluster_min,cluster_max)," //уникальный индекс по кластерам
    "KEY(cluster_min),"        //индекс по кластеру
    "KEY(cluster_max)"         //индекс по кластеру
    ") default charset=cp1251" //кодировка по умолчанию
  ))
  { Status->Text=mysql_error(&DB);
    ErrorMake=true;
    return;
  }
  //--------------------------------------------------
  //получим содержание кросс-статей
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

  AnsiString Text; //текст статьи
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
    { //закроем последний объект
      Text+="}";
      ObjType = "";
      ObjName = "";

      //удаляем переводы каретки
      int p;
      while(bool(p=Text.Pos("\r"))) Text=Text.Delete(p,1);
      //кодируем строки
      for(int i=0; StrCode[i].Code; i++) //цикл по кодировочной таблице
      { if(!StrCode[i].Str || !StrCode[i].Str[0]) continue;
        int l = strlen(StrCode[i].Str) - 1;
        while(bool(p=Text.Pos(StrCode[i].Str)))
        { Text[p]=StrCode[i].Code;
          Text=Text.Delete(p+1,l);
        }
      }

      //запишем статью
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
      //чистим текст
      Text="";
    }

    if(!IsRow) break; //строки закончились

    //получим кластеры
    strClust0=row[0];
    strClust1=row[1];
    ClusterMin=strClust0.ToInt();
    ClusterMax=strClust1.ToInt();
    if(ClusterMin>ClusterMax)
    { //меняем кластеры местами
      int X = ClusterMin;
      ClusterMin = ClusterMax;
      ClusterMax = X;
    }
    //открываем новый объект
    if(ObjType!=row[2] || ObjName!=row[3])
    { //закроем предыдущий объект
      if(!Text.IsEmpty()) Text+="}\n";
      //запомним тип и имя
      ObjType = row[2];
      ObjName = row[3];
      //конструктор
      if     (ObjType=="class")  Text+="@";
      else if(ObjType=="word")   Text+="$";
      else if(ObjType!="object")
      { Text+=ObjType;
        Text+=" ";
      }
      //открываем тело
      Text+=ObjName+"{\n";
    }
    //строка конструктора
    if(strcmp(row[4],"syn")==0)
    { //добавим синоним
      Text+="synonym = $";
      Text+=row[5];
    }
    else if(strcmp(row[4],"atr")==0)
    { //добавим атрибут
      Text+=row[5];
    }
    else if(strcmp(row[4],"prp")==0)
    { //добавим свойство
      Text+="property ";
      Text+=row[5];
      Text+="=";
      Text+=row[6];
    }
    else if(strcmp(row[4],"tmp")==0)
    { //добавим свойство
      Text+="template ";
      Text+=row[5];
      Text+="=";
      Text+=row[6];
    }
    else
    { //добавим секцию
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
  if(!ErrorMake) Status->Text="Таблица словарных ассоциаций собрана";
  ProgressBar->Position+=100;
  Application->ProcessMessages();
  //--------------------------------------------------
}
//==============================================================================
AnsiString Report;
bool __fastcall MakeEndsFile(char*FileExt)
{ //-------------------------------------------
  //загружаем файл
  AnsiString FileName = "test_ends.";
  FileName+=FileExt;
  MForm->Status->Text=L"загружаем файл...";
  int handle=open(FileName.c_str(), O_RDONLY|O_BINARY);
  if(handle==-1)
  { MForm->Status->Text=L"файл не найден!";
    return true;
  }
  long size=filelength(handle);
  if(size<0||size>1000000)
  { MForm->Status->Text=L"не удалось определить размер файла!";
    return true;
  }
  char*buf=new char[size+1];
  if(read(handle,buf,size)==-1)
  { delete buf;
    MForm->Status->Text=L"чтение файла невозможно!";
    return true;
  }
  buf[size]=0;  //null terminator
  close(handle);
  TStringList*EndsList = new TStringList;
  EndsList->Text = buf;
  close(handle);
  delete buf;
  //----------------------------------------------------------
  //перенесём данные файла в таблицу
  struct
  { AnsiString DelEnd, AddEnd;   //удаляемое и прибавляемое окончание
  }EndsArray[1000];
  int EndCount=0;
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

    EndsArray[EndCount].DelEnd = DelEnd;
    EndsArray[EndCount].AddEnd = AddEnd;
    EndCount++;
  }
  delete EndsList;
  //----------------------------------------------------------
  //пишем строки в файл
  FileName = "StrArray.";
  FileName+=FileExt;
  int hStr=open(FileName.c_str(),O_WRONLY|O_CREAT|O_TRUNC|O_BINARY|O_APPEND,
                          S_IREAD|S_IWRITE );
  if(hStr==-1)
  { MForm->Status->Text="Не удалось открыть файл StrArray!";
    return true;
  }
  for(int i=0; i<EndCount; i++)
  { int DelL = EndsArray[i].DelEnd.Length();
    int AddL = EndsArray[i].AddEnd.Length();
    //пишем DelEnd
    write(hStr,EndsArray[i].DelEnd.c_str(),DelL);
    write(hStr,"\0",1); //конец строки
    //пишем AddEnd
    write(hStr,EndsArray[i].AddEnd.c_str(),AddL);
    write(hStr,"\0",1); //конец строки
  }
  write(hStr,"\0",1); //завершающий ноль
  close(hStr);
  //----------------------------------------------------------
  //покажем Report
  Report += " строк - ";
  Report += EndCount;
  Report += "\r\n";
  for(int i=0; i<EndCount; i++)
    Report+=EndsArray[i].DelEnd + "; " + EndsArray[i].AddEnd + "\r\n";

  return false;
}

/*
bool __fastcall MakeEndsFile(char*FileExt)
{ //-------------------------------------------
  //загружаем файл
  AnsiString FileName = "test_ends.";
  FileName+=FileExt;
  MForm->Status->Text=L"загружаем файл...";
  int handle=open(FileName.c_str(), O_RDONLY|O_BINARY);
  if(handle==-1)
  { MForm->Status->Text=L"файл не найден!";
    return true;
  }
  long size=filelength(handle);
  if(size<0||size>1000000)
  { MForm->Status->Text=L"не удалось определить размер файла!";
    return true;
  }
  char*buf=new char[size+1];
  if(read(handle,buf,size)==-1)
  { delete buf;
    MForm->Status->Text=L"чтение файла невозможно!";
    return true;
  }
  buf[size]=0;  //null terminator
  close(handle);
  TStringList*EndsList = new TStringList;
  EndsList->Text = buf;
  close(handle);
  delete buf;
  //----------------------------------------------------------
  //перенесём данные файла в таблицу
  struct
  { AnsiString DelEnd, AddEnd;   //удаляемое и прибавляемое окончание
    char Ret;                    //число откатываемых символов
    AnsiString TreeEnd, UniqEnd; //древесные и уникальные окончания
    unsigned char Code;
  }EndsArray[1000];
  int EndCount=0;
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
  //разделим удаляемое окончание на древесное и уникальное
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
  //определим размеры строк и расставим коды
  char UniqLen=0, AddLen=0;
  unsigned char CurCode=0;
  for(int i=0; i<EndCount; i++)
  { EndsArray[i].Code = 0xFF; //без кода
    int UniqL = EndsArray[i].UniqEnd.Length();
    int AddL  = EndsArray[i].AddEnd.Length();
    if(UniqL || AddL)
    { //строка пишется в файл
      if(UniqLen<UniqL) UniqLen=UniqL;
      if(AddLen <AddL ) AddLen =AddL;
      //возможно эти строки встречались ранее
      for(int j=0; j<i; j++)
        if
        ( EndsArray[j].Code   !=0xFF &&
          EndsArray[i].UniqEnd==EndsArray[j].UniqEnd &&
          EndsArray[i].AddEnd ==EndsArray[j].AddEnd  &&
          EndsArray[i].Ret    ==EndsArray[j].Ret
        )
        { //присвоим найденный код
          EndsArray[i].Code = EndsArray[j].Code;
          break;
        }

      if(EndsArray[i].Code==0xFF)
      { //назначим строке новый код
        EndsArray[i].Code=CurCode++;
        if(CurCode>64)
        { MForm->Status->Text=L"количество строк превышает 64!";
          return true;
        }
      }
    }
  }
  //----------------------------------------------------------
  //пишем строки в файл
  FileName = "StrArray.";
  FileName+=FileExt;
  int hStr=open(FileName.c_str(),O_WRONLY|O_CREAT|O_TRUNC|O_BINARY|O_APPEND,
                          S_IREAD|S_IWRITE );
  if(hStr==-1)
  { MForm->Status->Text="Не удалось открыть файл StrArray!";
    return true;
  }
  CurCode=0;
  write(hStr,&UniqLen,1); //пишем размер уникальных строк
  write(hStr,&AddLen,1);  //пишем размер добавляемых строк
  for(int i=0; i<EndCount; i++)
  { if(EndsArray[i].Code!=CurCode) continue;
    CurCode++;
    int UniqL = EndsArray[i].UniqEnd.Length();
    int AddL  = EndsArray[i].AddEnd.Length();
    //UniqEnd пишем с разворотом
    for(int s=UniqL; s>0; s--)
      write(hStr,&EndsArray[i].UniqEnd[s],1); //пишем UniqEnd
    for(int s=UniqL; s<UniqLen; s++)
      write(hStr,"\0",1); //дополним нулями
    //пишем откат
    write(hStr,&EndsArray[i].Ret,1);
    //пишем AddEnd
    write(hStr,EndsArray[i].AddEnd.c_str(),AddL);
    for(int s=AddL; s<AddLen; s++)
      write(hStr,"\0",1); //дополним нулями
  }
  close(hStr);
  //----------------------------------------------------------
  //соберём таблицы
  struct
  { AnsiString Addr;       //адрес таблицы
    unsigned char Tab[32]; //таблица
  }Tabs[128];              //адресация позволяет иметь не больше 128 таблиц
  int TabCount=1;
  //обнуляем таблицы
  for(int i=0; i<128; i++) memset(Tabs[i].Tab, 0, 32);
  //заполняем таблицы
  for(int i=0; i<EndCount; i++)
  { int TreeL = EndsArray[i].TreeEnd.Length();
    int t0=-1;
    for(int s=1; s<=TreeL; s++)
    { //найдём таблицу
      int t=0;
      if(s<TreeL)
      { //выделим адрес
        AnsiString Addr = EndsArray[i].TreeEnd.SubString(s+1,100);
        for(int j=1; j<TabCount; j++)
          if(Tabs[j].Addr==Addr){ t=j; break; } //нашли таблицу
        if(!t)
        { //добавляем таблицу
          t=TabCount++;
          if(TabCount>128)
          { MForm->Status->Text=L"количество таблиц превышает 128!";
            return true;
          }
          Tabs[t].Addr=Addr;
        }
      }
      //найдем ячейку таблицы
      int a=EndsArray[i].TreeEnd[s]-'а';
      if(a<0 || a>31)
      { MForm->Status->Text=L"недопустимый символ в окончании!";
        return true;
      }
      //заполним ячейку таблицы
      if(t0<0)
        if(Tabs[t].Tab[a]==0)
          Tabs[t].Tab[a] = 'x'; //конец ветви
        else
          Tabs[t].Tab[a] = 'X'; //накладка вышла...
      else Tabs[t].Tab[a] = t0;

      t0=t;
    }
  }
  //----------------------------------------------------------
  //запишем таблицы
  FileName = "TreeTabs.";
  FileName+=FileExt;
  int hTab=open(FileName.c_str(),O_WRONLY|O_CREAT|O_TRUNC|O_BINARY|O_APPEND,
                          S_IREAD|S_IWRITE );
  if(hTab==-1)
  { MForm->Status->Text="Не удалось открыть файл TreeTabs!";
    return true;
  }

  for(int t=0; t<TabCount; t++) write(hTab, Tabs[t].Tab, 32);

  close(hTab);
  //----------------------------------------------------------
  //покажем Report
  Report += " строк - ";
  Report += CurCode;
  Report += ", таблиц - ";
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
{ //обработаем файлы
  Report = "verb:";
  ErrorMake = MakeEndsFile("verb");
  if(ErrorMake) return;
  Report+= "\r\n\r\nnoun:";
  ErrorMake = MakeEndsFile("noun");
  if(ErrorMake) return;
  Status->Text="Файлы окончаний упакованы";
  //покажем Report
  InputTxt(Report,"Анализ файлов окончаний","Закрыть");
}
//==============================================================================
void __fastcall TMForm::MakeHashTab(TObject*Sender)
{ Status->Text="Собираем Hash-таблицу";
  //--------------------------------------------------------
  //получим количество Hash-кодов
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
  { Status->Text="не удалось получить количество Hash-кодов";
    ErrorMake=true;
    return;
  }
  mysql_free_result(Result);
  //--------------------------------------------------------
  //подготовим буфер
  unsigned short*Buf = new unsigned short[Hdr.HashCount*4];
  memset(Buf, 0xFF, sizeof(unsigned short)*Hdr.HashCount*4);
  //--------------------------------------------------------
  //заполним буфер
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
    { Status->Text="не заполнены Hash или Addr";
      ErrorMake=true;
      return;
    }
    Hash = AnsiString(row[0]).ToInt();
    Addr = AnsiString(row[1]).ToInt();
    if(Buf[i]!=Hash)
    { //новый хеш
      j = 1;
      if(Buf[i]!=0xFFFF) i+=4;
      if(i>=Hdr.HashCount*4)
      { Status->Text="переполнение Hash-таблицы";
        ErrorMake=true;
        return;
      }
      Buf[i] = Hash;
    }
    else
    { //продолжение строки
      j++;
      if(j>3)
      { Status->Text="переполнение строки Hash-таблицы";
        ErrorMake=true;
        return;
      }
    }
    Buf[i+j] = Addr;
  }
  mysql_free_result(Result);
  //--------------------------------------------------------
  //запишем таблицу в файл
  int hTab=open("hash_tab.tmp", O_WRONLY|O_CREAT|O_TRUNC|O_BINARY|O_APPEND,
                                S_IREAD|S_IWRITE );
  if(hTab==-1)
  { Status->Text="Не удалось открыть файл hash_tab.tmp";
    ErrorMake=true;
    return;
  }

  write(hTab,Buf,sizeof(unsigned short)*Hdr.HashCount*4);

  close(hTab);
  //--------------------------------------------------------
  delete Buf;
  Status->Text="Hash-таблица собрана";
}
//==============================================================================

