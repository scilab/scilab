/*--------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*--------------------------------------------------------------------------*/
#include "ClearConsole.hxx"
/*--------------------------------------------------------------------------*/
using namespace  org_scilab_modules_gui_bridge_console;
BOOL ClearConsole(void)
{
  JavaVM scilabJVM = *getScilabJavaVM();
  jobject scilabConsoleObj = GetScilabConsoleObject();

  if (scilabConsoleObj) 
    {
      SwingScilabConsole *jConsole = new SwingScilabConsole(&scilabJVM, scilabConsoleObj);
      
      (*jConsole).clear();

      delete(jConsole);

      return TRUE;
    }

  return FALSE;
}
/*--------------------------------------------------------------------------*/
