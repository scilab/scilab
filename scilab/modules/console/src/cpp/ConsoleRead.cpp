/*--------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*--------------------------------------------------------------------------*/
#include "ConsoleRead.hxx"
/*--------------------------------------------------------------------------*/
using namespace  org_scilab_modules_gui_bridge_console;
char *ConsoleRead(void)
{
  /* Character string to be returned */
  char *strRead = NULL;

  JavaVM scilabJVM = *getScilabJavaVM();
  jobject scilabConsoleObj = GetScilabConsoleObject();

  if (scilabConsoleObj)
    {
      SwingScilabConsole *jConsole = new SwingScilabConsole(&scilabJVM, scilabConsoleObj);
      strRead = (*jConsole).readLine();
      delete(jConsole);
    }

  return strRead;
}
/*--------------------------------------------------------------------------*/

