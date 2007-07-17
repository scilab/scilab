/*-----------------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <jni.h>
#include "./../../jvm/includes/getScilabObject.h"
#include "../../../jvm/includes/getScilabJNIEnv.h"
#include "machine.h"
/*-----------------------------------------------------------------------------------*/
BOOL PromptToHome(void)
{
  jobject  ScilabObj = getScilabObject();
  JNIEnv *env = getScilabJNIEnv();

  if (env)
    {
      jclass class_Mine = (*env)->GetObjectClass(env, ScilabObj);
      if (class_Mine)
        {
          jfieldID id_Console =  (*env)->GetFieldID(env, class_Mine, "sciConsole","Lorg/scilab/modules/gui/console/Console;");
          if (id_Console)
            {
              jobject jConsole = (*env)->GetObjectField(env, ScilabObj, id_Console);
              if (jConsole)
                {
                  jclass cls = (*env)->GetObjectClass(env, jConsole);
                  if (cls)
                    {
                      jmethodID mid = (*env)->GetMethodID(env, cls, "toHome", "()V");
                      if (mid)
                        {
                          (*env)->CallVoidMethod(env, jConsole, mid);
                          return TRUE;
                        }
                      else
                        return FALSE;
                    }
                  else
                    return FALSE;
                }
              else
                return FALSE;
            }
          else
            return FALSE;
        }
      else
        return FALSE;
    }
  else
    return FALSE;
}
