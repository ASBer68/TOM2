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
{ //������ �����
  TForm4Form*Form = new TForm4Form(0);
  //����������������� ����� ���������
  if(But) Form->Button->Caption = But;
  Form->KeyEdit->Text=Key;
  Form->FormEdit->Text=WForm;
  //��� �����
	bool Ok=(Form->ShowModal()==mrOk);
  if(Ok) //�������
  { //��������� ����
    if(Form->KeyEdit->Text.IsEmpty() || Form->FormEdit->Text.IsEmpty())
      Ok=false;
    else
    { //�������� ���������
      Key=Form->KeyEdit->Text;
      WForm=Form->FormEdit->Text;
    }
  }
  //������� �����
  delete Form;
  return Ok;
}
//---------------------------------------------------------------------------
bool __fastcall InputKeyRange(AnsiString&Key1, AnsiString&Key2, char*But)
{ //������ �����
  TForm4Form*Form = new TForm4Form(0);
  //����������������� ���c� ���������
  Form->Caption="������� �������� ������";
  Form->KeyLabel->Caption="c";
  Form->FormLabel->Caption="��";
  if(But) Form->Button->Caption = But;
  Form->KeyEdit->Text=Key1;
  Form->FormEdit->Text=Key2;
  //��� �����
	bool Ok=(Form->ShowModal()==mrOk);
  if(Ok) //�������
  { //��������� ����
    if(Form->KeyEdit->Text.IsEmpty()) Ok=false;
    else
    { //�������� ���������
      Key1=Form->KeyEdit->Text;
      Key2=Form->FormEdit->Text;
    }
  }
  //������� �����
  delete Form;
  return Ok;
}
//---------------------------------------------------------------------------
/*
bool __fastcall InputAssociation(AnsiString&Obj, AnsiString&Prop)
{ //������ �����
  TForm4Form*Form = new TForm4Form(0);
  //����������������� ���c� ���������
  Form->Caption="���������� ������ ����������";
  Form->KeyLabel->Caption="������";
  Form->FormLabel->Caption="��������";
  Form->KeyEdit->Text=Obj;
  Form->FormEdit->Text=Prop;
  //��� �����
	bool Ok=(Form->ShowModal()==mrOk);
  if(Ok) //�������
  { //��������� ����
    if(Form->KeyEdit->Text.IsEmpty()) Ok=false;
    else
    { //�������� ���������
      Obj =Form->KeyEdit->Text;
      Prop=Form->FormEdit->Text;
    }
  }
  //������� �����
  delete Form;
  return Ok;
}
*/
//---------------------------------------------------------------------------

