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
 * Modifed for Scilab 1997
 *   Jean-Philippe Chancelier 
 */

/* WARNING: Do not write to stdout/stderr with functions not listed 
   in win/wtext.h */
#ifndef STRICT
#define STRICT
#endif
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#ifndef __GNUC__
#include <shellapi.h>
#endif

#include <string.h>		/* use only far items */
#include <stdlib.h>
#include <ctype.h>

#include "wgnuplib.h"
#include "wresource.h"
#include "wcommon.h"

/* font stuff */
#define TEXTFONTSIZE 9
#define TEXTFONTNAME "Terminal"

extern int C2F (scilines) (int *nl, int *nc);

/* limits */
#define MAXSTR 256
POINT ScreenMinSize =
{16, 4};
EXPORT LRESULT CALLBACK WndParentProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
EXPORT LRESULT CALLBACK WndTextProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void ReadTextIni (LPTW lptw);
void LimitMark (LPTW lptw, POINT FAR * lppt);

char szNoMemory[] = "out of memory";
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

#define NOTEXT 0xF0
#define MARKFORE RGB(255,255,255)
#define MARKBACK RGB(0,0,128)
#define TextFore(attr) TextColorTable[(attr) & 15]
#define TextBack(attr) TextColorTable[(attr>>4) & 15]

/*********************************************
 * message Loop 
 *********************************************/

extern HWND HelpModeless;	/* the modeless Help Window */

EXPORT void WINAPI
TextMessage (void)
{
  TextMessage1 (0);
  return;
}

/*********************************************
 * text window class 
 *********************************************/

static void 
CreateTextClass (LPTW lptw)
{
  WNDCLASS wndclass;
  wndclass.style = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc = WndTextProc;
  wndclass.cbClsExtra = 0;
  wndclass.cbWndExtra = 2 * sizeof (void FAR *);
  wndclass.hInstance = lptw->hInstance;
  wndclass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
  wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);
  wndclass.hbrBackground = NULL;
  lptw->hbrBackground = CreateSolidBrush (lptw->bSysColors ?
				GetSysColor (COLOR_WINDOW) : RGB (0, 0, 0));
  wndclass.lpszMenuName = NULL;
  wndclass.lpszClassName = szTextClass;
  RegisterClass (&wndclass);


  wndclass.style = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc = WndParentProc;
  wndclass.cbClsExtra = 0;
  wndclass.cbWndExtra = 2 * sizeof (void FAR *);
  wndclass.hInstance = lptw->hInstance;
  if (lptw->hIcon)
    wndclass.hIcon = lptw->hIcon;
  else
    wndclass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
  wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);
  wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
  wndclass.lpszMenuName = NULL;
  wndclass.lpszClassName = szParentClass;
  RegisterClass (&wndclass);
}

/*********************************************
 * make text window 
 *********************************************/

EXPORT int WINAPI 
TextInit (LPTW lptw)
{
  RECT rect;
  HMENU sysmenu;
  HGLOBAL hglobal;
  char buf[80];

  ReadTextIni (lptw);

  if (!lptw->hPrevInstance)
    CreateTextClass (lptw);

  if (lptw->KeyBufSize == 0)
    lptw->KeyBufSize = 256;

  if (lptw->ScreenSize.x < ScreenMinSize.x)
    lptw->ScreenSize.x = ScreenMinSize.x;
  if (lptw->ScreenSize.y < ScreenMinSize.y)
    lptw->ScreenSize.y = ScreenMinSize.y;

  lptw->CursorPos.x = lptw->CursorPos.y = 0;
  lptw->bFocus = FALSE;
  lptw->bGetCh = FALSE;
  lptw->CaretHeight = 0;
  if (!lptw->nCmdShow)
    lptw->nCmdShow = SW_SHOWNORMAL;
  if (!lptw->Attr)
    lptw->Attr = 0xf0;		/* black on white */

  hglobal = GlobalAlloc (GHND, lptw->ScreenSize.x * lptw->ScreenSize.y);
  lptw->ScreenBuffer = (BYTE FAR *) GlobalLock (hglobal);
  if (lptw->ScreenBuffer == (BYTE FAR *) NULL)
    {
      MessageBox ((HWND) NULL, szNoMemory, (LPSTR) NULL, MB_ICONHAND | MB_SYSTEMMODAL);
      return (1);
    }
  memset (lptw->ScreenBuffer, ' ', lptw->ScreenSize.x * lptw->ScreenSize.y);
  hglobal = GlobalAlloc (GHND, lptw->ScreenSize.x * lptw->ScreenSize.y);
  lptw->AttrBuffer = (BYTE FAR *) GlobalLock (hglobal);
  if (lptw->AttrBuffer == (BYTE FAR *) NULL)
    {
      MessageBox ((HWND) NULL, szNoMemory, (LPSTR) NULL, MB_ICONHAND | MB_SYSTEMMODAL);
      return (1);
    }
  memset (lptw->AttrBuffer, NOTEXT, lptw->ScreenSize.x * lptw->ScreenSize.y);
  hglobal = GlobalAlloc (LHND, lptw->KeyBufSize);
  lptw->KeyBuf = (BYTE FAR *) GlobalLock (hglobal);
  if (lptw->KeyBuf == (BYTE FAR *) NULL)
    {
      MessageBox ((HWND) NULL, szNoMemory, (LPSTR) NULL, MB_ICONHAND | MB_SYSTEMMODAL);
      return (1);
    }
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

  lptw->hPopMenu = CreatePopupMenu ();
  AppendMenu (lptw->hPopMenu, MF_STRING, M_COPY_CLIP, "&Copy to Clipboard");
  AppendMenu (lptw->hPopMenu, MF_STRING, M_PASTE, "&Paste");
  AppendMenu (lptw->hPopMenu, MF_STRING, M_CHOOSE_FONT, "Choose &Font...");
  AppendMenu (lptw->hPopMenu, MF_STRING | (lptw->bSysColors ? MF_CHECKED : MF_UNCHECKED),
	      M_SYSCOLORS, "&System Colors");
  if (lptw->IniFile != (LPSTR) NULL)
    {
      wsprintf (buf, "&Update %s", lptw->IniFile);
      AppendMenu (lptw->hPopMenu, MF_STRING, M_WRITEINI, (LPSTR) buf);
    }

  sysmenu = GetSystemMenu (lptw->hWndParent, 0);	/* get the sysmenu */
  AppendMenu (sysmenu, MF_SEPARATOR, 0, NULL);
  AppendMenu (sysmenu, MF_POPUP, (UINT) lptw->hPopMenu, "&Options");
  AppendMenu (sysmenu, MF_STRING, M_ABOUT, "&About");

  if (lptw->lpmw)
    LoadMacros (lptw);

  ShowWindow (lptw->hWndText, SW_SHOWNORMAL);
  BringWindowToTop (lptw->hWndText);
  SetFocus (lptw->hWndText);
  TextMessage ();
  return (0);
}

/* close a text window */

EXPORT void WINAPI 
TextClose (LPTW lptw)
{
  HGLOBAL hglobal;

  /* close window */
  if (lptw->hWndParent)
    DestroyWindow (lptw->hWndParent);
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

  if (lptw->lpmw)
    CloseMacros (lptw);
  lptw->hWndParent = (HWND) NULL;
}

/****************************************
 * XXXXX : a upgrader pour win32 
 * WritePrivateXXX sont obsoletes 
 * voir registry ds VC 
 ****************************************/

void 
WriteTextIni (LPTW lptw)
{
  RECT rect;
  LPSTR file = lptw->IniFile;
  LPSTR section = lptw->IniSection;
  char profile[80];
  int iconic;
  if ((file == (LPSTR) NULL) || (section == (LPSTR) NULL))
    return;
  iconic = IsIconic (lptw->hWndParent);
  if (iconic)
    ShowWindow (lptw->hWndParent, SW_SHOWNORMAL);
  GetWindowRect (lptw->hWndParent, &rect);
  wsprintf (profile, "%d %d", rect.left, rect.top);
  WritePrivateProfileString (section, "TextOrigin", profile, file);
  wsprintf (profile, "%d %d", rect.right - rect.left, rect.bottom - rect.top);
  WritePrivateProfileString (section, "TextSize", profile, file);
  wsprintf (profile, "%d", iconic);
  WritePrivateProfileString (section, "TextMinimized", profile, file);
  wsprintf (profile, "%s,%d", lptw->fontname, lptw->fontsize);
  WritePrivateProfileString (section, "TextFont", profile, file);
  wsprintf (profile, "%d", lptw->bSysColors);
  WritePrivateProfileString (section, "SysColors", profile, file);
  if (iconic)
    ShowWindow (lptw->hWndParent, SW_SHOWMINIMIZED);
  return;
}

void 
ReadTextIni (LPTW lptw)
{
  LPSTR file = lptw->IniFile;
  LPSTR section = lptw->IniSection;
  char profile[81];
  LPSTR p;
  BOOL bOKINI;

  bOKINI = (file != (LPSTR) NULL) && (section != (LPSTR) NULL);
  profile[0] = '\0';

  if (bOKINI)
    GetPrivateProfileString (section, "TextOrigin", "", profile, 80, file);
  if ((p = GetLInt (profile, &lptw->Origin.x)) == NULL)
    lptw->Origin.x = CW_USEDEFAULT;
  if ((p = GetLInt (p, &lptw->Origin.y)) == NULL)
    lptw->Origin.y = CW_USEDEFAULT;
  if ((file != (LPSTR) NULL) && (section != (LPSTR) NULL))
    GetPrivateProfileString (section, "TextSize", "", profile, 80, file);
  if ((p = GetLInt (profile, &lptw->Size.x)) == NULL)
    lptw->Size.x = CW_USEDEFAULT;
  if ((p = GetLInt (p, &lptw->Size.y)) == NULL)
    lptw->Size.y = CW_USEDEFAULT;

  if (bOKINI)
    GetPrivateProfileString (section, "TextFont", "", profile, 80, file);
  {
    char FAR *size;
    size = strchr (profile, ',');
    if (size)
      {
	*size++ = '\0';
	if ((p = GetInt (size, &lptw->fontsize)) == NULL)
	  lptw->fontsize = TEXTFONTSIZE;
      }
    strcpy (lptw->fontname, profile);
    if (lptw->fontsize == 0)
      lptw->fontsize = TEXTFONTSIZE;
    if (!(*lptw->fontname))
      strcpy (lptw->fontname, TEXTFONTNAME);
  }

  if (bOKINI)
    {
      int iconic;
      GetPrivateProfileString (section, "TextMinimized", "", profile, 80, file);
      if ((p = GetInt (profile, &iconic)) == NULL)
	iconic = 0;
      if (iconic)
	lptw->nCmdShow = SW_SHOWMINIMIZED;
    }
  lptw->bSysColors = FALSE;
  GetPrivateProfileString (section, "SysColors", "", profile, 80, file);
  if ((p = GetInt (profile, &lptw->bSysColors)) == NULL)
    lptw->bSysColors = 0;
}


/* Bring Cursor into text window */

EXPORT void WINAPI
TextToCursor (LPTW lptw)
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

void
NewLine (LPTW lptw)
{
  lptw->CursorPos.x = 0;
  lptw->CursorPos.y++;
  if (lptw->CursorPos.y >= lptw->ScreenSize.y)
    {
      int i = lptw->ScreenSize.x * (lptw->ScreenSize.y - 1);
      memmove (lptw->ScreenBuffer, lptw->ScreenBuffer + lptw->ScreenSize.x, i);
      memset (lptw->ScreenBuffer + i, ' ', lptw->ScreenSize.x);
      memmove (lptw->AttrBuffer, lptw->AttrBuffer + lptw->ScreenSize.x, i);
      memset (lptw->AttrBuffer + i, NOTEXT, lptw->ScreenSize.x);
      lptw->CursorPos.y--;
      ScrollWindow (lptw->hWndText, 0, -lptw->CharSize.y, NULL, NULL);
      lptw->MarkBegin.y--;
      lptw->MarkEnd.y--;
      LimitMark (lptw, &lptw->MarkBegin);
      LimitMark (lptw, &lptw->MarkEnd);
      UpdateWindow (lptw->hWndText);
    }
  if (lptw->CursorFlag)
    TextToCursor (lptw);
  TextMessage ();
}

/* Update count characters in window at cursor position */
/* Updates cursor position */
void
UpdateText (LPTW lptw, int count)
{
  HDC hdc;
  int xpos, ypos;
  xpos = lptw->CursorPos.x * lptw->CharSize.x - lptw->ScrollPos.x;
  ypos = lptw->CursorPos.y * lptw->CharSize.y - lptw->ScrollPos.y;
  hdc = GetDC (lptw->hWndText);
  if (lptw->bSysColors)
    {
      SetTextColor (hdc, GetSysColor (COLOR_WINDOWTEXT));
      SetBkColor (hdc, GetSysColor (COLOR_WINDOW));
    }
  else
    {
      SetTextColor (hdc, TextFore (lptw->Attr));
      SetBkColor (hdc, TextBack (lptw->Attr));
    }
  SelectFont (hdc, lptw->hfont);
  TextOut (hdc, xpos, ypos,
      (LPSTR) (lptw->ScreenBuffer + lptw->CursorPos.y * lptw->ScreenSize.x +
	       lptw->CursorPos.x), count);
  (void) ReleaseDC (lptw->hWndText, hdc);
  lptw->CursorPos.x += count;
  if (lptw->CursorPos.x >= lptw->ScreenSize.x)
    NewLine (lptw);
}

EXPORT int WINAPI
TextPutCh (LPTW lptw, BYTE ch)
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

void
TextPutStr (LPTW lptw, LPSTR str)
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


void
LimitMark (LPTW lptw, POINT FAR * lppt)
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

void
ClearMark (LPTW lptw, POINT pt)
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


/* output a line including attribute changes as needed */
void
DoLine (LPTW lptw, HDC hdc, int xpos, int ypos, int offset, int count)
{
  BYTE FAR *pa, attr;
  int idx, num;
  pa = lptw->AttrBuffer + offset;
  if ((offset < 0) || (offset >= lptw->ScreenSize.x * lptw->ScreenSize.y))
    MessageBox ((HWND) NULL, "panic", "panic", MB_OK | MB_ICONEXCLAMATION);
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
      if (lptw->bSysColors)
	{
	  SetTextColor (hdc, GetSysColor (COLOR_WINDOWTEXT));
	  SetBkColor (hdc, GetSysColor (COLOR_WINDOW));
	}
      else
	{
	  SetTextColor (hdc, TextFore (attr));
	  SetBkColor (hdc, TextBack (attr));
	}
      TextOut (hdc, xpos, ypos, (LPSTR) (lptw->ScreenBuffer + offset + idx),
	       count - num - idx);
      xpos += lptw->CharSize.x * (count - num - idx);
      idx = count - num;
    }
}

void
DoMark (LPTW lptw, POINT pt, POINT end, BOOL mark)
{
  int xpos, ypos;
  HDC hdc;
  int count;
  int offset;
  offset = lptw->ScreenSize.x * pt.y + pt.x;
  hdc = GetDC (lptw->hWndText);
  SelectFont (hdc, lptw->hfont);
  if (lptw->bSysColors)
    {
      SetTextColor (hdc, GetSysColor (COLOR_HIGHLIGHTTEXT));
      SetBkColor (hdc, GetSysColor (COLOR_HIGHLIGHT));
    }
  else
    {
      SetTextColor (hdc, MARKFORE);
      SetBkColor (hdc, MARKBACK);
    }
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
	  if (lptw->bSysColors)
	    {
	      SetTextColor (hdc, GetSysColor (COLOR_HIGHLIGHTTEXT));
	      SetBkColor (hdc, GetSysColor (COLOR_HIGHLIGHT));
	    }
	  else
	    {
	      SetTextColor (hdc, MARKFORE);
	      SetBkColor (hdc, MARKBACK);
	    }
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

void
UpdateMark (LPTW lptw, POINT pt)
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

/***********************************
 *  drag-drop feature 
 ***********************************/

void 
DragFunc (LPTW lptw, HDROP hdrop)
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

/***********************************
 *  Copy to Clipboard
 ***********************************/

void 
TextCopyClip (LPTW lptw)
{
  int size, count;
  HGLOBAL hGMem;
  LPSTR cbuf, cp;
  POINT pt, end;
  TEXTMETRIC tm;
  UINT type;
  HDC hdc;
  if ((lptw->MarkBegin.x == lptw->MarkEnd.x) &&
      (lptw->MarkBegin.y == lptw->MarkEnd.y))
    {
      /* copy user text */
      return;
    }
  size = (lptw->MarkEnd.y - lptw->MarkBegin.y + 1)
    * (lptw->ScreenSize.x + 2) + 1;
  hGMem = GlobalAlloc (GMEM_MOVEABLE, (DWORD) size);
  cbuf = cp = (LPSTR) GlobalLock (hGMem);
  if (cp == (LPSTR) NULL)
    return;
  pt.x = lptw->MarkBegin.x;
  pt.y = lptw->MarkBegin.y;
  end.x = lptw->MarkEnd.x;
  end.y = lptw->MarkEnd.y;

  while (pt.y < end.y)
    {
      /* copy to global buffer */
      count = lptw->ScreenSize.x - pt.x;
      memcpy (cp, lptw->ScreenBuffer + lptw->ScreenSize.x * pt.y + pt.x, count);
      /* remove trailing spaces */
      for (count = count - 1; count >= 0; count--)
	{
	  if (cp[count] != ' ')
	    break;
	  cp[count] = '\0';
	}
      cp[++count] = '\r';
      cp[++count] = '\n';
      cp[++count] = '\0';
      cp += count;
      pt.y++;
      pt.x = 0;
    }
  /* partial line */
  count = end.x - pt.x;
  if (end.y != lptw->ScreenSize.y)
    {
      memcpy (cp, lptw->ScreenBuffer + lptw->ScreenSize.x * pt.y + pt.x, count);
      cp[count] = '\0';
    }
  size = strlen (cbuf) + 1;
  GlobalUnlock (hGMem);
  hGMem = GlobalReAlloc (hGMem, (DWORD) size, GMEM_MOVEABLE);
  /* find out what type to put into clipboard */
  hdc = GetDC (lptw->hWndText);
  SelectFont (hdc, lptw->hfont);
  GetTextMetrics (hdc, (TEXTMETRIC FAR *) & tm);
  if (tm.tmCharSet == OEM_CHARSET)
    type = CF_OEMTEXT;
  else
    type = CF_TEXT;
  ReleaseDC (lptw->hWndText, hdc);
  /* give buffer to clipboard */
  OpenClipboard (lptw->hWndParent);
  EmptyClipboard ();
  SetClipboardData (type, hGMem);
  CloseClipboard ();
}

void
TextMakeFont (LPTW lptw)
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

void
TextSelectFont (LPTW lptw)
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

/* parent overlapped window */
EXPORT LRESULT CALLBACK
WndParentProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
	MMinfo[3].x = ScreenMinSize.x * tm.tmAveCharWidth
	  + GetSystemMetrics (SM_CXVSCROLL) + 2 * GetSystemMetrics (SM_CXFRAME);
	MMinfo[3].y = ScreenMinSize.y * tm.tmHeight
	  + GetSystemMetrics (SM_CYHSCROLL) + 2 * GetSystemMetrics (SM_CYFRAME)
	  + GetSystemMetrics (SM_CYCAPTION);
      }
      return (0);
    case WM_SIZE:
      SetWindowPos (lptw->hWndText, (HWND) NULL, 0, lptw->ButtonHeight,
		    LOWORD (lParam), HIWORD (lParam) - lptw->ButtonHeight,
		    SWP_NOZORDER | SWP_NOACTIVATE);
      return (0);
    case WM_COMMAND:
      if (IsWindow (lptw->hWndText))
	SetFocus (lptw->hWndText);
      SendMessage (lptw->hWndText, message, wParam, lParam);
      /* pass on menu commands */
      return (0);
    case WM_PAINT:
      {
	hdc = BeginPaint (hwnd, &ps);
	if (lptw->ButtonHeight)
	  {
	    HBRUSH hbrush;
	    GetClientRect (hwnd, &rect);
	    hbrush = CreateSolidBrush (GetSysColor (COLOR_BTNSHADOW));
	    rect.bottom = lptw->ButtonHeight - 1;
	    FillRect (hdc, &rect, hbrush);
	    DeleteBrush (hbrush);
	    SelectPen (hdc, GetStockPen (BLACK_PEN));
	    MoveToEx (hdc, rect.left, lptw->ButtonHeight - 1, NULL);
	    LineTo (hdc, rect.right, lptw->ButtonHeight - 1);
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
	RECT crect, wrect;
	TEXTMETRIC tm;
	lptw = ((CREATESTRUCT *) lParam)->lpCreateParams;
	SetWindowLong (hwnd, 0, (LONG) lptw);
	lptw->hWndParent = hwnd;
	/* get character size */
	TextMakeFont (lptw);
	hdc = GetDC (hwnd);
	SelectFont (hdc, lptw->hfont);
	GetTextMetrics (hdc, (LPTEXTMETRIC) & tm);
	lptw->CharSize.y = tm.tmHeight;
	lptw->CharSize.x = tm.tmAveCharWidth;
	lptw->CharAscent = tm.tmAscent;
	ReleaseDC (hwnd, hdc);
	GetClientRect (hwnd, &crect);
	if ((lptw->CharSize.y * lptw->ScreenSize.y < crect.bottom)
	    || (lptw->CharSize.x * lptw->ScreenSize.x < crect.right))
	  {
	    /* shrink size */
	    GetWindowRect (lptw->hWndParent, &wrect);
	    MoveWindow (lptw->hWndParent, wrect.left, wrect.top,
			wrect.right - wrect.left + (lptw->CharSize.x * lptw->ScreenSize.x - crect.right),
			wrect.bottom - wrect.top + (lptw->CharSize.y * lptw->ScreenSize.y + lptw->ButtonHeight - crect.bottom),
			TRUE);
	  }
	if ((lptw->DragPre != (LPSTR) NULL) && (lptw->DragPost != (LPSTR) NULL))
	  DragAcceptFiles (hwnd, TRUE);
      }
      break;
    case WM_DESTROY:
      DragAcceptFiles (hwnd, FALSE);
      DeleteFont (lptw->hfont);
      lptw->hfont = 0;
      break;
    case WM_CLOSE:
      if (lptw->shutdown)
	{
	  FARPROC lpShutDown = lptw->shutdown;
	  (*lpShutDown) ();
	}
      break;
    }
  return DefWindowProc (hwnd, message, wParam, lParam);
}

/* child text window */
EXPORT LRESULT CALLBACK
WndTextProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  HDC hdc;
  PAINTSTRUCT ps;
  RECT rect;
  int nYinc, nXinc, nl, nc;
  LPTW lptw;
  lptw = (LPTW) GetWindowLong (hwnd, 0);
  switch (message)
    {
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
      lptw->ClientSize.y = HIWORD (lParam);
      lptw->ClientSize.x = LOWORD (lParam);
      nc = lptw->ClientSize.x / lptw->CharSize.x;
      nl = lptw->ClientSize.y / lptw->CharSize.y;
/** send number of lines info to scilab **/
      nl = (nl > 5) ? nl : 5;
/** to avoid lines set to 0 when iconified **/
      C2F (scilines) (&nl, &nc);
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
      return (0);
    case WM_VSCROLL:
      switch (LOWORD (wParam))
	{
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
	  nYinc = min (-1, -lptw->ClientSize.y);
	  break;
	case SB_PAGEDOWN:
	  nYinc = max (1, lptw->ClientSize.y);
	  break;
	case SB_THUMBPOSITION:
	  nYinc = HIWORD (wParam) - lptw->ScrollPos.y;
	  break;
	default:
	  nYinc = 0;
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
    case WM_HSCROLL:
      switch (LOWORD (wParam))
	{
	case SB_LINEUP:
	  nXinc = -lptw->CharSize.x;
	  break;
	case SB_LINEDOWN:
	  nXinc = lptw->CharSize.x;
	  break;
	case SB_PAGEUP:
	  nXinc = min (-1, -lptw->ClientSize.x);
	  break;
	case SB_PAGEDOWN:
	  nXinc = max (1, lptw->ClientSize.x);
	  break;
	case SB_THUMBPOSITION:
	  nXinc = HIWORD (wParam) - lptw->ScrollPos.x;
	  break;
	default:
	  nXinc = 0;
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
	      SendMessage (hwnd, WM_HSCROLL, SB_LINEUP, (LPARAM) 0);
	      break;
	    case VK_RIGHT:
	      SendMessage (hwnd, WM_HSCROLL, SB_LINEDOWN, (LPARAM) 0);
	      break;
	    }
	}
      else
	{
	  switch (wParam)
	    {
	    case VK_HOME:
	    case VK_END:
	    case VK_PRIOR:
	    case VK_NEXT:
	    case VK_UP:
	    case VK_DOWN:
	    case VK_LEFT:
	    case VK_RIGHT:
	    case VK_DELETE:
	      {			/* store key in circular buffer */
		long count;
		count = lptw->KeyBufIn - lptw->KeyBufOut;
		if (count < 0)
		  count += lptw->KeyBufSize;
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
	    }
	}
      break;
    case WM_RBUTTONDOWN:
      {
	POINT pt;
	pt.x = LOWORD (lParam);
	pt.y = HIWORD (lParam);
	ClientToScreen (hwnd, &pt);
	TrackPopupMenu (lptw->hPopMenu, TPM_LEFTALIGN,
			pt.x, pt.y, 0, hwnd, NULL);
      }
      return (0);
    case WM_LBUTTONDOWN:
      {				/* start marking text */
	POINT pt;
	pt.x = LOWORD (lParam);
	pt.y = HIWORD (lParam);
	pt.x = (pt.x + lptw->ScrollPos.x) / lptw->CharSize.x;
	pt.y = (pt.y + lptw->ScrollPos.y) / lptw->CharSize.y;
	ClearMark (lptw, pt);
      }
      SetCapture (hwnd);	/* track the mouse */
      lptw->Marking = TRUE;
      break;
    case WM_LBUTTONUP:
      {				/* finish marking text */
	/* ensure begin mark is before end mark */
	ReleaseCapture ();
	lptw->Marking = FALSE;
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
      {				/* store key in circular buffer */
	long count;
	count = lptw->KeyBufIn - lptw->KeyBufOut;
	if (count < 0)
	  count += lptw->KeyBufSize;
	if (count < (int) lptw->KeyBufSize - 1)
	  {
	    *lptw->KeyBufIn++ = wParam;
	    if (lptw->KeyBufIn - lptw->KeyBuf >= (int) lptw->KeyBufSize)
	      lptw->KeyBufIn = lptw->KeyBuf;	/* wrap around */
	  }
      }
      return (0);
    case WM_COMMAND:
      if (LOWORD (wParam) < NUMMENU)
	SendMacro (lptw, LOWORD (wParam));
      else
	switch (LOWORD (wParam))
	  {
	  case M_COPY_CLIP:
	    TextCopyClip (lptw);
	    return 0;
	  case M_PASTE:
	    {
	      HGLOBAL hGMem;
	      BYTE *cbuf;
	      TEXTMETRIC tm;
	      UINT type;
	      /* find out what type to get from clipboard */
	      hdc = GetDC (hwnd);
	      SelectFont (hdc, lptw->hfont);
	      GetTextMetrics (hdc, (TEXTMETRIC *) & tm);
	      if (tm.tmCharSet == OEM_CHARSET)
		type = CF_OEMTEXT;
	      else
		type = CF_TEXT;
	      ReleaseDC (lptw->hWndText, hdc);
	      /* now get it from clipboard */
	      OpenClipboard (hwnd);
	      hGMem = GetClipboardData (type);
	      if (hGMem)
		{
		  cbuf = (BYTE *) GlobalLock (hGMem);
		  while (*cbuf)
		    {
		      if (*cbuf != '\n')
			SendMessage (lptw->hWndText, WM_CHAR, *cbuf, 1L);
		      cbuf++;
		    }
		  GlobalUnlock (hGMem);
		}
	      CloseClipboard ();
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
	  }
      return (0);
    case WM_SYSCOLORCHANGE:
      DeleteBrush (lptw->hbrBackground);
      lptw->hbrBackground = CreateSolidBrush (lptw->bSysColors ?
				GetSysColor (COLOR_WINDOW) : RGB (0, 0, 0));
      return (0);
    case WM_ERASEBKGND:
      return (1);		/* we will erase it ourselves */
    case WM_PAINT:
      {
	POINT source, width, dest;
	POINT MarkBegin, MarkEnd;
	hdc = BeginPaint (hwnd, &ps);
	if (ps.fErase)
	  FillRect (hdc, &ps.rcPaint, lptw->hbrBackground);
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
		    if (lptw->bSysColors)
		      {
			SetTextColor (hdc, GetSysColor (COLOR_HIGHLIGHTTEXT));
			SetBkColor (hdc, GetSysColor (COLOR_HIGHLIGHT));
		      }
		    else
		      {
			SetTextColor (hdc, MARKFORE);
			SetBkColor (hdc, MARKBACK);
		      }
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
      lptw = ((CREATESTRUCT *) lParam)->lpCreateParams;
      SetWindowLong (hwnd, 0, (LONG) lptw);
      lptw->hWndText = hwnd;
      break;
    case WM_DESTROY:
      DeleteBrush (lptw->hbrBackground);
      break;
    }
  return DefWindowProc (hwnd, message, wParam, lParam);
}

/****************************************
 * replacement stdio routines 
 ****************************************/

/* TRUE if key hit, FALSE if no key */
EXPORT int WINAPI
TextKBHit (LPTW lptw)
{
  return (lptw->KeyBufIn != lptw->KeyBufOut);
}

/* get character from keyboard, no echo */
/* need to add extended codes */

EXPORT int WINAPI
TextGetCh (LPTW lptw)
{
  int ch;
  TextToCursor (lptw);
  lptw->bGetCh = TRUE;
  if (lptw->bFocus)
    {
      SetCaretPos (lptw->CursorPos.x * lptw->CharSize.x - lptw->ScrollPos.x,
		   lptw->CursorPos.y * lptw->CharSize.y + lptw->CharAscent
		   - lptw->CaretHeight - lptw->ScrollPos.y);
      ShowCaret (lptw->hWndText);
    }
  /** 
    jpc 1998 : Here we must wait for an event so TextMessage 
    cannot be called directly in order not to use 
    99% of the cpu time doing nothing. 
    we can use TextMessage with a Sleep or TextMessage2 
    XXXX : must check if tck/tk works with both ?
    **/
  /** 
  do {
    Sleep(1);
    TextMessage();
  } while (!TextKBHit(lptw));
  **/

  while (!TextKBHit (lptw))
    {
      TextMessage2 ();
    }

  ch = *lptw->KeyBufOut++;
  if (ch == '\r')
    ch = '\n';
  if (lptw->KeyBufOut - lptw->KeyBuf >= (int) lptw->KeyBufSize)
    lptw->KeyBufOut = lptw->KeyBuf;	/* wrap around */
  if (lptw->bFocus)
    HideCaret (lptw->hWndText);
  lptw->bGetCh = FALSE;
  return ch;
}

/*** checks if a CtrlC was typed on th keyboard **/

int 
CtrlCHit (LPTW lptw)
{
  int ch;
  if (TextKBHit (lptw))
    {
      ch = *lptw->KeyBufOut++;
      if (lptw->KeyBufOut - lptw->KeyBuf >= (int) lptw->KeyBufSize)
	lptw->KeyBufOut = lptw->KeyBuf;		/* wrap around */
      if (ch == 3)
	{
	  SignalCtrC ();
	  return (1);
	}
    }
  return (0);
}



/* get character from keyboard, with echo */
EXPORT int WINAPI
TextGetChE (LPTW lptw)
{
  int ch;
  ch = TextGetCh (lptw);
  TextPutCh (lptw, (BYTE) ch);
  return ch;
}

EXPORT LPSTR WINAPI
TextGetS (LPTW lptw, LPSTR str, unsigned int size)
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

EXPORT int WINAPI
TextPutS (LPTW lptw, LPSTR str)
{
  TextPutStr (lptw, str);
  return str[strlen (str) - 1];
}

/****************************************
 * routines added for elvis 
 ****************************************/

EXPORT void WINAPI
TextGotoXY (LPTW lptw, int x, int y)
{
  lptw->CursorPos.x = x;
  lptw->CursorPos.y = y;
}

EXPORT int WINAPI
TextWhereX (LPTW lptw)
{
  return lptw->CursorPos.x;
}

EXPORT int WINAPI
TextWhereY (LPTW lptw)
{
  return lptw->CursorPos.y;
}

EXPORT void WINAPI
TextCursorHeight (LPTW lptw, int height)
{
  lptw->CaretHeight = height;
  if (lptw->bFocus)
    CreateCaret (lptw->hWndText, 0, lptw->CharSize.x, 2 + lptw->CaretHeight);
}

EXPORT void WINAPI
TextClearEOL (LPTW lptw)
{
  HDC hdc;
  int xpos, ypos;
  int from, len;
  POINT pt;
  pt.x = pt.y = 0;
  ClearMark (lptw, pt);
  from = lptw->CursorPos.y * lptw->ScreenSize.x + lptw->CursorPos.x;
  len = lptw->ScreenSize.x - lptw->CursorPos.x;
  memset (lptw->ScreenBuffer + from, ' ', len);
  memset (lptw->AttrBuffer + from, NOTEXT, len);
  xpos = lptw->CursorPos.x * lptw->CharSize.x - lptw->ScrollPos.x;
  ypos = lptw->CursorPos.y * lptw->CharSize.y - lptw->ScrollPos.y;
  hdc = GetDC (lptw->hWndText);
  if (lptw->bSysColors)
    {
      SetTextColor (hdc, GetSysColor (COLOR_WINDOWTEXT));
      SetBkColor (hdc, GetSysColor (COLOR_WINDOW));
    }
  else
    {
      SetTextColor (hdc, TextFore (lptw->Attr));
      SetBkColor (hdc, TextBack (lptw->Attr));
    }
  SelectFont (hdc, (lptw->hfont));
  TextOut (hdc, xpos, ypos,
      (LPSTR) (lptw->ScreenBuffer + lptw->CursorPos.y * lptw->ScreenSize.x +
	       lptw->CursorPos.x), lptw->ScreenSize.x - lptw->CursorPos.x);
  (void) ReleaseDC (lptw->hWndText, hdc);
}

EXPORT void WINAPI
TextClearEOS (LPTW lptw)
{
  RECT rect;
  int from, len;
  POINT pt;
  pt.x = pt.y = 0;
  ClearMark (lptw, pt);
  from = lptw->CursorPos.y * lptw->ScreenSize.x + lptw->CursorPos.x;
  len = lptw->ScreenSize.x - lptw->CursorPos.x +
    (lptw->ScreenSize.y - lptw->CursorPos.y - 1) * lptw->ScreenSize.x;
  memset (lptw->ScreenBuffer + from, ' ', len);
  memset (lptw->AttrBuffer + from, NOTEXT, len);
  GetClientRect (lptw->hWndText, (LPRECT) & rect);
  InvalidateRect (lptw->hWndText, (LPRECT) & rect, 1);
  UpdateWindow (lptw->hWndText);
}

EXPORT void WINAPI
TextInsertLine (LPTW lptw)
{
  RECT rect;
  int from, to, len;
  POINT pt;
  pt.x = pt.y = 0;
  ClearMark (lptw, pt);
  from = lptw->CursorPos.y * lptw->ScreenSize.x,
    to = (lptw->CursorPos.y + 1) * lptw->ScreenSize.x;
  len = (lptw->ScreenSize.y - lptw->CursorPos.y - 1) * lptw->ScreenSize.x;
  memmove (lptw->ScreenBuffer + to, lptw->ScreenBuffer + from, len);
  memmove (lptw->AttrBuffer + to, lptw->AttrBuffer + from, len);
  memset (lptw->ScreenBuffer + from, ' ', lptw->ScreenSize.x);
  memset (lptw->AttrBuffer + from, NOTEXT, lptw->ScreenSize.x);
  GetClientRect (lptw->hWndText, (LPRECT) & rect);
  InvalidateRect (lptw->hWndText, (LPRECT) & rect, 1);
  UpdateWindow (lptw->hWndText);
  if (lptw->CursorFlag)
    TextToCursor (lptw);
}

EXPORT void WINAPI
TextDeleteLine (LPTW lptw)
{
  RECT rect;
  int from, to, len;
  POINT pt;
  pt.x = pt.y = 0;
  ClearMark (lptw, pt);
  to = lptw->CursorPos.y * lptw->ScreenSize.x,
    from = (lptw->CursorPos.y + 1) * lptw->ScreenSize.x;
  len = (lptw->ScreenSize.y - lptw->CursorPos.y - 1) * lptw->ScreenSize.x;
  memmove (lptw->ScreenBuffer + to, lptw->ScreenBuffer + from, len);
  memmove (lptw->AttrBuffer + to, lptw->AttrBuffer + from, len);
  from = lptw->ScreenSize.x * (lptw->ScreenSize.y - 1);
  memset (lptw->ScreenBuffer + from, ' ', lptw->ScreenSize.x);
  memset (lptw->AttrBuffer + from, NOTEXT, lptw->ScreenSize.x);
  GetClientRect (lptw->hWndText, (LPRECT) & rect);
  InvalidateRect (lptw->hWndText, (LPRECT) & rect, 1);
  UpdateWindow (lptw->hWndText);
  if (lptw->CursorFlag)
    TextToCursor (lptw);
}

EXPORT void WINAPI
TextScrollReverse (LPTW lptw)
{
  RECT rect;
  int len = lptw->ScreenSize.x * (lptw->ScreenSize.y - 1);
  memmove (lptw->ScreenBuffer + lptw->ScreenSize.x, lptw->ScreenBuffer, len);
  memset (lptw->ScreenBuffer, ' ', lptw->ScreenSize.x);
  memmove (lptw->AttrBuffer + lptw->ScreenSize.x, lptw->AttrBuffer, len);
  memset (lptw->AttrBuffer, NOTEXT, lptw->ScreenSize.x);
  if (lptw->CursorPos.y)
    lptw->CursorPos.y--;
  ScrollWindow (lptw->hWndText, 0, +lptw->CharSize.y, NULL, NULL);
  GetClientRect (lptw->hWndText, (LPRECT) & rect);
  rect.top = lptw->ScreenSize.y * lptw->CharSize.y;
  if (rect.top < rect.bottom)
    InvalidateRect (lptw->hWndText, (LPRECT) & rect, 1);
  lptw->MarkBegin.y++;
  lptw->MarkEnd.y++;
  LimitMark (lptw, &lptw->MarkBegin);
  LimitMark (lptw, &lptw->MarkEnd);
  UpdateWindow (lptw->hWndText);
}

EXPORT void WINAPI
TextAttr (LPTW lptw, BYTE attr)
{
  lptw->Attr = attr;
}

/* About Box */

EXPORT BOOL CALLBACK
AboutDlgProc (HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
  switch (wMsg)
    {
    case WM_INITDIALOG:
      {
	char buf[80];
	GetWindowText (GetParent (hDlg), buf, 80);
	SetDlgItemText (hDlg, AB_TEXT1, buf);
	SetDlgItemText (hDlg, AB_TEXT2, (LPSTR) lParam);
#ifdef __DLL__
	wsprintf (buf, "wscilab.dll Version %s", (LPSTR) WSCILABVERSION);
	SetDlgItemText (hDlg, AB_TEXT3, buf);
#endif
      }
      return TRUE;
    case WM_DRAWITEM:
      {
	LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT) lParam;
	DrawIcon (lpdis->hDC, 0, 0,
		  (HICON) GetClassLong (GetParent (hDlg), GCL_HICON));
      }
      return FALSE;
    case WM_COMMAND:
      switch (LOWORD (wParam))
	{
	case IDCANCEL:
	case IDOK:
	  EndDialog (hDlg, LOWORD (wParam));
	  return TRUE;
	}
      break;
    }
  return FALSE;
}

EXPORT void WINAPI
AboutBox (HWND hwnd, LPSTR str)
{
  DLGPROC lpfnAboutDlgProc;
  lpfnAboutDlgProc = (DLGPROC) MyGetProcAddress ("AboutDlgProc", AboutDlgProc);
  DialogBoxParam (hdllInstance, "AboutDlgBox", hwnd, lpfnAboutDlgProc, (LPARAM) str);
  EnableWindow (hwnd, TRUE);
}
