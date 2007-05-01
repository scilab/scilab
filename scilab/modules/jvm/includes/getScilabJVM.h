/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GETSCILABJVM_H__
#define __GETSCILABJVM_H__

#include "jni.h" /* JavaVM */
#include "machine.h" /* BOOL */
#include "export_import_library_JVM.h" /* IMPORT_EXPORT_LIBJVM_DLL */

/**
* returns Scilab JavaVM
* @return JavaVM
*/
IMPORT_EXPORT_LIBJVM_DLL JavaVM *getScilabJVM(void);

#endif /* __GETSCILABJVM_H__ */
/*-----------------------------------------------------------------------------------*/ 
