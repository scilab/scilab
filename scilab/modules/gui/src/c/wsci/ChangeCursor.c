//#include "WinConsole.h"
#include "wtext.h"
#include "TextWindows.h"

extern int C2F (scilines) (int *nl, int *nc);

extern BOOL ScilabIsStarting;

void ChangeCursorWhenScilabIsReady(void)
{
	if ( (IsWindowInterface()) && (ScilabIsStarting) )
	{
		HCURSOR hCursor;
		LPTW lptw=GetTextWinScilab();
		int NumsMenu = 0;
		int nl=0, nc=0;
		HMENU hMenu=NULL;

		hCursor=LoadCursor(  lptw->hInstance,IDC_ARROW);
		SetClassLong(lptw->hWndParent, GCL_HCURSOR,	(LONG) hCursor); 
		SetClassLong(lptw->hWndText,GCL_HCURSOR,(LONG) hCursor); 
		InvalidateCursor(); 
		hMenu=GetMenu(lptw->hWndParent);
		NumsMenu=GetMenuItemCount (hMenu);
		EnableNMenus(lptw,NumsMenu);
		EnableToolBar(lptw);

		ScilabIsStarting=FALSE;

		ShowWindow (lptw->hWndParent,SW_SHOWDEFAULT);
		BringWindowToTop (lptw->hWndParent);
		SetFocus(lptw->hWndParent);
		SetFocus(lptw->hWndText);

		/* Initialize 'lines' */
		nc = lptw->ClientSize.x / lptw->CharSize.x;
		nl = lptw->ClientSize.y / lptw->CharSize.y;
		/** send number of lines info to scilab **/
		if ((lptw->ClientSize.x>0) && (lptw->ClientSize.y>0))	C2F (scilines) (&nl, &nc);

	}
}