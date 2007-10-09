/* Allan CORNET */
/* INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
#include "Events.h"
#include "MouseEvents.h"
#include "KeyEvents.h"
/*-----------------------------------------------------------------------------------*/
static BOOL firstCall = TRUE;
/*-----------------------------------------------------------------------------------*/
BOOL GetEventKeyboardAndMouse(  UINT message, WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC)
{
	if (firstCall)
	{
		//InitializeButtons();
		firstCall = FALSE;
	}
	switch(message)
	{
		HANDLE_MSG(ScilabGC->CWindow,WM_KEYDOWN,ON_EVENT_GRAPH_WM_KEYDOWN);
		HANDLE_MSG(ScilabGC->CWindow,WM_KEYUP,ON_EVENT_GRAPH_WM_KEYUP);
		
		HANDLE_MSG(ScilabGC->CWindow,WM_LBUTTONDOWN,ON_EVENT_GRAPH_WM_LBUTTONDOWN);
		HANDLE_MSG(ScilabGC->CWindow,WM_LBUTTONUP,ON_EVENT_GRAPH_WM_LBUTTONUP);
		HANDLE_MSG(ScilabGC->CWindow,WM_LBUTTONDBLCLK,ON_EVENT_GRAPH_WM_LBUTTONDBLCLK);

		HANDLE_MSG(ScilabGC->CWindow,WM_RBUTTONDOWN,ON_EVENT_GRAPH_WM_RBUTTONDOWN);
		HANDLE_MSG(ScilabGC->CWindow,WM_RBUTTONUP,ON_EVENT_GRAPH_WM_RBUTTONUP);
		HANDLE_MSG(ScilabGC->CWindow,WM_RBUTTONDBLCLK,ON_EVENT_GRAPH_WM_RBUTTONDBLCLK);

		HANDLE_MSG(ScilabGC->CWindow,WM_MBUTTONDOWN,ON_EVENT_GRAPH_WM_MBUTTONDOWN);
		HANDLE_MSG(ScilabGC->CWindow,WM_MBUTTONUP,ON_EVENT_GRAPH_WM_MBUTTONUP);
		HANDLE_MSG(ScilabGC->CWindow,WM_MBUTTONDBLCLK,ON_EVENT_GRAPH_WM_MBUTTONDBLCLK);

		HANDLE_MSG(ScilabGC->CWindow,WM_MOUSEMOVE,ON_EVENT_GRAPH_WM_MOUSEMOVE);

	}
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
