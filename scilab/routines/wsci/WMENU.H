#ifndef __WMENU_H__
#define  __WMENU_H__

#ifdef __MINGW32__
#ifndef __WIN32_IE
#define _WIN32_IE 0x0300
#endif 
#endif 

#include <windows.h>
#include <commdlg.h>
#include <stdio.h>
#include <ShlObj.h>
#include <time.h>
#include <string.h>		/* only use far items */
#include <math.h>
#include "wresource.h"
#include "wcommon.h"
#include "wtext.h"



/*-----------------------------------------------------------------------------------*/
/* Les Definitions */
/*-----------------------------------------------------------------------------------*/

/* Allan CORNET */
/* EzCreateFont */
/* Fonts Properties */
#define EZ_ATTR_BOLD 1
#define EZ_ATTR_ITALIC 2
#define EZ_ATTR_UNDERLINE 4
#define EZ_ATTR_STRIKEOUT 8


/* limits */
/* #define MAXSTR 255 : already in wtext.h */ 
#define MACROLEN 5000
/* #define NUMMENU 256  defined in wresourc.h */
#define MENUDEPTH 3

/* menu tokens */
#define CMDMIN 			129
/**********************/
#define NEWSCILAB		CMDMIN
#define EXEC 			CMDMIN+1
#define OPEN 			CMDMIN+2
#define GETF 			CMDMIN+3
#define LOAD 			CMDMIN+4
#define SAVE 			CMDMIN+5
#define GETCWD 			CMDMIN+6
#define CHDIR 			CMDMIN+7
#define EXIT   			CMDMIN+8
/**********************/
#define MCOPY 			EXIT+1
#define PASTE 			MCOPY+1
#define PRINT 			MCOPY+2
#define TOOLBAR			MCOPY+3
#define FRENCH			MCOPY+4
#define ENGLISH			MCOPY+5
#define CHOOSETHEFONT	MCOPY+6
#define EMPTYCLIPBOARD	MCOPY+7
#define SELECTALL		MCOPY+8
/**********************/
#define RESTART 		SELECTALL+1
#define PAUSE 			RESTART+1
#define RESUME 			RESTART+2
#define ABORT 			RESTART+3
#define CONSOLE 		RESTART+4
/**********************/
#define SET  			CONSOLE+1
#define RAISE 			SET+1
#define DELETESCIW 		SET+2
#define PLUS 			SET+3
#define MINUS 			SET+4
#define XBASC 			SET+5
/**********************/
#define SCIPAD 			XBASC+1
/**********************/
#define HELP 			SCIPAD+1
#define CONFIGBROWSER   HELP+1
#define DEMOS 			HELP+2
#define NEWSGROUP		HELP+3
#define EMAIL 			HELP+4
#define WEB  			HELP+5
#define CONTRIBUTIONS	HELP+6
#define BUGZILLA		HELP+7
#define ABOUT 			HELP+8
/**********************/
#define EOS 			ABOUT+1
/**********************/
#define M_CTRL_C		EOS+1
#define M_CTRL_U		M_CTRL_C+1
#define M_CTRL_P		M_CTRL_C+2
#define M_CTRL_B		M_CTRL_C+3
#define M_CTRL_F		M_CTRL_C+4
#define M_CTRL_A		M_CTRL_C+5
#define M_CTRL_E		M_CTRL_C+6
#define M_CTRL_H		M_CTRL_C+7
#define M_CTRL_D		M_CTRL_C+8
#define M_CTRL_W		M_CTRL_C+9
#define M_CTRL_K		M_CTRL_C+10
#define M_CTRL_L		M_CTRL_C+11
#define M_CTRL_N		M_CTRL_C+12
/**********************/
#define CLEARCOMMANDWINDOW	M_CTRL_N+1
#define CLEARHISTORY		CLEARCOMMANDWINDOW+1
#define CMDMAX 			CLEARHISTORY
/**********************/
#define URLNEWSGROUP "http://groups.google.com/groups?dq=&num=25&hl=en&lr=&ie=UTF-8&group=comp.soft-sys.math.scilab"
#define URLCONTRIBUTIONS "http://scilabsoft.inria.fr/contrib/index_contrib.php?page=howto.html"
#define URL "http://www.scilab.org"
#define URLBUGZILLA "http://scilabsoft.inria.fr/cgi-bin/bugzilla_bug/index.cgi"
#define MAILTO "Scilab@inria.fr"
#define CCMAILTO ""
#define SUBJECT "Doctor Scilab Please Help me"


#define NEWSTRING(x) if ((x=LocalAlloc(LPTR, MAXSTR+1)) == (char *)NULL) \
{ *ierr=1;return(FALSE);}

#define BUGGOTOCLEAN(str) \
      wsprintf(buf,str,nLine,lpmw->szMenuName); \
      MessageBox(lptw->hWndParent,(LPSTR) buf,lptw->Title,MB_ICONEXCLAMATION);\
      goto errorcleanup;

/*-----------------------------------------------------------------------------------*/
/* Les Variables Globales */
/*-----------------------------------------------------------------------------------*/


//int LanguageCode=0;
/* 0 ENGLISH 
   1 FRENCH
   ...
*/
char *keyword[] =
{
  "[EMPTYCLIPBOARD]","[SELECTALL]","[CLEARHISTORY]","[CONFIGBROWSER]","[OPEN]","[FRENCH]","[ENGLISH]",
  "[TOOLBAR]","[NEWSGROUP]","[CLEARCOMMANDWINDOW]","[NEWSCILAB]","[EXEC]","[GETF]","[LOAD]","[SAVE]","[CHDIR]","[GETCWD]","[EXIT]",
  "[MCOPY]","[PASTE]","[PRINT]","[CHOOSETHEFONT]",
  "[RESTART]","[PAUSE]","[RESUME]","[ABORT]","[CONSOLE]",
  "[SET]","[RAISE]","[DELETESCIW]","[PLUS]","[MINUS]","[XBASC]",
  "[SCIPAD]",
  "[HELP]","[DEMOS]","[WEB]","[EMAIL]","[CONTRIBUTIONS]","[BUGZILLA]","[ABOUT]",
  "{ENTER}", "{ESC}", "{TAB}",
  "[M_CTRL_A]", "[M_CTRL_B]", "[M_CTRL_C]", "[M_CTRL_D]", "[M_CTRL_E]", "[M_CTRL_F]", "{^G}", "[M_CTRL_H]",
  "{^I}", "{^J}", "[M_CTRL_K]", "[M_CTRL_L]", "{^M}", "[M_CTRL_N]", "{^O}", "[M_CTRL_P]",
  "{^Q}", "{^R}", "{^S}", "{^T}", "[M_CTRL_U]", "{^V}", "[M_CTRL_W]", "{^X}",
  "{^Y}", "{^Z}", "{^[}", "{^\\}", "{^]}", "{^^}", "{^_}",
  NULL};

BYTE keyeq[] =
{
  EMPTYCLIPBOARD,SELECTALL,CLEARHISTORY,CONFIGBROWSER,OPEN,FRENCH,ENGLISH,
  TOOLBAR,NEWSGROUP,CLEARCOMMANDWINDOW,NEWSCILAB,EXEC,GETF,LOAD,SAVE,CHDIR,GETCWD,EXIT,
  MCOPY,PASTE,PRINT,CHOOSETHEFONT,
  RESTART,PAUSE,RESUME,ABORT,CONSOLE,
  SET,RAISE,DELETESCIW,PLUS,MINUS,XBASC,
  SCIPAD,
  HELP,DEMOS,WEB,EMAIL,CONTRIBUTIONS,BUGZILLA,ABOUT,
  13, 27, 9,
  M_CTRL_A, M_CTRL_B, M_CTRL_C, M_CTRL_D, M_CTRL_E, M_CTRL_F, 7, M_CTRL_H,
  9, 10, M_CTRL_K, M_CTRL_L, 13, M_CTRL_N, 15, M_CTRL_P,
  17, 18, 19, 20, M_CTRL_U, 22, M_CTRL_W, 24,
  25, 26, 28, 29, 30, 31,
  0 /* NULL */ };

static integer lab_count = 0;
static char gwin_name[100], gwin_name1[100];

/*-----------------------------------------------------------------------------------*/
/* Les Fonctions */
/*-----------------------------------------------------------------------------------*/
EXPORT BOOL CALLBACK InputBoxDlgProc (HWND, UINT, WPARAM, LPARAM);
EXPORT LRESULT CALLBACK MenuButtonProc (HWND, UINT, WPARAM, LPARAM);

static void Countp ();
static void Countm ();
static void SendCountSet ();
static void SendCountRaise ();
static void SendCountDelete ();


/*--------------------------------*/
void Callback_NEWSCILAB(void);
void Callback_EXEC(void);
void Callback_OPEN(void);
void Callback_GETF(void);
void Callback_LOAD(void);
void Callback_SAVE(void);
void Callback_CHDIR(void);
void Callback_GETCWD(void);
void Callback_SELECTALL(void);
void Callback_EMPTYCLIPBOARD(void);
void Callback_MCOPY(void);
void Callback_PASTE(void);
void Callback_PRINT(void);
void Callback_TOOLBAR(void);
void Callback_FRENCH(void);
void Callback_ENGLISH(void);
void Callback_CHOOSETHEFONT(void);
void Callback_RESTART(void);
void Callback_ABORT(void);
void Callback_PAUSE(void);
void Callback_RESUME(void);
void Callback_CONSOLE(void);
void Callback_SCIPAD(void);
void Callback_HELP(void);
void Callback_DEMOS(void);
void Callback_WEB(void);
void Callback_CONTRIBUTIONS(void);
void Callback_BUGZILLA(void);
void Callback_EMAIL(void);
void Callback_NEWSGROUP(void);
void Callback_ABOUT(void);
void Callback_CLEARCOMMANDWINDOW(void);
void Callback_CONFIGUREBROWSER(void);
void Callback_CLEARHISTORY(void);
/*--------------------------------*/
void SendMacro (LPTW lptw, UINT m);
BOOL SciOpenSave (HWND hWndParent, BYTE ** s,BOOL save, char **d, int *ierr);
void MenuFixCurrentWin (int ivalue);
GFILE * Gfopen (LPSTR lpszFileName, int fnOpenMode);
void Gfclose (GFILE * gfile);
int Gfgets (LPSTR lp, int size, GFILE * gfile);
int GetLine (char *buffer, int len, GFILE * gfile);
void LeftJustify (char *d, char *s);
void TranslateMacro (char *string);
void LoadMacros (LPTW lptw);
void CloseMacros (LPTW lptw);

/*Boite de dialogue Load ou Save */
BOOL OpenSaveSCIFile(HWND hWndParent,char *titre,BOOL read,char *FileExt,char *file);
/* Remplace \ par / dans un chemin */
void ReplaceSlash(char *pathout,char *pathin);
/* WTEXT.C*/
/* extern void PasteFunction(LPTW lptw); */
extern void SwitchConsole(void);
/*--------------------------------*/
/* Envoye le signal CTRL+une touche */
void SendCTRLandAKey(int code);

/* Efface la fenetre de "commandes" */
extern void ClearCommandWindow(LPTW lptw,BOOL Clearfirstline);
/*--------------------------------*/
void PrintString(char *lines,char *Entete);
void PrintFile(char *filename);
/* Imprime la fenetre courante */
void PrintSelection(LPTW lptw,char *Entete);

void PageHeader(HDC hdc,LPSTR Entete);
void Footer(HDC hdc,int number);
HFONT EzCreateFont (HDC hdc, TCHAR * szFaceName, int iDeciPtHeight,int iDeciPtWidth, int iAttributes, BOOL fLogRes);
HDC GetPrinterDC(void);
/*--------------------------------*/
void ResetMenu(void);
/*--------------------------------*/
void ShowToolBar(LPTW lptw);
void HideToolBar(LPTW lptw);
void ToolBarOnOff(LPTW lptw);
/*--------------------------------*/
void ReLoadMenus(LPTW lptw);
void UpdateFileNameMenu(LPTW lptw);
/*--------------------------------*/
/* Cree une infobulle */
void CreateMyTooltip (HWND hwnd,char ToolTipString[30]);
/*--------------------------------*/
void SwitchLanguage(LPTW lptw);
/*--------------------------------*/
void CreateButton(LPTW lptw, char *ButtonText[BUTTONMAX], int index,int ButtonSizeX, int ButtonSizeY);
int GetXPosButton(LPTW lptw,int index,int SizeXButtonText,int SizeXButtonIcon);
/*--------------------------------*/
void ConfigureScilabStar(int LangCode);
int ModifyFile(char *fichier,char *motclef,char *chaine);
/*--------------------------------*/
extern void reset_history(void); /* see Console\history.h*/
/*--------------------------------*/
int HideToolBarWin32(int WinNum);
/*--------------------------------*/
extern void HideGraphToolBar(struct BCG * ScilabGC);
extern void ShowGraphToolBar(struct BCG * ScilabGC);
/*--------------------------------*/
extern void SaveCurrentLine(BOOL RewriteLineAtPrompt);
extern void GetCurrentPrompt(char *CurrentPrompt);
/*--------------------------------*/
static void CutLineForDisplay(char *CutLine,char *Line,int NumberOfCharByLine);
/*--------------------------------*/
void SetLanguageMenu(char *Language);
/*--------------------------------*/
extern BOOL ConvertPathUnixToWindowsFormat(char *pathunix,char *pathwindows);
/*--------------------------------*/
#endif /*  __WMENU_H__ */
