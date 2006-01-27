/*******************************************
 * Original source : GNUPLOT - win/wmenu.c
 * modified for Scilab 
 *******************************************
 * Copyright (C) 1992   Maurice Castro, Russell Lang
 *
 * Permission to use, copy, and distribute this software and its
 * documentation for any purpose with or without fee is hereby granted, 
 * provided that the above copyright notice appear in all copies and 
 * that both that copyright notice and this permission notice appear 
 * in supporting documentation.
 *
 * Permission to modify the software is granted, but not the right to
 * distribute the modified code.  Modifications are to be distributed 
 * as patches to released version.
 *  
 * This software is provided "as is" without express or implied warranty.
 * AUTHORS
 *   Maurice Castro
 *   Russell Lang
 *   Modified for Scilab (1997) : Jean-Philippe Chancelier 
 *   Modified for Scilab (2003-2004) : Allan CORNET
 */
#pragma comment(lib, "shell32.lib")
#include <ShlObj.h>
#include "wmenu.h"

#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"

#include "../os_specific/win_mem_alloc.h" /* MALLOC */

#ifndef STRICT
#define STRICT
#endif
/*-----------------------------------------------------------------------------------*/
extern HINSTANCE hdllInstance;
extern HDC TryToGetDC(HWND hWnd);
/* WTEXT.C*/
extern void SwitchConsole(void);
/* Efface la fenetre de "commandes" */
extern void ClearCommandWindow(LPTW lptw,BOOL Clearfirstline);
extern void reset_history(void); /* see Console\history.h*/
extern void HideGraphToolBar(struct BCG * ScilabGC);
extern void ShowGraphToolBar(struct BCG * ScilabGC);
extern void SaveCurrentLine(BOOL RewriteLineAtPrompt);
extern void GetCurrentPrompt(char *CurrentPrompt);
extern BOOL ConvertPathUnixToWindowsFormat(char *pathunix,char *pathwindows);
extern char *GetScilabDirectory(BOOL UnixStyle);
extern DWORD GetIhmTextBackgroundColor(void);
extern void InitIhmDefaultColor(void);
extern DWORD GetIhmTextColor(void);
extern BOOL SetIhmSystemDefaultTextBackgroundColor(void);
extern BOOL SetIhmSystemDefaultTextColor(void);
extern BOOL ConfigurePrinterDialogBox(void);
extern BOOL ChooseColorBox(int *R,int *G,int *B);
extern BOOL SetIhmTextBackgroundColor(int R,int G,int B,BOOL Refresh);
extern BOOL SetIhmTextColor(int R,int G,int B,BOOL Refresh);
extern LPTW GetTextWinScilab(void);
extern BOOL IsEmptyClipboard(LPTW lptw);
extern void PasteFunction(LPTW lptw,BOOL special);
extern BOOL IsAFile(char *chainefichier);
extern BOOL IsWindowInterface(void);
extern void AddHistory (char *line);
extern void GetCommentDateSession(char *line,int BeginSession);
extern void WINAPI FilesAssociationBox (HWND hwnd);
/*-----------------------------------------------------------------------------------*/
static integer lab_count = 0;
static char gwin_name[100], gwin_name1[100];
static void Countp ();
static void Countm ();
static void SendCountSet ();
static void SendCountRaise ();
static void SendCountDelete ();

/*-----------------------------------------------------------------------------------*/
void Callback_NEWSCILAB(void)
{
	STARTUPINFO start;
	SECURITY_ATTRIBUTES sec_attrs;
	PROCESS_INFORMATION child;
	char ScilexName[MAX_PATH];
	    		
	GetModuleFileName (NULL,ScilexName, MAX_PATH);

	memset (&start, 0, sizeof (start));
	start.cb = sizeof (start);
	start.dwFlags = STARTF_USESHOWWINDOW;
	start.wShowWindow = SW_SHOWMINIMIZED;
	sec_attrs.nLength = sizeof (sec_attrs);
	sec_attrs.lpSecurityDescriptor = NULL;
	sec_attrs.bInheritHandle = FALSE;

	if ( GetVersion() < 0x80000000 )
	{
		/* Windows NT */
		char *VarEnvironmnt=NULL;
		extern char ScilexWindowName[MAX_PATH];

		VarEnvironmnt=(char*)MALLOC((strlen("SCILAB_CREATOR=")+strlen(ScilexWindowName)+1)*sizeof(char));
		wsprintf(VarEnvironmnt,"SCILAB_CREATOR=%s",ScilexWindowName);
		putenv(VarEnvironmnt);
		if (VarEnvironmnt) 
		{
			FREE(VarEnvironmnt);
			VarEnvironmnt=NULL;
		}
		
		if (CreateProcess (ScilexName,"",&sec_attrs, NULL, FALSE, CREATE_NEW_CONSOLE,NULL, NULL, &start, &child))
		{
			CloseHandle (child.hThread);
			CloseHandle (child.hProcess);
		}
		else MessageBox(NULL,MSG_WARNING23,MSG_WARNING22,MB_ICONWARNING);

	}
	else
	{
		/* Win32s, Win95,Win98,WinME */
		if (CreateProcess (NULL,ScilexName, &sec_attrs, NULL, FALSE, CREATE_NEW_CONSOLE,  NULL, NULL, &start, &child))
		{
			CloseHandle (child.hThread);
			CloseHandle (child.hProcess);
		}
		else MessageBox(NULL,MSG_WARNING23,MSG_WARNING22,MB_ICONWARNING);
	}
}
/*-----------------------------------------------------------------------------------*/
void Callback_OPEN(void)
{
	char File[MAX_PATH];
	char ShortFile[MAX_PATH];
	char command[MAX_PATH];
	char TitleText[32];

	LPTW lptw=GetTextWinScilab();
	

	switch (lptw->lpmw->CodeLanguage)
	{
		case 1:
			strcpy(TitleText,MSG_SCIMSG32);
			break;
		case 0:default:
			strcpy(TitleText,MSG_SCIMSG33);
			break;
	}
	    			
	if ( OpenSaveSCIFile(lptw->hWndParent,TitleText,TRUE,"Files *.sce;*.sci\0*.sci;*.sce\0Files *.sci\0*.sci\0Files *.sce\0*.sce\0All *.*\0*.*\0",File) == TRUE)
	{
		extern BOOL PutLineInBuffer;
		char CommandBis[512];
		GetShortPathName(File,ShortFile,MAX_PATH);
		wsprintf(command,"%cscipad('%s');",CTRLU,ShortFile);
		wsprintf(CommandBis,"scipad('%s');",ShortFile);

		PutLineInBuffer=FALSE;
		if (IsToThePrompt ()) StoreCommand1(command,1);
		else StoreCommand(CommandBis);
		PutLineInBuffer=FALSE;
		
	}
}
/*-----------------------------------------------------------------------------------*/
void Callback_EXEC(void)
{
	char Fichier[MAX_PATH];
	char command[MAX_PATH];
	
	LPTW lptw=GetTextWinScilab();

	_try {
		if ( OpenSaveSCIFile(lptw->hWndParent,"Exec",TRUE,"Files *.sce;*.sci\0*.sci;*.sce\0Files *.sci\0*.sci\0Files *.sce\0*.sce\0All *.*\0*.*\0",Fichier) == TRUE)
		{
			SendCTRLandAKey(CTRLU);
			wsprintf(command,MSG_SCIMSG34,Fichier);
			StoreCommand1 (command,1);
		}	
	}
	_except (EXCEPTION_EXECUTE_HANDLER) 
	{
		
	}
	    			
	
}
/*-----------------------------------------------------------------------------------*/
void Callback_GETF(void)
{
	char Fichier[MAX_PATH];
	char command[MAX_PATH];
	
	LPTW lptw=GetTextWinScilab();
	    			
	if ( OpenSaveSCIFile(lptw->hWndParent,"Getf",TRUE,"Files *.sci\0*.sci\0All *.*\0*.*\0",Fichier) == TRUE)
	{
		SendCTRLandAKey(CTRLU);
		wsprintf(command,MSG_SCIMSG35,Fichier);
		StoreCommand1 (command,1);
	}
}
/*-----------------------------------------------------------------------------------*/
void Callback_LOAD(void)
{
	char Fichier[MAX_PATH];
	char command[MAX_PATH];
	    			
	LPTW lptw=GetTextWinScilab();
	    			
	if ( OpenSaveSCIFile(lptw->hWndParent,"Load",TRUE,"Files *.sav;*.bin\0*.sav;*.bin\0Files *.sav\0*.sav\0Files *.bin\0*.bin\0All *.*\0*.*\0",Fichier) == TRUE)
	{
		SendCTRLandAKey(CTRLU);
	    wsprintf(command,MSG_SCIMSG36,Fichier);
		StoreCommand1 (command,1);
	}
}
/*-----------------------------------------------------------------------------------*/
void Callback_SAVE(void)
{
	char Fichier[MAX_PATH];
	char command[MAX_PATH];
	
	LPTW lptw=GetTextWinScilab();
	    			
	if (OpenSaveSCIFile(lptw->hWndParent,"Save",FALSE,"Files *.sav\0*.sav\0Files *.bin\0*.bin\0All *.*\0*.*\0",Fichier) == TRUE)
	{
		SendCTRLandAKey(CTRLU);
		wsprintf(command,MSG_SCIMSG37,Fichier);
		StoreCommand1 (command,1);
	}
}
/*-----------------------------------------------------------------------------------*/
void Callback_CHDIR(void)
{
	#define NumberCharByLineInChdirBox	56
	BROWSEINFO InfoBrowserDirectory;
	char Path[MAX_PATH];
	char PathToDisplay[MAX_PATH*2];
	char command[MAX_PATH]; 
	char TextPath[MAX_PATH*2];
	
	LPITEMIDLIST pidl; 
			
	LPTW lptw=GetTextWinScilab();
		
	GetCurrentDirectory(MAX_PATH,Path);
	CutLineForDisplay(PathToDisplay,Path,NumberCharByLineInChdirBox);		
	InfoBrowserDirectory.hwndOwner = lptw->hWndParent; 
	InfoBrowserDirectory.pidlRoot = NULL; 
	switch (lptw->lpmw->CodeLanguage)
	{ 
		case 1:
		if (lstrlen(Path) < NumberCharByLineInChdirBox)
		{
			wsprintf(TextPath,"%s\n\n%s",MSG_SCIMSG38,PathToDisplay);
		}
		else
		{
			if ( lstrlen(Path) > (NumberCharByLineInChdirBox*2) )
			{
				wsprintf(TextPath,"%s",MSG_SCIMSG38);
			}
			else
			wsprintf(TextPath,"%s\n%s",MSG_SCIMSG38,PathToDisplay);
		}
		break;
				
		case 0:default:
		if (lstrlen(Path)<NumberCharByLineInChdirBox)
		{
			wsprintf(TextPath,"%s\n\n%s",MSG_SCIMSG39,PathToDisplay);
		}
		else
		{
			if ( lstrlen(Path) > (NumberCharByLineInChdirBox*2) )
			{
				wsprintf(TextPath,"%s",MSG_SCIMSG39);
			}
			else
			wsprintf(TextPath,"%s\n%s",MSG_SCIMSG39,PathToDisplay);
		}
		break;
	}
		
	InfoBrowserDirectory.lpszTitle=TextPath;
	InfoBrowserDirectory.pszDisplayName=Path; 
	InfoBrowserDirectory.ulFlags = BIF_STATUSTEXT|BIF_RETURNONLYFSDIRS; 
	InfoBrowserDirectory.lpfn =NULL;


	pidl=SHBrowseForFolder(&InfoBrowserDirectory);
	if (pidl!=NULL)
	{
	 	SHGetPathFromIDList(pidl, Path); 
	 	SendCTRLandAKey(CTRLU);
		wsprintf(command,"chdir('%s');",Path);
		StoreCommand1 (command,1);
		//WriteIntoScilab(lptw,command);
	}

}
/*-----------------------------------------------------------------------------------*/
void Callback_GETCWD(void)
{
	char *command=NULL;
	if (IsToThePrompt())
	{
		char save_prompt[10];
		SendCTRLandAKey(CTRLU);
		GetCurrentPrompt(save_prompt);
		command=(char*)MALLOC((strlen("printf('\n\n %%s\n\n%s',getcwd())")+strlen(save_prompt))*sizeof(char));
		wsprintf(command,"printf('\n\n %%s\n\n%s',getcwd())",save_prompt);
	}
	else
	{
		command=(char*)MALLOC((strlen("printf('\n\n %%s\n\n%s',getcwd())"))*sizeof(char));
		wsprintf(command,"printf('\n\n %%s\n\n',getcwd())");
	}

	StoreCommand (command);
	if (command) { FREE(command);command=NULL; }
	    		
}
/*-----------------------------------------------------------------------------------*/
void Callback_MCOPY(void)
{
	LPTW lptw=GetTextWinScilab();
	
	if ( HasAZoneTextSelected(lptw) == TRUE )TextCopyClip (lptw);
	else 
	{
		if (lptw->lpmw->CodeLanguage == 0)
		{
			MessageBox(lptw->hWndParent,MSG_SCIMSG40,MSG_SCIMSG42,MB_ICONINFORMATION);
		}
		else
		{
			MessageBox(lptw->hWndParent,MSG_SCIMSG41,MSG_SCIMSG42,MB_ICONINFORMATION);
		}
	}
}
/*-----------------------------------------------------------------------------------*/
void Callback_PASTE(void)
{
	LPTW lptw=GetTextWinScilab();
	
	if ( !IsEmptyClipboard(lptw) ) PasteFunction(lptw,FALSE);
	else 
	{
		if (lptw->lpmw->CodeLanguage == 0)
		{
			MessageBox(lptw->hWndParent,MSG_SCIMSG43,MSG_SCIMSG42,MB_ICONINFORMATION);
		}
		else
		{
			MessageBox(lptw->hWndParent,MSG_SCIMSG44,MSG_SCIMSG42,MB_ICONINFORMATION);
		}
	}
}
/*-----------------------------------------------------------------------------------*/
void Callback_PRINTSETUP(void)
{
	ConfigurePrinterDialogBox();
}
/*-----------------------------------------------------------------------------------*/
void Callback_PRINT(void)
{
	LPTW lptw=GetTextWinScilab();
	SelectAll(lptw,FALSE);
	TextCopyClip (lptw);
	UnSelect(lptw);


	if (lptw->lpmw->CodeLanguage == 0)
	{
		PrintSelection(lptw,MSG_SCIMSG44);
	}
	else
	{
		PrintSelection(lptw,MSG_SCIMSG45);
	}
}
/*-----------------------------------------------------------------------------------*/
void Callback_TOOLBAR(void)
{
	LPTW lptw=GetTextWinScilab();
	
	if (lptw->lpmw->LockToolBar == FALSE)
		{
			lptw->lpmw->ShowToolBar=!lptw->lpmw->ShowToolBar;
			ToolBarOnOff(lptw);
		}
}
/*-----------------------------------------------------------------------------------*/
void Callback_FRENCH(void)
{
	LPTW lptw=GetTextWinScilab();
	
	if (lptw->lpmw->CodeLanguage!=1)
		{
			StoreCommand1 ("setlanguage('fr');",0);
		}
}
/*-----------------------------------------------------------------------------------*/
void Callback_ENGLISH(void)
{
	LPTW lptw=GetTextWinScilab();
	
	if (lptw->lpmw->CodeLanguage!=0)
		{
			StoreCommand1 ("setlanguage('eng');",0);
		}
}
/*-----------------------------------------------------------------------------------*/
void Callback_TEXTCOLOR(void)
{
	int R,G,B;

	R=GetRValue(GetIhmTextColor());
	G=GetGValue(GetIhmTextColor());
	B=GetBValue(GetIhmTextColor());

	if (ChooseColorBox(&R,&G,&B))
	{
		LPTW lptw=GetTextWinScilab();

		SetIhmTextColor(R,G,B,TRUE);
		lptw->bSysColors=0;
	}
}
/*-----------------------------------------------------------------------------------*/
void Callback_BACKGROUNDCOLOR(void)
{
	int R,G,B;

	R=GetRValue(GetIhmTextBackgroundColor());
	G=GetGValue(GetIhmTextBackgroundColor());
	B=GetBValue(GetIhmTextBackgroundColor());

	if (ChooseColorBox(&R,&G,&B))
	{
		LPTW lptw=GetTextWinScilab();

		SetIhmTextBackgroundColor(R,G,B,TRUE);
		lptw->bSysColors=0;
	}
}
/*-----------------------------------------------------------------------------------*/
void Callback_SYSTEMCOLOR(void)
{
	LPTW lptw=GetTextWinScilab();

	lptw->bSysColors=1;
	SetIhmSystemDefaultTextColor();
	SetIhmSystemDefaultTextBackgroundColor();
}
/*-----------------------------------------------------------------------------------*/
void Callback_FILESASSOCIATIONBOX(void)
{
	if (IsWindowInterface())
	{
		LPTW lptw=GetTextWinScilab();
		FilesAssociationBox(lptw->hWndParent);
	}
	else
	{
		FilesAssociationBox(NULL);
	}
}
/*-----------------------------------------------------------------------------------*/
void Callback_CHOOSETHEFONT(void)
{
	LPTW lptw=GetTextWinScilab();

	TextSelectFont (lptw);
}
/*-----------------------------------------------------------------------------------*/
void Callback_RESTART(void)
{
	LPTW lptw=GetTextWinScilab();

	SendCTRLandAKey(CTRLU);

	ClearCommandWindow(lptw,TRUE);

	SendCTRLandAKey(CTRLU);
	StoreCommand1("abort;",1);

	SendCTRLandAKey(CTRLU);
	StoreCommand1("savehistory();",1);

	SendCTRLandAKey(CTRLU);
	StoreCommand1("resethistory();",1);

	SendCTRLandAKey(CTRLU);
	write_scilab("exec('SCI/scilab.star',-1);\n");
}
/*-----------------------------------------------------------------------------------*/
void Callback_ABORT(void)
{
	SendCTRLandAKey(CTRLU);
	StoreCommand1("abort;",1);
}
/*-----------------------------------------------------------------------------------*/
void Callback_PAUSE(void)
{
	SendCTRLandAKey(CTRLU);
	StoreCommand1("pause;",1);
}
/*-----------------------------------------------------------------------------------*/
void Callback_RESUME(void)
{
	SendCTRLandAKey(CTRLU);
	StoreCommand1("resume;",1);
	
}
/*-----------------------------------------------------------------------------------*/
void Callback_CONSOLE(void)
{
	SwitchConsole();
}
/*-----------------------------------------------------------------------------------*/
void Callback_SCIPAD(void)
{
	char Command[512];
	extern BOOL PutLineInBuffer;
	wsprintf(Command,"%cscipad();",CTRLU);

    PutLineInBuffer=FALSE;
	if (IsToThePrompt ()) StoreCommand1(Command,1);
	else StoreCommand("scipad();");
	PutLineInBuffer=FALSE;
}
/*-----------------------------------------------------------------------------------*/
void Callback_HELP(void)
{
	char Command[512];
	extern BOOL PutLineInBuffer;
	wsprintf(Command,"%chelp();",CTRLU);

	PutLineInBuffer=FALSE;
	if (IsToThePrompt ()) StoreCommand1(Command,1);
	else StoreCommand("help();");
	PutLineInBuffer=FALSE;
}
/*-----------------------------------------------------------------------------------*/
void Callback_DEMOS(void)
{
	LPTW lptw=GetTextWinScilab();

	SendCTRLandAKey(CTRLU);
	WriteIntoScilab(lptw,"exec('SCI/demos/alldems.dem');");
}
/*-----------------------------------------------------------------------------------*/
void Callback_WEB(void)
{
	int error =(int)ShellExecute(NULL, "open", URL, NULL, NULL, SW_SHOWNORMAL);
	if (error<= 32) MessageBox(NULL,MSG_WARNING24,MSG_WARNING22,MB_ICONWARNING);
}
/*-----------------------------------------------------------------------------------*/
void Callback_CONTRIBUTIONS(void)
{
	int error =(int)ShellExecute(NULL, "open", URLCONTRIBUTIONS, NULL, NULL, SW_SHOWNORMAL);
	if (error<= 32) MessageBox(NULL,MSG_WARNING24,MSG_WARNING22,MB_ICONWARNING);
}
/*-----------------------------------------------------------------------------------*/
void Callback_BUGZILLA(void)
{
	int error =(int)ShellExecute(NULL, "open", URLBUGZILLA, NULL, NULL, SW_SHOWNORMAL);
	if (error<= 32) MessageBox(NULL,MSG_WARNING24,MSG_WARNING22,MB_ICONWARNING);
}
/*-----------------------------------------------------------------------------------*/
void Callback_EMAIL(void)
{
	char Message[MAX_PATH];
	int error;
	    		
	wsprintf(Message,"mailto:%s?Subject=%s&CC=%s&body=%s",MAILTO, SUBJECT,CCMAILTO,"Message to Doctor Scilab");
	error =(int)ShellExecute(NULL, "open",Message,NULL,NULL,SW_SHOWNORMAL);
	if (error<= 32) MessageBox(NULL,MSG_WARNING25,MSG_WARNING22,MB_ICONWARNING);
}
/*-----------------------------------------------------------------------------------*/
void Callback_NEWSGROUP(void)
{
	int error =(int)ShellExecute(NULL, "open", URLNEWSGROUP, NULL, NULL, SW_SHOWNORMAL);
	if (error<= 32) MessageBox(NULL,MSG_WARNING24,MSG_WARNING22,MB_ICONWARNING);
}
/*-----------------------------------------------------------------------------------*/
void Callback_ABOUT(void)
{
	if (IsWindowInterface())
	{
		LPTW lptw=GetTextWinScilab();
		AboutBox(lptw->hWndParent);
	}
	else AboutBox(NULL);
}
/*-----------------------------------------------------------------------------------*/
void Callback_CLEARCOMMANDWINDOW(void)
{
	LPTW lptw=GetTextWinScilab();

	ClearCommandWindow(lptw,TRUE);
}
/*-----------------------------------------------------------------------------------*/
void Callback_CONFIGUREBROWSER(void)
{
	SendCTRLandAKey(CTRLU);
	StoreCommand1("help_menu(3);",0);
}
/*-----------------------------------------------------------------------------------*/
void Callback_CLEARHISTORY(void)
{
	#define MAXBUF	1024
	char Commentline[MAXBUF];

	reset_history();

	GetCommentDateSession(Commentline,TRUE);		
	AddHistory (Commentline);

}
/*-----------------------------------------------------------------------------------*/
void Callback_SELECTALL(void)
{
	LPTW lptw=GetTextWinScilab();
	SelectAll(lptw,TRUE);
}
/*-----------------------------------------------------------------------------------*/
void Callback_EMPTYCLIPBOARD(void)
{
	LPTW lptw=GetTextWinScilab();
	CleanClipboard(lptw);

}
/*-----------------------------------------------------------------------------------*/
/*********************************
 * Send a macro to the text window 
 * lptw : text window 
 * m    : macro reference in the text 
 *        window menu ( lptw->lpmw )
 * s = lpmw->macro[m]; is the macro code 
 *   the macro code is parsed in this routine 
 *   to detect special values which implies special 
 *   routine calling to get the string ( ex OPEN ) 
 *********************************/
void SendMacro (LPTW lptw, UINT m)
{

  BYTE *s;
  char *d;
  char *buf;
  BOOL flag = TRUE;
 
  LPMW lpmw = lptw->lpmw;

  if ((buf = LocalAlloc (LPTR, MAXSTR + 1)) == (char *) NULL) return;

  if (m >= NUMMENU || (int)m < 0) return;

  s = lpmw->macro[m];
  d = buf;
  *d = '\0';
  while (s && *s && (d - buf < MAXSTR))
    {
      if (*s >= CMDMIN && *s <= CMDMAX)
	{
	  /* int ierr = 0; */
	  /** Special cases **/
	  switch (*s)
	    {
	    	case NEWSCILAB:
	    		Callback_NEWSCILAB();
	    		return;
	    	break;
			case OPEN:
				SaveCurrentLine(TRUE);
				Callback_OPEN();
				return;
	    	case EXEC:
	   			Callback_EXEC();
	   		return;
	    		break;
	    	case GETF:
	    		Callback_GETF();
	    		return;
	    	break;
	    	case LOAD:
	    		Callback_LOAD();
	    		return;
	    	break;	
	    	case SAVE:
	    		Callback_SAVE();
	    		return;
	    	break;
	    	case CHDIR:
	    		Callback_CHDIR();
	    		return;
	    	break;
	    	case GETCWD:
	    		Callback_GETCWD();
	    		return;
	    	break;
	    	case EXIT:
	    	        ExitWindow();
	    		return;
	    	break;
			case SELECTALL:
				Callback_SELECTALL();
				return;
			break;
	    	case MCOPY:
	    		Callback_MCOPY();
			s++;
	    	break;
	    	case PASTE:
	    		Callback_PASTE();
	    	        return;
	    	break; 
	    	case EMPTYCLIPBOARD:
				Callback_EMPTYCLIPBOARD();
				return;
			break;
			case PRINTSETUP:
				Callback_PRINTSETUP();
				return;
				break;
	    	case PRINT:
	    		Callback_PRINT();
	    		return;
	    	break;
	    	case TOOLBAR:
	    		Callback_TOOLBAR();
	    		return;
	    	break;
	    	case FRENCH:
	    		Callback_FRENCH();
	    		return;
	    	break;
	    	case ENGLISH:
	    		Callback_ENGLISH();
	    		return;
	    	break;
			case TEXTCOLOR:
				Callback_TEXTCOLOR();
				return;
			case BACKGROUNDCOLOR:
				Callback_BACKGROUNDCOLOR();
				return;
			case SYSTEMCOLOR:
				Callback_SYSTEMCOLOR();
				return;
			case FILESASSOCIATION:
				Callback_FILESASSOCIATIONBOX();
				return;
	    	case CHOOSETHEFONT:
	    		Callback_CHOOSETHEFONT();
	      		s++;
	    	break;
	    	case RESTART:
	    		Callback_RESTART();
	    		return;
	    	break;
	    	case ABORT:
	    		Callback_ABORT();
	    		return;
	    	break;
	    	case PAUSE:
	    		Callback_PAUSE();
	    		return;
	    	break;
	    	case RESUME:
			Callback_RESUME();	    		
	    		return;
	    	break;
	    	case CONSOLE:
	      		Callback_CONSOLE();
	      		return;
	    	break;
	       	case SCIPAD:
				SaveCurrentLine(TRUE);
	    		Callback_SCIPAD();
	    		return;
	    	break;
	    	case HELP:
				SaveCurrentLine(TRUE);
				Callback_HELP();
	    		return;
	    	break;
			case CONFIGBROWSER:
				SaveCurrentLine(TRUE);
				Callback_CONFIGUREBROWSER();
				return;
			break;
			case CLEARHISTORY:
				Callback_CLEARHISTORY();
				return;
			break;
	    	case DEMOS:
	    		Callback_DEMOS();
	    		return;
	    	break;
	    	case WEB:
			Callback_WEB();
			return;
	    	break;
			case CONTRIBUTIONS:
	    		Callback_CONTRIBUTIONS();
	    		return;
	    	break;
	    	case BUGZILLA:
	    		Callback_BUGZILLA();
	    		return;
	    	break;
	    	case EMAIL:
	    		Callback_EMAIL();
 	    		return;
	    	break;
	    	case NEWSGROUP:
	    		Callback_NEWSGROUP();
	    		return;
	    	break;
	    	case ABOUT:
	    		Callback_ABOUT();
	    		return;
	    	break;
	    	case M_CTRL_C:
	    	{
	    		SignalCtrC ();
	    		write_scilab("\n");
	    	}
	    		return;
	    	break;
	    	
	    	case M_CTRL_U:
	    	{
	    		 SendCTRLandAKey(CTRLU);
	    	}
	    		return;
	    	break;
	    	
		case M_CTRL_P:
	    	{
	    		 SendCTRLandAKey(CTRLP);
	    	}
	    		return;
	    	break;
		case M_CTRL_B:
	    	{
	    		 SendCTRLandAKey(CTRLB);
	    	}
	    		return;
	    	break;	
		case M_CTRL_F:
	    	{
	    		 SendCTRLandAKey(CTRLF);
	    	}
	    		return;
	    	break;	
		case M_CTRL_A:
	    	{
	    		 SendCTRLandAKey(CTRLA);
	    	}
	    		return;
	    	break;	
		case M_CTRL_E:
	    	{
	    		 SendCTRLandAKey(CTRLE);
	    	}
	    		return;
	    	break;	
		case M_CTRL_H:
	    	{
	    		 SendCTRLandAKey(CTRLH);
	    	}
	    		return;
	    	break;	
		case M_CTRL_D:
	    	{
	    		 SendCTRLandAKey(CTRLD);
	    	}
	    		return;
	    	break;	
		case M_CTRL_W:
	    	{
	    		 SendCTRLandAKey(CTRLW);
	    	}
	    		return;
	    	break;	
		case M_CTRL_K:
	    	{
	    		 SendCTRLandAKey(CTRLK);
	    	}
	    		return;
	    	break;	
		case M_CTRL_L:
	    	{
	    		 SendCTRLandAKey(CTRLL);
	    	}
	    	return;
	    	break;	
	    	case CLEARCOMMANDWINDOW:
	    	{
	    		Callback_CLEARCOMMANDWINDOW();
	    	}
	    	return;
	    	break;	
		
		return;
	    	break;	
	    	case EOS:		/* [EOS] - End Of String - do nothing */
	    	default:
	      		s++;
	      	break;
   	
	    
	    }
	  if (!flag)
	    {			/* abort */
	      d = buf;
	      s = (BYTE *) "";
	    }
	}
      else
	{
	  *d++ = *s++;
	}
    }
  *d = '\0';
/** Now we can send the result to the textwindow **/
  ScilabMenuAction (buf);
  LocalFree (buf);
}
/*-----------------------------------------------------------------------------------*/
/******************************
 * change the menu associated to the current 
 * graphic window 
 ******************************/
void MenuFixCurrentWin (int ivalue)
{
  static int firstentry = 0, top = -1;
  int i;
  if (firstentry == 0)
    {
      strcpy (gwin_name, "&Graphic Window 0");
      firstentry++;
    }
  if (lab_count != ivalue)
    {
      C2F (xscion) (&i);
      if (i == 1)
	{
	  lab_count = ivalue;
	  strcpy (gwin_name1, gwin_name);
	  sprintf (gwin_name, "&Graphic Window %d", (int) ivalue);
	  C2F (chmenu) (&top, gwin_name1, gwin_name);
	}
    }
}
/*-----------------------------------------------------------------------------------*/
/****************************************
 * Actions for buttons of the graphic window
 * menu 
 ****************************************/
static void Countp ()
{
  MenuFixCurrentWin (lab_count + 1);
}
/*-----------------------------------------------------------------------------------*/
static void Countm ()
{
  MenuFixCurrentWin ((lab_count == 0) ? 0 : lab_count - 1);
}
/*-----------------------------------------------------------------------------------*/
static void SendCountSet ()
{
  scig_sel(lab_count);
}
/*-----------------------------------------------------------------------------------*/
static void SendCountRaise()
{
  scig_raise(lab_count);
}

/*-----------------------------------------------------------------------------------*/
static void SendCountDelete ()
{
  DeleteSGWin (lab_count);
}
/*-----------------------------------------------------------------------------------*/
/*********************************
 * File handling : for menu file 
 *********************************/
GFILE * Gfopen (LPSTR lpszFileName, int fnOpenMode)
{
  GFILE *gfile;
  gfile = (GFILE *) LocalAlloc (LPTR, sizeof (GFILE));
  if (!gfile)
    return NULL;
  gfile->hfile = _lopen (lpszFileName, fnOpenMode);
  if (gfile->hfile == HFILE_ERROR)
    {
      LocalFree (gfile);
      return NULL;
    }
  gfile->getleft = 0;
  gfile->getnext = 0;
  return gfile;
}
/*-----------------------------------------------------------------------------------*/
void Gfclose (GFILE * gfile)
{
  _lclose (gfile->hfile);
  LocalFree (gfile);
  return;
}
/*-----------------------------------------------------------------------------------*/
/* returns number of characters read */
int Gfgets (LPSTR lp, int size, GFILE * gfile)
{
  int i;
  int ch;
  for (i = 0; i < size; i++)
    {
      if (gfile->getleft <= 0)
	{
	  if ((gfile->getleft
	       = _lread (gfile->hfile, gfile->getbuf, GBUFSIZE)) == 0)
	    break;
	  gfile->getnext = 0;
	}
      ch = *lp++ = gfile->getbuf[gfile->getnext++];
      gfile->getleft--;
      if (ch == '\r')
	{
	  i--;
	  lp--;
	}
      if (ch == '\n')
	{
	  i++;
	  break;
	}
    }
  if (i < size)
    *lp++ = '\0';
  return i;
}
/*-----------------------------------------------------------------------------------*/
/* Get a line from the menu file */
/* Return number of lines read from file including comment lines */
int GetLine (char *buffer, int len, GFILE * gfile)
{
  BOOL status;
  int nLine = 0;

  status = (Gfgets (buffer, len, gfile) != 0);
  nLine++;
  while (status && (buffer[0] == '\0' || buffer[0] == '\n' || buffer[0] == ';'))
    {
      /* blank line or comment - ignore */
      status = (Gfgets (buffer, len, gfile) != 0);
      nLine++;
    }
  if (lstrlen (buffer) > 0)
    buffer[lstrlen (buffer) - 1] = '\0';	/* remove trailing \n */
  if (!status)
    nLine = 0;			/* zero lines if file error */
  return nLine;
}
/*-----------------------------------------------------------------------------------*/
/* Left justify string */
void LeftJustify (char *d, char *s)
{
  while (*s && (*s == ' ' || *s == '\t'))
    s++;			/* skip over space */
  do
    {
      *d++ = *s;
    }
  while (*s++);
}
/*-----------------------------------------------------------------------------------*/
/* Translate string to tokenized macro */
void TranslateMacro (char *string)
{
  int i, len;
  LPSTR ptr;
  for (i = 0; keyword[i] != (char *) NULL; i++)
    {
      if ((ptr = strstr (string, keyword[i])) != NULL)
	{
	  len = lstrlen (keyword[i]);
	  *ptr = keyeq[i];
	  lstrcpy (ptr + 1, ptr + len);
	  i--;			/* allows for more than one occurrence of keyword */
	}
    }
}
/*-----------------------------------------------------------------------------------*/
/**************************************************
 * Load Macros, and create Menu from Menu file
 **************************************************/
void LoadMacros (LPTW lptw)
{
  GFILE *menufile;
  BYTE *macroptr;
  char *buf;
  int nMenuLevel;
  HMENU hMenu[MENUDEPTH + 1+1];
  LPMW lpmw;
  int nLine = 1;
  int nInc;
  int nCountMenu;
  HGLOBAL hmacro;
  int i;
  HDC hdc;
  TEXTMETRIC tm;
  RECT rect;
  int ButtonX, ButtonY;
  char *ButtonText[BUTTONMAX];

  lpmw = lptw->lpmw;

  /* mark all buffers and menu file as unused */
  buf = (char *) NULL;
  hmacro = 0;
  lpmw->macro = (BYTE **) NULL;
  lpmw->szPrompt = (char *) NULL;
  lpmw->szAnswer = (char *) NULL;
  menufile = (GFILE *) NULL;

  /* open menu file */
  if ((menufile = Gfopen (lpmw->szMenuName, OF_READ)) == (GFILE *) NULL)
    goto errorcleanup;

  /* allocate buffers */
  if ((buf = LocalAlloc (LPTR, MAXSTR)) == (char *) NULL)
    goto nomemory;
  hmacro = GlobalAlloc (GHND, (NUMMENU) * sizeof (BYTE *));
  if ((lpmw->macro = (BYTE **) GlobalLock (hmacro)) == (BYTE **) NULL)
    goto nomemory;
  if ((lpmw->szPrompt = LocalAlloc (LPTR, MAXSTR)) == (char *) NULL)
    goto nomemory;
  if ((lpmw->szAnswer = LocalAlloc (LPTR, MAXSTR)) == (char *) NULL)
    goto nomemory;

  /* Initialize macro array * */
  for (i = 0; i < NUMMENU; i++)
    {
      lpmw->macro[i] = (BYTE *) 0;
    }
  lpmw->nCountMenu = 0;		/* counts predefined menus * */
  lpmw->nButton = 0;
  lpmw->hMenu = hMenu[0] = CreateMenu ();
  nMenuLevel = 0;

  while ((nInc = GetLine (buf, MAXSTR, menufile)) != 0)
    {
      nLine += nInc;
      LeftJustify (buf, buf);
      if (buf[0] == '\0')
	{
	  /* ignore blank lines */
	}
      else if (!lstrcmpi (buf, "[Menu]"))
	{
	  /* new menu */
	  if (!(nInc = GetLine (buf, MAXSTR, menufile)))
	    {
	      nLine += nInc;
	      BUGGOTOCLEAN ("Problem on line %d of %s\n");
	    }
	  LeftJustify (buf, buf);
	  if (nMenuLevel < MENUDEPTH+1)
	    nMenuLevel++;
	  else
	    {
	      BUGGOTOCLEAN (MSG_ERROR43);
	    }
	  hMenu[nMenuLevel] = CreateMenu ();
	  AppendMenu (hMenu[nMenuLevel > 0 ? nMenuLevel - 1 : 0],
		      MF_STRING | MF_POPUP, (UINT) hMenu[nMenuLevel],
		      (LPCSTR) buf);
	}
      else if (!lstrcmpi (buf, "[EndMenu]"))
	{
	  if (nMenuLevel > 0)
	    nMenuLevel--;	/* back up one menu */
	}
      else if (!lstrcmpi (buf, "[Button]"))
	{
	  /* button macro */
	  if (lpmw->nButton >= BUTTONMAX)
	    {
	      BUGGOTOCLEAN (MSG_ERROR44);
	    }
	  if (!(nInc = GetLine (buf, MAXSTR, menufile)))
	    {
	      nLine += nInc;
	      BUGGOTOCLEAN (MSG_ERROR45);
	    }
	  LeftJustify (buf, buf);
	  if ((macroptr = LocalAlloc (LPTR, lstrlen (buf) + 1)) != (BYTE *) NULL)
	    {
	      lstrcpy ((char *) macroptr, buf);
	    }
	  else
	    {
	      BUGGOTOCLEAN (MSG_ERROR46);
	    }
	  ButtonText[lpmw->nButton] = (char *) macroptr;
	  if (!(nInc = GetLine (buf, MAXSTR, menufile)))
	    {
	      nLine += nInc;
	      BUGGOTOCLEAN (MSG_ERROR45);
	    }
	  LeftJustify (buf, buf);
	  TranslateMacro (buf);
	  if ((macroptr = LocalAlloc (LPTR, lstrlen (buf) + 1)) != (BYTE *) NULL)
	    lstrcpy ((char *) macroptr, buf);
	  else
	    {
	      BUGGOTOCLEAN (MSG_ERROR46);
	    }
	  nCountMenu = WGFindMenuPos (lpmw->macro);
	  lpmw->nCountMenu++;
	  lpmw->hButtonID[lpmw->nButton] = nCountMenu;
	  lpmw->macro[nCountMenu] = macroptr;
	  lpmw->nButton++;
	}
      else
	{
	  nCountMenu = WGFindMenuPos (lpmw->macro);
	  /* menu item */
	  if (nCountMenu >= NUMMENU)
	    {
	      BUGGOTOCLEAN (MSG_ERROR44);
	    }
	  LeftJustify (buf, buf);
	  if (buf[0] == '-')
	    {
	      if (nMenuLevel == 0)
		AppendMenu (hMenu[0], MF_MENUBREAK, 0, (LPSTR) NULL);
	      else
		AppendMenu (hMenu[nMenuLevel], MF_SEPARATOR, 0, (LPSTR) NULL);
	    }
	  else if (buf[0] == '|')
	    {
	      AppendMenu (hMenu[nMenuLevel], MF_MENUBARBREAK, 0, (LPSTR) NULL);
	    }
	  else
	    {
	      AppendMenu (hMenu[nMenuLevel], MF_STRING, nCountMenu, (LPSTR) buf);
	      if (!(nInc = GetLine (buf, MAXSTR, menufile)))
		{
		  nLine += nInc;
		  BUGGOTOCLEAN (MSG_ERROR45);
		}
	      LeftJustify (buf, buf);
	      TranslateMacro (buf);
	      if ((macroptr = LocalAlloc (LPTR, lstrlen (buf) + 1)) != (BYTE *) NULL)
		{
		  lstrcpy ((char *) macroptr, buf);
		}
	      else
		{
		  BUGGOTOCLEAN ("Out of space for storing menu macros\n at line %d of %s\n");
		}
	      lpmw->macro[nCountMenu] = macroptr;
	      lpmw->nCountMenu++;
	    }
	}
    }
  nCountMenu = WGFindMenuPos (lpmw->macro);
  if ((nCountMenu - lpmw->nButton) > 0)
    {
      /* we have a menu bar so put it on the window */
      SetMenu (lptw->hWndParent, lpmw->hMenu);
      DrawMenuBar (lptw->hWndParent);
    }

  if (!lpmw->nButton)  goto cleanup;		/* no buttons */
    


  /* calculate size of buttons */
  hdc = (HDC)TryToGetDC (lptw->hWndParent);
  
  SelectFont (hdc, GetStockFont (SYSTEM_FIXED_FONT));
  GetTextMetrics (hdc, &tm);
  ButtonX = 10 * tm.tmAveCharWidth;
  //ButtonY = 6 * (tm.tmHeight + tm.tmExternalLeading) / 4;
  ButtonY = 24;
  ReleaseDC (lptw->hWndParent, hdc);

  /* move top of client text window down to allow space for buttons */
  lptw->ButtonHeight = ButtonY + 1;
  GetClientRect (lptw->hWndParent, &rect);
  SetWindowPos (lptw->hWndText, (HWND) NULL, 0, lptw->ButtonHeight,
		rect.right, rect.bottom - lptw->ButtonHeight,
		SWP_NOZORDER | SWP_NOACTIVATE);

  /* create the buttons */

  lpmw->lpfnMenuButtonProc = (WNDPROC) MyGetProcAddress ("MenuButtonProc", MenuButtonProc);
  for (i = 0; i < lpmw->nButton; i++)
    {
    	CreateButton(lptw,ButtonText, i,ButtonX, ButtonY);
    }
   

  goto cleanup;

nomemory:
  MessageBox (lptw->hWndParent, MSG_ERROR40, lptw->Title, MB_ICONEXCLAMATION);

errorcleanup:
  if (hmacro)
    {
      for (i = 0; i < NUMMENU; i++)
	{
	  if (lpmw->macro[i] != (BYTE *) 0)
	    {
	      LocalFree (lpmw->macro[i]);
	      lpmw->macro[i] = (BYTE *) 0;
	    }
	}
      GlobalUnlock (hmacro);
      GlobalFree (hmacro);
      lpmw->macro = (BYTE **) NULL;
    }
  if (lpmw->szPrompt != (char *) NULL)
    {
      LocalFree (lpmw->szPrompt);
      lpmw->szPrompt = (char *) NULL;
    }
  if (lpmw->szAnswer != (char *) NULL)
    {
      LocalFree (lpmw->szAnswer);
      lpmw->szAnswer = (char *) NULL;
    }

cleanup:
  if (buf != (char *) NULL)
    LocalFree (buf);
  if (menufile != (GFILE *) NULL)
    Gfclose (menufile);
  return;
}
/*-----------------------------------------------------------------------------------*/
void CreateButton(LPTW lptw, char *ButtonText[BUTTONMAX], int index,int ButtonSizeX, int ButtonSizeY)
{
	LPMW lpmw;
        lpmw = lptw->lpmw;
        
        if (strcmp(ButtonText[index],"--")==0)
        {
        	lpmw->IsAIcon[index]=TRUE;
        	GetXPosButton(lptw,index, 24,ButtonSizeY);
        	
        }
        else
        if ( ButtonText[index][0]=='|')
        {
                char LineTmp[MAX_PATH];
                char NameButton[MAX_PATH];
                char PathIconButton[MAX_PATH];
                char *Token=NULL;
                HICON IconButton;
                
                int i=0;
        
                
                
                strcpy(LineTmp,ButtonText[index]);
        
                Token=strtok(LineTmp,"|");
                strcpy(NameButton,Token);
                
                Token=strtok(NULL,"|");
                strcpy(PathIconButton,Token);
		
                if ( ( IsAFile(PathIconButton)==TRUE)                ||
                     ( strcmp(PathIconButton,"METANET_ICON")==0)     ||
                     ( strcmp(PathIconButton,"SCICOS_ICON")==0)      ||
                     ( strcmp(PathIconButton,"PRINT_ICON")==0)       ||
                     ( strcmp(PathIconButton,"CONSOLE_ICON")==0)     ||
                     ( strcmp(PathIconButton,"DIRECTORY_ICON")==0)   ||
                     ( strcmp(PathIconButton,"EXIT_ICON")==0)        ||
                     ( strcmp(PathIconButton,"FONT_ICON")==0)        ||
                     ( strcmp(PathIconButton,"HELP_ICON")==0)        ||
					 ( strcmp(PathIconButton,"NEW_ICON")==0)         ||
					 ( strcmp(PathIconButton,"COPY_ICON")==0)        ||
					 ( strcmp(PathIconButton,"PASTE_ICON")==0)       ||
					 ( strcmp(PathIconButton,"OPEN_ICON")==0)       ||
                     ( strcmp(PathIconButton,"SCILAB_ICON")==0)  )
                {
                        
                        if ( IsAFile(PathIconButton) == TRUE )
                        {
                          IconButton=(HICON)LoadImage(  GetModuleHandle(NULL), PathIconButton,IMAGE_ICON,24,24, LR_DEFAULTCOLOR|LR_LOADFROMFILE);
                        }
                        else                      
                        if ( strcmp(PathIconButton,"SCICOS_ICON")==0 )
                        {
							IconButton=(HICON)LoadImage( hdllInstance, (LPCSTR)IDI_SCICOS24,IMAGE_ICON,24,24, LR_DEFAULTCOLOR);
                          
                        }
                        else
                        if ( strcmp(PathIconButton,"METANET_ICON")==0 )
                        {
						  IconButton=(HICON)LoadImage( hdllInstance, (LPCSTR)IDI_METANET24,IMAGE_ICON,24,24, LR_DEFAULTCOLOR);
                          
                        }
                        if ( strcmp(PathIconButton,"PRINT_ICON")==0 )
                        {
							IconButton=(HICON)LoadImage( hdllInstance, (LPCSTR)IDI_PRINT,IMAGE_ICON,24,24, LR_DEFAULTCOLOR);
                        }
                        else
                     	if ( strcmp(PathIconButton,"CONSOLE_ICON")==0 )
                     	{
							IconButton=(HICON)LoadImage( hdllInstance, (LPCSTR)IDI_CONSOLE,IMAGE_ICON,24,24, LR_DEFAULTCOLOR);
                     	}
                     	else
                     	if ( strcmp(PathIconButton,"DIRECTORY_ICON")==0 )
                     	{
							IconButton=(HICON)LoadImage( hdllInstance, (LPCSTR)IDI_CHDIR,IMAGE_ICON,24,24, LR_DEFAULTCOLOR);
                     	}
                     	else
                     	if ( strcmp(PathIconButton,"EXIT_ICON")==0 )
                     	{
							IconButton=(HICON)LoadImage( hdllInstance, (LPCSTR)IDI_EXIT24,IMAGE_ICON,24,24, LR_DEFAULTCOLOR);
                     	}
                     	else
                     	if ( strcmp(PathIconButton,"FONT_ICON")==0 )
                     	{
							IconButton=(HICON)LoadImage( hdllInstance, (LPCSTR)IDI_FONTS,IMAGE_ICON,24,24, LR_DEFAULTCOLOR);
                     	}
                     	else
                     	if ( strcmp(PathIconButton,"HELP_ICON")==0 )
                     	{
							IconButton=(HICON)LoadImage( hdllInstance, (LPCSTR)IDI_HELP,IMAGE_ICON,24,24, LR_DEFAULTCOLOR);
                     	}
                     	else
                     	if ( strcmp(PathIconButton,"SCILAB_ICON")==0 )
                     	{
							IconButton=(HICON)LoadImage( hdllInstance, (LPCSTR)IDI_SCILAB24,IMAGE_ICON,24,24, LR_DEFAULTCOLOR);
                     	}
						else
						if ( strcmp(PathIconButton,"NEW_ICON")==0 )
                     	{
							IconButton=(HICON)LoadImage( hdllInstance, (LPCSTR)IDI_NEWFILE,IMAGE_ICON,24,24, LR_DEFAULTCOLOR);
                     	}
						else
						if ( strcmp(PathIconButton,"COPY_ICON")==0 )
                     	{
							IconButton=(HICON)LoadImage( hdllInstance, (LPCSTR)IDI_COPY,IMAGE_ICON,24,24, LR_DEFAULTCOLOR);
                     	}
						else
						if ( strcmp(PathIconButton,"PASTE_ICON")==0 )
                     	{
							IconButton=(HICON)LoadImage( hdllInstance, (LPCSTR)IDI_PASTE,IMAGE_ICON,24,24, LR_DEFAULTCOLOR);
                     	}
						else
						if ( strcmp(PathIconButton,"OPEN_ICON")==0 )
                     	{
							IconButton=(HICON)LoadImage( hdllInstance, (LPCSTR)IDI_OPEN,IMAGE_ICON,24,24, LR_DEFAULTCOLOR);
                     	}

						lpmw->IsAIcon[index]=TRUE;
                        lpmw->hButton[index] = CreateWindow("button",NameButton,WS_CHILD|WS_VISIBLE|BS_ICON ,
                                       GetXPosButton(lptw,index, 24,ButtonSizeY), 0,
                                       24, ButtonSizeY,
                                       lptw->hWndParent, (HMENU) index,
                                       lptw->hInstance, lptw);

                        /* Associe une infobulle */
                        CreateMyTooltip (lpmw->hButton[index], NameButton);     
               
                
                        lpmw->lpfnButtonProc[index] = (WNDPROC) GetWindowLong (lpmw->hButton[index], GWL_WNDPROC);
                        SetWindowLong(lpmw->hButton[index], GWL_WNDPROC, (LONG) lpmw->lpfnMenuButtonProc);
        
                        /*Associe icone */
                        SendMessage(lpmw->hButton[index],BM_SETIMAGE, IMAGE_ICON, (LPARAM)IconButton);
			
                }
                else
                {
                        char MsgErr[MAX_PATH];
                        wsprintf(MsgErr,MSG_ERROR47,ButtonText[index]);
                        MessageBox(NULL,MsgErr,MSG_ERROR48,MB_ICONEXCLAMATION);
                        exit(1);
                }     
        

        
        }
        else
        {
             lpmw->IsAIcon[index]=FALSE;                        	
             lpmw->hButton[index] = CreateWindow ("button",ButtonText[index],WS_CHILD|WS_VISIBLE,
                                       		  GetXPosButton(lptw,index,ButtonSizeX,ButtonSizeY), 0,
                                       		  ButtonSizeX, ButtonSizeY,
                                       		  lptw->hWndParent, (HMENU) index,
                                       		  lptw->hInstance, lptw);
           /* Associe une infobulle */
             CreateMyTooltip (lpmw->hButton[index], ButtonText[index]);    
             lpmw->lpfnButtonProc[index] = (WNDPROC) GetWindowLong (lpmw->hButton[index], GWL_WNDPROC);
             SetWindowLong (lpmw->hButton[index], GWL_WNDPROC, (LONG) lpmw->lpfnMenuButtonProc);
             
        }
}      
      

/*-----------------------------------------------------------------------------------*/
int GetXPosButton(LPTW lptw,int index,int SizeXButtonText,int SizeXButtonIcon)
{
	LPMW lpmw;
	int PosXButton=0;
        lpmw = lptw->lpmw;
 	
	if (index==0) lpmw->PositionX[index]=0;
	else
		{
		 if (lpmw->IsAIcon[index-1]==TRUE)
		 {
		 	lpmw->PositionX[index]=lpmw->PositionX[index-1]+24;
		 }
		 else
		 {
			 	lpmw->PositionX[index]=lpmw->PositionX[index-1]+80;
		 }
		}
        return lpmw->PositionX[index];
}
/*-----------------------------------------------------------------------------------*/
/***********************************************************************
 * MenuButtonProc() -  Message handling routine for Menu Buttons
 ***********************************************************************/
 /* Modifications Allan CORNET ToolBar */
EXPORT LRESULT CALLBACK MenuButtonProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  LPTW lptw;
  LPMW lpmw;
  LONG n = GetWindowLong (hwnd, GWL_ID);
  
  lptw = (LPTW) GetWindowLong (GetParent (hwnd), 0);
  lpmw = lptw->lpmw;



  switch (message)
    {
    	
    case WM_LBUTTONUP:
      {
	RECT rect;
	POINT pt;
	GetWindowRect (hwnd, &rect);
	GetCursorPos (&pt);
	if (PtInRect (&rect, pt)) SendMessage (lptw->hWndText, WM_COMMAND, lpmw->hButtonID[n], 0L);
	SetFocus (lptw->hWndText);
      }
      break;
     
     break;
      
    }
  return CallWindowProc ((lpmw->lpfnButtonProc[n]), hwnd, message, wParam, lParam);
}


/*-----------------------------------------------------------------------------------*/
/************************************
 * Cleaning everything : used 
 * when exiting Scilab  XXXXXX
 ************************************/
void CloseMacros (LPTW lptw)
{
  int i;
  HGLOBAL hglobal;
  LPMW lpmw;
  lpmw = lptw->lpmw;
  
  
  if (lpmw->macro != (BYTE **) NULL)
    {
      hglobal = GlobalHandle (lpmw->macro);
      if (hglobal)
	{
	  for (i = 0; i < NUMMENU; i++)
	    if (lpmw->macro[i] != (BYTE *) 0)
	      {
		LocalFree (lpmw->macro[i]);
		lpmw->macro[i] = (BYTE *) 0;
	      }
	  GlobalUnlock (hglobal);
	  GlobalFree (hglobal);
	  lpmw->macro = (BYTE **) NULL;
	}
    }
  if (lpmw->szPrompt != (char *) NULL)
    {
      LocalFree (lpmw->szPrompt);
      lpmw->szPrompt = (char *) NULL;
    }
  if (lpmw->szAnswer != (char *) NULL)
    {
      LocalFree (lpmw->szAnswer);
      lpmw->szAnswer = (char *) NULL;
    }
}
/*-----------------------------------------------------------------------------------*/
/***********************************************************************
 * InputBoxDlgProc() -  Message handling routine for Input dialog box   
 ***********************************************************************/
EXPORT BOOL CALLBACK InputBoxDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  LPTW lptw;
  LPMW lpmw;
  lptw = (LPTW) GetWindowLong (GetParent (hDlg), 0);
  lpmw = lptw->lpmw;

  switch (message)
    {
    case WM_INITDIALOG:
      SetDlgItemText (hDlg, ID_PROMPT, lpmw->szPrompt);
      return (TRUE);

    case WM_COMMAND:
      switch (LOWORD (wParam))
	{
	case ID_ANSWER:
	  return (TRUE);

	case IDOK:
	  lpmw->nChar = GetDlgItemText (hDlg, ID_ANSWER, lpmw->szAnswer, MAXSTR);
	  EndDialog (hDlg, TRUE);
	  return (TRUE);

	case IDCANCEL:
	  lpmw->szAnswer[0] = '\0';
	  EndDialog (hDlg, FALSE);
	  return (TRUE);

	default:
	  return (FALSE);
	}
    default:
      return (FALSE);
    }
}
/*-----------------------------------------------------------------------------------*/

BOOL OpenSaveSCIFile(HWND hWndParent,char *titre,BOOL read,char *FileExt,char *file)
{
	BOOL Retour=FALSE;
	char szBuffer[MAX_PATH] = "";
	OPENFILENAME OFN;
		
	ZeroMemory(&OFN, sizeof(OFN));
  OFN.lStructSize = sizeof(OFN);
  
  OFN.hwndOwner = hWndParent;
  OFN.hInstance = NULL;
  OFN.lpstrFilter = FileExt;
  OFN.nFilterIndex=1;
  OFN.lpstrFile = NULL;/*FILE NAME*/
  OFN.nMaxFile = 0;/*SIZE OF FILE NAME*/
  OFN.lpstrFile = szBuffer;
  OFN.nMaxFile = MAX_PATH;
  OFN.lpstrTitle = titre;
 
  if (read == TRUE)
	{
	 	/* Lecture */
	 	OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	  if( GetOpenFileName( &OFN ) )
  	{
   			lstrcpy(file,OFN.lpstrFile);
      	Retour=TRUE;
		}
		else Retour=FALSE;
	}
	else
	{
	 	/* Ecriture */
	 	OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR|OFN_HIDEREADONLY;
	  if(GetSaveFileName ( &OFN ) )
    {
			lstrcpy(file,OFN.lpstrFile);
			Retour=TRUE;
		}
		else Retour=FALSE;
	}
	
	return Retour;
}
/*-----------------------------------------------------------------------------------*/
/* Remplace \ par / dans un chemin */
void ReplaceSlash(char *pathout,char *pathin)
{
	int i=0;
	for (i=0;i < (int)strlen(pathin);i++)
		{

			if (pathin[i]=='\\')
			{
				pathout[i]='/';
			}
			else
			{
				pathout[i]=pathin[i];
			}

		}
	pathout[i]='\0';
}
/*-----------------------------------------------------------------------------------*/
/****************************************
 * Get a filename from system menu 
 * menu title and suffixes are read from s
 * the filename is stored in d 
 * d and s are both incremented 
 * flag == FALSE : in case of cancel 
 ****************************************/

BOOL SciOpenSave (HWND hWndParent, BYTE ** s,BOOL save, char **d, int *ierr)
{
  int i, nChar;
  OPENFILENAME ofn;
  char *szTitle;
  char *szFile;
  char *szFileTitle;
  char *szFilter;
  BOOL flag;

  NEWSTRING (szTitle);
  NEWSTRING (szFile);
  NEWSTRING (szFileTitle);
  NEWSTRING (szFilter);

  (*s)++;
  for (i = 0; (**s >= 32 && **s <= 126); i++)
  {
    szTitle[i] = **s;
   (*s)++;
  }				/* get dialog box title */
  szTitle[i] = '\0';
  (*s)++;
  for (i = 0; (**s >= 32 && **s <= 126); i++)
  {
    szFile[i] = **s;
    (*s)++;
  }				/* temporary copy of filter */
  szFile[i++] = '\0';
  lstrcpy (szFilter, "Default (");
  lstrcat (szFilter, szFile);
  lstrcat (szFilter, ")");
  i = lstrlen (szFilter);
  i++;				/* move past NULL */
  lstrcpy (szFilter + i, szFile);
  i += lstrlen (szFilter + i);
  i++;				/* move past NULL */
  lstrcpy (szFilter + i, "All Files (*.*)");
  i += lstrlen (szFilter + i);
  i++;				/* move past NULL */
  lstrcpy (szFilter + i, "*.*");
  i += lstrlen (szFilter + i);
  i++;				/* move past NULL */
  szFilter[i++] = '\0';		/* add a second NULL */
  /* the Windows 3.1 implentation - MC */
  szFile[0] = '\0';

  /* clear the structrure */
	ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof (ofn);
  ofn.hwndOwner = hWndParent;
  ofn.lpstrFilter = szFilter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = MAXSTR;
  ofn.lpstrFileTitle = szFileTitle;
  ofn.nMaxFileTitle = MAXSTR;
  ofn.lpstrTitle = szTitle;
  ofn.lpstrInitialDir = (LPSTR) NULL;

	if (save)
	{
		ofn.Flags = OFN_PATHMUSTEXIST;
		flag=GetSaveFileName (&ofn);
	}
	else
	{
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		flag=GetOpenFileName (&ofn);
	}

	if (flag)
	{
		nChar = lstrlen (ofn.lpstrFile);
		for (i = 0; i < nChar; i++)
		{
			**d = ofn.lpstrFile[i];
			(*d)++;
		}
	}
  
  FREE (szTitle);
  FREE (szFilter);
  FREE (szFile);
  FREE (szFileTitle);

  return (flag);
}
/*-----------------------------------------------------------------------------------*/
/* Envoye le signal CTRL + le code d'une touche */
void SendCTRLandAKey(int code)
{
	extern TW textwin;
	char command[MAX_PATH];
	char *d;
	
	wsprintf(command,"%c",(char)code);
	if ( IsToThePrompt ()  && (command[0] != '\0') )
	{
		d = command;
		while (*d)
			{
				SendMessage (textwin.hWndText, WM_CHAR, *d, 1L);
				d++;
			}
	}
}
/*-----------------------------------------------------------------------------------*/

void ReLoadMenus(LPTW lptw)
{
	int i=0;	      		
	LPMW lpmw;      		

	lpmw = lptw->lpmw;
	
	/* Destruction des menus */
	CloseMacros (lptw);
	/* Destruction des boutons ToolBar*/
	for (i = 0; i < lpmw->nButton; i++)
	{
		DestroyWindow(lpmw->hButton[i]);
    	}
	/* Contruction Menu et Toolbar */
	UpdateFileNameMenu(lptw);
	LoadMacros (lptw);
	/* Affichage Toolbar */
	ToolBarOnOff(lptw);
}
/*-----------------------------------------------------------------------------------*/   
void UpdateFileNameMenu(LPTW lptw)
{
	#define FILEMENUFRENCH "wscilabF.mnu"
	#define FILEMENUENGLISH "wscilabE.mnu"

	char *ScilabDirectory=NULL;
	
	LPMW lpmw= lptw->lpmw;
	
	ScilabDirectory=GetScilabDirectory(FALSE);

	if (lpmw->szMenuName!=NULL) FREE(lpmw->szMenuName);
    lpmw->szMenuName = (LPSTR) MALLOC (strlen (ScilabDirectory) +strlen("\\bin\\") +strlen (FILEMENUENGLISH) + 1);
	
	switch (lpmw->CodeLanguage)
	{
		case 1:
			wsprintf(lpmw->szMenuName,"%s\\bin\\%s",ScilabDirectory, FILEMENUFRENCH);
		break;
		default : case 0:
			wsprintf(lpmw->szMenuName,"%s\\bin\\%s",ScilabDirectory,FILEMENUENGLISH);
		break;
	}

	if (ScilabDirectory){FREE(ScilabDirectory);ScilabDirectory=NULL;}		
}
/*-----------------------------------------------------------------------------------*/   
void SwitchLanguage(LPTW lptw)
{
	char commandline[256];

	strcpy(commandline,"global LANGUAGE;%helps=initial_help_chapters(LANGUAGE);");

	StoreCommand1 (commandline, 0);
	switch (lptw->lpmw->CodeLanguage)
	{
		case 1:
			StoreCommand1 ("LANGUAGE=\"fr\";", 0);
		break;
		
		default: case 0:
			StoreCommand1 ("LANGUAGE=\"eng\";", 0);
		break;
	}
	StoreCommand1 (commandline, 0);

	strcpy(commandline,"global LANGUAGE;%helps=initial_help_chapters(LANGUAGE);");
	StoreCommand1 (commandline, 0);
    
	ReLoadMenus(lptw);
	ToolBarOnOff(lptw);
	OnRightClickMenu(lptw);

}
/*-----------------------------------------------------------------------------------*/
void ResetMenu(void)
{
	LPTW lptw=GetTextWinScilab();
	ReLoadMenus(lptw);
}   
/*-----------------------------------------------------------------------------------*/
void ConfigureScilabStar(int LangCode)
{
	#define SCILABSTAR		"scilab.star"

	char scilabstarfullpath[MAX_PATH];
	char scilabstartmpfullpath[MAX_PATH];
	char *WSCIPath=NULL;
	char wscitmp[MAX_PATH+1];
	
	int CodeRetour=-2;

	WSCIPath=getenv("SCI");
	ConvertPathUnixToWindowsFormat(WSCIPath,wscitmp);
	
	if (WSCIPath)
	{
		strcpy(scilabstarfullpath,wscitmp);
		strcat(scilabstarfullpath,"\\");
		strcat(scilabstarfullpath,SCILABSTAR);

		strcpy(scilabstartmpfullpath,scilabstarfullpath);
		strcat(scilabstartmpfullpath,".tmp");

		switch (LangCode)
		{
			case 1:
				CodeRetour=ModifyFile(scilabstarfullpath,"LANGUAGE=","LANGUAGE=\"fr\"\n");
			break;

			default : case 0:
				CodeRetour=ModifyFile(scilabstarfullpath,"LANGUAGE=","LANGUAGE=\"eng\"\n");
			break;
		}

		if (CodeRetour == 0 )
		{
			DeleteFile(scilabstarfullpath);
			if ( ! MoveFile(scilabstartmpfullpath,scilabstarfullpath) ) DeleteFile(scilabstartmpfullpath);
		}
		else
		{
		//	MessageBox(NULL,"Couldn't Modify Scilab.star","Error",MB_ICONWARNING);
		}
	}
}
/*-----------------------------------------------------------------------------------*/
int ModifyFile(char *fichier,char *motclef,char *chaine)
{
		int Retour=1;
		FILE *fileR,*fileW;
		char Ligne[MAX_PATH];
		char cmpchaine[MAX_PATH];
		
		fileR=NULL;
		fileW=NULL;

		fileR= fopen(fichier, "rt");
		strcpy(cmpchaine,fichier);
		strcat(cmpchaine,".tmp");

		fileW= fopen(cmpchaine, "wt");
		strcpy(cmpchaine,"");

		if ( (fileR) && (fileW) )
		{
			while( fgets(Ligne, MAX_PATH, fileR) != NULL)
			{ 
				strncpy(cmpchaine,Ligne,strlen(motclef));
				cmpchaine[strlen(motclef)]='\0';
				if (strcmp(cmpchaine,motclef)==0)
				{
					fputs(chaine,fileW);
					Retour=0;
				}
				else
				{
					fputs(Ligne,fileW);
				}
      			strcpy(Ligne,"\0");
      		}
			
    		fclose(fileR);
			fclose(fileW);
		}
		else
		{
			Retour=-1;
		}
		return Retour;
}
/*-----------------------------------------------------------------------------------*/

void SetLanguageMenu(char *Language)
{
	if (IsWindowInterface())
	{
		int LanguageCode=0;
		char *LanguageInScilabDotStar=NULL;
		LPTW lptw=GetTextWinScilab();

		if (strcmp(Language,"fr") == 0) LanguageCode=1;
		if (strcmp(Language,"eng") == 0) LanguageCode=0;

		if (lptw->lpmw->CodeLanguage != LanguageCode)
		{
			SendCTRLandAKey(CTRLU);
			lptw->lpmw->CodeLanguage=LanguageCode;
			SwitchLanguage(lptw);
			ConfigureScilabStar(lptw->lpmw->CodeLanguage);
			StoreCommand1 ("exec(SCI+'/contrib/loader.sce');",0);
		}

	}
	else
	{
 		// sciprint("Not in Console mode\n");
		// No Message
	}
}
/*-----------------------------------------------------------------------------------*/
int GetLanguageCodeInScilabDotStar(void)
{
	int ReturnLanguageCode=-1;
	char *WSCIPath=NULL;
	WSCIPath=getenv("SCI");

	if (WSCIPath)
	{
		#define SCILABSTAR		"scilab.star"
		#define MOTCLEF			"LANGUAGE="""

		FILE *fileR=NULL;
		char wscitmp[MAX_PATH+1];
		char Ligne[MAX_PATH];
		char scilabstarfullpath[MAX_PATH];
		char cmpchaine[MAX_PATH];

		ConvertPathUnixToWindowsFormat(WSCIPath,wscitmp);

		strcpy(scilabstarfullpath,wscitmp);
		strcat(scilabstarfullpath,"\\");
		strcat(scilabstarfullpath,SCILABSTAR);

		fileR= fopen(scilabstarfullpath, "rt");
		if (fileR)
		{
			while( fgets(Ligne, MAX_PATH, fileR) != NULL)
			{
				strncpy(cmpchaine,Ligne,strlen(MOTCLEF));
				cmpchaine[strlen(MOTCLEF)]='\0';
				if (strcmp(cmpchaine,MOTCLEF)==0)
				{
					int i=0,j=0;
					char LanguageTemp[MAX_PATH];

					strcpy(LanguageTemp,"\0");
					
					for (i=(int)strlen(MOTCLEF)+1;i<(int)strlen(Ligne)-2;i++)
					{
						LanguageTemp[j]=Ligne[i];
						j++;
					}
					LanguageTemp[j]='\0';

					if (strcmp(LanguageTemp,"fr")==0) ReturnLanguageCode=1;
					if (strcmp(LanguageTemp,"eng")==0) ReturnLanguageCode=0;

					fclose(fileR);
					return ReturnLanguageCode;
				}
				
				strcpy(Ligne,"\0");
			}
		}
		fclose(fileR);
	}
	return ReturnLanguageCode;
}
/*-----------------------------------------------------------------------------------*/
