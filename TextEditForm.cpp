//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TextEditForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTxtEdForm *TxtEdForm;
//---------------------------------------------------------------------------
__fastcall TTxtEdForm::TTxtEdForm(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
bool __fastcall InputTxt(AnsiString&Txt,char*Cap,char*But)
{ //ожидаем ввода текста
  bool Ok=false;
  TTxtEdForm*Form = new TTxtEdForm(0);
  if(Cap) Form->Caption = Cap;
  if(But) Form->Button->Caption = But;
  Form->Edit->Text=Txt;
	Ok=(Form->ShowModal()==mrOk);
  if(Ok) Txt=Form->Edit->Text; //успешно
  return Ok;
}
//---------------------------------------------------------------------------
