/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "WndConsoleParentProc.h"
#include "wtext.h"
#include "../../../../windows_tools/src/c/WinConsole.h"
/*-----------------------------------------------------------------------------------*/
extern LPTW GetTextWinScilab(void);
extern char *GetScilabDirectory(BOOL UnixStyle);
extern BOOL IsWindowInterface(void);
extern void SwitchConsole(void);
extern void SaveCurrentLine(BOOL RewriteLineAtPrompt);
extern void WriteRegistryTxt (LPTW lptw);
extern void SendCTRLandAKey(int code);
extern void WriteIntoKeyBuffer(LPTW lptw,char *StringCommand);
extern void InitIhmDefaultColor(void);
extern BOOL SetIhmSystemDefaultTextBackgroundColor(void);
extern BOOL ReceiveFromAnotherScilab(HWND hWndSend,PCOPYDATASTRUCT MyCopyDataStruct);
extern BOOL GetCommandFromAnotherScilab(char *TitleWindowSend,char *CommandLine);
extern BOOL ActivateTransparencyMode(HWND hWnd);
extern BOOL SetIhmSystemDefaultTextColor(void);
extern char *getCopyCurrentLine(void);
/*-----------------------------------------------------------------------------------*/
extern POINT ScreenMinSize;
extern char ScilexWindowName[PATH_MAX];
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_CONSOLE_WM_COPYDATA(HWND hwnd,HWND hWndSend,PCOPYDATASTRUCT MyCopyDataStruct);
BOOL ON_WND_CONSOLE_WM_CLOSE(HWND hwnd);
BOOL ON_WND_CONSOLE_WM_DESTROY(HWND hwnd);
BOOL ON_WND_CONSOLE_WM_CREATE(HWND hwnd,LPCREATESTRUCT lpCreateStruct);
BOOL ON_WND_CONSOLE_WM_DROPFILES(HWND hwnd,HDROP hDrop);
BOOL ON_WND_CONSOLE_WM_PAINT(HWND hwnd);
BOOL ON_WND_CONSOLE_WM_SYSCOLORCHANGE(HWND hwnd);
BOOL ON_WND_CONSOLE_WM_COMMAND(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
BOOL ON_WND_CONSOLE_WM_EXITSIZEMOVE(void);
BOOL ON_WND_CONSOLE_WM_ENTERSIZEMOVE(void);
BOOL ON_WND_CONSOLE_WM_SIZE(HWND hwnd,UINT state,int cx,int cy);
BOOL ON_WND_CONSOLE_WM_GETMINMAXINFO(HWND hwnd, LPMINMAXINFO lpMinMaxInfo);
BOOL ON_WND_CONSOLE_WM_SETFOCUS(HWND hwnd, HWND hwndOldFocus);
BOOL ON_WND_CONSOLE_WM_SYSCOMMAND(HWND hwnd, WPARAM wParam, LPARAM lParam);
BOOL ON_WND_CONSOLE_WM_ENDSESSION(HWND hwnd, WPARAM wParam, LPARAM lParam);
/*-----------------------------------------------------------------------------------*/
#if (_MSC_VER >= 1200) && (_MSC_VER < 1300) /* Visual Studio 6 */

#ifndef HANDLE_WM_COPYDATA

#define HANDLE_WM_COPYDATA(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (HWND)(wParam), (PCOPYDATASTRUCT)lParam), 0L)
#endif    
    
#ifndef HANDLE_WM_MOUSEWHEEL   
#define HANDLE_WM_MOUSEWHEEL(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (int)(short)HIWORD(wParam), (UINT)(short)LOWORD(wParam)), 0L)
#endif  

#endif
/*-----------------------------------------------------------------------------------*/
/* parent overlapped window */
EXPORT LRESULT CALLBACK WndParentProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
    {
		case WM_ENTERSIZEMOVE : ON_WND_CONSOLE_WM_ENTERSIZEMOVE();  break;
		case WM_EXITSIZEMOVE  : ON_WND_CONSOLE_WM_EXITSIZEMOVE();  break;
		case WM_SYSCOMMAND    : ON_WND_CONSOLE_WM_SYSCOMMAND(hwnd,wParam,lParam); break;
		case WM_ENDSESSION    : ON_WND_CONSOLE_WM_ENDSESSION(hwnd,wParam,lParam); break;

		HANDLE_MSG(hwnd,WM_COMMAND,ON_WND_CONSOLE_WM_COMMAND);
		HANDLE_MSG(hwnd,WM_SYSCOLORCHANGE,ON_WND_CONSOLE_WM_SYSCOLORCHANGE);
		HANDLE_MSG(hwnd,WM_PAINT,ON_WND_CONSOLE_WM_PAINT);
		HANDLE_MSG(hwnd,WM_DROPFILES,ON_WND_CONSOLE_WM_DROPFILES);
		HANDLE_MSG(hwnd,WM_CREATE,ON_WND_CONSOLE_WM_CREATE);
		HANDLE_MSG(hwnd,WM_DESTROY,ON_WND_CONSOLE_WM_DESTROY);
		HANDLE_MSG(hwnd,WM_CLOSE,ON_WND_CONSOLE_WM_CLOSE);
		HANDLE_MSG(hwnd,WM_COPYDATA,ON_WND_CONSOLE_WM_COPYDATA);
		HANDLE_MSG(hwnd,WM_SIZE,ON_WND_CONSOLE_WM_SIZE);
		HANDLE_MSG(hwnd,WM_GETMINMAXINFO,ON_WND_CONSOLE_WM_GETMINMAXINFO);
		HANDLE_MSG(hwnd,WM_SETFOCUS,ON_WND_CONSOLE_WM_SETFOCUS);
	}
  return DefWindowProc (hwnd, message, wParam, lParam);
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_CONSOLE_WM_COPYDATA(HWND hwnd,HWND hWndSend,PCOPYDATASTRUCT MyCopyDataStruct)
{
	char Command[PATH_MAX];
	char TitleWndSend[PATH_MAX];

	ReceiveFromAnotherScilab(hWndSend,MyCopyDataStruct);

	if ( GetCommandFromAnotherScilab(TitleWndSend,Command) )StoreCommand1 (Command, 0);

	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_CONSOLE_WM_CLOSE(HWND hwnd)
{
	LPTW lptw=GetTextWinScilab();
	SendMessage(lptw->hWndText,WM_CLOSE,(WPARAM)lptw->hWndParent,0);
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_CONSOLE_WM_DESTROY(HWND hwnd)
{
	LPTW lptw=GetTextWinScilab();

	DragAcceptFiles (hwnd, FALSE);
	DeleteFont (lptw->hfont);
	lptw->hfont = 0;

	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_CONSOLE_WM_CREATE(HWND hwnd,LPCREATESTRUCT lpCreateStruct)
{
	LPTW lptw = lpCreateStruct->lpCreateParams;

	SetWindowLong (hwnd, 0, (LONG) lptw);
	lptw->hWndParent = hwnd;
	/* get character size */

	DragAcceptFiles (hwnd, TRUE);

	/* Modification Allan CORNET 15/07/03 */
	/* Renomme la fenetre avec SCI_VERSION_STRING et numero x associé à la console*/  
	{
		char CopyNameConsole[PATH_MAX];
		char *ScilexConsoleName = NULL;
		char *FirstOccurence;
		char *SecondOccurence;
		
		ScilexConsoleName = getScilexConsoleName();

		if (ScilexConsoleName)
		{
			strcpy(CopyNameConsole,ScilexConsoleName);
			strcpy(CopyNameConsole,ScilexConsoleName);
			FirstOccurence = strtok(CopyNameConsole,"("); 
			SecondOccurence= strtok(NULL,"("); 
			wsprintf(ScilexWindowName,"%s (%s",SCI_VERSION_STRING,SecondOccurence);

			SetWindowText(hwnd,ScilexWindowName);  
			FREE(ScilexConsoleName);
			ScilexConsoleName = NULL;
		}

		
	}
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_CONSOLE_WM_DROPFILES(HWND hwnd,HDROP hDrop)
{
	LPTW lptw=GetTextWinScilab();
	DragFunc (lptw,hDrop);
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_CONSOLE_WM_PAINT(HWND hwnd)
{
	LPTW lptw=GetTextWinScilab();
	RECT rect;
	PAINTSTRUCT ps;
	HDC hdc;

	hdc = BeginPaint (hwnd, &ps);
	GetClientRect (hwnd, &rect);
	FillRect (hdc, &rect, lptw->hbrBackground);

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
	
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_CONSOLE_WM_SYSCOLORCHANGE(HWND hwnd)
{
	LPTW lptw=GetTextWinScilab();

	if (lptw->bSysColors)
	{
		InitIhmDefaultColor();
		SetIhmSystemDefaultTextBackgroundColor();
		SetIhmSystemDefaultTextColor();
	}
	SendMessage (lptw->hWndText, WM_SYSCOLORCHANGE, (WPARAM) 0, (LPARAM) 0);

	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_CONSOLE_WM_COMMAND(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	LPTW lptw=GetTextWinScilab();

	WPARAM wParam=id;
	LPARAM lParam=(LPARAM)hwndCtl;

	if ( IsWindow (lptw->hWndText) ) SetFocus (lptw->hWndText);

	/* pass on menu commands */
	SendMessage (lptw->hWndText, WM_COMMAND, wParam,lParam);
	
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_CONSOLE_WM_EXITSIZEMOVE(void)
{
	LPTW lptw=GetTextWinScilab();
	char *copycur_line = NULL;

	WriteRegistryTxt(lptw); /* Sauvegarde Position apres deplacement et redimensionnement */
	SendCTRLandAKey(CTRLU); /* Scrollbar */
	copycur_line=getCopyCurrentLine();
	if (copycur_line)
	{
		WriteIntoKeyBuffer(lptw,copycur_line);
		FREE(copycur_line);
		copycur_line = NULL;
	}
	
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_CONSOLE_WM_ENTERSIZEMOVE(void)
{
	SaveCurrentLine(FALSE);
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_CONSOLE_WM_SIZE(HWND hwnd,UINT state,int cx,int cy)
{
	LPTW lptw=GetTextWinScilab();
	
	if (lptw->lpmw->ShowToolBar)
	{
		/* Affichage Zone Toolbar */
		SetWindowPos (lptw->hWndText, (HWND) NULL, 0, lptw->ButtonHeight,cx, cy - lptw->ButtonHeight,SWP_NOZORDER | SWP_NOACTIVATE);
	}
	else
	{
		/* Pas de zone Toolbar */
		SetWindowPos (lptw->hWndText, (HWND) NULL, 0, 0,cx,cy,SWP_NOZORDER | SWP_NOACTIVATE);

	}	   
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_CONSOLE_WM_GETMINMAXINFO(HWND hwnd, LPMINMAXINFO lpMinMaxInfo)
{
	POINT *MMinfo = (POINT *) lpMinMaxInfo;
	/* minimum size */
	/* same minimum size as Linux */
	MMinfo[3].x = 400;
	MMinfo[3].y =320;
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_CONSOLE_WM_SETFOCUS(HWND hwnd, HWND hwndOldFocus)
{
	LPTW lptw=GetTextWinScilab();
	if (IsWindow (lptw->hWndText))	SetFocus (lptw->hWndText);
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_CONSOLE_WM_SYSCOMMAND(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LPTW lptw=GetTextWinScilab();
	switch (LOWORD (wParam))
	{
		case M_CONSOLE:
		{
			SwitchConsole();
			SetActiveWindow(hwnd);
		}
		break;
		
		case M_ABOUT:
			SendMessage (lptw->hWndText, WM_COMMAND, wParam, lParam);
		break;
	}
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_CONSOLE_WM_ENDSESSION(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
