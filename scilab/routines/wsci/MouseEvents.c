/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "MouseEvents.h"
/*-----------------------------------------------------------------------------------*/
#define CTRL_KEY 1000
#define SHIFT_KEY 2000
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
static int MOUSEX = 0;
static int MOUSEY = 0;
/*-----------------------------------------------------------------------------------*/
typedef struct ButtonEvent
{ 
	BOOL wait_dclick;
	BOOL dclick;
	BOOL lose_up;
	BOOL shortcut_move;
	int TimerId;
} ButtonEvent ;
/*-----------------------------------------------------------------------------------*/
#define DEBUG_PRINTF 1
/*-----------------------------------------------------------------------------------*/
static struct ButtonEvent LeftButton;
static struct ButtonEvent RightButton;
static struct ButtonEvent MiddleButton;
/*-----------------------------------------------------------------------------------*/
static void KillTimerLeft(HWND hwnd);
static void KillTimerRight(HWND hwnd);
static void KillTimerMiddle(HWND hwnd);
/*-----------------------------------------------------------------------------------*/
static void SetTimerLeft(HWND hwnd);
static void SetTimerRight(HWND hwnd);
static void SetTimerMiddle(HWND hwnd);
/*-----------------------------------------------------------------------------------*/
static void CALLBACK reset_click_counter_left(HWND hwnd,UINT msg,UINT_PTR id,DWORD data);
static void CALLBACK reset_click_counter_right(HWND hwnd,UINT msg,UINT_PTR id,DWORD data);
static void CALLBACK reset_click_counter_middle(HWND hwnd,UINT msg,UINT_PTR id,DWORD data);
/*-----------------------------------------------------------------------------------*/
static void SendPressed(int winId,int x,int y,int buttonId);
static void SendReleased(int winId,int x,int y,int buttonId);
static void SendClicked(int winId,int x,int y,int buttonId);
static void SendDoubleClicked(int winId,int x,int y,int buttonId);
static void SendMove(int winId,int x,int y);
/*-----------------------------------------------------------------------------------*/
BOOL focus = FALSE;
/*-----------------------------------------------------------------------------------*/
static void InitializeButton(struct ButtonEvent Button,int TimerId);
static void KillTimers(HWND hwnd);
/*-----------------------------------------------------------------------------------*/
void InitializeButtons(void)
{
	#define TimerId_left 10
	#define TimerId_right 20
	#define TimerId_middle 30
	InitializeButton(LeftButton,TimerId_left);
	InitializeButton(RightButton,TimerId_right);
	InitializeButton(MiddleButton,TimerId_middle);
}
/*-----------------------------------------------------------------------------------*/
static void InitializeButton(struct ButtonEvent Button,int TimerId)
{
	Button.dclick = FALSE;
	Button.lose_up = FALSE;
	Button.wait_dclick = FALSE;
	Button.TimerId = TimerId;
	Button.shortcut_move = FALSE;
}
/*-----------------------------------------------------------------------------------*/
static void KillTimers(HWND hwnd);
/*-----------------------------------------------------------------------------------*/
static void KillTimers(HWND hwnd)
{
	KillTimerLeft(hwnd);
	KillTimerRight(hwnd);
	KillTimerMiddle(hwnd);
}
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_MOUSEMOVE(HWND hwnd, int x, int y, UINT keyFlags)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);

	int horzsinPos=ScilabGC->horzsi.nPos;
	int vertsinPos=ScilabGC->vertsi.nPos;

	if (focus) 
	{
		KillTimers(hwnd);
		focus = FALSE;
		return ;
	}

	MOUSEX = x;
	MOUSEY = y;

	if (LeftButton.shortcut_move)
	{
		//SendPressed(ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos,PRESSED_LEFT);
		//KillTimers(hwnd);
		//return;
	}

	if (RightButton.shortcut_move)
	{
		//KillTimers(hwnd);
		// SendPressed(ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos,PRESSED_RIGHT);
		//return;
	}

	if (MiddleButton.shortcut_move)
	{
		//KillTimers(hwnd);
		// SendPressed(ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos,PRESSED_MIDDLE);
		//return;
	}
	
	SendMove(ScilabGC->CurWindow, MOUSEX +horzsinPos,MOUSEY +vertsinPos);
}
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_LBUTTONDOWN(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	if (focus) 
	{
		KillTimers(hwnd);
		focus = FALSE;

		MOUSEX= x;
		MOUSEY= y;
		SetTimerLeft(hwnd);
		LeftButton.shortcut_move = TRUE;
		return ;
	}
	MOUSEX= x;
	MOUSEY= y;

	if (LeftButton.wait_dclick) 
	{
	/* If we are waiting for a double-click, we only handle Button events
      in a special fashion.*/

	  KillTimers(hwnd);
	  LeftButton.dclick = TRUE;

      return;
	}

	SetTimerLeft(hwnd);

	LeftButton.shortcut_move = TRUE;
	RightButton.shortcut_move = FALSE;
	MiddleButton.shortcut_move = FALSE;
}
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_LBUTTONUP(HWND hwnd, int x, int y, UINT keyFlags)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	int horzsinPos=ScilabGC->horzsi.nPos;
	int vertsinPos=ScilabGC->vertsi.nPos;


	LeftButton.shortcut_move = FALSE;
	RightButton.shortcut_move = FALSE;
	MiddleButton.shortcut_move = FALSE;

	if (focus) 
	{
		KillTimers(hwnd);
		focus = FALSE;
		return ;
	}

	MOUSEX= x;
	MOUSEY= y;

	if (LeftButton.wait_dclick) 
	{
		LeftButton.lose_up = TRUE;
		return;
	}
	if (LeftButton.dclick)  return;

	/* button released */
	SendReleased(ScilabGC->CurWindow, x, y,RELEASED_LEFT);
}
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_LBUTTONDBLCLK(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{

	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	int horzsinPos=ScilabGC->horzsi.nPos;
	int vertsinPos=ScilabGC->vertsi.nPos;

	LeftButton.shortcut_move = FALSE;
	RightButton.shortcut_move = FALSE;
	MiddleButton.shortcut_move = FALSE;

	if (focus) 
	{
		focus = FALSE;
		return ;
	}

	MOUSEX= x;
	MOUSEY= y;

	SendDoubleClicked(ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, DBL_CLCK_LEFT);

	KillTimers(hwnd);
	LeftButton.dclick = TRUE;
}
/*-----------------------------------------------------------------------------------*/
static void CALLBACK reset_click_counter_left(HWND hwnd,UINT msg,UINT_PTR id,DWORD data)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	int horzsinPos=ScilabGC->horzsi.nPos;
	int vertsinPos=ScilabGC->vertsi.nPos;

	KillTimer(hwnd,LeftButton.TimerId);

	LeftButton.shortcut_move = FALSE;
	RightButton.shortcut_move = FALSE;
	MiddleButton.shortcut_move = FALSE;

	if (focus) 
	{
		focus = FALSE;
		return ;
	}

	if (LeftButton.dclick)
	{
		SendDoubleClicked(ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, DBL_CLCK_LEFT);
	}
	else
	{
		if (LeftButton.lose_up)
		{
			SendClicked(ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos,CLCK_LEFT);
			LeftButton.lose_up = FALSE;
		}
		else
		{
			SendPressed(ScilabGC->CurWindow,MOUSEX+horzsinPos,MOUSEY+vertsinPos,PRESSED_LEFT);
		}
	}
	LeftButton.wait_dclick = FALSE;
}

/*-----------------------------------------------------------------------------------*/
static void KillTimerLeft(HWND hwnd)
{
	KillTimer(hwnd,LeftButton.TimerId);
	LeftButton.wait_dclick = FALSE;
	LeftButton.dclick = FALSE;
	LeftButton.lose_up = FALSE;

	LeftButton.shortcut_move = FALSE;
}
/*-----------------------------------------------------------------------------------*/
static void SetTimerLeft(HWND hwnd)
{
	SetTimer(hwnd,LeftButton.TimerId,GetDoubleClickTime()/2,reset_click_counter_left);
	LeftButton.wait_dclick = TRUE;
	LeftButton.lose_up = FALSE;
	LeftButton.dclick = FALSE;
}
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_RBUTTONDOWN(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	if (focus) 
	{
		KillTimers(hwnd);
		focus = FALSE;

		MOUSEX= x;
		MOUSEY= y;

		SetTimerRight(hwnd);
		return ;
	}
	MOUSEX= x;
	MOUSEY= y;

	if (RightButton.wait_dclick) 
	{
	/* If we are waiting for a double-click, we only handle Button events
      in a special fashion.*/

	  KillTimers(hwnd);
	  RightButton.dclick = TRUE;

      return;
	}

	SetTimerRight(hwnd);
}
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_RBUTTONUP(HWND hwnd, int x, int y, UINT keyFlags)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	int horzsinPos=ScilabGC->horzsi.nPos;
	int vertsinPos=ScilabGC->vertsi.nPos;

	if (focus) 
	{
		KillTimers(hwnd);
		focus = FALSE;
		return ;
	}

	MOUSEX= x;
	MOUSEY= y;

	if (RightButton.wait_dclick) 
	{
		RightButton.lose_up = TRUE;
		return;
	}
	if (RightButton.dclick)  return;
	/* button released */
	SendReleased(ScilabGC->CurWindow, x, y,RELEASED_RIGHT);
}
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_RBUTTONDBLCLK(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{

	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	int horzsinPos=ScilabGC->horzsi.nPos;
	int vertsinPos=ScilabGC->vertsi.nPos;

	if (focus) 
	{
		focus = FALSE;
		return ;
	}

	MOUSEX= x;
	MOUSEY= y;

	SendDoubleClicked(ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, DBL_CLCK_RIGHT);

	KillTimers(hwnd);
	RightButton.dclick = TRUE;
}
/*-----------------------------------------------------------------------------------*/
static void CALLBACK reset_click_counter_right(HWND hwnd,UINT msg,UINT_PTR id,DWORD data)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	int horzsinPos=ScilabGC->horzsi.nPos;
	int vertsinPos=ScilabGC->vertsi.nPos;

	KillTimer(hwnd,RightButton.TimerId);

	if (focus) 
	{
		focus = FALSE;
		return ;
	}

	if (RightButton.dclick)
	{
		RightButton.dclick = FALSE;
		SendDoubleClicked(ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, DBL_CLCK_RIGHT);
	}
	else
	{
		if (RightButton.lose_up)
		{
			SendClicked(ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos,CLCK_RIGHT);
			RightButton.lose_up = FALSE;
		}
		else
		{
			SendPressed(ScilabGC->CurWindow,MOUSEX+horzsinPos,MOUSEY+vertsinPos,PRESSED_RIGHT);
		}
	}
	RightButton.wait_dclick = FALSE;
}

/*-----------------------------------------------------------------------------------*/
static void KillTimerRight(HWND hwnd)
{
	KillTimer(hwnd,RightButton.TimerId);
	RightButton.wait_dclick = FALSE;
	RightButton.dclick = FALSE;
	RightButton.lose_up = FALSE;
	RightButton.shortcut_move = FALSE;
}
/*-----------------------------------------------------------------------------------*/
static void SetTimerRight(HWND hwnd)
{
	SetTimer(hwnd,RightButton.TimerId,GetDoubleClickTime()/2,reset_click_counter_right);
	RightButton.wait_dclick = TRUE;
    RightButton.dclick = FALSE;
    RightButton.lose_up = FALSE;
}
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_MBUTTONDOWN(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	if (focus) 
	{
		KillTimers(hwnd);
		focus = FALSE;

		MOUSEX= x;
		MOUSEY= y;

		SetTimerMiddle(hwnd);
		return ;
	}
	MOUSEX= x;
	MOUSEY= y;

	if (MiddleButton.wait_dclick) 
	{
	/* If we are waiting for a double-click, we only handle Button events
      in a special fashion.*/

	  KillTimers(hwnd);
	  MiddleButton.dclick = TRUE;

      return;
	}

	SetTimerMiddle(hwnd);
}
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_MBUTTONUP(HWND hwnd, int x, int y, UINT keyFlags)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	int horzsinPos=ScilabGC->horzsi.nPos;
	int vertsinPos=ScilabGC->vertsi.nPos;

	if (focus) 
	{
		KillTimers(hwnd);
		focus = FALSE;
		return ;
	}

	MOUSEX= x;
	MOUSEY= y;

	if (MiddleButton.wait_dclick) 
	{
		MiddleButton.lose_up = TRUE;
		return;
	}
	if (MiddleButton.dclick)  return;
	/* button released */
	SendReleased(ScilabGC->CurWindow, x, y,RELEASED_MIDDLE);
}
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_MBUTTONDBLCLK(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{

	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	int horzsinPos=ScilabGC->horzsi.nPos;
	int vertsinPos=ScilabGC->vertsi.nPos;

	if (focus) 
	{
		focus = FALSE;
		return ;
	}

	MOUSEX= x;
	MOUSEY= y;

	SendDoubleClicked(ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos,DBL_CLCK_MIDDLE);

	KillTimers(hwnd);
	MiddleButton.dclick = TRUE;
}
/*-----------------------------------------------------------------------------------*/
static void CALLBACK reset_click_counter_middle(HWND hwnd,UINT msg,UINT_PTR id,DWORD data)
{
	struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
	int horzsinPos=ScilabGC->horzsi.nPos;
	int vertsinPos=ScilabGC->vertsi.nPos;

	KillTimer(hwnd,MiddleButton.TimerId);

	if (focus) 
	{
		focus = FALSE;
		return ;
	}

	if (MiddleButton.dclick)
	{
		MiddleButton.dclick = FALSE;
		SendDoubleClicked(ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos,DBL_CLCK_MIDDLE);
	}
	else
	{
		if (MiddleButton.lose_up)
		{
			SendClicked(ScilabGC->CurWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos,CLCK_MIDDLE);
			MiddleButton.lose_up = FALSE;
		}
		else
		{
			SendPressed(ScilabGC->CurWindow,MOUSEX+horzsinPos,MOUSEY+vertsinPos,PRESSED_MIDDLE);
		}
	}
	MiddleButton.wait_dclick = FALSE;
}

/*-----------------------------------------------------------------------------------*/
static void KillTimerMiddle(HWND hwnd)
{
	KillTimer(hwnd,MiddleButton.TimerId);
	MiddleButton.wait_dclick = FALSE;
	MiddleButton.dclick = FALSE;
	MiddleButton.lose_up = FALSE;
	MiddleButton.shortcut_move = FALSE;
}
/*-----------------------------------------------------------------------------------*/
static void SetTimerMiddle(HWND hwnd)
{
	SetTimer(hwnd,MiddleButton.TimerId,GetDoubleClickTime()/2,reset_click_counter_middle);
	MiddleButton.wait_dclick = TRUE;
    MiddleButton.dclick = FALSE;
    MiddleButton.lose_up = FALSE;
}
/*-----------------------------------------------------------------------------------*/
static void SendPressed(int winId,int x,int y,int buttonId)
{
	if (GetKeyState(VK_CONTROL)<0)
	{
		printf("%d BUTTON PRESS + CTRL %d\n",buttonId,winId);
		PushClickQueue (winId, x,y, CTRL_KEY+buttonId, 0, 0);
	}
	else
	if (GetKeyState(VK_SHIFT)<0)
	{
		printf("%d BUTTON PRESS + SHIFT %d\n",buttonId,winId);
		PushClickQueue (winId, x,y, SHIFT_KEY+buttonId, 0, 0);
	}
	else
	{
		printf("%d BUTTON PRESS %d\n",buttonId,winId);
		PushClickQueue (winId,x,y,buttonId, 0, 0);
	}
}
/*-----------------------------------------------------------------------------------*/
static void SendReleased(int winId,int x,int y,int buttonId)
{
	if (GetKeyState(VK_CONTROL)<0)
	{
		printf("%d CTRL+RELEASE %d\n",buttonId,winId);
		PushClickQueue (winId, x,y, buttonId +CTRL_KEY, 0, -1);
	}
	else
	if (GetKeyState(VK_SHIFT)<0)
	{
		printf("%d SHIFT+RELEASE %d\n",buttonId,winId);
		PushClickQueue (winId, x,y, buttonId +SHIFT_KEY, 0, -1);
	}
	else
	{
		printf("%d RELEASE %d\n",buttonId,winId);
		PushClickQueue (winId, x,y, buttonId , 0, -1);
	}
}
/*-----------------------------------------------------------------------------------*/
static void SendClicked(int winId,int x,int y,int buttonId)
{
	if (GetKeyState(VK_CONTROL)<0)
	{
		printf("%d BUTTON CTRL + CLICK %d\n",buttonId,winId);
		PushClickQueue (winId, x,y, CTRL_KEY+buttonId, 0, 0);
	}
	else
	if (GetKeyState(VK_SHIFT)<0)
	{
		printf("%d BUTTON SHIFT + CLICK %d\n",buttonId,winId);
		PushClickQueue (winId, x,y, SHIFT_KEY+buttonId, 0, 0);
	}
	else
	{
		printf("%d BUTTON CLICK %d\n",buttonId,winId);
		PushClickQueue (winId, x,y,buttonId, 0, 0);
	}
}
/*-----------------------------------------------------------------------------------*/
static void SendDoubleClicked(int winId,int x,int y,int buttonId)
{
	if (GetKeyState(VK_CONTROL)<0)
	{
		printf("%d BUTTON DBLCLK + CTRL %d\n",buttonId,winId);
		PushClickQueue (winId,x,y, CTRL_KEY+buttonId, 0, 0);
	}
	else
	if (GetKeyState(VK_SHIFT)<0)
	{
		printf("%d BUTTON DBLCLK + SHIFT %d\n",buttonId,winId);
		PushClickQueue (winId,x,y, SHIFT_KEY+buttonId, 0, 0);
	}
	else
	{
		printf("%d BUTTON DBLCLK %d\n",buttonId,winId);
		PushClickQueue (winId, x,y, buttonId, 0, 0);
	}
}
/*-----------------------------------------------------------------------------------*/
static void SendMove(int winId,int x,int y)
{
	PushClickQueue (winId,x,y, -1, 1, 0);
}
/*-----------------------------------------------------------------------------------*/