#ifndef __WGNUPLIB_H__
#define __WGNUPLIB_H__


/* from GNUPLOT - win/wgnuplib.h */
/*
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
 * AUTHORS
 *   Russell Lang
 * 
 * Modified for Scilab (1997)
 *   Jean-Philippe Chancelier
 *   Allan CORNET INRIA 2005
 */

#ifdef _WINDOWS
#define _Windows
#endif

#ifdef __DLL__ 
/** XXXXX #define EXPORT extern "C" __declspec (dllexport) **/
#define EXPORT __declspec (dllexport) 
#else
#define EXPORT 
#endif

/** Get reference to functions if DLL **/
#ifdef __DLL__
#define MyGetProcAddress(strProc,Proc) GetProcAddress(hdllInstance,strProc);
#else
#define MyGetProcAddress(strProc,Proc) Proc 
#endif

/*****************************************
 * wprinter.c - windows printer routines 
 *****************************************/

typedef struct tagPRINT {
	HDC		hdcPrn;
	HWND	hDlgPrint;
	BOOL	bUserAbort;
	POINT	pdef;
	POINT	psize;
	POINT	poff;
	struct tagPRINT  *next;
} PRINT;

typedef PRINT *  LP_PRINT; 

/*****************************************
 * wmenu.c - menu structure *
 *****************************************/

#define BUTTONMAX 20

typedef struct tagMW
{
  LPSTR	szMenuName;		/* required */
  HMENU	hMenu;
  BYTE          **macro;
  BYTE          *macrobuf;
  int		nCountMenu;
  DLGPROC	lpProcInput;
  char	        *szPrompt;
  char	        *szAnswer;
  int		nChar;
  int		nButton;
  HWND	    hButton[BUTTONMAX];
  int		hButtonID[BUTTONMAX];
  WNDPROC	lpfnMenuButtonProc;
  WNDPROC	lpfnButtonProc[BUTTONMAX];
  BOOL		IsAIcon[BUTTONMAX];
  int		PositionX[BUTTONMAX];
  BOOL		ShowToolBar;
  BOOL		LockToolBar;
  int		CodeLanguage;
  
} MW;
typedef MW  *LPMW;

/*****************************************
 * wtext.c text window structure 
 * If an optional item is not specified it must be zero 
 *****************************************/

#define SCI_DEFAULT_CHARSET ANSI_CHARSET

#define MAXFONTNAME 80
typedef struct tagTW
{
  LP_PRINT	lpr;		/* must be first */
  HINSTANCE hInstance;		/* required */
  HINSTANCE hPrevInstance;	/* required */
  LPSTR	Title;			/* required */
  LPMW	lpmw;			/* optional */
  POINT	ScreenSize;		/* optional */
  unsigned int KeyBufSize;	/* optional */
  int		nCmdShow;	/* optional */
  HICON	hIcon;			/* optional */
  HMENU	hPopMenu;
  HWND	hWndText;
  HWND	hWndParent;
  POINT	Origin;
  POINT	Size;
  BYTE  *ScreenBuffer;
  BYTE  *AttrBuffer;
  BYTE  *KeyBuf;
  BYTE  *KeyBufIn;
  BYTE  *KeyBufOut;
  BYTE	Attr;
  BOOL	bFocus;
  BOOL	bGetCh;
  BOOL	bSysColors;
  HBRUSH	hbrBackground;
  char	fontname[MAXFONTNAME];	/* font name */
  int		fontsize;				/* font size in pts */
  HFONT	hfont;
  int		CharAscent;
  int		ButtonHeight;
  int		CaretHeight;
  int		CursorFlag;
  POINT	CursorPos;
  POINT	ClientSize;
  POINT	CharSize;
  POINT	ScrollPos;
  POINT	ScrollMax;
  POINT	MarkBegin;
  POINT	MarkEnd;
  BOOL	Marking;
} TW;
typedef TW *  LPTW;


/*****************************************
 * wtext.c - Text Window *
 *****************************************/

EXPORT void WINAPI TextMessage(void);
EXPORT void TextMessage1(int);
EXPORT int  WINAPI  TextInit(LPTW lptw);
EXPORT void WINAPI  TextClose(LPTW lptw);
EXPORT void WINAPI  TextToCursor(LPTW lptw);
EXPORT int  WINAPI   TextKBHit(LPTW);
EXPORT int  WINAPI  TextGetCh(LPTW);
EXPORT int  WINAPI  TextGetChE(LPTW);
EXPORT LPSTR WINAPI  TextGetS(LPTW lptw, LPSTR str, unsigned int size);
EXPORT int  WINAPI  TextPutCh(LPTW, BYTE);
EXPORT int  WINAPI  TextPutS(LPTW lptw, LPSTR str);
EXPORT void WINAPI  TextGotoXY(LPTW lptw, int x, int y);
EXPORT int WINAPI  TextWhereX(LPTW lptw);
EXPORT int WINAPI  TextWhereY(LPTW lptw);
EXPORT void WINAPI  TextCursorHeight(LPTW lptw, int height);
EXPORT void WINAPI  TextClearEOL(LPTW lptw);
EXPORT void WINAPI  TextClearEOS(LPTW lptw);
EXPORT void WINAPI  TextInsertLine(LPTW lptw);
EXPORT void WINAPI  TextDeleteLine(LPTW lptw);
EXPORT void WINAPI  TextScrollReverse(LPTW lptw);
EXPORT void WINAPI  TextAttr(LPTW lptw, BYTE attr);
EXPORT void WINAPI  AboutBox(HWND hwnd);

/*****************************************
 * wgraph.c - graphics window *
 *****************************************/

typedef struct tagGW {
  LP_PRINT	lpr;		/* must be first */
  HINSTANCE	hInstance;	/* required */
  HINSTANCE	hPrevInstance;	/* required */
  LPSTR	Title;			/* required */
  int		xmax;		/* required */
  int		ymax;		/* required */
  LPTW	lptw;		/* optional */  /* associated text window */
  POINT	Origin;		/* optional */	/* origin of graph window */
  POINT	Size;		/* optional */	/* size of graph window */
  HMENU	hPopMenu;	/* popup menu */
  BOOL	resized;	/* has graph window been resized? */
  BOOL	graphtotop;	/* bring graph window to top after every plot? */
  BOOL	locked;		/* locked if being written */
  int	ButtonHeight; /* */
  LPSTR	szMenuName;		/* required */
} GW;

typedef GW *  LPGW;

#define SCILEX "scilex.exe"
#define WSCILEX "wscilex.exe"

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
/*************************/

#endif /*  __WGNUPLIB_H__ */
