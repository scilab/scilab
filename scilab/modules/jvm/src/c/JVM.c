/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include "JVM.h"
#include "JVM_commons.h"
#ifdef _MSC_VER
	#include "JVM_Windows.h"
#else
	#include "JVM_Unix.h"
#endif
#include "MALLOC.h"
#include "getScilabJVM.h"
/*-----------------------------------------------------------------------------------*/ 
static JavaVM *jvm_SCILAB=NULL;
/*-----------------------------------------------------------------------------------*/ 
static char *JAVACLASSPATH=NULL;
static char *JAVALIBRARYPATH=NULL;
/*-----------------------------------------------------------------------------------*/ 
JavaVM *getScilabJVM(void)
{
	return jvm_SCILAB;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL startJVM(char *SCI_PATH)
{
	/**
	* http://java.sun.com/javase/6/docs/technotes/guides/jni/spec/invocation.html#wp15956
	*/
	BOOL bOK=FALSE;
	JNIEnv *env=NULL;
	long status=0;
	JavaVMInitArgs vm_args;
	JavaVMOption jvm_options[3];
	jint res=0;
	int length_JAVACLASSPATH=0;
	int length_JAVALIBRARYPATH=0;
	int itmp=0;

	if (! LoadDynLibJVM(SCI_PATH) ) return FALSE;
	
	memset(&vm_args, 0, sizeof(vm_args));
	SciJNI_GetDefaultJavaVMInitArgs(&vm_args);

	length_JAVACLASSPATH = (int) ( strlen("-Djava.class.path=%s%s%s%s")+
		strlen(SCI_PATH)+
		strlen(DEFAULT_SCILAB_CLASSPATH)+
		strlen(PATH_SEPARATOR)+
		strlen(USER_CLASSPATH) );

	JAVACLASSPATH=(char*) MALLOC( sizeof(char)*	( length_JAVACLASSPATH +1) );
	sprintf(JAVACLASSPATH,"-Djava.class.path=%s%s%s%s",SCI_PATH,DEFAULT_SCILAB_CLASSPATH,PATH_SEPARATOR,USER_CLASSPATH);

	length_JAVALIBRARYPATH = (int)( strlen("-Djava.library.path=%s%s/lib%s%s%s")+
		strlen(SCI_PATH)+
		strlen(JRE_PATH)+
		strlen(PATH_SEPARATOR)+
		strlen(SCI_PATH)+
		strlen(DEFAULT_SCILAB_LIBRARYPATH) );

	JAVALIBRARYPATH=(char*)MALLOC(sizeof(char)* ( length_JAVALIBRARYPATH +1) );

	sprintf(JAVALIBRARYPATH,"-Djava.library.path=%s%s/lib%s%s%s",SCI_PATH,JRE_PATH,PATH_SEPARATOR,SCI_PATH,DEFAULT_SCILAB_LIBRARYPATH);
	/* JAVACLASSPATH & JAVALIBRARYPATH sont liberes à la fin de l'execution de la JVM */

#ifdef JNI_VERSION_1_6
	vm_args.version = JNI_VERSION_1_6;
#elif JNI_VERSION_1_4
	vm_args.version = JNI_VERSION_1_4;
#elif
	#ifdef _MSC_VER
		MessageBox(NULL,"Incorrect version JNI.","Error",MB_ICONEXCLAMATION|MB_OK);
	#else
		printf("\n Error : Incorrect version JNI.\n");
	#endif
		exit(1);
#endif
	jvm_options[0].optionString = "-Djava.compiler=NONE"; /* disable JIT */
    jvm_options[1].optionString = JAVACLASSPATH;
	jvm_options[2].optionString = JAVALIBRARYPATH;

	vm_args.options = jvm_options;
	vm_args.nOptions = 3;
	vm_args.ignoreUnrecognized = TRUE;

	status = SciJNI_CreateJavaVM(&jvm_SCILAB, (void**) &env, &vm_args);

	if (status == JNI_ERR)
	{
		FreeDynLibJVM();
		if (JAVACLASSPATH){FREE(JAVACLASSPATH);JAVACLASSPATH=NULL;}
		if (JAVALIBRARYPATH){FREE(JAVALIBRARYPATH);JAVALIBRARYPATH=NULL;}
		bOK=FALSE;
	}
	else
	{
		res = (*jvm_SCILAB)->AttachCurrentThread(jvm_SCILAB, (void**) &env, (void*)NULL);
		if (res != 0)
		{
#ifdef _MSC_VER
			MessageBox(NULL,"Call to AttachCurrentThread.","Error",MB_ICONEXCLAMATION|MB_OK);
#else
			printf("\n Error : Call to AttachCurrentThread.\n");
#endif
			FreeDynLibJVM();
			if (JAVACLASSPATH){FREE(JAVACLASSPATH);JAVACLASSPATH=NULL;}
			if (JAVALIBRARYPATH){FREE(JAVALIBRARYPATH);JAVALIBRARYPATH=NULL;}
			bOK=FALSE;
		}
		else bOK=TRUE;
	}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL finishJVM(void)
{
	BOOL bOK=FALSE;

	if ( (*jvm_SCILAB)->DestroyJavaVM(jvm_SCILAB) )
	{
		bOK=FALSE;
	}
	else 
	{
		jvm_SCILAB = NULL;
		bOK=TRUE;
	}

	FreeDynLibJVM();

	if (JAVACLASSPATH){FREE(JAVACLASSPATH);JAVACLASSPATH=NULL;}
	if (JAVALIBRARYPATH){FREE(JAVALIBRARYPATH);JAVALIBRARYPATH=NULL;}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 

