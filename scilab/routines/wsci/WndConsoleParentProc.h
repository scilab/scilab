/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __WNDCONSOLEPARENTPROC__
#define __WNDCONSOLEPARENTPROC__
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
#include "wresource.h"
/*-----------------------------------------------------------------------------------*/
EXPORT LRESULT CALLBACK WndParentProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
/*-----------------------------------------------------------------------------------*/
#endif /* __WNDCONSOLEPARENTPROC__ */
/*-----------------------------------------------------------------------------------*/