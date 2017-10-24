#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>
#pragma hdrstop
#include "MainForm.h"
#include "DBaction.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
void __fastcall TMForm::NewDatabase(TObject*Sender)
{ //создаём новую базу данных
  AnsiString DbName;
  if(InputStr(DbName,"Введите имя базы данных","Создать"))
  {
    AnsiString SQL="create database ";
    SQL+=DbName;
    SQL+=" character set = cp1251";
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    DBList->Items->Add(DbName);
    //подключаем созданную базу
    SQL="use ";
    SQL+=DbName;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    //запомним имя текущей базы
    CurrDB=DbName;

    //создаём необходимые таблицы
    //список слов
    if(mysql_query(&DB,
      "CREATE TABLE word("
      "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
      "name VARCHAR(30) NOT NULL UNIQUE KEY," //имя слова (уникальное)
      "main_form VARCHAR(30) ,"//основная форма
      "fkey VARCHAR(12),"      //ключ слова
      "defkey VARCHAR(12),"    //ключ слова по умолчанию
      "status INT,"      //состояние (заполнено, проверяется, в работе и т.п)
      "type VARCHAR(6)," //часть речи - существительное, глагол и т.п.
      "class_id INT,"    //класс слова - существительное, глагол и т.п.
      "cluster INT,"     //номер кластера
      "code TEXT,"       //код слова
      "obj_code TEXT,"   //код конструктора слова
      "glb_code TEXT,"   //глобальный код слова
      "KEY(main_form),"
      "KEY(cluster)"
      ") default charset=cp1251" //кодировка по умолчанию
    ))
    { Status->Text=mysql_error(&DB);
      return;
    }
    //список словоформ
    if(mysql_query(&DB,
      "CREATE TABLE form("
      "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY," //id словоформы
      "word_id INT NOT NULL,"     //id слова
      "form VARCHAR(30) NOT NULL,"//форма слова
      "search_form VARCHAR(30) NOT NULL,"//нормализованная для поиска форма слова
      "fkey VARCHAR(12) NOT NULL,"//ключ формы слова
      "word_cnt SMALLINT,"        //количество слов для этой формы
      "accentuation BOOLEAN,"     //=true - требуется ударение
      "UNIQUE KEY(word_id,fkey)," //уникальный индекс по id и ключу словоформы
      "KEY(search_form),"         //не уникальный индекс по словоформе
      "KEY(beg),"
      "KEY(end)"
      ") default charset=cp1251"  //кодировка по умолчанию
    ))
    { Status->Text=mysql_error(&DB);
      return;
    }
    //список значений слова
    if(mysql_query(&DB,
      "CREATE TABLE meaning("
      "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
      "word_id INT NOT NULL,"      //id слова
      "name VARCHAR(50) NOT NULL," //имя значения
      "type VARCHAR(30) NOT NULL," //тип значения
      "class_id INT,"              //класс значения
      "class2_id INT,"             //класс значения
      "class3_id INT,"             //класс значения
      "description TEXT,"          //описание
      "code TEXT,"                 //код объекта
      "hashtag VARCHAR(20),"       //поисковые тэги
      "UNIQUE KEY(word_id, name)"  //уникальный ключ
      ") default charset=cp1251"   //кодировка по умолчанию
    ))
    { Status->Text=mysql_error(&DB);
      return;
    }
    //синонимы
    if(mysql_query(&DB,
      "CREATE TABLE synonym("
      "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
      "object_id INT NOT NULL,"
      "word_id INT NOT NULL,"
      "add_name BOOLEAN NOT NULL," //дополнительное имя
      "UNIQUE KEY(object_id, word_id),"
      "KEY(object_id),"
      "KEY(word_id)"
      ") default charset=cp1251"
    ))
    { Status->Text=mysql_error(&DB);
      return;
    }
    //ассоциации
    if(mysql_query(&DB,
      "CREATE TABLE association("
      "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
      "object_id INT NOT NULL,"
      "type VARCHAR(3) NOT NULL," //тип значения
      "name VARCHAR(50) NOT NULL,"//имя значения
      "word_id INT,"              //ассоциация со словом
      "code TEXT,"                //код значения
      "comment TEXT,"             //комментарий
      "UNIQUE KEY(object_id, name),"
      "KEY(object_id),"
      "KEY(word_id)"
      ") default charset=cp1251"
    ))
    { Status->Text=mysql_error(&DB);
      return;
    }

    Status->Text="Ok, база данных создана.";
  }
}

//---------------------------------------------------------------------------
void __fastcall TMForm::SelectDatabase(TObject*Sender)
{ //выбор базы
  if(DBList->ItemIndex==-1) return;
  Status->Text=DBList->Items->operator[](DBList->ItemIndex);
  AnsiString SQL="use ";
  SQL+=DBList->Items->operator[](DBList->ItemIndex);
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  //проверка базы на наличие необходимых таблиц
  if(mysql_query(&DB,"show tables"))
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_RES*Result=mysql_use_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_ROW row;
  AnsiString Word="word";
  bool Ok=false;
  for(int r=0; bool(row=mysql_fetch_row(Result)); r++)
    if(Word==row[0])
    { Ok=true; break; }
  mysql_free_result(Result);

  if(!Ok)
  { Status->Text="База не содержит необходимых таблиц!";
    //отключаем базу
    if(!CurrDB.IsEmpty())
    { SQL="use ";
      SQL+=CurrDB;
      mysql_query(&DB,SQL.c_str());
//    DBList->Text=CurrDB;
    }
    return;
  }
  //установим текущую базу данных
  CurrDB=DBList->Items->operator[](DBList->ItemIndex);

  //покажем список слов
  if(RefreshWordList()) Status->Text="Ok!";
}
//---------------------------------------------------------------------------
//класс для запросов к MySQL
bool sql::ErrFlag=false;
__fastcall sql::sql(const AnsiString&TextSQL,bool&Flag)
{ //запрос к базе данных
  RES=0;
  Error=false;
  if(mysql_query(&DB, TextSQL.c_str()))
  { MForm->Status->Text=mysql_error(&DB);
    Flag=Error=true;
    return;
  }
  RES=mysql_store_result(&DB);
  if(!RES)
  { MForm->Status->Text=mysql_error(&DB);
    Flag=Error=true;
  }
}

__fastcall sql::~sql()
{ //освобождаем результат запроса
  mysql_free_result(RES);
}

bool __fastcall sql::Next(void)
{ //получаем следующую строку
  if(Error) return false;
  return (bool)(ROW=mysql_fetch_row(RES));
}

//---------------------------------------------------------------------------

