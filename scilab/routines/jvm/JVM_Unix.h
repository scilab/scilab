/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* Allan CORNET*/
/*-----------------------------------------------------------------------------------*/ 
#ifndef __JVM_UNIX__
#define __JVM_UNIX__
#include <dlfcn.h> 
#include "jni.h"
#include "jvm_common.h"
/*-----------------------------------------------------------------------------------*/ 
int GetJVMDll(char *SCILAB_PATH);
int FreeJVMDll(void);
jint MyJNI_CreateJavaVM(JavaVM **pvm, void **penv, void *args);
jint MyJNI_GetCreatedJavaVMs(JavaVM **vmBuf, jsize BufLen, jsize *nVMs);
/*-----------------------------------------------------------------------------------*/ 
#endif /* __JVM_UNIX__ */
