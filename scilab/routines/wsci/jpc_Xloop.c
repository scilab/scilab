/* Copyright (C) 1998 Chancelier Jean-Philippe */
/*
 * jpc_Xloop.c : 
 * (1997) : Jean-Philippe Chancelier 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#ifndef STRICT
#define STRICT
#endif
/*#include <windows.h>
  #include "wgnuplib.h"*/
#include "wresource.h"
#include "wcommon.h"
#include "../machine.h"
#ifdef WITH_TK
#include "tcl.h"
extern void inittk ();
extern void flushTKEvents ();
extern int  tcl_check_one_event(void);
#endif

/** do I want a scilab or an xscilab (here it means Windows ) */

int INXscilab = 0; /**  XXXX just to use zzledt1 **/

void
SetXsciOn ()
{
  switch_rlgets (1);
  INXscilab = 1;
#ifdef WITH_TK
  inittk ();
#endif
}

int C2F (xscion) (int *i)
{
  *i = INXscilab;
  return (0);
}

/*************************************************
 * Dealing with Events on the queue when computing in Scilab 
 * we also try to detect when CTRLC was  activated 
 *************************************************/

extern TW textwin;

void
TextMessage1 (int ctrlflag)
{
  MSG msg;
#ifdef WITH_TK
  flushTKEvents ();
#endif
  while (PeekMessage (&msg, 0, 0, 0, PM_NOREMOVE))
    {
#ifdef WITH_TK
      /* check for a tcl/tk event */
      if (Tcl_DoOneEvent (TCL_ALL_EVENTS | TCL_DONT_WAIT) == 1) continue ;
#endif
      PeekMessage (&msg, 0, 0, 0, PM_REMOVE);
      TranslateMessage (&msg);
      DispatchMessage (&msg);
    }
  if (ctrlflag == 1) 
    {
      CtrlCHit (&textwin);
    }
}
/** function used in wtext.c in function TextGetCh  must wait for an event **/

void
TextMessage2()
{
  MSG msg;
#ifdef WITH_TK
  flushTKEvents ();
#else 
  MSG msg;
#endif
#ifdef WITH_TK
  if (  tcl_check_one_event() == 1) 
    {
      /*sciprint("tcl event %l\r\n",msg.hwnd);*/
    }
#else 
  GetMessage(&msg, 0, 0, 0);
  TranslateMessage(&msg);
  DispatchMessage(&msg);
  while (  PeekMessage(&msg, 0, 0, 0,PM_REMOVE) ) 
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
#endif
}


int C2F (sxevents) ()
{
  if (INXscilab == 1)
    {
      TextMessage1 (1);
    }
  return (0);
}



/**********************************************************************
 * For Fortran call 
 **********************************************************************/

static void
strip_blank (source)
     char *source;
{
  char *p;
  p = source;
  /* look for end of string */
  while (*p != '\0')
    p++;
  while (p != source)
    {
      p--;
      if (*p != ' ')
	break;
      *p = '\0';
    }
}

/**********************************************************************/

void C2F (winsci) (char *pname, int *nos, int *idisp, char *display,
		   long int dummy1, long int dummy2)
{
  char *argv[10];
  int argc = 1;
  strip_blank (pname);
  argv[0] = pname;
  if (*idisp == 1)
    {
      argv[argc++] = "-display";
      strip_blank (display);
      argv[argc++] = display;
    }
  if (*nos == 1)
    argv[argc++] = "-ns";
  argv[argc++] = "-name";
  argv[argc++] = "Scilab";
  /* XXXXXX main_sci(argc,argv); */
}

void
sigblock ()
{
}


/**************************************************************************
 *  Command queue functions
 *  This function is used to store Scilab command in a queue 
 *  ( the queue is checked in the Scilab Event Loop )
 **************************************************************************/

typedef struct commandRec
{
  char *command;		/* command info one string two integers */
  struct commandRec *next;
}
CommandRec, *CommandRecPtr;

static CommandRec *commandQueue = NULL;

/***************************************
 * try to execute a command if we are at 
 * prompt level or add it to the end of command queue 
 * if flag == 1 a \n is added 
 ***************************************/

int 
scig_command_handler_none (char *command)
{
  return 0;
};

static Scig_command_handler scig_command_handler = scig_command_handler_none;

Scig_command_handler 
set_scig_command_handler (f)
     Scig_command_handler f;
{
  Scig_command_handler old = scig_command_handler;
  scig_command_handler = f;
  return old;
}

void 
reset_scig_command_handler ()
{
  scig_command_handler = scig_command_handler_none;
}

int
StoreCommand (command)
     char *command;
{
  return (StoreCommand1 (command, 1));
}


int
StoreCommand1 (command, flag)
     char *command;
     int flag;
{
  CommandRec *p, *q, *r;
  /* check if handler is using the command */
  if (scig_command_handler (command) == 1)
    return 0;
   if (flag==1&& get_is_reading ())
    {
      write_scilab (command);
      if (flag == 1)
	write_scilab ("\n");
      return 0;
      }
  p = (CommandRec *) malloc (sizeof (CommandRec));
  if (p == (CommandRec *) 0)
    {
      sciprint ("send_command : No more memory \r\n");
      return (1);
    }
  p->command = (char *) malloc ((strlen (command) + 1) * sizeof (char));
  if (p->command == (char *) 0)
    {
      sciprint ("send_command : No more memory \r\n");
      return (1);
    }
  strcpy (p->command, command);
  p->next = NULL;
  if (!commandQueue)
    commandQueue = p;
  else
    {
      q = commandQueue;
      while ((r = q->next))
	q = r;
      q->next = p;
    }
  if (flag==2&& get_is_reading ())
    write_scilab ("\n");
  return (0);
}

/************************************************
 * Gets info on the first queue element 
 * and remove it from the queue 
 ************************************************/

void
GetCommand (str)
     char *str;
{
  if (commandQueue != NULL)
    {
      CommandRec *p;
      p = commandQueue;
      strcpy (str, p->command);
      commandQueue = p->next;
      FREE (p->command);
      FREE (p);
    }
}


/************************************************
 * Checks if there's something on the 
 * commandQueue 
 ************************************************/

integer C2F (ismenu) ()
{
  if (commandQueue == NULL)
    return (0);
  else
    return (1);
}

/************************************************
 * menu/button info for Scilab 
 ************************************************/

int C2F (getmen) (btn_cmd, lb, entry)
     integer *
     entry, *lb;
     char *
       btn_cmd;
{
  if (C2F (ismenu) () == 1)
    {
      GetCommand (btn_cmd);
      *lb = strlen (btn_cmd);
    }
  else
    *entry = 0;
  return (0);
}
