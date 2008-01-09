/*--------------------------------------------------------------------------*/
/**
* @author Allan CORNET - INRIA 2007
* @author Vincent COUVERT - INRIA 2007
*/
/*--------------------------------------------------------------------------*/
#include "ConsolePrintf.hxx"
/*--------------------------------------------------------------------------*/
#include "org_scilab_modules_gui_bridge.hxx"
using namespace  org_scilab_modules_gui_bridge;
int ConsolePrintf(char *line)
{
  CallScilabBridge::display(getScilabJavaVM(), line);
  return 0;
}
/*--------------------------------------------------------------------------*/
