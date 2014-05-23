//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddPasswordUnit.h"
#include "MainUnit.h"
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
void __fastcall TAddPasswordForm::Initialize(UnicodeString Site, UnicodeString UserName, UnicodeString Password)
{
	LabeledEditSite->Text = Site;
	LabeledEditUserName->Text = UserName;
	LabeledEditPassword->Text = Password;
	CheckBoxShow->OnClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TAddPasswordForm::Initialize(TListItem *item)
{
	item = MainForm->ListView->Selected;
	Initialize(item->Caption, item->SubItems->Strings[0], item->SubItems->Strings[1]);
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

