//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MasterPasswordUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMasterPasswordForm *MasterPasswordForm;
//---------------------------------------------------------------------------
__fastcall TMasterPasswordForm::TMasterPasswordForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMasterPasswordForm::CheckBoxShowClick(TObject *Sender)
{
	switch(CheckBoxShow->Checked)
	{
	case false: LabeledEditMasterPassword->PasswordChar = '*';  break;
	case true:  LabeledEditMasterPassword->PasswordChar = NULL; break;
	}
}
//---------------------------------------------------------------------------

