/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __WNDGRAPHPROC__
#define __WNDGRAPHPROC__
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
#include "../graphics/bcg.h"
/*-----------------------------------------------------------------------------------*/
EXPORT LRESULT CALLBACK WndGraphProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
/*-----------------------------------------------------------------------------------*/
#endif /* __WNDGRAPHPROC__ */
