/*-----------------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include "GetScilabConsoleObject.hxx"
/*-----------------------------------------------------------------------------------*/
jobject GetScilabConsoleObject()
{
  jobject ScilabObj = getScilabObject();
  JNIEnv *env = getScilabJNIEnv();

  if ((env!=NULL) && (ScilabObj!=NULL))
    {
      jclass ScilabObjectClass = env->GetObjectClass(ScilabObj);
      if (ScilabObjectClass)
        {
          jmethodID mid_getSciConsole =  env->GetMethodID(ScilabObjectClass, "getSciConsole","()Lorg/scilab/modules/gui/console/Console;");
          if (mid_getSciConsole)
            {
              return env->CallObjectMethod(ScilabObj, mid_getSciConsole);
            }
        }
    }
  return NULL;
}
/*-----------------------------------------------------------------------------------*/
