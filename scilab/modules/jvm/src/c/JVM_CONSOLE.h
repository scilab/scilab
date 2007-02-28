/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* Allan CORNET*/
/*-----------------------------------------------------------------------------------*/ 
#ifndef __JVM_CONSOLE__
#define __JVM_CONSOLE__
/*-----------------------------------------------------------------------------------*/ 
#include "jni.h"
#include "jvm_common.h"
#ifdef _MSC_VER
  #include <windows.h>
#endif

#include <string.h>
/*-----------------------------------------------------------------------------------*/ 
int StartJVM_CONSOLE(char *SCILAB_PATH);
int TerminateJVM_CONSOLE(void);
IMPORT_EXPORT_LIBJVM_DLL JavaVM *Get_jvm_CONSOLE(void);
IMPORT_EXPORT_LIBJVM_DLL jobject Get_SciGUIConsole_Object(void);
IMPORT_EXPORT_LIBJVM_DLL int PutString(char *Str);
/*-----------------------------------------------------------------------------------*/ 
#endif /* __JVM_CONSOLE__ */
/*-----------------------------------------------------------------------------------*/ 
