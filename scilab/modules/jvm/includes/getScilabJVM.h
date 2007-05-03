/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GETSCILABJVM_H__
#define __GETSCILABJVM_H__

#include "jni.h" /* JavaVM */
#include "machine.h" /* BOOL */

/**
* returns Scilab JavaVM
* @return JavaVM
*/
JavaVM *getScilabJVM(void);

#endif /* __GETSCILABJVM_H__ */
/*-----------------------------------------------------------------------------------*/ 
