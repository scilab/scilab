/*------------------------------------------------------------------------
 *    Copyright (C) 2001-2003 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "../graphics/Math.h"
#include "All-extern.h"
#include "../sun/Sun.h"

/*---------------------------------------------------------------------
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
 *---------------------------------------------------------------------*/

typedef struct commandRec {
  char                *command;  /* command info one string two integers */
  struct commandRec   *next;
} CommandRec, *CommandRecPtr;

static CommandRec *commandQueue = NULL;

int scig_command_handler_none (char *command) {return 0;}

static Scig_command_handler scig_command_handler = scig_command_handler_none;

Scig_command_handler set_scig_command_handler(Scig_command_handler f)
{
  Scig_command_handler old = scig_command_handler;
  scig_command_handler = f;
  return old;
}

void reset_scig_command_handler() 
{
  scig_command_handler = scig_command_handler_none;
}

/*---------------------------------------------------------------
 * try to execute a command or add it to the end of command queue 
 *----------------------------------------------------------------*/
int
StoreCommand (command)
     char *command;
{
  return (StoreCommand1 (command, 1));
}

int StoreCommand1( char *command, int flag)
{
  CommandRec *p, *q, *r;

  /** first check if we have a special handler set for commands **/
  if ( scig_command_handler(command) == 1) return 0;

  if (flag==1&& get_is_reading ())
    {
      write_scilab (command);
      /*      if (flag == 1)
	      write_scilab ("\n");*/
      return 0;
    }

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

  /* this is important: to quit Xorgetchar */
  if (flag==2&& get_is_reading ())
    write_scilab ("");
     
  /*  if (get_is_reading()) 
    { 
      int i;
      C2F(xscion)(&i);
      if (i) write_scilab("");
      }*/

  return(0);
}

/*---------------------------------------------------------------------------
 * Gets info on the first queue element by copying 
 * command in str and remove it from the queue 
 *---------------------------------------------------------------------------*/

static void get_scilab_command(char *str) 
{
  if ( commandQueue != NULL)
    {
      CommandRec *p;
      p= commandQueue;
      strcpy(str, p->command);
      commandQueue = p->next;
      FREE(p->command);
      FREE(p);
    }
}

/*---------------------------------------------------------------------------
 * Checks if there's something on the 
 * commandQueue 
 *---------------------------------------------------------------------------*/

integer C2F(ismenu)()
{
  return ( commandQueue == NULL ) ? 0 : 1 ;
}

/*---------------------------------------------------------------------------
 * menu/button info for Scilab 
 ---------------------------------------------------------------------------*/

int C2F(getmen)(char * btn_cmd,integer * lb, integer * entry)
{
  if (C2F(ismenu)()==1) 
    {
      get_scilab_command(btn_cmd);
      *lb=strlen(btn_cmd);
    }
  else
    { 
      *lb =0;
      *entry=0;
    }
  return(0);
}

