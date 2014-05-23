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
	TLabeledEdit *LabeledEditUserName;
	void __fastcall CheckBoxShowClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TAddPasswordForm(TComponent* Owner);
	void __fastcall Initialize(UnicodeString Site, UnicodeString UserName, UnicodeString Password);
	void __fastcall Initialize(TListItem *item);
};
//---------------------------------------------------------------------------
extern PACKAGE TAddPasswordForm *AddPasswordForm;
//---------------------------------------------------------------------------
#endif
