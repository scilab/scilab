/* Allan CORNET */
/* INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
#include "Events.h"
/*-----------------------------------------------------------------------------------*/
extern int check_pointer_win __PARAMS ((int *x1,int *y1,int *win));
/*-----------------------------------------------------------------------------------*/
int GetEventKeyboardAndMouse(  UINT message, WPARAM wParam, LPARAM lParam,struct BCG *ScilabGC)
{
	int CodeKey=0;
	int x,y,iwin;

	switch(message)
	{
		case WM_KEYDOWN:
		/*http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winui/winui/WindowsUserInterface/UserInput/VirtualKeyCodes.asp*/
		{
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
			PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + 
			ScilabGC->horzsi.nPos,HIWORD (lParam) + ScilabGC->vertsi.nPos, 0, 0, 0);
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

		case WM_LBUTTONUP:
		{
			PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + 
			ScilabGC->horzsi.nPos,HIWORD (lParam) + ScilabGC->vertsi.nPos, -5, 0, 1);
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

		case WM_LBUTTONDBLCLK:
		{
			
			switch(wParam)
			{
				case MK_CONTROL:
					PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + 
					ScilabGC->horzsi.nPos,HIWORD (lParam) + ScilabGC->vertsi.nPos, CTRL_DBL_CLCK_LEFT, 0, 1);
				break;

				case MK_MBUTTON:
					
				break;

				case MK_RBUTTON:
					
				break;

				case MK_SHIFT:
					
				break;

				default:
					wait_for_click=1;
					PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + 
					ScilabGC->horzsi.nPos,HIWORD (lParam) + ScilabGC->vertsi.nPos, DBL_CLCK_LEFT, 0, 1);
				break;
			}
		}
		return (0);
	}

 //  /*  

	//switch (message)
	//{
	//	case WM_LBUTTONUP:
	//		{
	//			switch(wParam)
	//			{
	//				case MK_CONTROL:
	//					sciprint("WM_LBUTTONUP & ctrl\n");
	//					PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + 
	//				ScilabGC->horzsi.nPos,HIWORD (lParam) + ScilabGC->vertsi.nPos, CTRL_DBL_CLCK_LEFT, 0, 1);
	//					break;

	//				case MK_MBUTTON:
	//					sciprint("WM_LBUTTONUP & bouton milieu\n");
	//					break;

	//				case MK_RBUTTON:
	//					sciprint("WM_LBUTTONUP & bouton droit\n");
	//					break;

	//				case MK_SHIFT:
	//					sciprint("WM_LBUTTONUP & touche shift\n");
	//					break;
	//				default:
	//					sciprint("WM_LBUTTONUP \n");
	//					break;
	//			}
	//		}
	//	break;

	//	case WM_RBUTTONUP:
	//		{
	//			switch(wParam)
	//			{
	//				case MK_CONTROL:
	//					sciprint("WM_RBUTTONUP & ctrl\n");
	//					break;

	//				case MK_MBUTTON:
	//					sciprint("WM_RBUTTONUP & mmiddle buton\n");
	//					break;

	//				case MK_LBUTTON:
	//					sciprint("WM_RBUTTONUP & left button\n");
	//					break;

	//				case MK_SHIFT:
	//					sciprint("WM_RBUTTONUP & shift\n");
	//					break;
	//				default:
	//					sciprint("WM_RBUTTONUP\n");
	//					break;
	//			}
	//		}
	//	break;

	//	case WM_MBUTTONUP:
	//		{
	//			switch(wParam)
	//			{
	//				case MK_CONTROL:
	//					sciprint("WM_MBUTTONUP & ctrl\n");
	//					break;

	//				case MK_LBUTTON:
	//					sciprint("WM_MBUTTONUP & left button\n");
	//					break;

	//				case MK_RBUTTON:
	//					sciprint("WM_MBUTTONUP & right button\n");
	//					break;

	//				case MK_SHIFT:
	//					sciprint("WM_MBUTTONUP & shift\n");
	//					break;
	//				default:
	//					sciprint("WM_MBUTTONUP\n");
	//					break;
	//			}
	//		}
	//	break;

	//	case WM_KEYDOWN:
	//		{
	//			
	//			sciprint("a key %d\n",wParam);

	//		
	//		}
	//	break;

	//	case WM_LBUTTONDBLCLK:
	//		{
	//			switch(wParam)
	//			{
	//				case MK_CONTROL:
	//					sciprint("WM_LBUTTONDBLCLK & contrl\n");
	//					break;

	//				case MK_MBUTTON:
	//					sciprint("WM_LBUTTONDBLCLK & middle button\n");
	//					break;

	//				case MK_RBUTTON:
	//					sciprint("WM_LBUTTONDBLCLK & right button\n");
	//					break;

	//				case MK_SHIFT:
	//					sciprint("WM_LBUTTONDBLCLK & shift\n");
	//					break;
	//				default:
	//					sciprint("WM_LBUTTONDBLCLK\n");
	//					break;

	//			}
	//		}
	//	break;

	//	case WM_MBUTTONDBLCLK:
	//		{
	//			switch(wParam)
	//			{
	//				case MK_CONTROL:
	//					sciprint("WM_MBUTTONDBLCLK & contrl\n");
	//					break;

	//				case MK_LBUTTON:
	//					sciprint("WM_MBUTTONDBLCLK & left button\n");
	//					break;

	//				case MK_RBUTTON:
	//					sciprint("WM_MBUTTONDBLCLK & right button\n");
	//					break;

	//				case MK_SHIFT:
	//					sciprint("WM_MBUTTONDBLCLK & shift\n");
	//					break;
	//				default:
	//					sciprint("WM_MBUTTONDBLCLK\n");
	//					break;
	//			}
	//		}
	//	break;

	//	case WM_RBUTTONDBLCLK:
	//		{
	//			switch(wParam)
	//			{
	//				case MK_CONTROL:
	//					sciprint("WM_RBUTTONDBLCLK & control\n");
	//					break;

	//				case MK_MBUTTON:
	//					sciprint("WM_RBUTTONDBLCLK & middle button\n");
	//					break;

	//				case MK_LBUTTON:
	//					sciprint("WM_RBUTTONDBLCLK & left button\n");
	//					break;

	//				case MK_SHIFT:
	//					sciprint("WM_RBUTTONDBLCLK & shift button\n");
	//					break;
	//				default:
	//					sciprint("WM_RBUTTONDBLCLK\n");
	//					break;
	//			}
	//		}
	//	break;

	//}*/
	return CodeKey;
}
/*-----------------------------------------------------------------------------------*/
