/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include <jni.h>
#include "setlookandfeel.h"
#include "MALLOC.h"
#include "getScilabJNIEnv.h"
/*--------------------------------------------------------------------------*/ 
BOOL setlookandfeel(char *lookandfeekstr)
{
	BOOL bOK=FALSE;
	JNIEnv *env = getScilabJNIEnv();

	if (env)
	{
		jclass classLookAndFeel = (*env)->FindClass(env, "org/scilab/modules/gui/utils/LookAndFeelManager");
		if (classLookAndFeel)
		{
			jmethodID methodgetInstalledLookAndFeels = (*env)->GetStaticMethodID(env,classLookAndFeel, "setLookAndFeel", "(Ljava/lang/String;)Z"); 
			jobjectArray jStrings = (*env)->CallStaticObjectMethod(env,classLookAndFeel,methodgetInstalledLookAndFeels,NULL);
			jstring jstr = (*env)->NewStringUTF(env,lookandfeekstr);
			jboolean jbool = (*env)->CallStaticBooleanMethod(env,classLookAndFeel,methodgetInstalledLookAndFeels,jstr);
			bOK = (BOOL) jbool;
		}
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/ 
BOOL setsystemlookandfeel(void)
{
	BOOL bOK=FALSE;
	JNIEnv *env = getScilabJNIEnv();

	if (env)
	{
		jclass classLookAndFeel = (*env)->FindClass(env, "org/scilab/modules/gui/utils/LookAndFeelManager");
		if (classLookAndFeel)
		{
			jmethodID methodgetInstalledLookAndFeels = (*env)->GetStaticMethodID(env,classLookAndFeel, "setSystemLookAndFeel", "()Z"); 
			jboolean jbool = (*env)->CallStaticBooleanMethod(env,classLookAndFeel,methodgetInstalledLookAndFeels);
			bOK = (BOOL) jbool;
		}
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/ 
