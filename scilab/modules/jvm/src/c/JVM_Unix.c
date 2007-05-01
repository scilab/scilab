/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 - 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <dlfcn.h> 
#include <string.h>
#include <stdlib.h>
#include "JVM_commons.h"
#include "JVM_Unix.h"
#include "sci_mem_alloc.h"
/*-----------------------------------------------------------------------------------*/ 
static void *hLibJVM = NULL;
/*-----------------------------------------------------------------------------------*/ 
jint SciJNI_GetDefaultJavaVMInitArgs(void *args)
{
	jint (*GetDefaultJavaVMInitArgs)(void *args);
	GetDefaultJavaVMInitArgs = (jint (*)(void *args))dlsym(hLibJVM, "JNI_GetDefaultJavaVMInitArgs"); 
	
	return GetDefaultJavaVMInitArgs(args);
}
/*-----------------------------------------------------------------------------------*/ 
jint SciJNI_CreateJavaVM(JavaVM **pvm, void **penv, void *args)
{
  jint (*createJavaVM)(JavaVM**, JNIEnv**, JavaVMInitArgs*); 
  createJavaVM = (jint (*)(JavaVM**, JNIEnv**, JavaVMInitArgs*))dlsym(hLibJVM, "JNI_CreateJavaVM"); 

	return createJavaVM(pvm,(JNIEnv**)penv,args); 
}
/*-----------------------------------------------------------------------------------*/ 
jint SciJNI_GetCreatedJavaVMs(JavaVM **vmBuf, jsize BufLen, jsize *nVMs)
{
	jint (*getcreatedjavaVMs)(JavaVM **, jsize, jsize *); 
	getcreatedjavaVMs=(jint (*)(JavaVM **, jsize, jsize *))dlsym(hLibJVM, "JNI_GetCreatedJavaVMs" ); 
	
	return getcreatedjavaVMs(vmBuf,BufLen,nVMs);
}
/*-----------------------------------------------------------------------------------*/ 
BOOL LoadDynLibJVM(char *SCILAB_PATH)
{
	BOOL bOK=FALSE;
	char *JVMDLLFULLNAME=NULL;
	
	JVMDLLFULLNAME=(char*)MALLOC( (strlen(SCILAB_PATH)+strlen(JRE_PATH)+strlen("/bin/client/libjava")+strlen(SHARED_LIB_EXT)+1)*sizeof(char));
	/* A voir pour H.P(.sl) et autres comme Mac*/
	/* http://www.informatik.uni-frankfurt.de/doc/man/hpux/shl_load.3x.html */
	sprintf(JVMDLLFULLNAME,"%s%s%s%s",SCILAB_PATH,JRE_PATH,"/bin/client/libjava",SHARED_LIB_EXT);

	hLibJVM = dlopen(JVMDLLFULLNAME, RTLD_NOW | RTLD_GLOBAL); 
	
	if (hLibJVM == NULL)
	{
		if (JVMDLLFULLNAME){FREE(JVMDLLFULLNAME);JVMDLLFULLNAME=NULL;} 
			
		/* We try LD_LIBRARY_PATH Paths */
		JVMDLLFULLNAME=(char*)MALLOC( (strlen("libjava")+strlen(SHARED_LIB_EXT)+1)*sizeof(char));
		sprintf(JVMDLLFULLNAME,"%s%s","libjava",SHARED_LIB_EXT);
		hLibJVM = dlopen(JVMDLLFULLNAME, RTLD_NOW | RTLD_GLOBAL); 
	}

	if (hLibJVM) bOK=TRUE;
  
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL FreeDynLibJVM(void)
{
	BOOL bOK=FALSE;
	
	if (!dlclose(hLibJVM))
	{
		bOK=TRUE;
	}
	
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
