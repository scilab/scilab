/*-----------------------------------------------------------------------------------*/
/**
* @author Allan CORNET - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include <stdlib.h>

#ifdef WITH_JAVA
#include <jni.h>
#include "./../../jvm/includes/getScilabObject.h"
#include "../../../jvm/includes/getScilabJNIEnv.h"
#endif

#include "ShellPrintf.h"
/*-----------------------------------------------------------------------------------*/
int ShellPrintf(char *line)
{
	int len = 0;
	//jobject  ScilabObj = getScilabObject();
	//JNIEnv *env = getScilabJNIEnv();

	///* get the class */
	//jclass class_Mine = (*env)->GetObjectClass(env, ScilabObj);


	//jfieldID id_Console =  (*env)->GetFieldID(env, class_Mine, "console","Lorg/scilab/modules/console/ScilabConsole;");

	//jobject jConsole = (*env)->GetObjectField(env, ScilabObj, id_Console);

	//{
	//	jstring jstr;
	//	jclass cls = (*env)->GetObjectClass(env, jConsole);
	//	jmethodID mid = (*env)->GetMethodID(env, cls, "printf", "(Ljava/lang/String;)V");
	//	jstr = (*env)->NewStringUTF(env,line);

	//	(*env)->CallVoidMethod(env, jConsole, mid, jstr);

	//}

	return len;
}
/*-----------------------------------------------------------------------------------*/
