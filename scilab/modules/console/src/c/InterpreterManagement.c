/** 
 * @copyright INRIA 2007
 * @author Vincent COUVERT
 */

#include "InterpreterManagement.h"

int putCommandInScilabQueue(char *command)
{
  return StoreCommand1(command, 1);
}
