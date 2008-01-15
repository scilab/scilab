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
#include "sigbas.h"
/*--------------------------------------------------------------------------*/
int putCommandInScilabQueue(char *command)
{
  return StoreCommand(command);
}
/*--------------------------------------------------------------------------*/
int interruptScilab(void)
{
  int scilabSignal = SIGINT;
  C2F(sigbas)(&scilabSignal);
  return 0;
}
/*--------------------------------------------------------------------------*/
