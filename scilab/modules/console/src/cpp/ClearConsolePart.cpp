/*--------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*--------------------------------------------------------------------------*/
#include "ClearConsolePart.hxx"
/*--------------------------------------------------------------------------*/
#include "org_scilab_modules_gui_bridge.hxx"
using namespace  org_scilab_modules_gui_bridge;
BOOL ClearConsolePart(int nbLines)
{
  CallScilabBridge::clear(getScilabJavaVM(), nbLines);
  return TRUE;
}
/*--------------------------------------------------------------------------*/
