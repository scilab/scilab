/*-----------------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include <string>
#include "ConsoleRead.hxx"
/*-----------------------------------------------------------------------------------*/
using namespace  org_scilab_modules_gui_bridge_console;

char *ConsoleRead()
{
  /* Character string to be returned */
  std::string strRead;
  char *strValue = NULL;

  JavaVM scilabJVM = *getScilabJavaVM();
  jobject scilabConsoleObj = GetScilabConsoleObject();

  if (scilabConsoleObj)
    {
      SwingScilabConsole *jConsole = new SwingScilabConsole(&scilabJVM, scilabConsoleObj);

      strValue = (*jConsole).readLine();

      if (strValue)
      {
	   strRead = *new std::string(strRead);
	   free(strValue); // malloc in (*jConsole).readLine
	   strValue = NULL;
      }

      delete(jConsole);
    }

  return (char *) strRead.c_str();
}
/*-----------------------------------------------------------------------------------*/

