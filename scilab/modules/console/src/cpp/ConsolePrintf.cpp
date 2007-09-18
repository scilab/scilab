/*-----------------------------------------------------------------------------------*/
/**
* @author Allan CORNET - INRIA 2007
* @author Vincent COUVERT - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include "ConsolePrintf.hxx"
/*-----------------------------------------------------------------------------------*/
using namespace  org_scilab_modules_gui_bridge_console;
int ConsolePrintf(char *line)
{
  JavaVM scilabJVM = *getScilabJavaVM();
  jobject scilabConsoleObj = GetScilabConsoleObject();

  if (scilabConsoleObj) 
    {
      SwingScilabConsole *jConsole = new SwingScilabConsole(&scilabJVM, scilabConsoleObj);
      
      (*jConsole).display(line);

      delete(jConsole);
    }

  /* Should return the length of the string displayed */
  return 0;
}
/*-----------------------------------------------------------------------------------*/
