//---------------------------------------------------------------------------

#ifndef FormFormH
#define FormFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm4Form : public TForm
{
__published:	// IDE-managed Components
  TLabel *FormLabel;
  TLabel *KeyLabel;
  TEdit *FormEdit;
  TEdit *KeyEdit;
  TButton *Button;
private:	// User declarations
public:		// User declarations
  __fastcall TForm4Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm4Form *Form4Form;
//---------------------------------------------------------------------------
#endif
