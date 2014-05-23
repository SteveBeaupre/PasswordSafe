//---------------------------------------------------------------------------

#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
#pragma comment(lib, "VortezSDK.lib")
#include "VortezSDK.h"
#include "UnicodeToChar.h"
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu;
	TMenuItem *FileMenu;
	TListView *ListView;
	TButton *ButtonAdd;
	TButton *ButtonDelete;
	TButton *ButtonMoveUp;
	TButton *ButtonMoveDown;
	TMenuItem *MenuOpenFile;
	TMenuItem *MenuSaveAsFile;
	TMenuItem *MenuSaveFile;
	TMenuItem *MenuCloseFile;
	TMenuItem *MenuExit;
	TMenuItem *N1;
	TMenuItem *N2;
	TButton *ButtonEdit;
	TSaveDialog *SaveDialog;
	TOpenDialog *OpenDialog;
	TCheckBox *CheckBoxShowPassword;
	TPopupMenu *PopupMenu;
	TMenuItem *CopyToClipboard1;
	void __fastcall ButtonAddClick(TObject *Sender);
	void __fastcall MenuExitClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall MenuSaveAsFileClick(TObject *Sender);
	void __fastcall MenuOpenFileClick(TObject *Sender);
	void __fastcall ButtonEditClick(TObject *Sender);
	void __fastcall ButtonDeleteClick(TObject *Sender);
	void __fastcall ButtonMoveUpClick(TObject *Sender);
	void __fastcall ButtonMoveDownClick(TObject *Sender);
	void __fastcall MenuSaveFileClick(TObject *Sender);
	void __fastcall MenuCloseFileClick(TObject *Sender);
	void __fastcall CheckBoxShowPasswordClick(TObject *Sender);
	void __fastcall CopyToClipboard1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);

	void __fastcall SetFileName(UnicodeString s);
	void __fastcall SetModified(bool mod);

	void __fastcall AddItem(TListItem *item, void *pswform);
	void __fastcall AddItem(UnicodeString Site, UnicodeString UserName, UnicodeString Password);
	int __fastcall  CalcEntrySize(int i);

	int  __fastcall CalcBufferSize();
	bool __fastcall LoadList(char *fname, char *password);
	bool __fastcall SaveList(char *fname, char *password);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
