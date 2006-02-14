/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "Toolbar.h"
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/
static BOOL DefaultShowToolBar=TRUE;
/*-----------------------------------------------------------------------------------*/
extern GW graphwin;
extern BOOL IsWindowInterface(void);
extern LPTW GetTextWinScilab(void);
/*-----------------------------------------------------------------------------------*/
#define ToolBarHeight 24
#define ButtonToolBarWeight 24
/*-----------------------------------------------------------------------------------*/
void SetDefaultShowToolBar(BOOL valShowToolBar)
{
	DefaultShowToolBar=valShowToolBar;
}
/*-----------------------------------------------------------------------------------*/
void HideGraphToolBar(struct BCG * ScilabGC)
{
	int i=0;
	ScilabGC->lpmw.ShowToolBar=FALSE;

	for (i=0;i<ScilabGC->lpmw.nButton;i++)
	{
		ShowWindow(ScilabGC->lpmw.hButton[i],SW_HIDE);
	}
}
/*-----------------------------------------------------------------------------------*/
void ShowGraphToolBar(struct BCG * ScilabGC)
{
	int i=0;

	ScilabGC->lpmw.ShowToolBar=TRUE;
	for (i=0;i<ScilabGC->lpmw.nButton;i++)
	{
		ShowWindow(ScilabGC->lpmw.hButton[i],SW_SHOWNORMAL);
	}

	if (ScilabGC->graphicsversion!=0)  ShowWindow(ScilabGC->lpmw.hButton[ScilabGC->lpmw.nButton-1],SW_HIDE);
}
/*-----------------------------------------------------------------------------------*/
void CreateGraphToolBar(struct BCG * ScilabGC) 
{
	ScilabGC->lpmw.nButton=0;

	CreateButtonToolBar(ScilabGC,MSG_SCIMSG14,(HMENU)TOOLBAR_ZOOM,IDI_ZOOM,MSG_SCIMSG14,WS_CHILD|WS_VISIBLE|BS_ICON);
	CreateButtonToolBar(ScilabGC,MSG_SCIMSG15,(HMENU)TOOLBAR_UNZOOM,IDI_UNZOOM,MSG_SCIMSG15,WS_CHILD|WS_VISIBLE|BS_ICON);
	CreateButtonToolBar(ScilabGC,MSG_SCIMSG16,(HMENU)TOOLBAR_ROTATE3D,IDI_3DROT,MSG_SCIMSG16,WS_CHILD|WS_VISIBLE|BS_ICON);
	CreateButtonToolBar(ScilabGC,MSG_SCIMSG106,(HMENU)TOOLBAR_GED,IDI_GED,MSG_SCIMSG106,WS_CHILD|WS_VISIBLE|BS_ICON);
	CreateButtonToolBar(ScilabGC,MSG_SCIMSG113,(HMENU)TOOLBAR_PICKER,IDI_PICKER,MSG_SCIMSG113,WS_CHILD|WS_VISIBLE|BS_ICON|BS_AUTOCHECKBOX|BS_PUSHLIKE);
	/*CreateButtonToolBar(ScilabGC,MSG_SCIMSG107,(HMENU)TOOLBAR_LINE,IDI_LINE,MSG_SCIMSG107,WS_CHILD|WS_VISIBLE|BS_ICON);
	CreateButtonToolBar(ScilabGC,MSG_SCIMSG120,(HMENU)TOOLBAR_POLYLINE,IDI_POLYLINE,MSG_SCIMSG120,WS_CHILD|WS_VISIBLE|BS_ICON);
	CreateButtonToolBar(ScilabGC,MSG_SCIMSG108,(HMENU)TOOLBAR_ARROW,IDI_ARROW,MSG_SCIMSG108,WS_CHILD|WS_VISIBLE|BS_ICON);
	CreateButtonToolBar(ScilabGC,MSG_SCIMSG109,(HMENU)TOOLBAR_DOUBLEARROW,IDI_DOUBLEARROW,MSG_SCIMSG109,WS_CHILD|WS_VISIBLE|BS_ICON);
	CreateButtonToolBar(ScilabGC,MSG_SCIMSG110,(HMENU)TOOLBAR_TEXT,IDI_TEXT,MSG_SCIMSG110,WS_CHILD|WS_VISIBLE|BS_ICON);
	CreateButtonToolBar(ScilabGC,MSG_SCIMSG111,(HMENU)TOOLBAR_RECTANGLE,IDI_RECTANGLE,MSG_SCIMSG111,WS_CHILD|WS_VISIBLE|BS_ICON);
	CreateButtonToolBar(ScilabGC,MSG_SCIMSG112,(HMENU)TOOLBAR_CIRCLE,IDI_CIRCLE,MSG_SCIMSG112,WS_CHILD|WS_VISIBLE|BS_ICON);*/

	ScilabGC->lpmw.LockToolBar=FALSE;

	if (DefaultShowToolBar == FALSE) HideGraphToolBar(ScilabGC);
	else
	{
		ScilabGC->lpmw.ShowToolBar=TRUE;
	}

}
/*-----------------------------------------------------------------------------------*/
void RefreshGraphToolBar(struct BCG * ScilabGC) 
{
	int i=0;
	
	if (ScilabGC->graphicsversion!=0) 
	{
		for (i=3;i<ScilabGC->lpmw.nButton;i++)
		{
			ShowWindow(ScilabGC->lpmw.hButton[i],SW_HIDE);
		}
	}
	else
	{
		if (ScilabGC->lpmw.ShowToolBar)
		{
			for (i=3;i<ScilabGC->lpmw.nButton;i++)
			{
				ShowWindow(ScilabGC->lpmw.hButton[i],SW_SHOWNORMAL);
			}
		}
	}

	for (i=0;i<ScilabGC->lpmw.nButton;i++)
	{
		InvalidateRect(ScilabGC->lpmw.hButton[i],NULL,TRUE);
	}
}
/*-----------------------------------------------------------------------------------*/
void ShowToolBar(LPTW lptw)
{
	int i=0;	      		
	LPMW lpmw;      		
	RECT rect;

	lpmw = lptw->lpmw;

	GetClientRect (lptw->hWndParent, &rect);
	SetWindowPos (lptw->hWndText, (HWND) NULL, 0, lptw->ButtonHeight,
		rect.right, rect.bottom - lptw->ButtonHeight,SWP_NOZORDER | SWP_NOACTIVATE);	
	for (i = 0; i < lpmw->nButton; i++)
	{
		ShowWindow( lpmw->hButton[i] , SW_SHOWNORMAL );
	}

	InvalidateRect(lptw->hWndParent, (LPRECT) NULL, TRUE);
	InvalidateRect(lptw->hWndText, (LPRECT) NULL, TRUE);
	UpdateWindow (lptw->hWndText);
}
/*-----------------------------------------------------------------------------------*/   
void HideToolBar(LPTW lptw)
{
	int i=0;	      		
	LPMW lpmw;      		
	RECT rect;
	lpmw = lptw->lpmw;

	GetClientRect (lptw->hWndParent, &rect);
	SetWindowPos (lptw->hWndText, (HWND) NULL,0,0,
		rect.right, rect.bottom,SWP_NOZORDER | SWP_NOACTIVATE);
	for (i = 0; i < lpmw->nButton; i++)
	{
		ShowWindow( lpmw->hButton[i] , SW_HIDE );
	}
	InvalidateRect(lptw->hWndParent, (LPRECT) NULL, TRUE);
	InvalidateRect(lptw->hWndText, (LPRECT) NULL, TRUE);
	UpdateWindow (lptw->hWndText);

}
/*-----------------------------------------------------------------------------------*/   
void ToolBarOnOff(LPTW lptw)
{
	BOOL ON=lptw->lpmw->ShowToolBar;
	if  (ON)
	{
		ShowToolBar(lptw);
	}
	else
	{
		HideToolBar(lptw);
	}
}
/*-----------------------------------------------------------------------------------*/   
void CreateMyTooltip (HWND hwnd,char ToolTipString[30])
{
	INITCOMMONCONTROLSEX iccex; 
	HWND hwndTT;                 // handle to the ToolTip control

	TOOLINFO ti; // struct specifying info about tool in ToolTip control
	unsigned int uid = 0;       // for ti initialization

	LPTSTR lptstr = ToolTipString;
	RECT rect;                  // for client area coordinates

	/* INITIALIZE COMMON CONTROLS */
	iccex.dwICC = ICC_WIN95_CLASSES;
	iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	InitCommonControlsEx(&iccex);

	/* CREATE A TOOLTIP WINDOW */
	hwndTT = CreateWindowEx(WS_EX_TOPMOST,
		TOOLTIPS_CLASS,
		NULL,
		WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,		
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hwnd,
		NULL,
		NULL,
		NULL
		);

	SetWindowPos(hwndTT,
		HWND_TOPMOST,
		0,
		0,
		0,
		0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	/* GET COORDINATES OF THE MAIN CLIENT AREA */
	GetClientRect (hwnd, &rect);

	/* INITIALIZE MEMBERS OF THE TOOLINFO STRUCTURE */
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_SUBCLASS;
	ti.hwnd = hwnd;
	ti.hinst = NULL;
	ti.uId = uid;
	ti.lpszText = lptstr;
	// ToolTip control will cover the whole window
	ti.rect.left = rect.left;    
	ti.rect.top = rect.top;
	ti.rect.right = rect.right;
	ti.rect.bottom = rect.bottom;

	/* SEND AN ADDTOOL MESSAGE TO THE TOOLTIP CONTROL WINDOW */
	SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);	

} 
/*-----------------------------------------------------------------------------------*/   
int HideToolBarWin32(int WinNum)
{
	if (WinNum == -1)
	{
		if (IsWindowInterface())
		{
			LPTW lptw=GetTextWinScilab();

			HideToolBar(lptw);
			lptw->lpmw->ShowToolBar=FALSE;
			lptw->lpmw->LockToolBar=TRUE;
		}
		else
		{
			sciprint(MSG_WARNING29 );
		}
	}
	else
	{
		struct BCG *ScilabGC=NULL;
		ScilabGC = GetWindowXgcNumber (WinNum);
		if (ScilabGC != (struct BCG *) 0)
		{
			HideGraphToolBar(ScilabGC);
			ScilabGC->lpmw.LockToolBar=TRUE;
		}
	}

	return 0;
}
/*-----------------------------------------------------------------------------------*/
int ToolBarWin32(int WinNum,char *onoff)
{
	int bON;
	if (WinNum == -1)
	{
		if (IsWindowInterface())
		{
			LPTW lptw=GetTextWinScilab();

			if (strcmp(onoff,"off")==0)
			{
				if (!lptw->lpmw->LockToolBar)
				{
					lptw->lpmw->ShowToolBar=FALSE;
					ToolBarOnOff(lptw);
				}
			}

			if (strcmp(onoff,"on")==0)
			{
				if (!lptw->lpmw->LockToolBar)
				{
					lptw->lpmw->ShowToolBar=TRUE;
					ToolBarOnOff(lptw);
				}
			}

			bON=lptw->lpmw->ShowToolBar;
		}
		else
		{
			sciprint(MSG_WARNING29);
			bON=FALSE;
		}
	}
	else
	{
		struct BCG *ScilabGC=NULL;
		ScilabGC = GetWindowXgcNumber (WinNum);
		if (ScilabGC != (struct BCG *) 0)
		{
			if (strcmp(onoff,"off")==0)
			{
				if (!ScilabGC->lpmw.LockToolBar)
				{
					HideGraphToolBar(ScilabGC);
				}

			}

			if (strcmp(onoff,"on")==0)
			{
				if (!ScilabGC->lpmw.LockToolBar)
				{
					ShowGraphToolBar(ScilabGC);
				}
			}

			bON=(ScilabGC->lpmw.ShowToolBar);
		}
		else bON=FALSE;
	}
	return bON;
}
/*-----------------------------------------------------------------------------------*/
int GetStateToolBarWin32(int WinNum)
{
	int bAns;
	if (WinNum == -1)
	{
		if (IsWindowInterface())
		{
			LPTW lptw=GetTextWinScilab();

			bAns=lptw->lpmw->ShowToolBar;
		}
		else
		{
			sciprint(MSG_WARNING29);
			bAns=FALSE;
		}
	}
	else
	{
		struct BCG *ScilabGC=NULL;
		ScilabGC = GetWindowXgcNumber (WinNum);
		if (ScilabGC != (struct BCG *) 0)
		{
			bAns=(ScilabGC->lpmw.ShowToolBar);
		}
		else bAns=FALSE;
	}

	return bAns;
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
void ModifyEntityPickerToolbar(struct BCG * ScilabGC,BOOL Pressed)
{
	HWND hwndPicker=NULL;
	hwndPicker=GetDlgItem(ScilabGC->CWindow,TOOLBAR_PICKER);

	if (Pressed)
	{
		SendMessage(hwndPicker,(UINT) BM_SETCHECK,BST_CHECKED,0);
	}
	else
	{
		SendMessage(hwndPicker,(UINT) BM_SETCHECK,BST_UNCHECKED,0);  
	}
}
/*-----------------------------------------------------------------------------------*/
void CreateButtonToolBar(struct BCG * ScilabGC,char *Name,HMENU CallbackID,int ID_ICON,char *ToolTipString,DWORD Style)
{
	HICON IconButton;
	ScilabGC->lpmw.hButton[ScilabGC->lpmw.nButton]= CreateWindow("button",Name,Style,
																																ButtonToolBarWeight*ScilabGC->lpmw.nButton, 0,
																																ButtonToolBarWeight, ToolBarHeight,
																																ScilabGC->CWindow,(HMENU) CallbackID,
																																graphwin.hInstance, NULL);

  IconButton=(HICON)LoadImage( GetModuleHandle(MSG_SCIMSG9), (LPCSTR)ID_ICON,IMAGE_ICON,ButtonToolBarWeight,ToolBarHeight,LR_DEFAULTCOLOR);
	SendMessage(ScilabGC->lpmw.hButton[ScilabGC->lpmw.nButton],BM_SETIMAGE, IMAGE_ICON, (LPARAM)IconButton);

	CreateMyTooltip (ScilabGC->lpmw.hButton[ScilabGC->lpmw.nButton], ToolTipString); 

	ScilabGC->lpmw.nButton++;
}
/*-----------------------------------------------------------------------------------*/
