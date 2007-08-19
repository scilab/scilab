/*-----------------------------------------------------------------------------------*/
/** 
 * @copyright INRIA 2007
 * @author Vincent COUVERT
 */
/*-----------------------------------------------------------------------------------*/
#include "InterpreterManagement.h"
#include "dynamic_menus.h"
#include "ScilabEventsLoop.h"
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
