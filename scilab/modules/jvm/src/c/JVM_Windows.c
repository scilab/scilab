/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006-2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "JVM_commons.h"
#include "JVM_Windows.h"
#include "win_mem_alloc.h" /* MALLOC */
#include "../../fileio/includes/FileExist.h"
/*-----------------------------------------------------------------------------------*/ 
#define JVM_TYPE "client"
/* #define JVM_TYPE "server" */
/* Every form of Sun's Java runtime comes with both the "client VM" and the "server VM."
Unfortunately, Java applications and applets run by default in the client VM.
The Server VM is much faster than the Client VM,
but it has the downside of taking around 10% longer to start up, and it uses more memory.
*/
/*-----------------------------------------------------------------------------------*/ 
static HINSTANCE hLibJVM = NULL;
/*-----------------------------------------------------------------------------------*/ 
typedef jint (JNICALL *JNI_CreateJavaVMPROC) (JavaVM **pvm, void **penv, void *args);
typedef jint (JNICALL *JNI_GetCreatedJavaVMsPROC)(JavaVM **vmBuf, jsize BufLen, jsize *nVMs);
typedef jint (JNICALL *JNI_GetDefaultJavaVMInitArgsPROC)(void *args);
/*-----------------------------------------------------------------------------------*/ 
static JNI_GetDefaultJavaVMInitArgsPROC ptr_JNI_GetDefaultJavaVMInitArgs = NULL;
static JNI_CreateJavaVMPROC ptr_JNI_CreateJavaVM  = NULL;
static JNI_GetCreatedJavaVMsPROC ptr_JNI_GetCreatedJavaVMs = NULL;
/*-----------------------------------------------------------------------------------*/ 
static char *Search_Java_RuntimeLib_in_Windows_Registry(void);
static JavaVM *SearchCreatedJavaVMPath(void);
static JavaVM *SearchCreatedJavaVMEmbedded(char *SCILAB_PATH);
static JavaVM *SearchCreatedJavaVMRegistry(void);
static BOOL LoadFuntionsJVM(HINSTANCE hLibJVM);
/*-----------------------------------------------------------------------------------*/ 
static BOOL EMBEDDED_JRE=FALSE;
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
BOOL LoadDynLibJVM(char *SCILAB_PATH)
{
	/* 1] search in SCI/java/jre */
	/* 2] search in windows registry */
	/* 3] search in PATH */
	/* else ERROR Java not found */

	BOOL bOK=FALSE;
	char *JVMDLLFULLNAME=NULL;
	
	/* 1] search in scilab SCI/java/jre */
	JVMDLLFULLNAME=(char*)MALLOC( (strlen(SCILAB_PATH)+strlen(JRE_PATH)+strlen("/bin/")+strlen(JVM_TYPE)+strlen("/jvm")+strlen(SHARED_LIB_EXT)+1)*sizeof(char));
	sprintf(JVMDLLFULLNAME,"%s%s%s%s%s%s",SCILAB_PATH,JRE_PATH,"/bin/",JVM_TYPE,"/jvm",SHARED_LIB_EXT);

	hLibJVM = LoadLibrary(JVMDLLFULLNAME);

	if (!LoadFuntionsJVM(hLibJVM))
	{
		/* 2] search in windows registry */
		/* We try to find JRE on Windows registry*/
		if (JVMDLLFULLNAME){FREE(JVMDLLFULLNAME);JVMDLLFULLNAME=NULL;};
		JVMDLLFULLNAME=Search_Java_RuntimeLib_in_Windows_Registry();
		hLibJVM = LoadLibrary(JVMDLLFULLNAME);

		/* 3] search in PATH */
		if (!LoadFuntionsJVM(hLibJVM))
		{
			if (JVMDLLFULLNAME){FREE(JVMDLLFULLNAME);JVMDLLFULLNAME=NULL;};
			JVMDLLFULLNAME=(char*)MALLOC( (strlen("jvm")+strlen(SHARED_LIB_EXT)+1)*sizeof(char));
			sprintf(JVMDLLFULLNAME,"%s%s",SCILAB_PATH,JRE_PATH,"jvm",SHARED_LIB_EXT);
			hLibJVM = LoadLibrary(JVMDLLFULLNAME);
		}
	}
	else EMBEDDED_JRE=TRUE;

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
		ptr_JNI_GetDefaultJavaVMInitArgs = NULL; 
		ptr_JNI_CreateJavaVM = NULL; 
		ptr_JNI_GetCreatedJavaVMs = NULL; 
		bOK=TRUE;
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
char *Search_Java_RuntimeLib_in_Windows_Registry(void)
{
	#define JRE_HKEY "Software\\JavaSoft\\Java Runtime Environment"
	char *RuntimeLib=NULL;
	char value[MAX_PATH];
	char newKey[MAX_PATH];

	HKEY  regKey;
	DWORD size  = MAX_PATH;
	int   result;

#ifdef _WIN64 /* if Win64 search only 64 bits JRE version */
	if ((result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, JRE_HKEY, 0, KEY_READ | KEY_WOW64_64KEY, &regKey)) != ERROR_SUCCESS)
#else
	if ((result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, JRE_HKEY, 0, KEY_READ | KEY_WOW64_32KEY, &regKey)) != ERROR_SUCCESS)
#endif
	{
		return NULL;
	}

	if ((result = RegQueryValueEx(regKey, "CurrentVersion", NULL, NULL, (LPBYTE)value, &size)) != ERROR_SUCCESS)
	{
		RegCloseKey(regKey);
		return NULL;
	}

	RegCloseKey(regKey);
	value[size] = '\0';
	size = MAX_PATH;

	strcpy(newKey, JRE_HKEY);
	strcat(newKey, "\\");
	strcat(newKey, value);

#ifdef _WIN64
	if ((result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, newKey, 0, KEY_READ | KEY_WOW64_64KEY, &regKey)) != ERROR_SUCCESS)
#else
	if ((result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, newKey, 0, KEY_READ | KEY_WOW64_32KEY, &regKey)) != ERROR_SUCCESS)
#endif
	{
		return NULL;
	}

	if ((result = RegQueryValueEx(regKey, "RuntimeLib", NULL, NULL,(LPBYTE)value, &size)) != ERROR_SUCCESS)
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
BOOL withEmbeddedJRE(void)
{
	return EMBEDDED_JRE;
}
/*-----------------------------------------------------------------------------------*/ 
static BOOL LoadFuntionsJVM(HINSTANCE hLibJVM_)
{
	BOOL bOK=FALSE;
	if (hLibJVM_)
	{
		ptr_JNI_GetDefaultJavaVMInitArgs = (JNI_GetDefaultJavaVMInitArgsPROC) GetProcAddress(hLibJVM_, "JNI_GetDefaultJavaVMInitArgs" ); 
		ptr_JNI_CreateJavaVM = (JNI_CreateJavaVMPROC) GetProcAddress(hLibJVM_, "JNI_CreateJavaVM" ); 
		ptr_JNI_GetCreatedJavaVMs = (JNI_GetCreatedJavaVMsPROC) GetProcAddress(hLibJVM_, "JNI_GetCreatedJavaVMs" ); 

		if (ptr_JNI_GetDefaultJavaVMInitArgs && ptr_JNI_CreateJavaVM && ptr_JNI_GetCreatedJavaVMs) bOK=TRUE;
	}
	return bOK;
}
/*----------------------------------------------------------------------------------*/ 
static JavaVM *SearchCreatedJavaVMEmbedded(char *SCILAB_PATH)
{
	JavaVM *jvm = NULL;
	jsize jvm_count = 0;
	jint res=0;
	char *JVMDLLFULLNAME=NULL;

	/* search in SCI/java/jre */
	JVMDLLFULLNAME=(char*)MALLOC( (strlen(SCILAB_PATH)+strlen(JRE_PATH)+strlen("/bin/")+strlen(JVM_TYPE)+strlen("/jvm")+strlen(SHARED_LIB_EXT)+1)*sizeof(char));
	sprintf(JVMDLLFULLNAME,"%s%s%s%s%s%s",SCILAB_PATH,JRE_PATH,"/bin/",JVM_TYPE,"/jvm",SHARED_LIB_EXT);

	FreeDynLibJVM();
	hLibJVM = LoadLibrary(JVMDLLFULLNAME);
	if (JVMDLLFULLNAME){FREE(JVMDLLFULLNAME);JVMDLLFULLNAME=NULL;}

	if (LoadFuntionsJVM(hLibJVM))
	{
		res = SciJNI_GetCreatedJavaVMs (&jvm, 1, &jvm_count);

		if ( jvm_count == 1 ) return jvm;
		else jvm = NULL;
	}

	return jvm;
}
/*----------------------------------------------------------------------------------*/ 
JavaVM *SearchCreatedJavaVMRegistry(void)
{
	JavaVM *jvm = NULL;
	jsize jvm_count = 0;
	jint res=0;
	char *JVMDLLFULLNAME=NULL;

	JVMDLLFULLNAME=Search_Java_RuntimeLib_in_Windows_Registry();
	FreeDynLibJVM();
	hLibJVM = LoadLibrary(JVMDLLFULLNAME);
	if (JVMDLLFULLNAME){FREE(JVMDLLFULLNAME);JVMDLLFULLNAME=NULL;};

	if (LoadFuntionsJVM(hLibJVM))
	{
		res = SciJNI_GetCreatedJavaVMs (&jvm, 1, &jvm_count);
		
		if ( jvm_count == 1 ) return jvm;
		else jvm = NULL;
	}
	return jvm;
}
/*----------------------------------------------------------------------------------*/ 
static JavaVM *SearchCreatedJavaVMPath(void)
{
	JavaVM *jvm = NULL;
	jsize jvm_count = 0;
	jint res=0;
	char *JVMDLLFULLNAME=NULL;
	
	FreeDynLibJVM();

	JVMDLLFULLNAME=(char*)MALLOC( (strlen("jvm")+strlen(SHARED_LIB_EXT)+1)*sizeof(char));
	sprintf(JVMDLLFULLNAME,"%s%s","jvm",SHARED_LIB_EXT);

	hLibJVM = LoadLibrary(JVMDLLFULLNAME);
	if (JVMDLLFULLNAME){FREE(JVMDLLFULLNAME);JVMDLLFULLNAME=NULL;};

	if (LoadFuntionsJVM(hLibJVM))
	{
		res = SciJNI_GetCreatedJavaVMs (&jvm, 1, &jvm_count);
		if ( jvm_count == 1 ) return jvm;
		else jvm = NULL;
	}
	return jvm;
}
/*----------------------------------------------------------------------------------*/ 
JavaVM *FindCreatedJavaVM(char *SCILAB_PATH)
{
	JavaVM *jvm = NULL;

	jvm = SearchCreatedJavaVMEmbedded(SCILAB_PATH);
	if (jvm) return jvm;
	else
	{
		jvm = SearchCreatedJavaVMRegistry();
		if (jvm) return jvm;
		else
		{
			jvm = SearchCreatedJavaVMPath();
			if (jvm) return jvm;
		}
	}
	return NULL;
}
/*-----------------------------------------------------------------------------------*/ 
