/*--------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*--------------------------------------------------------------------------*/
#include "ScilabLinesUpdate.hxx"
/*--------------------------------------------------------------------------*/
#include "org_scilab_modules_gui_bridge.hxx"
using namespace  org_scilab_modules_gui_bridge;
BOOL ScilabLinesUpdate(void)
{
  CallScilabBridge::scilabLinesUpdate(getScilabJavaVM());
  return TRUE;
}
/*--------------------------------------------------------------------------*/
