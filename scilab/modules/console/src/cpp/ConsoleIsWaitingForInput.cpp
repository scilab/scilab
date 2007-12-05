/*--------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*--------------------------------------------------------------------------*/
#include "ConsoleIsWaitingForInput.hxx"
/*--------------------------------------------------------------------------*/
using namespace  org_scilab_modules_gui_bridge_console;
BOOL ConsoleIsWaitingForInput(void)
{
  /* Value to be returned */
  BOOL returnFlag = FALSE;

  JavaVM scilabJVM = *getScilabJavaVM();
  jobject scilabConsoleObj = GetScilabConsoleObject();

  if (scilabConsoleObj)
    {
      SwingScilabConsole *jConsole = new SwingScilabConsole(&scilabJVM, scilabConsoleObj);
      returnFlag = (*jConsole).isWaitingForInput();
      delete(jConsole);
    }

  return returnFlag;
}
/*--------------------------------------------------------------------------*/

