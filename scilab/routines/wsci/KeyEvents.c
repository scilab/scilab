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

/* Code à la linux */
#define SCI_VK_CANCEL	66377
#define SCI_VK_PAUSE	65299
#define SCI_VK_SLEEP	65300 
#define SCI_VK_INSERT	65379
#define SCI_VK_DELETE	65535
#define SCI_VK_END	65367
#define SCI_VK_HOME	65360
#define SCI_VK_PRIOR	65365
#define SCI_VK_NEXT	65366
#define SCI_VK_BACK	65288
#define SCI_VK_NUMLOCK 65407 
#define SCI_VK_SCROLL 65300 
#define SCI_VK_SNAPSHOT 0
#define SCI_VK_TAB 65289
#define SCI_VK_ESCAPE 65307
#define SCI_VK_CLEAR 65408
#define SCI_VK_LEFT 65361
#define SCI_VK_F1 65470

/*-----------------------------------------------------------------------------------*/
BOOL IsASpecialKey(UINT vk);
UINT GetScilabSpecialKeyCode(UINT vk);
BOOL is_dead_key (int vk); 
/*-----------------------------------------------------------------------------------*/
/* http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winui/winui/WindowsUserInterface/UserInput/VirtualKeyCodes.asp */
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_KEYUP(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	if ( is_dead_key (vk) )
	{
		// Nothing to do
	}
	else
	{
		if (IsASpecialKey(vk))
		{
			struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
			int x,y,iwin;
			check_pointer_win(&x,&y,&iwin);
			PushClickQueue (ScilabGC->CurWindow, x,y,-(int)GetScilabSpecialKeyCode(vk),0,1);
		}
		else
		{
			WORD Char=0;
			BYTE KeyState[256]; // Etat des 256 touches du clavier 
			memset ( KeyState, 0, sizeof(KeyState) ); 
			if ( GetKeyboardState ( KeyState ) ) 
			{
				int RetToAscii=0;
				int x,y,iwin;
				struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
				int CodeKey=0;

				RetToAscii=ToAscii ( (UINT) vk,MapVirtualKey(vk, 0), KeyState, &Char, 0 );
				if (RetToAscii)
				{
					if (GetKeyState (VK_CONTROL) < 0)
					{
						if ((Char>=1) && (Char<=29))
						{
							CodeKey=('a'-1+Char)+CTRL_KEY;
						}
					}
					else CodeKey=Char;

					check_pointer_win(&x,&y,&iwin);
					PushClickQueue (ScilabGC->CurWindow, x,y,-(int)CodeKey,0,1);
				}
			}
		}
	}
}
/*-----------------------------------------------------------------------------------*/
void ON_EVENT_GRAPH_WM_KEYDOWN(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	if ( is_dead_key (vk) )
	{
		// Nothing to do
	}
	else
	{
		if (IsASpecialKey(vk))
		{
			struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
			int x,y,iwin;
			check_pointer_win(&x,&y,&iwin);
			PushClickQueue (ScilabGC->CurWindow, x,y,(int)GetScilabSpecialKeyCode(vk),0,0);
		}
		else
		{
			WORD Char=0;
			BYTE KeyState[256]; // Etat des 256 touches du clavier 
			memset ( KeyState, 0, sizeof(KeyState) ); 
			if ( GetKeyboardState ( KeyState ) ) 
			{
				int RetToAscii=0;
				int x,y,iwin;
				struct BCG *ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
				int CodeKey=0;

				RetToAscii=ToAscii ( (UINT) vk,MapVirtualKey(vk, 0), KeyState, &Char, 0 );
				if (RetToAscii)
				{
					if (GetKeyState (VK_CONTROL) < 0)
					{
						if ((Char>=1) && (Char<=29))
						{
							CodeKey=('a'-1+Char)+CTRL_KEY;
						}
					}
					else CodeKey=Char;

					check_pointer_win(&x,&y,&iwin);
					PushClickQueue (ScilabGC->CurWindow, x,y,(int)CodeKey,0,0);
				}
			}
		}
	}
}
/*-----------------------------------------------------------------------------------*/
BOOL IsASpecialKey(UINT vk)
{
	BOOL bOK=FALSE;

	if (vk == VK_CANCEL) return TRUE;
	if (vk == VK_PAUSE) return TRUE;
	if (vk == VK_SLEEP) return TRUE;
	if (vk == VK_INSERT) return TRUE;
	if (vk == VK_DELETE) return TRUE;
	if (vk == VK_END) return TRUE;
	if (vk == VK_HOME) return TRUE;
	if (vk == VK_PRIOR) return TRUE;
	if (vk == VK_NEXT) return TRUE;
	if (vk == VK_BACK) return TRUE;
	if (vk == VK_NUMLOCK) return TRUE;
	if (vk == VK_SCROLL) return TRUE;
	if (vk == VK_TAB) return TRUE;
	if (vk == VK_ESCAPE ) return TRUE;
	if (vk == VK_SNAPSHOT ) return TRUE;
	if (vk == VK_CLEAR ) return TRUE;
	if ( (vk>=VK_LEFT) && (vk<=VK_DOWN) ) return TRUE;
	if ( (vk>=VK_F1) && (vk<=VK_F24) ) return TRUE;

	return FALSE;
}
/*-----------------------------------------------------------------------------------*/
UINT GetScilabSpecialKeyCode(UINT vk)
{
	UINT ScilabSpecialKeyCode=0;

	if (vk == VK_CANCEL)  ScilabSpecialKeyCode=SCI_VK_CANCEL;
	if (vk == VK_PAUSE)  ScilabSpecialKeyCode=SCI_VK_PAUSE;
	if (vk == VK_SLEEP)  ScilabSpecialKeyCode=SCI_VK_SLEEP; 
	if (vk == VK_INSERT) ScilabSpecialKeyCode=SCI_VK_INSERT;
	if (vk == VK_DELETE) ScilabSpecialKeyCode=SCI_VK_DELETE;
	if (vk == VK_END)  ScilabSpecialKeyCode=SCI_VK_END;
	if (vk == VK_HOME)  ScilabSpecialKeyCode=SCI_VK_HOME;
	if (vk == VK_PRIOR)  ScilabSpecialKeyCode=SCI_VK_PRIOR;
	if (vk == VK_NEXT)  ScilabSpecialKeyCode=SCI_VK_NEXT;
	if (vk == VK_BACK)  ScilabSpecialKeyCode=SCI_VK_BACK;
	if (vk == VK_NUMLOCK)  ScilabSpecialKeyCode=SCI_VK_NUMLOCK; 
	if (vk == VK_SCROLL)  ScilabSpecialKeyCode=SCI_VK_SCROLL; 
	if (vk == VK_SNAPSHOT ) ScilabSpecialKeyCode=SCI_VK_SNAPSHOT;
	if (vk == VK_TAB)   ScilabSpecialKeyCode=SCI_VK_TAB;
	if (vk ==  VK_ESCAPE ) ScilabSpecialKeyCode=SCI_VK_ESCAPE;
  if (vk == VK_CLEAR ) ScilabSpecialKeyCode=SCI_VK_CLEAR;
	if ( (vk>=VK_LEFT) && (vk<=VK_DOWN) )  ScilabSpecialKeyCode= (SCI_VK_LEFT-VK_LEFT)+vk;
	if ( (vk>=VK_F1) && (vk<=VK_F24) )  ScilabSpecialKeyCode= (SCI_VK_F1-VK_F1)+vk;

	return ScilabSpecialKeyCode;
}
/*-----------------------------------------------------------------------------------*/
BOOL is_dead_key (int vk) 
{ 
	unsigned int code = MapVirtualKey ( vk, 2 ); 

	// Windows 95 retourne 0x8000, NT retourne 0x80000000 
	return (code & 0x80008000) ? TRUE : FALSE; 
} 
/*-----------------------------------------------------------------------------------*/
