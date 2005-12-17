/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __WNDCONSOLETEXTPROC__
#define __WNDCONSOLETEXTPROC__
/*-----------------------------------------------------------------------------------*/
#include <windows.h>
#include <windowsx.h>
#include "wgnuplib.h"
#include "resource.h"
#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"
#include "../version.h"
#include "../machine.h"
#include "../os_specific/win_mem_alloc.h"
#include "wresource.h"
/*-----------------------------------------------------------------------------------*/
EXPORT LRESULT CALLBACK WndTextProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
/*-----------------------------------------------------------------------------------*/
#endif /* __WNDCONSOLETEXTPROC__ */
/*-----------------------------------------------------------------------------------*/