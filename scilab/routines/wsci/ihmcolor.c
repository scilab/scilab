/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "ihmcolor.h"
#include "win_mem_alloc.h" /* MALLOC */
#include "wgnuplib.h"
/*-----------------------------------------------------------------------------------*/
static DWORD CurrentIHMTextColor=0;
static DWORD CurrentIHMTextBackgroundColor=16666667;
/*-----------------------------------------------------------------------------------*/
BOOL SetIhmTextColor(int R,int G,int B,BOOL Refresh)
{
	BOOL bOK=FALSE;

	CurrentIHMTextColor=RGB(R,G,B);

	if (Refresh)
	{
		extern char ScilexWindowName[MAX_PATH];
		LPTW lptw;
		lptw = (LPTW) GetWindowLong (FindWindow(NULL,ScilexWindowName), 0);

		if (InvalidateRect (lptw->hWndText, NULL, TRUE)) bOK=TRUE;

	}
	else  bOK=TRUE;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL SetIhmTextBackgroundColor(int R,int G,int B,BOOL Refresh)
{
	BOOL bOK=FALSE;
	
	CurrentIHMTextBackgroundColor=RGB(R,G,B);

	if (Refresh)
	{
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
	}
	else bOK=TRUE;

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
BOOL ChooseColorBox(int *R,int *G,int *B)
{
	BOOL bOk=FALSE;

	CHOOSECOLOR cc;                 // common dialog box structure 
	static COLORREF acrCustClr[16]; // array of custom colors 
	static DWORD rgbCurrent;        // initial color selection
	extern char ScilexWindowName[MAX_PATH];
	LPTW lptw;

	lptw = (LPTW) GetWindowLong (FindWindow(NULL,ScilexWindowName), 0);

	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = lptw->hWndParent;
	cc.lpCustColors = (LPDWORD) acrCustClr;
	cc.rgbResult = RGB(*R,*G,*B);
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc)==TRUE) 
	{
		*R=GetRValue(cc.rgbResult);
		*G=GetGValue(cc.rgbResult);
		*B=GetBValue(cc.rgbResult);
		bOk=TRUE;
	}
	else bOk=FALSE;


	return bOk;
}
/*-----------------------------------------------------------------------------------*/