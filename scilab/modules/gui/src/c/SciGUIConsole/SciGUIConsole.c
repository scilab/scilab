/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "SciGUIConsole.h"
#include "../../jvm/includes/JVM_CONSOLE.h"
/*-----------------------------------------------------------------------------------*/ 
static jobject SciGUIConsoleObject;
/*-----------------------------------------------------------------------------------*/ 
int PutString_SciGUIConsole(JNIEnv *env,char *Str);
/*-----------------------------------------------------------------------------------*/ 
IMPORT_EXPORT_SCIGUICONSOLE_DLL jobject Get_SciGUIConsole_Object(void)
{
	return (jobject)SciGUIConsoleObject;
}
/*-----------------------------------------------------------------------------------*/ 
IMPORT_EXPORT_SCIGUICONSOLE_DLL int JVM_Create_SciGUIConsole_Object(JNIEnv *env)
{
	jclass cls=NULL;
	jmethodID mid=NULL;
	int bOK=FALSE;

	cls = (*env)->FindClass(env, "SciGUIConsole");
	if (cls)
	{
		mid = (*env)->GetMethodID(env,cls,"<init>","()V");
		if (mid)
		{
			SciGUIConsoleObject = (*env)->NewObject(env,cls,mid); 
			if (SciGUIConsoleObject)
			{
				bOK=TRUE;
			}
		}
	}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
IMPORT_EXPORT_SCIGUICONSOLE_DLL int Initialize_SciGUIConsole_Object(JNIEnv *env)
{
	int bOK=FALSE;
	jclass cls=NULL;
	jmethodID mid=NULL;

	cls = (*env)->FindClass(env, "SciGUIConsole");
	if (cls)
	{
		mid = (*env)->GetMethodID(env, cls, "Initialize","()V");
		if (mid)
		{
			(*env)->CallObjectMethod(env,(jobject)SciGUIConsoleObject, mid,NULL);
			bOK=TRUE;
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
IMPORT_EXPORT_SCIGUICONSOLE_DLL int Events_Loop_SciGUIConsole(JNIEnv *env)
{
	int bOK=FALSE;
	jclass cls=NULL;
	jmethodID mid=NULL;

	cls = (*env)->FindClass(env, "SciGUIConsole");
	if (cls)
	{
		mid = (*env)->GetMethodID(env, cls, "EventsLoop",  "()V");
		if (mid)
		{
			(*env)->CallObjectMethod(env,(jobject)SciGUIConsoleObject, mid,NULL);
			bOK=TRUE;
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
int PutString_SciGUIConsole(JNIEnv *env,char *Str)
{
	int bOK=FALSE;
	jclass cls=NULL;
	jmethodID mid=NULL;
	jstring jstr;

	cls = (*env)->FindClass(env, "SciGUIConsole");
	if (cls)
	{
		mid = (*env)->GetMethodID(env, cls, "PutString",  "(Ljava/lang/String;)V");
		if (mid)
		{
			jstr = (*env)->NewStringUTF(env, Str);
			(*env)->CallObjectMethod(env,(jobject)SciGUIConsoleObject, mid,jstr );
			bOK=TRUE;
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
IMPORT_EXPORT_SCIGUICONSOLE_DLL int PutString(char *Str)
{
	int bOK=FALSE;
	JavaVM *jvm_CONSOLE=NULL;
	JNIEnv *env=NULL;	

	jvm_CONSOLE=Get_jvm_CONSOLE();
	if (jvm_CONSOLE)
	{
		(*jvm_CONSOLE)->GetEnv(jvm_CONSOLE, (void **)&env, JNI_VERSION_1_4);
		if (env)
		{
			PutString_SciGUIConsole(env,Str);
			bOK=TRUE;
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
IMPORT_EXPORT_SCIGUICONSOLE_DLL int IsEnabled_SciGUIConsole(JNIEnv *env)
{
	int bOK=FALSE;
	jclass cls=NULL;
	jmethodID mid=NULL;

	if (SciGUIConsoleObject)
	{
		cls = (*env)->FindClass(env, "SciGUIConsole");
		if (cls)
		{
			mid = (*env)->GetMethodID(env, cls, "IsEnabled",  "()Z");
			if (mid)
			{
				bOK=(*env)->CallBooleanMethod(env,SciGUIConsoleObject,mid);
			}
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
IMPORT_EXPORT_SCIGUICONSOLE_DLL int Dispose_SciGUIConsole(JNIEnv *env)
{
	int bOK=FALSE;
	jclass cls=NULL;
	jmethodID mid=NULL;

	if (SciGUIConsoleObject)
	{
		cls = (*env)->FindClass(env, "SciGUIConsole");
		if (cls)
		{
			mid = (*env)->GetMethodID(env, cls, "dispose",  "()V");
			if (mid)
			{
				(*env)->CallVoidMethod(env,SciGUIConsoleObject,mid);
				bOK=TRUE;
			}
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
