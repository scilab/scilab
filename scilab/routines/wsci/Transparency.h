/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __TRANSPARENCY__
#define  __TRANSPARENCY__
/*-----------------------------------------------------------------------------------*/
#define  _WIN32_WINNT 0x0500
/*-----------------------------------------------------------------------------------*/
#include <windows.h>
#include <windowsx.h>
#include "wgnuplib.h"
#include "resource.h"
#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"
#include "../version.h"
#include "../os_specific/win_mem_alloc.h"

/*-----------------------------------------------------------------------------------*/
BOOL EnableTransparencyMode(HWND hWnd);
BOOL DisableTransparencyMode(HWND hWnd);
BOOL IsEnableTransparencyMode(void);
void ScilabFxFadeOut(void);
int GetCurrentAlphaLevel(void);
void SetCurrentAlphaLevel(HWND hWnd ,int Alpha);
BOOL IncreaseAlphaLevel(void);
BOOL DecreaseAlphaLevel(void);

/*-----------------------------------------------------------------------------------*/
#endif /*__TRANSPARENCY__*/
/*-----------------------------------------------------------------------------------*/