/*--------------------------------------------------
 * Copyright (C) 1998 Chancelier Jean-Philippe 
 *--------------------------------------------------*/

#ifdef SYSV
#include <string.h>
#else
#include <strings.h>
#endif

#include "x_ptyxP.h"
#include "x_data.h"
#include "../machine.h"
#include "All-extern-x.h"
#include "All-extern.h"

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xos.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>

/**
 added by HUYNH Olivier on the 10/06/2004 09:55
 hpux uses time.h instead of select.h
**/
#ifdef hpux
	#include <sys/time.h>
#else 
#include <sys/select.h>
#endif 

#ifdef aix
#include <sys/select.h>
#endif

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#include "../os_specific/Os_specific.h" 

#ifdef WITH_TK
#include "../tclsci/TCL_Global.h"
#endif

extern int  Scierror __PARAMS((int iv,char *fmt,...));


#define LF                    0x000a
#define MAXBUFFSIZE 256
static int ClickMenu=0;
static char buffstring[MAXBUFFSIZE];
static int lenbuffstring;
static int posinbuff=0;
extern int Xscilab  __PARAMS((Display **dpy, Widget *topwid));  
extern void xevents1  __PARAMS((void));  

/*-------------------------------------------------------
 * Functions to set or to get the scilab status 
 * is it a scilab or a scilab -nw 
 *-------------------------------------------------------*/


static fd_set Select_mask_ref,select_mask,Write_mask_ref,write_mask;
static int inter_max_plus1 = 0;
Display *the_dpy = (Display *) NULL;
int BasicScilab = 1;
XtAppContext app_con;
int IsClick_menu(void);
void Click_menu(int n);
int charfromclick(void);

static int INXscilab=0;
static int intoemacs=0;

void SetXsciOn(void)
{
#ifdef WITH_TK
  initTCLTK();
#endif
  INXscilab=1;
}

int IntoEmacs(void )
{
  return(strcmp(getenv("TERM"),"dumb")==0);
}

int C2F(xscion)(int *i)
{
  *i=INXscilab;
  return(0);
}


int C2F(checkevts)(int *i)
{
  *i= Max(INXscilab,1-BasicScilab);
  return(0);
}



static String bgfallback_resources[] = {
#include "Xscilab.ad.h"
  NULL,
};

/*-------------------------------------------------------
 * Display Init :
 * This function is initialize only Once  
 * the scilab display 
 *-------------------------------------------------------*/

static XtActionsRec actionProcs[] = {
  {"DeleteWindow", DeleteWindow},
  {"SGDeleteWindow", SGDeleteWindow},
  {"KeyboardMapping", KeyboardMapping}
};



/* 
 * used to start tk at run time when scilab 
 * was started with scilab -nw 
 * open a display with DisplayInit and initialize 
 * Tk. If TK_Started is set to one then the initialization 
 * was correct.
 */

static void basic_scilab_mask( Display **dpy);

void sci_tk_activate(void)
{
  Display *dpy = (Display *) NULL;
  Widget toplevel = (Widget) NULL;
  /*Cout("You have started Scilab in a mode in which TK is not initialized.\n");
    Cout("Trying to initialize \n");*/
  DisplayInit("",&dpy,&toplevel);
#ifdef WITH_TK
  initTCLTK();
#endif
  BasicScilab = 0;
  basic_scilab_mask(&dpy);
#ifdef WITH_TK
  flushTKEvents();
#endif
}

void DisplayInit(char *string,Display **dpy,Widget * toplevel)
{
  static XrmOptionDescRec *optionDescList = NULL;
  static Display *dpy1;
  static Widget toplevel1;
  static int count=0;
  int argc=0;
  char *argv = 0;
  if ( count > 0) 
    {
      *dpy=dpy1;
      *toplevel=toplevel1;
      return;
    }
  count++;
  Xscilab(dpy,toplevel);
  if ( *toplevel != (Widget) NULL)
    {
      toplevel1 = *toplevel;
      dpy1 = *dpy;
    }
  else
    {
      *toplevel=toplevel1=XtAppInitialize (&app_con,"Xscilab",optionDescList,
					   0,&argc, (String *)argv,
					   bgfallback_resources,(ArgList) NULL,(Cardinal) 0);
      XtAppAddActions(app_con, actionProcs, XtNumber(actionProcs));
      the_dpy = *dpy=dpy1=XtDisplay(toplevel1);
      BasicScilab = 0;
      basic_scilab_mask(dpy);
      intoemacs=IntoEmacs();
    }
  XSync(dpy1,0);
}

/* fix the select masks : used graphics or tk is activated from a scilab -nw */

static int Xsocket=0,fd_in=0,fd_out=0,fd_err=0 ;

static void basic_scilab_mask( Display **dpy)
{
  Xsocket = ConnectionNumber(*dpy);
  fd_in = fileno(stdin) ;
  fd_out = fileno(stdout);
  fd_err = fileno(stderr);

  FD_ZERO(&Select_mask_ref);
  FD_SET(fd_in , &Select_mask_ref);
  FD_SET(Xsocket, &Select_mask_ref);
#ifdef WITH_TK 
  if ( XTKsocket != 0 ) FD_SET(XTKsocket, &Select_mask_ref);
#endif 
  FD_ZERO(&Write_mask_ref);
  /* the two next FD_SET causes select not to wait 
   * 
   */
  /* FD_SET(fd_out,&Write_mask_ref);
     FD_SET(fd_err,&Write_mask_ref);*/

  inter_max_plus1 = Max(fd_in,Xsocket);      
  inter_max_plus1 = Max(fd_out,inter_max_plus1);
  inter_max_plus1 = Max(fd_err,inter_max_plus1);
#ifdef WITH_TK 
  inter_max_plus1 = Max(XTKsocket,inter_max_plus1);
#endif 
  inter_max_plus1++;
}  


/*-------------------------------------------------------
 * 
 *  Xorgetchar : function used while in the scilab -nw mode 
 *      by zzledt to get the next typed char (in stdin)
 *      but also checks for X11 events if we are using an X Window 
 *      ( ex a graphic window) with scilab -nw )
 *      
 *      stdin is supposed to be changed 
 *      so as not to wait for <cr> this is done inside zzledt 
 *      (in the following code  the key function is select )
 *
 *  Additionally, the case of emacs-shell is added
 *                the case of calling from user graphical menu is added
 *                now it is possible to run scicos from -nw mode
 *-------------------------------------------------------*/

int Xorgetchar(int interrupt)
     /* interrupt is not used here (console mode) */
{
  register int i;
  static struct timeval select_timeout;
  static int state = 0;
  if ( BasicScilab) return(getchar());
  for( ; ; ) {
    XFlush(the_dpy); /* always flush writes before waiting */
    fflush(stdout); 
    fflush(stderr); 
#ifdef WITH_TK 
    flushTKEvents();
#endif 
    /* Initialize masks  */
    select_mask = Select_mask_ref;
    write_mask  = Write_mask_ref;

    select_timeout.tv_sec = 0;
    select_timeout.tv_usec = 10;
    i = select(inter_max_plus1, &select_mask, &write_mask, (fd_set *)NULL, &select_timeout);
    if (i < 0) {
      if (errno != EINTR)
	{ 
	  Scistring("Error\n");
	  exit(0);
	  continue;
	}
    } 

    /* if there's something to output */
    if ( FD_ISSET(fd_out,&write_mask)) { 
      fflush(stdout); 
    }
    if ( FD_ISSET(fd_err,&write_mask)) { 
      fflush(stderr); 
    }

#ifdef WITH_TK 
    if ( XTKsocket != 0 && FD_ISSET(XTKsocket,&select_mask )) { 
      flushTKEvents();
    }
#endif 

    /* if there's something to read */
    if (FD_ISSET(fd_in,&select_mask) || IsClick_menu()) 
      state=1;
    else
      if (QLength(the_dpy) ||  FD_ISSET(Xsocket,&select_mask ) ||!(intoemacs))
	state=0;

    if (state) {
      if(IsClick_menu())
	i=charfromclick();
      else
	i=getchar();
      if (i==LF) state=0;
      return(i);
    }
	
    C2F(sxevents)();
    if (interrupt&&(C2F(ismenu)()==1)) return(-1);

  }


}




/*-------------------------------------------------------
 *  Dealing with X11 Events.
 *  xevents is called by Xorgetchar and also by DispatchEvents in 
 *  routines/system 
 *  xevents must work for scilab and scilab -nw 
 *-------------------------------------------------------*/

int C2F(sxevents)()
{
  if (INXscilab==1) 
    {
      xevents1();
    }
  else 
    {
      XEvent event;
      if (BasicScilab) return(0);
      if ( the_dpy == (Display *) NULL)  return(0);
#ifdef WITH_TK 
      if ( XTKsocket != 0 ) flushTKEvents();
#endif 
      if (!XPending (the_dpy))
	/* protect against events/errors being swallowed by us or Xlib */
	return(0);
      do {
	XNextEvent (the_dpy, &event);
	XtDispatchEvent(&event);
      } while (QLength(the_dpy) > 0);
    }
  return(0);
}



void Click_menu(int n);

static void str_to_xterm ( string, nbytes)
     register char *string;
     int nbytes;
{
  register TScreen *screen = &term->screen;
  StringInput(screen,string,strlen(string));
}


static void str_to_xterm_nw ( string, nbytes)
     register char *string;
     int nbytes;
{
  posinbuff=0;
  if (nbytes>MAXBUFFSIZE-1)
    {
      Scierror(1020,"%s\n", "The menu  name is too long  to be strored");
      return;
    }
  sprintf(buffstring,string);
  buffstring[nbytes]='\n';
  lenbuffstring=nbytes+1;
  Click_menu(1);
}

void write_scilab(s)
     char   *s;
{
  int  i;
  C2F(xscion)(&i);
  if (i==1)
    str_to_xterm(s,strlen(s));
  else
    str_to_xterm_nw(s,strlen(s));
  /*      	printf(" asynchronous actions are not supported with -nw option\n");*/
}

int IsClick_menu(void)
{
  return(ClickMenu);
}

void Click_menu(int n)
{
  ClickMenu=n;
  return;
}

int charfromclick(void)
{
  if (posinbuff==lenbuffstring-1)
    {
      ClickMenu=0;
    }
  if (posinbuff==lenbuffstring)
    return(9999);
  else
    return(buffstring[posinbuff++]);
}
  






