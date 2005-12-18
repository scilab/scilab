/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "Transparency.h"
/*-----------------------------------------------------------------------------------*/
#ifndef WS_EX_LAYERED
  #define WS_EX_LAYERED           0x00080000
#endif

#ifndef LWA_COLORKEY
#define LWA_COLORKEY            0x00000001
#endif

#ifndef LWA_ALPHA
#define LWA_ALPHA               0x00000002
#endif
/*-----------------------------------------------------------------------------------*/
typedef BOOL (WINAPI *SLWA_FUNC)(HWND hwnd,COLORREF crKey,BYTE bAlpha,DWORD dwFlags);
SLWA_FUNC SetLayeredWindowAttributes;
/*-----------------------------------------------------------------------------------*/
static BOOL LoadUser32DLL=FALSE;
static int CurrentAlphaLevel=255;
static BOOL TransparencyON=FALSE;
static HMODULE hUser32=NULL;
/*-----------------------------------------------------------------------------------*/
extern LPTW GetTextWinScilab(void);
/*-----------------------------------------------------------------------------------*/
BOOL IsEnableTransparencyMode(void)
{
	return TransparencyON;
}
/*-----------------------------------------------------------------------------------*/
BOOL ActivateTransparencyMode(HWND hWnd)
{
	BOOL bOK=FALSE;
	int OS=GetOSVersion();

	if ( (OS!=OS_WIN32_WINDOWS_NT_3_51) &&
		(OS!=OS_WIN32_WINDOWS_NT_4_0) &&
		(OS!=OS_WIN32_WINDOWS_95) &&
		(OS!=OS_WIN32_WINDOWS_98) &&
		(OS!=OS_WIN32_WINDOWS_Me) )
	{
		if (TransparencyON) return FALSE;

		if (LoadUser32DLL==FALSE)
		{
			hUser32 = GetModuleHandle("USER32.DLL");
			LoadUser32DLL=TRUE;
		}
		if (!hUser32) return FALSE;

		SetWindowLong(hWnd,	GWL_EXSTYLE,GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes = (SLWA_FUNC)GetProcAddress(hUser32, "SetLayeredWindowAttributes");
		TransparencyON=TRUE;
		bOK=TRUE;
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL DisableTransparencyMode(HWND hWnd)
{
	BOOL bOK=FALSE;
	int OS=GetOSVersion();

	if ( (OS!=OS_WIN32_WINDOWS_NT_3_51) &&
		(OS!=OS_WIN32_WINDOWS_NT_4_0) &&
		(OS!=OS_WIN32_WINDOWS_95) &&
		(OS!=OS_WIN32_WINDOWS_98) &&
		(OS!=OS_WIN32_WINDOWS_Me) )
	{
			SetWindowLong(hWnd,	GWL_EXSTYLE,CS_HREDRAW | CS_VREDRAW);
			CurrentAlphaLevel=255;
			TransparencyON=FALSE;
			bOK=TRUE;
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
void ScilabFxFadeOut(void)
{
	LPTW lptw=GetTextWinScilab();
	int i=0;
	int OS=GetOSVersion();

	if ( (OS!=OS_WIN32_WINDOWS_NT_3_51) &&
		(OS!=OS_WIN32_WINDOWS_NT_4_0) &&
		(OS!=OS_WIN32_WINDOWS_95) &&
		(OS!=OS_WIN32_WINDOWS_98) &&
		(OS!=OS_WIN32_WINDOWS_Me) )
	{
		for (i=GetCurrentAlphaLevel();i>=0;i=i-16)
		{
			SetCurrentAlphaLevel(lptw->hWndParent ,i);
			SetCurrentAlphaLevel(lptw->hWndText ,i);
		}
	}
}
/*-----------------------------------------------------------------------------------*/
int GetCurrentAlphaLevel(void)
{
	return CurrentAlphaLevel;
}
/*-----------------------------------------------------------------------------------*/
void SetCurrentAlphaLevel(HWND hWnd ,int Alpha)
{
	int OS=GetOSVersion();

	if ( (OS!=OS_WIN32_WINDOWS_NT_3_51) &&
		(OS!=OS_WIN32_WINDOWS_NT_4_0) &&
		(OS!=OS_WIN32_WINDOWS_95) &&
		(OS!=OS_WIN32_WINDOWS_98) &&
		(OS!=OS_WIN32_WINDOWS_Me) )
	{
		if ( (Alpha>=0) && (Alpha<=255) )
		{
			
			SetLayeredWindowAttributes(hWnd, (COLORREF)NULL,CurrentAlphaLevel, LWA_ALPHA);
		}
		CurrentAlphaLevel=Alpha;
	}
}
/*-----------------------------------------------------------------------------------*/
BOOL IncreaseAlphaLevel(void)
{
	BOOL bOK=FALSE;
	LPTW lptw=GetTextWinScilab();
	int Alpha=GetCurrentAlphaLevel();

	if (Alpha==255)
	{
		DisableTransparencyMode(lptw->hWndParent);
		DisableTransparencyMode(lptw->hWndText);
		DragAcceptFiles (lptw->hWndParent, TRUE);
		InvalidateRect(lptw->hWndParent,NULL,TRUE);
		InvalidateRect(lptw->hWndText,NULL,TRUE);
	}
	else
	{
		if (Alpha<255)
		{
			SetCurrentAlphaLevel(lptw->hWndParent ,Alpha++);
			SetCurrentAlphaLevel(lptw->hWndText ,Alpha++);
			if (Alpha>255) Alpha=255;
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL DecreaseAlphaLevel(void)
{
	BOOL bOK=FALSE;
	LPTW lptw=GetTextWinScilab();
	int Alpha=GetCurrentAlphaLevel();

	if (Alpha<0) Alpha=0;
	else
	{
		SetCurrentAlphaLevel(lptw->hWndParent ,Alpha--);
		SetCurrentAlphaLevel(lptw->hWndText ,Alpha--);
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/


