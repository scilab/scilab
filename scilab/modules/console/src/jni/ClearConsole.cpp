/*-----------------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <jni.h>
#include "ClearConsole.hxx"
extern "C" {
#include "./../../jvm/includes/getScilabObject.h"
#include "../../../jvm/includes/getScilabJNIEnv.h"
}
/*-----------------------------------------------------------------------------------*/
BOOL ClearConsole(void)
{
  jobject  ScilabObj = getScilabObject();
  JNIEnv *env = getScilabJNIEnv();

  if (env)
    {
      jclass class_Mine = env->GetObjectClass(ScilabObj);
      if (class_Mine)
        {
          jfieldID id_Console =  env->GetFieldID(class_Mine, "sciConsole","Lorg/scilab/modules/gui/console/Console;");
          if (id_Console)
            {
              jobject jConsole = env->GetObjectField(ScilabObj, id_Console);
              if (jConsole)
                {
                  jclass cls = env->GetObjectClass(jConsole);
                  if (cls)
                    {
                      jmethodID mid = env->GetMethodID(cls, "clear", "()V");
                      if (mid)
                        {
                          env->CallVoidMethod(jConsole, mid);
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
/*-----------------------------------------------------------------------------------*/
