/*******************************************
 * Original source : GNUPLOT - win/wtext.h
 * modified for Scilab 
 *******************************************
 * Copyright (C) 1992   Russell Lang
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
 * 
 *
 * AUTHORS
 *   Russell Lang
 * Modified for Scilab 1997
 *   Jean-Philippe Chancelier 
 *   Allan CORNET (2003)
 */


#include <string.h>		/* use only far items */
#include <stdlib.h>
#include <ctype.h>


#include "wresource.h"
#include "resource.h"
#include "wcommon.h"
#include "..\version.h"


/*-----------------------------------------------------------------------------------*/
/* Les Definitions */
/*-----------------------------------------------------------------------------------*/
/**********************/
/* Code Touches CTRL+Key */
#define CTRLA		1
#define CTRLB		2
#define CTRLC		3
#define CTRLD		4
#define CTRLE		5
#define CTRLF		6
#define CTRLH		8
#define CTRLK		11
#define CTRLL		12
#define CTRLN		14
#define CTRLP		16
#define CTRLU		21
#define CTRLW		23

/**********************/

/* Taille du buffer circulaire des touches*/
#define KeyBufferSize 256

/* font stuff */
#define TEXTFONTSIZE 9
#define TEXTFONTNAME "Terminal"

/* Definition des codes touche */
#define VK_V        86
#define VK_C        67

/* limits */
#define MAXSTR 256

#define NOTEXT 0xF0
#define MARKFORE RGB(255,255,255)
#define MARKBACK RGB(0,0,128)
#define TextFore(attr) TextColorTable[(attr) & 15]
#define TextBack(attr) TextColorTable[(attr>>4) & 15]


/* redefine functions that can talk to tty devices, to use 
 * implementation in winmain.c/wgnuplot.dll */

#define kbhit()  MyKBHit()
#define getche() MyGetChE()
#define getch()  MyGetCh()
#define _getch()  MyGetCh()
#define putch(ch)  MyPutCh(ch)

#define fgetc(file) MyFGetC(file)
#undef  getchar
#define getchar()   MyFGetC(stdin)
#undef  getc
#define getc(file)  MyFGetC(file)
#define fgets(str,sz,file)  MyFGetS(str,sz,file)
#define gets(str)  	    MyGetS(str)

#define fputc(ch,file) MyFPutC(ch,file)
#undef  putchar
#define putchar(ch)    MyFPutC(ch,stdout)
#undef  putc
#define putc(ch,file)  MyFPutC(ch,file)
#define fputs(str,file)  MyFPutS(str,file)
#define puts(str)        MyPutS(str)

#define fprintf MyFPrintF
#define printf MyPrintF

#define fwrite(ptr, size, n, stream) MyFWrite(ptr, size, n, stream)
#define fread(ptr, size, n, stream) MyFRead(ptr, size, n, stream)

/* now cause errors for some unimplemented functions */

#define vprintf dontuse_vprintf
#define vfprintf dontuse_vfprintf
#define fscanf dontuse_fscanf
#define scanf dontuse_scanf
#define clreol dontuse_clreol
#define clrscr dontuse_clrscr
#define gotoxy dontuse_gotoxy
#define wherex dontuse_wherex
#define wherey dontuse_wherey
#define cgets dontuse_cgets
#define cprintf dontuse_cprintf
#define cputs dontuse_cputs
#define cscanf dontuse_cscanf
#define ungetch dontuse_ungetch

/* now for the prototypes */

int MyPutCh(int ch);
int MyKBHit(void);
int MyGetCh(void);
int MyGetChE(void);
int MyFGetC(FILE *file);
char * MyGetS(char *str);
char * MyFGetS(char *str, unsigned int size, FILE *file);
int MyFPutC(int ch, FILE *file);
int MyFPutS(char *str, FILE *file);
int MyPutS(char *str);
int MyFPrintF(FILE *file, char *fmt, ...);
int MyPrintF(char *fmt, ...);
size_t MyFWrite(const void *ptr, size_t size, size_t n, FILE *stream);
size_t MyFRead(void *ptr, size_t size, size_t n, FILE *stream);

/*-----------------------------------------------------------------------------------*/
/* Les Variables Globales */
/*-----------------------------------------------------------------------------------*/

/*--------------------------*/
int Windows_Console_State;
/* 0 Hide
   1 Show
*/
/*--------------------------*/

static COLORREF TextColorTable[16] =
{
  RGB (0, 0, 0),		/* black */
  RGB (0, 0, 128),		/* dark blue */
  RGB (0, 128, 0),		/* dark green */
  RGB (0, 128, 128),		/* dark cyan */
  RGB (128, 0, 0),		/* dark red */
  RGB (128, 0, 128),		/* dark magenta */
  RGB (128, 128, 0),		/* dark yellow */
  RGB (128, 128, 128),		/* dark grey */
  RGB (192, 192, 192),		/* light grey */
  RGB (0, 0, 255),		/* blue */
  RGB (0, 255, 0),		/* green */
  RGB (0, 255, 255),		/* cyan */
  RGB (255, 0, 0),		/* red */
  RGB (255, 0, 255),		/* magenta */
  RGB (255, 255, 0),		/* yellow */
  RGB (255, 255, 255),		/* white */
};


/*-----------------------------------------------------------------------------------*/
/* Les Fonctions */
/*-----------------------------------------------------------------------------------*/
EXPORT LRESULT CALLBACK WndParentProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

EXPORT LRESULT CALLBACK WndTextProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
EXPORT BOOL CALLBACK AboutDlgProc (HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam);
extern int C2F (scilines) (int *nl, int *nc);

EXPORT void WINAPI TextMessage (void);
static void CreateTextClass (LPTW lptw);
EXPORT int WINAPI TextInit (LPTW lptw);
EXPORT void WINAPI TextClose (LPTW lptw);
void WriteTextIni (LPTW lptw);
void ReadTextIni (LPTW lptw);
EXPORT void WINAPI TextToCursor (LPTW lptw);
void NewLine (LPTW lptw);
void UpdateText (LPTW lptw, int count);
EXPORT int WINAPI TextPutCh (LPTW lptw, BYTE ch);
void TextPutStr (LPTW lptw, LPSTR str);
void ReadTextIni (LPTW lptw);
void LimitMark (LPTW lptw, POINT FAR * lppt);
void ClearMark (LPTW lptw, POINT pt);
void DoLine (LPTW lptw, HDC hdc, int xpos, int ypos, int offset, int count);
void DoMark (LPTW lptw, POINT pt, POINT end, BOOL mark);
void UpdateMark (LPTW lptw, POINT pt);
void DragFunc (LPTW lptw, HDROP hdrop);
void TextCopyClip (LPTW lptw);
void TextMakeFont (LPTW lptw);
void TextSelectFont (LPTW lptw);
EXPORT int WINAPI TextKBHit (LPTW lptw);
EXPORT int WINAPI TextGetCh (LPTW lptw);
int CtrlCHit (LPTW lptw);
EXPORT int WINAPI TextGetChE (LPTW lptw);
EXPORT LPSTR WINAPI TextGetS (LPTW lptw, LPSTR str, unsigned int size);
EXPORT int WINAPI TextPutS (LPTW lptw, LPSTR str);
EXPORT void WINAPI TextGotoXY (LPTW lptw, int x, int y);
EXPORT int WINAPI TextWhereX (LPTW lptw);
EXPORT int WINAPI TextWhereY (LPTW lptw);
EXPORT void WINAPI TextCursorHeight (LPTW lptw, int height);
EXPORT void WINAPI TextClearEOL (LPTW lptw);
EXPORT void WINAPI TextClearEOS (LPTW lptw);
EXPORT void WINAPI TextInsertLine (LPTW lptw);
EXPORT void WINAPI TextDeleteLine (LPTW lptw);
EXPORT void WINAPI TextScrollReverse (LPTW lptw);


extern void CreateThreadPaste(char *Text);
extern BOOL IsReadyForAnewLign(void);
extern void SetReadyOrNotForAnewLign(BOOL Ready);
extern void PasteFunction(LPTW lptw,BOOL special);
extern BOOL IsEmptyClipboard(LPTW lptw);
extern void TextCopyClip(LPTW lptw);
extern void CleanClipboard(LPTW lptw);
extern HANDLE GetHandleThreadPaste(void);
extern BOOL GetThreadPasteRunning(void);
extern void SetThreadPasteRunning(BOOL Running);



/*--------------------------*/
/* voir fichier winmain.c*/
extern char ScilexConsoleName[MAX_PATH];
extern void Kill_Scilex_Win98(void);
extern void Kill_Scilex(void);
/* voir fichier wmenu.c*/
extern void SendCTRLandAKey(int code);
/*--------------------------*/
void HomeFunction(void);
void ResizeScreenBuffer(LPTW lptw);
void ReAllocScreenBuffer(LPTW lptw);
void InitScreenBuffer(LPTW lptw);
void ReorganizeScreenBuffer(LPTW lptw);

extern void SaveCurrentLine(BOOL RewriteLineAtPrompt);
extern BOOL IsWindowInterface(void);
extern void ReplaceSlash(char *pathout,char *pathin);
extern BOOL IsAFile(char *chainefichier);
extern void ToolBarOnOff(LPTW lptw);
extern void ReLoadMenus(LPTW lptw);
extern void ResetMenu(void);
int ClearScreenConsole _PARAMS((char *fname));
void ClearCommandWindow(LPTW lptw,BOOL Clearfirstline);
void HideScilex(void);
void ShowScilex(void);
/*--------------------------*/
void SwitchConsole(void);
/*--------------------------*/
void CleanClipboard(LPTW lptw);
void SelectAll(LPTW lptw,BOOL DoAMark);
void UnSelect(LPTW lptw);
void HelpOn(LPTW lptw);
void OpenSelection(LPTW lptw);
void CutSelection(LPTW lptw);
void EvaluateSelection(LPTW lptw);
void OnRightClickMenu(LPTW lptw);
void InvalidateCursor( void ); 
/*--------------------------*/
BOOL HasAZoneTextSelected(LPTW lptw);
/*--------------------------*/
DWORD WINAPI SendInputText(LPVOID lpParam );
/*--------------------------*/
void CleanPromptFromText(char *Text);
/*--------------------------*/
int ReplacePrompt(char *Text,char *prompt);
/* retourne TRUE si Text a été modifié */
/*--------------------------*/
void ForceToActiveWindowParent(void);
/*--------------------------*/
void PrintSelection(LPTW lptw,char *Entete);
/*--------------------------*/
void ExitWindow(void);
/*--------------------------*/
void write_scilab_synchro(char *line);
DWORD WINAPI WriteTextThread(LPVOID lpParam);
/*--------------------------*/
int ShowWindowFunction _PARAMS((char *fname));
/*--------------------------*/
void MessageBoxNewGraphicMode(void);
BOOL CALLBACK MessageBoxNewGraphicModeDlgProc(HWND hwnd,UINT Message, WPARAM wParam, LPARAM lParam);
/*--------------------------*/
BOOL WriteIntoScilab(LPTW lptw,char *StringCommand);
void WriteIntoKeyBuffer(LPTW lptw,char *StringCommand);
/*--------------------------*/
BOOL IsToThePrompt(void);