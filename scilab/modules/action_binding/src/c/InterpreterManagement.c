/*--------------------------------------------------------------------------*/
/** 
 * @copyright INRIA 2007
 * @author Vincent COUVERT
 */
/*--------------------------------------------------------------------------*/
#include <signal.h> /* for SIGINT */
#include "InterpreterManagement.h"
#include "dynamic_menus.h"
#include "machine.h"
#include "ScilabEventsLoop.h"
#include "sigbas.h"
/*--------------------------------------------------------------------------*/
int putCommandInScilabQueue(char *command)
{
  return StoreCommand(command);
}
/*--------------------------------------------------------------------------*/
int execScilabEventLoop(void)
{
	ScilabEventsLoop();
	return 0;
}
/*--------------------------------------------------------------------------*/
int interruptScilab(void)
{
  int scilabSignal = SIGINT;
  C2F(sigbas)(&scilabSignal);
  return 0;
}
/*--------------------------------------------------------------------------*/
BOOL haveCommandsInTheQueue(void)
{
  return ismenu();
}
/*--------------------------------------------------------------------------*/
