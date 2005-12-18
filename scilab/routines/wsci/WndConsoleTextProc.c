/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "WndConsoleTextProc.h"
/*-----------------------------------------------------------------------------------*/
/* Definition des codes touche */
#define VK_V        86
#define VK_C        67
#ifndef WM_MOUSEWHEEL
	#define WM_MOUSEWHEEL 0x020A
#endif
/*-----------------------------------------------------------------------------------*/
extern BOOL HasAZoneTextSelected(LPTW lptw);
extern void TextCopyClip(LPTW lptw);
extern int StoreCommand1 (char *command,int flag);
extern void ClearCommandWindow(LPTW lptw,BOOL Clearfirstline);
extern void ToolBarOnOff(LPTW lptw);
extern void ResetMenu(void);
extern void SwitchConsole(void);
extern BOOL IsEmptyClipboard(LPTW lptw);
extern void PasteFunction(LPTW lptw,BOOL special);
extern void ClearMark (LPTW lptw, POINT pt);
extern void UpdateMark (LPTW lptw, POINT pt);
extern void LimitMark (LPTW lptw, POINT FAR * lppt);
extern void CreateThreadPaste(char *Text);
extern void SendMacro (LPTW lptw, UINT m);
extern void TextSelectFont (LPTW lptw);
extern void WriteRegistryTxt (LPTW lptw);
extern void SaveCurrentLine(BOOL RewriteLineAtPrompt);
extern void HelpOn(LPTW lptw);
extern void PrintSelection(LPTW lptw,char *Entete);
extern void OpenSelection(LPTW lptw);
extern void CutSelection(LPTW lptw);
extern void EvaluateSelection(LPTW lptw);
extern void SelectAll(LPTW lptw,BOOL DoAMark);
extern void InitIhmDefaultColor(void);
extern BOOL SetIhmSystemDefaultTextBackgroundColor(void);
extern HDC TryToGetDC(HWND hWnd);
extern void TextMakeFont (LPTW lptw);
extern void DoLine(LPTW lptw, HDC hdc, int xpos, int ypos, int offset, int count);
extern void ResizeScreenBuffer(LPTW lptw);
extern BOOL SetIhmSystemDefaultTextColor(void);
extern LPTW GetTextWinScilab(void);
extern BOOL ActivateTransparencyMode(HWND hWnd);
extern void IncreaseAlphaLevel(void);
extern void DecreaseAlphaLevel(void);
extern int C2F (scilines) (int *nl, int *nc);
extern void ExitWindow(void);
extern BOOL IsEnableTransparencyMode(void);
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_TEXT_WM_CLOSE(HWND hwnd);
BOOL ON_WND_TEXT_WM_DESTROY(HWND hwnd);
BOOL ON_WND_TEXT_WM_CREATE(HWND hwnd,LPCREATESTRUCT lpCreateStruct);
BOOL ON_WND_TEXT_WM_PAINT(HWND hwnd);
BOOL ON_WND_TEXT_WM_SYSCOLORCHANGE(HWND hwnd);
BOOL ON_WND_TEXT_WM_COMMAND(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void ON_WND_TEXT_WM_CHAR(HWND hwnd, TCHAR ch, int cRepeat);
void ON_WND_TEXT_WM_SETTEXT(HWND hwnd, LPCTSTR lpszText);
void ON_WND_TEXT_WM_MOUSEMOVE(HWND hwnd, int x, int y, UINT keyFlags);
void ON_WND_TEXT_WM_LBUTTONUP(HWND hwnd, int x, int y, UINT keyFlags);
void ON_WND_TEXT_WM_LBUTTONDOWN(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
void ON_WND_TEXT_WM_MBUTTONUP(HWND hwnd, int x, int y, UINT keyFlags);
void ON_WND_TEXT_WM_RBUTTONDOWN(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
void ON_WND_TEXT_WM_KEY(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
void ON_WND_TEXT_WM_VSCROLL(HWND hwnd, HWND hwndCtl, UINT code, int pos);
void ON_WND_TEXT_WM_HSCROLL(HWND hwnd, HWND hwndCtl, UINT code, int pos);
void ON_WND_TEXT_WM_MOUSEWHEEL(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys);
void ON_WND_TEXT_WM_SIZE(HWND hwnd, UINT state, int cx, int cy);
void ON_WND_TEXT_WM_KILLFOCUS(HWND hwnd, HWND hwndNewFocus);
void ON_WND_TEXT_WM_SETFOCUS(HWND hwnd, HWND hwndOldFocus);
BOOL ON_WND_TEXT_WM_SETCURSOR(HWND hwnd, HWND hwndCursor, UINT codeHitTest, UINT msg);
/*-----------------------------------------------------------------------------------*/
#if (_MSC_VER >= 1200) && (_MSC_VER < 1300) /* Visual Studio 6 */

#ifndef HANDLE_WM_MOUSEWHEEL   
#define HANDLE_WM_MOUSEWHEEL(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (int)(short)HIWORD(wParam), (UINT)(short)LOWORD(wParam)), 0L)
#endif  

#endif
/*-----------------------------------------------------------------------------------*/
/* child text window */
EXPORT LRESULT CALLBACK WndTextProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd,WM_SETCURSOR,ON_WND_TEXT_WM_SETCURSOR);
		HANDLE_MSG(hwnd,WM_SETFOCUS,ON_WND_TEXT_WM_SETFOCUS);
		HANDLE_MSG(hwnd,WM_KILLFOCUS,ON_WND_TEXT_WM_KILLFOCUS);
		HANDLE_MSG(hwnd,WM_SIZE,ON_WND_TEXT_WM_SIZE);
		HANDLE_MSG(hwnd,WM_MOUSEWHEEL,ON_WND_TEXT_WM_MOUSEWHEEL);
		HANDLE_MSG(hwnd,WM_HSCROLL,ON_WND_TEXT_WM_HSCROLL);
		HANDLE_MSG(hwnd,WM_VSCROLL,ON_WND_TEXT_WM_VSCROLL);
		HANDLE_MSG(hwnd,WM_KEYDOWN,ON_WND_TEXT_WM_KEY);
		HANDLE_MSG(hwnd,WM_RBUTTONDOWN,ON_WND_TEXT_WM_RBUTTONDOWN);
		HANDLE_MSG(hwnd,WM_MBUTTONUP,ON_WND_TEXT_WM_MBUTTONUP);
		HANDLE_MSG(hwnd,WM_LBUTTONDOWN,ON_WND_TEXT_WM_LBUTTONDOWN);
		HANDLE_MSG(hwnd,WM_LBUTTONUP,ON_WND_TEXT_WM_LBUTTONUP);
		HANDLE_MSG(hwnd,WM_MOUSEMOVE,ON_WND_TEXT_WM_MOUSEMOVE);
		HANDLE_MSG(hwnd,WM_SETTEXT,ON_WND_TEXT_WM_SETTEXT);
		HANDLE_MSG(hwnd,WM_COMMAND,ON_WND_TEXT_WM_COMMAND);
		HANDLE_MSG(hwnd,WM_SYSCOLORCHANGE,ON_WND_TEXT_WM_SYSCOLORCHANGE);
		HANDLE_MSG(hwnd,WM_PAINT,ON_WND_TEXT_WM_PAINT);
		HANDLE_MSG(hwnd,WM_CREATE,ON_WND_TEXT_WM_CREATE);
		HANDLE_MSG(hwnd,WM_DESTROY,ON_WND_TEXT_WM_DESTROY);
		HANDLE_MSG(hwnd,WM_CLOSE,ON_WND_TEXT_WM_CLOSE);
		HANDLE_MSG(hwnd,WM_CHAR,ON_WND_TEXT_WM_CHAR);
	}
    
	return DefWindowProc (hwnd, message, wParam, lParam);
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_TEXT_WM_CLOSE(HWND hwnd)
{
	ExitWindow();
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_TEXT_WM_DESTROY(HWND hwnd)
{
	LPTW lptw=GetTextWinScilab();
	DeleteObject(lptw->hbrBackground);
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_TEXT_WM_CREATE(HWND hwnd,LPCREATESTRUCT lpCreateStruct)
{
	TEXTMETRIC tm;
	HDC hdc;
	LPTW lptw = lpCreateStruct->lpCreateParams;

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

	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_TEXT_WM_PAINT(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	LPTW lptw=GetTextWinScilab();

	POINT source, width, dest;
	POINT MarkBegin, MarkEnd;

	if (TextWhereY(lptw)+50 > lptw->ScreenSize.y) ResizeScreenBuffer(lptw);

	hdc = BeginPaint (hwnd, &ps);
	GetClientRect (hwnd, &rect);
	FillRect (hdc, &rect, lptw->hbrBackground);
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

	if (source.x < 0) source.x = 0;
	if (source.y < 0) source.y = 0;
	if (source.x + width.x > lptw->ScreenSize.x) width.x = lptw->ScreenSize.x - source.x;
	if (source.y + width.y > lptw->ScreenSize.y) width.y = lptw->ScreenSize.y - source.y;
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

			if (source.y == MarkBegin.y) start = MarkBegin.x;
			else start = 0;

			if (source.y == MarkEnd.y) end = MarkEnd.x;
			else end = lptw->ScreenSize.x;

			/* do stuff before marked text */
			offset = 0;
			count = start - source.x;
			if (count > 0) DoLine (lptw, hdc, dest.x, dest.y,source.y * lptw->ScreenSize.x + source.x, count);

			/* then the marked text */
			offset += count;
			count = end - start;
			if ((count > 0) && (offset < width.x))
			{
				SetTextColor (hdc, GetSysColor (COLOR_HIGHLIGHTTEXT));
				SetBkColor (hdc, GetSysColor (COLOR_HIGHLIGHT));

				TextOut (hdc, dest.x + lptw->CharSize.x * offset, dest.y,
					(LPSTR) (lptw->ScreenBuffer + source.y * lptw->ScreenSize.x
					+ source.x + offset), count);
			}

			/* then stuff after marked text */
			offset += count;
			count = width.x + source.x - end;
			if ((count > 0) && (offset < width.x)) DoLine (lptw, hdc, dest.x + lptw->CharSize.x * offset, dest.y,
				source.y * lptw->ScreenSize.x + source.x + offset, count);
		}
		else
		{
			DoLine (lptw, hdc, dest.x, dest.y,source.y * lptw->ScreenSize.x + source.x, width.x);
		}
		dest.y += lptw->CharSize.y;
		source.y++;
		width.y--;
	}
	EndPaint (hwnd, &ps);

	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_TEXT_WM_SYSCOLORCHANGE(HWND hwnd)
{
	LPTW lptw=GetTextWinScilab();

	if (lptw->bSysColors)
	{
		InitIhmDefaultColor();
		SetIhmSystemDefaultTextBackgroundColor();
		SetIhmSystemDefaultTextColor();
	}
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_TEXT_WM_COMMAND(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	LPTW lptw=GetTextWinScilab();

	if (id < NUMMENU) SendMacro (lptw, id);
	else
	switch (id)
	{
		case M_COPY_CLIP:
			{
				TextCopyClip (lptw);
			}
		break;

		case M_PASTE:
			{
				PasteFunction(lptw,FALSE);
			}
		break;

		case M_SPECIALPASTE:
			{
				PasteFunction(lptw,TRUE);
			}
		break;

		case M_CHOOSE_FONT:
			{
				TextSelectFont (lptw);
			}
		break;

		case M_WRITEINI:
			{
				WriteRegistryTxt (lptw);
			}
		break;
		
		case M_ABOUT:
			{
				AboutBox (hwnd);
			}
		break;

		case M_HELPON:
			{
				SaveCurrentLine(TRUE);
				HelpOn(lptw);
			}
		break;

		case M_PRINTSELECTION:
			{
				TextCopyClip (lptw);
				if (lptw->lpmw->CodeLanguage == 0)
				{
					PrintSelection(lptw,MSG_SCIMSG44);
				}
				else
				{
					PrintSelection(lptw,MSG_SCIMSG45);
				}
			}
		break;

		case M_OPENSELECTION:
			{
				SaveCurrentLine(TRUE);
				OpenSelection(lptw);
			}
		break;

		case M_CUT:
			{
				CutSelection(lptw);
			}
		break;

		case M_EVALSELECTION:
			{
				EvaluateSelection(lptw);
			}
		break;

		case M_SELECT_ALL: 
			{
				SelectAll(lptw,TRUE);
			}
		break;

	}
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
void ON_WND_TEXT_WM_CHAR(HWND hwnd, TCHAR ch, int cRepeat)
{
	LPTW lptw=GetTextWinScilab();
	long count=0;

	count = lptw->KeyBufIn - lptw->KeyBufOut;

	if (count < 0) count = count+ lptw->KeyBufSize;

	if (count < (long)lptw->KeyBufSize-2) 
	{
		if (ch==10) ch=13;
		*lptw->KeyBufIn++ = ch;

		if (lptw->KeyBufIn - lptw->KeyBuf >= (signed)lptw->KeyBufSize)	lptw->KeyBufIn = lptw->KeyBuf;	/* wrap around */
	}
}
/*-----------------------------------------------------------------------------------*/
void ON_WND_TEXT_WM_SETTEXT(HWND hwnd, LPCTSTR lpszText)
{
	CreateThreadPaste((char*)lpszText); 
}
/*-----------------------------------------------------------------------------------*/
void ON_WND_TEXT_WM_MOUSEMOVE(HWND hwnd, int x, int y, UINT keyFlags)
{
	LPTW lptw=GetTextWinScilab();

	if ((keyFlags & MK_LBUTTON) && lptw->Marking)
	{
		RECT rect;
		POINT pt;
		pt.x = x;
		pt.y = y;

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
				else pt.y = (pt.y + lptw->ScrollPos.y) / lptw->CharSize.y;

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
			while ((nYinc || nXinc) && !PtInRect (&rect, pt) &&	(GetAsyncKeyState (VK_LBUTTON) < 0));
		}
	}
}
/*-----------------------------------------------------------------------------------*/
void ON_WND_TEXT_WM_LBUTTONUP(HWND hwnd, int x, int y, UINT keyFlags)
{
	LPTW lptw=GetTextWinScilab();

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
/*-----------------------------------------------------------------------------------*/
void ON_WND_TEXT_WM_LBUTTONDOWN(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	LPTW lptw=GetTextWinScilab();
    POINT pt;

	pt.x = x;
	pt.y = y;
	pt.x = (pt.x + lptw->ScrollPos.x)/lptw->CharSize.x;
	pt.y = (pt.y + lptw->ScrollPos.y)/lptw->CharSize.y;
	ClearMark(lptw, pt);
	SetCapture(hwnd);	/* track the mouse */
	lptw->Marking = TRUE;
}
/*-----------------------------------------------------------------------------------*/
void ON_WND_TEXT_WM_MBUTTONUP(HWND hwnd, int x, int y, UINT keyFlags)
{
	LPTW lptw=GetTextWinScilab();
	PasteFunction(lptw,FALSE);
}
/*-----------------------------------------------------------------------------------*/
void ON_WND_TEXT_WM_RBUTTONDOWN(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	LPTW lptw=GetTextWinScilab();
	POINT pt;
	pt.x = x;
	pt.y = y;

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
/*-----------------------------------------------------------------------------------*/
void ON_WND_TEXT_WM_KEY(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	if (GetKeyState (VK_SHIFT) < 0)
	{
		switch (vk)
		{
			case VK_UP:
				{
				}
			break;

			case VK_DOWN:
				{
				}
			break;

			case VK_LEFT:
				{
				}
			break;

			case VK_RIGHT:
				{
				}
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
		if (GetKeyState (VK_CONTROL) < 0)
		{
			switch (vk)
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

				/* Touches Control-V effectue un "coller" du presse papier */
				case VK_V :
				{
					SendMessage (hwnd, WM_COMMAND,M_PASTE, (LPARAM) 0);
				}
				break;
				/* Touches Control-Insert Control-C effectue un "copier" dans presse papier */
				case VK_INSERT : 
				{
					LPTW lptw=GetTextWinScilab();
					if ( HasAZoneTextSelected(lptw) == TRUE ) TextCopyClip (lptw);
				}
				break;

				case VK_ADD :
					if (!IsEnableTransparencyMode())
					{
						LPTW lptw=GetTextWinScilab();
						ActivateTransparencyMode(lptw->hWndParent);
						ActivateTransparencyMode(lptw->hWndText);
					}
					IncreaseAlphaLevel();
				break;
				case VK_SUBTRACT:
					if (!IsEnableTransparencyMode())
					{
						LPTW lptw=GetTextWinScilab();
						ActivateTransparencyMode(lptw->hWndParent);
						ActivateTransparencyMode(lptw->hWndText);
					}
					DecreaseAlphaLevel();
				break;

				default:
				break;
			}
		}
		else
		{
			switch (vk)
			{
				case VK_UP: case VK_DOWN: case VK_LEFT: case VK_RIGHT:
				case VK_HOME: case VK_END: case VK_PRIOR:
				case VK_NEXT: case VK_DELETE:
				{			/* store key in circular buffer */
					LPTW lptw=GetTextWinScilab();
					long count;

					count = lptw->KeyBufIn - lptw->KeyBufOut;

					if (count < 0) count += lptw->KeyBufSize;

					if (count < (int) lptw->KeyBufSize - 2)
					{
						*lptw->KeyBufIn++ = 0;
						if (lptw->KeyBufIn - lptw->KeyBuf >= (int) lptw->KeyBufSize) lptw->KeyBufIn = lptw->KeyBuf;	/* wrap around */

						*lptw->KeyBufIn++ = flags;
						if (lptw->KeyBufIn - lptw->KeyBuf >= (int) lptw->KeyBufSize) lptw->KeyBufIn = lptw->KeyBuf;	/* wrap around */
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
					LPTW lptw=GetTextWinScilab();
					ClearCommandWindow(lptw,TRUE);
				}
				break;

				case VK_F3:
				{
					LPTW lptw=GetTextWinScilab();
					if (lptw->lpmw->LockToolBar == FALSE)
					{
						lptw->lpmw->ShowToolBar=!lptw->lpmw->ShowToolBar;

						ToolBarOnOff(lptw);
					}
				}
				break;

				/* Reset des Menus & Boutons*/
				case VK_F11:
				{
					ResetMenu();
				}
				break;        

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
}
/*-----------------------------------------------------------------------------------*/
void ON_WND_TEXT_WM_VSCROLL(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	int nYinc=0;

	LPTW lptw=GetTextWinScilab();
	switch (code)
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
		nYinc = pos - lptw->ScrollPos.y;
		break;
	case SB_THUMBTRACK:
		nYinc = pos - lptw->ScrollPos.y;
		break;
	default:
		nYinc = 0;
		break;

	}

	if ((nYinc = max (-lptw->ScrollPos.y,min (nYinc, lptw->ScrollMax.y - lptw->ScrollPos.y))) != 0)
	{
		lptw->ScrollPos.y += nYinc;
		ScrollWindow (hwnd, 0, -nYinc, NULL, NULL);
		SetScrollPos (hwnd, SB_VERT, lptw->ScrollPos.y, TRUE);
		UpdateWindow (hwnd);
	}

}
/*-----------------------------------------------------------------------------------*/
void ON_WND_TEXT_WM_HSCROLL(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	int nXinc=0;
	LPTW lptw=GetTextWinScilab();

	switch (code)
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
		nXinc = pos - lptw->ScrollPos.x;
		break;

	case SB_THUMBTRACK:
		nXinc = pos - lptw->ScrollPos.x;
		break;

	default:
		nXinc = 0;
		break;
	}

	if ((nXinc = max (-lptw->ScrollPos.x, min (nXinc, lptw->ScrollMax.x - lptw->ScrollPos.x)))	!= 0)
	{
		lptw->ScrollPos.x += nXinc;
		ScrollWindow (hwnd, -nXinc, 0, NULL, NULL);
		SetScrollPos (hwnd, SB_HORZ, lptw->ScrollPos.x, TRUE);
		UpdateWindow (hwnd);
	}	

}
/*-----------------------------------------------------------------------------------*/
void ON_WND_TEXT_WM_MOUSEWHEEL(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys)
{
	if (zDelta!=0)
	{
		int steps=yPos/zDelta;
	
		if( steps > 0 ) 
		{
			SendMessage (hwnd, WM_VSCROLL, SB_LINEUP, (LPARAM) 0);
		}

		if( steps < 0 ) 
		{
			SendMessage (hwnd, WM_VSCROLL, SB_LINEDOWN, (LPARAM) 0);
		}
	}
}
/*-----------------------------------------------------------------------------------*/
void ON_WND_TEXT_WM_SIZE(HWND hwnd, UINT state, int cx, int cy)
{
	int nl=0, nc=0;
	LPTW lptw=GetTextWinScilab();
	
	lptw->ClientSize.y = cy;
	lptw->ClientSize.x = cx;
	nc = lptw->ClientSize.x / lptw->CharSize.x;
	nl = lptw->ClientSize.y / lptw->CharSize.y;
	/** send number of lines info to scilab **/
	if ((cx>0) && (cy>0))	C2F (scilines) (&nl, &nc);

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
/*-----------------------------------------------------------------------------------*/
void ON_WND_TEXT_WM_KILLFOCUS(HWND hwnd, HWND hwndNewFocus)
{
	LPTW lptw=GetTextWinScilab();
	DestroyCaret ();
	lptw->bFocus = FALSE;
}
/*-----------------------------------------------------------------------------------*/
void ON_WND_TEXT_WM_SETFOCUS(HWND hwnd, HWND hwndOldFocus)
{
	LPTW lptw=GetTextWinScilab();
	lptw->bFocus = TRUE;
	CreateCaret (hwnd, 0, lptw->CharSize.x, 2 + lptw->CaretHeight);
	SetCaretPos (lptw->CursorPos.x * lptw->CharSize.x - lptw->ScrollPos.x,
			 lptw->CursorPos.y * lptw->CharSize.y + lptw->CharAscent
			 - lptw->CaretHeight - lptw->ScrollPos.y);
	if (lptw->bGetCh) ShowCaret (hwnd);
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_TEXT_WM_SETCURSOR(HWND hwnd, HWND hwndCursor, UINT codeHitTest, UINT msg)
{
	SetCursor( LoadCursor( NULL, IDC_ARROW ) );
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
