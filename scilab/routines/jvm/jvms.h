/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* Allan CORNET*/
/*-----------------------------------------------------------------------------------*/ 
#ifndef __JVMS__
#define __JVMS__
/*-----------------------------------------------------------------------------------*/ 
#include "jni.h"
#include "jvm_common.h"
#ifdef _MSC_VER
  #include <windows.h>
#endif
#include <string.h>
#include "../machine.h"
/*-----------------------------------------------------------------------------------*/ 
#ifndef NULL
  #define NULL 0
#endif
#ifndef TRUE 
#define TRUE  1
#endif 
#ifndef FALSE
#define FALSE 0
#endif
/*-----------------------------------------------------------------------------------*/ 
IMPORT_EXPORT_LIBJVM_DLL int StartJVMs(char *SCILAB_PATH);
IMPORT_EXPORT_LIBJVM_DLL int TerminateJVMs(void);
/*-----------------------------------------------------------------------------------*/ 
#endif /* __JVMS__ */
/*-----------------------------------------------------------------------------------*/ 

