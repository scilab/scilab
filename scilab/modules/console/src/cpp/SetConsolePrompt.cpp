/*--------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*--------------------------------------------------------------------------*/
#include "SetConsolePrompt.hxx"
#include "org_scilab_modules_gui_bridge.hxx"
/*--------------------------------------------------------------------------*/
using namespace  org_scilab_modules_gui_bridge;
BOOL SetConsolePrompt(char *Sci_prompt)
{
  CallScilabBridge::setPrompt(getScilabJavaVM(), Sci_prompt);
  return TRUE;
}
