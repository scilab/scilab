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
#define JVM_TYPE "client"
/* #define JVM_TYPE "server" */
/* Every form of Sun's Java runtime comes with both the "client VM" and the "server VM."
Unfortunately, Java applications and applets run by default in the client VM.
The Server VM is much faster than the Client VM,
but it has the downside of taking around 10% longer to start up, and it uses more memory.
*/
/*-----------------------------------------------------------------------------------*/ 
static void *hLibJVM = NULL;
/*-----------------------------------------------------------------------------------*/ 
typedef jint (JNICALL *JNI_CreateJavaVMPROC) (JavaVM **pvm, void **penv, void *args);
typedef jint (JNICALL *JNI_GetCreatedJavaVMsPROC)(JavaVM **vmBuf, jsize BufLen, jsize *nVMs);
typedef jint (JNICALL *JNI_GetDefaultJavaVMInitArgsPROC)(void *args);
/*-----------------------------------------------------------------------------------*/ 
static JNI_GetDefaultJavaVMInitArgsPROC ptr_JNI_GetDefaultJavaVMInitArgs = NULL;
static JNI_CreateJavaVMPROC ptr_JNI_CreateJavaVM  = NULL;
static JNI_GetCreatedJavaVMsPROC ptr_JNI_GetCreatedJavaVMs = NULL;
/*-----------------------------------------------------------------------------------*/ 
static JavaVM *SearchCreatedJavaVMEmbedded(char *SCILAB_PATH);
static JavaVM *SearchCreatedJavaVMPath(void);
static BOOL LoadFuntionsJVM(char *filedynlib);
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
	/* 2] search in PATH */
	/* else ERROR Java not found */

	BOOL bOK=FALSE;
	char *JVMDLLFULLNAME=NULL;
	
	/* 1] search in SCI/java/jre */
	JVMDLLFULLNAME=(char*)MALLOC( (strlen(SCILAB_PATH)+strlen(JRE_PATH)+strlen("/bin/")+strlen(JVM_TYPE)+strlen("/libjava")+strlen(SHARED_LIB_EXT)+1)*sizeof(char));
	sprintf(JVMDLLFULLNAME,"%s%s%s%s%s%s",SCILAB_PATH,JRE_PATH,"/bin/",JVM_TYPE,"/libjava",SHARED_LIB_EXT);

	if (!LoadFuntionsJVM(JVMDLLFULLNAME))
	{
		  /* 2] search in PATH */
			if (JVMDLLFULLNAME){FREE(JVMDLLFULLNAME);JVMDLLFULLNAME=NULL;};
			JVMDLLFULLNAME=(char*)MALLOC( (strlen("libjava")+strlen(SHARED_LIB_EXT)+1)*sizeof(char));
			sprintf(JVMDLLFULLNAME,"%s%s","libjava",SHARED_LIB_EXT);
			if (LoadFuntionsJVM(JVMDLLFULLNAME)) bOK=TRUE;
	}
	else 
	{
		EMBEDDED_JRE=TRUE;
		bOK=TRUE;
	}

	if (JVMDLLFULLNAME){FREE(JVMDLLFULLNAME);JVMDLLFULLNAME=NULL;};
	
	return bOK;

}
/*-----------------------------------------------------------------------------------*/ 
BOOL FreeDynLibJVM(void)
{
	BOOL bOK=FALSE;
	
	if (!dlclose(hLibJVM))
	{
		ptr_JNI_GetDefaultJavaVMInitArgs = NULL; 
		ptr_JNI_CreateJavaVM = NULL; 
		ptr_JNI_GetCreatedJavaVMs = NULL; 
		bOK=TRUE;
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL withEmbeddedJRE(void)
{
	return EMBEDDED_JRE;
}
/*-----------------------------------------------------------------------------------*/ 
static BOOL LoadFuntionsJVM(char *filedynlib)
{
	BOOL bOK=FALSE;
	
	hLibJVM = dlopen(filedynlib, RTLD_NOW | RTLD_GLOBAL); 
	
	if (hLibJVM)
	{
		ptr_JNI_GetDefaultJavaVMInitArgs = (JNI_GetDefaultJavaVMInitArgsPROC) dlsym(hLibJVM, "JNI_GetDefaultJavaVMInitArgs" ); 
		ptr_JNI_CreateJavaVM = (JNI_CreateJavaVMPROC) dlsym(hLibJVM, "JNI_CreateJavaVM" ); 
		ptr_JNI_GetCreatedJavaVMs = (JNI_GetCreatedJavaVMsPROC) dlsym(hLibJVM, "JNI_GetCreatedJavaVMs" ); 
	
		if (ptr_JNI_GetDefaultJavaVMInitArgs && ptr_JNI_CreateJavaVM && ptr_JNI_GetCreatedJavaVMs) bOK=TRUE;
  }
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
static JavaVM *SearchCreatedJavaVMEmbedded(char *SCILAB_PATH)
{
	JavaVM *jvm = NULL;
	jsize jvm_count = 0;
	jint res=0;
	char *JVMDLLFULLNAME=NULL;

	/* search in SCI/java/jre */
	JVMDLLFULLNAME=(char*)MALLOC( (strlen(SCILAB_PATH)+strlen(JRE_PATH)+strlen("/bin/")+strlen(JVM_TYPE)+strlen("/libjava")+strlen(SHARED_LIB_EXT)+1)*sizeof(char));
	sprintf(JVMDLLFULLNAME,"%s%s%s%s%s%s",SCILAB_PATH,JRE_PATH,"/bin/",JVM_TYPE,"/libjava",SHARED_LIB_EXT);

	FreeDynLibJVM();
	
	if (LoadFuntionsJVM(JVMDLLFULLNAME))
	{
		res = SciJNI_GetCreatedJavaVMs (&jvm, 1, &jvm_count);

		if ( jvm_count == 1 ) 
		{
			if (JVMDLLFULLNAME){FREE(JVMDLLFULLNAME);JVMDLLFULLNAME=NULL;}
			return jvm;
		}
		else jvm = NULL;
	}
	if (JVMDLLFULLNAME){FREE(JVMDLLFULLNAME);JVMDLLFULLNAME=NULL;}
	return jvm;
}
/*-----------------------------------------------------------------------------------*/ 
static JavaVM *SearchCreatedJavaVMPath(void)
{
	JavaVM *jvm = NULL;
	jsize jvm_count = 0;
	jint res=0;
	char *JVMDLLFULLNAME=NULL;
	
	FreeDynLibJVM();

	JVMDLLFULLNAME=(char*)MALLOC( (strlen("libjava")+strlen(SHARED_LIB_EXT)+1)*sizeof(char));
	sprintf(JVMDLLFULLNAME,"%s%s","libjava",SHARED_LIB_EXT);

	if (LoadFuntionsJVM(JVMDLLFULLNAME))
	{
		res = SciJNI_GetCreatedJavaVMs (&jvm, 1, &jvm_count);
		if ( jvm_count == 1 ) 
		{
			if (JVMDLLFULLNAME){FREE(JVMDLLFULLNAME);JVMDLLFULLNAME=NULL;};		
			return jvm;
		}
		else jvm = NULL;
	}
	if (JVMDLLFULLNAME){FREE(JVMDLLFULLNAME);JVMDLLFULLNAME=NULL;};		
	return jvm;
}
/*-----------------------------------------------------------------------------------*/ 
JavaVM *FindCreatedJavaVM(char *SCILAB_PATH)
{
	JavaVM *jvm = NULL;

	jvm = SearchCreatedJavaVMEmbedded(SCILAB_PATH);
	if (jvm) return jvm;
	else
	{
			jvm = SearchCreatedJavaVMPath();
			if (jvm) return jvm;
	}
	return NULL;
}
/*-----------------------------------------------------------------------------------*/ 
