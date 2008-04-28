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
 *
 *  PUBLIC : int StoreCommand( char *command)
 *           integer C2F(ismenu)()
 *           int C2F(getmen)(char * btn_cmd,integer * lb, integer * entry)
 */
/*--------------------------------------------------------------------------*/

#ifdef _MSC_VER
	#define IMPORT_SIGNAL __declspec(dllimport)
#else
	#define IMPORT_SIGNAL extern
#endif

typedef struct commandRec
{
  char              *command;		/* command info one string two integers */
  int               flag; /* 1 if the command execution cannot be interrupted */
  struct commandRec *next;
} CommandRec;
/*--------------------------------------------------------------------------*/

/*
** Extern Signal to say we git a StoreCommand.
*/
IMPORT_SIGNAL __threadSignal LaunchScilab;

/*--------------------------------------------------------------------------*/
static CommandRec *commandQueue = NULL;
static __threadLock commandQueueSingleAccess;

/*--------------------------------------------------------------------------*/
int StoreCommand ( char *command)
{
  return (StoreCommandWithFlag (command, 0));
}

/*--------------------------------------------------------------------------*/
/*
 * try to execute a command or add it to the end of command queue
 * flag = 0 : the command is not shown in scilab window
 * flag = 1 : the command is shown in scilab window (if at prompt) and executed sequentially
 */
/*--------------------------------------------------------------------------*/
int StoreCommandWithFlag (char *command,int flag)
{
  CommandRec *p, *q, *r;

	p = (CommandRec *) MALLOC (sizeof (CommandRec));
	if (p == (CommandRec *) 0)
	  {
	    sciprint(_("%s: No more memory.\n"),"send_command");
	    return (1);
	  }
	p->flag = flag;
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
	    __Lock(&commandQueueSingleAccess);
	    q = commandQueue;
	    while ((r = q->next))	q = r;
	    q->next = p;
	    __UnLock(&commandQueueSingleAccess);
	  }
	//**
	//** We have something to do, awake Scilab !!!!!!
	//**
	__Signal(&LaunchScilab);
	return (0);
}

/*--------------------------------------------------------------------------*/
/*
 * Gets info on the first queue element
 * and remove it from the queue
 */
/*--------------------------------------------------------------------------*/
int GetCommand ( char *str)
{
  int flag = 0;
  if (commandQueue != NULL)
    {

      CommandRec *p;

      __Lock(&commandQueueSingleAccess);
      p = commandQueue;
      strcpy (str, p->command);
      flag=p->flag;

      commandQueue = p->next;
         FREE (p->command);
      FREE (p);
      if (C2F(iop).ddt==-1) {
        if (flag==0) { sciprint_full(_("Unqueuing %s - No option.\n"),str); }
        else         { sciprint_full(_("Unqueuing %s - seq.\n"),str); }
      }
      __UnLock(&commandQueueSingleAccess);
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
