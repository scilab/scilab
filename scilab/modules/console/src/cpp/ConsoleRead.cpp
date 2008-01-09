/*--------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*--------------------------------------------------------------------------*/
#include "ConsoleRead.hxx"
/*--------------------------------------------------------------------------*/
#include "org_scilab_modules_gui_bridge.hxx"
using namespace  org_scilab_modules_gui_bridge;
char *ConsoleRead(void)
{
  return CallScilabBridge::readLine(getScilabJavaVM());
}
/*--------------------------------------------------------------------------*/

