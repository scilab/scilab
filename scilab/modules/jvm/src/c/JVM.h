/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __JVM_H__
#define __JVM_H__

#include "machine.h" /* BOOL */

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

#endif /* __JVM_H__ */
/*-----------------------------------------------------------------------------------*/ 
