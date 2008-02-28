/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2008 - ENPC  - Jean-Philippe Chancelier <jpc@cermics.enpc.fr>
 * Copyright (C) 2004-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "stack-c.h"
#include "MALLOC.h"
#include "dynamic_menus.h"
#include "sciprint.h"
#include "sciprint_full.h"
#include "localization.h"
#include "Thread_Wrapper.h"

/*--------------------------------------------------------------------------*/
/*
 *  Command queue functions
 *  This function is used to store Scilab command in a queue
 *  ( the queue is checked in the Scilab Event Loop )
 *  The queue is filled by the function related to dynamic buttons and menus
 *  One can also set a handler to deal with the commands
 *  if he wants to bypass the queue
 *
 *  PUBLIC : set_scig_command_handler(Scig_command_handler f)
 *           void reset_scig_command_handler()
 *           int StoreCommand( char *command)
 *           integer C2F(ismenu)()
 *           int C2F(getmen)(char * btn_cmd,integer * lb, integer * entry)
 */
/*--------------------------------------------------------------------------*/
typedef int (*Scig_command_handler) __PARAMS((char *));
typedef struct commandRec
{
  char              *command;		/* command info one string two integers */
  int               flag; /* 1 if the command execution cannot be interrupted */
  struct commandRec *next;
} CommandRec, *CommandRecPtr;
/*--------------------------------------------------------------------------*/
int  scig_command_handler_none (char *command);
extern Scig_command_handler set_scig_command_handler __PARAMS((Scig_command_handler f));
extern int C2F (getmen) __PARAMS((char *btn_cmd, integer *lb, integer *entry));
extern void  reset_scig_command_handler __PARAMS((void));
extern void write_scilab  __PARAMS((char *s));

/*
** Extern Signal to say we git a StoreCommand.
*/
extern __threadSignal LaunchScilab;

#ifdef _MSC_VER
extern BOOL IsToThePrompt(void);
#endif /*_MSC_VER*/
/*--------------------------------------------------------------------------*/
static CommandRec *commandQueue = NULL;
static Scig_command_handler scig_command_handler = scig_command_handler_none;
/*--------------------------------------------------------------------------*/
/*
 * changing the default command handler
 */
/*--------------------------------------------------------------------------*/
int  scig_command_handler_none (char *command)
{
  return 0;
}
/*--------------------------------------------------------------------------*/
Scig_command_handler set_scig_command_handler (Scig_command_handler f)
{
  Scig_command_handler old = scig_command_handler;
  scig_command_handler = f;
  return old;
}
/*--------------------------------------------------------------------------*/
void reset_scig_command_handler ()
{
  scig_command_handler = scig_command_handler_none;
}
/*--------------------------------------------------------------------------*/
int StoreCommand ( char *command)
{
  return (StoreCommand1 (command, 0));
}
/*--------------------------------------------------------------------------*/
/*
 * try to execute a command or add it to the end of command queue
 * flag = 0 : the command is not shown in scilab window
 * flag = 1 : the command is shown in scilab window (if at prompt)
 */
/*--------------------------------------------------------------------------*/
int StoreCommand1 (char *command,int flag)
{
#ifdef _MSC_VER
  //if ( (flag == 1) && ( !IsToThePrompt () ) ) flag=0;
#endif
  switch (flag)
    {
    case 1: /* the command is shown in scilab window (if at prompt) */
      {
	//write_scilab (command);
	return (0);
      }
      break;
    case 0:
    default : /* the command is not shown in Scilab */
      {
	CommandRec *p, *q, *r;

	/** first check if we have a special handler set for commands **/
	if (scig_command_handler (command) == 1)  return 0;

	p = (CommandRec *) MALLOC (sizeof (CommandRec));
	if (p == (CommandRec *) 0)
	  {
	    sciprint(_("%s: No more memory.\n"),"send_command");
	    return (1);
	  }
	p->flag = 0;
	p->command = (char *) MALLOC ((strlen (command) + 1) * sizeof (char));
	if (p->command == (char *) 0)
	  {
	    FREE(p);
	    sciprint(_("%s: No more memory.\n"),"send_command");
	    return (1);
	  }
	strcpy (p->command, command);
	p->next = NULL;
	if (!commandQueue) commandQueue = p;
	else
	  {
	    q = commandQueue;
	    while ((r = q->next))	q = r;
	    q->next = p;
	  }
#ifdef _MSC_VER
	//if (IsToThePrompt ()) write_scilab ("\n");
#endif
	//**
	//** BLOUNO !!!!!!
	//**
	__Signal(&LaunchScilab);
	return (0);

	break;
      }
    }
}

void SetCommandflag(int flag)
{
  CommandRec *p, *r;
  if (commandQueue != NULL) {
    p = commandQueue;
    while ((r = p->next))	p = r;
    p->flag=flag;
  }
}
/*--------------------------------------------------------------------------*/
/*
 * Gets info on the first queue element
 * and remove it from the queue
 */
/*--------------------------------------------------------------------------*/
int GetCommand ( char *str)
{
  int flag;
  flag = 0;
  if (commandQueue != NULL)
    {

      CommandRec *p;


      p = commandQueue;
      strcpy (str, p->command);
      flag=p->flag;

      commandQueue = p->next;
      FREE (p->command);
      FREE (p);
      if (C2F(iop).ddt==-1) {
        if (flag==0) { sciprint_full("   Unqueuing %s - No option\n",str); }
        else         { sciprint_full("   Unqueuing %s - seq\n",str); }
      }
    }
  return flag;
}

integer ismenu(void)
{
  /* Do not manage commands while compiling scilab function */
  if ( (commandQueue == NULL) || (C2F(com).comp[0] != 0))
    return(0) ;
  else
    return(1);
}
/*--------------------------------------------------------------------------*/
/*
 * menu/button info for Scilab
 */
/*--------------------------------------------------------------------------*/
int C2F(getmen)(char * btn_cmd,integer * lb, integer * entry)
{
  int flag;
  if (ismenu()==1)
    {
      flag=GetCommand(btn_cmd);
      *lb=(integer)strlen(btn_cmd);
      *entry=0;  /* This parameter entry seems to be unused. Probably a very old thing... */
    }
  else
    {
      flag=0;
      *lb =0;
      *entry=0;  /* This parameter entry seems to be unused. Probably a very old thing... */
    }
  return flag;
}
/*--------------------------------------------------------------------------*/
