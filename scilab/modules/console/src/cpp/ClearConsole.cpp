/*--------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*--------------------------------------------------------------------------*/
#include "ClearConsole.hxx"
/*--------------------------------------------------------------------------*/
#include "org_scilab_modules_gui_bridge.hxx"
using namespace  org_scilab_modules_gui_bridge;
BOOL ClearConsole(void)
{
  CallScilabBridge::clear(getScilabJavaVM());
  return TRUE;
}
/*--------------------------------------------------------------------------*/
