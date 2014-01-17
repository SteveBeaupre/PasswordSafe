//---------------------------------------------------------------------------

#ifndef AddPasswordUnitH
#define AddPasswordUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TAddPasswordForm : public TForm
{
__published:	// IDE-managed Components
	TLabeledEdit *LabeledEditSite;
	TLabeledEdit *LabeledEditPassword;
	TButton *ButtonOk;
	TButton *ButtonCancel;
	TCheckBox *CheckBoxShow;
private:	// User declarations
public:		// User declarations
	__fastcall TAddPasswordForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAddPasswordForm *AddPasswordForm;
//---------------------------------------------------------------------------
#endif
