/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "JVM_UNIX.h"
/*-----------------------------------------------------------------------------------*/ 
static void *hLibJVM = NULL;
/*-----------------------------------------------------------------------------------*/ 
jint MyJNI_CreateJavaVM(JavaVM **pvm, void **penv, void *args)
{
  jint (*createJavaVM)(JavaVM**, JNIEnv**, JavaVMInitArgs*); 
  createJavaVM = (jint (*)(JavaVM**, JNIEnv**, JavaVMInitArgs*))dlsym(hLibJVM, "JNI_CreateJavaVM"); 

	return createJavaVM(pvm,penv,args); 
}
/*-----------------------------------------------------------------------------------*/ 
jint MyJNI_GetCreatedJavaVMs(JavaVM **vmBuf, jsize BufLen, jsize *nVMs)
{
	jint (*getcreatedjavaVMs)(JavaVM **, jsize, jsize *); 
	getcreatedjavaVMs=(jint (*)(JavaVM **, jsize, jsize *))dlsym(hLibJVM, "JNI_GetCreatedJavaVMs" ); 
	
	return getcreatedjavaVMs(vmBuf,BufLen,nVMs);
}
/*-----------------------------------------------------------------------------------*/ 
int GetJVMDll(char *SCILAB_PATH)
{
	int bOK=FALSE;
	char *JVMDLLFULLNAME=NULL;
	
	JVMDLLFULLNAME=(char*)malloc( (strlen(SCILAB_PATH)+strlen(JRE_PATH)+strlen("/bin/client/libjava.so")+1)*sizeof(char));
	/* A voir pour H.P(.sl) et autres come Mac*/
	/* http://www.informatik.uni-frankfurt.de/doc/man/hpux/shl_load.3x.html */
	sprintf(JVMDLLFULLNAME,"%s%s%s",SCILAB_PATH,JRE_PATH,"/bin/client/libjava.so");

	hLibJVM = dlopen("libjava.so", RTLD_NOW | RTLD_GLOBAL); 
	
	if (JVMDLLFULLNAME){free(JVMDLLFULLNAME);JVMDLLFULLNAME=NULL;};

	if (hLibJVM) bOK=TRUE;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
int FreeJVMDll(void)
{
	int bOK=FALSE;
	
	if (!dlclose(hLibJVM))
	{
		bOK=TRUE;
	}
	
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
