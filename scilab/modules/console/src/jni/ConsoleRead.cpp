/*-----------------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include "ConsoleRead.hxx"
/*-----------------------------------------------------------------------------------*/
#define WK_BUF_SIZE 520
/*-----------------------------------------------------------------------------------*/
using namespace  org_scilab_modules_gui_bridge_console;
char *ConsoleRead()
{
  /* Character string to be returned */
  char *strRead = NULL;
  char *strValue = NULL;

  JavaVM scilabJVM = *getScilabJavaVM();
  jobject scilabConsoleObj = GetScilabConsoleObject();

  if (scilabConsoleObj) 
    {
      SwingScilabConsole *jConsole = new SwingScilabConsole(&scilabJVM, scilabConsoleObj);
      
      strValue = (*jConsole).readLine();

      if (strValue)
        {
          strRead = new char[WK_BUF_SIZE+1];
          if (strRead) 
            strcpy(strRead,strValue);
        }
      
      delete(jConsole);
    }

  return strRead;
}
