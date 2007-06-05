/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __JVM_H__
#define __JVM_H__

#include "machine.h" /* BOOL */
#include <jni.h>
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


#endif /* __JVM_H__ */
/*-----------------------------------------------------------------------------------*/ 
