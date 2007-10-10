/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "MouseEvents.h"
/*-----------------------------------------------------------------------------------*/
#define CTRL_KEY 1000
#define SHIFT_KEY 2000
/*-----------------------------------------------------------------------------------*/
#define FOCUS_CHANGED 30
/*-----------------------------------------------------------------------------------*/
#define PRESSED_LEFT 0
#define RELEASED_LEFT -5
#define CLCK_LEFT 3
#define DBL_CLCK_LEFT 10
/*-----------------------------------------------------------------------------------*/
#define PRESSED_MIDDLE 1
#define RELEASED_MIDDLE -4
#define CLCK_MIDDLE 4
#define DBL_CLCK_MIDDLE 11
/*-----------------------------------------------------------------------------------*/
#define PRESSED_RIGHT 2
#define RELEASED_RIGHT -3
#define CLCK_RIGHT 5
#define DBL_CLCK_RIGHT 12
/*-----------------------------------------------------------------------------------*/
static int MOUSEX=0;
static int MOUSEY=0;
/*-----------------------------------------------------------------------------------*/
#define TimerIdLeft 10
#define TimerIdMiddle 20
#define TimerIdRight 30
/*-----------------------------------------------------------------------------------*/
static BOOL wait_dclick_left = FALSE;
static BOOL dclick_left = FALSE;
static BOOL lose_up_left=FALSE;
/*-----------------------------------------------------------------------------------*/
static BOOL wait_dclick_middle = FALSE;
static BOOL dclick_middle = FALSE;
static BOOL lose_up_middle=FALSE;
/*-----------------------------------------------------------------------------------*/
static BOOL wait_dclick_right = FALSE;
static BOOL dclick_right = FALSE;
static BOOL lose_up_right=FALSE;
/*-----------------------------------------------------------------------------------*/
static void CALLBACK reset_left_click_counter(HWND hwnd,UINT msg,UINT_PTR id,DWORD data);
static void CALLBACK reset_middle_click_counter(HWND hwnd,UINT msg,UINT_PTR id,DWORD data);
static void CALLBACK reset_right_click_counter(HWND hwnd,UINT msg,UINT_PTR id,DWORD data);
/*-----------------------------------------------------------------------------------*/
static void KillTimerLeft(HWND hwnd);
static void KillTimerMiddle(HWND hwnd);
static void KillTimerRight(HWND hwnd);
/*-----------------------------------------------------------------------------------*/
BOOL focushaschangedBorder = FALSE;
BOOL focushaschangedViewport = FALSE;
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_MOUSEMOVE(HWND hwnd, int x, int y, UINT keyFlags)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);

	int horzsinPos=ScilabGC->horzsi.nPos;
	int vertsinPos=ScilabGC->vertsi.nPos;

	
	if (lose_up_left)
	{
		lose_up_left = FALSE;
		KillTimerLeft(hwnd);
		KillTimerMiddle(hwnd);
		KillTimerRight(hwnd);

		wait_dclick_left=FALSE;

		if (GetKeyState(VK_CONTROL)<0)
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, CTRL_KEY+CLCK_LEFT, 0, 0);
		}
		else
		if (GetKeyState(VK_SHIFT)<0)
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, SHIFT_KEY+CLCK_LEFT, 0, 0);
		}
		else
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos,CLCK_LEFT, 0, 0);
		}
		
		return ;
	}

	if (lose_up_middle)
	{
		lose_up_middle = FALSE;
		KillTimerLeft(hwnd);
		KillTimerMiddle(hwnd);
		KillTimerRight(hwnd);

		wait_dclick_middle=FALSE;

		if (GetKeyState(VK_CONTROL)<0)
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos,CTRL_KEY+CLCK_MIDDLE, 0, 0);
		}
		else
		if (GetKeyState(VK_SHIFT)<0)
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, SHIFT_KEY+CLCK_MIDDLE, 0, 0);
		}
		else
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos,CLCK_MIDDLE, 0, 0);
		}
		
		return;
	}


	if (lose_up_right)
	{
		lose_up_right = FALSE;
		KillTimerLeft(hwnd);
		KillTimerMiddle(hwnd);
		KillTimerRight(hwnd);

		wait_dclick_right=FALSE;

		if (GetKeyState(VK_CONTROL)<0)
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos,CTRL_KEY+CLCK_RIGHT, 0, 0);
		}
		else
		if (GetKeyState(VK_SHIFT)<0)
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, SHIFT_KEY+CLCK_RIGHT, 0, 0);
		}
		else
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos,CLCK_RIGHT, 0, 0);
		}
		
		return;
	}

	if ( wait_dclick_left || wait_dclick_middle || wait_dclick_right )
	{
		if (wait_dclick_left)
		{
			KillTimerMiddle(hwnd);
			KillTimerRight(hwnd);

			wait_dclick_left=FALSE;
			KillTimer(hwnd,TimerIdLeft);

			if (GetKeyState(VK_CONTROL)<0)
			{
				
				if (focushaschangedViewport) 
				{
					PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos,20+PRESSED_LEFT, 0, 0);
					focushaschangedViewport = FALSE;
				}
				else
				{
					PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, CTRL_KEY+PRESSED_LEFT, 0, 0);
				}

			}
			else
			if (GetKeyState(VK_SHIFT)<0)
			{
				
				if (focushaschangedViewport)
				{
					PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos,PRESSED_LEFT+20, 0, 0);
					focushaschangedViewport = FALSE;
				}
				else
				{
					PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, SHIFT_KEY+PRESSED_LEFT, 0, 0);
				}

			}
			else
			{
				if (focushaschangedViewport)
				{
					PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos,PRESSED_LEFT+20, 0, 0);
					focushaschangedViewport = FALSE;
				}
				else
				{
					PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos,PRESSED_LEFT, 0, 0);
				}

			}
			return ;
		}

		if (wait_dclick_middle)
		{
			KillTimerLeft(hwnd);
			KillTimerRight(hwnd);

			wait_dclick_middle=FALSE;
			KillTimer(hwnd,TimerIdMiddle);

			if (GetKeyState(VK_CONTROL)<0)
			{
				if (focushaschangedViewport)
				{
					PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, PRESSED_MIDDLE+20, 0, 0);
					focushaschangedViewport = FALSE;
				}
				else
				{
					PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, CTRL_KEY+PRESSED_MIDDLE, 0, 0);
				}
			}
			else
			if (GetKeyState(VK_SHIFT)<0)
			{
				if (focushaschangedViewport)
				{
					PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos,PRESSED_MIDDLE+20, 0, 0);
					focushaschangedViewport = FALSE;
				}
				else
				{
					PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, SHIFT_KEY+PRESSED_MIDDLE, 0, 0);
				}
			}
			else
			{
				if (focushaschangedViewport)
				{
					PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos,PRESSED_MIDDLE+20, 0, 0);
					focushaschangedViewport = FALSE;
				}
				else
				{
					PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos,PRESSED_MIDDLE, 0, 0);
				}
			}
			return ;
		}

		if (wait_dclick_right)
		{
			KillTimerLeft(hwnd);
			KillTimerMiddle(hwnd);

			wait_dclick_right=FALSE;
			KillTimer(hwnd,TimerIdRight);

			if (GetKeyState(VK_CONTROL)<0)
			{
				if (focushaschangedViewport)
				{
					PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos,PRESSED_RIGHT+20, 0, 0);
					focushaschangedViewport = FALSE;
				}
				else
				{
					PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, CTRL_KEY+PRESSED_RIGHT, 0, 0);
				}
				
			}
			else
			if (GetKeyState(VK_SHIFT)<0)
			{
				if (focushaschangedViewport)
				{
					PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos,PRESSED_RIGHT+20, 0, 0);
					focushaschangedViewport = FALSE;
				}
				else
				{
					PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, SHIFT_KEY+PRESSED_RIGHT, 0, 0);
				}
				
			}
			else
			{
				if (focushaschangedViewport)
				{
					PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos,PRESSED_RIGHT+20, 0, 0);
					focushaschangedViewport = FALSE;
				}
				else
				{
					PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos,PRESSED_RIGHT, 0, 0);
				}
				
			}
			return ;
		}
	}
	else
	{
		PushClickQueue (ScilabGC->CurWindow,(int) MOUSEX +horzsinPos,MOUSEY +vertsinPos, -1, 1, 0);
		MOUSEX= x;
		MOUSEY= y;
	}

}
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_LBUTTONDOWN(HWND hwnd, BOOL fDoubleClick, int x, int y,UINT keyFlags)
{
	KillTimerMiddle(hwnd);
	KillTimerRight(hwnd);

	MOUSEX= x;
	MOUSEY= y;

	if (wait_dclick_left)
	{
		wait_dclick_left = FALSE;
		dclick_left = TRUE;
		lose_up_left = FALSE;
		KillTimer(hwnd,TimerIdLeft);
		return;
	}

	SetTimer(hwnd,TimerIdLeft,GetDoubleClickTime(),reset_left_click_counter);
	wait_dclick_left = TRUE;
	lose_up_left = FALSE;
	dclick_left = FALSE;
}
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_LBUTTONUP(HWND hwnd, int x, int y, UINT keyFlags)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	int horzsinPos=ScilabGC->horzsi.nPos;
	int vertsinPos=ScilabGC->vertsi.nPos;

	KillTimerMiddle(hwnd);
	KillTimerRight(hwnd);

	MOUSEX= x;
	MOUSEY= y;


	if (wait_dclick_left)
	{
		lose_up_left=TRUE;
		return;
	}

	if (!dclick_left) 
	{
		if (GetKeyState(VK_CONTROL)<0)
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, RELEASED_LEFT+CTRL_KEY, 0, -1);
		}
		else
		if (GetKeyState(VK_SHIFT)<0)
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, RELEASED_LEFT+SHIFT_KEY, 0, -1);
		}
		else
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, RELEASED_LEFT, 0, -1);
		}
		
	}
	else
	if (lose_up_left)
	{
		lose_up_left=FALSE;
	}
}
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_LBUTTONDBLCLK(HWND hwnd, BOOL fDoubleClick, int x, int y,UINT keyFlags)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	int horzsinPos=ScilabGC->horzsi.nPos;
	int vertsinPos=ScilabGC->vertsi.nPos;

	KillTimerMiddle(hwnd);
	KillTimerRight(hwnd);

	MOUSEX= x;
	MOUSEY= y;


	wait_dclick_left=FALSE;
	dclick_left = TRUE;
	KillTimer(hwnd,TimerIdLeft);
	lose_up_left=TRUE;

	if (GetKeyState(VK_CONTROL)<0)
	{
		PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, CTRL_KEY+DBL_CLCK_LEFT, 0, 0);
	}
	else
	if (GetKeyState(VK_SHIFT)<0)
	{
		PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, SHIFT_KEY+DBL_CLCK_LEFT, 0, 0);
	}
	else
	{
		PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, DBL_CLCK_LEFT, 0, 0);
	}
	
}

/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_MBUTTONDOWN(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	MOUSEX= x;
	MOUSEY= y;

	KillTimerLeft(hwnd);
	KillTimerRight(hwnd);

	if (wait_dclick_middle)
	{
		wait_dclick_middle = FALSE;
		dclick_middle = TRUE;
		lose_up_middle = FALSE;
		KillTimer(hwnd,TimerIdMiddle);
		return;
	}

	SetTimer(hwnd,TimerIdMiddle,GetDoubleClickTime(),reset_middle_click_counter);
	wait_dclick_middle = TRUE;
	lose_up_middle = FALSE;
	dclick_middle = FALSE;
}
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_MBUTTONUP(HWND hwnd, int x, int y, UINT keyFlags)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	int horzsinPos=ScilabGC->horzsi.nPos;
	int vertsinPos=ScilabGC->vertsi.nPos;

	KillTimerLeft(hwnd);
	KillTimerRight(hwnd);

	MOUSEX= x;
	MOUSEY= y;

	if (wait_dclick_middle)
	{
		lose_up_middle=TRUE;
		return;
	}

	if (!dclick_middle) 
	{
		if (GetKeyState(VK_CONTROL)<0)
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, RELEASED_MIDDLE+CTRL_KEY, 0, -1);
		}
		else
		if (GetKeyState(VK_SHIFT)<0)
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, RELEASED_MIDDLE+SHIFT_KEY, 0, -1);
		}
		else
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, RELEASED_MIDDLE, 0, -1);
		}
	}
	else
	if (lose_up_middle)
	{
		lose_up_middle=FALSE;
	}
}
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_MBUTTONDBLCLK(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	int horzsinPos=ScilabGC->horzsi.nPos;
	int vertsinPos=ScilabGC->vertsi.nPos;

	MOUSEX= x;
	MOUSEY= y;

	KillTimerLeft(hwnd);
	KillTimerRight(hwnd);

	wait_dclick_middle=FALSE;
	dclick_middle = TRUE;
	KillTimer(hwnd,TimerIdMiddle);
	lose_up_middle=TRUE;

	if (GetKeyState(VK_CONTROL)<0)
	{
		PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, CTRL_KEY+DBL_CLCK_MIDDLE, 0, 0);
	}
	else
	if (GetKeyState(VK_SHIFT)<0)
	{
		PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, SHIFT_KEY+DBL_CLCK_MIDDLE, 0, 0);
	}
	else
	{
		PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, DBL_CLCK_MIDDLE, 0, 0);
	}
}
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_RBUTTONDOWN(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	MOUSEX= x;
	MOUSEY= y;

	KillTimerLeft(hwnd);
	KillTimerRight(hwnd);

	if (wait_dclick_right)
	{
		wait_dclick_right = FALSE;
		dclick_right = TRUE;
		lose_up_right = FALSE;
		KillTimer(hwnd,TimerIdRight);
		return;
	}

	SetTimer(hwnd,TimerIdRight,GetDoubleClickTime(),reset_right_click_counter);
	wait_dclick_right = TRUE;
	lose_up_right = FALSE;
	dclick_right = FALSE;
}
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_RBUTTONUP(HWND hwnd, int x, int y, UINT keyFlags)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	int horzsinPos=ScilabGC->horzsi.nPos;
	int vertsinPos=ScilabGC->vertsi.nPos;

	MOUSEX= x;
	MOUSEY= y;

	KillTimerLeft(hwnd);
	KillTimerMiddle(hwnd);

	if (wait_dclick_right)
	{
		lose_up_right=TRUE;
		return;
	}

	if (!dclick_right) 
	{
		if (GetKeyState(VK_CONTROL)<0)
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, RELEASED_RIGHT+CTRL_KEY, 0, -1);
		}
		else
		if (GetKeyState(VK_SHIFT)<0)
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, RELEASED_RIGHT+SHIFT_KEY, 0, -1);
		}
		else
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, RELEASED_RIGHT, 0, -1);
		}
	}
	else
	if (lose_up_right)
	{
		lose_up_right=FALSE;
	}
}
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_RBUTTONDBLCLK(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	int horzsinPos=ScilabGC->horzsi.nPos;
	int vertsinPos=ScilabGC->vertsi.nPos;

	MOUSEX= x;
	MOUSEY= y;

	KillTimerLeft(hwnd);
	KillTimerMiddle(hwnd);

	wait_dclick_right=FALSE;
	dclick_right = TRUE;
	KillTimer(hwnd,TimerIdRight);
	lose_up_right=TRUE;

	if (GetKeyState(VK_CONTROL)<0)
	{
		PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, CTRL_KEY+DBL_CLCK_RIGHT, 0, 0);
	}
	else
	if (GetKeyState(VK_SHIFT)<0)
	{
		PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, SHIFT_KEY+DBL_CLCK_RIGHT, 0, 0);
	}
	else
	{
		PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, DBL_CLCK_RIGHT, 0, 0);
	}
}
/*-----------------------------------------------------------------------------------*/
void CALLBACK reset_left_click_counter(HWND hwnd,UINT msg,UINT_PTR id,DWORD data)
	{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	int horzsinPos=ScilabGC->horzsi.nPos;
	int vertsinPos=ScilabGC->vertsi.nPos;

	KillTimerMiddle(hwnd);
	KillTimerRight(hwnd);

	wait_dclick_left=FALSE;
	KillTimer(hwnd,TimerIdLeft);


	if (dclick_left)
	{
		if (GetKeyState(VK_CONTROL)<0)
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, CTRL_KEY+DBL_CLCK_LEFT, 0, 0);
		}
		else
		if (GetKeyState(VK_SHIFT)<0)
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, SHIFT_KEY+DBL_CLCK_LEFT, 0, 0);
		}
		else
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, DBL_CLCK_LEFT, 0, 0);
		}
		
	}
	else
	{
		if (lose_up_left)
		{
			if (GetKeyState(VK_CONTROL)<0)
			{
				PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, CTRL_KEY+CLCK_LEFT, 0, 0);
			}
			else
			if (GetKeyState(VK_SHIFT)<0)
			{
				PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, SHIFT_KEY+CLCK_LEFT, 0, 0);
			}
			else
			{
				PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos,CLCK_LEFT, 0, 0);
			}
			lose_up_left=FALSE;
			
		}
		else
		{
			if (GetKeyState(VK_CONTROL)<0)
			{
				PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, CTRL_KEY+PRESSED_LEFT, 0, 0);
			}
			else
			if (GetKeyState(VK_SHIFT)<0)
			{
				PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, SHIFT_KEY+PRESSED_LEFT, 0, 0);
			}
			else
			{
				PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos,PRESSED_LEFT, 0, 0);
				
			}
			
		}
	}
}
/*-----------------------------------------------------------------------------------*/
void CALLBACK reset_middle_click_counter(HWND hwnd,UINT msg,UINT_PTR id,DWORD data)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	int horzsinPos=ScilabGC->horzsi.nPos;
	int vertsinPos=ScilabGC->vertsi.nPos;

	KillTimerLeft(hwnd);
	KillTimerRight(hwnd);

	wait_dclick_left=FALSE;
	KillTimer(hwnd,TimerIdMiddle);

	if (dclick_middle)
	{
		if (GetKeyState(VK_CONTROL)<0)
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, CTRL_KEY+DBL_CLCK_MIDDLE, 0, 0);
		}
		else
		if (GetKeyState(VK_SHIFT)<0)
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, SHIFT_KEY+DBL_CLCK_MIDDLE, 0, 0);
		}
		else
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, DBL_CLCK_MIDDLE, 0, 0);
		}
	}
	else
	{
		if (lose_up_middle)
		{
			if (GetKeyState(VK_CONTROL)<0)
			{
				PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, CTRL_KEY+CLCK_MIDDLE, 0, 0);
			}
			else
			if (GetKeyState(VK_SHIFT)<0)
			{
				PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, SHIFT_KEY+CLCK_MIDDLE, 0, 0);
			}
			else
			{
				PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, CLCK_MIDDLE, 0, 0);
			}
			lose_up_middle=FALSE;
		}
		else
		{
			if (GetKeyState(VK_CONTROL)<0)
			{
				PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, CTRL_KEY+PRESSED_MIDDLE, 0, 0);
			}
			else
			if (GetKeyState(VK_SHIFT)<0)
			{
				PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, SHIFT_KEY+PRESSED_MIDDLE, 0, 0);
			}
			else
			{
				PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos,PRESSED_MIDDLE, 0, 0);
			}
		}
	}
}
/*-----------------------------------------------------------------------------------*/
void CALLBACK reset_right_click_counter(HWND hwnd,UINT msg,UINT_PTR id,DWORD data)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	int horzsinPos=ScilabGC->horzsi.nPos;
	int vertsinPos=ScilabGC->vertsi.nPos;

	KillTimerLeft(hwnd);
	KillTimerMiddle(hwnd);

	wait_dclick_right=FALSE;
	KillTimer(hwnd,TimerIdRight);

	if (dclick_right)
	{
		if (GetKeyState(VK_CONTROL)<0)
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, CTRL_KEY+DBL_CLCK_RIGHT, 0, 0);
		}
		else
		if (GetKeyState(VK_SHIFT)<0)
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, SHIFT_KEY+DBL_CLCK_RIGHT, 0, 0);
		}
		else
		{
			PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, DBL_CLCK_RIGHT, 0, 0);
		}
	}
	else
	{
		if (lose_up_right)
		{
			if (GetKeyState(VK_CONTROL)<0)
			{
				PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, CTRL_KEY+CLCK_RIGHT, 0, 0);
			}
			else
			if (GetKeyState(VK_SHIFT)<0)
			{
				PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, SHIFT_KEY+CLCK_RIGHT, 0, 0);
			}
			else
			{
				PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, CLCK_RIGHT, 0, 0);
			}
			lose_up_right=FALSE;
		}
		else
		{
			if (GetKeyState(VK_CONTROL)<0)
			{
				PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, CTRL_KEY+PRESSED_RIGHT, 0, 0);
			}
			else
			if (GetKeyState(VK_SHIFT)<0)
			{
				PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, SHIFT_KEY+PRESSED_RIGHT, 0, 0);
			}
			else
			{
				PushClickQueue (ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos,PRESSED_RIGHT, 0, 0);
			}
		}
	}
}
/*-----------------------------------------------------------------------------------*/
static void KillTimerLeft(HWND hwnd)
{
	wait_dclick_left = FALSE;
	dclick_left = FALSE;
	lose_up_left=FALSE;
	KillTimer(hwnd,TimerIdLeft);
}
/*-----------------------------------------------------------------------------------*/
static void KillTimerMiddle(HWND hwnd)
{
	wait_dclick_middle = FALSE;
	dclick_middle = FALSE;
	lose_up_middle=FALSE;
	KillTimer(hwnd,TimerIdMiddle);
}
/*-----------------------------------------------------------------------------------*/
static void KillTimerRight(HWND hwnd)
{
	wait_dclick_right = FALSE;
	dclick_right = FALSE;
	lose_up_right=FALSE;
	KillTimer(hwnd,TimerIdRight);
}
/*-----------------------------------------------------------------------------------*/
