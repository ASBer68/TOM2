#include <vcl.h>
#pragma hdrstop
#include "FormForm.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TForm4Form::TForm4Form(TComponent*Owner):TForm(Owner)
{
}
//---------------------------------------------------------------------------
bool __fastcall InputForm(AnsiString&Key, AnsiString&WForm, char*But)
{ //создаём форму
  TForm4Form*Form = new TForm4Form(0);
  //предустанавливаем тектс элементов
  if(But) Form->Button->Caption = But;
  Form->KeyEdit->Text=Key;
  Form->FormEdit->Text=WForm;
  //ждём ввода
	bool Ok=(Form->ShowModal()==mrOk);
  if(Ok) //успешно
  { //проверяем ввод
    if(Form->KeyEdit->Text.IsEmpty() || Form->FormEdit->Text.IsEmpty())
      Ok=false;
    else
    { //забираем результат
      Key=Form->KeyEdit->Text;
      WForm=Form->FormEdit->Text;
    }
  }
  //удаляем форму
  delete Form;
  return Ok;
}
//---------------------------------------------------------------------------
bool __fastcall InputKeyRange(AnsiString&Key1, AnsiString&Key2, char*But)
{ //создаём форму
  TForm4Form*Form = new TForm4Form(0);
  //предустанавливаем текcт элементов
  Form->Caption="Укажите диапазон ключей";
  Form->KeyLabel->Caption="c";
  Form->FormLabel->Caption="по";
  if(But) Form->Button->Caption = But;
  Form->KeyEdit->Text=Key1;
  Form->FormEdit->Text=Key2;
  //ждём ввода
	bool Ok=(Form->ShowModal()==mrOk);
  if(Ok) //успешно
  { //проверяем ввод
    if(Form->KeyEdit->Text.IsEmpty()) Ok=false;
    else
    { //забираем результат
      Key1=Form->KeyEdit->Text;
      Key2=Form->FormEdit->Text;
    }
  }
  //удаляем форму
  delete Form;
  return Ok;
}
//---------------------------------------------------------------------------
/*
bool __fastcall InputAssociation(AnsiString&Obj, AnsiString&Prop)
{ //создаём форму
  TForm4Form*Form = new TForm4Form(0);
  //предустанавливаем текcт элементов
  Form->Caption="Добавление нового ассоциации";
  Form->KeyLabel->Caption="Объект";
  Form->FormLabel->Caption="Свойство";
  Form->KeyEdit->Text=Obj;
  Form->FormEdit->Text=Prop;
  //ждём ввода
	bool Ok=(Form->ShowModal()==mrOk);
  if(Ok) //успешно
  { //проверяем ввод
    if(Form->KeyEdit->Text.IsEmpty()) Ok=false;
    else
    { //забираем результат
      Obj =Form->KeyEdit->Text;
      Prop=Form->FormEdit->Text;
    }
  }
  //удаляем форму
  delete Form;
  return Ok;
}
*/
//---------------------------------------------------------------------------

