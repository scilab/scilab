/* Allan CORNET */
/* INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
#include "Events.h"
/*-----------------------------------------------------------------------------------*/
extern int check_pointer_win __PARAMS ((int *x1,int *y1,int *win));
/*-----------------------------------------------------------------------------------*/
#define MaxCB 50
/*-----------------------------------------------------------------------------------*/
Scig_deletegwin_handler scig_deletegwin_handler = scig_deletegwin_handler_sci;
static Scig_click_handler scig_click_handler = scig_click_handler_sci;
static But ClickBuf[MaxCB];
static int lastc = 0;
/* used by xclick_any and xclick */ 
static int wait_for_click=0;
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
int PushClickQueue (int win,int x,int y,int ibut,int motion,int release)
{
  /* If we are in xclick_any or xclick then send info */
  if ( wait_for_click==1)
    {

      SciClickInfo.win= win;
      SciClickInfo.x= x;
      SciClickInfo.y= y;
      SciClickInfo.ibutton= ibut;
      SciClickInfo.motion= motion;
      SciClickInfo.release= release;
      return 0;
    }
     
  /* first let a click_handler do the job  */
  if ( scig_click_handler(win,x,y,ibut,motion,release)== 1) return 0;
  /* do not record motion events and release button 
   * this is left for a futur release 
   */
  if (motion == 1 || release == 1)
    return 0;
  if (lastc == MaxCB)
    {
      int i;
      for (i = 1; i < MaxCB; i++)
	{
	  ClickBuf[i - 1] = ClickBuf[i];
	}
      ClickBuf[lastc - 1].win = win;
      ClickBuf[lastc - 1].x = x;
      ClickBuf[lastc - 1].y = y;
      ClickBuf[lastc - 1].ibutton = ibut;
      ClickBuf[lastc - 1].motion = motion;
      ClickBuf[lastc - 1].release = release;
    }
  else
    {
      ClickBuf[lastc].win = win;
      ClickBuf[lastc].x = x;
      ClickBuf[lastc].y = y;
      ClickBuf[lastc].ibutton = ibut;
      ClickBuf[lastc].motion = motion;
      ClickBuf[lastc].release = release;
      lastc++;
    }
  return (0);
}
/*-----------------------------------------------------------------------------------*/
int CheckClickQueue (integer *win,integer *x, integer *y, integer *ibut)
{
  int i;
  for (i = 0; i < lastc; i++)
    {
      int j;
      if (ClickBuf[i].win == *win || *win == -1)
	{
	  *win = ClickBuf[i].win;
	  *x = ClickBuf[i].x;
	  *y = ClickBuf[i].y;
	  *ibut = ClickBuf[i].ibutton;
	  for (j = i + 1; j < lastc; j++)
	    {
	      ClickBuf[j - 1].win = ClickBuf[j].win;
	      ClickBuf[j - 1].x = ClickBuf[j].x;
	      ClickBuf[j - 1].y = ClickBuf[j].y;
	      ClickBuf[j - 1].ibutton = ClickBuf[j].ibutton;
	      ClickBuf[j - 1].motion = ClickBuf[j].motion;
	      ClickBuf[j - 1].release = ClickBuf[j].release;
	    }
	  lastc--;
	  return (1);
	}
    }
  return (0);
}
/*-----------------------------------------------------------------------------------*/
int ClearClickQueue (int win)
{
  int i;
  if (win == -1)
    {
      lastc = 0;
      return 0;
    }
  for (i = 0; i < lastc; i++)
    {
      int j;
      if (ClickBuf[i].win == win)
	{
	  for (j = i + 1; j < lastc; j++)
	    {
	      ClickBuf[j - 1].win = ClickBuf[j].win;
	      ClickBuf[j - 1].x = ClickBuf[j].x;
	      ClickBuf[j - 1].y = ClickBuf[j].y;
	      ClickBuf[j - 1].ibutton = ClickBuf[j].ibutton;
	      ClickBuf[j - 1].motion = ClickBuf[j].motion;
	      ClickBuf[j - 1].release = ClickBuf[j].release;
	    }
	  lastc--;
	}
    }
  lastc = 0;
  return (0);
}
/*-----------------------------------------------------------------------------------*/
void set_wait_click(val)
{  
  wait_for_click=val;
  if ( val == 1 ) 
    SciClickInfo.win=-1; 
}
/*-----------------------------------------------------------------------------------*/
int scig_click_handler_none (int win,int x,int y,int ibut,int motion,int release)
{
  return 0;
};
/*-----------------------------------------------------------------------------------*/
int scig_click_handler_sci (int win,int x,int y,int ibut,int motion,int release)

{
  static char buf[256];
  struct BCG *SciGc;

  SciGc = GetWindowXgcNumber(win);
  if (strlen(SciGc->EventHandler)!=0) {
    sprintf(buf,"%s(%d,%d,%d,%d)",SciGc->EventHandler,win,x,y,ibut);
    StoreCommand1(buf,0);
    return 1;}
  else
    return 0;
};
/*-----------------------------------------------------------------------------------*/
Scig_click_handler set_scig_click_handler (f)
     Scig_click_handler f;
{
  Scig_click_handler old = scig_click_handler;
  scig_click_handler = f;
  return old;
}
/*-----------------------------------------------------------------------------------*/
void reset_scig_click_handler (void)
{
  scig_click_handler = scig_click_handler_sci;
}
/*-----------------------------------------------------------------------------------*/
/* add handlers for delete action */
void scig_deletegwin_handler_none (int win)
{
};
/*-----------------------------------------------------------------------------------*/
void scig_deletegwin_handler_sci (int win)
{
  static char buf[256];
  struct BCG *SciGc;

  SciGc = GetWindowXgcNumber(win);
  if (strlen(SciGc->EventHandler)!=0) {
    sprintf(buf,"%s(%d,0,0,-1000)",SciGc->EventHandler,win);
    StoreCommand1(buf,0);
  }
};
/*-----------------------------------------------------------------------------------*/
Scig_deletegwin_handler set_scig_deletegwin_handler (Scig_deletegwin_handler f)
{
  Scig_deletegwin_handler old = scig_deletegwin_handler;
  scig_deletegwin_handler = f;
  return old;
}
/*-----------------------------------------------------------------------------------*/
void reset_scig_deletegwin_handler ()
{
  scig_deletegwin_handler = scig_deletegwin_handler_sci;
}
/*-----------------------------------------------------------------------------------*/
void C2F(seteventhandler)(int *win_num,char *name,int *ierr)
{  
  struct BCG *SciGc;

  /*ButtonPressMask|PointerMotionMask|ButtonReleaseMask|KeyPressMask */
  *ierr = 0;
  SciGc = GetWindowXgcNumber(*win_num);
  if ( SciGc ==  NULL ) {*ierr=1;return;}
  strncpy(SciGc->EventHandler,name,24);
}
/*-----------------------------------------------------------------------------------*/