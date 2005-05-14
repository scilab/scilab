/*------------------------------------------------------------------------
 *    Copyright (C) 2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/
#include <stdio.h>
#include "bcg.h"

typedef struct but
{ int win; /* graphic window containing the locator */
  int x,y; /* Locator position  coordinates */
  int ibutton; /*mouse button or key*/
  int motion; /* is it a move event */
  int  release; /* is it a release event */
} But;


typedef int (*Scig_click_handler) (int,int,int,int,int,int);
typedef void (*Scig_deletegwin_handler) (int);
extern int StoreCommand1(char *,int);

int demo_menu_activate=0; /* add a demo menu in the graphic Window */


#define MaxCB 50 /* We keep the last  MaxCB click events on a queue just in case */
static But ClickBuf[MaxCB];
static int lastc = 0;
static int wait_for_click=0;

/* nextused to prevent the user from destroying a graphic window 
 * when acquiring for example a zoom rectangle */
static int sci_graphic_protect = 0;
/*-----------------------------------------------------------------------------------*/

int scig_click_handler_none (int win,int x,int y,int ibut,
			     int motion,int release) 
{return 0;}
/*-----------------------------------------------------------------------------------*/

int scig_click_handler_sci (int win,int x,int y,int ibut,int motion,int release)
{
  static char buf[256];
  struct BCG *SciGc = (struct BCG *) NULL;

  SciGc = GetWindowXgcNumber(win);

  if(SciGc == (struct BCG *) NULL) 
	  return 0;

  if (strlen(SciGc->EventHandler)!=0) {
    sprintf(buf,"%s(%d,%d,%d,%d)",SciGc->EventHandler,win,x,y,ibut);
    StoreCommand1(buf,0);
    return 1;}
  else
    return 0;
}

/*-----------------------------------------------------------------------------------*/
static Scig_click_handler scig_click_handler = scig_click_handler_sci;

/*-----------------------------------------------------------------------------------*/

Scig_click_handler set_scig_click_handler( Scig_click_handler f) 
{
  Scig_click_handler old = scig_click_handler;
  scig_click_handler = f;
  return old;
}
/*-----------------------------------------------------------------------------------*/

void reset_scig_click_handler() 
{
  scig_click_handler = scig_click_handler_sci;
}
/*-----------------------------------------------------------------------------------*/
void scig_deletegwin_handler_none (int win)
{
}
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
}
/*-----------------------------------------------------------------------------------*/

Scig_deletegwin_handler scig_deletegwin_handler = scig_deletegwin_handler_sci;

/*-----------------------------------------------------------------------------------*/
Scig_deletegwin_handler set_scig_deletegwin_handler(Scig_deletegwin_handler f) 
     
{
  Scig_deletegwin_handler old = scig_deletegwin_handler;
  scig_deletegwin_handler = f;
  return old;
}
/*-----------------------------------------------------------------------------------*/
void reset_scig_deletegwin_handler() 
{
  scig_deletegwin_handler = scig_deletegwin_handler_sci;
}

/*-----------------------------------------------------------------------------------*/
void set_delete_win_mode(void) {  sci_graphic_protect = 0 ;}
/*-----------------------------------------------------------------------------------*/
void set_no_delete_win_mode(void)  {  sci_graphic_protect = 1 ;}
/*-----------------------------------------------------------------------------------*/
int  get_delete_win_mode(void) {  return sci_graphic_protect;}
/*-----------------------------------------------------------------------------------*/

/*---------------------------------------------------------
 * Mouse queue Handling 
 * the default behaviour is to store mouse clicks in a queue. 
 * But one can also set a handler to deal with 
 * mouse motion and click: the handler returns 1 
 * if he take care of the click and returns 0 if 
 * he want the queue to be used 
 ---------------------------------------------------------*/


int PushClickQueue(int win,int x,int y,int ibut,
		   int motion,int release) 
{
  /* first let a click_handler do the job  */
  if (  wait_for_click==0){
    if ( scig_click_handler(win,x,y,ibut,motion,release)== 1) return 0;
  }
  /* do not record motion events and release button 
   * this is left for a futur release 
   */
  /*if (motion==1) printf("%d ",(wait_for_click&2)&&motion);*/
  if (  wait_for_click==0 &&(motion == 1 || release == 1) ) return 0;
  if (((wait_for_click&2)&&motion)||((wait_for_click&4)&&release)||(motion==0&&release==0)) {
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
/*-----------------------------------------------------------------------------------*/
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
/*-----------------------------------------------------------------------------------*/
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
/*-----------------------------------------------------------------------------------*/

void seteventhandler(int *win_num,char *name,int *ierr)
{  
  struct BCG *SciGc;
  /*ButtonPressMask|PointerMotionMask|ButtonReleaseMask|KeyPressMask */
  *ierr = 0;
  SciGc = GetWindowXgcNumber(*win_num);
  if ( SciGc ==  NULL ) {*ierr=1;return;}
  strncpy(SciGc->EventHandler,name,24);
}
/*-----------------------------------------------------------------------------------*/
void set_wait_click(val)
{  
  wait_for_click=val;
}

/*-----------------------------------------------------------------------------------*/
int get_wait_click()
{  
  return wait_for_click;
}

