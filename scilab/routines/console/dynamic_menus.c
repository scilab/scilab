/*------------------------------------------------------------------------
 *    Copyright (C) 2001-2003 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 *    Changed: steer, jpc 2004 
 *--------------------------------------------------------------------------*/

#include <string.h>
#include "../machine.h"
#include "../graphics/Math.h"  /* malloc */

#ifndef NULL
#define NULL 0
#endif

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


extern void write_scilab  __PARAMS((char *s));
extern int get_is_reading  __PARAMS((void));
extern void sciprint __PARAMS((char *fmt,...));


typedef int (*Scig_command_handler) __PARAMS((char *));
extern Scig_command_handler set_scig_command_handler __PARAMS((Scig_command_handler f));
int  scig_command_handler_none (char *command);
extern int StoreCommand  __PARAMS((char *command));  
extern int StoreCommand1  __PARAMS((char *command, int flag));  

extern void GetCommand  __PARAMS((char *str));  
extern integer C2F (ismenu) __PARAMS((void));  
extern int C2F (getmen) __PARAMS((char *btn_cmd, integer *lb, integer *entry));  
extern void  reset_scig_command_handler __PARAMS((void));


static int wait_for_input_end=0; 

typedef struct commandRec
{
  char              *command;		/* command info one string two integers */
  struct commandRec *next;
} CommandRec, *CommandRecPtr;

static CommandRec *commandQueue = NULL;
static Scig_command_handler scig_command_handler = scig_command_handler_none;


/*-------------------------------------------------*
 * changing the default command handler 
 *-------------------------------------------------*/

int  scig_command_handler_none (char *command) {return 0;};

Scig_command_handler set_scig_command_handler (Scig_command_handler f)
{
  Scig_command_handler old = scig_command_handler;
  scig_command_handler = f;
  return old;
}

void reset_scig_command_handler ()
{
  scig_command_handler = scig_command_handler_none;
}

/*---------------------------------------------------------------
 * try to execute a command or add it to the end of command queue 
 *----------------------------------------------------------------*/

int StoreCommand ( char *command)
{
  return (StoreCommand1 (command, 0)); /* jpc 1->0 */
}

/*---------------------------------------------------------------
 * try to execute a command or add it to the end of command queue
 * flag = 1 : the command is shown in scilab window (if at prompt)
 * flag = 2 : only a new line is send in scilab window (if at prompt) 
 *----------------------------------------------------------------*/

int StoreCommand1 (char *command,int flag)
{
  CommandRec *p, *q, *r;

  /** first check if we have a special handler set for commands **/
  if (scig_command_handler (command) == 1)  return 0;

  if (flag==1 && get_is_reading ())
    {
      write_scilab (command);
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
  if (get_is_reading ())
    { 
#ifdef WIN32
      if (flag==2) { /* for windows */
	write_scilab ("\n");
      }
#endif
    }
  return (0);
}

/*-------------------------------------------------
 * Gets info on the first queue element 
 * and remove it from the queue 
 -------------------------------------------------*/

void GetCommand ( char *str)
{
	
  if (commandQueue != NULL)
    {

      CommandRec *p;
          	
      p = commandQueue;
      strcpy (str, p->command);
      commandQueue = p->next;
      free (p->command);
      free (p);
    }
}

/*-------------------------------------------------
 * Checks if there's something on the 
 * commandQueue 
 *-------------------------------------------------*/

integer C2F(ismenu)()
{
  if ( commandQueue == NULL ) 
    return(0) ;
  else 
    return(1);
}

/*-------------------------------------------------
 * menu/button info for Scilab 
 *-------------------------------------------------*/

int C2F(getmen)(char * btn_cmd,integer * lb, integer * entry)
{
  if (C2F(ismenu)()==1) 
    {
      GetCommand(btn_cmd);
      *lb=(integer)strlen(btn_cmd);
    }
  else
    { 
      *lb =0;
      *entry=0;
    }
  return(0);
}


/*-------------------------------------------------
 * ??????????????? 
 *-------------------------------------------------*/


void C2F(waitforinputend)( int *flag)
{
  wait_for_input_end=*flag;
}


int iswaitingforinputend()
{
  int iwait;
  iwait=wait_for_input_end;
  wait_for_input_end=0;
  return iwait;
}
