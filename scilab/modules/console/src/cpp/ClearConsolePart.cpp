/*-----------------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include "ClearConsolePart.hxx"
/*-----------------------------------------------------------------------------------*/
using namespace  org_scilab_modules_gui_bridge_console;
BOOL ClearConsolePart(int nbLines)
{
  JavaVM scilabJVM = *getScilabJavaVM();
  jobject scilabConsoleObj = GetScilabConsoleObject();

  if (scilabConsoleObj) 
    {
      SwingScilabConsole *jConsole = new SwingScilabConsole(&scilabJVM, scilabConsoleObj);
      
      (*jConsole).clear(nbLines);

      delete(jConsole);

      return TRUE;
    }

  return FALSE;
}
/*-----------------------------------------------------------------------------------*/
