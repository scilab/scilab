/*-----------------------------------------------------------------------------------*/
/**
* @author Allan CORNET - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <jni.h>
#include "./../../jvm/includes/getScilabObject.h"
#include "../../../jvm/includes/getScilabJNIEnv.h"
#include "ShellPrintf.h"
/*-----------------------------------------------------------------------------------*/
int ShellPrintf(char *line)
{
	int len = 0;
	jobject  ScilabObj = getScilabObject();
	JNIEnv *env = getScilabJNIEnv();

	/* get the class */
	jclass class_Mine = (*env)->GetObjectClass(env, ScilabObj);

	jfieldID id_Console =  (*env)->GetFieldID(env, class_Mine, "sciConsole","Lorg/scilab/modules/gui/console/Console;");
	if (id_Console)
	{
		jobject jConsole = (*env)->GetObjectField(env, ScilabObj, id_Console);
		{
			if (jConsole)
			{
				jclass cls = (*env)->GetObjectClass(env, jConsole);
				if (cls)
				{
					jmethodID mid = (*env)->GetMethodID(env, cls, "display", "(Ljava/lang/String;)V");
					if (mid)
					{
						jstring jstr;
						jstr = (*env)->NewStringUTF(env,line);
						(*env)->CallVoidMethod(env, jConsole, mid, jstr);
					}
				}
			}
		}
	}


	return len;
}
/*-----------------------------------------------------------------------------------*/
