/* Allan CORNET */
/* INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
#include "Events.h"
/*-----------------------------------------------------------------------------------*/
extern int check_pointer_win __PARAMS ((int *x1,int *y1,int *win));

static BOOL DoubleClick=FALSE;
static BOOL TimerON=FALSE;
static int Precedent=0;
/*-----------------------------------------------------------------------------------*/
int GetEventKeyboardAndMouse(  UINT message, WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC)
{
	int CodeKey=0;
	int x,y,iwin;

	switch(message)
	{
		case WM_TIMER:
		{
			
		    KillTimer(ScilabGC->CWindow, wParam );

			if ( wParam == WM_LBUTTONDOWN ) 
			{
					//sciprint("bouton pressed\n");

					PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + 
					ScilabGC->horzsi.nPos,HIWORD (lParam) + ScilabGC->vertsi.nPos, 0, 0, 0);
					DoubleClick=FALSE;
					
			}

			if ( wParam == WM_LBUTTONUP )
			{
				if (!DoubleClick)
				{
					if (Precedent==WM_LBUTTONDOWN)
						{
							//sciprint("bouton simple\n");
						}
					else
						{
							//sciprint("bouton released\n");
							Precedent=wParam;
						}

					PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + 
					ScilabGC->horzsi.nPos,HIWORD (lParam) + ScilabGC->vertsi.nPos, -5, 0, 1);
			
				}
				else 
				{
					DoubleClick=FALSE;
					Precedent=wParam;
				}
				
			
			}
		    
		}
		return (0);

		case WM_KEYDOWN:
		/*http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winui/winui/WindowsUserInterface/UserInput/VirtualKeyCodes.asp*/
		{
			/*check_pointer_win(&x,&y,&iwin);
			if (GetKeyState (VK_CONTROL) < 0)
			{
				PushClickQueue (ScilabGC->CurWindow, x,y,1000+MapVirtualKey(wParam,0),0,0);
			}
			else
			{
				PushClickQueue (ScilabGC->CurWindow, x,y,MapVirtualKey(wParam,0),0,0);
			}*/
		}
		return (0);

		case WM_CHAR:
		{
			check_pointer_win(&x,&y,&iwin);
			PushClickQueue (ScilabGC->CurWindow, x,y,wParam,0,0);
		}
		return (0);

		case WM_MOUSEMOVE:
		{
			PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + 
			ScilabGC->horzsi.nPos,HIWORD (lParam) + ScilabGC->vertsi.nPos, -1, 1, 0);
		}
		return 0;

		case WM_LBUTTONDOWN:
		{
			Precedent=message;
			SetTimer(ScilabGC->CWindow,message,GetDoubleClickTime(),NULL);
		}
		return (0);

		case WM_LBUTTONUP:
		{
			Precedent=message;
			SetTimer(ScilabGC->CWindow,message,GetDoubleClickTime(),NULL);
		}
		return (0);


		case WM_LBUTTONDBLCLK:
		{
			KillTimer( ScilabGC->CWindow, WM_LBUTTONDOWN );
			KillTimer(ScilabGC->CWindow, WM_LBUTTONUP );

			PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + 
			ScilabGC->horzsi.nPos,HIWORD (lParam) + ScilabGC->vertsi.nPos, DBL_CLCK_LEFT,0, 0);
		}
		return (0);


		case WM_MBUTTONDOWN:
		{
			PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + 
			ScilabGC->horzsi.nPos,HIWORD (lParam) + ScilabGC->vertsi.nPos, 1, 0, 0);
		}
		return (0);

		case WM_RBUTTONDOWN:
		{
			PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + 
			ScilabGC->horzsi.nPos,HIWORD (lParam) + ScilabGC->vertsi.nPos, 2, 0, 0);
		}
		return (0);


		case WM_MBUTTONUP:
		{
			PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + 
			ScilabGC->horzsi.nPos,HIWORD (lParam) + ScilabGC->vertsi.nPos, -4, 0, 1);
		}
		return (0);

		case WM_RBUTTONUP:
		{
			PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + 
			ScilabGC->horzsi.nPos,HIWORD (lParam) + ScilabGC->vertsi.nPos, -3, 0, 1);
		}
		return (0);

	}

 	return CodeKey;
}
/*-----------------------------------------------------------------------------------*/
