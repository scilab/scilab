/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __TERMINATEJVM_H__
#define __TERMINATEJVM_H__

#include "machine.h"
#include "export_import_library_JVM.h"

/**
* Terminate JVM 
* @return TRUE or FALSE
*/

IMPORT_EXPORT_LIBJVM_DLL BOOL TerminateJVM(void);

#endif /* __TERMINATEJVM_H__ */
/*-----------------------------------------------------------------------------------*/ 
