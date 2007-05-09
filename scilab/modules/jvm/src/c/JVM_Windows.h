/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2007 */
/* Allan CORNET*/
/*-----------------------------------------------------------------------------------*/ 
#ifndef __JVM_WINDOWS__
#define __JVM_WINDOWS__
/*-----------------------------------------------------------------------------------*/ 
#include <Windows.h>
#include <jni.h>
#include "machine.h"
/*-----------------------------------------------------------------------------------*/ 

/**
* Load JVM dynamic library
* @return TRUE or FALSE
*/
BOOL LoadDynLibJVM(char *SCILAB_PATH);

/**
* Free JVM dynamic library
* @return TRUE or FALSE
*/
BOOL FreeDynLibJVM(void);

/**
* wrapping JNI_CreateJavaVM
*/
jint SciJNI_CreateJavaVM(JavaVM **pvm, void **penv, void *args);

/**
* wrapping JNI_GetCreatedJavaVMs
*/
jint SciJNI_GetCreatedJavaVMs(JavaVM **vmBuf, jsize BufLen, jsize *nVMs);

/**
* wrapping JNI_GetDefaultJavaVMInitArgs
*/
jint SciJNI_GetDefaultJavaVMInitArgs(void *args);

/**
* Check if Scilab uses embedded JRE
* @return TRUE or FALSE
*/
BOOL withEmbeddedJRE(void);

/**
* search a Created JavaVM
* @return a JavaVM or NULL (not found)
*/
JavaVM *FindCreatedJavaVM(char *SCILAB_PATH);
/*-----------------------------------------------------------------------------------*/ 
#endif /* __JVM_WINDOWS__ */
