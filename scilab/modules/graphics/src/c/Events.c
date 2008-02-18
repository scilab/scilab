/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2001 - ENPC - Jean-Philipe Chancelier
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdio.h>
#include <string.h>
#include "WindowList.h"
#include "Events.h"
#include "GetProperty.h"
#include "dynamic_menus.h"

typedef struct but
{ int win; /* graphic window containing the locator */
  int x,y; /* Locator position  coordinates */
  int ibutton; /*mouse button or key*/
  int motion; /* is it a move event */
  int  release; /* is it a release event */
} But;



int demo_menu_activate=0; /* add a demo menu in the graphic Window */


#define MaxCB 50 /* We keep the last  MaxCB click events on a queue just in case */
static But ClickBuf[MaxCB];
static int lastc = 0;
static int wait_for_click=0; /* set to 1 when in xclick or xgetmouse to disable event handler*/
static int event_select=5;/* record only press and release events, ignoring move*/

/* next used to prevent the user from destroying a graphic window 
 * when acquiring for example a zoom rectangle */
static int sci_graphic_protect = 0;
static Scig_deletegwin_handler scig_deletegwin_handler = scig_deletegwin_handler_sci;

/*--------------------------------------------------------------------------*/

int scig_click_handler_none (int win,int x,int y,int ibut,
			     int motion,int release) 
{return 0;}
/*--------------------------------------------------------------------------*/

int scig_click_handler_sci (int win,int x,int y,int ibut,int motion,int release)
{
  static char buf[256];
  sciPointObj * pFigure = NULL ;

  pFigure = getFigureFromIndex(win);

  if(pFigure == NULL)
  {
    return 0;
  }

  if (   sciGetIsEventHandlerEnable(pFigure)
      && strlen(sciGetEventHandler(pFigure)) > 0 )
  {
    sprintf(buf,"%s(%d,%d,%d,%d)",sciGetEventHandler(pFigure),win,x,y,ibut);
    StoreCommand(buf);
    return 1;
  }
  else
  {
    return 0;
  }
}

/*--------------------------------------------------------------------------*/
static Scig_click_handler scig_click_handler = scig_click_handler_sci;

/*--------------------------------------------------------------------------*/

Scig_click_handler set_scig_click_handler( Scig_click_handler f) 
{
  Scig_click_handler old = scig_click_handler;
  scig_click_handler = f;
  return old;
}
/*--------------------------------------------------------------------------*/

void reset_scig_click_handler() 
{
  scig_click_handler = scig_click_handler_sci;
}
/*--------------------------------------------------------------------------*/
void scig_deletegwin_handler_none (int win)
{
}
/*--------------------------------------------------------------------------*/
void scig_deletegwin_handler_sci (int win)
{
  static char buf[256];
  sciPointObj * pFigure = NULL ;

  pFigure = getFigureFromIndex(win);
  if (   pFigure != NULL
      && sciGetIsEventHandlerEnable(pFigure)
      && strlen(sciGetEventHandler(pFigure)) > 0 )
  {
    sprintf(buf,"%s(%d,0,0,-1000)",sciGetEventHandler(pFigure),win);
    StoreCommand(buf);
  }
}
/*--------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------*/
Scig_deletegwin_handler set_scig_deletegwin_handler(Scig_deletegwin_handler f) 
     
{
  Scig_deletegwin_handler old = scig_deletegwin_handler;
  scig_deletegwin_handler = f;
  return old;
}
/*--------------------------------------------------------------------------*/
Scig_deletegwin_handler get_scig_deletegwin_handler( void )
{
  return scig_deletegwin_handler ;
}
/*--------------------------------------------------------------------------*/
void reset_scig_deletegwin_handler() 
{
  scig_deletegwin_handler = scig_deletegwin_handler_sci;
}

/*--------------------------------------------------------------------------*/
void set_delete_win_mode(void) {  sci_graphic_protect = 0 ;}
/*--------------------------------------------------------------------------*/
void set_no_delete_win_mode(void)  {  sci_graphic_protect = 1 ;}
/*--------------------------------------------------------------------------*/
int  get_delete_win_mode(void) {  return sci_graphic_protect;}
/*--------------------------------------------------------------------------*/

/*---------------------------------------------------------
 * Mouse queue Handling 
 * the default behaviour is to store mouse clicks in a queue. 
 * But one can also set a handler to deal with 
 * mouse motion and click: the handler returns 1 
 * if he take care of the click and returns 0 if 
 * he want the queue to be used 
 ---------------------------------------------------------*/
int PushClickQueue(int win,int x,int y,int ibut, int motion,int release) 
{
  /* first let a click_handler do the job  */
  if ( ( wait_for_click == 0 ) && ( scig_click_handler(win,x,y,ibut,motion,release) == 1 ) ) return 0;
  if (  event_select==0 &&(motion == 1 || release == 1) ) return 0;
		
  if (((event_select&2)&&motion)||((event_select&4)&&release)||(motion==0&&release==0))
	{
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
  }
  return(0);
}
/*--------------------------------------------------------------------------*/
int CheckClickQueue(int *win,int *x,int *y,int *ibut,int *motion,int *release)
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
	  *motion= ClickBuf[i].motion ;
	  *release= ClickBuf[i].release ;
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
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/
void set_wait_click(int val)
{  
  if (val==1) {
    wait_for_click=val;
  }else
    wait_for_click=val;
}

/*--------------------------------------------------------------------------*/
int get_wait_click()
{  
  return wait_for_click;
}

void set_event_select(int val)
{  
    event_select=val;
}

/*--------------------------------------------------------------------------*/
int get_event_select()
{  
  return event_select;
}

