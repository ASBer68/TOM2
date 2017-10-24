#include <io.h>
#include <fcntl.h>
#pragma hdrstop
#include "MainForm.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// загрузчики из файлов различных словарей
//---------------------------------------------------------------------------
char* __fastcall TMForm::LoadFile(void)
{ //загружаем файл, возвращаем ссылку на буфер
  Status->Text="«агружаем файл";
  if(OpenDialog->Execute())
  { //откроем файл
    int handle=open(OpenDialog->FileName.c_str(),O_RDONLY|O_BINARY);
    if(handle==-1)
    { Status->Text="ќшибка открыти€ файла";
      return 0;
    };
    //получим размер файла
    long size=filelength(handle);
    if(size==-1||size>1000000000)
    { Status->Text="‘айл не загружен!";
      return 0;
    };
    char*buf=new char[size+1];
    //читаем файл
    if(read(handle,buf,size)==-1)
    if(size==-1||size>1000000000)
    { Status->Text="ќшибка чтени€ файла";
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
  Status->Text="√рузим слова из файла";
  //-----------------------------------------
  //разбор файла
  char*S=buf;
  AnsiString Word, MainForm, F, Type, SQL;
  int Field=0; //номер пол€
  StopFlag=false;
  while(*S&&!StopFlag)
  { switch(*S)
    { case 0: StopFlag=true;
      case'\n':case'\r': //конец строки
        Application->ProcessMessages(); //чтоб не вешалась
        if(!Word.IsEmpty())
        { Status->Text=Word;
          ProgressBar->Position=S-buf;
          //Sleep(10);
          //нормализуем основную форму
          MainForm = MainForm.LowerCase();
          //добавим слово
          AnsiString SQL="INSERT INTO word SET name = '";
          SQL+=Word + "', main_form = '";
          SQL+=MainForm + "', type = '";
          SQL+=Type+ "', status = 10";
          if(mysql_query(&DB,SQL.c_str()))
          { //Status->Text=mysql_error(&DB);
            //return;

            // что делать с ошибками?

          }


        }
        //обнулим переменные дл€ новой строки
        Word=""; MainForm=""; F=""; Type="";
        Field=0;
        break;

      case ';': case ',': Field++; break; //разделители полей
      case ' ': break; //игнорируем пробелы
      case 'Є':
        if(Field==2)
        { Word+="Є";
          MainForm+="е";
        }
        break; //игнорируем пробелы

      //составл€ем поле
      default: switch(Field)
      { case 0: F+=*S; break;    //встречаемость
        case 2: Word+=*S; MainForm+=*S; break; //слово
        case 3: Type+=*S; break; //часть речи
      }
    }
    S++;
  }
  //-----------------------------------------
  Status->Text="—лова загружены";
  ProgressBar->Position=0;
  delete buf;
}
//---------------------------------------------------------------------------
void __fastcall TMForm::LoadAAZaliznyak(TObject*Sender)
{ char*buf=LoadFile();
  if(!buf) return;
  Status->Text="«агружаем словоформы из файла";
  //-----------------------------------------
  //разбор файла
  char*S=buf;
  AnsiString Word, Id, Type, Form, Key, SQL;
  int Field=0, p; //номер пол€
  bool StrEnd=false;
  StopFlag=false;
  while(*S&&!StopFlag)
  { switch(*S)
    { case 0: StopFlag=true; //конец файла
      case'\n':case'\r': //конец строки
        StrEnd=true;
        if(!Id.IsEmpty())
        { //обновим статус слова
          SQL="UPDATE word SET status = 20 WHERE id = " + Id;
          mysql_query(&DB,SQL.c_str());
        }
      case',':       //конец пол€
        if
        ( !Id.IsEmpty() && !Form.IsEmpty() && //есть слово и форма
          (Field<=15 || Type!="verb") //дл€ глаголов берЄм только 15 форм
        )
        { //добавим форму
          Key=Field; //ключ по номеру пол€
          //подменим знак ударени€
          while((bool)(p=Form.Pos("'"))) Form[p]='`';
          //нормализуем форму
          AnsiString SForm = Form.LowerCase(); //нижний регистр
          while((bool)(p=SForm.Pos("Є"))) SForm[p]='е'; //Є -> е
          while((bool)(p=SForm.Pos("`"))) SForm=SForm.Delete(p,1);//без ударени€

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
        { //обнулим переменные дл€ новой строки
          StrEnd=false;
          Word=""; Id=""; Form="";
          Field=0;
        }
        break;

      case'#': //конец слова
        Field=1;
        Application->ProcessMessages(); //чтоб не вешалась
        ProgressBar->Position=S-buf;
        if(!Word.IsEmpty()) //есть слово
        { //приводим слово к стандартному виду
          Word = Word.LowerCase();
          int p;
          while((bool)(p=Word.Pos("Є"))) Word[p]='е'; //Є -> е
          //найдем слово, получим Id
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
          else Word=""; //нет такого слова
          mysql_free_result(Result);
        }
        break;

      case ' ': break; //игнорируем пробелы

      //составл€ем поле
      default:
        if(Field) Form+=*S;
        else Word+=*S;
    }
    S++;
  }
  //-----------------------------------------
  Status->Text="«агрузка завершена";
  ProgressBar->Position=0;
  delete buf;
}

//---------------------------------------------------------------------------
char*HagenNoun[][2]=
{ " ед",  "≈ч",
  " мн",  "ћч",
  " им",  "»п",
  " род", "–п",
  " дат", "ƒп",
  " вин", "¬п",
  " тв",  "“п",
  " пр",  "ѕп",
  0,0
};
char*HagenVerb[][2]=
{ " инф", "»вѕф",
  " пов", "Ќв2лѕф",
  " прош","ѕв",
  " наст","Ќв",
  " буд", "Ѕв",
  " муж", "ћр",
  " жен", "∆р",
  " ср",  "—р",
  " 1-е", "1л",
  " 2-е", "2л",
  " 3-е", "3л",
  " ед",  "≈ч“ы",
  " мн",  "ћч",
  0,0
};
char*HagenAdj[][2]= //прилагательные
{ " ед",  "≈ч",
  " мн",  "ћч",
  " им",  "»п",
  " род", "–п",
  " дат", "ƒп",
  " вин", "¬п",
  " тв",  "“п",
  " пр",  "ѕп",
  " муж", "ћр",
  " жен", "∆р",
  " ср",  "—р",
  " крат","»п ф",
  0,0
};
char*HagenPart[][2]= //причасти€
{ " 1вид",  "1в",
  " 2вид",  "2в",
  " перех", "ѕр",
  " страд", "—т",
  " воз",   "¬з",
  " несов", "Ќш",
  " сов",   "—ш",
  " прош",  "ѕв",
  " наст",  "Ќв",
  " буд",   "Ѕв",

  " ед",  "≈ч",
  " мн",  "ћч",
  " им",  "»п",
  " род", "–п",
  " дат", "ƒп",
  " вин", "¬п",
  " тв",  "“п",
  " пр",  "ѕп",
  " муж", "ћр",
  " жен", "∆р",
  " ср",  "—р",
  " крат","»п ф",
  0,0
};


void __fastcall TMForm::LoadHagen(TObject*Sender)
{ char*buf=LoadFile();
  if(!buf) return;
  Status->Text="«агружаем словоформы из файла";
  //-----------------------------------------
  //разбор файла
  AnsiString SQL, Word, Id, Type, Key, DefKey;
  AnsiString FormRp; //форма родительного падежа
  char** HagenMorph;
  char*Str=buf;//начало поиска
  char*Sign;
  int L=0;
  StopFlag=false;
  while(Str[0]&&!StopFlag)
  {
    Str+=L; //двигаем строку
    if(Str[0]=='*') Str++;

    char*N=strchr(Str,'\n');//перевод строки
    char*R=strchr(Str,'\r');//возврат каретки
    if(!N && !R)
      L=strlen(Str); //длина последней строки
    else
    { if(R&&R<N) N=R;
      L=N-Str+2; //длина строки
    }

    if(L<=5)
    { //это разделитель слов
      Id = "";
      continue;
    }

    if(N) N[0] = 0;

    //обрезаем по табул€цию
    char*Tab=strchr(Str,'\t'); //после первого tab идЄт описание слова
    if(Tab)
    { Tab[0] = 0;
      Tab++;
    }
    //приводим слово к стандартному виду
    Word = Str;
    Word = Word.LowerCase();
    int p;
    while((bool)(p=Word.Pos("Є"))) Word[p]='е'; //Є -> е

    bool FirstForm = false;
    if(strstr(Tab,"сущ ") && strstr(Tab," ед ") && strstr(Tab," им"))
    { //строка содержит начало существительного
      FirstForm = true;
      Sign      = "сущ ";
      Type      = "noun";
      HagenMorph= &HagenNoun[0][0];
      DefKey    = "»п≈ч";
      //соберЄм ключ слова
      Key="";
      if     (strstr(Tab," муж ")) Key = "ћр";
      else if(strstr(Tab," жен ")) Key = "∆р";
      else if(strstr(Tab," ср "))  Key = "—р";
      if     (strstr(Tab," одуш "))Key+= "ќд";
      else if(strstr(Tab," неод "))Key+= "Ќд";
    }
    else if(strstr(Tab,"гл ") && strstr(Tab," инф"))
    { //строка содержит начало глагола
      FirstForm = true;
      Sign      = "гл ";
      Type      = "verb";
      HagenMorph= &HagenVerb[0][0];
    }
    else if
    ( Id.IsEmpty() && strstr(Tab,"прч ") &&
      ( strstr(Tab," ед муж им") ||
        strstr(Tab,"крат ") && strstr(Tab," ед муж")
      )
    )
    { //строка содержит начало причасти€
      FirstForm = true;
      Sign      = "прч ";
      Type      = "part";
      HagenMorph= &HagenPart[0][0];
      Key       = "";
      DefKey    = "—р»п≈чЌд";
    }

    if(FirstForm)
    { //двигаем прогресс
      ProgressBar->Position=Str-buf;
      Application->ProcessMessages();
      //найдем слово, получим Id
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
      else Id=""; //нет такого слова
      mysql_free_result(Result);

      if(Id.IsEmpty()) continue; //нет загружаемого слова

      if(Type=="verb")
      { //запрашиваем совершЄнность
        AnsiString Q="будет ";
        Q+=Word;
        Q+=" - это правильно?";
        switch
        ( Application->MessageBoxA( Q.c_str(),"—овершЄнность глагола",
                                    MB_YESNOCANCEL|MB_ICONQUESTION)
        )
        { case IDYES: //несовершенное
            Key = "Ќш";
            DefKey = "Ќвћр3л≈ч";
            break;
          case IDNO: //совершенное
            Key = "—ш";
            DefKey = "ѕвћр3л≈ч";
            break;
          default:
            Id = "";
            continue; //пропускаем глагол
        }
      }

      //обновим статус слова
      SQL="UPDATE word SET status = 25, fkey = '";
      SQL+=Key + "', defkey = '";
      SQL+=DefKey + "' WHERE id = ";
      SQL+=Id;
      mysql_query(&DB,SQL.c_str());
      //обновим статус загрузки
      Status->Text=Word;
      Application->ProcessMessages();
    }

    if(Id.IsEmpty()) continue;      //нет загружаемого слова
    if(!strstr(Tab,Sign))
    { Id=="";
      continue; //сигнатура не соответствует типу слова
    }

    //соберем ключ текущей формы
    Key="";
    for(int i=0; HagenMorph[i]; i+=2)
      if(strstr(Tab,HagenMorph[i]))
        Key+=HagenMorph[i+1];
    //2-€ форма тв.падежа
    if(strstr(Tab," ед жен тв") && Word.SubString(Word.Length()-1,2)=="ою")
      Key+="ѕс";
    //одушевлЄнность вн.падежа
    if((Type=="adj"||Type=="part") && (strstr(Tab," муж")||strstr(Tab," мн")))
      if(strstr(Tab," род")) FormRp = Word; //запомним род.падеж
      else if(strstr(Tab," вин")) //определим одушевлЄнность винительного
        Key+=(Word==FormRp)?"ќд":"Ќд";


    //добавим форму
    SQL="INSERT INTO form SET word_id = ";
    SQL+=Id + ", form = '";
    SQL+=Str;
    SQL+="', search_form = '";
    SQL+=Word+ "', fkey = '";
    SQL+=Key + "'";
    mysql_query(&DB,SQL.c_str());

  }

  Status->Text="«агрузка завершена";
  ProgressBar->Position=0;
  delete buf;
}

//---------------------------------------------------------------------------
//загрузка базы из текстового файла
void __fastcall TMForm::LoadText(TObject*Sender)
{ char*buf=LoadFile();
  if(!buf) return;
  Status->Text="√рузим слова из файла";
  //-----------------------------------------
  //разбор файла
  char*S=buf;
  StopFlag=false;
/*
  while(!StopFlag)
  { if(


  }
*/
  //-----------------------------------------
  Status->Text="—лова загружены";
  ProgressBar->Position=0;
  delete buf;
}
//---------------------------------------------------------------------------

