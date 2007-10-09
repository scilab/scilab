/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __MOUSEEVENTS__
#define __MOUSEEVENTS__
/*-----------------------------------------------------------------------------------*/ 
#include <windows.h>
#include "wcommon.h"
/*-----------------------------------------------------------------------------------*/ 
void ON_EVENT_GRAPH_WM_LBUTTONDOWN(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
void ON_EVENT_GRAPH_WM_LBUTTONUP(HWND hwnd, int x, int y, UINT keyFlags);
void ON_EVENT_GRAPH_WM_LBUTTONDBLCLK(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
void ON_EVENT_GRAPH_WM_RBUTTONDOWN(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
void ON_EVENT_GRAPH_WM_RBUTTONUP(HWND hwnd, int x, int y, UINT keyFlags);
void ON_EVENT_GRAPH_WM_RBUTTONDBLCLK(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
void ON_EVENT_GRAPH_WM_MBUTTONDOWN(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
void ON_EVENT_GRAPH_WM_MBUTTONUP(HWND hwnd, int x, int y, UINT keyFlags);
void ON_EVENT_GRAPH_WM_MBUTTONDBLCLK(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
void ON_EVENT_GRAPH_WM_MOUSEMOVE(HWND hwnd, int x, int y, UINT keyFlags);
//void InitializeButtons(void);
/*-----------------------------------------------------------------------------------*/ 
#endif /* __MOUSEEVENTS__ */
/*-----------------------------------------------------------------------------------*/ 
