/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2007 */
/* Allan CORNET*/
/*-----------------------------------------------------------------------------------*/ 
#ifndef __JVM_FUNCTIONS_H__
#define __JVM_FUNCTIONS_H__

#include <jni.h> /* JVM functions */
#include "machine.h" /* BOOL */

/**
* Load JVM dynamic library
* @return TRUE or FALSE
*/
BOOL LoadDynLibJVM(char *SCILAB_PATH);

/**
* Free JVM dynamic library
* @return <ReturnValue> TRUE or FALSE
*/
BOOL FreeDynLibJVM(void);

/**
* wrapping JNI_CreateJavaVM
* @param pvm   
* @param penv  
* @param args  
* @return <ReturnValue>
*/
jint SciJNI_CreateJavaVM(JavaVM **pvm, void **penv, void *args);

/**
* wrapping JNI_GetCreatedJavaVMs
* @param vmBuf 
* @param BufLen    
* @param nVMs  
* @return <ReturnValue>
*/
jint SciJNI_GetCreatedJavaVMs(JavaVM **vmBuf, jsize BufLen, jsize *nVMs);

/**
* wrapping JNI_GetDefaultJavaVMInitArgs
* @param args  
* @return <ReturnValue>
*/
jint SciJNI_GetDefaultJavaVMInitArgs(void *args);

/**
* Check if Scilab uses embedded JRE
* @return <ReturnValue> TRUE or FALSE
*/
BOOL withEmbeddedJRE(void);

/**
* search a Created JavaVM
* @return a JavaVM or NULL (not found)
*/
JavaVM *FindCreatedJavaVM(char *SCILAB_PATH);

/**
* Load Functions for JVM
* @return TRUE or FALSE
*/
BOOL LoadFuntionsJVM(char *filedynlib);

#endif /* __JVM_FUNCTIONS_H__ */
/*-----------------------------------------------------------------------------------*/ 

