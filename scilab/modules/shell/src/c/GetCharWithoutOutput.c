/*-----------------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <jni.h>
#include "machine.h"
#include "./../../jvm/includes/getScilabObject.h"
#include "../../../jvm/includes/getScilabJNIEnv.h"
#include "MALLOC.h"
#include "GetCharWithoutOutput.h"
/*-----------------------------------------------------------------------------------*/
int GetCharWithoutOutput(void)
{
  jobject  ScilabObj = getScilabObject();
  JNIEnv *env = getScilabJNIEnv();

  int intValue = 0;
  
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
                        jmethodID mid = (*env)->GetMethodID(env, cls, "getCharWithoutOutput", "()I");
                        if (mid)
                          {
                            intValue = (int) (*env)->CallObjectMethod(env, jConsole, mid);
                          }
                      }
                  }
              }
        }
    }
  return intValue;
}
