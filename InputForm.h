//---------------------------------------------------------------------------

#ifndef InputFormH
#define InputFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TInpForm : public TForm
{
__published:	// IDE-managed Components
  TEdit *Edit;
  TButton *Button;
private:	// User declarations
public:		// User declarations
  __fastcall TInpForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TInpForm *InpForm;
//---------------------------------------------------------------------------
#endif
