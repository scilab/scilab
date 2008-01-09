/*--------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*--------------------------------------------------------------------------*/
#include "ConsoleIsWaitingForInput.hxx"
/*--------------------------------------------------------------------------*/
#include "org_scilab_modules_gui_bridge.hxx"
using namespace  org_scilab_modules_gui_bridge;
BOOL ConsoleIsWaitingForInput(void)
{
  return CallScilabBridge::isWaitingForInput(getScilabJavaVM());
}
/*--------------------------------------------------------------------------*/

