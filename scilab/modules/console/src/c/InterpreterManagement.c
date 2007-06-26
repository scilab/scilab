/** 
 * @copyright INRIA 2007
 * @author Vincent COUVERT
 */

#include "InterpreterManagement.h"

int PutCommandInScilabQueue(char *command)
{
  return StoreCommand1(command, 1);
}
