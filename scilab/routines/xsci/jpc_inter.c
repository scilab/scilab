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

#ifdef aix
#include <sys/select.h>
#endif
#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#include "../sun/Sun.h"

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

static int INXscilab=0;
static int intoemacs;

void SetXsciOn(void)
{
#ifdef WITH_TK
  inittk();
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


int pty_mask = 1;
int X_mask =16;
int Select_mask;
int select_mask = 0;
int write_mask = 2;
int Write_mask = 2;
int max_plus1 = 5;

Display *the_dpy = (Display *) NULL;
int BasicScilab = 1;
XtAppContext app_con;
int IsClick_menu(void);
void Click_menu(int n);
int charfromclick(void);

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

void DisplayInit(string, dpy, toplevel)
     char *string;
     Display **dpy;
     Widget *toplevel;
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
      int Xsocket,fd ;
      *toplevel=toplevel1=XtAppInitialize (&app_con,"Xscilab",optionDescList,
					   0,&argc, (String *)argv,
					   bgfallback_resources,(ArgList) NULL,(Cardinal) 0);
      XtAppAddActions(app_con, actionProcs, XtNumber(actionProcs));
      the_dpy = *dpy=dpy1=XtDisplay(toplevel1);
      BasicScilab = 0;
      Xsocket = ConnectionNumber(dpy1);
      X_mask = 1 << Xsocket;
      fd = fileno(stdin) ;
      pty_mask = 1   << fd;
      Select_mask = pty_mask | X_mask;  
      Write_mask = 1 << fileno(stdout);
      max_plus1 = (fd < Xsocket) ? (1 + Xsocket) : (1 + fd);
      intoemacs=IntoEmacs();
    }
  XSync(dpy1,0);
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

int Xorgetchar()
{
  register int i;
  static struct timeval select_timeout;
  static int state=0;
  if ( BasicScilab) return(getchar());
  for( ; ; ) {
    XFlush(the_dpy); /* always flush writes before waiting */
    /* Update the masks and, unless X events are already in the queue,
       wait for I/O to be possible. */
    select_mask = Select_mask;
    write_mask  = Write_mask;
    select_timeout.tv_sec = 1;
    select_timeout.tv_usec = 0;
    i = select(max_plus1, (fd_set *)&select_mask, 
	       (fd_set *) &write_mask, (fd_set *)NULL,
	       QLength(the_dpy) ? &select_timeout
	       : (struct timeval *) NULL);
    if (i < 0) {
      if (errno != EINTR)
	{ 
	  Scistring("Error\n");
	  exit(0);
	  continue;
	}
    } 
    if (write_mask & Write_mask) {	  fflush(stdout);}

    /* if there's something to read */
    if ((select_mask & pty_mask) || IsClick_menu()) 
      state=1;
    else
      if (QLength(the_dpy) || (select_mask & X_mask) ||!(intoemacs))
	state=0;

    if (state) {
      if(IsClick_menu())
	i=charfromclick();
      else
	i=getchar();
      if (i==LF) state=0;
      return(i);
    }
	
    /* if there are X events already in our queue, it
       counts as being readable */
    if (QLength(the_dpy) || (select_mask & X_mask)) 
      { C2F(sxevents)();	}
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


/*-------------------------------------------------------*
 *  Command queue functions
 *  This function is used to store Scilab command in a queue 
 *  ( the queue is checked in the Scilab Event Loop )
 *  The queue is filled by the function related to dynamic buttons and menus 
 *
 *  One can also set a handler to deal with the commands 
 *  if he wants to bypass the queue
 *-------------------------------------------------------*/

typedef struct commandRec {
  char                *command;  /* command info one string two integers */
  struct commandRec   *next;
} CommandRec, *CommandRecPtr;

static CommandRec *commandQueue = NULL;

int scig_command_handler_none (win,x,y,ibut,motion,release)int win,x,y,ibut,motion,release;
{return 0;};

static Scig_command_handler scig_command_handler = scig_command_handler_none;

Scig_command_handler set_scig_command_handler(f) 
     Scig_command_handler f;
{
  Scig_command_handler old = scig_command_handler;
  scig_command_handler = f;
  return old;
}

void reset_scig_command_handler() 
{
  scig_command_handler = scig_command_handler_none;
}



/* try to execute a command or add it to the end of command queue */

int StoreCommand(command)
     char *command;
{
  CommandRec *p, *q, *r;
  int i;


  /** first check if we have a special handler set for commands **/
  if ( scig_command_handler(command) == 1) return 0;
  /*  if (get_is_reading()) 
    { 
      write_scilab(command);
      C2F(xscion)(&i);
      if (i) write_scilab("\n");
      return 0;
      }*/
  p = (CommandRec *) malloc( sizeof(CommandRec));
  if ( p == (CommandRec *) 0 ) 
    {
      sciprint("send_command : No more memory \r\n");
      return(1);
    }
  p->command = (char *) malloc( ( strlen(command)+1)*sizeof(char));
  if ( p->command == (char *) 0 ) 
    {
      sciprint("send_command : No more memory \r\n");
      return(1);
    }
  strcpy(p->command,command);
  p->next = NULL;
  if ( !commandQueue)
    commandQueue = p;
  else 
    {
      q = commandQueue;
      while ((r = q->next))
	q = r;
      q->next = p;
    }
  if (get_is_reading()) 
    { 
      C2F(xscion)(&i);
      if (i) write_scilab(" ");
      }
  return(0);
}

/************************************************
 * Gets info on the first queue element 
 * and remove it from the queue 
 ************************************************/

void GetCommand(str)
     char *str;
{
  if ( commandQueue != NULL)
    {
      CommandRec *p;
      p= commandQueue;
      strcpy(str, p->command);
      commandQueue = p->next;
      free(p->command);
      free(p);
    }
}


/************************************************
 * Checks if there's something on the 
 * commandQueue 
 ************************************************/

integer C2F(ismenu)()
{
  if ( commandQueue == NULL ) 
    return(0) ;
  else 
    return(1);
}

/************************************************
 * menu/button info for Scilab 
 ************************************************/

int C2F(getmen)(btn_cmd,lb,entry)
     integer *entry,*lb;
     char *btn_cmd;
{
  if (C2F(ismenu)()==1) 
    {
      GetCommand(btn_cmd);
      *lb=strlen(btn_cmd);
    }
  else
    { 
      *lb =0;
      *entry=0;
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
  






