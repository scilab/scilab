/* Copyright (C) 1998-2002 Chancelier Jean-Philippe */
/* Scilab wgmenu.c 

 *  Menus for graphic window  
 *  Jean-Philippe Chancelier 
 *	Allan CORNET 2004
 */

#ifndef __WGMENU__
#define __WGMENU__

#include <stdio.h>
#ifndef STRICT
#define STRICT
#endif
/*#include <windows.h>
  #include <windowsx.h>*/
#include <string.h>		/* only use  items */

#ifdef __CYGWIN32__
#include <unistd.h>		/* for getpid */
#endif
#if (defined __MSC__) || (defined __MINGW32__)
#include <process.h>		/* for getpid */
#endif

#ifdef __ABSC__
#define getpid() getpid_()
#endif

#ifdef __STDC__
#include <stdlib.h>
#else
int system ();
#endif

/*#include "wgnuplib.h"*/
#include "wresource.h"
#include "wcommon.h"
#include <commdlg.h>
#include "../graphics/Graphics.h"
#include "../sun/men_Sutils.h"
extern TW textwin;

/* limits */
#define MAXSTR 255
#define MACROLEN 5000
/* #define NUMMENU 256  defined in wresourc.h */
#define MENUDEPTH 3

/* menu tokens */
#define CMDMIN 131
/**********************/
/** Warning must be like OPEN and SAVE in the wmenu.c file */
#define LOADSCG CMDMIN /*131*/
#define SAVESCG CMDMIN+1 /*132*/
#define NEWFIG	CMDMIN+2
#define CLOSE   CMDMIN+3
#define SCIPS   CMDMIN+4
#define SCIPR   CMDMIN+5
#define PRINT   CMDMIN+6
#define UPDINI  CMDMIN+7
/**********************/
#define SCIGSEL UPDINI+1
#define TOOLBARGRAPH SCIGSEL+1
#define REDRAW	SCIGSEL+2
#define CLEARWG SCIGSEL+3
#define COPYCLIP SCIGSEL+4
#define COPYCLIP1 SCIGSEL+5
/**********************/
#define ZOOM COPYCLIP1+1
#define UNZOOM ZOOM+1
#define ROT3D ZOOM+2
/**********************/
#define EOS     ROT3D+1
/**********************/
#define CMDMAX EOS
/**********************/

#define BUGGOTOCLEAN(str) \
      wsprintf(buf,str,nLine,ScilabGC->lpgw->szMenuName); \
      MessageBox(ScilabGC->hWndParent,(LPSTR) buf,ScilabGC->lpgw->Title,MB_ICONEXCLAMATION);\
      goto errorcleanup;



static char filename[MAXSTR], filename1[MAXSTR];

static char *keyword[] =
{
  "[TOOLBARGRAPH]","[NEWFIG]","[ZOOM]", "[UNZOOM]", "[ROT3D]", "[PRINT]", "[COPYCLIP]", "[COPYCLIP1]",
  "[REDRAW]", "[LOADSCG]", "[SAVESCG]", "[CLEARWG]", "[SCIPS]", "[SCIPR]",
  "[SCIGSEL]", "[UPDINI]", "[EOS]", "[CLOSE]",
  "{ENTER}", "{ESC}", "{TAB}",
  "{^A}", "{^B}", "{^C}", "{^D}", "{^E}", "{^F}", "{^G}", "{^H}",
  "{^I}", "{^J}", "{^K}", "{^L}", "{^M}", "{^N}", "{^O}", "{^P}",
  "{^Q}", "{^R}", "{^S}", "{^T}", "{^U}", "{^V}", "{^W}", "{^X}",
  "{^Y}", "{^Z}", "{^[}", "{^\\}", "{^]}", "{^^}", "{^_}",
  NULL};

static BYTE keyeq[] =
{
  TOOLBARGRAPH,NEWFIG,ZOOM, UNZOOM, ROT3D, PRINT, COPYCLIP, COPYCLIP1,
  REDRAW, LOADSCG, SAVESCG, CLEARWG, SCIPS, SCIPR, SCIGSEL, UPDINI, EOS,
  CLOSE,
  13, 27, 9,
  1, 2, 3, 4, 5, 6, 7, 8,
  9, 10, 11, 12, 13, 14, 15, 16,
  17, 18, 19, 20, 21, 22, 23, 24,
  25, 26, 28, 29, 30, 31,
  0 /* NULL */ };

/*****************************
  menu for exporting to ps or xfig 
  ***************************/

typedef struct tagLS
{
  int colored;
  int land;
  int use_printer;
  int ps_type;
}
LS;

static LS ls =
{1, 0, 0, 0};

static char *Print_Formats[] =
{
  "Postscript",
  "Postscript No Preamble",
  "Postscript-Latex",
  "Xfig",
  "GIF",
  "PPM",
  "BMP",
  "Enhanced Meta File (.emf)"
};


static void dos2win32 (char *filename, char *filename1);
static void ExploreMenu (HMENU hmen, BYTE ** macro);
static void SciDelMenu (LPMW lpmw, char *name);
static void SciChMenu (LPMW lpmw, char *name, char *new_name);
static void SciSetMenu (HMENU hmen, char *name, int num, int flag);
static void SciChMenu (LPMW lpmw, char *name, char *new_name);
static void SciDelMenu (LPMW lpmw, char *name);
static void SavePs (struct BCG *ScilabGC);
static void PrintPs (struct BCG *ScilabGC);
static void TranslateMacro (char *string);
static void ExploreMenu (HMENU hmen, BYTE ** hmacro);
static void scig_replay_hdc (char c, integer win_num, HDC hdc, int width,int height,  int scale);
static void scig_command_scilabgc (int number, void f (struct BCG *));

extern BOOL IsWindowInterface();
extern void Write_Scilab_Console (char *buf);
extern void Write_Scilab_Window (char *buf);
extern void ResetMenu(void);
extern struct BCG *GetWindowXgcNumber (integer i);
extern void HideGraphToolBar(struct BCG * ScilabGC);
extern void ShowGraphToolBar(struct BCG * ScilabGC);
extern void SaveCurrentLine(BOOL RewriteLineAtPrompt);
extern void ExportBMP(struct BCG *ScilabGC,char *pszflname);
extern void ExportEMF(struct BCG *ScilabGC,char *pszflname);

void SendGraphMacro (struct BCG *ScilabGC, UINT m);
void ScilabMenuAction (char *buf);
void write_scilab (char *buf);
void LoadGraphMacros (struct BCG *ScilabGC);
void CloseGraphMacros (struct BCG *ScilabGC);
void scig_h_winmeth_print (integer number);
void scig_h_copyclip (integer number);
void scig_h_copyclip1 (integer number);
void scig_print (integer number);
void scig_export (integer number);
void UpdateFileGraphNameMenu(struct BCG *ScilabGC);

int WGFindMenuPos (BYTE ** macros);
int C2F (setmen) (integer * win_num, char *button_name,
				  integer * entries, integer * ptrentries,
				  integer * ne, integer * ierr);
int C2F (unsmen) (integer * win_num, char *button_name, integer * entries,
				  integer * ptrentries, integer * ne, integer * ierr);

EXPORT BOOL CALLBACK ExportStyleDlgProc (HWND hdlg, UINT wmsg, WPARAM wparam, LPARAM lparam);

BOOL ExportStyle (struct BCG * ScilabGC);
void NewFigure(struct BCG * ScilabGC);
int FindFreeGraphicWindow(struct BCG * ScilabGC);



#endif /*__WGMENU__*/