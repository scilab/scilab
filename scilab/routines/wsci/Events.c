/* Allan CORNET */
/* INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
#include "Events.h"
/*-----------------------------------------------------------------------------------*/
extern int check_pointer_win __PARAMS ((int *x1,int *y1,int *win));
/*-----------------------------------------------------------------------------------*/

static BOOL bTimerLeftPressedON=FALSE;
static BOOL LeftPressedON=FALSE;
static BOOL bTimerLeftSingleClickON=FALSE;

static BOOL bTimerRightPressedON=FALSE;
static BOOL RightPressedON=FALSE;
static BOOL bTimerRightSingleClickON=FALSE;

static BOOL bTimerMiddlePressedON=FALSE;
static BOOL MiddlePressedON=FALSE;
static BOOL bTimerMiddleSingleClickON=FALSE;

static struct BCG *LocalScilabGC=NULL;

static int CurrentWindow=0;
static int MOUSEX=0;
static int MOUSEY=0;
static int horzsinPos=0;
static int vertsinPos=0;

/*-----------------------------------------------------------------------------------*/
void CALLBACK LeftPressedClick(HWND hwnd,UINT msg,UINT_PTR id,DWORD data)
{
	KillTimer(hwnd, id);
	bTimerLeftPressedON=FALSE;
	LeftPressedON=TRUE;
	

	if (GetKeyState(VK_CONTROL)<0)
	{
		/*sciprint("CONTROL + Left Button Pressed\n");*/
		PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, PRESSED_LEFT+CTRL_KEY, 0, 0);
	}
	else
	{
		/*sciprint("Left Button Pressed\n");*/
		PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, PRESSED_LEFT, 0, 0);
	}
	
}
/*-----------------------------------------------------------------------------------*/
void CALLBACK LeftSingleClick(HWND hwnd,UINT msg,UINT_PTR id,DWORD data)
{
	KillTimer(hwnd, id);
	bTimerLeftSingleClickON=FALSE;
	
	if (GetKeyState(VK_CONTROL)<0)
	{
		/*sciprint("CONTROL + Single Left Click\n");*/
		PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, CLCK_LEFT+CTRL_KEY, 0, 0);
	}
	else
	{
		/*sciprint("Single Left Click\n");*/
		PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, CLCK_LEFT, 0, 0);
	}
}
/*-----------------------------------------------------------------------------------*/
void CALLBACK MiddlePressedClick(HWND hwnd,UINT msg,UINT_PTR id,DWORD data)
{
	KillTimer(hwnd, id);
	bTimerMiddlePressedON=FALSE;
	MiddlePressedON=TRUE;
	
	if (GetKeyState(VK_CONTROL)<0)
	{
		/*sciprint("CONTROL + Middle Button Pressed\n");*/
		PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, PRESSED_MIDDLE+CTRL_KEY, 0, 0);
	}
	else
	{
		/*sciprint("Middle Button Pressed\n");*/
		PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, PRESSED_MIDDLE, 0, 0);
	}
}
/*-----------------------------------------------------------------------------------*/
void CALLBACK MiddleSingleClick(HWND hwnd,UINT msg,UINT_PTR id,DWORD data)
{
	KillTimer(hwnd, id);
	bTimerMiddleSingleClickON=FALSE;
	
	if (GetKeyState(VK_CONTROL)<0)
	{
		/*sciprint("CONTROL + Single Middle Click\n");*/
		PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, CLCK_MIDDLE+CTRL_KEY, 0, 0);
	}
	else
	{
		/*sciprint("Single Middle Click\n");*/
		PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, CLCK_MIDDLE, 0, 0);
	}

}
/*-----------------------------------------------------------------------------------*/
void CALLBACK RightPressedClick(HWND hwnd,UINT msg,UINT_PTR id,DWORD data)
{
	KillTimer(hwnd, id);
	bTimerRightPressedON=FALSE;
	RightPressedON=TRUE;

	if (GetKeyState(VK_CONTROL)<0)
	{
		/*sciprint("CONTROL + Right Button Pressed\n");*/
		PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, PRESSED_RIGHT+CTRL_KEY, 0, 0);
	}
	else
	{
		/*sciprint("Right Button Pressed\n");*/
		PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, PRESSED_RIGHT, 0, 0);
	}
}
/*-----------------------------------------------------------------------------------*/
void CALLBACK RightSingleClick(HWND hwnd,UINT msg,UINT_PTR id,DWORD data)
{
	KillTimer(hwnd, id);
	bTimerRightSingleClickON=FALSE;

	if (GetKeyState(VK_CONTROL)<0)
	{
		/*sciprint("CONTROL + Single Right Click\n");*/
		PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, CLCK_RIGHT+CTRL_KEY, 0, 0);
		
	}
	else
	{
		/*sciprint("Single Right Click\n");*/
		PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, CLCK_RIGHT, 0, 0);
	}
}
/*-----------------------------------------------------------------------------------*/
int GetEventKeyboardAndMouse(  UINT message, WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC)
{
	int CodeKey=0;
	int x,y,iwin;

	LocalScilabGC=ScilabGC;

	switch(message)
	{
		case WM_KEYDOWN:
		/*http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winui/winui/WindowsUserInterface/UserInput/VirtualKeyCodes.asp*/
		{
			int RetToAscii=0;
			BYTE KeyState[256]; // address of key-state array
			WORD Char; // buffer for translated key
			GetKeyboardState(KeyState);

			RetToAscii=ToAscii ( wParam, 0, KeyState, &Char, 0 );
			if (RetToAscii == 1)
			{
				if ( (GetKeyState (VK_CONTROL) < 0) && (GetKeyState (VK_SHIFT) <0) )
				{
					CodeKey=MapVirtualKey(wParam,2)+CTRL_KEY;
				}
				else
				if (GetKeyState (VK_CONTROL) < 0)
				{
					CodeKey=tolower((char)MapVirtualKey(wParam,2))+CTRL_KEY;
				}
				else
				if (GetKeyState (VK_SHIFT) < 0)
				{
					CodeKey=MapVirtualKey(wParam,2);
				}
				else
				{
					CodeKey= tolower((char)Char);
				}
				check_pointer_win(&x,&y,&iwin);
				PushClickQueue (ScilabGC->CurWindow, x,y,(int)CodeKey,0,0);
			}
			else
			{
				/* Not a character */
				/* A key */
			}
		}
		return (0);

		case WM_KEYUP:
		{
			int RetToAscii=0;
			BYTE KeyState[256]; // address of key-state array
			WORD Char; // buffer for translated key
			GetKeyboardState(KeyState);

			RetToAscii=ToAscii ( wParam, 0, KeyState, &Char, 0 );
			if (RetToAscii == 1)
			{
				if ( (GetKeyState (VK_CONTROL) < 0) && (GetKeyState (VK_SHIFT) <0) )
				{
					CodeKey=MapVirtualKey(wParam,2)+CTRL_KEY;
				}
				else
				if (GetKeyState (VK_CONTROL) < 0)
				{
					CodeKey=tolower((char)MapVirtualKey(wParam,2))+CTRL_KEY;
				}
				else
				if (GetKeyState (VK_SHIFT) < 0)
				{
					CodeKey=MapVirtualKey(wParam,2);
				}
				else
				{
					CodeKey= tolower((char)Char);
				}
				check_pointer_win(&x,&y,&iwin);
				PushClickQueue (ScilabGC->CurWindow, x,y,-(int)CodeKey,0,1);
			}
			else
			{
				/* Not a character */
				/* A key */
			}
		}
		return (0);

		case WM_LBUTTONDOWN:
		{
			ON_WM_LBUTTONDOWN(wParam,lParam,ScilabGC);
		}
		return (0);

		case WM_LBUTTONUP:
		{
			ON_WM_LBUTTONUP(wParam,lParam,ScilabGC);
		}
		return (0);

		case WM_LBUTTONDBLCLK:
		{
			ON_WM_LBUTTONDBLCLK(wParam,lParam,ScilabGC);
		}
		return (0);

		case WM_RBUTTONDOWN:
		{
			ON_WM_RBUTTONDOWN(wParam,lParam,ScilabGC);
		}
		return (0);

		case WM_RBUTTONUP:
		{
			ON_WM_RBUTTONUP(wParam,lParam,ScilabGC);
		}
		return (0);

		case WM_RBUTTONDBLCLK:
		{
			ON_WM_RBUTTONDBLCLK(wParam,lParam,ScilabGC);
		}
		return (0);

		case WM_MBUTTONDOWN:
		{
			ON_WM_MBUTTONDOWN(wParam,lParam,ScilabGC);
		}
		return (0);

		case WM_MBUTTONUP:
		{
			ON_WM_MBUTTONUP(wParam,lParam,ScilabGC);
		}
    	return (0);

		case WM_MBUTTONDBLCLK:
		{
			ON_WM_MBUTTONDBLCLK(wParam,lParam,ScilabGC);
		}
		return (0);

		case WM_MOUSEMOVE:
		{
			ON_WM_MOVE(wParam,lParam,ScilabGC);
		}
		return 0;
	}
	
 	return CodeKey;
}
/*-----------------------------------------------------------------------------------*/
void ON_WM_LBUTTONDOWN(WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC)
{
	CurrentWindow=ScilabGC->CurWindow;
	horzsinPos=ScilabGC->horzsi.nPos;
	vertsinPos=ScilabGC->vertsi.nPos;

	MOUSEX= ((int) LOWORD (lParam));
	MOUSEY= ((int) HIWORD (lParam));

	if (bTimerLeftPressedON) 
	{
		KillTimer(ScilabGC->CWindow, TIMER1LEFTBUTTON);
		bTimerLeftPressedON=FALSE;
	} 
	else
	{	
		bTimerLeftPressedON=TRUE;
		LeftPressedON=FALSE;
		SetTimer(ScilabGC->CWindow, TIMER1LEFTBUTTON, GetDoubleClickTime(),LeftPressedClick);
	}
}
/*-----------------------------------------------------------------------------------*/
void ON_WM_LBUTTONUP(WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC)
{
	CurrentWindow=ScilabGC->CurWindow;
	horzsinPos=ScilabGC->horzsi.nPos;
	vertsinPos=ScilabGC->vertsi.nPos;

	MOUSEX= ((int) LOWORD (lParam));
	MOUSEY= ((int) HIWORD (lParam));

	if (LeftPressedON)
	{
		if (bTimerLeftPressedON) 
		{
			KillTimer(ScilabGC->CWindow, TIMER1LEFTBUTTON);
			bTimerLeftPressedON=FALSE;
		} 

		if (GetKeyState(VK_CONTROL)<0)
		{
			/*sciprint("CONTROL + Left Button Released\n");*/
			PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, RELEASED_LEFT+CTRL_KEY, 0, -1);
		}
		else
		{
			/*sciprint("Left Button Released\n");*/
			PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, RELEASED_LEFT, 0, -1);
		}
				
		LeftPressedON=FALSE;
	}
	else
	{
		if (bTimerLeftPressedON) 
		{
			KillTimer(ScilabGC->CWindow, TIMER1LEFTBUTTON);
			bTimerLeftPressedON=FALSE;
			LeftPressedON=FALSE;
					
			if (bTimerLeftSingleClickON)
			{
				KillTimer(ScilabGC->CWindow, TIMER2LEFTBUTTON);
				bTimerLeftSingleClickON=FALSE;
			}
			else
			{
				bTimerLeftSingleClickON=TRUE;
				SetTimer(ScilabGC->CWindow, TIMER2LEFTBUTTON, GetDoubleClickTime()/2,LeftSingleClick);
			}
		} 
	}
}
/*-----------------------------------------------------------------------------------*/
void ON_WM_LBUTTONDBLCLK(WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC)
{
	CurrentWindow=ScilabGC->CurWindow;
	horzsinPos=ScilabGC->horzsi.nPos;
	vertsinPos=ScilabGC->vertsi.nPos;

	MOUSEX= ((int) LOWORD (lParam));
	MOUSEY= ((int) HIWORD (lParam));

	if (bTimerLeftSingleClickON)
	{
		KillTimer(ScilabGC->CWindow, TIMER2LEFTBUTTON);
		bTimerLeftSingleClickON=FALSE;
	}

	if (bTimerLeftPressedON) 
	{
		KillTimer(ScilabGC->CWindow, TIMER1LEFTBUTTON);
		bTimerLeftPressedON=FALSE;
		LeftPressedON=FALSE;
	}
			
	if (GetKeyState(VK_CONTROL)<0)
	{
		/*sciprint("CONTROL + Double Left Click\n");*/
		PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, CTRL_KEY+DBL_CLCK_LEFT, 0, 0);
	}
	else
	{
		/*sciprint("Double Left Click\n");*/
		PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, DBL_CLCK_LEFT, 0, 0);
	}
}
/*-----------------------------------------------------------------------------------*/
void ON_WM_MBUTTONDOWN(WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC)
{
	CurrentWindow=ScilabGC->CurWindow;
	horzsinPos=ScilabGC->horzsi.nPos;
	vertsinPos=ScilabGC->vertsi.nPos;

	MOUSEX= ((int) LOWORD (lParam));
	MOUSEY= ((int) HIWORD (lParam));

	if (bTimerMiddlePressedON) 
	{
		KillTimer(ScilabGC->CWindow, TIMER1MIDDLEBUTTON);
		bTimerMiddlePressedON=FALSE;
	} 
	else
	{	
		bTimerMiddlePressedON=TRUE;
		MiddlePressedON=FALSE;
		SetTimer(ScilabGC->CWindow, TIMER1MIDDLEBUTTON, GetDoubleClickTime(),MiddlePressedClick);
	}
}
/*-----------------------------------------------------------------------------------*/
void ON_WM_MBUTTONUP(WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC)
{
	CurrentWindow=ScilabGC->CurWindow;
	horzsinPos=ScilabGC->horzsi.nPos;
	vertsinPos=ScilabGC->vertsi.nPos;

	MOUSEX= ((int) LOWORD (lParam));
	MOUSEY= ((int) HIWORD (lParam));

	if (MiddlePressedON)
	{
		if (bTimerMiddlePressedON) 
		{
			KillTimer(ScilabGC->CWindow, TIMER1MIDDLEBUTTON);
			bTimerMiddlePressedON=FALSE;
		} 

		if (GetKeyState(VK_CONTROL)<0)
		{
			/*sciprint("CONTROL + Middle Button Released\n");*/
			PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, RELEASED_MIDDLE+CTRL_KEY, 0, -1);
		}
		else
		{
			/*sciprint("Middle Button Released\n");*/
			PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, RELEASED_MIDDLE, 0, -1);
		}
				
		MiddlePressedON=FALSE;
	}
	else
	{
		if (bTimerMiddlePressedON) 
		{
			KillTimer(ScilabGC->CWindow, TIMER1MIDDLEBUTTON);
			bTimerMiddlePressedON=FALSE;
			MiddlePressedON=FALSE;
					
			if (bTimerMiddleSingleClickON)
			{
				KillTimer(ScilabGC->CWindow, TIMER2MIDDLEBUTTON);
				bTimerMiddleSingleClickON=FALSE;
			}
			else
			{
				bTimerMiddleSingleClickON=TRUE;
				SetTimer(ScilabGC->CWindow, TIMER2MIDDLEBUTTON, GetDoubleClickTime()/2,MiddleSingleClick);
			}
		} 
	}
}
/*-----------------------------------------------------------------------------------*/
void ON_WM_MBUTTONDBLCLK(WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC)
{
	CurrentWindow=ScilabGC->CurWindow;
	horzsinPos=ScilabGC->horzsi.nPos;
	vertsinPos=ScilabGC->vertsi.nPos;

	MOUSEX= ((int) LOWORD (lParam));
	MOUSEY= ((int) HIWORD (lParam));

	if (bTimerMiddleSingleClickON)
	{
		KillTimer(ScilabGC->CWindow, TIMER2MIDDLEBUTTON);
		bTimerMiddleSingleClickON=FALSE;
	}

	if (bTimerMiddlePressedON) 
	{
		KillTimer(ScilabGC->CWindow, TIMER1MIDDLEBUTTON);
		bTimerMiddlePressedON=FALSE;
		MiddlePressedON=FALSE;
	}
			
	if (GetKeyState(VK_CONTROL)<0)
	{
		/*sciprint("CONTROL + Double Middle Click\n");*/
		PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, CTRL_KEY+DBL_CLCK_MIDDLE, 0, 0);
	}
	else
	{
		/*sciprint("Double Middle Click\n");*/
		PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, DBL_CLCK_MIDDLE, 0, 0);
	}
}
/*-----------------------------------------------------------------------------------*/
void ON_WM_RBUTTONDOWN(WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC)
{
	CurrentWindow=ScilabGC->CurWindow;
	horzsinPos=ScilabGC->horzsi.nPos;
	vertsinPos=ScilabGC->vertsi.nPos;

	MOUSEX= ((int) LOWORD (lParam));
	MOUSEY= ((int) HIWORD (lParam));

	if (bTimerRightPressedON) 
	{
		KillTimer(ScilabGC->CWindow, TIMER1RIGHTBUTTON);
		bTimerRightPressedON=FALSE;
	} 
	else
	{	
		bTimerRightPressedON=TRUE;
		RightPressedON=FALSE;
		SetTimer(ScilabGC->CWindow, TIMER1RIGHTBUTTON, GetDoubleClickTime(),RightPressedClick);
	}
}
/*-----------------------------------------------------------------------------------*/
void ON_WM_RBUTTONUP(WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC)
{
	CurrentWindow=ScilabGC->CurWindow;
	horzsinPos=ScilabGC->horzsi.nPos;
	vertsinPos=ScilabGC->vertsi.nPos;

	MOUSEX= ((int) LOWORD (lParam));
	MOUSEY= ((int) HIWORD (lParam));

	if (RightPressedON)
	{
		if (bTimerRightPressedON) 
		{
			KillTimer(ScilabGC->CWindow, TIMER1RIGHTBUTTON);
			bTimerRightPressedON=FALSE;
		} 

		if (GetKeyState(VK_CONTROL)<0)
		{
			/*sciprint("CONTROL + Right Button Released\n");*/
			PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, RELEASED_RIGHT+CTRL_KEY, 0, -1);
		}
		else
		{
			/*sciprint("Right Button Released\n");*/
			PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, RELEASED_RIGHT, 0, -1);
		}
				
		RightPressedON=FALSE;
	}
	else
	{
		if (bTimerRightPressedON) 
		{
			KillTimer(ScilabGC->CWindow, TIMER1RIGHTBUTTON);
			bTimerRightPressedON=FALSE;
			RightPressedON=FALSE;
					
			if (bTimerRightSingleClickON)
			{
				KillTimer(ScilabGC->CWindow, TIMER2RIGHTBUTTON);
				bTimerRightSingleClickON=FALSE;
			}
			else
			{
				bTimerRightSingleClickON=TRUE;
				SetTimer(ScilabGC->CWindow, TIMER2RIGHTBUTTON, GetDoubleClickTime()/2,RightSingleClick);
			}
		} 
	}
}
/*-----------------------------------------------------------------------------------*/
void ON_WM_RBUTTONDBLCLK(WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC)
{
	CurrentWindow=ScilabGC->CurWindow;
	horzsinPos=ScilabGC->horzsi.nPos;
	vertsinPos=ScilabGC->vertsi.nPos;

	MOUSEX= ((int) LOWORD (lParam));
	MOUSEY= ((int) HIWORD (lParam));

	if (bTimerRightSingleClickON)
	{
		KillTimer(ScilabGC->CWindow, TIMER2RIGHTBUTTON);
		bTimerRightSingleClickON=FALSE;
	}

	if (bTimerRightPressedON) 
	{
		KillTimer(ScilabGC->CWindow, TIMER1RIGHTBUTTON);
		bTimerRightPressedON=FALSE;
		RightPressedON=FALSE;
	}
			
	if (GetKeyState(VK_CONTROL)<0)
	{
		/*sciprint("CONTROL + Double Right Click\n");*/
		PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, CTRL_KEY+DBL_CLCK_RIGHT, 0, 0);
	}
	else
	{
		/*sciprint("Double Right Click\n");*/
		PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY + vertsinPos, DBL_CLCK_RIGHT, 0, 0);
	}
}
/*-----------------------------------------------------------------------------------*/
void ON_WM_MOVE(WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC)
{
	horzsinPos=ScilabGC->horzsi.nPos;
	vertsinPos=ScilabGC->vertsi.nPos;

	/* Kill all timer */
	KillTimer(ScilabGC->CWindow, TIMER2LEFTBUTTON);
	KillTimer(ScilabGC->CWindow, TIMER1LEFTBUTTON);
	LeftPressedON=FALSE;
	
	KillTimer(ScilabGC->CWindow, TIMER2MIDDLEBUTTON);
	KillTimer(ScilabGC->CWindow, TIMER1MIDDLEBUTTON);
	MiddlePressedON=FALSE;
	
	KillTimer(ScilabGC->CWindow, TIMER2RIGHTBUTTON);
	KillTimer(ScilabGC->CWindow, TIMER1RIGHTBUTTON);
	RightPressedON=FALSE;
	
	if ( (bTimerLeftPressedON) || (bTimerRightPressedON) || (bTimerMiddlePressedON) ||
		 (bTimerLeftSingleClickON) || (bTimerMiddleSingleClickON) || (bTimerRightSingleClickON) )
	{
		if ( (bTimerLeftPressedON) || (bTimerLeftSingleClickON) )
		{
			if (GetKeyState(VK_CONTROL)<0)
			{
				PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, CTRL_KEY+CLCK_LEFT, 0, 0);
			}
			else
			{
				PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, CLCK_LEFT, 0, 0);
			}

			bTimerLeftSingleClickON=FALSE;
			bTimerLeftPressedON=FALSE;
			return;
		}

		if ( (bTimerMiddlePressedON) || (bTimerMiddleSingleClickON) )
		{
			if (GetKeyState(VK_CONTROL)<0)
			{
				PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, CTRL_KEY+CLCK_MIDDLE, 0, 0);
			}
			else
			{
				PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, CLCK_MIDDLE, 0, 0);
			}
			
			bTimerMiddlePressedON=FALSE;
			bTimerMiddleSingleClickON=FALSE;
			return;
		}

		if( (bTimerRightPressedON) || (bTimerRightSingleClickON) )
		{
			if (GetKeyState(VK_CONTROL)<0)
			{
				PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos,CTRL_KEY+CLCK_RIGHT, 0, 0);
			}
			else
			{
				PushClickQueue (CurrentWindow, MOUSEX+horzsinPos,MOUSEY+vertsinPos, CLCK_RIGHT, 0, 0);
			}
			
			bTimerRightPressedON=FALSE;
			bTimerRightSingleClickON=FALSE;
			return;
		}
	}
	else
	{
		int PosX=(int) LOWORD (lParam);
		int PosY=(int) HIWORD (lParam);
		PushClickQueue (ScilabGC->CurWindow,(int) PosX +horzsinPos,PosY +vertsinPos, -1, 1, 0);
	}
}

/*-----------------------------------------------------------------------------------*/
