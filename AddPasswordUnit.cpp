//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddPasswordUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAddPasswordForm *AddPasswordForm;
//---------------------------------------------------------------------------
__fastcall TAddPasswordForm::TAddPasswordForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAddPasswordForm::CheckBoxShowClick(TObject *Sender)
{
	switch(CheckBoxShow->Checked)
	{
	case false: LabeledEditPassword->PasswordChar = '*';  break;
	case true:  LabeledEditPassword->PasswordChar = NULL; break;
	}
}
//---------------------------------------------------------------------------

