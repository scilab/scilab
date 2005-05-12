/*******************************************
 * Original source : GNUPLOT - win/wtext.c 
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
 * AUTHORS
 *   Russell Lang
 * 
 * Modifed for Scilab 1997    : Jean-Philippe Chancelier 
 * Modified for Scilab (2003) : Allan CORNET
 */

/* WARNING: Do not write to stdout/stderr with functions not listed 
   in win/wtext.h */
   
#ifndef STRICT    
#define STRICT
#endif

//#include <WinAble.h>
#include "WTEXT.h"   
#include "wmcopydata.h"
#include "WinConsole.h"

/*-----------------------------------------------------------------------------------*/
char ScilexWindowName[MAX_PATH];

POINT ScreenMinSize = {16, 4};
POINT ScrollSize = {80,360}; /* the default is {80,120} */

char szNoMemory[] = "out of memory";


/* Utiliser par write_scilab_synchro */
HANDLE hThreadWrite;
CRITICAL_SECTION Sync; /* Section Critique pour Write_scilab */

static BOOL ConsoleIsMinimized=FALSE;
static BOOL WriteInKeyBuf=FALSE;

extern HDC TryToGetDC(HWND hWnd);
extern int C2F(sciquit)() ;
/*-----------------------------------------------------------------------------------*/
/*********************************************
 * message Loop 
 *********************************************/
EXPORT void WINAPI TextMessage (void)
{
  TextMessage1 (0);
  return;
}
/*-----------------------------------------------------------------------------------*/
static BOOL RegisterParentWindowClass (LPTW lptw)
{
	BOOL bOK=FALSE;
	WNDCLASS Parentwndclass;
	Parentwndclass.lpszClassName = szParentClass;
	Parentwndclass.lpfnWndProc = WndParentProc;
	Parentwndclass.style = CS_HREDRAW | CS_VREDRAW;
	Parentwndclass.cbClsExtra = 0;
	Parentwndclass.cbWndExtra = 2 * sizeof (void FAR *);
	Parentwndclass.hInstance = lptw->hInstance;
	Parentwndclass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	Parentwndclass.hCursor = LoadCursor (NULL, IDC_WAIT);
	Parentwndclass.hbrBackground =(HBRUSH) CreateSolidBrush (GetSysColor(COLOR_WINDOW)) ;
	Parentwndclass.lpszMenuName = NULL;

	if (!RegisterClass(&Parentwndclass))
	{
		DWORD dwLastError = GetLastError();
		char buff1[1000], buff2[1000];

		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwLastError, 0, buff1, sizeof (buff1), NULL);
		sprintf(buff2, "Parentwndclass Win32 error %ld occured", dwLastError);

		MessageBox(NULL, buff1, buff2, MB_ICONERROR);

		bOK=FALSE;
	}
	else bOK=TRUE;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
static BOOL RegisterTextWindowClass (LPTW lptw)
{
	BOOL bOK=FALSE;
	WNDCLASS Textwndclass;
	Textwndclass.style = CS_HREDRAW | CS_VREDRAW;
	Textwndclass.lpfnWndProc = WndTextProc;
	Textwndclass.cbClsExtra = 0;
	Textwndclass.cbWndExtra = 2 * sizeof (void FAR *);
	Textwndclass.hInstance = lptw->hInstance;
	Textwndclass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	Textwndclass.hCursor = LoadCursor (NULL, IDC_WAIT);
	Textwndclass.hbrBackground =(HBRUSH) CreateSolidBrush (GetSysColor (COLOR_WINDOW));
	Textwndclass.lpszMenuName = NULL;
	Textwndclass.lpszClassName = szTextClass;
	if (!RegisterClass(&Textwndclass))
	{
		DWORD dwLastError = GetLastError();
		char buff1[1000], buff2[1000];

		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwLastError, 0, buff1, sizeof (buff1), NULL);
		sprintf(buff2, "Textwndclass Win32 error %ld occured", dwLastError);

		MessageBox(NULL, buff1, buff2, MB_ICONERROR);

		bOK=FALSE;
	}
	else 
	{
		lptw->hbrBackground =(HBRUSH) CreateSolidBrush (GetSysColor (COLOR_WINDOW));
		bOK=TRUE;
	}

	return bOK;

}
/*-----------------------------------------------------------------------------------*/
/*********************************************
 * text window class 
 *********************************************/
static void CreateTextClass (LPTW lptw)
{	
	if (!RegisterParentWindowClass (lptw)) exit(1);
	if (!RegisterTextWindowClass (lptw)) exit(1);
}
/*-----------------------------------------------------------------------------------*/
/*********************************************
 * make text window 
 *********************************************/
EXPORT int WINAPI TextInit (LPTW lptw)
{
  RECT rect;
  HMENU sysmenu;
  HGLOBAL hglobal;
  

  ReadTextIni (lptw);

  if (!lptw->hPrevInstance)    CreateTextClass (lptw);

  if (lptw->KeyBufSize == 0)    lptw->KeyBufSize = KeyBufferSize;

   /* Modified by D.Abdemouche 20/05/03 */
  if (lptw->ScreenSize.x < ScrollSize.x)  lptw->ScreenSize.x = ScrollSize.x;
  if (lptw->ScreenSize.y < ScrollSize.y)  lptw->ScreenSize.y = ScrollSize.y;

  lptw->CursorPos.x = lptw->CursorPos.y = 0;
  lptw->bFocus = FALSE;
  lptw->bGetCh = FALSE;
  lptw->CaretHeight = 0;
  if (!lptw->nCmdShow)
    lptw->nCmdShow = SW_SHOWNORMAL;
  if (!lptw->Attr)
    lptw->Attr = 0xf0;		/* black on white */

  hglobal = GlobalAlloc (GHND, lptw->ScreenSize.x * lptw->ScreenSize.y);

  /* sauvegarde de hglobal pour reallocation ultérieure */
  lptw->ScreenBuffer = (BYTE FAR *) GlobalLock (hglobal);
  if (lptw->ScreenBuffer == (BYTE FAR *) NULL)
    {
      MessageBox ((HWND) NULL, szNoMemory, (LPSTR) NULL, MB_ICONHAND | MB_SYSTEMMODAL);
      return (1);
    }
  _fmemset (lptw->ScreenBuffer, ' ', lptw->ScreenSize.x * lptw->ScreenSize.y);
  hglobal = GlobalAlloc (GHND, lptw->ScreenSize.x * lptw->ScreenSize.y);
  lptw->AttrBuffer = (BYTE FAR *) GlobalLock (hglobal);
  if (lptw->AttrBuffer == (BYTE FAR *) NULL)
    {
      MessageBox ((HWND) NULL, szNoMemory, (LPSTR) NULL, MB_ICONHAND | MB_SYSTEMMODAL);
      return (1);
    }
  _fmemset (lptw->AttrBuffer, NOTEXT, lptw->ScreenSize.x * lptw->ScreenSize.y);
  
  hglobal = GlobalAlloc (GHND, lptw->KeyBufSize);
    /* hglobal = GlobalAlloc (LHND, lptw->KeyBufSize); */
  /* Modification Allan CORNET LHND pour LocalAlloc et non pour GlobalAlloc*/
  lptw->KeyBuf = (BYTE FAR *) GlobalLock (hglobal);
  
  if (lptw->KeyBuf == (BYTE FAR *) NULL)
    {
      MessageBox ((HWND) NULL, szNoMemory, (LPSTR) NULL, MB_ICONHAND | MB_SYSTEMMODAL);
      return (1);
    }
  _fmemset (lptw->KeyBuf, ' ', lptw->KeyBufSize);  
  lptw->KeyBufIn = lptw->KeyBufOut = lptw->KeyBuf;
  
  
  lptw->hWndParent = CreateWindow (szParentClass, lptw->Title,
				   WS_OVERLAPPEDWINDOW,
				   lptw->Origin.x, lptw->Origin.y,
				   lptw->Size.x, lptw->Size.y,
				   NULL, NULL, lptw->hInstance, lptw);

  if (lptw->hWndParent == (HWND) NULL)
    {
      MessageBox ((HWND) NULL, "Couldn't open parent text window", (LPSTR) NULL, MB_ICONHAND | MB_SYSTEMMODAL);
      return (1);
    }
  ShowWindow (lptw->hWndParent, lptw->nCmdShow);
  GetClientRect (lptw->hWndParent, &rect);

  lptw->hWndText = CreateWindow (szTextClass, lptw->Title,
				 WS_CHILD | WS_VSCROLL | WS_HSCROLL,
				 0, lptw->ButtonHeight,
			       rect.right, rect.bottom - lptw->ButtonHeight,
			     lptw->hWndParent, NULL, lptw->hInstance, lptw);
			     
  if (lptw->hWndText == (HWND) NULL)
    {
      MessageBox ((HWND) NULL, "Couldn't open text window", (LPSTR) NULL, MB_ICONHAND | MB_SYSTEMMODAL);
      return (1);
    }
    
  OnRightClickMenu(lptw) ; 
 
  sysmenu = GetSystemMenu (lptw->hWndParent, 0);	/* get the sysmenu */
  /*AppendMenu (sysmenu, MF_SEPARATOR, 0, NULL);
  AppendMenu (sysmenu, MF_POPUP, (UINT) lptw->hPopMenu, "&Options");*/
  AppendMenu (sysmenu, MF_SEPARATOR, 0, NULL);
  AppendMenu (sysmenu, MF_STRING, M_CONSOLE, "&Console");
  AppendMenu (sysmenu, MF_SEPARATOR, 0, NULL);
  AppendMenu (sysmenu, MF_STRING, M_ABOUT, "&About");

  if (lptw->lpmw)    LoadMacros (lptw);
  ReLoadMenus(lptw);

  DisableMenus(lptw);

  ToolBarOnOff(lptw);
  DisableToolBar(lptw);
  OnRightClickMenu(lptw);

  ShowWindow (lptw->hWndText, SW_SHOWNORMAL);
  BringWindowToTop (lptw->hWndText);
  SetFocus (lptw->hWndText);
  TextMessage ();
  
  return (0);
}
/*-----------------------------------------------------------------------------------*/
/* close a text window */
EXPORT void WINAPI TextClose (LPTW lptw)
{
  HGLOBAL hglobal;

  /* close window */
  if (lptw->hWndParent)DestroyWindow (lptw->hWndParent);
  TextMessage ();

  hglobal = GlobalHandle (lptw->ScreenBuffer);
  if (hglobal)
    {
      GlobalUnlock (hglobal);
      GlobalFree (hglobal);
    }
  hglobal = GlobalHandle (lptw->AttrBuffer);
  if (hglobal)
    {
      GlobalUnlock (hglobal);
      GlobalFree (hglobal);
    }
  hglobal = GlobalHandle (lptw->KeyBuf);
  if (hglobal)
    {
      GlobalUnlock (hglobal);
      GlobalFree (hglobal);
    }

  if (lptw->lpmw)  CloseMacros (lptw);
  lptw->hWndParent = (HWND) NULL;
}
/*-----------------------------------------------------------------------------------*/
/* Bring Cursor into text window */
EXPORT void WINAPI TextToCursor (LPTW lptw)
{
  int nXinc = 0;
  int nYinc = 0;
  int cxCursor;
  int cyCursor;
  cyCursor = lptw->CursorPos.y * lptw->CharSize.y;
  if ((cyCursor + lptw->CharSize.y > lptw->ScrollPos.y + lptw->ClientSize.y)
      || (cyCursor < lptw->ScrollPos.y))
    {
      nYinc = max (0, cyCursor + lptw->CharSize.y - lptw->ClientSize.y) - lptw->ScrollPos.y;
      nYinc = min (nYinc, lptw->ScrollMax.y - lptw->ScrollPos.y);
    }
  cxCursor = lptw->CursorPos.x * lptw->CharSize.x;
  if ((cxCursor + lptw->CharSize.x > lptw->ScrollPos.x + lptw->ClientSize.x)
      || (cxCursor < lptw->ScrollPos.x))
    {
      nXinc = max (0, cxCursor + lptw->CharSize.x - lptw->ClientSize.x / 2) - lptw->ScrollPos.x;
      nXinc = min (nXinc, lptw->ScrollMax.x - lptw->ScrollPos.x);
    }
  if (nYinc || nXinc)
    {
      lptw->ScrollPos.y += nYinc;
      lptw->ScrollPos.x += nXinc;
      ScrollWindow (lptw->hWndText, -nXinc, -nYinc, NULL, NULL);
      SetScrollPos (lptw->hWndText, SB_VERT, lptw->ScrollPos.y, TRUE);

      SetScrollPos (lptw->hWndText, SB_HORZ, lptw->ScrollPos.x, TRUE);
      UpdateWindow (lptw->hWndText);
    }
}
/*-----------------------------------------------------------------------------------*/
void NewLine (LPTW lptw)
{
	lptw->CursorPos.x = 0;
	lptw->CursorPos.y++;
	if (lptw->CursorPos.y >= lptw->ScreenSize.y) {
	    int i =  lptw->ScreenSize.x * (lptw->ScreenSize.y - 1);
		_fmemmove(lptw->ScreenBuffer, lptw->ScreenBuffer+lptw->ScreenSize.x, i);
		_fmemset(lptw->ScreenBuffer + i, ' ', lptw->ScreenSize.x);
		_fmemmove(lptw->AttrBuffer, lptw->AttrBuffer+lptw->ScreenSize.x, i);
		_fmemset(lptw->AttrBuffer + i, NOTEXT, lptw->ScreenSize.x);
		lptw->CursorPos.y--;
		ScrollWindow(lptw->hWndText,0,-lptw->CharSize.y,NULL,NULL);
		lptw->MarkBegin.y--;
		lptw->MarkEnd.y--;
		LimitMark(lptw, &lptw->MarkBegin);
		LimitMark(lptw, &lptw->MarkEnd);
		UpdateWindow(lptw->hWndText);
	}
	if (lptw->CursorFlag)
		TextToCursor(lptw);
	TextMessage();
}

/*-----------------------------------------------------------------------------------*/
/* Update count characters in window at cursor position */
/* Updates cursor position */
void UpdateText (LPTW lptw, int count)
{
  HDC hdc;
  int xpos, ypos;
  xpos = lptw->CursorPos.x * lptw->CharSize.x - lptw->ScrollPos.x;
  ypos = lptw->CursorPos.y * lptw->CharSize.y - lptw->ScrollPos.y;
  hdc = GetDC (lptw->hWndText);

  SetTextColor (hdc, GetSysColor (COLOR_WINDOWTEXT));
  SetBkColor (hdc, GetSysColor (COLOR_WINDOW));

  //if (lptw->bSysColors)
  //  {
  //    SetTextColor (hdc, GetSysColor (COLOR_WINDOWTEXT));
  //    SetBkColor (hdc, GetSysColor (COLOR_WINDOW));
  //  }
  //else
  //  {
  //    SetTextColor (hdc, TextFore (lptw->Attr));
  //    SetBkColor (hdc, TextBack (lptw->Attr));
  //  }

  SelectFont (hdc, lptw->hfont);
  TextOut (hdc, xpos, ypos,
      (LPSTR) (lptw->ScreenBuffer + lptw->CursorPos.y * lptw->ScreenSize.x +
	       lptw->CursorPos.x), count);
  (void) ReleaseDC (lptw->hWndText, hdc);
  lptw->CursorPos.x += count;
  if (lptw->CursorPos.x >= lptw->ScreenSize.x)
    NewLine (lptw);
}
/*-----------------------------------------------------------------------------------*/
EXPORT int WINAPI TextPutCh (LPTW lptw, BYTE ch)
{
  int pos;

  switch (ch)
    {
    case '\r':
      lptw->CursorPos.x = 0;
      if (lptw->CursorFlag)
	TextToCursor (lptw);
      break;
    case '\n':
      NewLine (lptw);
      break;
    case 7:
      MessageBeep (MB_OK);
      if (lptw->CursorFlag)
	TextToCursor (lptw);
      break;
    case '\t':
      {
	int n;
	for (n = 8 - (lptw->CursorPos.x % 8); n > 0; n--)
	  TextPutCh (lptw, ' ');
      }
      break;
    case 0x08:
    case 0x7f:
      lptw->CursorPos.x--;
      if (lptw->CursorPos.x < 0)
	{
	  lptw->CursorPos.x = lptw->ScreenSize.x - 1;
	  lptw->CursorPos.y--;
	}
      if (lptw->CursorPos.y < 0)
	lptw->CursorPos.y = 0;
      break;
    default:
	
      pos = lptw->CursorPos.y * lptw->ScreenSize.x + lptw->CursorPos.x;
      lptw->ScreenBuffer[pos] = ch;
      lptw->AttrBuffer[pos] = lptw->Attr;
      UpdateText (lptw, 1);
    }
  return ch;
}
/*-----------------------------------------------------------------------------------*/
void TextPutStr (LPTW lptw, LPSTR str)
{
  BYTE FAR *p, FAR * pa;
  int count, limit;
  while (*str)
    {
      p = lptw->ScreenBuffer + lptw->CursorPos.y * lptw->ScreenSize.x + lptw->CursorPos.x;
      pa = lptw->AttrBuffer + lptw->CursorPos.y * lptw->ScreenSize.x + lptw->CursorPos.x;
      limit = lptw->ScreenSize.x - lptw->CursorPos.x;
      for (count = 0; (count < limit) && *str
	   && (isprint ((int) *str) || *str == '\t'); count++)
	{
	  if (*str == '\t')
	    {
	      int n;
	      for (n = 8 - ((lptw->CursorPos.x + count) % 8); (count < limit) & (n > 0); n--, count++)
		{
		  *p++ = ' ';
		  *pa++ = lptw->Attr;
		}
	      str++;
	      count--;
	    }
	  else
	    {
	      *p++ = *str++;
	      *pa++ = lptw->Attr;
	    }
	}
      if (count > 0)
	{
	  UpdateText (lptw, count);
	}
      if (*str == '\n')
	{
	  NewLine (lptw);
	  str++;
	}
      else if (*str && !isprint ((int) *str) && *str != '\t')
	{
	  TextPutCh (lptw, *str++);
	}
    }
}
/*-----------------------------------------------------------------------------------*/
void LimitMark (LPTW lptw, POINT FAR * lppt)
{
  if (lppt->x < 0)
    lppt->x = 0;
  if (lppt->y < 0)
    {
      lppt->x = 0;
      lppt->y = 0;
    }
  if (lppt->x > lptw->ScreenSize.x)
    lppt->x = lptw->ScreenSize.x;
  if (lppt->y >= lptw->ScreenSize.y)
    {
      lppt->x = 0;
      lppt->y = lptw->ScreenSize.y;
    }
}
/*-----------------------------------------------------------------------------------*/
void ClearMark (LPTW lptw, POINT pt)
{
  RECT rect1, rect2, rect3;
  int tmp;
  if ((lptw->MarkBegin.x != lptw->MarkEnd.x) ||
      (lptw->MarkBegin.y != lptw->MarkEnd.y))
    {
      if (lptw->MarkBegin.x > lptw->MarkEnd.x)
	{
	  tmp = lptw->MarkBegin.x;
	  lptw->MarkBegin.x = lptw->MarkEnd.x;
	  lptw->MarkEnd.x = tmp;
	}
      if (lptw->MarkBegin.y > lptw->MarkEnd.y)
	{
	  tmp = lptw->MarkBegin.y;
	  lptw->MarkBegin.y = lptw->MarkEnd.y;
	  lptw->MarkEnd.y = tmp;
	}
      /* calculate bounding rectangle in character coordinates */
      if (lptw->MarkBegin.y != lptw->MarkEnd.y)
	{
	  rect1.left = 0;
	  rect1.right = lptw->ScreenSize.x;
	}
      else
	{
	  rect1.left = lptw->MarkBegin.x;
	  rect1.right = lptw->MarkEnd.x + 1;
	}
      rect1.top = lptw->MarkBegin.y;
      rect1.bottom = lptw->MarkEnd.y + 1;
      /* now convert to client coordinates */
      rect1.left = rect1.left * lptw->CharSize.x - lptw->ScrollPos.x;
      rect1.right = rect1.right * lptw->CharSize.x - lptw->ScrollPos.x;
      rect1.top = rect1.top * lptw->CharSize.y - lptw->ScrollPos.y;
      rect1.bottom = rect1.bottom * lptw->CharSize.y - lptw->ScrollPos.y;
      /* get client rect and calculate intersection */
      GetClientRect (lptw->hWndText, &rect2);
      IntersectRect (&rect3, &rect1, &rect2);
      /* update window if necessary */
      if (!IsRectEmpty (&rect3))
	{
	  InvalidateRect (lptw->hWndText, &rect3, TRUE);
	}
    }
  LimitMark (lptw, &pt);
  lptw->MarkBegin.x = lptw->MarkEnd.x = pt.x;
  lptw->MarkBegin.y = lptw->MarkEnd.y = pt.y;
  UpdateWindow (lptw->hWndText);
}
/*-----------------------------------------------------------------------------------*/
/* output a line including attribute changes as needed */
void DoLine(LPTW lptw, HDC hdc, int xpos, int ypos, int offset, int count)
{
	BYTE FAR *pa, attr;
	int idx, num;
	pa = lptw->AttrBuffer + offset;
	if ((offset < 0) || (offset >= lptw->ScreenSize.x*lptw->ScreenSize.y))
	MessageBox((HWND)NULL, "panic", "panic", MB_OK | MB_ICONEXCLAMATION);
	idx = 0;
	num = count;
	while (num > 0)
	{
		attr = *pa;
		while ((num > 0) && (attr == *pa))
		{
			/* skip over bytes with same attribute */
			num--;
			pa++;
		}

		SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
		SetBkColor(hdc, GetSysColor(COLOR_WINDOW));

		//if (lptw->bSysColors)
		//{
		//    SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
		//    SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
		//}
		//else
		//{
		//    SetTextColor(hdc, TextFore(attr));
		//    SetBkColor(hdc, TextBack(attr));
		//}
		TextOut(hdc,xpos,ypos, (LPSTR)(lptw->ScreenBuffer + offset + idx),
			count-num-idx);
		xpos += lptw->CharSize.x * (count-num-idx);
		idx = count-num;
	}
}
/*-----------------------------------------------------------------------------------*/
void DoMark (LPTW lptw, POINT pt, POINT end, BOOL mark)
{
  int xpos, ypos;
  HDC hdc;
  int count;
  int offset;
  offset = lptw->ScreenSize.x * pt.y + pt.x;
  hdc = GetDC (lptw->hWndText);
  SelectFont (hdc, lptw->hfont);

  SetTextColor (hdc, GetSysColor (COLOR_HIGHLIGHTTEXT));
  SetBkColor (hdc, GetSysColor (COLOR_HIGHLIGHT));

  //if (lptw->bSysColors)
  //  {
  //    SetTextColor (hdc, GetSysColor (COLOR_HIGHLIGHTTEXT));
  //    SetBkColor (hdc, GetSysColor (COLOR_HIGHLIGHT));
  //  }
  //else
  //  {
  //    SetTextColor (hdc, MARKFORE);
  //    SetBkColor (hdc, MARKBACK);
  //  }

  while (pt.y < end.y)
    {
      /* multiple lines */
      xpos = pt.x * lptw->CharSize.x - lptw->ScrollPos.x;
      ypos = pt.y * lptw->CharSize.y - lptw->ScrollPos.y;
      count = lptw->ScreenSize.x - pt.x;
      if (mark)
	TextOut (hdc, xpos, ypos, (LPSTR) (lptw->ScreenBuffer + offset), count);
      else
	{
	  DoLine (lptw, hdc, xpos, ypos, offset, count);

	  SetTextColor (hdc, GetSysColor (COLOR_HIGHLIGHTTEXT));
	  SetBkColor (hdc, GetSysColor (COLOR_HIGHLIGHT));

	  //if (lptw->bSysColors)
	  //  {
	  //    SetTextColor (hdc, GetSysColor (COLOR_HIGHLIGHTTEXT));
	  //    SetBkColor (hdc, GetSysColor (COLOR_HIGHLIGHT));
	  //  }
	  //else
	  //  {
	  //    SetTextColor (hdc, MARKFORE);
	  //    SetBkColor (hdc, MARKBACK);
	  //  }
	}
      offset += count;
      pt.y++;
      pt.x = 0;
    }
  /* partial line */
  xpos = pt.x * lptw->CharSize.x - lptw->ScrollPos.x;
  ypos = pt.y * lptw->CharSize.y - lptw->ScrollPos.y;
  count = end.x - pt.x;
  if (end.y != lptw->ScreenSize.y)
    {
      if (mark)
	TextOut (hdc, xpos, ypos, (LPSTR) (lptw->ScreenBuffer + offset), count);
      else
	DoLine (lptw, hdc, xpos, ypos, offset, count);
    }
  (void) ReleaseDC (lptw->hWndText, hdc);
}
/*-----------------------------------------------------------------------------------*/
void UpdateMark (LPTW lptw, POINT pt)
{
  int begin, point, end;
  LimitMark (lptw, &pt);
  begin = lptw->ScreenSize.x * lptw->MarkBegin.y + lptw->MarkBegin.x;
  point = lptw->ScreenSize.x * pt.y + pt.x;
  end = lptw->ScreenSize.x * lptw->MarkEnd.y + lptw->MarkEnd.x;

  if (begin <= end)
    {
      /* forward mark */
      if (point >= end)
	{
	  /* extend marked area */
	  DoMark (lptw, lptw->MarkEnd, pt, TRUE);
	}
      else if (point >= begin)
	{
	  /* retract marked area */
	  DoMark (lptw, pt, lptw->MarkEnd, FALSE);
	}
      else
	{			/* retract and reverse */
	  DoMark (lptw, lptw->MarkBegin, lptw->MarkEnd, FALSE);
	  DoMark (lptw, pt, lptw->MarkBegin, TRUE);
	}
    }
  else
    {
      /* reverse mark */
      if (point <= end)
	{
	  /* extend marked area */
	  DoMark (lptw, pt, lptw->MarkEnd, TRUE);
	}
      else if (point <= begin)
	{
	  /* retract marked area */
	  DoMark (lptw, lptw->MarkEnd, pt, FALSE);
	}
      else
	{			/* retract and reverse */
	  DoMark (lptw, lptw->MarkEnd, lptw->MarkBegin, FALSE);
	  DoMark (lptw, lptw->MarkBegin, pt, TRUE);
	}
    }
  lptw->MarkEnd.x = pt.x;
  lptw->MarkEnd.y = pt.y;
}
/*-----------------------------------------------------------------------------------*/
/***********************************
 *  drag-drop feature 
 ***********************************/
void DragFunc (LPTW lptw, HDROP hdrop)
{
  static char szFile[80];
  int i, cFiles;
  LPSTR p;
  if ((lptw->DragPre == (LPSTR) NULL) || (lptw->DragPost == (LPSTR) NULL))
    return;
  cFiles = DragQueryFile (hdrop, 0xffffffff, (LPSTR) NULL, 0);
  for (i = 0; i < cFiles; i++)
    {
      DragQueryFile (hdrop, i, szFile, 80);
      for (p = lptw->DragPre; *p; p++)
	SendMessage (lptw->hWndText, WM_CHAR, *p, 1L);
      for (p = szFile; *p; p++)
	SendMessage (lptw->hWndText, WM_CHAR, *p, 1L);
      for (p = lptw->DragPost; *p; p++)
	SendMessage (lptw->hWndText, WM_CHAR, *p, 1L);
    }
  DragFinish (hdrop);
}
/*-----------------------------------------------------------------------------------*/
void TextMakeFont (LPTW lptw)
{
  LOGFONT lf;
  TEXTMETRIC tm;
  LPSTR p;
  HDC hdc;

  hdc = GetDC (lptw->hWndText);
  memset (&lf, 0, sizeof (LOGFONT));
  strncpy (lf.lfFaceName, lptw->fontname, LF_FACESIZE);
  lf.lfHeight = -MulDiv (lptw->fontsize, GetDeviceCaps (hdc, LOGPIXELSY), 72);
  lf.lfPitchAndFamily = FIXED_PITCH;
  lf.lfCharSet = SCI_DEFAULT_CHARSET;
  if ((p = strstr (lptw->fontname, " Italic")) != (LPSTR) NULL)
    {
      lf.lfFaceName[(unsigned int) (p - lptw->fontname)] = '\0';
      lf.lfItalic = TRUE;
    }
  if ((p = strstr (lptw->fontname, " Bold")) != (LPSTR) NULL)
    {
      lf.lfFaceName[(unsigned int) (p - lptw->fontname)] = '\0';
      lf.lfWeight = FW_BOLD;
    }
  if (lptw->hfont != 0)
    DeleteFont (lptw->hfont);
  lptw->hfont = CreateFontIndirect ((LOGFONT FAR *) & lf);
  /* get text size */
  SelectFont (hdc, lptw->hfont);
  GetTextMetrics (hdc, (TEXTMETRIC FAR *) & tm);
  lptw->CharSize.y = tm.tmHeight;
  lptw->CharSize.x = tm.tmAveCharWidth;
  lptw->CharAscent = tm.tmAscent;
  if (lptw->bFocus)
    CreateCaret (lptw->hWndText, 0, lptw->CharSize.x, 2 + lptw->CaretHeight);
  ReleaseDC (lptw->hWndText, hdc);
  return;
}
/*-----------------------------------------------------------------------------------*/
void TextSelectFont (LPTW lptw)
{
  LOGFONT lf;
  CHOOSEFONT cf;
  HDC hdc;
  char lpszStyle[LF_FACESIZE];
  LPSTR p;
  /* Set all structure fields to zero. */
  memset (&cf, 0, sizeof (CHOOSEFONT));
  memset (&lf, 0, sizeof (LOGFONT));
  cf.lStructSize = sizeof (CHOOSEFONT);
  cf.hwndOwner = lptw->hWndParent;
  strncpy (lf.lfFaceName, lptw->fontname, LF_FACESIZE);
  if ((p = strstr (lptw->fontname, " Bold")) != (LPSTR) NULL)
    {
      strncpy (lpszStyle, p + 1, LF_FACESIZE);
      lf.lfFaceName[(unsigned int) (p - lptw->fontname)] = '\0';
    }
  else if ((p = strstr (lptw->fontname, " Italic")) != (LPSTR) NULL)
    {
      strncpy (lpszStyle, p + 1, LF_FACESIZE);
      lf.lfFaceName[(unsigned int) (p - lptw->fontname)] = '\0';
    }
  else
    strcpy (lpszStyle, "Regular");
  cf.lpszStyle = lpszStyle;
  hdc = GetDC (lptw->hWndText);
  lf.lfHeight = -MulDiv (lptw->fontsize, GetDeviceCaps (hdc, LOGPIXELSY), 72);
  ReleaseDC (lptw->hWndText, hdc);
  lf.lfPitchAndFamily = FIXED_PITCH;
  cf.lpLogFont = &lf;
  cf.nFontType = SCREEN_FONTTYPE;
  cf.Flags = CF_SCREENFONTS | CF_FIXEDPITCHONLY | CF_INITTOLOGFONTSTRUCT | CF_USESTYLE;
  if (ChooseFont (&cf))
    {
      RECT rect;
      strcpy (lptw->fontname, lf.lfFaceName);
      lptw->fontsize = cf.iPointSize / 10;
      if (cf.nFontType & BOLD_FONTTYPE)
	lstrcat (lptw->fontname, " Bold");
      if (cf.nFontType & ITALIC_FONTTYPE)
	lstrcat (lptw->fontname, " Italic");
      TextMakeFont (lptw);
      /* force a window update */
      GetClientRect (lptw->hWndText, (LPRECT) & rect);
      SendMessage (lptw->hWndText, WM_SIZE, SIZE_RESTORED,
	       MAKELPARAM (rect.right - rect.left, rect.bottom - rect.top));
      GetClientRect (lptw->hWndText, (LPRECT) & rect);
      InvalidateRect (lptw->hWndText, (LPRECT) & rect, 1);
      UpdateWindow (lptw->hWndText);
    }
}
/*-----------------------------------------------------------------------------------*/
/* parent overlapped window */
EXPORT LRESULT CALLBACK WndParentProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  HDC hdc;
  PAINTSTRUCT ps;
  RECT rect;
  LPTW lptw;
  
  lptw = (LPTW) GetWindowLong (hwnd, 0);
  switch (message)
    {
		case WM_SYSCOMMAND:
			switch (LOWORD (wParam))
			{
				case M_CONSOLE:
					{
	  		      		SwitchConsole();
  						SetActiveWindow(hwnd);
					}
				return 0;
				case M_SPECIALPASTE:
				case M_COPY_CLIP:
				case M_PASTE:
				case M_CHOOSE_FONT:
				case M_SYSCOLORS:
				case M_WRITEINI:
				case M_ABOUT:
						SendMessage (lptw->hWndText, WM_COMMAND, wParam, lParam);
			}
		break;
		case WM_SETFOCUS:
			if (IsWindow (lptw->hWndText))
				{
					SetFocus (lptw->hWndText);
					return (0);
				}
		break;
		case WM_GETMINMAXINFO:
			{
				POINT *MMinfo = (POINT *) lParam;
				TEXTMETRIC tm;
				hdc = GetDC (hwnd);
				SelectFont (hdc, GetStockFont (OEM_FIXED_FONT));
				GetTextMetrics (hdc, (LPTEXTMETRIC) & tm);
				ReleaseDC (hwnd, hdc);
				/* minimum size */
				MMinfo[3].x = ScreenMinSize.x * tm.tmAveCharWidth+ GetSystemMetrics (SM_CXVSCROLL) + 2 * GetSystemMetrics (SM_CXFRAME);
				MMinfo[3].y = ScreenMinSize.y * tm.tmHeight+ GetSystemMetrics (SM_CYHSCROLL) + 2 * GetSystemMetrics (SM_CYFRAME)+ GetSystemMetrics (SM_CYCAPTION);
			}
		return (0);
		case WM_SIZE:
			{
    			if (lptw->lpmw->ShowToolBar)
    			{
    		  		/* Affichage Zone Toolbar */
    				SetWindowPos (lptw->hWndText, (HWND) NULL, 0, lptw->ButtonHeight,
		    					  LOWORD (lParam), HIWORD (lParam) - lptw->ButtonHeight,
		    					  SWP_NOZORDER | SWP_NOACTIVATE);
    			}
    			else
    			{
    				/* Pas de zone Toolbar */
    				SetWindowPos (lptw->hWndText, (HWND) NULL, 0, 0,
		    					  LOWORD (lParam), HIWORD (lParam),
		    					  SWP_NOZORDER | SWP_NOACTIVATE);
	
    			}	   
    			InvalidateRect(lptw->hWndParent, (LPRECT) NULL, TRUE);
    			InvalidateRect(lptw->hWndText, (LPRECT) NULL, TRUE);
    			UpdateWindow (lptw->hWndText);
			}
		return (0);

	case WM_ENTERSIZEMOVE :
		SaveCurrentLine(FALSE);
	return (0);

	case WM_EXITSIZEMOVE :
		{
		extern char copycur_line[1024];
		WriteTextIni(lptw); /* Sauvegarde Position apres deplacement et redimensionnement */
		SendCTRLandAKey(CTRLU); /* Scrollbar */
		WriteIntoKeyBuffer(lptw,copycur_line);
		
		}
	return (0);
	case WM_COMMAND:
		if (IsWindow (lptw->hWndText))SetFocus (lptw->hWndText);
		SendMessage (lptw->hWndText, message, wParam, lParam);
		/* pass on menu commands */
		return (0);
		case WM_PAINT:
		{
			hdc = BeginPaint (hwnd, &ps);

			

			if (lptw->ButtonHeight)
			{
				HPEN hPen,hPenOld; 
				HBRUSH hbrush;

				GetClientRect (hwnd, &rect);
				hbrush = CreateSolidBrush (GetSysColor (COLOR_BTNFACE));
				rect.bottom = lptw->ButtonHeight - 1;
				FillRect (hdc, &rect, hbrush);
				DeleteBrush (hbrush);
			
				hPen = CreatePen(PS_SOLID,1,RGB(255,255,255));
				hPenOld = SelectObject(hdc, hPen); 
				MoveToEx (hdc, rect.left-1,0, NULL);
				LineTo (hdc, rect.right+1,0);
				SelectObject(hdc, hPenOld); 
				DeleteObject(hPen); 

				hPen = CreatePen(PS_SOLID,1,GetSysColor (COLOR_GRAYTEXT));
				hPenOld = SelectObject(hdc, hPen); 
				MoveToEx (hdc, rect.left, lptw->ButtonHeight - 1, NULL);
				LineTo (hdc, rect.right, lptw->ButtonHeight - 1);
				SelectObject(hdc, hPenOld); 
				DeleteObject(hPen); 
			}
			EndPaint (hwnd, &ps);
			return 0;
		}
		case WM_DROPFILES:
		{
			DragFunc (lptw, (HDROP) wParam);
		}
		break;
		case WM_CREATE:
		{

			
			lptw = ((CREATESTRUCT *) lParam)->lpCreateParams;
	
			SetWindowLong (hwnd, 0, (LONG) lptw);
			lptw->hWndParent = hwnd;
			/* get character size */
			
			if ((lptw->DragPre != (LPSTR) NULL) && (lptw->DragPost != (LPSTR) NULL)) DragAcceptFiles (hwnd, TRUE);
	
			/* Modification Allan CORNET 15/07/03 */
			/* Renomme la fenetre avec VERSION et numero x associé à la console*/  
			{
				char CopyNameConsole[MAX_PATH];
				char *FirstOccurence;
				char *SecondOccurence;
		
				strcpy(CopyNameConsole,ScilexConsoleName);
				FirstOccurence = strtok(CopyNameConsole,"("); 
				SecondOccurence= strtok(NULL,"("); 
    			wsprintf(ScilexWindowName,"%s (%s",VERSION,SecondOccurence);
    		
    			SetWindowText(hwnd,ScilexWindowName);  
    		}
        }
		break;
		case WM_DESTROY:
			DragAcceptFiles (hwnd, FALSE);
			DeleteFont (lptw->hfont);
			lptw->hfont = 0;
      
			/* Tue le Process Scilex si OS est Windows 9x */
			Kill_Scilex_Win98();
        break;
		case WM_CLOSE: 
           /* Allan CORNET 11/07/03 Bug Win 98 */
           /* Sortie Meme durant l'execution d'un script */
           /* 28/11/03 */
           ExitWindow();
           return (0);
		break;

		case WM_COPYDATA:
		{
			char Command[MAX_PATH];
			char TitleWndSend[MAX_PATH];

			ReceiveFromAnotherScilab(wParam,lParam);

			if ( GetCommandFromAnotherScilab(TitleWndSend,Command) )
				StoreCommand1 (Command, 0);
			return (0);
		}
		break;
	}
  return DefWindowProc (hwnd, message, wParam, lParam);
}
/*-----------------------------------------------------------------------------------*/
/* child text window */
EXPORT LRESULT CALLBACK WndTextProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	
  HDC hdc;
  PAINTSTRUCT ps;
  RECT rect;
  int nYinc=0, nXinc=0, nl, nc;
  LPTW lptw;
  lptw = (LPTW) GetWindowLong (hwnd, 0);
  switch (message)
    {
	case WM_SETCURSOR :
         SetCursor( LoadCursor( NULL, IDC_ARROW ) );
     break;
    case WM_SETFOCUS:
      lptw->bFocus = TRUE;
      CreateCaret (hwnd, 0, lptw->CharSize.x, 2 + lptw->CaretHeight);
      SetCaretPos (lptw->CursorPos.x * lptw->CharSize.x - lptw->ScrollPos.x,
		   lptw->CursorPos.y * lptw->CharSize.y + lptw->CharAscent
		   - lptw->CaretHeight - lptw->ScrollPos.y);
      if (lptw->bGetCh)
	ShowCaret (hwnd);
      break;
    case WM_KILLFOCUS:
      DestroyCaret ();
      lptw->bFocus = FALSE;
      break;
    case WM_SIZE:
	  {
	  lptw->ClientSize.y = HIWORD (lParam);
      lptw->ClientSize.x = LOWORD (lParam);
      nc = lptw->ClientSize.x / lptw->CharSize.x;
      nl = lptw->ClientSize.y / lptw->CharSize.y;
	  /** send number of lines info to scilab **/
      nl = (nl > 5) ? nl : 5;
	  /** to avoid lines set to 0 when iconified **/
      //C2F (scilines) (&nl, &nc);
      
	  lptw->ScrollMax.y = max (0, lptw->CharSize.y * lptw->ScreenSize.y - lptw->ClientSize.y);
      lptw->ScrollPos.y = min (lptw->ScrollPos.y, lptw->ScrollMax.y);

      SetScrollRange (hwnd, SB_VERT, 0, lptw->ScrollMax.y, FALSE);    
      SetScrollPos (hwnd, SB_VERT, lptw->ScrollPos.y, TRUE);

      lptw->ScrollMax.x = max (0, lptw->CharSize.x * lptw->ScreenSize.x - lptw->ClientSize.x);
      lptw->ScrollPos.x = min (lptw->ScrollPos.x, lptw->ScrollMax.x);

      SetScrollRange (hwnd, SB_HORZ, 0, lptw->ScrollMax.x, FALSE);
      SetScrollPos (hwnd, SB_HORZ, lptw->ScrollPos.x, TRUE);

		
	if (lptw->bFocus && lptw->bGetCh)
	{
	  SetCaretPos (lptw->CursorPos.x * lptw->CharSize.x - lptw->ScrollPos.x,
		     lptw->CursorPos.y * lptw->CharSize.y + lptw->CharAscent
		       - lptw->CaretHeight - lptw->ScrollPos.y);
	  ShowCaret (hwnd);
	}
	  }	
	  return (0);

	   
    case 0x020A :/*WM_MOUSEWHEEL*/
		{
			int steps=((short) HIWORD(wParam))/120;
			if( steps > 0 ) 
				{
					SendMessage (hwnd, WM_VSCROLL, SB_LINEUP, (LPARAM) 0);
				}

				if( steps < 0 ) 
				{
					SendMessage (hwnd, WM_VSCROLL, SB_LINEDOWN, (LPARAM) 0);
				}
		}
	return (0);  
     
	
    case WM_VSCROLL: /* Messages ScrollBar Verticale */
    /* Modification Allan CORNET 09/07/03 */
    /* Gestion ScrollBar en deplacement et autres*/
      switch (LOWORD (wParam))
	{
		case SB_ENDSCROLL:
			nYinc = 0;
		break;
		case SB_TOP:
			nYinc = -lptw->ScrollPos.y;
		break;
		case SB_BOTTOM:
			nYinc = lptw->ScrollMax.y - lptw->ScrollPos.y;
		break;
		case SB_LINEUP:
			nYinc = -lptw->CharSize.y;
		break;
		case SB_LINEDOWN:
			nYinc = lptw->CharSize.y;
		break;
		case SB_PAGEUP:
			nYinc = min(-1,-lptw->ClientSize.y);
		break;
		case SB_PAGEDOWN:
			nYinc = max(1,lptw->ClientSize.y);
		break;
		case SB_THUMBPOSITION:
			nYinc = HIWORD (wParam) - lptw->ScrollPos.y;
		break;
		case SB_THUMBTRACK:
			nYinc = HIWORD (wParam) - lptw->ScrollPos.y;
		break;
		default:
			nYinc = 0;
		break;
		
	}
	
	if ((nYinc = max (-lptw->ScrollPos.y,
			min (nYinc, lptw->ScrollMax.y - lptw->ScrollPos.y)))
	  != 0)
	{
	  lptw->ScrollPos.y += nYinc;
	  ScrollWindow (hwnd, 0, -nYinc, NULL, NULL);
	  SetScrollPos (hwnd, SB_VERT, lptw->ScrollPos.y, TRUE);
	  UpdateWindow (hwnd);
	}
      
      return (0);
      

	

    case WM_HSCROLL: /* Messages ScrollBar Horizontale */
    /* Correction Allan CORNET 09/07/03 */
    /* Gestion de la ScrollBar Horizontale */
    /* Gestion ScrollBar en deplacement et autres*/
      switch (LOWORD (wParam))
	{
		case SB_ENDSCROLL:
			nXinc = 0;
		break;

		case SB_LEFT:
			nXinc = 0;
		break;

		case SB_RIGHT:
			 nXinc =lptw->ClientSize.x;
		break;

		case SB_LINELEFT:
			nXinc = -lptw->CharSize.x;
		break;

		case SB_LINERIGHT:
			nXinc = lptw->CharSize.x;
		break;

		case SB_PAGELEFT:
			nXinc = min(-1,-lptw->ClientSize.x);
		break;

		case SB_PAGERIGHT:
			nXinc = max(1,lptw->ClientSize.x);
		break;

		case SB_THUMBPOSITION:
			nXinc = HIWORD (wParam) - lptw->ScrollPos.x;
		break;

		case SB_THUMBTRACK:
			nXinc = HIWORD (wParam) - lptw->ScrollPos.x;
		break;

		default:
	  		nXinc = 0;
	  	break;
	}
		
	if ((nXinc = max (-lptw->ScrollPos.x,
			min (nXinc, lptw->ScrollMax.x - lptw->ScrollPos.x)))
	  != 0)
	{
	  	lptw->ScrollPos.x += nXinc;
	  	ScrollWindow (hwnd, -nXinc, 0, NULL, NULL);
	  	SetScrollPos (hwnd, SB_HORZ, lptw->ScrollPos.x, TRUE);
	  	UpdateWindow (hwnd);
	}	

      return (0);
      
    case WM_KEYDOWN:
      if (GetKeyState (VK_SHIFT) < 0)
	{
	  switch (wParam)
	    {
	    case VK_HOME:
	      SendMessage (hwnd, WM_VSCROLL, SB_TOP, (LPARAM) 0);
	      break;
	    case VK_END:
	      SendMessage (hwnd, WM_VSCROLL, SB_BOTTOM, (LPARAM) 0);
	      break;
	    case VK_PRIOR:
	      SendMessage (hwnd, WM_VSCROLL, SB_PAGEUP, (LPARAM) 0);
	      break;
	    case VK_NEXT:
	      SendMessage (hwnd, WM_VSCROLL, SB_PAGEDOWN, (LPARAM) 0);
	      break;
	    case VK_UP:
	      SendMessage (hwnd, WM_VSCROLL, SB_LINEUP, (LPARAM) 0);
	      break;
	    case VK_DOWN:
	      SendMessage (hwnd, WM_VSCROLL, SB_LINEDOWN, (LPARAM) 0);
	      break;
	    case VK_LEFT:
	      SendMessage (hwnd, WM_HSCROLL, SB_LINELEFT, (LPARAM) 0);
	      break;
	    case VK_RIGHT:
	      SendMessage (hwnd, WM_HSCROLL, SB_LINERIGHT, (LPARAM) 0);
	      break;
	    case VK_INSERT:
	      /* Modification Allan CORNET 09/07/03 */
	      /* Touches Shift-Insert effectue un "coller" du presse papier */
	    	{
			SendMessage (hwnd, WM_COMMAND,M_PASTE, (LPARAM) 0);
	    	}
	      break;	
	    }
	}
      else
	{
		/* Modification Allan CORNET 09/07/03 */
	      
		if (GetKeyState (VK_CONTROL) < 0)
		{
			switch (wParam)
	    		{
	    			/* Touches Control-V effectue un "coller" du presse papier */
	    			case VK_V :
	    			{
	    				SendMessage (hwnd, WM_COMMAND,M_PASTE, (LPARAM) 0);
	    			}
	    			break;
	    			/* Touches Control-Insert Control-C effectue un "copier" dans presse papier */
	    			case VK_INSERT : 
	    			{
	    				if ( HasAZoneTextSelected(lptw) == TRUE ) TextCopyClip (lptw);
	    				return 0;
	    			}
	    			
	    			default:
	    			break;
	    		}
	    		
		}
		else
		{
	  switch (wParam)
	    {
	    	case VK_HOME:  case VK_END:    case VK_PRIOR:
	     	case VK_NEXT:  case VK_DELETE:
	     	case VK_UP:     case VK_DOWN: case VK_LEFT:  case VK_RIGHT:
	    	
	      		{			/* store key in circular buffer */
				long count;
				count = lptw->KeyBufIn - lptw->KeyBufOut;
				if (count < 0) count += lptw->KeyBufSize;
				if (count < (int) lptw->KeyBufSize - 2)
		  		{
		    			*lptw->KeyBufIn++ = 0;
		    			if (lptw->KeyBufIn - lptw->KeyBuf >= (int) lptw->KeyBufSize)
		      			lptw->KeyBufIn = lptw->KeyBuf;	/* wrap around */
		    			*lptw->KeyBufIn++ = HIWORD (lParam) & 0xff;
		    			if (lptw->KeyBufIn - lptw->KeyBuf >= (int) lptw->KeyBufSize)
		      			lptw->KeyBufIn = lptw->KeyBuf;	/* wrap around */
		  		}
		  		
	      		}
	      		
	      	break;
	      	
	      	
		/* Appel l'aide */	      
      		case VK_F1:
      		{
      			StoreCommand1 ("help()", 0);
      		}
      		break;
      		/* Efface la fenetre de commandes */
      		case VK_F2:
	      	{
	      		ClearCommandWindow(lptw,TRUE);
	      	}
	      	break;
	      	case VK_F3:
	      	{
			if (lptw->lpmw->LockToolBar == FALSE)
				{
					lptw->lpmw->ShowToolBar=!lptw->lpmw->ShowToolBar;
					
						ToolBarOnOff(lptw);
				}
	      		
	      	}
	      	break;

			/*
			case VK_F4:
			{
				SendCommandToAnotherScilab("Scilab-3.0-CVS (0)","Scilab-3.0-CVS (1)","scipad(SCI+'/scilab.star');");
			}
			break;
			*/
	      	
	      	/* Allan CORNET 21/10/03*/
	      	/* Reset des Menus & Boutons*/
	      	case VK_F11:
	      	{
	      		ResetMenu();
	      	}
      		break;        
	      	
	      	/* Allan CORNET 15/07/03*/
	      	/* Affiche ou cache la fenetre Scilex */
	      	case VK_F12:
	      	{
	      		SwitchConsole();
  			SetActiveWindow(hwnd);
      		}
      		break;        
      		
	    }
	}
	}
      break;
    case WM_RBUTTONDOWN:
      {
	POINT pt;
	pt.x = LOWORD (lParam);
	pt.y = HIWORD (lParam);
	
	
	
	ClientToScreen (hwnd, &pt);
	
	if ( IsEmptyClipboard(lptw) ) 
	  {
		EnableMenuItem(lptw->hPopMenu,M_PASTE,MF_GRAYED);
		EnableMenuItem(lptw->hPopMenu,M_SPECIALPASTE,MF_GRAYED);
	  }
	else
	  {
	  	EnableMenuItem(lptw->hPopMenu,M_PASTE,MF_ENABLED);
		EnableMenuItem(lptw->hPopMenu,M_SPECIALPASTE,MF_ENABLED);
	  }
	TrackPopupMenu (lptw->hPopMenu, TPM_LEFTALIGN,	pt.x, pt.y, 0, hwnd, NULL);
    	

      }
      return (0);
      case WM_MBUTTONUP: /*Bouton du milieu */
      {
      	PasteFunction(lptw,FALSE);
      }
      return (0);
    case WM_LBUTTONDOWN:
    /* Modification Allan CORNET le 12/08/03 */
    { 
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	pt.x = (pt.x + lptw->ScrollPos.x)/lptw->CharSize.x;
	pt.y = (pt.y + lptw->ScrollPos.y)/lptw->CharSize.y;
	ClearMark(lptw, pt);
	
	SetCapture(hwnd);	/* track the mouse */
	lptw->Marking = TRUE;
     }		
    break;

     
    case WM_LBUTTONUP:
      {	
      	/* finish marking text */
      	
      	ReleaseCapture ();
	lptw->Marking = FALSE;
        

	/* ensure begin mark is before end mark */
	
	if ((lptw->ScreenSize.x * lptw->MarkBegin.y + lptw->MarkBegin.x) >
	    		(lptw->ScreenSize.x * lptw->MarkEnd.y + lptw->MarkEnd.x))
	  	{
	    		POINT tmp;
	    		tmp.x = lptw->MarkBegin.x;
	    		tmp.y = lptw->MarkBegin.y;
	    		lptw->MarkBegin.x = lptw->MarkEnd.x;
	    		lptw->MarkBegin.y = lptw->MarkEnd.y;
	    		lptw->MarkEnd.x = tmp.x;
	    		lptw->MarkEnd.y = tmp.y;
   
	  	}

	  if  ( HasAZoneTextSelected(lptw) )
	  {
	  	/* Zone Selectionnée --> Activation Menus */
	  		EnableMenuItem(lptw->hPopMenu,M_COPY_CLIP,MF_ENABLED);
	    	EnableMenuItem(lptw->hPopMenu,M_HELPON,MF_ENABLED);
	    	EnableMenuItem(lptw->hPopMenu,M_PRINTSELECTION,MF_ENABLED);
	    	EnableMenuItem(lptw->hPopMenu,M_OPENSELECTION,MF_ENABLED);
	    	EnableMenuItem(lptw->hPopMenu,M_EVALSELECTION,MF_ENABLED);
	    	/* EnableMenuItem(lptw->hPopMenu,M_CUT,MF_ENABLED); */
	    	
	  }
	  else
	  {
	  	/*Pas de Zone Selectionnée -> Menus non activés */
	  	/* EnableMenuItem(lptw->hPopMenu,M_CUT,MF_GRAYED); */
	    	EnableMenuItem(lptw->hPopMenu,M_COPY_CLIP,MF_GRAYED);
	    	EnableMenuItem(lptw->hPopMenu,M_HELPON,MF_GRAYED);
	    	EnableMenuItem(lptw->hPopMenu,M_PRINTSELECTION,MF_GRAYED);
	    	EnableMenuItem(lptw->hPopMenu,M_OPENSELECTION,MF_GRAYED);
	    	EnableMenuItem(lptw->hPopMenu,M_EVALSELECTION,MF_GRAYED);

	  }
	
		
      }
      break;
    
    break;
    case WM_MOUSEMOVE:

		 
      if ((wParam & MK_LBUTTON) && lptw->Marking)
	{
		
	  RECT rect;
	  POINT pt;
	  pt.x = LOWORD (lParam);
	  pt.y = HIWORD (lParam);
	  
	  
	  
	  GetClientRect (hwnd, &rect);
	  if (PtInRect (&rect, pt))
	    {
	      pt.x = (pt.x + lptw->ScrollPos.x) / lptw->CharSize.x;
	      pt.y = (pt.y + lptw->ScrollPos.y) / lptw->CharSize.y;
	      UpdateMark (lptw, pt);
	    }
	  else
	    {
	      int nXinc;
	      int nYinc;
	      do
		{
		  nXinc = 0;
		  nYinc = 0;
		  if (pt.x > rect.right)
		    {
		      nXinc = lptw->CharSize.x * 4;
		      pt.x = (rect.right + lptw->ScrollPos.x) / lptw->CharSize.x + 2;
		    }
		  else if (pt.x < rect.left)
		    {
		      nXinc = -lptw->CharSize.x * 4;
		      pt.x = (rect.left + lptw->ScrollPos.x) / lptw->CharSize.x - 2;
		    }
		  else
		    pt.x = (pt.x + lptw->ScrollPos.x) / lptw->CharSize.x;
		  if (pt.y > rect.bottom)
		    {
		      nYinc = lptw->CharSize.y;
		      pt.y = (rect.bottom + lptw->ScrollPos.y) / lptw->CharSize.y + 1;
		    }
		  else if (pt.y < rect.top)
		    {
		      nYinc = -lptw->CharSize.y;
		      pt.y = (rect.top + lptw->ScrollPos.y) / lptw->CharSize.y - 1;
		    }
		  else
		    pt.y = (pt.y + lptw->ScrollPos.y) / lptw->CharSize.y;
		  LimitMark (lptw, &pt);
		  nXinc = max (nXinc, -lptw->ScrollPos.x);
		  nYinc = max (nYinc, -lptw->ScrollPos.y);
		  nYinc = min (nYinc, lptw->ScrollMax.y - lptw->ScrollPos.y);
		  nXinc = min (nXinc, lptw->ScrollMax.x - lptw->ScrollPos.x);
		  if (nYinc || nXinc)
		    {
		      lptw->ScrollPos.y += nYinc;
		      lptw->ScrollPos.x += nXinc;
		      ScrollWindow (lptw->hWndText, -nXinc, -nYinc, NULL, NULL);
		      SetScrollPos (lptw->hWndText, SB_VERT, lptw->ScrollPos.y, TRUE);
		      SetScrollPos (lptw->hWndText, SB_HORZ, lptw->ScrollPos.x, TRUE);
		      UpdateWindow (lptw->hWndText);
		    }
		  UpdateMark (lptw, pt);
		  GetCursorPos (&pt);
		  ScreenToClient (hwnd, &pt);
		}
	      while ((nYinc || nXinc) && !PtInRect (&rect, pt) &&
		     (GetAsyncKeyState (VK_LBUTTON) < 0));
	    }
	
	}
      break;
    case WM_CHAR:
    {
	  	long count=0;
    	
    	
		count = lptw->KeyBufIn - lptw->KeyBufOut;
		if (count < 0) count = count+ lptw->KeyBufSize;
		if (count < (long)lptw->KeyBufSize-2) 
			{
				if (wParam==10) wParam=13;
				*lptw->KeyBufIn++ = wParam;
				
				if (lptw->KeyBufIn - lptw->KeyBuf >= (signed)lptw->KeyBufSize)
				lptw->KeyBufIn = lptw->KeyBuf;	/* wrap around */
				
			}
		
    }
      return (0);
    case WM_SETTEXT :  
    	{
    		
   		CreateThreadPaste((char *)lParam); 
     	}
      return (0);
      
    case WM_COMMAND:
      if (LOWORD (wParam) < NUMMENU) SendMacro (lptw, LOWORD (wParam));
      else
	switch (LOWORD (wParam))
	  {
	  case M_COPY_CLIP:
	    TextCopyClip (lptw);
	    return 0;
	  case M_PASTE:
	    {
	    	PasteFunction(lptw,FALSE);
	      return 0;
	    }
	   case M_SPECIALPASTE:
	    {
	    	PasteFunction(lptw,TRUE);
	      return 0;
	    }
	  case M_CHOOSE_FONT:
	    TextSelectFont (lptw);
	    return 0;
	  case M_SYSCOLORS:
	    lptw->bSysColors = !lptw->bSysColors;
	    if (lptw->bSysColors)
	      CheckMenuItem (lptw->hPopMenu, M_SYSCOLORS, MF_BYCOMMAND | MF_CHECKED);
	    else
	      CheckMenuItem (lptw->hPopMenu, M_SYSCOLORS, MF_BYCOMMAND | MF_UNCHECKED);
	    SendMessage (hwnd, WM_SYSCOLORCHANGE, (WPARAM) 0, (LPARAM) 0);
	    InvalidateRect (hwnd, (LPRECT) NULL, 1);
	    UpdateWindow (hwnd);
	    return 0;
	  case M_WRITEINI:
	    WriteTextIni (lptw);
	    return 0;
	  case M_ABOUT:
	  
	    AboutBox (hwnd, lptw->AboutText);
	    
	    return 0;
	    
	  case M_HELPON:
	  {
        SaveCurrentLine(TRUE);
	  	HelpOn(lptw);
	  }
	  return 0;
	  
	  case M_PRINTSELECTION:
	  {
		 TextCopyClip (lptw);
		 if (lptw->lpmw->CodeLanguage == 0)
			{
				PrintSelection(lptw,"Scilab Command Window");
			}
		else
			{
				PrintSelection(lptw,"Console Scilab");
			}
	  }
	   return 0;
	  
	  case M_OPENSELECTION:
	  {
		SaveCurrentLine(TRUE);
	  	OpenSelection(lptw);
	  }
	  return 0;
	  
	  case M_CUT:
	  {
	  	CutSelection(lptw);
	  }
	  return 0;
	  
	  case M_EVALSELECTION:
	  {
	  	EvaluateSelection(lptw);
	  }
	  return 0;

	  case M_SELECT_ALL: 
	  {
		  SelectAll(lptw,TRUE);
	  }
	  return 0;

	  }
      return (0);
    case WM_SYSCOLORCHANGE:
      DeleteBrush (lptw->hbrBackground);
      lptw->hbrBackground = CreateSolidBrush (GetSysColor (COLOR_WINDOW));
      return (0);
    case WM_ERASEBKGND:
      return (1);		/* we will erase it ourselves */
    case WM_PAINT:
      {
	POINT source, width, dest;
	POINT MarkBegin, MarkEnd;
		
	if (TextWhereY(lptw)+50 > lptw->ScreenSize.y) ResizeScreenBuffer(lptw);

	
	hdc = BeginPaint (hwnd, &ps);
	{
		HBRUSH hbrush;
		GetClientRect (hwnd, &rect);
		hbrush = CreateSolidBrush (GetSysColor (COLOR_WINDOW));
		FillRect (hdc, &rect, hbrush);
		DeleteBrush (hbrush);
	}
	if (ps.fErase)  FillRect (hdc, &ps.rcPaint, lptw->hbrBackground);


	SelectFont (hdc, lptw->hfont);
	SetMapMode (hdc, MM_TEXT);
	SetBkMode (hdc, OPAQUE);
	GetClientRect (hwnd, &rect);
	source.x = (rect.left + lptw->ScrollPos.x) / lptw->CharSize.x;	/* source */
	source.y = (rect.top + lptw->ScrollPos.y) / lptw->CharSize.y;
	dest.x = source.x * lptw->CharSize.x - lptw->ScrollPos.x;	/* destination */
	dest.y = source.y * lptw->CharSize.y - lptw->ScrollPos.y;
	width.x = ((rect.right + lptw->ScrollPos.x + lptw->CharSize.x - 1) / lptw->CharSize.x) - source.x;	/* width */
	width.y = ((rect.bottom + lptw->ScrollPos.y + lptw->CharSize.y - 1) / lptw->CharSize.y) - source.y;
	if (source.x < 0)
	  source.x = 0;
	if (source.y < 0)
	  source.y = 0;
	if (source.x + width.x > lptw->ScreenSize.x)
	  width.x = lptw->ScreenSize.x - source.x;
	if (source.y + width.y > lptw->ScreenSize.y)
	  width.y = lptw->ScreenSize.y - source.y;
	/* ensure begin mark is before end mark */
	if ((lptw->ScreenSize.x * lptw->MarkBegin.y + lptw->MarkBegin.x) >
	    (lptw->ScreenSize.x * lptw->MarkEnd.y + lptw->MarkEnd.x))
	  {
	    MarkBegin.x = lptw->MarkEnd.x;
	    MarkBegin.y = lptw->MarkEnd.y;
	    MarkEnd.x = lptw->MarkBegin.x;
	    MarkEnd.y = lptw->MarkBegin.y;
	  }
	else
	  {
	    MarkBegin.x = lptw->MarkBegin.x;
	    MarkBegin.y = lptw->MarkBegin.y;
	    MarkEnd.x = lptw->MarkEnd.x;
	    MarkEnd.y = lptw->MarkEnd.y;
	  }
	/* for each line */
	while (width.y > 0)
	  {
	    if ((source.y >= MarkBegin.y) && (source.y <= MarkEnd.y))
	      {
		int start, end;
		int count, offset;
		if (source.y == MarkBegin.y)
		  start = MarkBegin.x;
		else
		  start = 0;
		if (source.y == MarkEnd.y)
		  end = MarkEnd.x;
		else
		  end = lptw->ScreenSize.x;
		/* do stuff before marked text */
		offset = 0;
		count = start - source.x;
		if (count > 0)
		  DoLine (lptw, hdc, dest.x, dest.y,
			  source.y * lptw->ScreenSize.x + source.x, count);
		/* then the marked text */
		offset += count;
		count = end - start;
		if ((count > 0) && (offset < width.x))
		  {
			SetTextColor (hdc, GetSysColor (COLOR_HIGHLIGHTTEXT));
			SetBkColor (hdc, GetSysColor (COLOR_HIGHLIGHT));
		 //   if (lptw->bSysColors)
		 //     {
			//SetTextColor (hdc, GetSysColor (COLOR_HIGHLIGHTTEXT));
			//SetBkColor (hdc, GetSysColor (COLOR_HIGHLIGHT));
		 //     }
		 //   else
		 //     {
			//SetTextColor (hdc, MARKFORE);
			//SetBkColor (hdc, MARKBACK);
		 //     }
		    TextOut (hdc, dest.x + lptw->CharSize.x * offset, dest.y,
			     (LPSTR) (lptw->ScreenBuffer + source.y * lptw->ScreenSize.x
				      + source.x + offset), count);
		  }
		/* then stuff after marked text */
		offset += count;
		count = width.x + source.x - end;
		if ((count > 0) && (offset < width.x))
		  DoLine (lptw, hdc, dest.x + lptw->CharSize.x * offset, dest.y,
		  source.y * lptw->ScreenSize.x + source.x + offset, count);
	      }
	    else
	      {
		DoLine (lptw, hdc, dest.x, dest.y,
			source.y * lptw->ScreenSize.x + source.x, width.x);
	      }
	    dest.y += lptw->CharSize.y;
	    source.y++;
	    width.y--;
	  }
	EndPaint (hwnd, &ps);
	

	return 0;
      }
    case WM_CREATE:
	{
		TEXTMETRIC tm;
		lptw = ((CREATESTRUCT *) lParam)->lpCreateParams;
		SetWindowLong (hwnd, 0, (LONG) lptw);
		lptw->hWndText = hwnd;
	  
		TextMakeFont (lptw);
		hdc = (HDC)TryToGetDC (hwnd);
		SelectFont (hdc, lptw->hfont);
		GetTextMetrics (hdc, (LPTEXTMETRIC) & tm);
		lptw->CharSize.y = tm.tmHeight;
		lptw->CharSize.x = tm.tmAveCharWidth;
		lptw->CharAscent = tm.tmAscent;
		ReleaseDC (hwnd, hdc);
	}
      break;
    case WM_DESTROY:
      //DeleteBrush (lptw->hbrBackground);
      /* Modification Allan CORNET 05/08/03 */
      DeleteObject(lptw->hbrBackground);
      break;
    }
  return DefWindowProc (hwnd, message, wParam, lParam);
}
/*-----------------------------------------------------------------------------------*/
/****************************************
 * replacement stdio routines 
 ****************************************/
/* TRUE if key hit, FALSE if no key */
EXPORT int WINAPI TextKBHit (LPTW lptw)
{
  return (lptw->KeyBufIn != lptw->KeyBufOut);
}
/*-----------------------------------------------------------------------------------*/
/* get character from keyboard, no echo */
/* need to add extended codes */
/* Modification Allan CORNET */
/* 08/08/03 */
/* Si une zone de texte est selectionné Ctrl+C --> Copy to Clipboard */
/* sinon --> Envoye du signal Ctrl+c */
EXPORT int WINAPI TextGetCh (LPTW lptw)
{
  int ch;
  
  lptw->bGetCh = TRUE;
  
  TextToCursor (lptw);
  
  if (lptw->bFocus)
    {
      SetCaretPos (lptw->CursorPos.x * lptw->CharSize.x - lptw->ScrollPos.x,
		   lptw->CursorPos.y * lptw->CharSize.y + lptw->CharAscent
		   - lptw->CaretHeight - lptw->ScrollPos.y);
      ShowCaret (lptw->hWndText);
    }

  /** 
    Here we must wait for caracters while 
    taking into account events. 
    TextMessage cannot be called directly in order not to use 
    99% of the cpu time doing nothing. 
    we can use TextMessage with a Sleep or TextMessage2 
    without a sleep but the second option does not work 
    properly. 
  **/

  do 
  {
    if ( (!GetThreadPasteRunning()) && (!WriteInKeyBuf) ) Sleep(1); 
    TextMessage();
  } while (!TextKBHit(lptw));

/*   
  while (!TextKBHit (lptw))
    {
      TextMessage2 ();
    }
*/  
  
  ch = *lptw->KeyBufOut++;
  
  //if (ch == '\r') ch = '\n';
  
  
  /* Interception de Ctrl+C */
  if (ch == (char)3) 
  {
  	if ( HasAZoneTextSelected(lptw) )
	  {
	  	TextCopyClip(lptw);
	  	SendCTRLandAKey(CTRLE);
	  	return 1;
	  	
	  }
  }
  
  if (lptw->KeyBufOut - lptw->KeyBuf >= (int) lptw->KeyBufSize)   lptw->KeyBufOut = lptw->KeyBuf;	/* wrap around */

  if (lptw->bFocus)  HideCaret (lptw->hWndText);
  
  lptw->bGetCh = FALSE;
  return ch;
  
}
/*-----------------------------------------------------------------------------------*/
/*** checks if a CtrlC was typed on th keyboard **/
/* Modification Allan CORNET */
/* 08/08/03 */
/* Si une zone de texte est selectionné Ctrl+C --> Copy to Clipboard */
/* sinon --> Envoye du signal Ctrl+c */
int CtrlCHit (LPTW lptw)
{
  int ch;
  
  if (TextKBHit (lptw))
    {
      ch = *lptw->KeyBufOut++;
      if (lptw->KeyBufOut - lptw->KeyBuf >= (int) lptw->KeyBufSize)
	lptw->KeyBufOut = lptw->KeyBuf;		/* wrap around */
      if (ch == 3)
	{
	
	  if ( HasAZoneTextSelected(lptw) )
	  {
	  	TextCopyClip(lptw);
	  }
	  else
	  {
	  	SignalCtrC ();
	  }
	  
	  
	  return (1);
	}
    }
  return (0);
}
/*-----------------------------------------------------------------------------------*/
/* get character from keyboard, with echo */
EXPORT int WINAPI TextGetChE (LPTW lptw)
{
  int ch;
  ch = TextGetCh (lptw);
  TextPutCh (lptw, (BYTE) ch);
  return ch;
}
/*-----------------------------------------------------------------------------------*/
EXPORT LPSTR WINAPI TextGetS (LPTW lptw, LPSTR str, unsigned int size)
{
  LPSTR next = str;
  
  while (--size > 0)
    {
      switch (*next = TextGetChE (lptw))
	{
	case EOF:
	  *next = '\0';
	  if (next == str)
	    return (char *) NULL;
	  return str;
	case '\n':
	  *(next + 1) = '\0';
	  return str;
	case 0x08:
	case 0x7f:
	  if (next > str)
	    --next;
	  break;
	default:
	  ++next;
	}
    }
  *next = '\0';
  return str;
}
/*-----------------------------------------------------------------------------------*/
EXPORT int WINAPI TextPutS (LPTW lptw, LPSTR str)
{
  TextPutStr (lptw, str);
  return str[strlen (str) - 1];
}
/*-----------------------------------------------------------------------------------*/
/****************************************
 * routines added for elvis 
 ****************************************/
EXPORT void WINAPI TextGotoXY (LPTW lptw, int x, int y)
{
  lptw->CursorPos.x = x;
  lptw->CursorPos.y = y;
}
/*-----------------------------------------------------------------------------------*/
EXPORT int WINAPI TextWhereX (LPTW lptw)
{
  return lptw->CursorPos.x;
}
/*-----------------------------------------------------------------------------------*/
EXPORT int WINAPI TextWhereY (LPTW lptw)
{
  return lptw->CursorPos.y;
}
/*-----------------------------------------------------------------------------------*/
EXPORT void WINAPI TextCursorHeight (LPTW lptw, int height)
{
  lptw->CaretHeight = height;
  if (lptw->bFocus)
    CreateCaret (lptw->hWndText, 0, lptw->CharSize.x, 2 + lptw->CaretHeight);
}
/*-----------------------------------------------------------------------------------*/
EXPORT void WINAPI TextClearEOL (LPTW lptw)
{
HDC hdc;
int xpos, ypos;
int from, len;
POINT pt;
	pt.x = pt.y = 0;
	ClearMark(lptw, pt);
	from = lptw->CursorPos.y*lptw->ScreenSize.x + lptw->CursorPos.x;
	len = lptw->ScreenSize.x-lptw->CursorPos.x;
	_fmemset(lptw->ScreenBuffer + from, ' ', len);
	_fmemset(lptw->AttrBuffer + from, NOTEXT, len);
	xpos = lptw->CursorPos.x*lptw->CharSize.x - lptw->ScrollPos.x;
	ypos = lptw->CursorPos.y*lptw->CharSize.y - lptw->ScrollPos.y;
	hdc = GetDC(lptw->hWndText);

	SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
	SetBkColor(hdc, GetSysColor(COLOR_WINDOW));

	//if (lptw->bSysColors) {
	//    SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
	//    SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
	//}
	//else {
	//    SetTextColor(hdc, TextFore(lptw->Attr));
	//    SetBkColor(hdc, TextBack(lptw->Attr));
	//}
	SelectObject(hdc, (lptw->hfont));
	TextOut(hdc,xpos,ypos,
		(LPSTR)(lptw->ScreenBuffer + lptw->CursorPos.y*lptw->ScreenSize.x + 
		lptw->CursorPos.x), lptw->ScreenSize.x-lptw->CursorPos.x);
	(void)ReleaseDC(lptw->hWndText,hdc);
}


/*-----------------------------------------------------------------------------------*/
EXPORT void WINAPI TextClearEOS (LPTW lptw)
{
  RECT rect;
  int from, len;
  POINT pt;
  pt.x = pt.y = 0;
  ClearMark (lptw, pt);
  from = lptw->CursorPos.y * lptw->ScreenSize.x + lptw->CursorPos.x;
  len = lptw->ScreenSize.x - lptw->CursorPos.x +
    (lptw->ScreenSize.y - lptw->CursorPos.y - 1) * lptw->ScreenSize.x;
  _fmemset(lptw->ScreenBuffer + from, ' ', len);
  _fmemset(lptw->AttrBuffer + from, NOTEXT, len);
  GetClientRect (lptw->hWndText, (LPRECT) & rect);
  InvalidateRect (lptw->hWndText, (LPRECT) & rect, 1);
  UpdateWindow (lptw->hWndText);
}
/*-----------------------------------------------------------------------------------*/
EXPORT void WINAPI TextInsertLine (LPTW lptw)
{
  RECT rect;
  int from, to, len;
  POINT pt;
  pt.x = pt.y = 0;
  ClearMark (lptw, pt);
  from = lptw->CursorPos.y * lptw->ScreenSize.x,
    to = (lptw->CursorPos.y + 1) * lptw->ScreenSize.x;
  len = (lptw->ScreenSize.y - lptw->CursorPos.y - 1) * lptw->ScreenSize.x;
  _fmemmove(lptw->ScreenBuffer + to, lptw->ScreenBuffer + from, len);
  _fmemmove(lptw->AttrBuffer + to, lptw->AttrBuffer + from, len);
  _fmemset(lptw->ScreenBuffer + from, ' ', lptw->ScreenSize.x);
  _fmemset(lptw->AttrBuffer + from, NOTEXT, lptw->ScreenSize.x);
  GetClientRect (lptw->hWndText, (LPRECT) & rect);
  InvalidateRect (lptw->hWndText, (LPRECT) & rect, 1);
  UpdateWindow (lptw->hWndText);
  if (lptw->CursorFlag)    TextToCursor (lptw);
}
/*-----------------------------------------------------------------------------------*/
EXPORT void WINAPI TextDeleteLine (LPTW lptw)
{
  RECT rect;
  int from, to, len;
  POINT pt;
  pt.x = pt.y = 0;
  ClearMark (lptw, pt);
  to = lptw->CursorPos.y * lptw->ScreenSize.x,
    from = (lptw->CursorPos.y + 1) * lptw->ScreenSize.x;
  len = (lptw->ScreenSize.y - lptw->CursorPos.y - 1) * lptw->ScreenSize.x;
  _fmemmove(lptw->ScreenBuffer + to, lptw->ScreenBuffer + from, len);
  _fmemmove(lptw->AttrBuffer + to, lptw->AttrBuffer + from, len);
  from = lptw->ScreenSize.x * (lptw->ScreenSize.y - 1);
  _fmemset(lptw->ScreenBuffer + from, ' ', lptw->ScreenSize.x);
  _fmemset(lptw->AttrBuffer + from, NOTEXT, lptw->ScreenSize.x);
  GetClientRect (lptw->hWndText, (LPRECT) & rect);
  InvalidateRect (lptw->hWndText, (LPRECT) & rect, 1);
  UpdateWindow (lptw->hWndText);
  if (lptw->CursorFlag) TextToCursor (lptw);
}
/*-----------------------------------------------------------------------------------*/
EXPORT void WINAPI TextScrollReverse (LPTW lptw)
{
  RECT rect;
  int len = lptw->ScreenSize.x * (lptw->ScreenSize.y - 1);
  _fmemmove(lptw->ScreenBuffer+lptw->ScreenSize.x, lptw->ScreenBuffer, len);
  _fmemset(lptw->ScreenBuffer, ' ', lptw->ScreenSize.x);
  _fmemmove(lptw->AttrBuffer+lptw->ScreenSize.x, lptw->AttrBuffer, len);
  _fmemset(lptw->AttrBuffer, NOTEXT, lptw->ScreenSize.x);
  if (lptw->CursorPos.y)
    lptw->CursorPos.y--;
  ScrollWindow (lptw->hWndText, 0, +lptw->CharSize.y, NULL, NULL);
  GetClientRect (lptw->hWndText, (LPRECT) & rect);
  rect.top = lptw->ScreenSize.y * lptw->CharSize.y;
  if (rect.top < rect.bottom) InvalidateRect (lptw->hWndText, (LPRECT) & rect, 1);
  lptw->MarkBegin.y++;
  lptw->MarkEnd.y++;
  LimitMark (lptw, &lptw->MarkBegin);
  LimitMark (lptw, &lptw->MarkEnd);
  UpdateWindow (lptw->hWndText);
}
/*-----------------------------------------------------------------------------------*/
EXPORT void WINAPI TextAttr (LPTW lptw, BYTE attr)
{
  lptw->Attr = attr;
}
/*-----------------------------------------------------------------------------------*/
/* About Box */
/* Allan CORNET le 18/07/03*/
/* About Box avec lien sur licence */
/*-----------------------------------------------------------------------------------*/

EXPORT BOOL CALLBACK AboutDlgProc (HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam)
	{
	switch (wMsg)
		{
		case WM_INITDIALOG:    

			{
			char buffer[MAX_PATH];
			int cpubuild=_M_IX86;
			wsprintf(buffer,"%s %s","Copyright ® ",DEFAULT_MES);
			SetDlgItemText(hDlg,IDC_VERSION_SPLASH,VERSION);
			SetDlgItemText(hDlg,IDC_COPYRIGHT_SPLASH,buffer);
			wsprintf(buffer,"%s %s",__DATE__,__TIME__);
			SetDlgItemText(hDlg,IDC_BUILD,buffer);
			#if __MAKEFILEVC__
				strcpy(buffer,"Makefile VC ");
			#else
				#if __ABSC__
				strcpy(buffer,"Makefile ABSOFT ");
				#else
					#if _DEBUG
					strcpy(buffer,"Debug ");
					strcat(buffer,"Blend");
					#else
					strcpy(buffer,"Release ");
					switch(cpubuild)
					{
						case 500: // Pentium
							strcat(buffer,"Pentium");
						break;
						case 600: // Pentium Pro
							strcat(buffer,"Pentium II and more");
						break;
						case 400: // 486
							strcat(buffer,"486");
						break;
						case 300: // 386
							strcat(buffer,"386");
						break;
					}
					#endif
				#endif
			#endif
			SetDlgItemText(hDlg,IDC_COMPILMODE,buffer);
			}
		return TRUE;

		case WM_DRAWITEM:
			{
			}
		return FALSE;

		case WM_COMMAND:
			{
			switch (LOWORD (wParam))
				{
				case IDC_LICENCE:
					{
					#define LICENCEFR "Licence.txt"
					#define LICENSEENG "License.txt"
					#define	PATHBIN "bin\\"

					char Chemin[MAX_PATH];
					char szModuleName[MAX_PATH];
					LPSTR tail;
					HINSTANCE hInstance=NULL;
					int error=0;
					extern char ScilexWindowName[MAX_PATH];
					LPTW lptw;
					lptw = (LPTW) GetWindowLong (FindWindow(NULL,ScilexWindowName), 0);

					hInstance=(HINSTANCE) GetModuleHandle(NULL);   		
					GetModuleFileName (hInstance,szModuleName, MAX_PATH);

					if ((tail = strrchr (szModuleName, '\\')) != (LPSTR) NULL)
						{
						tail++;
						*tail = '\0';
						}
					szModuleName[strlen(szModuleName)-strlen(PATHBIN)]='\0';
					strcpy(Chemin,szModuleName);

					switch (lptw->lpmw->CodeLanguage)
						{
						case 1: /* French */
							{
							strcat(Chemin,LICENCEFR);
							}
						break;

						default: case 0: /*English */
							{
							strcat(Chemin,LICENSEENG);
							}
						break;
						}

					error =(int)ShellExecute(NULL, "open", Chemin, NULL, NULL, SW_SHOWNORMAL);
					if (error<= 32) 
						{
						switch (lptw->lpmw->CodeLanguage)
							{
							case 1: /* French */
								{
								MessageBox(NULL,"Impossible d'ouvrir le fichier Licence.txt","Attention",MB_ICONWARNING);
								}
							break;

							default: case 0: /*English */
								{
								MessageBox(NULL,"Couldn't Open License.txt","Warning",MB_ICONWARNING);
								}
							break;
							}
						}
					}
				break;
				case IDOK:
					EndDialog (hDlg, LOWORD (wParam));
					return TRUE;

				}
			}	
		break;

		case WM_DESTROY :
			{
			EndDialog (hDlg, LOWORD (wParam));
			return TRUE;
			}
		break;

		case WM_CLOSE :
			{
			EndDialog (hDlg, LOWORD (wParam));
			return TRUE;
			}
		break;
		}
	return FALSE;
	}
/*-----------------------------------------------------------------------------------*/
EXPORT void WINAPI AboutBox (HWND hwnd, LPSTR str)
{
  
  HWND hdlg;
  hdlg = CreateDialog(hdllInstance, "AboutDlgBox", hwnd,AboutDlgProc);
  ShowWindow(	hdlg,SW_SHOW );
}

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
void HelpOn(LPTW lptw)
/* Affiche l'aide concernant la zone de texte selectionnée */
{
	HDC hdc;
	HGLOBAL hGMem;
	
	TEXTMETRIC tm;
	UINT type;
	LPSTR lpMem; /* Pointeur sur la chaine du clipboard */
	char *MessagePaste=NULL;
	char Command[MAX_PATH];
		
	strcpy(Command,"");
	
	
	/* Copie dans le presse papier */
	/* La zone sélectionnée */
	TextCopyClip (lptw);
		
	/*Récupere ce qu'il y a dans le presse papier*/
	
	hdc = GetDC (lptw->hWndText);
	SelectFont (hdc, lptw->hfont);
	GetTextMetrics (hdc, (TEXTMETRIC *) & tm);
	if (tm.tmCharSet == OEM_CHARSET) type = CF_OEMTEXT;
	else type = CF_TEXT;
	ReleaseDC (lptw->hWndText, hdc);
	/* now get it from clipboard */
	OpenClipboard (lptw->hWndText);
	hGMem = GetClipboardData (type);
	if (hGMem)
	{
		int i=0;
		int l=0;
		
		lpMem= GlobalLock (hGMem);
		l=strlen(lpMem);
		MessagePaste=(char*)malloc( (l+1)*sizeof(char));
		strcpy(MessagePaste,lpMem);
		MessagePaste[l]='\0';
		GlobalUnlock (hGMem);
		
		/* On enleve les prompts */
		/*CleanPromptFromText(MessagePaste);*/
		
		/* On enleve  : et retour chariot */
		l=strlen(MessagePaste);
		for (i=0;i<l;i++)
		{
			if (MessagePaste[i]==':') MessagePaste[i]=' ';
			if (MessagePaste[i]=='\n') MessagePaste[i]=' ';
			if (MessagePaste[i]=='\r') MessagePaste[i]=' ';
		}	
		

	}
	CloseClipboard ();
	
	
	if (MessagePaste)
	{
		
		int minlen=strlen("help \"\"");

		if ( strlen(MessagePaste)+minlen > MAX_PATH)
		{
			char MessageTMP[MAX_PATH];
			strncpy(MessageTMP,MessagePaste,MAX_PATH-minlen);
			wsprintf(Command,"help \"%s\"",MessageTMP);
		}
		else
		{
			wsprintf(Command,"help \"%s\"",MessagePaste);
		}
		
		if (strcmp (Command,"help \"\"")!=0 ) StoreCommand1 (Command,0);
		free(MessagePaste);
	}
	
}
/*-----------------------------------------------------------------------------------*/
/* A faire ulterieurement */
void CutSelection(LPTW lptw)
{
}
/*-----------------------------------------------------------------------------------*/
void OnRightClickMenu(LPTW lptw)
/* Cree le menu contextuel */
{
  /* Modification Allan CORNET */
   
  if (lptw->hPopMenu) DestroyMenu(lptw->hPopMenu);    	
  lptw->hPopMenu = CreatePopupMenu ();
  
  switch (lptw->lpmw->CodeLanguage)
  {
  	case 1: /* French */
  	{
  		/*AppendMenu (lptw->hPopMenu, MF_STRING|MF_GRAYED, M_CUT, "&Couper	Ctrl+X");*/
		AppendMenu (lptw->hPopMenu, MF_STRING|MF_ENABLED, M_SELECT_ALL, "Sélection&ner Tout");
		AppendMenu (lptw->hPopMenu, MF_SEPARATOR, 0, NULL);
  		AppendMenu (lptw->hPopMenu, MF_STRING|MF_GRAYED, M_COPY_CLIP, "Co&pier	Ctrl+C");
  		AppendMenu (lptw->hPopMenu, MF_STRING|MF_GRAYED, M_PASTE, "C&oller	Ctrl+V");
  		//AppendMenu (lptw->hPopMenu, MF_STRING|MF_GRAYED, M_SPECIALPASTE, "Coller &Special	Shft+V");
  		AppendMenu (lptw->hPopMenu, MF_STRING|MF_GRAYED,M_PRINTSELECTION, "&Imprimer Selection...");
  		AppendMenu (lptw->hPopMenu, MF_SEPARATOR, 0, NULL);
  		AppendMenu (lptw->hPopMenu, MF_STRING|MF_GRAYED,M_EVALSELECTION, "&Evaluer Selection");
  		AppendMenu (lptw->hPopMenu, MF_STRING|MF_GRAYED,M_OPENSELECTION, "&Ouvrir Selection");
  		AppendMenu (lptw->hPopMenu, MF_STRING|MF_GRAYED,M_HELPON, "&Aide sur");
  	}
  	break;
  	default: case 0: /*English */
  	{
  		/*AppendMenu (lptw->hPopMenu, MF_STRING|MF_GRAYED, M_CUT, "C&ut	Ctrl+X");*/
		AppendMenu (lptw->hPopMenu, MF_STRING|MF_ENABLED, M_SELECT_ALL, "Select &All");
		AppendMenu (lptw->hPopMenu, MF_SEPARATOR, 0, NULL);
  		AppendMenu (lptw->hPopMenu, MF_STRING|MF_GRAYED, M_COPY_CLIP, "&Copy	Ctrl+C");
  		AppendMenu (lptw->hPopMenu, MF_STRING|MF_GRAYED, M_PASTE, "&Paste	Ctrl+V");
  		//AppendMenu (lptw->hPopMenu, MF_STRING|MF_GRAYED, M_SPECIALPASTE, "&Special Paste	Shft+V");
  		AppendMenu (lptw->hPopMenu, MF_STRING|MF_GRAYED,M_PRINTSELECTION, "&Print Selection...");
  		AppendMenu (lptw->hPopMenu, MF_SEPARATOR, 0, NULL);
  		AppendMenu (lptw->hPopMenu, MF_STRING|MF_GRAYED,M_EVALSELECTION, "&Evaluate Selection");
  		AppendMenu (lptw->hPopMenu, MF_STRING|MF_GRAYED,M_OPENSELECTION, "&Open Selection");
  		AppendMenu (lptw->hPopMenu, MF_STRING|MF_GRAYED,M_HELPON, "&Help on");

  	}
  	break;
  }
	SendMessage (lptw->hWndText, WM_LBUTTONUP,0, 0);  
  

	
}
/*-----------------------------------------------------------------------------------*/
void EvaluateSelection(LPTW lptw)
{
	HDC hdc;
	HGLOBAL hGMem;
	
	TEXTMETRIC tm;
	UINT type;
	LPSTR lpMem; /* Pointeur sur la chaine du clipboard */
	char *MessagePaste=NULL;
	
	
	/* Copie dans le presse papier */
	/* La zone sélectionnée */
	TextCopyClip (lptw);
		
	/*Récupere ce qu'il y a dans le presse papier*/
	
	hdc = GetDC (lptw->hWndText);
	SelectFont (hdc, lptw->hfont);
	GetTextMetrics (hdc, (TEXTMETRIC *) & tm);
	if (tm.tmCharSet == OEM_CHARSET) type = CF_OEMTEXT;
	else type = CF_TEXT;
	ReleaseDC (lptw->hWndText, hdc);
	/* now get it from clipboard */
	OpenClipboard (lptw->hWndText);
	hGMem = GetClipboardData (type);
	if (hGMem)
	{
		int i=0;
		int l=0;
		
		lpMem= GlobalLock (hGMem);
		l=strlen(lpMem);
		MessagePaste=(char*)malloc(l*sizeof(char));
		strcpy(MessagePaste,lpMem);
		GlobalUnlock (hGMem);
	}
	CloseClipboard ();
	strcat(MessagePaste,"\n");
	write_scilab_synchro(MessagePaste);	
	
}
/*-----------------------------------------------------------------------------------*/
void OpenSelection(LPTW lptw)
/* Ouvre un fichier nommé par la zone de texte selectionnée */
{
	HDC hdc;
	HGLOBAL hGMem;
	
	TEXTMETRIC tm;
	UINT type;
	LPSTR lpMem; /* Pointeur sur la chaine du clipboard */
	char *MessagePaste=NULL;
	char FileName[MAX_PATH];
	char FileNameSCE[MAX_PATH];	
	char FileNameSCI[MAX_PATH];	
	char Command[MAX_PATH];
	
	
	/* Copie dans le presse papier */
	/* La zone sélectionnée */
	TextCopyClip (lptw);
		
	/*Récupere ce qu'il y a dans le presse papier*/
	
	hdc = GetDC (lptw->hWndText);
	SelectFont (hdc, lptw->hfont);
	GetTextMetrics (hdc, (TEXTMETRIC *) & tm);
	if (tm.tmCharSet == OEM_CHARSET) type = CF_OEMTEXT;
	else type = CF_TEXT;
	ReleaseDC (lptw->hWndText, hdc);
	/* now get it from clipboard */
	OpenClipboard (lptw->hWndText);
	hGMem = GetClipboardData (type);
	if (hGMem)
	{
		int i=0;
		int l=0;
		
		lpMem= GlobalLock (hGMem);
		l=strlen(lpMem);
		MessagePaste=(char*)malloc(l*sizeof(char));
		strcpy(MessagePaste,lpMem);
		GlobalUnlock (hGMem);
		

		
		
		/* On enleve et retour chariot */
		l=strlen(MessagePaste);
		for (i=0;i<l;i++)
		{
			if (MessagePaste[i]=='\n') MessagePaste[i]='\0';
			if (MessagePaste[i]=='\r') MessagePaste[i]='\0';
		}	
	}
	CloseClipboard ();
	
	strcpy(FileName,MessagePaste);	
	
	strcpy(FileNameSCI,MessagePaste);
	strcat(FileNameSCI,".sci");
	
	strcpy(FileNameSCE,MessagePaste);
	strcat(FileNameSCE,".sce");
	
	//MessageBox(NULL,FileName,FileName,MB_OK);
	
	if ( IsAFile(FileNameSCI) )
	{
		char Fichier[MAX_PATH];
		GetShortPathName(FileNameSCI,Fichier,MAX_PATH);
		ReplaceSlash(FileNameSCI,Fichier);
		wsprintf(Command,"scipad('%s')",FileNameSCI);
		
		StoreCommand1 (Command, 0);
		return ;			
	}
	
	if ( IsAFile(FileNameSCE) )
	{
		char Fichier[MAX_PATH];
		GetShortPathName(FileNameSCE,Fichier,MAX_PATH);
		ReplaceSlash(FileNameSCE,Fichier);
		wsprintf(Command,"scipad('%s')",FileNameSCE);
		StoreCommand1 (Command, 0);
		return ;			
	}
	
	if ( IsAFile(FileName) )
	{
		char Fichier[MAX_PATH];
		GetShortPathName(FileName,Fichier,MAX_PATH);
		ReplaceSlash(FileName,Fichier);
		//MessageBox(NULL,FileName,Fichier,MB_OK);
		wsprintf(Command,"scipad('%s')",FileName);
		StoreCommand1 (Command, 0);
		return ;			
	}
	else
	{
		/*cree un fichier avec l'extension SCI */
		char Message[MAX_PATH];
		
		wsprintf(Message,"File %s does not exist.\nDo you want create it ?",FileNameSCI);
		if ( MessageBox(lptw->hWndText,Message,"Scilab Editor",MB_YESNO|MB_ICONWARNING)== IDYES )
		{
			FILE *fp;
			char Fichier[MAX_PATH];
			
			fp=fopen(FileNameSCI,"wt");
			if ( fp )
			{
				fclose(fp);
				GetShortPathName(FileNameSCI,Fichier,MAX_PATH);
				ReplaceSlash(FileNameSCI,Fichier);
				wsprintf(Command,"scipad('%s')",FileNameSCI);
				StoreCommand1 (Command, 0);			
			}
			else
			{
				sciprint("\nCannot create file. It does not appear to be a valid name\n");	
				SendCTRLandAKey(CTRLL);
			}
			
		}
	}
	
	
}
/*-----------------------------------------------------------------------------------*/
BOOL HasAZoneTextSelected(LPTW lptw)
{
	BOOL ValRetour=FALSE;
	
	if  ( (lptw->ScreenSize.x * lptw->MarkBegin.y + lptw->MarkBegin.x) -
	      (lptw->ScreenSize.x * lptw->MarkEnd.y + lptw->MarkEnd.x) != 0) 
	      {ValRetour=TRUE;}
	
	return ValRetour;
}
/*-----------------------------------------------------------------------------------*/
int ClearScreenConsole _PARAMS((char *fname, unsigned long fname_len))
{
	if (Rhs == 0) /* aucun parametre On Efface tout l'ecran */
        {
		if ( IsWindowInterface() )
		{
			LPTW lptw;
			lptw = (LPTW) GetWindowLong (FindWindow(NULL,ScilexWindowName), 0);
			ClearCommandWindow(lptw,FALSE);
		}
		else
		{
			system("cls");	
		}
	}
	else /* on remonte le curseur en effacant du nombre de lignes indiqué */
	{
		if ( IsWindowInterface() )
		{
			int X=0,Y=0;
			int cx=0,cy=0;

			static int l1, m1, n1;
			int NbrLineToRemove=0;
					
			CheckRhs(1,1);
  			CheckLhs(1,1);
  			GetRhsVar(1,"i",&m1,&n1,&l1);
  			NbrLineToRemove=*istk(l1)+2;
  			LhsVar(1)=0;
	
	 		if (NbrLineToRemove>0)
	 		{
	 			 LPTW lptw;
	 			 int i = 0;
	 			 lptw = (LPTW) GetWindowLong (FindWindow(NULL,ScilexWindowName), 0);
				 
				 X=lptw->CursorPos.x;
				 Y=lptw->CursorPos.y-NbrLineToRemove;
				 if (Y>0)
				 {
				 	i=(Y+1)*lptw->ScreenSize.x;
				 			 
	 			 	_fmemset(lptw->ScreenBuffer + i, ' ', (NbrLineToRemove+1)*lptw->ScreenSize.x);
	 			 	_fmemset(lptw->AttrBuffer + i, NOTEXT, (NbrLineToRemove+1)*lptw->ScreenSize.x);
	 			 
	 				TextGotoXY (lptw, X,Y);
	 				InvalidateRect (lptw->hWndText, NULL, TRUE);
				 }
				 else
				 {
				 	sciprint("\n Not in screen \n");
				 }
			}	
			else
	 		{
	 			sciprint("Error %d invalid number\n",NbrLineToRemove);
	 		}
		}
		else
		{
			sciprint("\n Not in console mode \n");
		}
	}
return 0;	
}
/*-----------------------------------------------------------------------------------*/
/* Efface la fenetre de commandes */
void ClearCommandWindow(LPTW lptw,BOOL Clearfirstline)
{
	/* Deselectionne une eventuelle zone de texte */
	if ( HasAZoneTextSelected(lptw) )
	{
		POINT pt;
		pt.x = 0;
		pt.y = 0;
		pt.x = (pt.x + lptw->ScrollPos.x)/lptw->CharSize.x;
		pt.y = (pt.y + lptw->ScrollPos.y)/lptw->CharSize.y;
		ClearMark(lptw, pt);
		lptw->Marking = FALSE;
	}
	if (Clearfirstline) SendCTRLandAKey(CTRLU); /*On efface la ligne courante */
	InitScreenBuffer(lptw);
	
	/* Initialisation su buffer Ecran */
	_fmemset (lptw->ScreenBuffer, ' ', lptw->ScreenSize.x * lptw->ScreenSize.y);
	
	/* Repositionnement de la ligne de commande */
	TextGotoXY (lptw, 0, 0);
	/* Remise à zero des scrollbars */
	lptw->ScrollPos.x=0;
	lptw->ScrollPos.y=0;
	SetScrollPos (lptw->hWndText, SB_VERT, lptw->ScrollPos.y, TRUE);
      	SetScrollPos (lptw->hWndText, SB_HORZ, lptw->ScrollPos.x, TRUE);
      	/* Reactualisation de la fenetre */
      	UpdateWindow (lptw->hWndText);
	InvalidateRect (lptw->hWndText, NULL, TRUE);
}
/*-----------------------------------------------------------------------------------*/
void InitScreenBuffer(LPTW lptw)
{
	HGLOBAL hglobal;
	
	/* Destruction du Screenbuffer */
	hglobal = GlobalHandle (lptw->ScreenBuffer);
  	if (hglobal)
    	{
      		GlobalUnlock (hglobal);
      		GlobalFree (hglobal);
    	}
   	
    	hglobal = GlobalHandle (lptw->AttrBuffer);
  	if (hglobal)
    	{
      		GlobalUnlock (hglobal);
      		GlobalFree (hglobal);
    	}
	
        /* Réinitialisation des dimensions */
	/*lptw->ScreenSize.x = ScrollSize.x; / * 80 par défaut*/
  	/*lptw->ScreenSize.y = ScrollSize.y; / * 360 par défaut*/
	
     	lptw->ScrollMax.y = max (0, lptw->CharSize.y * lptw->ScreenSize.y - lptw->ClientSize.y);
      	lptw->ScrollPos.y = min (lptw->ScrollPos.y, lptw->ScrollMax.y);

        /* Allocation Memoire ScreenBuffer */	
	hglobal = GlobalAlloc (GHND, lptw->ScreenSize.x * lptw->ScreenSize.y);

    	lptw->ScreenBuffer = (BYTE FAR *) GlobalLock (hglobal);
  	if (lptw->ScreenBuffer == (BYTE FAR *) NULL)
    	{
      		MessageBox ((HWND) NULL, szNoMemory, (LPSTR) NULL, MB_ICONHAND | MB_SYSTEMMODAL);
      		exit(1);
    	}
  	_fmemset (lptw->ScreenBuffer, ' ', lptw->ScreenSize.x * lptw->ScreenSize.y);
		
	hglobal = GlobalAlloc (GHND, lptw->ScreenSize.x * lptw->ScreenSize.y);
  	lptw->AttrBuffer = (BYTE FAR *) GlobalLock (hglobal);
  	if (lptw->AttrBuffer == (BYTE FAR *) NULL)
    	{
      		MessageBox ((HWND) NULL, szNoMemory, (LPSTR) NULL, MB_ICONHAND | MB_SYSTEMMODAL);
      		exit(1);
    	}
  	_fmemset (lptw->AttrBuffer, NOTEXT, lptw->ScreenSize.x * lptw->ScreenSize.y);
   	
   	SetScrollRange (lptw->hWndText, SB_VERT, 0, lptw->ScrollMax.y, FALSE);
   	SetScrollPos (lptw->hWndText, SB_VERT, lptw->ScrollPos.y, TRUE);
      	UpdateWindow (lptw->hWndText);
}
/*-----------------------------------------------------------------------------------*/
void ForceToActiveWindowParent(void)
{
	LPTW lptw;
	lptw = (LPTW) GetWindowLong (FindWindow(NULL,ScilexWindowName), 0);
	
	while (GetForegroundWindow()!= lptw->hWndParent)
	{
		SetForegroundWindow(lptw->hWndParent);
		SetActiveWindow(lptw->hWndParent);
		Sleep(1);
	}
}
/*-----------------------------------------------------------------------------------*/
/* Enleve les prompts de la chaine entrée */
/*
   -->
   -%d->
   >>
*/
void CleanPromptFromText(char *Text)
{
	char *CleanText=NULL;
	
	char prompt[6];

	int LenText=0;
	int i=0;
	
	LenText=strlen(Text)+1;
	CleanText=(char*)malloc(LenText*sizeof(char));
	strcpy(CleanText,Text);
	
	strcpy(prompt,"-->");
	ReplacePrompt(CleanText,prompt);
	
	strcpy(prompt,">>");
	ReplacePrompt(CleanText,prompt);
	

	for (i=1;i<127;i++)
	{
		char TmpPrompt[6];
		wsprintf(TmpPrompt,"-%d->",i);
		ReplacePrompt(CleanText,TmpPrompt);
		strcpy(TmpPrompt," ");
	}
	
	strcpy(Text,CleanText);
	
	free(CleanText);
}
/*-----------------------------------------------------------------------------------*/
int ReplacePrompt(char *Text,char *prompt)
/* retourne TRUE si Text a été modifié */
{
	int Retour=FALSE;
	int l=0;
	char *TextTMP=NULL;
	char *LocalPrompt=NULL;
	
	char *OccurenceDebutPrompt=NULL;
	
	LocalPrompt=(char*)malloc((strlen(prompt)+1)*sizeof(char));
	TextTMP=(char*)malloc((strlen(Text)+1)*sizeof(char));
	
	strcpy(TextTMP,Text);
	strcpy(LocalPrompt,prompt);
	
	l=strlen(LocalPrompt);

	while ( OccurenceDebutPrompt = strstr(TextTMP,LocalPrompt) )
	{
		int j=0;
		if (OccurenceDebutPrompt)
		{
			for(j=0;j< (signed)(  strlen(OccurenceDebutPrompt)- strlen(LocalPrompt) );j++)		
			{
				OccurenceDebutPrompt[j]=OccurenceDebutPrompt[j+strlen(LocalPrompt)];
			}
			OccurenceDebutPrompt[j]='\0';
		Retour=TRUE;	
		}	

	}
	
	strcpy(Text,TextTMP);		
	
	free(TextTMP);
	free(LocalPrompt);
	return Retour;
	
}
/*-----------------------------------------------------------------------------------*/
void HomeFunction _PARAMS((char *fname, unsigned long fname_len))
{
	LPTW lptw;
	lptw = (LPTW) GetWindowLong (FindWindow(NULL,ScilexWindowName), 0);
	
	if ( IsWindowInterface() )
	{
  		int nYinc = 0;
  		int cy= 0;
  	  		
  		cy = lptw->CursorPos.y * lptw->CharSize.y;
		nYinc =  cy  - lptw->ScrollPos.y;
		
		lptw->ScrollPos.y += nYinc;
     		ScrollWindow (lptw->hWndText, 0, -nYinc, NULL, NULL);
      	      		
      		UpdateWindow (lptw->hWndText);	
//      		InvalidateRect (lptw->hWndText, NULL, TRUE);
      		
      	}
      	else sciprint("  Only in Graphical Version.\n"); 
	
	
	
}
/*-----------------------------------------------------------------------------------*/

/* Redimensionnement dynamique du Screenbuffer */
void ResizeScreenBuffer(LPTW lptw)
{
	#define ScreenBufferSizeMaxY 25200 /* Valeur pour compatibilité avec W9x */
	
	
	/* Limitation de la taille du buffer d'ecran */	
	if (lptw->ScreenSize.y < ScreenBufferSizeMaxY )
	{
		/* On agrandit le buffer écran */
		 ReAllocScreenBuffer(lptw);
	}
	else
	{
		/* On remonte dans le buffer écran */
		ReorganizeScreenBuffer(lptw);

	}
	
	/* Mise à jour Scrollbar*/
	SetScrollRange (lptw->hWndText, SB_VERT, 0, lptw->ScrollMax.y, FALSE);
	
	SetScrollPos (lptw->hWndText, SB_VERT, lptw->ScrollPos.y, TRUE);

	SetScrollPos (lptw->hWndText, SB_HORZ, lptw->ScrollPos.x, TRUE);


	
}
/*-----------------------------------------------------------------------------------*/
void ReAllocScreenBuffer(LPTW lptw)
{
	#define AddLines 360 /* Ajout de l'equivalent d'une page initiale */
	
	HGLOBAL hglobal;
	int NombredeCaracteres=0;
	BYTE *CopyOfScreenBuffer=NULL;
	BYTE *CopyOfAttribBuffer=NULL;
	int NewScreenSizeY=0;
		
	/* Nombre de caracteres réellement utilisé */
	NombredeCaracteres=lptw->CursorPos.y * lptw->ScreenSize.x + lptw->CursorPos.x;
	CopyOfScreenBuffer=(char*)malloc( (NombredeCaracteres+1)* sizeof(char));
	if (CopyOfScreenBuffer == NULL)
    	{
      		MessageBox ((HWND) NULL, szNoMemory, (LPSTR) NULL, MB_ICONHAND | MB_SYSTEMMODAL);
      		return;
    	}
    	/* Copie des caracteres dans un buffer intermédiaire */
	strncpy(CopyOfScreenBuffer,(LPSTR)lptw->ScreenBuffer,NombredeCaracteres);
	

	/* Allocation du nouveau buffer */
	NewScreenSizeY=lptw->ScreenSize.y+AddLines;
	
	
	hglobal = GlobalHandle (lptw->ScreenBuffer);
  	if (hglobal)
    	{
      		GlobalUnlock (hglobal);
      		GlobalFree (hglobal);
    	}
		
	hglobal = GlobalAlloc (GHND, lptw->ScreenSize.x * NewScreenSizeY);

    	lptw->ScreenBuffer = (BYTE FAR *) GlobalLock (hglobal);
  	if (lptw->ScreenBuffer == (BYTE FAR *) NULL)
    	{
      		MessageBox ((HWND) NULL, szNoMemory, (LPSTR) NULL, MB_ICONHAND | MB_SYSTEMMODAL);
      		exit(1);
    	}
    	/* initialisation du Screenbuffer */
  	_fmemset (lptw->ScreenBuffer, ' ', lptw->ScreenSize.x * NewScreenSizeY);
  	/* Recopie */
	memcpy(lptw->ScreenBuffer,CopyOfScreenBuffer,NombredeCaracteres);
	/* Libération du buffer intermédiaire */
	free(CopyOfScreenBuffer);
	
	/* Idem à ci-dessus pour le buffer des couleurs des caracteres */		
	CopyOfAttribBuffer=(char*)malloc( (NombredeCaracteres+1)* sizeof(char));
	if (CopyOfAttribBuffer == NULL)
    	{
      		MessageBox ((HWND) NULL, szNoMemory, (LPSTR) NULL, MB_ICONHAND | MB_SYSTEMMODAL);
      		exit(1);
    	}
	strncpy(CopyOfAttribBuffer,(LPSTR)lptw->AttrBuffer,NombredeCaracteres);
	hglobal = GlobalHandle (lptw->AttrBuffer);
  	if (hglobal)
    	{
      		GlobalUnlock (hglobal);
      		GlobalFree (hglobal);
    	}

	hglobal = GlobalAlloc (GHND, lptw->ScreenSize.x * NewScreenSizeY);
  	lptw->AttrBuffer = (BYTE FAR *) GlobalLock (hglobal);
  	if (lptw->AttrBuffer == (BYTE FAR *) NULL)
    	{
      		MessageBox ((HWND) NULL, szNoMemory, (LPSTR) NULL, MB_ICONHAND | MB_SYSTEMMODAL);
      		exit(1);
    	}
  	_fmemset (lptw->AttrBuffer, NOTEXT, lptw->ScreenSize.x * NewScreenSizeY);
  	memcpy(lptw->AttrBuffer,CopyOfAttribBuffer,NombredeCaracteres);
	free(CopyOfAttribBuffer);
	
	/* Nouveau Max. de la scrollbar verticale */
	lptw->ScrollMax.y=lptw->ScrollMax.y+(lptw->CharSize.y*AddLines);
	
	lptw->ScreenSize.y=NewScreenSizeY;
}
/*-----------------------------------------------------------------------------------*/
void ReorganizeScreenBuffer(LPTW lptw)
{
	#define RemoveLines 120
	int DecalageY=0;
	int NombredeCaracteres=0;
	
	BYTE *CopyOfScreenBuffer=NULL;
	BYTE *CopyOfAttribBuffer=NULL;
      	
     	      			
      	NombredeCaracteres=lptw->CursorPos.y * lptw->ScreenSize.x + lptw->CursorPos.x;
     	
      	DecalageY=lptw->ScreenSize.x*RemoveLines;

      	
	CopyOfScreenBuffer=(char*)malloc( (NombredeCaracteres+1)* sizeof(char));
			
      	strncpy(CopyOfScreenBuffer,(LPSTR)(lptw->ScreenBuffer+DecalageY),NombredeCaracteres-DecalageY);
      	_fmemset (lptw->ScreenBuffer, ' ', lptw->ScreenSize.x * lptw->ScreenSize.y);
      	strncpy((LPSTR)lptw->ScreenBuffer,CopyOfScreenBuffer,NombredeCaracteres-DecalageY);
      	free(CopyOfScreenBuffer);
      			
      			
      	CopyOfAttribBuffer=(char*)malloc( (NombredeCaracteres+1)* sizeof(char));
      	strncpy(CopyOfAttribBuffer,(LPSTR)(lptw->AttrBuffer+DecalageY),NombredeCaracteres-DecalageY);
      	_fmemset (lptw->AttrBuffer, NOTEXT, lptw->ScreenSize.x * lptw->ScreenSize.y);
      	strncpy((LPSTR)lptw->AttrBuffer,CopyOfAttribBuffer,NombredeCaracteres-DecalageY);
      	free(CopyOfAttribBuffer);
      	
      	
      	lptw->CursorPos.y=lptw->CursorPos.y-RemoveLines;
      	lptw->ScrollPos.y=lptw->ScrollPos.y-(RemoveLines*lptw->CharSize.y);
      	
  	

}
/*-----------------------------------------------------------------------------------*/
void ExitWindow(void)
{
  LPTW lptw;
  char Message[255];
  char Title[50];
  lptw = (LPTW) GetWindowLong (FindWindow(NULL,ScilexWindowName), 0);	
  if ( (get_is_reading()==FALSE) || (GetThreadPasteRunning()) )
  {
    if (GetThreadPasteRunning())SuspendThread(GetHandleThreadPaste());
    
    switch (lptw->lpmw->CodeLanguage)
    {
    	case 1:
    		strcpy(Message,"Etes vous sûr de quitter ?");
    		strcpy(Title,"Quitter");
    	break;
    	case 0:default:
    		strcpy(Message,"Are you sure to quit ?");
    		strcpy(Title,"Quit");
    	break;
    }   
    
    if (MessageBox(lptw->hWndParent,Message,Title,MB_SYSTEMMODAL|MB_YESNO|MB_ICONWARNING)==IDYES)
    {
    	/* Stop la thread Coller si en cours*/
        ResumeThread(GetHandleThreadPaste());
        if (GetThreadPasteRunning())
        {
        	TerminateThread(GetHandleThreadPaste(),1);
           	SetThreadPasteRunning(FALSE);
           	CloseHandle( GetHandleThreadPaste() );
        }
        WriteTextIni (lptw);
    	C2F(sciquit)();
        Kill_Scilex();
     }
     else
     {
     	if (GetThreadPasteRunning()) ResumeThread(GetHandleThreadPaste());
     }
   }
   else
   {
   	WriteTextIni (lptw);
	C2F(sciquit)();
	Kill_Scilex();
   }
	   	
}
/*-----------------------------------------------------------------------------------*/
void write_scilab_synchro(char *line)
/* write_scilab_synchro attend le prompt pour ecrire une ligne sur la console*/
/* une ligne c a d sans retour chariot ou un seul */
{
	DWORD IdThreadWrite;
		
	InitializeCriticalSection(&Sync);
	hThreadWrite=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)WriteTextThread,line,0,(LPDWORD)&IdThreadWrite);
	CloseHandle(hThreadWrite);
	
}
/*-----------------------------------------------------------------------------------*/
DWORD WINAPI WriteTextThread(LPVOID lpParam)
{
	#define TEMPOTOUCHE  1
	LPTW lptw;
	char *line;
	
	line=(char *)lpParam;
	lptw = (LPTW) GetWindowLong (FindWindow(NULL,ScilexWindowName), 0);
	
	
	EnterCriticalSection(&Sync);
	while ( C2F (ismenu) () == 1 ) {Sleep(TEMPOTOUCHE);}
	/* Il n'y a plus rien dans la queue des commandes */
	while ( lptw->bGetCh == FALSE ) {Sleep(TEMPOTOUCHE);}
		
	/* Nous sommes au prompt */
	write_scilab(line);
	LeaveCriticalSection(&Sync);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int ShowWindowFunction _PARAMS((char *fname,unsigned long fname_len))
{
  static int l1, m1, n1;	
  if (IsWindowInterface())
  {
    if (Rhs == 0)
    	{
    		sciprint("Error no window num.\n");
    	}
  	else
  	{
  		struct BCG *ScilabGC=NULL;
  		int num_win=-2;
  		CheckLhs(1,1);
  		GetRhsVar(1,"i",&m1,&n1,&l1);
  		num_win=*istk(l1);
  		LhsVar(1)=0;
  		ScilabGC = GetWindowXgcNumber (num_win);
  		if (num_win == -1)
  		{
  			extern char ScilexWindowName[MAX_PATH];
			LPTW lptw;
			lptw = (LPTW) GetWindowLong (FindWindow(NULL,ScilexWindowName), 0);
			if ( IsIconic(lptw->hWndParent) )
			{
				ShowWindow(lptw->hWndParent,SW_RESTORE);
				ConsoleIsMinimized=FALSE;
				
	
			}
			else
			{
				ShowWindow(lptw->hWndParent,SW_MINIMIZE);
				ConsoleIsMinimized=TRUE;
			} 
			

  		}
  		else if (ScilabGC != (struct BCG *) 0)
		{
			if ( IsIconic(ScilabGC->hWndParent) )
			{
				ShowWindow(ScilabGC->hWndParent,SW_RESTORE);
			}
			else
			{
				ShowWindow(ScilabGC->hWndParent,SW_MINIMIZE);
				ForceToActiveWindowParent();
			}
			
			
  		}
  		
 	}
  }		
  else
 	{
 		sciprint("Not in Console mode\n");
 	}
return 0;
}
/*-----------------------------------------------------------------------------------*/
void InvalidateCursor( void ) 
{
	POINT pt; // Screen coordinates!
	GetCursorPos( &pt );
	SetCursorPos( pt.x, pt.y );
}
/*-----------------------------------------------------------------------------------*/
void MessageBoxNewGraphicMode(void)
{
	HKEY key;
	DWORD result,size=4;
	char Clef[MAX_PATH];
	int DontShowMessageNewGraphicMode,Ans;

	wsprintf(Clef,"SOFTWARE\\Scilab\\%s\\Settings",VERSION);
  	result=RegOpenKeyEx(HKEY_CURRENT_USER, Clef, 0, KEY_QUERY_VALUE , &key);

	if ( RegQueryValueEx(key, "DontShowMessageNewGraphicMode", 0, NULL, (LPBYTE)&Ans, &size) !=  ERROR_SUCCESS )
  	{
  			DontShowMessageNewGraphicMode = 0;
	}
	else
	{
			DontShowMessageNewGraphicMode = Ans;
	}

	if ( result == ERROR_SUCCESS ) RegCloseKey(key);

	if (DontShowMessageNewGraphicMode == 0)
	{
		DialogBox(hdllInstance, "IDD_MESSAGE_NEW_GRAPHIC", NULL,MessageBoxNewGraphicModeDlgProc);
	}
}
/*-----------------------------------------------------------------------------------*/
BOOL CALLBACK MessageBoxNewGraphicModeDlgProc(HWND hwnd,UINT Message, WPARAM wParam, LPARAM lParam)
{
   int LanguageCode=0;
   extern char ScilexWindowName[MAX_PATH];
   LPTW lptw;
   
   HKEY key;
   DWORD result,size=4;
   int Language;
   char Clef[MAX_PATH];
   lptw = (LPTW) GetWindowLong (FindWindow(NULL,ScilexWindowName), 0);
   wsprintf(Clef,"SOFTWARE\\Scilab\\%s\\Settings",VERSION);
   result=RegOpenKeyEx(HKEY_CURRENT_USER, Clef, 0, KEY_QUERY_VALUE , &key);

   	if ( RegQueryValueEx(key, "Language", 0, NULL, (LPBYTE)&Language, &size) !=  ERROR_SUCCESS )
  	{
		LanguageCode = 0; /* English Default*/
	}
	else
	{
		LanguageCode = Language;
	}


   if ( result == ERROR_SUCCESS ) RegCloseKey(key);

   switch(Message)
   {
      case WM_INITDIALOG:
         CheckDlgButton(hwnd, IDC_CHECKNEWGRAPHIC, BST_UNCHECKED);
		 switch (LanguageCode)
		{
    		case 1:
			SetWindowText(hwnd,"Remarque Importante");
			SetDlgItemText(hwnd,IDC_NEWGRAPHICMESSAGE,
"Cette version de Scilab utilise par défaut le nouveau mode graphique.\n\nVous pouvez revenir à l'ancien mode par l'instruction:\n\tset old_style on\n\nCependant les animations doivent être adaptées au nouveau mode graphique.\n\nPour plus d'information, consulter l'aide en ligne de \"graphics_entities\".\n\n\nVeuillez noter que l'ancien mode sera obsolète dans les futures versions.");

    		SetDlgItemText(hwnd,IDC_CHECKNEWGRAPHIC,"Ne plus afficher ce message");
    		break;

    		case 0:default:
			SetWindowText(hwnd,"Warning:");
			SetDlgItemText(hwnd,IDC_NEWGRAPHICMESSAGE,
"This Scilab version uses, by default, the new graphics mode.\n\nYou can go back to the old graphics entering the instruction:\n\tset old_style on\n\nNote however that the animations should be adapted to the new graphics.\n\nFor more information, see \"graphics_entities\" help page.\n\n\nPlease notice that the old graphics mode will be obsolete in future releases.");
			SetDlgItemText(hwnd,IDC_CHECKNEWGRAPHIC,"Don't show this screen at startup");
    		break;
		}
      return TRUE;

      case WM_COMMAND:
         switch(LOWORD(wParam))
         {
            case IDOK:
				{
					HKEY key;
					DWORD result,dwsize=4;
					char Clef[MAX_PATH];
					int DontShowMessageNewGraphicMode;
	
				  	wsprintf(Clef,"SOFTWARE\\Scilab\\%s\\Settings",VERSION);  	
  					RegCreateKeyEx(HKEY_CURRENT_USER, Clef, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &result);

					if(IsDlgButtonChecked(hwnd, IDC_CHECKNEWGRAPHIC) == BST_CHECKED)
					{
						//The Box is CHECKED!
						DontShowMessageNewGraphicMode=1;
						RegSetValueEx(key, "DontShowMessageNewGraphicMode", 0, REG_DWORD, (LPBYTE)&DontShowMessageNewGraphicMode, dwsize);
                     
					}
					else
					{
						//The Box is NOT checked!
						DontShowMessageNewGraphicMode=0;
						RegSetValueEx(key, "DontShowMessageNewGraphicMode", 0, REG_DWORD, (LPBYTE)&DontShowMessageNewGraphicMode, dwsize);
					}

					RegCloseKey(key);
				}
				
               EndDialog(hwnd, IDOK);
            return TRUE;
         }
      break;
   }
   return FALSE;
}
/*-----------------------------------------------------------------------------------*/
void SelectAll(LPTW lptw,BOOL DoAMark)
{
	POINT pt;
	pt.x = 0;
	pt.y = 0;
	pt.x = (pt.x + lptw->ScrollPos.x)/lptw->CharSize.x;
	pt.y = (pt.y + lptw->ScrollPos.y)/lptw->CharSize.y;
	ClearMark(lptw, pt);

	lptw->MarkBegin.x=0;
	lptw->MarkBegin.y=0;
	lptw->MarkEnd.x=lptw->CursorPos.x;
	lptw->MarkEnd.y=lptw->CursorPos.y;

	SendMessage (lptw->hWndText, WM_HSCROLL,SB_LEFT, (LPARAM) 0);
	SendMessage (lptw->hWndText, WM_VSCROLL, SB_TOP, (LPARAM) 0);
		  

	if (DoAMark)
	{
		DoMark (lptw, lptw->MarkBegin,lptw->MarkEnd, TRUE);
	
	
	if ((lptw->ScreenSize.x * lptw->MarkBegin.y + lptw->MarkBegin.x) >
	    		(lptw->ScreenSize.x * lptw->MarkEnd.y + lptw->MarkEnd.x))
	{
		POINT tmp;
		tmp.x = lptw->MarkBegin.x;
		tmp.y = lptw->MarkBegin.y;
		lptw->MarkBegin.x = lptw->MarkEnd.x;
		lptw->MarkBegin.y = lptw->MarkEnd.y;
		lptw->MarkEnd.x = tmp.x;
		lptw->MarkEnd.y = tmp.y;
   	}
}
   if  ( HasAZoneTextSelected(lptw) )
  {
	  	/* Zone Selectionnée --> Activation Menus */
	  	EnableMenuItem(lptw->hPopMenu,M_COPY_CLIP,MF_ENABLED);
	    EnableMenuItem(lptw->hPopMenu,M_HELPON,MF_ENABLED);
	   	EnableMenuItem(lptw->hPopMenu,M_PRINTSELECTION,MF_ENABLED);
	   	EnableMenuItem(lptw->hPopMenu,M_OPENSELECTION,MF_ENABLED);
	   	EnableMenuItem(lptw->hPopMenu,M_EVALSELECTION,MF_ENABLED);
	   	/* EnableMenuItem(lptw->hPopMenu,M_CUT,MF_ENABLED); */
	    	
	  }

}
/*-----------------------------------------------------------------------------------*/
void UnSelect(LPTW lptw)
{
	POINT pt;
	pt=lptw->CursorPos;

	ClearMark(lptw, pt);
	if  ( !HasAZoneTextSelected(lptw) )
	  {
	  	/* Zone Selectionnée --> Activation Menus */
	  	EnableMenuItem(lptw->hPopMenu,M_COPY_CLIP,MF_GRAYED);
	    EnableMenuItem(lptw->hPopMenu,M_HELPON,MF_GRAYED);
	   	EnableMenuItem(lptw->hPopMenu,M_PRINTSELECTION,MF_GRAYED);
	   	EnableMenuItem(lptw->hPopMenu,M_OPENSELECTION,MF_GRAYED);
	   	EnableMenuItem(lptw->hPopMenu,M_EVALSELECTION,MF_GRAYED);
	   	/* EnableMenuItem(lptw->hPopMenu,M_CUT,MF_ENABLED); */
	    	
	  }
}
/*-----------------------------------------------------------------------------------*/
/* Tente d'écrire dans la Console Graphique */
/* Retourne FALSE si la commande a été envoyée mais pas affichée */
/* La Commande ne doit etre constituée que d'une ligne */
BOOL WriteIntoScilab(LPTW lptw,char *StringCommand)
{
	BOOL retour=FALSE;
	
	if  (( ( C2F (ismenu) () == 1 ) || ( lptw->bGetCh == FALSE ) ) && (!get_is_reading()) )
	{
		StoreCommand(StringCommand);
	}
	else
	{
		int lg=0;
		char *CommandLine=NULL;
		
		lg=strlen(StringCommand);
		CommandLine=(char*)malloc( (lg+1)*sizeof(char) );
		wsprintf(CommandLine,"%s\n",StringCommand);

		WriteIntoKeyBuffer(lptw,CommandLine);
		free(CommandLine);
		retour=TRUE;
	}

	return retour;
}
/*-----------------------------------------------------------------------------------*/
/* Ecrit des caracteres dans le Buffer Clavier */
void WriteIntoKeyBuffer(LPTW lptw,char *StringCommand)
{
	int lg=0;
	int i=0;

	lg=strlen(StringCommand);
	WriteInKeyBuf=TRUE;
	while(i<lg)
		{
			long count;
			count = lptw->KeyBufIn - lptw->KeyBufOut;
			
			if (count < 0) count = count+lptw->KeyBufSize;
			if (count < (long) (lptw->KeyBufSize-1)) 
			{
				if (StringCommand[i] == '\t') *lptw->KeyBufIn++ = ' ';
				else *lptw->KeyBufIn++ = StringCommand[i];
				if (lptw->KeyBufIn - lptw->KeyBuf >= (signed)lptw->KeyBufSize)
				lptw->KeyBufIn = lptw->KeyBuf;	/* wrap around */
			}
			i++;	
		}
	WriteInKeyBuf=FALSE;
}
/*-----------------------------------------------------------------------------------*/
BOOL IsToThePrompt(void)
{
	BOOL retour=FALSE;
	extern char ScilexWindowName[MAX_PATH];
	LPTW lptw;
	lptw = (LPTW) GetWindowLong (FindWindow(NULL,ScilexWindowName), 0);

	if  (( ( C2F (ismenu) () == 1 ) || ( lptw->bGetCh == FALSE ) ) && (!get_is_reading()) ) retour=FALSE;
	else retour=TRUE;

	return retour;
}
/*-----------------------------------------------------------------------------------*/
void EnableNMenus(LPTW lptw,int numbermenus)
{
  int i=0;
  for(i=0;i<numbermenus;i++)
  {
	  EnableMenuItem (lptw->lpmw->hMenu, i, MF_ENABLED| MF_BYPOSITION);
  }
  DrawMenuBar(lptw->hWndParent); 	
}
/*-----------------------------------------------------------------------------------*/
void EnableMenus(LPTW lptw)
{
  int Nums = GetMenuItemCount (lptw->lpmw->hMenu);
  EnableNMenus(lptw,Nums);
}
/*-----------------------------------------------------------------------------------*/
void DisableMenus(LPTW lptw)
{
  int i=0;
  int Nums = GetMenuItemCount (lptw->lpmw->hMenu);

  for(i=0;i<Nums;i++)
  {
	  EnableMenuItem (lptw->lpmw->hMenu, i, MF_GRAYED| MF_BYPOSITION);
  }
  DrawMenuBar(lptw->hWndParent); 
}
/*-----------------------------------------------------------------------------------*/
void EnableToolBar(LPTW lptw)
{
	int i=0;
	if (lptw->lpmw->ShowToolBar)
	{
		for (i = 0; i < lptw->lpmw->nButton; i++)
		{
			ShowWindow( lptw->lpmw->hButton[i] , SW_SHOWNORMAL );
		}
	}
}
/*-----------------------------------------------------------------------------------*/
void DisableToolBar(LPTW lptw)
{
	int i=0;
	if (lptw->lpmw->ShowToolBar)
	{
		for (i = 0; i < lptw->lpmw->nButton; i++)
		{
			ShowWindow( lptw->lpmw->hButton[i] , SW_HIDE );
		}
	}
}
/*-----------------------------------------------------------------------------------*/
