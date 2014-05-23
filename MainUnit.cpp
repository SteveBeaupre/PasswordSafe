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
//#define OLD_LOADING_CODE
//#define NO_LOAD_ENCRYPTION
#define NO_SAVE_ENCRYPTION
//---------------------------------------------------------------------------
#define MAX_MASTER_PASS_SIZE  64
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
	CheckBoxShowPassword->OnClick(this);
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
void __fastcall TMainForm::AddItem(UnicodeString Site, UnicodeString UserName, UnicodeString Password)
{
	TListItem *item = ListView->Items->Add();

	item->Caption = Site;
	item->SubItems->Add(UserName);
	item->SubItems->Add(Password);

	SetModified(true);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::AddItem(TListItem *item, void *pswform)
{
	if(item == NULL)
		item = ListView->Items->Add();

	TAddPasswordForm *pAddPasswordForm = (TAddPasswordForm*)pswform;

	item->Caption = pAddPasswordForm->LabeledEditSite->Text;
	item->SubItems->Add(pAddPasswordForm->LabeledEditUserName->Text);
	item->SubItems->Add(pAddPasswordForm->LabeledEditPassword->Text);

	SetModified(true);
}
//---------------------------------------------------------------------------
int __fastcall TMainForm::CalcEntrySize(int i)
{
	// Get the list item
	TListItem *item = ListView->Items->Item[i];
	UnicodeString itSite = item->Caption;
	UnicodeString itName = item->SubItems->Strings[0];
	UnicodeString itPass = item->SubItems->Strings[1];

	return ((sizeof(int) * 3) + itSite.Length() + itName.Length() + itPass.Length());
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonAddClick(TObject *Sender)
{
	AddPasswordForm->Initialize("","","");

	if(AddPasswordForm->ShowModal() == mrOk){
		if(AddPasswordForm->LabeledEditSite->Text.Length() > 0){
			AddItem(NULL, AddPasswordForm);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonEditClick(TObject *Sender)
{
	TListItem *item = ListView->Selected;
	if(item){
		AddPasswordForm->Initialize(item);

		if(AddPasswordForm->ShowModal() == mrOk){
			AddItem(item, AddPasswordForm);
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

		MasterPasswordForm->LabeledEditMasterPassword->Text = "";
		MasterPasswordForm->CheckBoxShow->OnClick(this);

		if(MasterPasswordForm->ShowModal() == mrOk){

			CRawBuffer fname(MAX_PATH);
			CRawBuffer MasterPassword(MasterPasswordForm->LabeledEditMasterPassword->MaxLength);
			ConvertUnicodeToChar(MasterPassword.GetStrBuffer(), MasterPassword.GetSize(), MasterPasswordForm->LabeledEditMasterPassword->Text.c_str());
			ConvertUnicodeToChar(fname.GetStrBuffer(), fname.GetSize(), OpenDialog->FileName.c_str());

			if(LoadList(fname.GetStrBuffer(), MasterPassword.GetStrBuffer())){
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

		MasterPasswordForm->CheckBoxShow->OnClick(this);

		if(MasterPasswordForm->ShowModal() == mrOk){

			CRawBuffer fname(MAX_PATH);
			CRawBuffer MasterPassword(MasterPasswordForm->LabeledEditMasterPassword->MaxLength);
			ConvertUnicodeToChar(fname.GetStrBuffer(), MAX_PATH, SaveDialog->FileName.c_str());
			ConvertUnicodeToChar(MasterPassword.GetStrBuffer(), MasterPassword.GetSize(), MasterPasswordForm->LabeledEditMasterPassword->Text.c_str());

			if(SaveList(fname.GetStrBuffer(), MasterPassword.GetStrBuffer())){
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
		MasterPasswordForm->CheckBoxShow->OnClick(this);
		if(MasterPasswordForm->ShowModal() == mrOk){
			CRawBuffer fname(MAX_PATH);
			CRawBuffer MasterPassword(MasterPasswordForm->LabeledEditMasterPassword->MaxLength);
			ConvertUnicodeToChar(fname.GetStrBuffer(), MAX_PATH, CurrentFileName.c_str());
			ConvertUnicodeToChar(MasterPassword.GetStrBuffer(), MasterPassword.GetSize(), MasterPasswordForm->LabeledEditMasterPassword->Text.c_str());
			if(SaveList(fname.GetStrBuffer(), MasterPassword.GetStrBuffer()))
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
				MasterPasswordForm->CheckBoxShow->OnClick(this);
				if(MasterPasswordForm->ShowModal() == mrOk){

					CRawBuffer fname(MAX_PATH);
					CRawBuffer MasterPassword(MasterPasswordForm->LabeledEditMasterPassword->MaxLength);
					ConvertUnicodeToChar(fname.GetStrBuffer(), MAX_PATH, CurrentFileName.c_str());
					ConvertUnicodeToChar(MasterPassword.GetStrBuffer(), MasterPassword.GetSize(), MasterPasswordForm->LabeledEditMasterPassword->Text.c_str());

					SaveList(fname.GetStrBuffer(), MasterPassword.GetStrBuffer());
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
	int size = 0;
	for(int i = 0; i < ListView->Items->Count; i++)
		size += CalcEntrySize(i);

	return size + sizeof(DWORD);
}
//---------------------------------------------------------------------------
bool __fastcall TMainForm::LoadList(char *fname, char *password)
{
	// Check the file's size
	CFileManager FileManager;
	UINT fsize = FileManager.GetSize(fname);
	if(fsize < sizeof(DWORD) * 2){
		ShowMessage("Invalid or corrupted file.");
		return false;
	}

	CFileIO f;
	if(f.OpenForReading(fname)){

		// Clear the list
		ListView->Clear();

		// Read the hash from the file header
		DWORD Hash = 0;
		f.Read(&Hash, sizeof(DWORD));
		// Remove the size of the hash for the file size remaining
		fsize -= sizeof(DWORD);

		// Calculate the buffer size
		int BufIndx = 0;
		CRawBuffer Buffer(fsize);
		BYTE *pBuffer = Buffer.GetBuffer();

		// Read the rest of the file
		f.Read(pBuffer, fsize);

		// Master password
		int MPLen = strlen(password);
		CRawBuffer MPBuffer(MPLen);
		memcpy(MPBuffer.GetBuffer(), password, MPLen);

		// Decrypt the buffer using the hash of the master password
		#ifndef NO_LOAD_ENCRYPTION
		Buffer.Decrypt(MPBuffer.Hash());
		#endif
		//Buffer.SaveToFile("C:\\Temp\\Decrypted.bin");

		// Calculate the current buffer hash
		DWORD BufHash = Buffer.Hash();

		// Check if they match...
		if(BufHash != Hash){
			ShowMessage("Invalid Password.");
			f.Close();
			return false;
		}

		// Save the number of entries
		DWORD NumEntries = 0;
		memcpy(&NumEntries, &pBuffer[BufIndx], sizeof(DWORD));
		BufIndx += sizeof(DWORD);

		// For each entry
		for(DWORD i = 0; i < NumEntries; i++){

			int StringsLens[3];
			CRawBuffer StringsBuffer[3];

			#ifdef OLD_LOADING_CODE
			for(int j = 0; j < 3; j++){
				memcpy(&StringsLens[j], &pBuffer[BufIndx], sizeof(int));
				BufIndx += sizeof(int);
			}

			for(int j = 0; j < 3; j++){
				int Len = StringsLens[j];
				StringsBuffer[j].Allocate(Len+1);
				memcpy(StringsBuffer[j].GetBuffer(), &pBuffer[BufIndx], Len);
				BufIndx += Len;
			}
			#else
			for(int j = 0; j < 3; j++){

				memcpy(&StringsLens[j], &pBuffer[BufIndx], sizeof(int));
				BufIndx += sizeof(int);

				int Len = StringsLens[j];

				StringsBuffer[j].Allocate(Len+1);
				if(Len > 0){
					char *str = &pBuffer[BufIndx];
					memcpy(StringsBuffer[j].GetBuffer(), str, Len);
					BufIndx += Len;
				}
			}
			#endif

			// Set the list view item
			AddItem(StringsBuffer[0].GetStrBuffer(), StringsBuffer[1].GetStrBuffer(), StringsBuffer[2].GetStrBuffer());
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

		// Store the number of entries
		DWORD NumEntries = ListView->Items->Count;

		// Calculate the size and allocate a buffer
		int BufIndx = 0;
		const int BufSize = CalcBufferSize();
		CRawBuffer Buffer(BufSize);
		BYTE *pBuffer = Buffer.GetBuffer();

		// Save the number of entries
		memcpy(&pBuffer[BufIndx], &NumEntries, sizeof(DWORD));
		BufIndx += sizeof(DWORD);

		// For each entries
		for(DWORD i = 0; i < NumEntries; i++){

			// Get the list item
			TListItem *item = ListView->Items->Item[i];

			UnicodeString StringsBuffer[3];

			StringsBuffer[0] = item->Caption;
			StringsBuffer[1] = item->SubItems->Strings[0];
			StringsBuffer[2] = item->SubItems->Strings[1];

			for(int j = 0; j < 3; j++){

				int Len = StringsBuffer[j].Length();

				CRawBuffer Buffer(Len+1);
				ConvertUnicodeToChar(Buffer.GetBuffer(), Len+1, StringsBuffer[j].c_str());

				memcpy(&pBuffer[BufIndx], &Len, sizeof(int));
				BufIndx += sizeof(int);

				if(Len > 0){
					memcpy(&pBuffer[BufIndx], Buffer.GetBuffer(), Len);
					BufIndx += Len;
				}
			}
		}

		// Store the buffer's hash
		DWORD Hash = Buffer.Hash();

		// Save the master password hash
		int MPLen = strlen(password);
		CRawBuffer MPBuffer(MPLen);
		memcpy(MPBuffer.GetBuffer(), password, MPLen);

		// Encrypt the buffer using the master password hash
		//Buffer.SaveToFile("C:\\Temp\\Original.bin");
		#ifndef NO_SAVE_ENCRYPTION
		Buffer.Encrypt(MPBuffer.Hash());
		#endif

		// Write the hash to the file, then the buffer
		f.Write(&Hash, sizeof(DWORD));
		f.Write(Buffer.GetBuffer(), Buffer.GetSize());

		// Close the file and return success
		f.Close();
		return true;
	}

	return false;
}

void __fastcall TMainForm::CheckBoxShowPasswordClick(TObject *Sender)
{
	TListColumn *pPswColum = ListView->Column[2];

	static int colwidth = 185;

	// Show/Hide the password column
	if(CheckBoxShowPassword->Checked){
		pPswColum->Width = colwidth;
		pPswColum->AutoSize = true;
	} else {
		pPswColum->Width = 0;
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CopyToClipboard1Click(TObject *Sender)
{
	TListItem *item = ListView->Selected;

	// Copy the selected item into the clipboard
	if(item){
		TEdit *edb = new TEdit(this);

		edb->Parent = this;
		edb->Visible = false;
		edb->Text = item->SubItems->Strings[1];
		edb->SelectAll();
		edb->CopyToClipboard();

		delete edb;
	}
}
//---------------------------------------------------------------------------

