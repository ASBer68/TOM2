//---------------------------------------------------------------------------

#ifndef TextEditFormH
#define TextEditFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TTxtEdForm : public TForm
{
__published:	// IDE-managed Components
  TMemo *Edit;
  TButton *Button;
  TButton *Button1;
private:	// User declarations
public:		// User declarations
  __fastcall TTxtEdForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTxtEdForm *TxtEdForm;
//---------------------------------------------------------------------------
#endif
