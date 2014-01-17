//---------------------------------------------------------------------------

#ifndef MasterPasswordUnitH
#define MasterPasswordUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TMasterPasswordForm : public TForm
{
__published:	// IDE-managed Components
	TLabeledEdit *LabeledEditMasterPassword;
	TButton *ButtonCancel;
	TButton *ButtonOk;
	TCheckBox *CheckBoxShow;
private:	// User declarations
public:		// User declarations
	__fastcall TMasterPasswordForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMasterPasswordForm *MasterPasswordForm;
//---------------------------------------------------------------------------
#endif
