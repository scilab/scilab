/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "WndParentGraphProc.h"
/*-----------------------------------------------------------------------------------*/
extern TW textwin;
extern GW graphwin;
extern HDC TryToGetDC(HWND hWnd);
extern void DragFunc (LPTW lptw, HDROP hdrop);
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_PARENTGRAPH_WM_CLOSE(HWND hwnd);
BOOL ON_WND_PARENTGRAPH_WM_DESTROY(HWND hwnd);
BOOL ON_WND_PARENTGRAPH_WM_CREATE(HWND hwnd,LPCREATESTRUCT lpCreateStruct);
BOOL ON_WND_PARENTGRAPH_WM_PAINT(HWND hwnd);
BOOL ON_WND_PARENTGRAPH_WM_DROPFILES(HWND hwnd,HDROP hDrop);
BOOL ON_WND_PARENTGRAPH_WM_COMMAND(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
BOOL ON_WND_PARENTGRAPH_WM_SIZE(HWND hwnd,UINT state,int cx,int cy);
void ON_WND_PARENTGRAPH_WM_EXITSIZEMOVE(HWND hwnd);
void ON_WND_PARENTGRAPH_WM_ENTERSIZEMOVE(HWND hwnd);
BOOL ON_WND_PARENTGRAPH_WM_GETMINMAXINFO(HWND hwnd, LPMINMAXINFO lpMinMaxInfo);
void ON_WND_PARENTGRAPH_WM_CHAR(HWND hwnd, TCHAR ch, int cRepeat);
void ON_WND_PARENTGRAPH_WM_KEY(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
BOOL ON_WND_PARENTGRAPH_WM_SETFOCUS(HWND hwnd, HWND hwndOldFocus);
BOOL ON_WND_PARENTGRAPH_WM_SYSCOMMAND(HWND hwnd, WPARAM wParam, LPARAM lParam);
void ON_WND_PARENTGRAPH_WM_KILLFOCUS(HWND hwnd, HWND hwndNewFocus);
/*-----------------------------------------------------------------------------------*/
/* parent overlapped window */
EXPORT LRESULT CALLBACK WndParentGraphProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_ENTERSIZEMOVE : ON_WND_PARENTGRAPH_WM_ENTERSIZEMOVE(hwnd);  break;
		case WM_EXITSIZEMOVE  : ON_WND_PARENTGRAPH_WM_EXITSIZEMOVE(hwnd);  break;
		case WM_SYSCOMMAND    : ON_WND_PARENTGRAPH_WM_SYSCOMMAND(hwnd,wParam,lParam); break;



		HANDLE_MSG(hwnd,WM_SIZE,ON_WND_PARENTGRAPH_WM_SIZE);
		HANDLE_MSG(hwnd,WM_COMMAND,ON_WND_PARENTGRAPH_WM_COMMAND);
		HANDLE_MSG(hwnd,WM_DROPFILES,ON_WND_PARENTGRAPH_WM_DROPFILES);
		HANDLE_MSG(hwnd,WM_PAINT,ON_WND_PARENTGRAPH_WM_PAINT);
		HANDLE_MSG(hwnd,WM_CREATE,ON_WND_PARENTGRAPH_WM_CREATE);
		HANDLE_MSG(hwnd,WM_DESTROY,ON_WND_PARENTGRAPH_WM_DESTROY);
		HANDLE_MSG(hwnd,WM_CLOSE,ON_WND_PARENTGRAPH_WM_CLOSE);
		HANDLE_MSG(hwnd,WM_GETMINMAXINFO,ON_WND_PARENTGRAPH_WM_GETMINMAXINFO);
		HANDLE_MSG(hwnd,WM_CHAR,ON_WND_PARENTGRAPH_WM_CHAR);
		HANDLE_MSG(hwnd,WM_KEYDOWN,ON_WND_PARENTGRAPH_WM_KEY);
		HANDLE_MSG(hwnd,WM_SETFOCUS,ON_WND_PARENTGRAPH_WM_SETFOCUS);
		HANDLE_MSG(hwnd,WM_KILLFOCUS,ON_WND_PARENTGRAPH_WM_KILLFOCUS);
	}
	return DefWindowProc (hwnd, message, wParam, lParam);
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_PARENTGRAPH_WM_CLOSE(HWND hwnd)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	SendMessage (ScilabGC->CWindow, WM_CLOSE, 0, 0);
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_PARENTGRAPH_WM_DESTROY(HWND hwnd)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	SendMessage (ScilabGC->CWindow, WM_DESTROY, 0, 0);
	DragAcceptFiles (hwnd, FALSE);
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_PARENTGRAPH_WM_CREATE(HWND hwnd,LPCREATESTRUCT lpCreateStruct)
{
	struct BCG *ScilabGC = NULL;
	ScilabGC = lpCreateStruct->lpCreateParams;
	SetWindowLong (hwnd, 0, (LONG) ScilabGC);
	ScilabGC->hWndParent = hwnd;
	if (ScilabGC->lpgw->lptw) DragAcceptFiles (hwnd, TRUE);
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_PARENTGRAPH_WM_PAINT(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc;

	hdc = BeginPaint (hwnd, &ps);
	EndPaint (hwnd, &ps);
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_PARENTGRAPH_WM_DROPFILES(HWND hwnd,HDROP hDrop)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);

	DragFunc (ScilabGC->lpgw->lptw, hDrop);
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_PARENTGRAPH_WM_COMMAND(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	WPARAM wParam=id;
	LPARAM lParam=(LPARAM)hwndCtl;

	if (IsWindow (ScilabGC->CWindow)) SetFocus (ScilabGC->CWindow);
	SendMessage (ScilabGC->CWindow, WM_COMMAND, wParam, lParam);
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_PARENTGRAPH_WM_SIZE(HWND hwnd,UINT state,int cx,int cy)
{
	RECT rect, rect1, rrect, rrect1;
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);

	GetWindowRect (ScilabGC->Statusbar, &rect);
	SetWindowPos (ScilabGC->Statusbar, (HWND) NULL, 0,cy - (rect.bottom - rect.top),
		cx, (rect.bottom - rect.top),
		SWP_NOZORDER | SWP_NOACTIVATE);

	switch (state) /* Maximized case . F.leray 01.07.04 */
	{
		case SIZE_MAXIMIZED:
			GetWindowRect (ScilabGC->Statusbar, &rrect1);
			GetClientRect (ScilabGC->hWndParent, &rrect);
			ScilabGC->CWindowWidthView = rrect.right;
			ScilabGC->CWindowHeightView = rrect.bottom - (rrect1.bottom - rrect1.top);
			if (ScilabGC->CurResizeStatus != 0)
			{
				ScilabGC->CWindowWidth = ScilabGC->CWindowWidthView;
				ScilabGC->CWindowHeight = ScilabGC->CWindowHeightView;
			}
		break;

		default:
		break;
	}

	SetWindowPos (ScilabGC->CWindow, (HWND) NULL, 0,0,
		cx,cy - (rect.bottom - rect.top),
		SWP_NOZORDER | SWP_NOACTIVATE);

	GetWindowRect (ScilabGC->Statusbar, &rect1);
	GetClientRect (ScilabGC->hWndParent, &rect);
	ScilabGC->CWindowWidthView = rect.right;
	ScilabGC->CWindowHeightView = rect.bottom - (rect1.bottom - rect1.top);
	/* MAJ D.ABDEMOUCHE*/
	if (ScilabGC->CurResizeStatus != 0)
	{
		ScilabGC->CWindowWidth = ScilabGC->CWindowWidthView;
		ScilabGC->CWindowHeight = ScilabGC->CWindowHeightView;
	}
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
void ON_WND_PARENTGRAPH_WM_EXITSIZEMOVE(HWND hwnd)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	ScilabGC->in_sizemove=0;
	InvalidateRect (ScilabGC->CWindow, (LPRECT) NULL, FALSE);
}
/*-----------------------------------------------------------------------------------*/
void ON_WND_PARENTGRAPH_WM_ENTERSIZEMOVE(HWND hwnd)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	/* set  ScilabGC->in_sizemove to zero if you want graphics 
	* redraw during resizing or to one to prevent redraw while sizing.
	*/

	ScilabGC->in_sizemove=0;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_PARENTGRAPH_WM_GETMINMAXINFO(HWND hwnd, LPMINMAXINFO lpMinMaxInfo)
{
	POINT *MMinfo = (POINT *) lpMinMaxInfo;
	/* minimum size */
	/* same minimum size as Linux */
	MMinfo[3].x = 0;
	MMinfo[3].y = 10;
	
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
void ON_WND_PARENTGRAPH_WM_CHAR(HWND hwnd, TCHAR ch, int cRepeat)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	SendMessage (ScilabGC->CWindow, WM_CHAR, ch, MAKELPARAM((cRepeat),0));
}
/*-----------------------------------------------------------------------------------*/
void ON_WND_PARENTGRAPH_WM_KEY(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);

	if ( fDown) SendMessage (ScilabGC->CWindow, WM_KEYDOWN,(WPARAM)(UINT)(vk), MAKELPARAM((cRepeat), (flags)));
	else SendMessage (ScilabGC->CWindow, WM_KEYUP,(WPARAM)(UINT)(vk), MAKELPARAM((cRepeat), (flags)));
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_PARENTGRAPH_WM_SETFOCUS(HWND hwnd, HWND hwndOldFocus)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	extern BOOL focushaschangedBorder;
	extern BOOL focushaschangedViewport;

	focushaschangedBorder = TRUE;
	focushaschangedViewport = FALSE;

	SetFocus (ScilabGC->CWindow);

	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_PARENTGRAPH_WM_SYSCOMMAND(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD (wParam))
	{
		case M_COPY_CLIP:
		case M_PASTE:
		case M_WRITEINI:
		case M_ABOUT:
			{
				struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
				SendMessage (ScilabGC->CWindow, WM_COMMAND,wParam, lParam);
			}
		break;
	}
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
void ON_WND_PARENTGRAPH_WM_KILLFOCUS(HWND hwnd, HWND hwndNewFocus)
{
	SendMessage (textwin.hWndText, WM_KILLFOCUS, (WPARAM)(HWND)(hwndNewFocus), 0L);
}
/*-----------------------------------------------------------------------------------*/
