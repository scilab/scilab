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
/*-----------------------------------------------------------------------------------*/
extern LPTW GetTextWinScilab(void);
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
		SetWindowLong(hWnd,	GWL_EXSTYLE,GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		bOK=TRUE;
		if (LoadUser32DLL==FALSE)
		{
			HMODULE hUser32 = GetModuleHandle("USER32.DLL");
			if (!hUser32) return FALSE;
			SetLayeredWindowAttributes = (SLWA_FUNC)GetProcAddress(hUser32, "SetLayeredWindowAttributes");
			LoadUser32DLL=TRUE;
		}
		SetCurrentAlphaLevel(hWnd ,CurrentAlphaLevel);
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
		for (i=GetCurrentAlphaLevel();i>=0;i--)
		{
			SetCurrentAlphaLevel(lptw->hWndParent ,i);
			SetCurrentAlphaLevel(lptw->hWndText ,i);
			Sleep(5);
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
			CurrentAlphaLevel=Alpha;
			SetLayeredWindowAttributes(hWnd, (COLORREF)NULL,CurrentAlphaLevel, LWA_ALPHA);
		}
	}
}
/*-----------------------------------------------------------------------------------*/
BOOL IncreaseAlphaLevel(void)
{
	BOOL bOK=FALSE;
	LPTW lptw=GetTextWinScilab();
	int Alpha=GetCurrentAlphaLevel();

	SetCurrentAlphaLevel(lptw->hWndParent ,Alpha++);
	SetCurrentAlphaLevel(lptw->hWndText ,Alpha++);

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL DecreaseAlphaLevel(void)
{
	BOOL bOK=FALSE;
	LPTW lptw=GetTextWinScilab();
	int Alpha=GetCurrentAlphaLevel();

	SetCurrentAlphaLevel(lptw->hWndParent ,Alpha--);
	SetCurrentAlphaLevel(lptw->hWndText ,Alpha--);

	return bOK;
}
/*-----------------------------------------------------------------------------------*/


