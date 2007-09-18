/*-----------------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include "PromptToHome.hxx"
/*-----------------------------------------------------------------------------------*/
using namespace  org_scilab_modules_gui_bridge_console;
BOOL PromptToHome(void)
{
  JavaVM scilabJVM = *getScilabJavaVM();
  jobject scilabConsoleObj = GetScilabConsoleObject();

  if (scilabConsoleObj) 
    {
      SwingScilabConsole *jConsole = new SwingScilabConsole(&scilabJVM, scilabConsoleObj);
      
      (*jConsole).toHome();

      delete(jConsole);

      return TRUE;
    }

  return FALSE;
}
