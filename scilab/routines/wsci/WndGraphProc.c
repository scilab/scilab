/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "WndGraphProc.h"
/*-----------------------------------------------------------------------------------*/
extern int GetEventKeyboardAndMouse(  UINT message, WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC);
extern void SendGraphMacro (struct BCG *ScilabGC, UINT m);
extern int StoreCommand ( char *command);
extern void ShowGraphToolBar(struct BCG * ScilabGC);
extern void HideGraphToolBar(struct BCG * ScilabGC);
extern void ScilabPaint (HWND hwnd, struct BCG *ScilabGC);
extern int sciGetScrollInfo(struct BCG *Scilabgc, int sb_ctl, SCROLLINFO *si);
extern int sciSetScrollInfo(struct BCG *Scilabgc, int sb_ctl, SCROLLINFO *si, BOOLEAN bRedraw);
extern int ScilabGResize (HWND hwnd, struct BCG *ScilabGC, WPARAM wParam);
extern void DragFunc (LPTW lptw, HDROP hdrop);
extern int C2F (deletewin) (integer * number);
extern int PushClickQueue(int win,int x,int y,int ibut,int motion,int release) ;
extern void set_wait_click(val);
extern BOOL SendMacroEntityPicker(struct BCG * ScilabGC,int id);
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_GRAPH_WM_SETFOCUS(HWND hwnd, HWND hwndOldFocus);
BOOL ON_WND_GRAPH_WM_CLOSE(HWND hwnd);
BOOL ON_WND_GRAPH_WM_DESTROY(HWND hwnd);
BOOL ON_WND_GRAPH_WM_DROPFILES(HWND hwnd,HDROP hDrop);
void ON_WND_GRAPH_WM_VSCROLL(HWND hwnd, HWND hwndCtl, UINT code, int pos);
void ON_WND_GRAPH_WM_HSCROLL(HWND hwnd, HWND hwndCtl, UINT code, int pos);
BOOL ON_WND_GRAPH_WM_SIZE(HWND hwnd,UINT state,int cx,int cy);
BOOL ON_WND_GRAPH_WM_PAINT(HWND hwnd);
BOOL ON_WND_GRAPH_WM_CREATE(HWND hwnd,LPCREATESTRUCT lpCreateStruct);
void ON_WND_GRAPH_WM_KEY(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
BOOL ON_WND_GRAPH_WM_COMMAND(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
/*-----------------------------------------------------------------------------------*/
/* The Event Handler for the graphic windows  */
EXPORT LRESULT CALLBACK WndGraphProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	struct BCG *ScilabGC = (struct BCG *) NULL;
	ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);

	GetEventKeyboardAndMouse(message,wParam,lParam,ScilabGC);

	switch (message)
	{
		HANDLE_MSG(hwnd,WM_COMMAND,ON_WND_GRAPH_WM_COMMAND);
		HANDLE_MSG(hwnd,WM_KEYDOWN,ON_WND_GRAPH_WM_KEY);
		HANDLE_MSG(hwnd,WM_CREATE,ON_WND_GRAPH_WM_CREATE);
		HANDLE_MSG(hwnd,WM_PAINT,ON_WND_GRAPH_WM_PAINT);
		HANDLE_MSG(hwnd,WM_SIZE,ON_WND_GRAPH_WM_SIZE);
		HANDLE_MSG(hwnd,WM_HSCROLL,ON_WND_GRAPH_WM_HSCROLL);
		HANDLE_MSG(hwnd,WM_VSCROLL,ON_WND_GRAPH_WM_VSCROLL);
		HANDLE_MSG(hwnd,WM_DROPFILES,ON_WND_GRAPH_WM_DROPFILES);
		HANDLE_MSG(hwnd,WM_DESTROY,ON_WND_GRAPH_WM_DESTROY);
		HANDLE_MSG(hwnd,WM_CLOSE,ON_WND_GRAPH_WM_CLOSE);
		HANDLE_MSG(hwnd,WM_SETFOCUS,ON_WND_GRAPH_WM_SETFOCUS);
	}
	return DefWindowProc (hwnd, message, wParam, lParam);
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_GRAPH_WM_CLOSE(HWND hwnd)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	int curWindowNum = ScilabGC->CurWindow ;

	PostQuitMessage (0);
	C2F (deletewin) (&(curWindowNum));
	SetWindowLong (hwnd, 0, (LONG) 0L);

	PushClickQueue (curWindowNum,0,0,-100, 0, -1);
	set_wait_click(0);

	/* Ajout pour probleme fermeture fenetre scicos
	si une boite de dialogue TK est presente */
	TextMessage1 (0);

	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_GRAPH_WM_DESTROY(HWND hwnd)
{
	PostQuitMessage (0);
	DragAcceptFiles (hwnd, FALSE);
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_GRAPH_WM_DROPFILES(HWND hwnd,HDROP hDrop)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	if (ScilabGC->lpgw->lptw) DragFunc (ScilabGC->lpgw->lptw, hDrop);
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
void ON_WND_GRAPH_WM_VSCROLL(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	SCROLLINFO vertsi;
	int deltay = 0;
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	/* initialisation de SCROLLINFs */
	sciGetScrollInfo (ScilabGC, SB_VERT, &vertsi);
	switch (code)
	{
		/* deltay = 0; */
	case SB_PAGEUP:
		deltay = vertsi.nPos;
		vertsi.nPos = max (vertsi.nMin, vertsi.nPos - 50);
		deltay = deltay - vertsi.nPos;
		break;
	case SB_PAGEDOWN:
		deltay = vertsi.nPos;
		vertsi.nPos = min (vertsi.nMax - (int) vertsi.nPage, vertsi.nPos +50);
		deltay = deltay - vertsi.nPos;
		break;
	case SB_LINEUP:
		deltay = vertsi.nPos;
		vertsi.nPos = max (vertsi.nMin, vertsi.nPos - 5);
		deltay = deltay - vertsi.nPos;
		break;
	case SB_LINEDOWN:
		deltay = vertsi.nPos;
		vertsi.nPos = min (vertsi.nMax - (int) vertsi.nPage, vertsi.nPos +5);
		deltay = deltay - vertsi.nPos;
		break;
	case SB_THUMBTRACK:
		deltay = vertsi.nPos;
		vertsi.nPos = min (vertsi.nMax, max (vertsi.nMin, pos));
		deltay = deltay - vertsi.nPos;
		break; 
	default:
		deltay = 0;
		break;
	}
	sciSetScrollInfo (ScilabGC, SB_VERT, &vertsi, TRUE);
	InvalidateRect (ScilabGC->CWindow, (LPRECT) NULL, FALSE);
	/* UpdateWindow (ScilabGC->CWindow); */ 

}
/*-----------------------------------------------------------------------------------*/
void ON_WND_GRAPH_WM_HSCROLL(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	SCROLLINFO horzsi;
	int deltax = 0;

	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	/* initialisation de SCROLLINFOs */
	sciGetScrollInfo (ScilabGC, SB_HORZ, &horzsi);
	switch (code)
	{
		/* deltax = 0;*/
	case SB_PAGEUP:
		deltax = horzsi.nPos;
		horzsi.nPos = max (horzsi.nMin, horzsi.nPos - 50);
		deltax = deltax - horzsi.nPos;
		break;
	case SB_PAGEDOWN:
		deltax = horzsi.nPos;
		horzsi.nPos = min (horzsi.nMax - (int)horzsi.nPage, horzsi.nPos + 50);
		deltax = deltax - horzsi.nPos;
		break;
	case SB_LINEUP:
		deltax = horzsi.nPos;
		horzsi.nPos = max (horzsi.nMin, horzsi.nPos - 5);
		deltax = deltax - horzsi.nPos;
		break;
	case SB_LINEDOWN:
		deltax = horzsi.nPos;
		horzsi.nPos = min (horzsi.nMax - (int) horzsi.nPage, horzsi.nPos + 5);
		deltax = deltax - horzsi.nPos;
		break;
	case SB_THUMBTRACK:
		deltax = horzsi.nPos;
		horzsi.nPos = max (horzsi.nMin, min (horzsi.nMax,pos));
		deltax = deltax - horzsi.nPos;
		break;
	default:
		deltax=0;
		break;
	}
	sciSetScrollInfo (ScilabGC, SB_HORZ, &horzsi, TRUE);
	InvalidateRect (ScilabGC->CWindow, (LPRECT) NULL, FALSE);

}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_GRAPH_WM_SIZE(HWND hwnd,UINT state,int cx,int cy)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	int deltax = 0;
	int deltay = 0;
	SCROLLINFO vertsi;
	SCROLLINFO horzsi;

	/* initialisation de SCROLLINFs */
	sciGetScrollInfo (ScilabGC, SB_HORZ, &horzsi);
	sciGetScrollInfo (ScilabGC, SB_VERT, &vertsi);
	/* MAJ D.ABDEMOUCHE*/
	if (ScilabGC->CurResizeStatus == 0)
	{
		horzsi.nPage = cx;
		vertsi.nPage = cy;
		/* on recupere la veritable position des scroll bar
		* on a ainsi la possibilite de tirer le graphe
		* quand on agrandi la fenetre 
		*/
		horzsi.nPos = GetScrollPos (ScilabGC->CWindow, SB_HORZ);
		vertsi.nPos = GetScrollPos (ScilabGC->CWindow, SB_VERT);
	}
	else
	{
		/* eventually resize the pixmap size */
		if (  ScilabGC->CurPixmapStatus  == 1 )
		{
			ScilabGResize (hwnd, ScilabGC, state); 
		}
		horzsi.nMax = cx;
		vertsi.nMax = cy;
		horzsi.nPage = horzsi.nMax;
		vertsi.nPage = vertsi.nMax;
	}
	sciSetScrollInfo (ScilabGC, SB_HORZ, &(horzsi), TRUE);
	sciSetScrollInfo (ScilabGC, SB_VERT, &(vertsi), TRUE);
	/* on force le reclacule les positions des scrollbars
	et leur validation */
	sciGetScrollInfo (ScilabGC, SB_HORZ, &horzsi);
	sciGetScrollInfo (ScilabGC, SB_VERT, &vertsi);
	InvalidateRect (ScilabGC->CWindow, (LPRECT) NULL,FALSE);

	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_GRAPH_WM_PAINT(HWND hwnd)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	ScilabPaint (hwnd, ScilabGC);
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_GRAPH_WM_CREATE(HWND hwnd,LPCREATESTRUCT lpCreateStruct)
{
	struct BCG *ScilabGC = lpCreateStruct->lpCreateParams;

	SetWindowLong (hwnd, 0, (LONG) ScilabGC);
	ScilabGC->CWindow = hwnd;
	if (ScilabGC->lpgw->lptw) DragAcceptFiles (hwnd, TRUE);

	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
void ON_WND_GRAPH_WM_KEY(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	switch (vk)
	{
		case VK_F3:
		{
			struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
			if (ScilabGC->lpmw.LockToolBar == FALSE)
			{
				if (ScilabGC->lpmw.ShowToolBar)	HideGraphToolBar(ScilabGC);
				else ShowGraphToolBar(ScilabGC);
			}

		}
		break;

		default:
		break;
	}
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_GRAPH_WM_COMMAND(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);

	if (id < NUMMENU) 
	{
		if (!SendMacroEntityPicker(ScilabGC, id)) SendGraphMacro (ScilabGC, id);
	}
	else
	{
		switch (id)
		{
			case TOOLBAR_ROTATE3D:
				scig_3drot (ScilabGC->CurWindow);
			break;

			case TOOLBAR_ZOOM:
				scig_2dzoom (ScilabGC->CurWindow);
			break;

			case TOOLBAR_UNZOOM:
				scig_unzoom (ScilabGC->CurWindow);
			break;

			case TOOLBAR_GED:
			{
				if (ScilabGC->graphicsversion == 0)
				{
					char command[1024];
					wsprintf(command,"ged(4,%d);",ScilabGC->CurWindow);
					StoreCommand(command);
				}
			}
			break;

			case TOOLBAR_PICKER:
			{
				LRESULT lResult;

				HWND hwndPicker=NULL;
				hwndPicker=GetDlgItem(ScilabGC->CWindow,TOOLBAR_PICKER);

				lResult=SendMessage(hwndPicker,(UINT) BM_GETCHECK,0,0);  

				if (lResult == BST_CHECKED)
				{
					char command[1024];
					wsprintf(command,"ged(6,%d);",ScilabGC->CurWindow);
					StoreCommand(command);
					
				}
				else
				{
					char command[1024];
					wsprintf(command,"ged(7,%d);",ScilabGC->CurWindow);
					StoreCommand(command);
				}
				
			}
			break;
/*
			case TOOLBAR_LINE:
			{
				if (ScilabGC->graphicsversion == 0)
				{
					char command[1024];
					wsprintf(command,"ged_insert(1,%d);",ScilabGC->CurWindow);
					StoreCommand(command);
				}
			}
			break;

			case TOOLBAR_POLYLINE:
				{
				if (ScilabGC->graphicsversion == 0)
					{
					char command[1024];
					wsprintf(command,"ged_insert(2,%d);",ScilabGC->CurWindow);
					StoreCommand(command);
					}
				}
			break;

			case TOOLBAR_ARROW:
			{
				if (ScilabGC->graphicsversion == 0)
				{
					char command[1024];
					wsprintf(command,"ged_insert(3,%d);",ScilabGC->CurWindow);
					StoreCommand(command);
				}
			}
			break;

			case TOOLBAR_DOUBLEARROW:
			{
				if (ScilabGC->graphicsversion == 0)
				{
					char command[1024];
					wsprintf(command,"ged_insert(4,%d);",ScilabGC->CurWindow);
					StoreCommand(command);
				}
			}
			break;
	
			case TOOLBAR_TEXT:
			{
				if (ScilabGC->graphicsversion == 0)
				{
					char command[1024];
					wsprintf(command,"ged_insert(4,%d);",ScilabGC->CurWindow);
					StoreCommand(command);
				}
			}
			break;

			case TOOLBAR_RECTANGLE:
			{
				if (ScilabGC->graphicsversion == 0)
				{
					char command[1024];
					wsprintf(command,"ged_insert(5,%d);",ScilabGC->CurWindow);
					StoreCommand(command);
				}
			}
			break;

			case TOOLBAR_CIRCLE:
			{
				if (ScilabGC->graphicsversion == 0)
				{
					char command[1024];
					wsprintf(command,"ged_insert(6,%d);",ScilabGC->CurWindow);
					StoreCommand(command);
				}
			}
			break; */
		}
		SetFocus(ScilabGC->CWindow);
		SetActiveWindow(ScilabGC->CWindow);
	}
		

	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ON_WND_GRAPH_WM_SETFOCUS(HWND hwnd, HWND hwndOldFocus)
{
	extern BOOL focushaschangedViewport;
	extern BOOL focushaschangedBorder;

	
	if (focushaschangedBorder) 
	{
		focushaschangedViewport  = TRUE;
		focushaschangedBorder = TRUE;
	}
	
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
