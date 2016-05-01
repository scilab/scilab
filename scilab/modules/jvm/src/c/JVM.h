
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
