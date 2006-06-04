/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "JVM_Windows.h"
/*-----------------------------------------------------------------------------------*/ 
static HINSTANCE hLibJVM = NULL;
/*-----------------------------------------------------------------------------------*/ 
typedef  jint (JNICALL * JNI_CreateJavaVMPROC) (JavaVM **, void **, void *);
typedef  jint (JNICALL * JNI_GetCreatedJavaVMsPROC)(JavaVM **, jsize, jsize *);
/*-----------------------------------------------------------------------------------*/ 
jint MyJNI_CreateJavaVM(JavaVM **pvm, void **penv, void *args)
{
	JNI_CreateJavaVMPROC ptr_JNI_CreateJavaVM;
  ptr_JNI_CreateJavaVM = (JNI_CreateJavaVMPROC) GetProcAddress(hLibJVM, "JNI_CreateJavaVM" ); 
	return (*ptr_JNI_CreateJavaVM)(pvm,penv,args);
}

/*-----------------------------------------------------------------------------------*/ 
jint MyJNI_GetCreatedJavaVMs(JavaVM **vmBuf, jsize BufLen, jsize *nVMs)
{
	JNI_GetCreatedJavaVMsPROC ptr_JNI_GetCreatedJavaVMs;
  ptr_JNI_GetCreatedJavaVMs = (JNI_GetCreatedJavaVMsPROC) GetProcAddress(hLibJVM, "JNI_GetCreatedJavaVMs" ); 
	return (*ptr_JNI_GetCreatedJavaVMs)(vmBuf,BufLen,nVMs);
}
/*-----------------------------------------------------------------------------------*/ 
int GetJVMDll(char *SCILAB_PATH)
{
	int bOK=FALSE;
	char *JVMDLLFULLNAME=NULL;
	
	JVMDLLFULLNAME=(char*)malloc( (strlen(SCILAB_PATH)+strlen(JRE_PATH)+strlen("/bin/client/jvm.dll")+1)*sizeof(char));
	sprintf(JVMDLLFULLNAME,"%s%s%s",SCILAB_PATH,JRE_PATH,"/bin/client/jvm.dll");

	hLibJVM = LoadLibrary(JVMDLLFULLNAME);
	if (JVMDLLFULLNAME){free(JVMDLLFULLNAME);JVMDLLFULLNAME=NULL;};

	if (hLibJVM) bOK=TRUE;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
int FreeJVMDll(void)
{
	int bOK=FALSE;
	
	if (FreeLibrary(hLibJVM))
	{
		bOK=TRUE;
	}
	
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
