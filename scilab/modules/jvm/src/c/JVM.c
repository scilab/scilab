
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/ 
#include <string.h>
#include <stdlib.h>
#include "JVM.h"
#include "JVM_commons.h"
#include "JVM_functions.h"
#include "MALLOC.h"
#include "getScilabJavaVM.h"
#include "fromjava.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
/* #define _JNI_DEBUG_TRACE */
/*--------------------------------------------------------------------------*/ 
static JavaVM *jvm_SCILAB=NULL;
/*--------------------------------------------------------------------------*/ 
static char *JAVACLASSPATH=NULL;
static char *JAVALIBRARYPATH=NULL;
static BOOL HadAlreadyJavaVm=FALSE;
/*--------------------------------------------------------------------------*/ 
JavaVM *getScilabJavaVM(void)
{
	return jvm_SCILAB;
}
/*--------------------------------------------------------------------------*/ 
JNIEnv *getScilabJNIEnv(void)
{
	JNIEnv *JNIEnv_SCILAB=NULL;
	jint res=0;
	if (jvm_SCILAB)
	{
#ifdef JNI_VERSION_1_6
		res = (*jvm_SCILAB)->GetEnv(jvm_SCILAB, (void **)&JNIEnv_SCILAB, JNI_VERSION_1_6);
#elif JNI_VERSION_1_4
		res = (*jvm_SCILAB)->GetEnv(jvm_SCILAB, (void **)&JNIEnv_SCILAB, JNI_VERSION_1_4);
#endif
	}
	else
	{
#ifdef _MSC_VER
		MessageBox(NULL,_("\nError: Cannot return Scilab Java environment (jvm_SCILAB): check if the JVM has been loaded by Scilab before calling this function.\n"),_("Error"),MB_ICONEXCLAMATION|MB_OK);
#else
		printf(_("\nError: Cannot return Scilab Java environment (jvm_SCILAB): check if the JVM has been loaded by Scilab before calling this function.\n"));
#endif
	}
	return JNIEnv_SCILAB;
}
/*--------------------------------------------------------------------------*/ 
BOOL startJVM(char *SCI_PATH)
{
	JNIEnv *env=NULL;
	JavaVM *ptr_jvm = NULL;
	jint res=0;

	if (IsFromJava())
	{
		ptr_jvm = FindCreatedJavaVM(SCI_PATH);
		if (ptr_jvm) /* a jvm already exists in scilab process */
		{
			HadAlreadyJavaVm = TRUE;
			jvm_SCILAB = ptr_jvm;
			env = getScilabJNIEnv();
		}
		else 
		{
			FreeDynLibJVM();
			return FALSE;
		}
	}
	else
	{
		if (! LoadDynLibJVM(SCI_PATH) ) return FALSE;
		else
		{
			/**
			* http://java.sun.com/javase/6/docs/technotes/guides/jni/spec/invocation.html#wp15956
			*/
			long status=0;
			JavaVMInitArgs vm_args;
#ifdef _JNI_DEBUG_TRACE
			JavaVMOption jvm_options[5];
#else
			JavaVMOption jvm_options[3];
#endif
			int length_JAVACLASSPATH=0;
			int length_JAVALIBRARYPATH=0;

			HadAlreadyJavaVm = FALSE;

			memset(&vm_args, 0, sizeof(vm_args));

			SciJNI_GetDefaultJavaVMInitArgs(&vm_args);
#define CLASSPATH_COMMAND_FORMAT  "-Djava.class.path=%s%s%s%s%s%s%s"
			length_JAVACLASSPATH = (int) ( strlen(CLASSPATH_COMMAND_FORMAT)+
				strlen(SCI_PATH)+
				strlen(DEFAULT_SCILAB_CLASSPATH)+
				strlen(PATH_SEPARATOR)+
				strlen(USER_CLASSPATH)+
				strlen(PATH_SEPARATOR)+
				strlen(SCI_PATH)  +
				strlen(JVM_CLASSPATH));

			JAVACLASSPATH=(char*) MALLOC( sizeof(char)*	( length_JAVACLASSPATH +1) );
			sprintf(JAVACLASSPATH, CLASSPATH_COMMAND_FORMAT,SCI_PATH,DEFAULT_SCILAB_CLASSPATH,PATH_SEPARATOR,USER_CLASSPATH,PATH_SEPARATOR,SCI_PATH,JVM_CLASSPATH);

#define JAVA_LIBRARY_PATH_COMMAND_FORMAT "-Djava.library.path=%s%s/lib%s%s%s"
			/* @TODO this should be changed for Linux/Unix as we don't know where are the libs */
			length_JAVALIBRARYPATH = (int)( strlen(JAVA_LIBRARY_PATH_COMMAND_FORMAT)+
				strlen(SCI_PATH)+
				strlen(JRE_PATH)+
				strlen(PATH_SEPARATOR)+
				strlen(SCI_PATH)+
				strlen(DEFAULT_SCILAB_LIBRARYPATH) );

			JAVALIBRARYPATH=(char*)MALLOC(sizeof(char)* ( length_JAVALIBRARYPATH +1) );

			sprintf(JAVALIBRARYPATH,JAVA_LIBRARY_PATH_COMMAND_FORMAT,SCI_PATH,JRE_PATH,PATH_SEPARATOR,SCI_PATH,DEFAULT_SCILAB_LIBRARYPATH);
			/* JAVACLASSPATH & JAVALIBRARYPATH sont liberes à la fin de l'execution de la JVM */

#ifdef JNI_VERSION_1_6
			vm_args.version = JNI_VERSION_1_6;
#elif JNI_VERSION_1_4
			vm_args.version = JNI_VERSION_1_4;
#else
#ifdef _MSC_VER
			MessageBox(NULL,_("\nIncorrect version JNI (needs at least JDK 1.4).\n"),_("Error"),MB_ICONEXCLAMATION|MB_OK);
#else
			printf(_("\nIncorrect version JNI (needs at least JDK 1.4).\n"));
#endif
			exit(1);
#endif
			jvm_options[0].optionString = "-Djava.compiler=NONE"; /* disable JIT */
			jvm_options[1].optionString = JAVACLASSPATH;
			jvm_options[2].optionString = JAVALIBRARYPATH;
                        
#ifdef _JNI_DEBUG_TRACE
                        jvm_options[3].optionString = "-verbose:jni"; /* print JNI msgs */
                        jvm_options[4].optionString = "-Xcheck:jni"; /* print JNI errors */
#endif

			vm_args.options = jvm_options;
#ifdef _JNI_DEBUG_TRACE
			vm_args.nOptions = 5;
#else
			vm_args.nOptions = 3;
#endif
			vm_args.ignoreUnrecognized = TRUE;

			status = SciJNI_CreateJavaVM(&jvm_SCILAB, (void**) &env, &vm_args);

			if (status != JNI_OK)
			{
				printf(_("Error in the creation of the Java VM : %s\n"),getJniErrorFromStatusCode(status));
				FreeDynLibJVM();
				if (JAVACLASSPATH){FREE(JAVACLASSPATH);JAVACLASSPATH=NULL;}
				if (JAVALIBRARYPATH){FREE(JAVALIBRARYPATH);JAVALIBRARYPATH=NULL;}
				return FALSE;
			}
		}
	}

	res = (*jvm_SCILAB)->AttachCurrentThread(jvm_SCILAB, (void**) &env, (void*)NULL);

	if (res != 0)
	{
#ifdef _MSC_VER
		MessageBox(NULL,_("\nCall to AttachCurrentThread.\n"),_("Error"),MB_ICONEXCLAMATION|MB_OK);
#else
		printf(_("\nCall to AttachCurrentThread.\n"));
#endif
		FreeDynLibJVM();
		if (JAVACLASSPATH){FREE(JAVACLASSPATH);JAVACLASSPATH=NULL;}
		if (JAVALIBRARYPATH){FREE(JAVALIBRARYPATH);JAVALIBRARYPATH=NULL;}
		return FALSE;
	}
	else return TRUE;
}
/*--------------------------------------------------------------------------*/ 
BOOL finishJVM(void)
{
	BOOL bOK=FALSE;

	if (FreeDynLibJVM())
	{
		jvm_SCILAB = NULL;
		bOK=TRUE;
	}

	if (JAVACLASSPATH){FREE(JAVACLASSPATH);JAVACLASSPATH=NULL;}
	if (JAVALIBRARYPATH){FREE(JAVALIBRARYPATH);JAVALIBRARYPATH=NULL;}

	return bOK;
}
/*--------------------------------------------------------------------------*/ 
