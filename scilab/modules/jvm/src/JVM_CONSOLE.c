/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "JVM_CONSOLE.h"
#ifdef _MSC_VER
#include "JVM_Windows.h"
#endif
#ifndef _MSC_VER
	#include <pthread.h>
#endif
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
static JavaVM *jvm_CONSOLE=NULL;
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
static HANDLE hThreadJVM_CONSOLE=NULL;
#else
static pthread_t tid;
#endif

/*-----------------------------------------------------------------------------------*/ 
IMPORT_DLL int JVM_Create_SciGUIConsole_Object(JNIEnv *env);
IMPORT_DLL int Initialize_SciGUIConsole_Object(JNIEnv *env);
IMPORT_DLL int Events_Loop_SciGUIConsole(JNIEnv *env);
IMPORT_DLL int IsEnabled_SciGUIConsole(JNIEnv *env);
/*-----------------------------------------------------------------------------------*/ 
void Thread_JVM_CONSOLE(void *arg);
int Create_JVM_Thread_CONSOLE(void);
int Terminate_JVM_Thread_CONSOLE(void);
/*-----------------------------------------------------------------------------------*/
void Thread_JVM_CONSOLE(void *arg)
{
	JNIEnv *env;
	jint res;
	
	res = (*jvm_CONSOLE)->AttachCurrentThread(jvm_CONSOLE, (void**) &env, (void*)NULL);

	JVM_Create_SciGUIConsole_Object(env);

	Initialize_SciGUIConsole_Object(env);

	Events_Loop_SciGUIConsole(env);

	res = (*jvm_CONSOLE)->DetachCurrentThread(jvm_CONSOLE);

}
/*-----------------------------------------------------------------------------------*/ 
IMPORT_EXPORT_LIBJVM_DLL JavaVM *Get_jvm_CONSOLE(void)
{
	return jvm_CONSOLE;
}
/*-----------------------------------------------------------------------------------*/ 
int Create_JVM_Thread_CONSOLE(void)
{
	int bOK=FALSE;
	#ifdef _MSC_VER
		DWORD IdThreadJVM_CONSOLE;
		hThreadJVM_CONSOLE=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Thread_JVM_CONSOLE,NULL,0,(LPDWORD)&IdThreadJVM_CONSOLE);
		if (hThreadJVM_CONSOLE)
		{
			bOK=TRUE;
		}
	#else
		int thr_id=0;
		thr_id=pthread_create(&tid, NULL,(void*)Thread_JVM_CONSOLE, NULL);
		if (thr_id)
		{
			bOK=TRUE;
		}
	#endif
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
int Terminate_JVM_Thread_CONSOLE(void)
{
	int bOK=FALSE;
	#ifdef _MSC_VER
	if (TerminateThread(hThreadJVM_CONSOLE,0))
	{
		CloseHandle(hThreadJVM_CONSOLE);
		bOK=TRUE;
	}
	else
	{
		bOK=FALSE;
	}
	#else
	if (pthread_cancel(tid))
	{
		bOK=FALSE;
	}
	else
	{
		bOK=TRUE;
	}
	#endif
	
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
 int StartJVM_CONSOLE(char *SCILAB_PATH)
{
	JNIEnv *env=NULL;
	int bOK=FALSE;
	long status=0;
	char *JAVACLASSPATH=NULL;
	char *JAVALIBRARYPATH=NULL;

	JavaVMInitArgs vm_args;
	JavaVMOption options[3];
	
	JAVACLASSPATH=(char*)MALLOC( (strlen("-Djava.class.path=%s/bin;.")+strlen(SCILAB_PATH)+strlen(SCILAB_PATH)+strlen("/Java/SWT/SWT.jar")+strlen(SCILAB_PATH)+strlen("/modules/jvm/classes")+10)*sizeof(char) );
	sprintf(JAVACLASSPATH,"-Djava.class.path=%s/bin%c%s%s%c%s%s%c%s",SCILAB_PATH,PATH_SEPARATOR,SCILAB_PATH,"/modules/jvm/bin/SWT/SWT.jar",PATH_SEPARATOR,SCILAB_PATH,"/modules/jvm/classes",PATH_SEPARATOR,USER_CLASSPATH);

	JAVALIBRARYPATH=(char*)MALLOC( (strlen("-Djava.library.path=%s/lib")+strlen(JRE_PATH)+strlen(SCILAB_PATH)+strlen("/modules/jvm/bin/SWT")+10)*sizeof(char) );
	sprintf(JAVALIBRARYPATH,"-Djava.library.path=%s/lib%c%s%s",JRE_PATH,PATH_SEPARATOR,SCILAB_PATH,"/modules/jvm/bin/SWT");

	/* JAVACLASSPATH & JAVALIBRARYPATH sont liberes à la fin de l'execution de la JVM */

	options[0].optionString = "-Djava.compiler=NONE"; /* disable JIT */
	options[1].optionString = JAVACLASSPATH; /* user classes */
	options[2].optionString = JAVALIBRARYPATH;  /* set native library path */

	vm_args.version = JNI_VERSION_1_4;
	vm_args.options = options;
	vm_args.nOptions = 3;
	vm_args.ignoreUnrecognized = TRUE;

  #if _MSC_VER
		status = MyJNI_CreateJavaVM(&jvm_CONSOLE, (void**) &env, &vm_args);
	#else
		status = JNI_CreateJavaVM(&jvm_CONSOLE, (void**) &env, &vm_args);
	#endif

	if (status == JNI_ERR)
	{
		bOK=FALSE;
	}
	else
	{
		bOK=TRUE;
	}

		
	Create_JVM_Thread_CONSOLE();

	while (! IsEnabled_SciGUIConsole(env))
	{
	}

	
	return status;
}
/*-----------------------------------------------------------------------------------*/ 
int TerminateJVM_CONSOLE(void)
{
	int bOK=FALSE;
	JNIEnv *env=NULL;	

	if( (*jvm_CONSOLE)->GetEnv(jvm_CONSOLE, (void **)&env, JNI_VERSION_1_4) ) 
	{
		bOK=FALSE;
	}
	else
	{
		if ( (*jvm_CONSOLE)->DestroyJavaVM(jvm_CONSOLE) )
   		{
			bOK=FALSE;
		}
		else 
		{
			if (Terminate_JVM_Thread_CONSOLE())	bOK=TRUE;
			else bOK=FALSE;
		}
	}
	
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
