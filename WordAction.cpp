#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>
#pragma hdrstop
#include "MainForm.h"
#pragma package(smart_init)
//===========================================================================
// отобразим текущее слово
//--------------------------------------------------
bool __fastcall TMForm::RefreshCurrWord(void)
{ //всё что нужно знать о слове
  AssocList->Items->Clear();
  AnsiString SQL=
    "SELECT word.id, word.cluster, "           // 0, 1
    "  word.type, meaning.name, word.status, " // 2, 3, 4
    "  word.name, word.fkey, word.defkey, "    // 5, 6, 7
    "  word.obj_code, word.glb_code "          // 8, 9
    "FROM word "
    "LEFT JOIN word AS clust ON clust.id = word.cluster "
    "LEFT JOIN meaning ON meaning.id = word.class_id "
    "WHERE word.name = '";
  SQL+=CurrWord;
  SQL+="'";
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return false;
  }
  MYSQL_RES*Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return false;
  }
  AnsiString ClusterId, Text;
  bool Prep;
  MYSQL_ROW row;
  if(bool(row=mysql_fetch_row(Result)))
  { //id
    WordId = row[0];
    ClusterId= row[1];
    //кнопки
    Prep = strcmp(row[2],"prep") == 0;
    WordTypeButton->Caption = (row[2])?row[2]:"NULL";
    WordClassButton->Caption= (row[3])?row[3]:"Класс...";
    StatusButton->Caption=AnsiString("Status = ") + row[4];
    //текст
    Text = "$";
    Text+= row[5];
    Text+= "\r\n\r\nkey = #";
    Text+= row[6];
    Text+= "\r\ndefkey = #";
    Text+= row[7];
    Text+= "\r\n";
    //код
    ObjCode ->Text = row[8];
    GlobCode->Text = row[9];
  }
  else
  { Status->Text="Слово не найдено!";
    return false;
  }
  mysql_free_result(Result);
  //--------------------------------------------------
  //отобразим словоформы
  SQL="SELECT fkey, form FROM form WHERE fkey<>'-' AND word_id = ";
  SQL+=WordId;
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return false;
  }
  Result=mysql_use_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return false;
  }
  while(bool(row=mysql_fetch_row(Result)))
  { if(Prep)
    { //предлог
      Text+="\r\n";
      Text+=row[1];
      Text+=" ";
      Text+=row[0];
    }
    else if(row[0] && row[1])
    { //слово
      Text+="\r\n#";
      Text+=row[0];
      Text+=" = ";
      Text+=row[1];
    }
  }
  mysql_free_result(Result);
  //--------------------------------------------------
  //отобразим кластер слова
  if(!ClusterId.IsEmpty())
  { SQL="SELECT name FROM word WHERE cluster = ";
    SQL+=ClusterId;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return false;
    }
    Result=mysql_use_result(&DB);
    if(!Result)
    { Status->Text=mysql_error(&DB);
      return false;
    }
    Text+="\r\n\r\nКластер слова:\r\n";
    while(bool(row=mysql_fetch_row(Result)))
    { Text+=row[0];
      Text+="\r\n";
    }
    mysql_free_result(Result);
  }
  //--------------------------------------------------
  //отобразим зависимые объекты
  SQL="SELECT meaning.type, meaning.name "
      "FROM association "
      "JOIN meaning ON meaning.id = object_id "
      "WHERE association.word_id = ";
  SQL+=WordId;
  SQL+=" GROUP BY meaning.id";
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return false;
  }
  Result=mysql_use_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return false;
  }
  bool H=true;
  while(bool(row=mysql_fetch_row(Result)))
  { if(H)
    { H = false;
      Text+="\r\nПри загрузке модифицирует объекты:";
    }
    Text+="\r\n";
    Text+=row[0];
    Text+=" ";
    Text+=row[1];
  }
  mysql_free_result(Result);
  //--------------------------------------------------
  WordCode->Text = Text;
  //--------------------------------------------------
  return true;
}

//---------------------------------------------------------------------------
//транскрипция слова (условная)
//нужна для более оптимального распределения начал и окончаний
AnsiString __fastcall GetTranscription(const char*Form)
{ AnsiString Res;
  bool PrGl=true; //начало слова эквивалентно предыдущему гласному
  bool PrSf=false;//предыдущий "мягкий" гласный
  for(const char*s=Form; *s; s++)
  { switch(*s)
    { case '`': break; //игнорируем ударение
      case 'я': Res+=(PrGl?"йа":(PrSf?"а":"ьа")); break;
      case 'ю': Res+=(PrGl?"йу":(PrSf?"у":"ьу")); break;
      case 'ё'://Res+=(PrGl?"йо":(PrSf?"о":"ьо"));break; как е (компромис)
      case 'е': Res+=(PrGl?"йе":(PrSf?"е":"ье")); break;
      case 'и': Res+=(PrGl?"йи":(PrSf?"и":"ьи")); break;
      case 'ы': Res+='и'; break;
      case 'ь': Res+=(PrGl?"?":(PrSf?"":"ь")); break;
      case 'ъ': break; //никогда не нужен
      default:  Res+=*s;  break;
    }
    PrGl=(bool) strchr("аоуеёиыэюяьъ",*s);
    PrSf=(bool) strchr("шщжцчх",*s);
  }
  //мягкие согласные заменяем латинскими буквами
  int Pos;
  while((bool)(Pos=Res.Pos("ь")))
  { Res=Res.Delete(Pos,1);
    Pos--;
    switch(Res[Pos])
    { case 'б': Res[Pos]='b'; break;
      case 'в': Res[Pos]='v'; break;
      case 'г': Res[Pos]='g'; break;
      case 'д': Res[Pos]='d'; break;
      case 'з': Res[Pos]='z'; break;
      case 'к': Res[Pos]='k'; break;
      case 'л': Res[Pos]='l'; break;
      case 'м': Res[Pos]='m'; break;
      case 'н': Res[Pos]='n'; break;
      case 'п': Res[Pos]='p'; break;
      case 'р': Res[Pos]='r'; break;
      case 'с': Res[Pos]='s'; break;
      case 'т': Res[Pos]='t'; break;
    }
  }
  return Res;
}

//---------------------------------------------------------------------------
void __fastcall TMForm::TestTranscription(TObject*Sender)
{ AnsiString WordForm;
  if(InputStr(WordForm,"Введите слово","Транскрипция"))
  {
    Status->Text=GetTranscription(WordForm.c_str());
  }
}

void __fastcall TMForm::SetTranscription(TObject*Sender)
{ Status->Text="транскрипция форм";
  if(mysql_query(&DB,"SELECT id, form FROM form"))
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_RES*Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_ROW row;
  AnsiString SQL;
  while(bool(row=mysql_fetch_row(Result)))
  { Status->Text=row[1];
    Application->ProcessMessages();

    SQL="UPDATE form SET search_form = '";
    SQL+=GetTranscription(row[1]);
    SQL+="' WHERE id = ";
    SQL+=row[0];
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      mysql_free_result(Result);
      return;
    }
  }
  mysql_free_result(Result);
  Status->Text="транскрипция завершена";
}

//---------------------------------------------------------------------------
void __fastcall TMForm::SetTraditional(TObject*Sender)
{ //заполняем все search_form на основании форм слов
  Status->Text="обработка форм";
  if(mysql_query(&DB,"SELECT id, form FROM form"))
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_RES*Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  int p;
  AnsiString SQL, Form;
  MYSQL_ROW row;
  while(bool(row=mysql_fetch_row(Result)))
  { Status->Text=row[1];
    Application->ProcessMessages();
    Form = row[1];
    Form = Form.LowerCase();
    while((bool)(p=Form.Pos("ё"))) Form[p]='е'; //ё->е
    while((bool)(p=Form.Pos("`"))) Form = Form.Delete(p,1); //убираем ударения

    SQL="UPDATE form SET search_form = '";
    SQL+=Form;
    SQL+="' WHERE id = ";
    SQL+=row[0];
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      mysql_free_result(Result);
      return;
    }
  }
  mysql_free_result(Result);
  Status->Text="обработка завершена";
}

//---------------------------------------------------------------------------
const char*SuffixList[]=   //список суффиксов с беглой гласной
{ "ек", "ел", "ен", "ец", "ол", "он", "ок", "ьк", "ьн", 0 };

const char*VerbSuffixList[][2]= //список суффиксов для глаголов
{ "ачн", "ач",   //начну - начал
  "бер", "бр",   //беру - брал
  "беж", "бег",  //бежал - бегу
  "буж", "буд",  //бужу - будить
  "вед", "в",    //веду - вел
  "гон", "гн",   //гоните - гнал
  "ден", "дел",  //надену - надел
  "дер", "др",   //деру - драл
  "зов", "зв",   //зову - звал
  "кля", "клян", //клялся - клянусь
  "ляг", "лег",  //лягла - лег
  "меч", "мет",  //мечу - метать
  "ним", "н",    //отниму - отнял, обниму - обнял
  "пиш", "пис",  //пишу - писал
  "тзв", "тозв", //отзову - отозвал
  "тын", "т",
  "тыщ", "тыск", //отыщет - отыскал
  "чев", "ч",    //ночевал - ночую
  "чуч", "чут",  //очучусь - очутился

  "ажм", "аж",   //нажму - нажал (3 буквы не подтверждаются!)
  "ожм", "ож",   //пожму - пожал
  "ижм", "иж",   //прижму - прижал
  "ужм", "уж",   //ужму - ужал
  "ежм", "еж",   //пережму - пережал

  0
};

void __fastcall TMForm::FormWithoutEnds(char*FName)
{ //подготовка форм слов к хешированию (без окончаний)
  Status->Text="обработка форм";
  //грузим файлы окончаний
  TStringList*NounList = new TStringList;
  TStringList*VerbList = new TStringList;
  //грузим файл существительных и прилагательных
  AnsiString FileName = FName;
  FileName += ".noun";
  int handle=open(FileName.c_str(), O_RDONLY|O_BINARY);
  if(handle==-1)
  { Status->Text=L"файл .noun не найден!";
    return;
  }
  long size=filelength(handle);
  if(size<0||size>1000000)
  { Status->Text=L"не удалось определить размер файла .noun!";
    delete NounList;
    delete VerbList;
    return;
  }
  char*buf=new char[size+1];
  if(read(handle,buf,size)==-1)
  { delete buf;
    delete NounList;
    delete VerbList;
    Status->Text=L"чтение файла .noun невозможно!";
    return;
  }
  buf[size]=0;  //null terminator
  NounList->Text = buf;
  close(handle);
  delete buf;
  //грузим файл окончаний глаголов
  FileName = FName;
  FileName += ".verb";
  handle=open(FileName.c_str(), O_RDONLY|O_BINARY);
  if(handle==-1)
  { Status->Text=L"файл .verb не найден!";
    delete NounList;
    delete VerbList;
    return;
  }
  size=filelength(handle);
  if(size<0||size>1000000)
  { Status->Text=L"не удалось определить размер файла .verb!";
    delete NounList;
    delete VerbList;
    return;
  }
  buf=new char[size+1];
  if(read(handle,buf,size)==-1)
  { delete buf;
    delete NounList;
    delete VerbList;
    Status->Text=L"чтение файла .verb невозможно!";
    return;
  }
  buf[size]=0;  //null terminator
  VerbList->Text = buf;
  close(handle);
  delete buf;

  //обработаем все формы слова
  if
  ( mysql_query(&DB, "SELECT form.id, search_form, type "
                     "FROM form "
                     "JOIN word ON word_id = word.id")
  )
  { Status->Text=mysql_error(&DB);
    delete NounList;
    delete VerbList;
    return;
  }
  MYSQL_RES*Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    delete NounList;
    delete VerbList;
    return;
  }
  int p;
  AnsiString SQL, Form, End, Type;
  MYSQL_ROW row;
  while(bool(row=mysql_fetch_row(Result)))
  { Status->Text=row[1];
    Application->ProcessMessages();
    Form = row[1];
    Type = row[2];
    End = "";
    //ограничение на длину основания
    int MinBas = 3;
    if(Form.SubString(1,3)=="стр") MinBas = 4;
/*
    if(Form.SubString(1,3)=="осн") MinBas = 4;
    if(Form.SubString(1,4)=="волн")MinBas = 5;
*/
    if(Type=="verb")
    { //глагол может быть возвратным
      int l=Form.Length();
      bool Reflexive = l>2 && Form[l-1]=='с' && (Form[l]=='я' || Form[l]=='ь');
      AnsiString BaseForm = Form;
      if(Reflexive) Form = Form.Delete(l-1,2); //без ся
      //убираем окончание глагола
      for(int i=0; i<VerbList->Count; i++)
      { AnsiString DelEnd, AddEnd;
        int p=VerbList->Strings[i].Pos("\t"); //разделитель строки
        if(p)
        { DelEnd = VerbList->Strings[i].SubString(1,p-1);
          AddEnd = VerbList->Strings[i].SubString(p+1,1000);
        }
        else DelEnd = VerbList->Strings[i];

        int l = DelEnd.Length();
        int f = Form.Length() - l + 1;
        if(f+AddEnd.Length() > MinBas && Form.SubString(f,l)==DelEnd)
        { Form = Form.Delete(f,l) + AddEnd; //меняем окончание
          End = DelEnd;
          break;
        }
        if(Reflexive && DelEnd!="ь" && DelEnd!="я")
        { //возможно окончание подойдет к базовой форме
          f = BaseForm.Length() - l + 1;
          if(f+AddEnd.Length() > MinBas && BaseForm.SubString(f,l)==DelEnd)
          { Form = BaseForm.Delete(f,l) + AddEnd; //меняем окончание
            End = DelEnd;
            Reflexive = false;
            break;
          }
        }
      }
      //обработка суффиксов
      int f = Form.Length()-2;
      for(int i=0; f>0 && VerbSuffixList[i][0]; i++)
      { AnsiString Suf = Form.SubString(f,3);
        if(Suf==VerbSuffixList[i][0])
        { Form = Form.Delete(f,3) + VerbSuffixList[i][1];
          f = Form.Length()-2;
        }
      }
      //возвратный глагол
      if(Reflexive) Form += "с";
    }
    else
    { //убираем окончание не глаголов
      for(int i=0; i<NounList->Count; i++)
      { int l = NounList->Strings[i].Length();
        int f = Form.Length() - l + 1;
        if(f > MinBas && Form.SubString(f,l)==NounList->Strings[i])
        { Form = Form.Delete(f,l); //убираем окончание
          End = NounList->Strings[i];
          break;
        }
      }
      //убираем повторяющиеся буквы
      AnsiString Form2=Form.SubString(1,1);
      for(int i=2; i<=Form.Length(); i++)
        if(Form2[Form2.Length()] != Form[i]) Form2+=Form[i];
      Form=Form2;
      //убираем нен
      int L=Form.Length();
      if(L>6 && Form.SubString(L-2,4)=="нен") //утренен, ранен
      { Form = Form.SubString(1,L-2);
        L-=2;
      }
      //убираем беглую гласную в суфиксе
      if(L>5)
      { bool Suffix = false;
        for(int i=0; !Suffix && SuffixList[i]; i++)
          Suffix = Form.SubString(L-1,2)==SuffixList[i];
        if(Suffix && strchr("вгджзклмнпрстшч",Form[L-2])) Form=Form.Delete(L-1,1);
      }
      //убираем повторяющиеся буквы
      Form2=Form.SubString(1,1);
      for(int i=2; i<=Form.Length(); i++)
        if(Form2[Form2.Length()] != Form[i]) Form2+=Form[i];
      Form=Form2;
    }

    //считаем хеш
    unsigned int Hash32 = 0x67452301;
    if(Type=="verb") Hash32 = 0xEFCDAB89;
    for(int p=1; p<=Form.Length(); p++)
    { unsigned int HBit = Hash32 >> 27; //старшие биты сдвигаем в младшие
      Hash32 = (Hash32 << 5) | HBit;
      Hash32 ^= Form[p] - 'а'; //добавляем текущий символ
    }

    unsigned short Hash16 = 0x6745;
    if(Type=="verb") Hash16 = 0xAB89;
    for(int p=1; p<=Form.Length(); p++)
    {
      //unsigned short HBit = Hash16 >> 11; //старшие биты сдвигаем в младшие
      //Hash16 = (Hash16 << 5) | HBit;
      //Hash16 ^= Form[p] - 'а'; //добавляем текущий символ
      unsigned short Ch = (Form[p] - 'а')^031;
      unsigned short HBit;
      if(Ch&0x16)
      { HBit = Hash16 >> 11; //старшие биты сдвигаем в младшие
        Hash16 = (Hash16 << 5) | HBit;
      }
      else if(Ch&0x8)
      { HBit = Hash16 >> 12; //старшие биты сдвигаем в младшие
        Hash16 = (Hash16 << 4) | HBit;
      }
      else if(Ch&0x4)
      { HBit = Hash16 >> 13; //старшие биты сдвигаем в младшие
        Hash16 = (Hash16 << 3) | HBit;
      }
      else
      { HBit = Hash16 >> 14; //старшие биты сдвигаем в младшие
        Hash16 = (Hash16 << 2) | HBit;
      }
      Hash16 ^= Ch; //добавляем текущий символ
    }

    SQL="UPDATE form SET work_form = '";
    SQL+=Form;
    SQL+="', work_end = '";
    SQL+=End;
    SQL+="', hash32 = ";
    SQL+=Hash32;
    SQL+=", hash16 = ";
    SQL+=Hash16;
    SQL+=" WHERE id = ";
    SQL+=row[0];
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      mysql_free_result(Result);
      delete NounList;
      delete VerbList;
      return;
    }
  }
  mysql_free_result(Result);
  delete NounList;
  delete VerbList;

  Status->Text="обработка завершена";
}

void __fastcall TMForm::SetWithoutEnd(TObject*Sender)
{ //подготовка форм слов к хешированию (без окончаний)
  FormWithoutEnds("test_ends");
}


//---------------------------------------------------------------------------
//пишем слово в файл
bool __fastcall TMForm::WordToFile(const AnsiString&Id, int FileH)
{
  if(FileH==-1)
  { Status->Text="файл не открыт";
    return false; //лог не открыт
  }
  //возьмем код слова из базы
  AnsiString SQL="SELECT code FROM word WHERE id = ";
  SQL+=Id;
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return false;
  }
  MYSQL_RES*Result=mysql_use_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return false;
  }
  MYSQL_ROW row;
  bool Ok=true;
  if(bool(row=mysql_fetch_row(Result)))
  { if(row[0] && row[0][0]) //есть непустой код
    { write(FileH,"\n",1);
      write(FileH,row[0],StrLen(row[0]));
      write(FileH,"\n",1);
    }
    else
    { Ok=false;
      Status->Text="Для слова не сформирован код!";
    }
  }
  else
  { Ok=false;
    Status->Text="Слово не найдено!";
  }
  mysql_free_result(Result);
  return Ok;
}
//===========================================================================
AnsiString WordName;
void __fastcall TMForm::NewWord(TObject *Sender)
{ //добавим новое слово
  WordName="";
  AnsiString MainForm, Type="noun";
  Status->Text="Ожидаем ввода слова...";
  if(InputStr(WordName,"Введите слово","Добавить"))
  { //проверим существование слова
    AnsiString SQL="SELECT name FROM word WHERE name = '";
    SQL+=WordName + "'";
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
    bool Ex = (row=mysql_fetch_row(Result));
    mysql_free_result(Result);
    if(Ex)
    { Status->Text = "Слово существует";
      return;
    }
    //запросим основную форму
    MainForm = WordName;
    InputStr(MainForm,"Введите основную форму",0);
    //нормализуем основную форму
    MainForm = MainForm.LowerCase();
    int Pos;
    while((bool)(Pos=MainForm.Pos("ё"))) MainForm[Pos]='е'; //ё -> е
    //запросим тип
    InputStr(Type,"Введите тип слова",0);
    //добавим слово
    SQL="INSERT INTO word SET name = '";
    SQL+=WordName + "', main_form = '";
    SQL+=MainForm + "', type = '";
    SQL+=Type + "', status = 1";
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      WordName="";
      return;
    }

    RefreshWordList();
    Status->Text="Ok, слово добавлено";
  }
  else Status->Text="";
}
//---------------------------------------------------------------------------
//редактируем слово
void __fastcall TMForm::EditWord(TObject*Sender)
{ if(WordId.IsEmpty())
  { Status->Text="не выбрано слово!";
    return;
  }
  AnsiString WordName, MainForm;
  Status->Text="Ожидаем ввода слова...";
  if(InputStr(WordName,"Введите слово","Изменить"))
  { //запросим основную форму
    MainForm = WordName;
    InputStr(MainForm,"Введите основную форму","Изменить");
    //нормализуем основную форму
    MainForm = MainForm.LowerCase();
    int Pos;
    while((bool)(Pos=MainForm.Pos("ё"))) MainForm[Pos]='е'; //ё -> е
    //изменим слово
    AnsiString SQL="UPDATE word SET name = '";
    SQL+=WordName + "', main_form = '";
    SQL+=MainForm + "' WHERE id = " + WordId;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    CurrWord = WordName;
    RefreshCaption();
    RefreshCurrWord();
    Status->Text="Ok, слово изменено";
  }
  else Status->Text="";
}
//---------------------------------------------------------------------------
/*
void __fastcall TMForm::SaveWord(TObject*Sender)
{ if(WordId.IsEmpty())
  { Status->Text="не выбрано слово!";
    return;
  }
  Status->Text="Сохраняем слово в файл...";
  SaveDialog->FileName=CurrWord->Caption+".word";
  if(SaveDialog->Execute())
  { int handle=open(SaveDialog->FileName.c_str(),O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
    if(WordToFile(WordId,handle)) Status->Text="Cлово сохранено";
    close(handle);
  }
  else Status->Text="";
}
*/
//---------------------------------------------------------------------------
/*
void __fastcall TMForm::SelectWord(TObject *Sender)
{
  //клик на списке слов
  int Item=-1;
  for(int i=0; i<WordList->Items->Count; i++)
    if(WordList->Selected[i]) Item=i;
  if(Item==-1) return;
  //найдено слово
  CurrWord = WordList->Items->operator[](Item);
  if(RefreshCurrWord())
    Status->Text=CurrWord;
  //--------------------------------------------------
  //отобразим значения слова
  RefreshMeaningList();
  //--------------------------------------------------
  RefreshCaption();
}
*/
//---------------------------------------------------------------------------
void __fastcall TMForm::EditKey(TObject *Sender)
{ if(WordId.IsEmpty())
  { Status->Text="не выбрано слово!";
    return;
  }
  //---------------------------------------------------
  AnsiString SQL="SELECT fkey FROM word WHERE id = ";
  SQL+=WordId;
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_RES*Result=mysql_use_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  AnsiString Key;
  MYSQL_ROW row;
  if(bool(row=mysql_fetch_row(Result))) Key=row[0];
  mysql_free_result(Result);
  //---------------------------------------------------
  if(InputStr(Key,"Введите ключ","Сохранить"))
  { SQL="UPDATE word SET fkey = '";
    SQL+=Key + "' WHERE id = " + WordId;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    RefreshCurrWord();
    Status->Text="Ok, ключ сохранен";
  }
}

//---------------------------------------------------------------------------
void __fastcall TMForm::EditDefKey(TObject *Sender)
{ if(WordId.IsEmpty())
  { Status->Text="не выбрано слово!";
    return;
  }
  //---------------------------------------------------
  AnsiString SQL="SELECT defkey FROM word WHERE id = ";
  SQL+=WordId;
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_RES*Result=mysql_use_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  AnsiString Key;
  MYSQL_ROW row;
  if(bool(row=mysql_fetch_row(Result))) Key=row[0];
  mysql_free_result(Result);
  //---------------------------------------------------
  if(InputStr( Key,"Введите ключ по умолчанию","Сохранить"))
  { SQL="UPDATE word SET defkey = '";
    SQL+=Key + "' WHERE id = " + WordId;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    RefreshCurrWord();
    Status->Text="Ok, ключ сохранен";
  }
}
//---------------------------------------------------------------------------
void __fastcall TMForm::SetStatus(TObject*Sender)
{ if(WordId.IsEmpty())
  { Status->Text="не выбрано слово!";
    return;
  }
  AnsiString Stat;
  if(InputStr( Stat,"Введите статус","Установить"))
  { AnsiString SQL="UPDATE word SET status = '";
    SQL+=Stat + "' WHERE id = " + WordId;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    if(RefreshCurrWord()) //обновим вид слова
      Status->Text="Ok, статус установлен";
  }
}

//---------------------------------------------------------------------------
void __fastcall TMForm::NewForm(TObject*Sender)
{ Status->Text="Ожидается ввод словоформы...";
  if(WordId.IsEmpty())
  { Status->Text="не выбрано слово!";
    return;
  }
  //добавим форму
  AnsiString Key, Form;
  if(InputForm(Key,Form,"Добавить"))
  { //нормализуем форму
    int Pos;
    AnsiString SForm = Form.LowerCase(); //нижний регистр
    while((bool)(Pos=SForm.Pos("ё"))) SForm[Pos]='е'; //ё -> е
    while((bool)(Pos=SForm.Pos("`"))) SForm=SForm.Delete(Pos,1);//без ударения

    AnsiString SQL="INSERT INTO form SET word_id = ";
    SQL+=WordId + ", form = '";
    SQL+=Form + "', search_form = '";
    SQL+=SForm+ "', fkey = '";
    SQL+=Key  + "'";
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    RefreshCurrWord();
    Status->Text="Ok, форма добавлена";
  }
  else Status->Text="";
}
//---------------------------------------------------------------------------
// редактируем существующую форму
void __fastcall TMForm::EditForm(TObject*Sender)
{ if(WordId.IsEmpty())
  { Status->Text="не выбрано слово!";
    return;
  }
  Status->Text="";
  AnsiString Key;
  if(!InputStr( Key,"Введите ключ изменяемой формы","Редактировать"))
    return;
  //получим словоформу по ключу
  AnsiString Id, Form;
  AnsiString SQL="SELECT id, form FROM form WHERE word_id = ";
  SQL+=WordId + " and fkey = '";
  SQL+=Key  + "'";
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
  { Id=row[0];
    Form=row[1];
  }
  mysql_free_result(Result);

  if(Form.IsEmpty())
  { Status->Text="нет словоформы с таким ключом!";
    return;
  }
  //редактируем словоформу и ключ
  AnsiString NewKey = Key;
  if(InputForm(NewKey,Form,"Записать"))
  { //нормализуем форму
    int Pos;
    AnsiString SForm = Form.LowerCase(); //нижний регистр
    while((bool)(Pos=SForm.Pos("ё"))) SForm[Pos]='е'; //ё -> е
    while((bool)(Pos=SForm.Pos("`"))) SForm=SForm.Delete(Pos,1);//без ударения
    //обновим словоформу
    SQL="UPDATE form SET fkey = '";
    SQL+=NewKey + "', form = '";
    SQL+=Form + "', search_form = '";
    SQL+=SForm + "' WHERE id = " + Id;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    RefreshCurrWord();
    Status->Text="Ok, форма исправлена";
  }
}
//---------------------------------------------------------------------------
// удаляем словоформу
void __fastcall TMForm::DeleteWordForm(TObject*Sender)
{ if(WordId.IsEmpty())
  { Status->Text="не выбрано слово!";
    return;
  }
  Status->Text="";
  AnsiString Key1, Key2;
  if(!InputKeyRange(Key1, Key2, "Удалить"))
    return;
  //удалим словоформу по id и ключу
  AnsiString SQL="DELETE FROM form WHERE word_id = ";
  SQL+=WordId;
  if(Key2.IsEmpty()) //удаляем одну форму
  { SQL+=" and fkey = '";
    SQL+=Key1 + "'";
  }
  else //удаляем диапазон форм
  { SQL+=" and fkey >= '";
    SQL+=Key1 + "' and fkey <= '";
    SQL+=Key2 + "'";
  }
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  RefreshCurrWord();
  Status->Text="Ok, форма удалена";
}

//---------------------------------------------------------------------------
void __fastcall TMForm::SetWordClass(TObject*Sender)
{ if(WordId.IsEmpty()) //значение должно быть выбрано
  { Status->Text="не выбрано слово!";
    return;
  }
  //получим имя старого класса
  TToolButton*Btn = (TToolButton*)Sender;
  AnsiString ClassName, OldClassName;
  if(Btn->Caption.Pos("...")==0) OldClassName = ClassName = Btn->Caption;
  //получим имя нового класса
  Status->Text="Ожидаем ввода имени класса...";
  if(InputStr(ClassName,"Введите имя класса",0) && ClassName!=OldClassName)
  { //найдем Id класса
    AnsiString ClsId;
    AnsiString SQL =
      "SELECT meaning.id FROM meaning "
      "WHERE name = '";
    SQL+=ClassName + "'";
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
    if(bool(row=mysql_fetch_row(Result)))
      ClsId = row[0];
    else
    { MessageBox(0,"Создать новый класс?","Класс не найден!",MB_YESNO|MB_ICONQUESTION);

      //...

      Status->Text="класс не найден!";
      mysql_free_result(Result);
      return;
    }
    mysql_free_result(Result);
    //установим класс
    SQL="UPDATE word SET class_id = ";
    SQL+=ClsId + " WHERE id = " + WordId;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    Btn->Caption = ClassName;
    Status->Text="Ok, класс установлен.";
  }
  else Status->Text="Класс не изменён...";
}
//---------------------------------------------------------------------------
//перенумеруем словоформы
void __fastcall TMForm::ReNumberClick(TObject *Sender)
{ if(WordId.IsEmpty())
  { Status->Text="не выбрано слово!";
    return;
  }
  Status->Text="";
  AnsiString SQL="SELECT id, fkey FROM form WHERE word_id = ";
  SQL+=WordId;
  SQL+=" ORDER BY id";
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_RES*Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  AnsiString Id, Key;
  int i=1;
  MYSQL_ROW row;
  while(bool(row=mysql_fetch_row(Result)))
  { Id = row[0];
    Key= row[1];
    //обновим словоформу
    SQL="UPDATE form SET fkey = '";
    SQL+=i;
    SQL+="' WHERE id = ";
    SQL+= Id;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }

    i++;
  }
  mysql_free_result(Result);

  RefreshCurrWord();
  Status->Text="Ok, словоформы перенумерованы";
}

//---------------------------------------------------------------------------
//перенесём указанные словоформы в отдельное слово
void __fastcall TMForm::SplitClick(TObject *Sender)
{ if(WordId.IsEmpty())
  { Status->Text="не выбрано слово!";
    return;
  }
  //запросим диапазон словоформ
  AnsiString Key1, Key2;
  if(!InputKeyRange(Key1, Key2, "Перенести")) return;
  if(Key2.IsEmpty()) return;
  //выберем слово для переноса
  NewWord(0);
  if(WordName.IsEmpty()) return; //слово не добавлено
  //получим word_id
  AnsiString SQL="SELECT id FROM word WHERE name = '";
  SQL+=WordName+"'";
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_RES*Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  AnsiString NewId;
  MYSQL_ROW row;
  if(bool(row=mysql_fetch_row(Result))) NewId = row[0];
  else
  { Status->Text="что-то не так...";
    return;
  }
  mysql_free_result(Result);
  //перенесём словоформы
  SQL="UPDATE form SET word_id = ";
  SQL+=NewId + " WHERE word_id = ";
  SQL+= WordId;
  SQL+=" and fkey >= '";
  SQL+=Key1 + "' and fkey <= '";
  SQL+=Key2 + "'";

  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  RefreshCurrWord();
  Status->Text="Ok, словоформы перенумерованы";
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TMForm::SaveObjCode(TObject*Sender)
{ if(WordId.IsEmpty())
  { Status->Text="не выбрано слово!";
    return;
  }
  AnsiString SQL;
  //сохраним конструктор слова
  if(ObjCode->Text.IsEmpty())
    SQL="UPDATE word SET obj_code = NULL WHERE id = ";
  else
  { SQL="UPDATE word SET obj_code = '";
    SQL+=ObjCode->Text+"' WHERE id = ";
  }
  SQL+=WordId;
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  //сохраним глобальный код
  if(GlobCode->Text.IsEmpty())
    SQL="UPDATE word SET glb_code = NULL WHERE id = ";
  else
  { SQL="UPDATE word SET glb_code = '";
    SQL+=GlobCode->Text+"' WHERE id = ";
  }
  SQL+=WordId;
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  Status->Text="Ok, код сохранен";
}
//---------------------------------------------------------------------------
void __fastcall TMForm::ShowSynonym(TObject*Sender)
{ //покажем синоним
  if(CurrSynonym.IsEmpty()) return;
  //позиционируем список слов
  FindWord->Text = CurrSynonym;
  RefreshWordList();
}
//---------------------------------------------------------------------------


