/*------------------------------------------------------------------------
 *    Copyright (C) 2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

#include <stdio.h>			/* For the Syntax message */
#include <signal.h>
#include <string.h>
#include "../sun/men_Sutils.h"
#include "../menusX/men_scilab.h"
#include "All-extern.h"
#include "../graphics/Math.h"
#include "../graphics/bcg.h"

#include <gtk/gtk.h>


/* #include "../graphics/periGtk-bcg.h" */

int demo_menu_activate=0; /* add a demo menu in the graphic Window */

/*---------------------------------------------------------
 * Checking events in the Graphic Window 
 * in fact just the XtAddEventHandler is used 
 * used in xclick 
 * We keep the last  MaxCB click events on a queue just in case 
 * xclick wants them ( xclick(1)) 
 ---------------------------------------------------------*/

typedef struct but {
  int win,x,y,ibutton,motion,release;
} But;

#define MaxCB 50
static But ClickBuf[MaxCB];
static int lastc = 0;

/*---------------------------------------------------------
 * Mouse queue Handling 
 * the default behaviour is to store mouse clicks in a queue. 
 * But one can also set a handler to deal with 
 * mouse motion and click: the handler returns 1 
 * if he take care of the click and returns 0 if 
 * he want the queue to be used 
 ---------------------------------------------------------*/

int scig_click_handler_none (int win,int x,int y,int ibut,
			     int motion,int release) 
{return 0;};

int scig_click_handler_sci (int win,int x,int y,int ibut,int motion,int release)

{
  static char buf[256];
  struct BCG *SciGc;

  SciGc = GetWindowXgcNumber(win);
  if (strlen(SciGc->EventHandler)!=0) {
    sprintf(buf,"%s(%d,%d,%d,%d)",SciGc->EventHandler,win,x,y,ibut);
    StoreCommand1(buf,2);
    return 1;}
  else
    return 0;
};

static Scig_click_handler scig_click_handler = scig_click_handler_sci;
/*static Scig_click_handler scig_click_handler = scig_click_handler_none;*/


Scig_click_handler set_scig_click_handler( Scig_click_handler f) 
{
  Scig_click_handler old = scig_click_handler;
  scig_click_handler = f;
  return old;
}

void reset_scig_click_handler() 
{
  scig_click_handler = scig_click_handler_sci;
}

/* A reprendre avec une vraie queue */ 



int PushClickQueue(int win,int x,int y,int ibut,
		   int motion,int release) 
{
  /* first let a click_handler do the job  */
  if ( scig_click_handler(win,x,y,ibut,motion,release)== 1) return 0;
  /* do not record motion events and release button 
   * this is left for a futur release 
   */
  if ( motion == 1 || release == 1 ) return 0;
  /* store click event in a queue */
  if ( lastc == MaxCB ) 
    {
      int i;
      for ( i= 1 ; i < MaxCB ; i ++ ) 
	{
	  ClickBuf[i-1]=ClickBuf[i];
	}
      ClickBuf[lastc-1].win = win;
      ClickBuf[lastc-1].x = x;
      ClickBuf[lastc-1].y = y;
      ClickBuf[lastc-1].ibutton = ibut;
      ClickBuf[lastc-1].motion = motion;
      ClickBuf[lastc-1].release = release;
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
  return(0);
}

int CheckClickQueue(int *win,int *x,int *y,int *ibut)
{
  int i;
  for ( i = 0 ; i < lastc ; i++ )
    {
      int j ;
      if ( ClickBuf[i].win == *win || *win == -1 ) 
	{
	  *win = ClickBuf[i].win;
	  *x= ClickBuf[i].x ;
	  *y= ClickBuf[i].y ;
	  *ibut= ClickBuf[i].ibutton; 
	  for ( j = i+1 ; j < lastc ; j++ ) 
	    {
	      ClickBuf[j-1].win = ClickBuf[j].win ;
	      ClickBuf[j-1].x   = ClickBuf[j].x ;
	      ClickBuf[j-1].y =  ClickBuf[j].y ;
	      ClickBuf[j-1].ibutton = ClickBuf[j].ibutton ;
	      ClickBuf[j-1].motion =  ClickBuf[j].motion ;
	      ClickBuf[j-1].release = ClickBuf[j].release ;
	    }
	  lastc--;
	  return(1);
	}
    }
  return(0);
}

int ClearClickQueue(int win)
{
  int i;
  if ( win == -1 ) 
    {
      lastc = 0;
      return 0;
    }
  for ( i = 0 ; i < lastc ; i++ )
    {
      int j ;
      if ( ClickBuf[i].win == win  ) 
	{
	  for ( j = i+1 ; j < lastc ; j++ ) 
	    {
	      ClickBuf[j-1].win = ClickBuf[j].win ;
	      ClickBuf[j-1].x   = ClickBuf[j].x ;
	      ClickBuf[j-1].y =  ClickBuf[j].y ;
	      ClickBuf[j-1].ibutton = ClickBuf[j].ibutton ;
	      ClickBuf[j-1].motion =  ClickBuf[j].motion ;
	      ClickBuf[j-1].release = ClickBuf[j].release ;
	    }
	  lastc--;
	}
    }
  lastc=0;
  return(0);
}



void C2F(seteventhandler)(int *win_num,char *name,int *ierr)
{  
  struct BCG *SciGc;
  /*ButtonPressMask|PointerMotionMask|ButtonReleaseMask|KeyPressMask */
  *ierr = 0;
  SciGc = GetWindowXgcNumber(*win_num);
  if ( SciGc ==  NULL ) {*ierr=1;return;}
  strncpy(SciGc->EventHandler,name,24);
}

