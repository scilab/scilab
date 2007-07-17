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
#include "ShellRead.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/
#define WK_BUF_SIZE 520
/*-----------------------------------------------------------------------------------*/
char *ShellRead()
{
  jobject  ScilabObj = getScilabObject();
  JNIEnv *env = getScilabJNIEnv();

  char *strRead = NULL;
  
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
                        jmethodID mid = (*env)->GetMethodID(env, cls, "readLine", "()Ljava/lang/String;");
                        if (mid)
                          {
                            jstring jstrValue = (jstring) (*env)->CallObjectMethod(env, jConsole, mid);
                            if (jstrValue)
                              {
                                const char *strValue = NULL;
                                strValue = (*env)->GetStringUTFChars(env,jstrValue, 0);
                                if (strValue)
								{
									strRead = (char*) MALLOC(sizeof(char)*(WK_BUF_SIZE + 1));
									if (strRead) strcpy(strRead,strValue);
								}
                              }
                          }
                      }
                  }
              }
        }
    }
  return strRead;
}
