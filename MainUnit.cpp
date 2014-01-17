//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "MainUnit.h"
#include "AddPasswordUnit.h"
#include "MasterPasswordUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
const UnicodeString AppCaption = "Password Safe";
UnicodeString CurrentFileName;
bool CurrentFileModified;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	Caption = AppCaption;
	CurrentFileName = "";
	CurrentFileModified = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::MenuExitClick(TObject *Sender)
{
	MenuCloseFile->OnClick(this);
	this->Close();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonAddClick(TObject *Sender)
{
	AddPasswordForm->LabeledEditSite->Text = "";
	AddPasswordForm->LabeledEditPassword->Text = "";

	if(AddPasswordForm->ShowModal() == mrOk){
		if(AddPasswordForm->LabeledEditSite->Text.Length() > 0){
			TListItem *item = ListView->Items->Add();
			item->Caption = AddPasswordForm->LabeledEditSite->Text;
			item->SubItems->Add(AddPasswordForm->LabeledEditPassword->Text);
			SetModified(true);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonEditClick(TObject *Sender)
{
	TListItem *item = ListView->Selected;
	if(item){
		AddPasswordForm->LabeledEditSite->Text = item->Caption;
		AddPasswordForm->LabeledEditPassword->Text = item->SubItems->Strings[0];

		if(AddPasswordForm->ShowModal() == mrOk){
			item->Caption = AddPasswordForm->LabeledEditSite->Text;
			item->SubItems->Strings[0] = AddPasswordForm->LabeledEditPassword->Text;
			SetModified(true);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonDeleteClick(TObject *Sender)
{
	TListItem *item = ListView->Selected;
	if(item){
		ListView->DeleteSelected();
		SetModified(true);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonMoveUpClick(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonMoveDownClick(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMainForm::MenuOpenFileClick(TObject *Sender)
{
	if(CurrentFileModified)
		MenuCloseFile->OnClick(this);

	if(OpenDialog->Execute()){
		char fname[MAX_PATH];
		ConvertUnicodeToChar(fname, MAX_PATH, OpenDialog->FileName.c_str());
		MasterPasswordForm->LabeledEditMasterPassword->Text = "";
		if(MasterPasswordForm->ShowModal() == mrOk){
			char MasterPassword[MAX_PATH];
			ConvertUnicodeToChar(MasterPassword, MAX_PATH, MasterPasswordForm->LabeledEditMasterPassword->Text.c_str());
			if(LoadList(fname, MasterPassword)){
				SetFileName(OpenDialog->FileName);
				SetModified(false);
			} else {
				ShowMessage("An error occured while loading the file \"" + OpenDialog->FileName + "\"");
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::MenuSaveAsFileClick(TObject *Sender)
{
	if(SaveDialog->Execute()){
		char fname[MAX_PATH];
		ConvertUnicodeToChar(fname, MAX_PATH, SaveDialog->FileName.c_str());
		if(MasterPasswordForm->ShowModal() == mrOk){
			char MasterPassword[MAX_PATH];
			ConvertUnicodeToChar(MasterPassword, MAX_PATH, MasterPasswordForm->LabeledEditMasterPassword->Text.c_str());
			if(SaveList(fname, MasterPassword)){
				SetFileName(SaveDialog->FileName);
				SetModified(false);
			} else {
				ShowMessage("An error occured while saving the file \"" + OpenDialog->FileName + "\"");
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::MenuSaveFileClick(TObject *Sender)
{
	if(CurrentFileName.Length() > 0){
		if(MasterPasswordForm->ShowModal() == mrOk){
			char MasterPassword[MAX_PATH];
			ConvertUnicodeToChar(MasterPassword, MAX_PATH, MasterPasswordForm->LabeledEditMasterPassword->Text.c_str());
			char fname[MAX_PATH];
			ConvertUnicodeToChar(fname, MAX_PATH, CurrentFileName.c_str());
			if(SaveList(fname, MasterPassword))
				SetModified(false);
        }
	} else {
		if(CurrentFileModified)
			MenuSaveAsFile->OnClick(this);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::MenuCloseFileClick(TObject *Sender)
{
	if(CurrentFileModified){
		if(MessageBoxA(this->Handle, "The current file has been modified. Save changes?", "Warning", MB_YESNOCANCEL) == ID_YES){
			if(CurrentFileName.Length() > 0){
				if(MasterPasswordForm->ShowModal() == mrOk){
					char MasterPassword[MAX_PATH];
					ConvertUnicodeToChar(MasterPassword, MAX_PATH, MasterPasswordForm->LabeledEditMasterPassword->Text.c_str());
					char fname[MAX_PATH];
					ConvertUnicodeToChar(fname, MAX_PATH, CurrentFileName.c_str());
					SaveList(fname, MasterPassword);
					SetModified(false);
                }
			} else {
				MenuSaveAsFile->OnClick(this);
			}

			SetFileName("");
			SetModified(false);
			ListView->Clear();
		}
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMainForm::SetFileName(UnicodeString s)
{
	if(s.Length() > 0){
		this->Caption = AppCaption + " - " + ExtractFileName(s);
		if(CurrentFileModified){
			this->Caption = this->Caption + "*";
		}
	} else {
		this->Caption = AppCaption;
		if(CurrentFileModified){
			this->Caption = this->Caption + " - *";
		}
	}

	CurrentFileName = s;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SetModified(bool mod)
{
	CurrentFileModified = mod;
	SetFileName(CurrentFileName);

	//MenuSaveFile->Enabled = mod;
	//MenuSaveAsFile->Enabled = mod;
	MenuCloseFile->Enabled = CurrentFileName.Length() > 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int __fastcall TMainForm::CalcBufferSize()
{
	int size = sizeof(int);

	DWORD NumEntries = ListView->Items->Count;

	for(DWORD i = 0; i < NumEntries; i++){
		TListItem *item = ListView->Items->Item[i];
		UnicodeString itName = item->Caption;
		UnicodeString itPass = item->SubItems->Strings[0];

		int NameLen = itName.Length();
		int PassLen = itPass.Length();

		size += (sizeof(int) * 2) + NameLen + PassLen;
	}

	return size;
}
//---------------------------------------------------------------------------
bool __fastcall TMainForm::LoadList(char *fname, char *password)
{
	CFileIO f;
	if(f.OpenForReading(fname)){

		ListView->Clear();

		DWORD Hash = 0;
		f.Read(&Hash, sizeof(DWORD));

		CFileManager FileManager;
		int fsize = FileManager.GetSize(fname);
		fsize -= sizeof(DWORD);
		if(fsize < 8){
			ShowMessage("Invalid or corrupted file.");
			f.Close();
			return false;
		}

		int BufIndx = 0;
		CRawBuffer Buffer(fsize);
		BYTE *pBuffer = Buffer.GetBuffer();

		f.Read(pBuffer, fsize);

		int MPLen = strlen(password);
		CRawBuffer MPBuffer(MPLen);
		memcpy(MPBuffer.GetBuffer(), password, MPLen);

		Buffer.Decrypt(MPBuffer.Hash());
		//Buffer.SaveToFile("C:\\Temp\\Decrypted.bin");

		DWORD BufHash = Buffer.Hash();

		if(BufHash != Hash){
			ShowMessage("Invalid Password.");
			f.Close();
			return false;
		}

		DWORD NumEntries = 0;
		memcpy(&NumEntries, &pBuffer[BufIndx], sizeof(DWORD));
		BufIndx += sizeof(DWORD);

		for(DWORD i = 0; i < NumEntries; i++){

			int NameLen = 0;
			int PassLen = 0;
			memcpy(&NameLen, &pBuffer[BufIndx], sizeof(int));
			BufIndx += sizeof(int);
			memcpy(&PassLen, &pBuffer[BufIndx], sizeof(int));
			BufIndx += sizeof(int);

			char *pName = new char[NameLen+1];
			char *pPass = new char[PassLen+1];
			ZeroMemory(pName, NameLen+1);
			ZeroMemory(pPass, PassLen+1);

			memcpy(pName, &pBuffer[BufIndx], NameLen);
			BufIndx += NameLen;
			memcpy(pPass, &pBuffer[BufIndx], PassLen);
			BufIndx += PassLen;

			TListItem *item = ListView->Items->Add();
			item->Caption = UnicodeString(pName);
			item->SubItems->Add(UnicodeString(pPass));

			delete [] pPass;
			delete [] pName;
		}

		f.Close();
		return true;
	}

	return false;
}
//---------------------------------------------------------------------------
bool __fastcall TMainForm::SaveList(char *fname, char *password)
{
	CFileIO f;
	if(f.OpenForWriting(fname)){

		DWORD NumEntries = ListView->Items->Count;

		int BufIndx = 0;
		const int BufSize = CalcBufferSize();
		CRawBuffer Buffer(BufSize);
		BYTE *pBuffer = Buffer.GetBuffer();

		memcpy(&pBuffer[BufIndx], &NumEntries, sizeof(DWORD));
		BufIndx += sizeof(DWORD);

		for(DWORD i = 0; i < NumEntries; i++){

			TListItem *item = ListView->Items->Item[i];
			UnicodeString itName = item->Caption;
			UnicodeString itPass = item->SubItems->Strings[0];

			int NameLen = itName.Length();
			int PassLen = itPass.Length();
			char *pName = new char[NameLen+1];
			char *pPass = new char[PassLen+1];
			ConvertUnicodeToChar(pName, NameLen+1, itName.c_str());
			ConvertUnicodeToChar(pPass, PassLen+1, itPass.c_str());

			memcpy(&pBuffer[BufIndx], &NameLen, sizeof(int));
			BufIndx += sizeof(int);
			memcpy(&pBuffer[BufIndx], &PassLen, sizeof(int));
			BufIndx += sizeof(int);
			memcpy(&pBuffer[BufIndx], pName, NameLen);
			BufIndx += NameLen;
			memcpy(&pBuffer[BufIndx], pPass, PassLen);
			BufIndx += PassLen;

			delete [] pPass;
			delete [] pName;
		}

		DWORD Hash = Buffer.Hash();

		int MPLen = strlen(password);
		CRawBuffer MPBuffer(MPLen);
		memcpy(MPBuffer.GetBuffer(), password, MPLen);

		//Buffer.SaveToFile("C:\\Temp\\Original.bin");
		Buffer.Encrypt(MPBuffer.Hash());

		f.Write(&Hash, sizeof(DWORD));
		f.Write(Buffer.GetBuffer(), Buffer.GetSize());

		f.Close();
		return true;
	}

	return false;
}
//---------------------------------------------------------------------------

