/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006-2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "JVM_commons.h"
#include "JVM_Windows.h"
#include "win_mem_alloc.h" /* MALLOC */
#include "../../fileio/includes/FileExist.h"
/*-----------------------------------------------------------------------------------*/ 
static HINSTANCE hLibJVM = NULL;
/*-----------------------------------------------------------------------------------*/ 
typedef jint (JNICALL * JNI_CreateJavaVMPROC) (JavaVM **, void **, void *);
typedef jint (JNICALL * JNI_GetCreatedJavaVMsPROC)(JavaVM **, jsize, jsize *);
typedef jint (JNICALL * JNI_GetDefaultJavaVMInitArgsPROC)(void *args);
/*-----------------------------------------------------------------------------------*/ 
static char *Search_Java_RuntimeLib_in_Windows_Registry(void);
/*-----------------------------------------------------------------------------------*/ 
jint SciJNI_GetDefaultJavaVMInitArgs(void *args)
{
	JNI_GetDefaultJavaVMInitArgsPROC ptr_JNI_GetDefaultJavaVMInitArgs;
	ptr_JNI_GetDefaultJavaVMInitArgs = (JNI_GetDefaultJavaVMInitArgsPROC) GetProcAddress(hLibJVM, "JNI_GetDefaultJavaVMInitArgs" ); 
	return (*ptr_JNI_GetDefaultJavaVMInitArgs)(args);
}
/*-----------------------------------------------------------------------------------*/ 
jint SciJNI_CreateJavaVM(JavaVM **pvm, void **penv, void *args)
{
	JNI_CreateJavaVMPROC ptr_JNI_CreateJavaVM;
	ptr_JNI_CreateJavaVM = (JNI_CreateJavaVMPROC) GetProcAddress(hLibJVM, "JNI_CreateJavaVM" ); 
	return (*ptr_JNI_CreateJavaVM)(pvm,penv,args);
}
/*-----------------------------------------------------------------------------------*/ 
jint SciJNI_GetCreatedJavaVMs(JavaVM **vmBuf, jsize BufLen, jsize *nVMs)
{
	JNI_GetCreatedJavaVMsPROC ptr_JNI_GetCreatedJavaVMs;
	ptr_JNI_GetCreatedJavaVMs = (JNI_GetCreatedJavaVMsPROC) GetProcAddress(hLibJVM, "JNI_GetCreatedJavaVMs" ); 
	return (*ptr_JNI_GetCreatedJavaVMs)(vmBuf,BufLen,nVMs);
}
/*-----------------------------------------------------------------------------------*/ 
BOOL LoadDynLibJVM(char *SCILAB_PATH)
{
	BOOL bOK=FALSE;
	char *JVMDLLFULLNAME=NULL;
	
	JVMDLLFULLNAME=(char*)MALLOC( (strlen(SCILAB_PATH)+strlen(JRE_PATH)+strlen("/bin/client/jvm")+strlen(SHARED_LIB_EXT)+1)*sizeof(char));
	
	sprintf(JVMDLLFULLNAME,"%s%s%s%s",SCILAB_PATH,JRE_PATH,"/bin/client/jvm",SHARED_LIB_EXT);

	hLibJVM = LoadLibrary(JVMDLLFULLNAME);

	if (hLibJVM == NULL)
	{
		/* We try to find JRE on Windows registry*/
		if (JVMDLLFULLNAME){FREE(JVMDLLFULLNAME);JVMDLLFULLNAME=NULL;};
		JVMDLLFULLNAME=Search_Java_RuntimeLib_in_Windows_Registry();
		hLibJVM = LoadLibrary(JVMDLLFULLNAME);
	}

	if (JVMDLLFULLNAME){FREE(JVMDLLFULLNAME);JVMDLLFULLNAME=NULL;};
	if (hLibJVM) bOK=TRUE;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL FreeDynLibJVM(void)
{
	BOOL bOK=FALSE;
	
	if (FreeLibrary(hLibJVM))
	{
		bOK=TRUE;
	}
	
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
char *Search_Java_RuntimeLib_in_Windows_Registry(void)
{
	char *RuntimeLib=NULL;
	char value[MAX_PATH];
	char newKey[MAX_PATH];

	HKEY  regKey;
	DWORD size  = MAX_PATH;
	int   result;

	if ((result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\JavaSoft\\Java Runtime Environment", 0, KEY_READ, &regKey)) != ERROR_SUCCESS)
	{
		return NULL;
	}

	if ((result = RegQueryValueEx(regKey, "CurrentVersion", NULL, NULL, value, &size)) != ERROR_SUCCESS)
	{
		RegCloseKey(regKey);
		return NULL;
	}

	RegCloseKey(regKey);
	value[size] = '\0';
	size = MAX_PATH;

	strcpy(newKey, "Software\\JavaSoft\\Java Runtime Environment\\");
	strcat(newKey, value);

	if ((result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, newKey, 0, KEY_READ, &regKey)) != ERROR_SUCCESS)
	{
		return NULL;
	}

	if ((result = RegQueryValueEx(regKey, "RuntimeLib", NULL, NULL, value, &size)) != ERROR_SUCCESS)
	{
		RegCloseKey(regKey);
		return NULL;
	}

	RegCloseKey(regKey);

	/* check file Exist */
	if (FileExist(value))
	{
		RuntimeLib=(char*)MALLOC(sizeof(char)*(strlen(value)+1));
		strcpy(RuntimeLib,value);
	}

	return RuntimeLib;
}
/*-----------------------------------------------------------------------------------*/ 
