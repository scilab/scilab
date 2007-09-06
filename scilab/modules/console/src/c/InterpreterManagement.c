/*-----------------------------------------------------------------------------------*/
/** 
 * @copyright INRIA 2007
 * @author Vincent COUVERT
 */
/*-----------------------------------------------------------------------------------*/
#include "InterpreterManagement.h"
#include "dynamic_menus.h"
#include "ScilabEventsLoop.h"
#include <signal.h> /* for SIGINT */
/*-----------------------------------------------------------------------------------*/
int putCommandInScilabQueue(char *command)
{
  return StoreCommand(command);
}
/*-----------------------------------------------------------------------------------*/
int execScilabEventLoop(void)
{
	ScilabEventsLoop();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int interruptScilab(void)
{
  int signal = SIGINT;
  C2F(sigbas)(&signal);
  return 0;
}
/*-----------------------------------------------------------------------------------*/
