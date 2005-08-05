/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "ihmcolor.h"
#include "win_mem_alloc.h" /* MALLOC */
#include "wgnuplib.h"
/*-----------------------------------------------------------------------------------*/
static DWORD CurrentIHMTextColor=0;
static DWORD CurrentIHMTextBackgroundColor=0;
/*-----------------------------------------------------------------------------------*/
BOOL SetIhmTextColor(int R,int G,int B)
{
	BOOL bOK=FALSE;
	extern char ScilexWindowName[MAX_PATH];
	LPTW lptw;

	lptw = (LPTW) GetWindowLong (FindWindow(NULL,ScilexWindowName), 0);

	CurrentIHMTextColor=RGB(R,G,B);
	if (InvalidateRect (lptw->hWndText, NULL, TRUE)) bOK=TRUE;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL SetIhmTextBackgroundColor(int R,int G,int B)
{
	BOOL bOK=FALSE;
	HBRUSH NewHbrBackground=NULL;
	extern char ScilexWindowName[MAX_PATH];
	LPTW lptw;

	lptw = (LPTW) GetWindowLong (FindWindow(NULL,ScilexWindowName), 0);

	CurrentIHMTextBackgroundColor=RGB(R,G,B);
	NewHbrBackground=CreateSolidBrush (CurrentIHMTextBackgroundColor);

	if (SetClassLong(lptw->hWndParent,GCL_HBRBACKGROUND,(LONG)NewHbrBackground)==0) bOK=FALSE;
	else bOK=TRUE;

	if ((bOK) && (SetClassLong(lptw->hWndText,GCL_HBRBACKGROUND,(LONG)NewHbrBackground)==0)) bOK=FALSE;
	else bOK=TRUE;

	if ( (bOK) && (DeleteObject((HBRUSH)(lptw->hbrBackground)))) bOK=TRUE;
	else bOK=FALSE;

	lptw->hbrBackground=NewHbrBackground;
	if ((bOK) && (InvalidateRect (lptw->hWndText, NULL, TRUE))) bOK=TRUE;
	else bOK=FALSE;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL SetIhmSystemDefaultTextColor(void)
{
	BOOL bOK=FALSE;
	extern char ScilexWindowName[MAX_PATH];
	LPTW lptw;

	lptw = (LPTW) GetWindowLong (FindWindow(NULL,ScilexWindowName), 0);

	CurrentIHMTextColor=GetSysColor(COLOR_WINDOWTEXT);
	if (InvalidateRect (lptw->hWndText, NULL, TRUE)) bOK=TRUE;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL SetIhmSystemDefaultTextBackgroundColor(void)
{
	BOOL bOK=FALSE;

	HBRUSH NewHbrBackground=NULL;
	extern char ScilexWindowName[MAX_PATH];
	LPTW lptw;
	lptw = (LPTW) GetWindowLong (FindWindow(NULL,ScilexWindowName), 0);
	
	CurrentIHMTextBackgroundColor=GetSysColor(COLOR_WINDOW);

	NewHbrBackground=CreateSolidBrush (CurrentIHMTextBackgroundColor);

	if (SetClassLong(lptw->hWndParent,GCL_HBRBACKGROUND,(LONG)NewHbrBackground)==0) bOK=FALSE;
	else bOK=TRUE;

	if ((bOK) && (SetClassLong(lptw->hWndText,GCL_HBRBACKGROUND,(LONG)NewHbrBackground)==0)) bOK=FALSE;
	else bOK=TRUE;

	if ( (bOK) && (DeleteObject((HBRUSH)(lptw->hbrBackground)))) bOK=TRUE;
	else bOK=FALSE;

	lptw->hbrBackground=NewHbrBackground;
	if ((bOK) && (InvalidateRect (lptw->hWndText, NULL, TRUE))) bOK=TRUE;
	else bOK=FALSE;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
DWORD GetIhmTextColor(void)
{
	return CurrentIHMTextColor;
}
/*-----------------------------------------------------------------------------------*/
DWORD GetIhmTextBackgroundColor(void)
{
	return CurrentIHMTextBackgroundColor;
}
/*-----------------------------------------------------------------------------------*/
void InitIhmDefaultColor(void)
{
	CurrentIHMTextColor=GetSysColor(COLOR_WINDOWTEXT);
	CurrentIHMTextBackgroundColor=GetSysColor(COLOR_WINDOW);
}
/*-----------------------------------------------------------------------------------*/
