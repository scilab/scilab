/*------------------------------------------------------------------------
    Graphic library for 2D and 3D plotting 
    Copyright (C) 1998 Chancelier Jean-Philippe
    jpc@cergrene.enpc.fr 
 --------------------------------------------------------------------------*/

#include "machine.h"
#include <QuickDraw.h>
#include <MacTypes.h>
#include <WindowMgr.h>
#include <ControlMgr.h>
#include <EventMgr.h>

#ifdef THINK_C	/* SYSDEP.H */
#include <stdlib.h>
#define __STDC__
#define  SYS5
#define  AIFF
#define  WINDOWS	/* with winmac.c */
#else
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#endif

#ifdef SYS5
#define  index(A,B) strchr(A,B)
#include <fcntl.h>
#include <string.h>
#else
#include <strings.h>
#endif


#include <stdio.h>
#define XINIT    10	/* set default window location */
#define YINIT    50
#define WIDTH    300	/* start off v small - user resizes */
#define HEIGHT   300
#define BDR      2
#define MSGX     16	/* offset for message in window */
#define MSGY     16
#define TXHGHT   14	/* baseline offset for text */
#define SBARWIDTH 16
#define SBarWidth 16
/* protoypes */
static void myWwait(char *msg);
void myWwinWait(WindowPtr win, char *msg);

/* static variables */

static winstance = 0;	/* counts windows to offset across screen */
#define STACKDEPTH 4	/* number of windows down to go */

WindowPtr lwin = 0;
Rect dragRect;
static int width = 5;

#define	MAXWIN 16
WindowPtr ourWins[MAXWIN] = {NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL,
NULL, NULL, NULL, NULL};

/* Clears the current window  */
UpdateCWindow()
{
  WindowPtr theWindow;
  extern WindowPtr C2F(GetWindowNumber)();
  int verbose = 0, win1, narg;
  C2F(dr)("xget", "window", &verbose, &win1, &narg,(int *) 0, (int *) 0, (int *) 0, 0, 0);
  theWindow = C2F(GetWindowNumber)(win1);
  if ( theWindow != NULL) 
  	{
  	  	GrafPtr savePort;
  	  	Scistring("UpdateCWindow In \n");
  		GetPort(&savePort);
  		SetPort(theWindow);
  		EraseRect(&theWindow->portRect);
  		DrawGrowIcon(theWindow);
  		SetPort(savePort);
  		Scistring("UpdateCWindow Out \n");
  	}
}

/* Called when a window Needs update */

UpdateWindow(theWindow)
  WindowPtr theWindow;
{
  GrafPtr savePort;
  GetPort(&savePort);
  SetPort(theWindow);
  BeginUpdate(theWindow);
  EraseRect(&theWindow->portRect);
  DrawGrowIcon(theWindow);
  DrawContents(theWindow);
  EndUpdate(theWindow);
  SetPort(savePort);
}

/** No effect **/

DoContent(theWindow, theEvent)
     WindowPtr	theWindow;
     EventRecord	*theEvent;
{
  int				cntlCode;
  ControlHandle 	theControl;
  int				pageSize;
  GrafPtr			savePort;
/*
  GetPort(&savePort);
  SetPort(theWindow);
  GlobalToLocal( &theEvent->where );
  if ((cntlCode = FindControl(theEvent->where, theWindow, &theControl)) == 0) {
    if (PtInRect( theEvent->where, &myWindow->portRect ))
      DoMyContent(theWindow, theEvent);		
  }
  else if (cntlCode == inThumb) {
    TrackControl(theControl, theEvent->where, 0L);
    AdjustText();
  }
  else
    TrackControl(theControl, theEvent->where, &ScrollProc);

  SetPort(savePort);
*/
}

/** Called when a Window is sized **/

MyGrowWindow(w, p)
  WindowPtr w;
  Point p;
{
  GrafPtr savePort;
  long theResult;
  Rect r, oView;
  GetPort(&savePort);
  SetPort(w);
  SetRect(&r, 80, 80, screenBits.bounds.right, screenBits.bounds.bottom);
  theResult = GrowWindow(w, p, &r);
  if (theResult == 0)
    return;
  SizeWindow(w, LoWord(theResult), HiWord(theResult), 1);
  InvalRect(&w->portRect);
  oView = w->portRect;
  HidePen();
  DrawContents(w);
  DrawGrowIcon(w);
  ShowPen();
  SetPort(savePort);
}

/*
CloseMyWindow()
{
  HideWindow( myWindow );
  DisposeWindow( myWindow );
}
*/
/* called during program initialisation */

int Graphable()	
{	
  /* Always use graphs on mac, so just sets it up */
  /* mac screen initialisation ? */
  InitGraf(&thePort);
  return (1);			
  /* graphics available */
}

WaitForUpdate(WindowPtr w)
{
  MainEvent(updateEvt);
}

RedrawGWin(WindowPtr w)
{
  Rect myRect;
}

/** Searches if the Window win is a graphic one and then redraws it **/

DrawContents(WindowPtr win)
{
  Rect myRect;
  int msgW, msgH, color = true, i, winnumber;
  winnumber = 0;
  for (i = 0; i < MAXWIN; ++i)
    if (ourWins[i] == win)
    {
      WindowPtr loc;
      int verbose = 0, win1, narg;
      	sciprint("Tape redraw of %d\r\n",i);
    
      C2F(dr)("xget", "window", &verbose, &win1, &narg
	  ,(int *) 0, (int *) 0, (int *) 0, 0, 0);
      winnumber = i;
      /* fait planter */
      /* C2F(dr)("xset","window",&i,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);*/
      Tape_Replay("v", &winnumber);
      /*C2F(dr)("xset","window",&win1,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);*/
    }
}

/* print click-Exit message in most recently active window */

int ExitGraph()	
{
  myWwait("click here to EXIT");
  return (lwin == NULL);
}

static void myWprintLines(win, msg, x, y, pw, ph)	/* prints str with embdd
							 * '\n's */
  WindowPtr win;	/* on multiple lines */
  char *msg;	/* returns overall width & depth of area printed */
  int x, y;
  int *pw, *ph;
{
  int lin = 0, w, slen;
  char *s1, *s2;
  *pw = 0;
  *ph = 0;
  s1 = msg;
  do
  {
    s2 = strchr(s1, '\n');
    if (s2)
      *s2 = '\0';
    AddText(MSGX, MSGY + lin * TXHGHT, s1);
    if (s2)
      *s2 = '\n';
    if ((w = 8 * (s2 - s1)) > *pw)	/* hack to figure width of text */
      *pw = w;
    *ph += TXHGHT;
    s1 = s2 + 1;
    ++lin;
  } while (s2 != NULL);
}

static void myWwait(msg)
  char *msg;
{
  if (lwin != NULL)
    myWwinWait(lwin, msg);
}

void myWwinWait(win, msg)
  WindowPtr win;
  char *msg;
{
  int msgW, msgH;
  Rect r;
  char dummy;
  long oldRefCon;

  /* cursNorm(); */
  BeginUpdate(win);
  oldRefCon = ((WindowPeek) win)->refCon;
  ((WindowPeek) win)->refCon = (long) msg;
  myWprintLines(win, msg, MSGX, MSGY, &msgW, &msgH);
  dummy = MainEvent(mouseDown);
  r.top = MSGY - TXHGHT + 4;
  r.left = MSGX;
  r.bottom = r.top + msgH;
  r.right = r.left + msgW;
  EraseRect(&r);
  ((WindowPeek) win)->refCon = oldRefCon;
  /* cursPop(); */
  EndUpdate(win);
  Scistring( " j'envoie un message \n");
}

AddText(x, y, s)	/* Just put a string on screen */
  int x, y;
  char *s;	/* null terminated */
{
  TextFont(4);			/* Monaco */
  TextSize(9);			/* 9 pt   */
  MoveTo(x, y);			/* Add text label in top left of unit */
  CtoPstr(s);
  DrawString(s);
  PtoCstr(s);
}

int MainEvent(int waitforevent)
{
  EventRecord myEvent;
  WindowPtr whichWindow;
  Rect r;
  int done = 0;
  SystemTask();
  /* Debugger();	 */
  do
    {
      if (GetNextEvent(everyEvent, &myEvent))
	{
	  done = 0;
	  switch (myEvent.what)
	    {
	    case mouseDown:
	      switch (FindWindow(myEvent.where, &whichWindow))
		{
		case inDesk:
		  SysBeep(10L);
		  SysBeep(10L);
		  break;
		  /* case inGoAway: if (ours(whichWindow)) if (TrackGoAway(
		   * whichWindow, myEvent.where) ) DoFile(fmClose); break; */
		case inMenuBar:
		  Scistring("in menu bar \n");
		  AdjustMenus();
		  HandleMenu(MenuSelect(myEvent.where));
		  Scistring("out menu bar \n");
		  break;
		case inSysWindow:
		  SystemClick(&myEvent, whichWindow);
		  break;
		case inDrag:
		  if (ours(whichWindow))
		    {
		      Scistring( " Trying to drag The Window\n");
		      Scistring( " OK Going on \n");
		       DragWindow(whichWindow, myEvent.where, &dragRect);
		    }
		  break;
		case inGrow:
		  if (ours(whichWindow))
		    {
		    	Scistring( " Enlarging a Graphic Window\n");
		    	MyGrowWindow(whichWindow, myEvent.where);
		    		}
		  break;
		case inContent:
		  if (whichWindow != FrontWindow())
		    SelectWindow(whichWindow);
		  else if (ours(whichWindow))
		    Scistring( " Docontent on  a Graphic Window\n");
		    /* DoContent(whichWindow, &myEvent);*/
		    done = 1;
		  break;
		default:
		  break;
		}			
	      /* end switch FindWindow	 */
	      break;
	    case keyDown:
	    case autoKey:
	      /* { register char	theChar;
	       * 
	       * theChar = myEvent.message & charCodeMask; if ((myEvent.modifiers &
	       * cmdKey) != 0) return( DoCommand( MenuKey( theChar ) )); else{
	       * ShowSelect(); dirty = 1; } }	 */
	      done = 1;		/* that will do */
	      break;
	    case activateEvt:
	      if (ours(whichWindow = (WindowPtr) myEvent.message))
		{
		  r = (*whichWindow).portRect;
		  r.top = r.bottom - (SBarWidth + 1);
		  r.left = r.left - (SBarWidth + 1);
		  InvalRect(&r);
		  if (myEvent.modifiers & activeFlag)
		    {
		      /* ShowControl( vScroll ); */
		      /* DisableItem( myMenus[editM], undoCommand ); */
		    } else
		      {
			/* HideControl( vScroll ); */
			ZeroScrap();
		      }
		}
	      break;
	    case updateEvt:
	      if (ours(whichWindow = (WindowPtr) myEvent.message))
		{
		  Scistring( " Trying to Update \n");
		  UpdateWindow(whichWindow);
		  done = 1;
		}
	      break;
	    default:
	      Scistring( " defaut : Nothing done \n");
	      break;
	      ;
	    }				/* end of case myEvent.what */
	}
    } while (done == 0 || myEvent.what != waitforevent);
  return (myEvent.what);
}

RegWin(WindowPtr w)
{
  int i;

  for (i = 0; i < MAXWIN; ++i)
    if (ourWins[i] == NULL)
    {
      ourWins[i] = w;
      i = MAXWIN;
    }
}

RmvWin(WindowPtr w)
{
  int i;

  for (i = 0; i < MAXWIN; ++i)
    if (ourWins[i] == w)
    {
      ourWins[i] = NULL;
      i = MAXWIN;
    }
}

int ours(WindowPtr w)
{
  int i;
  int rc = 0;

  for (i = 0; i < MAXWIN; ++i)
    if (ourWins[i] == w)
    {
      rc = 1;
      i = MAXWIN;
    }
  return (rc);
}
