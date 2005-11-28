/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "KeyEvents.h"
/*-----------------------------------------------------------------------------------*/
extern int check_pointer_win __PARAMS ((int *x1,int *y1,int *win));
/*-----------------------------------------------------------------------------------*/
#define CTRL_KEY 1000
#define SHIFT_KEY 2000
#ifndef VK_SLEEP
  #define VK_SLEEP 0x5F
#endif
/*-----------------------------------------------------------------------------------*/
BOOL IsASpecialKey(UINT vk);
UINT GetScilabSpecialKeyCode(UINT vk);
/*-----------------------------------------------------------------------------------*/
/* http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winui/winui/WindowsUserInterface/UserInput/VirtualKeyCodes.asp */
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_KEYUP(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	if ( IsASpecialKey(vk) )
	{
		struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
		int x,y,iwin;

		check_pointer_win(&x,&y,&iwin);
		PushClickQueue (ScilabGC->CurWindow, x,y,-(int)GetScilabSpecialKeyCode(vk),0,1);
	}
	else
	{
		int CodeKey=0;
		int RetToAscii=0;
		BYTE KeyState[256]; // address of key-state array
		WORD Char; // buffer for translated key
		UINT VirtualKey=0;

		GetKeyboardState(KeyState);
		RetToAscii=ToAscii ( vk, 0, KeyState, &Char, 0 );
		VirtualKey=MapVirtualKey(vk,2);
		if (RetToAscii == 1)
		{
			int x,y,iwin;
			struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);

			if ( (GetKeyState (VK_CONTROL) < 0) && (GetKeyState (VK_SHIFT) <0) )
			{
				CodeKey=VirtualKey+CTRL_KEY;
			}
			else
			{
				if (GetKeyState (VK_CONTROL) < 0)
				{
					CodeKey=tolower((char)VirtualKey)+CTRL_KEY;
				}
				else if (GetKeyState (VK_SHIFT) < 0)
				{
					CodeKey=VirtualKey;
				}
				else
				{
					CodeKey= tolower((char)Char);
				}
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

}
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_KEYDOWN(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	if ( IsASpecialKey(vk) )
	{
		struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
		int x,y,iwin;

		check_pointer_win(&x,&y,&iwin);
		PushClickQueue (ScilabGC->CurWindow, x,y,(int)GetScilabSpecialKeyCode(vk),0,0);
	}
	else
	{
		int CodeKey=0;
		int RetToAscii=0;
		BYTE KeyState[256]; // address of key-state array
		WORD Char; // buffer for translated key
		UINT VirtualKey=0;

		GetKeyboardState(KeyState);
		RetToAscii=ToAscii ( vk, 0, KeyState, &Char, 0 );
		VirtualKey=MapVirtualKey(vk,2);
		if (RetToAscii == 1)
		{
			int x,y,iwin;
			struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);

			if ( (GetKeyState (VK_CONTROL) < 0) && (GetKeyState (VK_SHIFT) <0) )
			{
				CodeKey=VirtualKey+CTRL_KEY;
			}
			else
			{
				if (GetKeyState (VK_CONTROL) < 0)
				{
					CodeKey=tolower((char)VirtualKey)+CTRL_KEY;
				}
				else if (GetKeyState (VK_SHIFT) < 0)
				{
					CodeKey=VirtualKey;
				}
				else
				{
					CodeKey= tolower((char)Char);
				}
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
}
/*-----------------------------------------------------------------------------------*/
BOOL IsASpecialKey(UINT vk)
{
	BOOL bOK=FALSE;

	if (vk == VK_CANCEL) return TRUE;
	if (vk == VK_PAUSE) return TRUE;
	if (vk == VK_SNAPSHOT) return TRUE;
	if (vk == VK_SLEEP) return TRUE;
	if (vk == VK_INSERT) return TRUE;
	if (vk == VK_END) return TRUE;
	if (vk == VK_HOME) return TRUE;
	if (vk == VK_PRIOR) return TRUE;
	if (vk == VK_NEXT) return TRUE;
	if (vk == VK_BACK) return TRUE;
	if (vk == VK_NUMLOCK) return TRUE;
	if (vk == VK_TAB) return TRUE;
	if (vk ==  VK_ESCAPE ) return TRUE;
	if ( (vk>=VK_LEFT) && (vk<=VK_DOWN) ) return TRUE;
	if ( (vk>=VK_F1) && (vk<=VK_F24) ) return TRUE;

	return FALSE;
}
/*-----------------------------------------------------------------------------------*/
UINT GetScilabSpecialKeyCode(UINT vk)
{
	UINT ScilabSpecialKeyCode=0;

	if (vk == VK_CANCEL)  ScilabSpecialKeyCode=65288;
	if (vk == VK_PAUSE)  ScilabSpecialKeyCode=65299;
	if (vk == VK_SNAPSHOT)  ScilabSpecialKeyCode=0;
	if (vk == VK_SLEEP)  ScilabSpecialKeyCode=65300; 

	if (vk == VK_INSERT) ScilabSpecialKeyCode=65379;

	if (vk == VK_END)  ScilabSpecialKeyCode=65367;
	if (vk == VK_HOME)  ScilabSpecialKeyCode=65360;
	if (vk == VK_PRIOR)  ScilabSpecialKeyCode=65365;
	if (vk == VK_NEXT)  ScilabSpecialKeyCode=65366;
	if (vk == VK_BACK)  ScilabSpecialKeyCode=65289;

	if (vk == VK_NUMLOCK)  ScilabSpecialKeyCode=65407; 

	if (vk == VK_TAB)   ScilabSpecialKeyCode=65289;
	if (vk ==  VK_ESCAPE ) ScilabSpecialKeyCode=65307;

	if ( (vk>=VK_LEFT) && (vk<=VK_DOWN) )  ScilabSpecialKeyCode= (65361-VK_LEFT)+vk;
	if ( (vk>=VK_F1) && (vk<=VK_F24) )  ScilabSpecialKeyCode= (65470-VK_F1)+vk;

	return ScilabSpecialKeyCode;
}
/*-----------------------------------------------------------------------------------*/
