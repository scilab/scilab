/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdio.h>
#include "JVM_commons.h"
#include "dynamiclibrary.h"
#include "localization.h"
/*-----------------------------------------------------------------------------------*/ 
static DynLibHandle hLibJVM = NULL;
/*-----------------------------------------------------------------------------------*/ 
typedef jint (JNICALL *JNI_CreateJavaVMPROC) (JavaVM **pvm, void **penv, void *args);
typedef jint (JNICALL *JNI_GetCreatedJavaVMsPROC)(JavaVM **vmBuf, jsize BufLen, jsize *nVMs);
typedef jint (JNICALL *JNI_GetDefaultJavaVMInitArgsPROC)(void *args);
/*-----------------------------------------------------------------------------------*/ 
static JNI_GetDefaultJavaVMInitArgsPROC ptr_JNI_GetDefaultJavaVMInitArgs = NULL;
static JNI_CreateJavaVMPROC ptr_JNI_CreateJavaVM  = NULL;
static JNI_GetCreatedJavaVMsPROC ptr_JNI_GetCreatedJavaVMs = NULL;
/*-----------------------------------------------------------------------------------*/ 
jint SciJNI_GetDefaultJavaVMInitArgs(void *args)
{
	jint res = JNI_ERR;
	if (ptr_JNI_GetDefaultJavaVMInitArgs) res = (ptr_JNI_GetDefaultJavaVMInitArgs)(args);
	return res;
}
/*-----------------------------------------------------------------------------------*/ 
jint SciJNI_CreateJavaVM(JavaVM **pvm, void **penv, void *args)
{
	jint res = JNI_ERR;
	if (ptr_JNI_CreateJavaVM) res=(ptr_JNI_CreateJavaVM)(pvm,penv,args);
	return res;
}
/*-----------------------------------------------------------------------------------*/ 
jint SciJNI_GetCreatedJavaVMs(JavaVM **vmBuf, jsize BufLen, jsize *nVMs)
{
	jint res = JNI_ERR;
	if (ptr_JNI_GetCreatedJavaVMs) res = (ptr_JNI_GetCreatedJavaVMs)(vmBuf,BufLen,nVMs);
	return res;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL FreeDynLibJVM(void)
{
	if (hLibJVM)
		{
			if (FreeDynLibrary(hLibJVM))
				{
					ptr_JNI_GetDefaultJavaVMInitArgs = NULL; 
					ptr_JNI_CreateJavaVM = NULL; 
					ptr_JNI_GetCreatedJavaVMs = NULL; 
					return TRUE;
				}
		}
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL LoadFuntionsJVM(char *filedynlib)
{

	hLibJVM = LoadDynLibrary(filedynlib); 
	
	if (hLibJVM)
	{
		ptr_JNI_GetDefaultJavaVMInitArgs = (JNI_GetDefaultJavaVMInitArgsPROC) GetDynLibFuncPtr(hLibJVM, "JNI_GetDefaultJavaVMInitArgs" ); 
		ptr_JNI_CreateJavaVM = (JNI_CreateJavaVMPROC) GetDynLibFuncPtr(hLibJVM, "JNI_CreateJavaVM" ); 
		ptr_JNI_GetCreatedJavaVMs = (JNI_GetCreatedJavaVMsPROC) GetDynLibFuncPtr(hLibJVM, "JNI_GetCreatedJavaVMs" ); 

		if (ptr_JNI_GetDefaultJavaVMInitArgs && ptr_JNI_CreateJavaVM && ptr_JNI_GetCreatedJavaVMs) return TRUE;
	}
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/ 
char *getJniErrorFromStatusCode(long status){
	switch (status){
		case JNI_ERR:
			return _("Unknown JNI error");
			break;
		case JNI_EDETACHED:
			return _("Thread detached from the VM");
			break;
		case JNI_EVERSION:
			return _("JNI version error");
			break;
		case JNI_ENOMEM:
			return _("JNI: not enough memory");
			break;
		case JNI_EEXIST:
			return _("VM already created");
			break;
		case JNI_EINVAL:
			return _("JNI: invalid arguments");
			break;
		default:
			return _("Undefined error code in the JNI. Weird problem");
			break;
	}
}
/*-----------------------------------------------------------------------------------*/ 
