/*-----------------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <jni.h>
#include "machine.h"
extern "C" {
#include "../../../jvm/includes/getScilabJNIEnv.h"
#include "../../../jvm/includes/getScilabObject.h"
}
#include "../jni/org_scilab_modules_gui_bridge_console.hxx"
#include "ShellRead.hxx"
#include "MALLOC.h"

/*-----------------------------------------------------------------------------------*/
#define WK_BUF_SIZE 520
/*-----------------------------------------------------------------------------------*/
using namespace  org_scilab_modules_gui_bridge_console;
char *ShellRead()
{
  /* Character string to be return */
  char *strRead = NULL;

  /* Gets Scilab JNI environment */
  JNIEnv *env = getScilabJNIEnv();
  if (env)
    {
      /* Gets Scilab JAVA object */
      jobject scilabObj = getScilabObject();
      if (scilabObj)
        {
          /* Gets Scilab object class */
          jclass scilabObjClass = env->GetObjectClass(scilabObj);
          if (scilabObjClass)
            {
              /* Get the ID of the method which returns the sciConsole object */
              jmethodID getSciConsoleID =  env->GetMethodID(scilabObjClass, "getSciConsole", "()Lorg/scilab/modules/gui/console/Console;");
              if (getSciConsoleID)
                {
                  /* Gets sciConsole object */
                  jobject sciConsoleObj = env->CallObjectMethod(scilabObj, getSciConsoleID);
                  if (sciConsoleObj) 
                    {
                      /* WITH GIWS GENERATED FILES */
//                       SwingScilabConsole *sciConsole = new SwingScilabConsole(env, sciConsoleObj);
                      
//                       /* Call Java to get the user input */
//                       char *strValue = (*sciConsole).readLine();
                      
//                       if (strValue)
//                         {
//                           strRead = (char*) MALLOC(sizeof(char)*(WK_BUF_SIZE + 1));
//                           if (strRead) 
//                             strcpy(strRead,strValue);
//                           else
//                             std::cerr << "Could not alloc return variable." << std::endl;
//                         }
//                       else
//                         std::cerr << "Could not read user input value." << std::endl;
                      /* END WITH GIWS GENERATED FILES */
                      
                      jclass sciConsoleObjClass = env->GetObjectClass(sciConsoleObj);
                      if (sciConsoleObjClass)
                        {
                          jmethodID readlineID = env->GetMethodID(sciConsoleObjClass, "readLine", "()Ljava/lang/String;");
                          if (readlineID)
                            {
                              jstring jstrValue = (jstring) env->CallObjectMethod(sciConsoleObj, readlineID);
                              if (jstrValue)
                                {
                                  const char *strValue = NULL;
                                  strValue = env->GetStringUTFChars(jstrValue, (jboolean) 0);
                                  if (strValue)
                                    {
                                      strRead = (char*) MALLOC(sizeof(char)*(WK_BUF_SIZE + 1));
                                      if (strRead) 
                                        strcpy(strRead,strValue);
                                      else
                                        std::cerr << "Could not alloc return variable." << std::endl;
                                    }
                                  else
                                    std::cerr << "Could not get user input value." << std::endl;
                                }
                              else
                                std::cerr << "Could not read user input value." << std::endl;
                            }
                          else
                            std::cerr << "Could not get readline method." << std::endl;
                        }
                      else
                        std::cerr << "Could not get Scilab Console object class." << std::endl;
                    }
                  else
                    std::cerr << "Could not get sciConsole object." << std::endl;
                }
              else
                std::cerr << "Could not get getSciConsole method ID." << std::endl;
            }
          else
            std::cerr << "Could not get Scilab JAVA object class." << std::endl;
        }
      else
        std::cerr << "Could not get Scilab JAVA object." << std::endl;
    }
  else
    std::cerr << "Could not get Scilab JNI environment." << std::endl;
  
  return strRead;
}
