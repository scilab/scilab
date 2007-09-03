/*-----------------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include <iostream>

#include <stdlib.h>
#include <jni.h>
#include "machine.h"
extern "C" {
#include "../../../jvm/includes/getScilabJNIEnv.h"
#include "../../../jvm/includes/getScilabObject.h"
#include "../../../jvm/includes/getScilabJavaVM.h"
}
//#include "../jni/org_scilab_modules_gui_bridge_console.hxx"
#include "SetConsolePrompt.hxx"
#include "MALLOC.h"

/*-----------------------------------------------------------------------------------*/
//using namespace  org_scilab_modules_gui_bridge_console;
BOOL SetConsolePrompt(char *Sci_prompt)
{
  /* Boolean flag to be return */
  BOOL res = FALSE;

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
                      jclass sciConsoleObjClass = env->GetObjectClass(sciConsoleObj);
                      if (sciConsoleObjClass)
                        {
                          jmethodID setPromptID = env->GetMethodID(sciConsoleObjClass, "setPrompt", "(Ljava/lang/String;)V");
                          if (setPromptID)
                            {
                              jstring jstr = env->NewStringUTF(Sci_prompt);
                              env->CallVoidMethod(sciConsoleObj, setPromptID, jstr);
                              res = TRUE;
                            }
                          else
                            std::cerr << "Could not get setPrompt method." << std::endl;
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
  
  return res;
}
