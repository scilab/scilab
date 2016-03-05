
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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

/**
* Returns if jvm symbols (JNI_GetCreatedJavaVMs, JNI_CreateJavaVM, ...) are already loaded
* @return TRUE or FALSE
*/
BOOL hasJvmSymbolsLoaded(void);

#endif /* __JVM_FUNCTIONS_H__ */
/*--------------------------------------------------------------------------*/

