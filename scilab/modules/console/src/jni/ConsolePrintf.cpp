/*-----------------------------------------------------------------------------------*/
/**
* @author Allan CORNET - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <jni.h>
extern "C" {
#include "./../../jvm/includes/getScilabObject.h"
#include "../../../jvm/includes/getScilabJNIEnv.h"
}
#include "ConsolePrintf.hxx"
/*-----------------------------------------------------------------------------------*/
int ConsolePrintf(char *line)
{
	int len = 0;
	jobject  ScilabObj = getScilabObject();
	JNIEnv *env = getScilabJNIEnv();

	/* get the class */
	jclass class_Mine = env->GetObjectClass(ScilabObj);

	jfieldID id_Console =  env->GetFieldID(class_Mine, "sciConsole","Lorg/scilab/modules/gui/console/Console;");
	if (id_Console)
	{
		jobject jConsole = env->GetObjectField(ScilabObj, id_Console);
		{
			if (jConsole)
			{
				jclass cls = env->GetObjectClass(jConsole);
				if (cls)
				{
					jmethodID mid = env->GetMethodID(cls, "display", "(Ljava/lang/String;)V");
					if (mid)
					{
						jstring jstr;
						jstr = env->NewStringUTF(line);
						env->CallVoidMethod(jConsole, mid, jstr);
					}
				}
			}
		}
	}


	return len;
}
/*-----------------------------------------------------------------------------------*/
