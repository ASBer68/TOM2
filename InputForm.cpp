#include <vcl.h>
#pragma hdrstop
#include "InputForm.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TInpForm::TInpForm(TComponent*Owner):TForm(Owner)
{}
//---------------------------------------------------------------------------
bool __fastcall InputStr(AnsiString&Str,char*Cap,char*But)
{ //ожидаем ввода строки
  TInpForm*Form = new TInpForm(0);
  if(Cap) Form->Caption = Cap;
  if(But) Form->Button->Caption = But;
  Form->Edit->Text = Str;
	bool Ok=(Form->ShowModal()==mrOk && !Form->Edit->Text.IsEmpty());
  if(Ok) Str=Form->Edit->Text; //успешно

  delete Form;
  return Ok;
}
//---------------------------------------------------------------------------





