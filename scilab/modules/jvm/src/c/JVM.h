
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
#ifndef __JVM_H__
#define __JVM_H__

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "BOOL.h" /* BOOL */
/**
* start JVM used in InitializeJVM function
* @param Scilab path
* @return TRUE or FALSE
*/
BOOL startJVM(char *SCI_PATH);

/**
* finish JVM in TerminateJVM function
* @return TRUE or FALSE
*/
BOOL finishJVM(void);


/**
 * Returns the current JNIEnv of Scilab
 * @return The JNI environnement of Scilab
 */
JNIEnv *getScilabJNIEnv(void);

#ifdef __cplusplus
}
#endif

#endif /* __JVM_H__ */
/*--------------------------------------------------------------------------*/ 
