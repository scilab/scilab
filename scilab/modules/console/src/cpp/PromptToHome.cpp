/*--------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*--------------------------------------------------------------------------*/
#include "PromptToHome.hxx"
#include "org_scilab_modules_gui_bridge.hxx"
/*--------------------------------------------------------------------------*/
using namespace  org_scilab_modules_gui_bridge;
BOOL PromptToHome(void)
{
  CallScilabBridge::toHome(getScilabJavaVM());
  return TRUE;
}
