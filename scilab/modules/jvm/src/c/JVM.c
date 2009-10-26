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
#include <stdio.h>
#include "JVM.h"
#include "JVM_functions.h"
#include "MALLOC.h"
#include "getScilabJavaVM.h"
#include "getScilabJNIEnv.h"
#include "fromjava.h"
#include "localization.h"
#include "getJvmOptions.h"
/*--------------------------------------------------------------------------*/ 
static JavaVM *jvm_SCILAB=NULL;
/*--------------------------------------------------------------------------*/ 
static BOOL HadAlreadyJavaVm = FALSE;
/*--------------------------------------------------------------------------*/ 
static JavaVMOption *jvm_options = NULL;
static int nOptions = 0;
/*--------------------------------------------------------------------------*/ 
static void freeJavaVMOption(void)
{
	if (jvm_options)
	{
		int j = 0;
		for (j = 0 ; j < nOptions ; j++)
		{
			if (jvm_options[j].optionString)
			{
				FREE(jvm_options[j].optionString);
				jvm_options[j].optionString = NULL;
			}
		}
		nOptions = 0;
	}
}
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
		if(res == JNI_ERR) 
		{
		#ifdef _MSC_VER
			MessageBox(NULL,gettext("\nError: Cannot return Scilab Java environment (JNIEnv_SCILAB).\n"),gettext("Error"),MB_ICONEXCLAMATION|MB_OK);
		#else
			fprintf(stderr,_("\nError: Cannot return Scilab Java environment (JNIEnv_SCILAB).\n"));
		#endif
			return NULL;
		}

		if(res == JNI_EDETACHED)
		{
			(*jvm_SCILAB)->AttachCurrentThread( jvm_SCILAB, (void **) &JNIEnv_SCILAB , NULL ) ;
		}
	}
	else
	{
#ifdef _MSC_VER
		MessageBox(NULL,gettext("\nError: Cannot return Scilab Java environment (jvm_SCILAB): check if the JVM has been loaded by Scilab before calling this function.\n"),gettext("Error"),MB_ICONEXCLAMATION|MB_OK);
#else
		fprintf(stderr,_("\nError: Cannot return Scilab Java environment (jvm_SCILAB): check if the JVM has been loaded by Scilab before calling this function.\n"));
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
			fprintf(stderr,_("\nWeird error. Calling from Java but haven't been able to find the already existing JVM.\n"));
			FreeDynLibJVM();
			return FALSE;
		}
	}
	else
	{
		if (! LoadDynLibJVM(SCI_PATH) ) 
		{
			fprintf(stderr,_("\nCould not load JVM dynamic library (libjava).\n"));
			return FALSE;
		}
		else
		{
			/**
			* http://java.sun.com/javase/6/docs/technotes/guides/jni/spec/invocation.html#wp15956
			*/
			#define JVM_OPTIONS_FILENAME_FORMAT "%s/etc/jvm_options.xml"
			char *jvm_options_filename = NULL;

			long status = 0;
			JavaVMInitArgs vm_args;

			jvm_options_filename = (char*)MALLOC(sizeof(char)*((int)strlen(JVM_OPTIONS_FILENAME_FORMAT)+ (int)strlen(SCI_PATH) + 1));
			sprintf(jvm_options_filename,JVM_OPTIONS_FILENAME_FORMAT,SCI_PATH);

			jvm_options = getJvmOptions(SCI_PATH,jvm_options_filename,&nOptions);

			if ( (jvm_options == NULL) || (nOptions == 0) )
			{
				#ifdef _MSC_VER
				{
					#define FORMAT_MSG "\nImpossible to read %s.\n"
					char *msg = (char*)MALLOC(sizeof(char)*( (int)strlen(_(FORMAT_MSG)) +
								(int)strlen(jvm_options_filename) + 1));
					if (msg)
					{
						sprintf(msg,_(FORMAT_MSG),jvm_options_filename);
						MessageBox(NULL,msg,gettext("Error"),MB_ICONEXCLAMATION|MB_OK);
						FREE(msg);
						msg = NULL;
					}
				}
				#else
				fprintf(stderr,_("\nImpossible to read %s.\n"),jvm_options_filename);
				#endif
				if (jvm_options_filename)
				{
					FREE(jvm_options_filename);
					jvm_options_filename = NULL;
				}
				exit(1);
			}
			else
			{
				if (jvm_options_filename)
				{
					FREE(jvm_options_filename);
					jvm_options_filename = NULL;
				}

				HadAlreadyJavaVm = FALSE;

				memset(&vm_args, 0, sizeof(vm_args));
				SciJNI_GetDefaultJavaVMInitArgs(&vm_args);

				#ifdef JNI_VERSION_1_6
				vm_args.version = JNI_VERSION_1_6;
				#elif JNI_VERSION_1_4
				vm_args.version = JNI_VERSION_1_4;
				#else
				#ifdef _MSC_VER
				MessageBox(NULL,gettext("\nIncorrect version JNI (needs at least JDK 1.4).\n"),gettext("Error"),MB_ICONEXCLAMATION|MB_OK);
				#else
				fprintf(stderr,gettext("\nIncorrect version JNI (needs at least JDK 1.4).\n"));
				#endif
				exit(1);
				#endif

				vm_args.options = jvm_options;
				vm_args.nOptions = nOptions;
				vm_args.ignoreUnrecognized = FALSE;
				status = SciJNI_CreateJavaVM(&jvm_SCILAB, (JNIEnv**) &env, &vm_args);

				if (status != JNI_OK)
				{
					fprintf(stderr,_("Error in the creation of the Java VM: %s\n"),getJniErrorFromStatusCode(status));
					FreeDynLibJVM();
					return FALSE;
				}
			}
		}
	}

	res = (*jvm_SCILAB)->AttachCurrentThread(jvm_SCILAB, (void**) &env, (void*)NULL);

	if (res != 0)
	{
#ifdef _MSC_VER
		MessageBox(NULL,gettext("\nJVM error in AttachCurrentThread: Could not attach to the current thread.\n"),gettext("Error"),MB_ICONEXCLAMATION|MB_OK);
#else
		fprintf(stderr,gettext("\nJVM error in AttachCurrentThread: Could not attach to the current thread.\n"));
#endif
		FreeDynLibJVM();
		freeJavaVMOption();
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
	freeJavaVMOption();
	return bOK;
}
/*--------------------------------------------------------------------------*/ 
