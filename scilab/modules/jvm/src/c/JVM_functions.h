
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
#ifndef __JVM_FUNCTIONS_H__
#define __JVM_FUNCTIONS_H__

#include <jni.h> /* JVM functions */
#include "BOOL.h" /* BOOL */

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
jint SciJNI_CreateJavaVM(JavaVM **pvm, JNIEnv **penv, JavaVMInitArgs *args);

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
jint SciJNI_GetDefaultJavaVMInitArgs(JavaVMInitArgs *args);

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
BOOL LoadFunctionsJVM(char *filedynlib);


/**
 * Returns the description of the error message from the JNI return status
 *
 * @param status the JNI status
 * @return The message describing the problem
 */
char *getJniErrorFromStatusCode(long status);

#endif /* __JVM_FUNCTIONS_H__ */
/*--------------------------------------------------------------------------*/ 

