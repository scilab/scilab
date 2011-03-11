/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#include <string.h>
#include <stdlib.h>
#include "machine.h"
#include "JVM_commons.h"
#include "JVM_functions.h"
#include "sci_mem_alloc.h"
/*--------------------------------------------------------------------------*/ 
#define JVM_TYPE "client"
/* #define JVM_TYPE "server" */
/* Every form of Sun's Java runtime comes with both the "client VM" and the "server VM."
Unfortunately, Java applications and applets run by default in the client VM.
The Server VM is much faster than the Client VM,
but it has the downside of taking around 10% longer to start up, and it uses more memory.
*/
/*--------------------------------------------------------------------------*/ 
static JavaVM *SearchCreatedJavaVMEmbedded(char *SCILAB_PATH);
static JavaVM *SearchCreatedJavaVMPath(void);

#ifdef __APPLE__
/* I guess Apple likes to make my life harder ... 
 * They are renaming the name of the dynamic lib especially for jni lib...
 * Therefor, I must change the name only for mac os X from dynlib
 */
#undef SHARED_LIB_EXT
#define SHARED_LIB_EXT ".jnilib"
#define LIBJAVANAME "libjava"
#else
#define LIBJAVANAME "libjvm"
#endif

/*--------------------------------------------------------------------------*/ 
static BOOL EMBEDDED_JRE=FALSE;
/*--------------------------------------------------------------------------*/ 
BOOL LoadDynLibJVM(char *SCILAB_PATH)
{
	/* 1. search in SCI/java/jre 
	 * 2. search in LD_LIBRARY_PATH and co (see man dlopen) 
	 * else ERROR Java not found */

	BOOL bOK=FALSE;
	char *JVMLibFullName=NULL;
	
	/* 1. search in SCI/java/jre */
	JVMLibFullName=(char*)MALLOC( (strlen(SCILAB_PATH)+strlen(JRE_PATH)+strlen("/bin/")+strlen(JVM_TYPE)+strlen("/libjava")+strlen(SHARED_LIB_EXT)+1)*sizeof(char));
	sprintf(JVMLibFullName,"%s%s%s%s%s%s",SCILAB_PATH,JRE_PATH,"/bin/",JVM_TYPE,"/libjava",SHARED_LIB_EXT);

	if (LoadFunctionsJVM(JVMLibFullName)==NULL)
	{
		  /* 2. search in LD_LIBRARY_PATH */
			if (JVMLibFullName){FREE(JVMLibFullName);JVMLibFullName=NULL;};

                        JVMLibFullName=(char*)MALLOC( (strlen(LIBJAVANAME)+strlen(SHARED_LIB_EXT)+1)*sizeof(char));
                        sprintf(JVMLibFullName,"%s%s",LIBJAVANAME,SHARED_LIB_EXT);
			if (LoadFunctionsJVM(JVMLibFullName)) bOK=TRUE;
	}
	else 
	{
		EMBEDDED_JRE=TRUE;
		bOK=TRUE;
	}

	if (JVMLibFullName){FREE(JVMLibFullName);JVMLibFullName=NULL;};
	
	return bOK;

}
/*--------------------------------------------------------------------------*/ 
BOOL withEmbeddedJRE(void)
{
	return EMBEDDED_JRE;
}
/*--------------------------------------------------------------------------*/ 
static JavaVM *SearchCreatedJavaVMEmbedded(char *SCILAB_PATH)
{
	JavaVM *jvm = NULL;
	jsize jvm_count = 0;
	jint res=0;
	char *JVMLibFullName=NULL;

	/* search in SCI/java/jre */
	JVMLibFullName=(char*)MALLOC( (strlen(SCILAB_PATH)+strlen(JRE_PATH)+strlen("/bin/")+strlen(JVM_TYPE)+strlen("/libjava")+strlen(SHARED_LIB_EXT)+1)*sizeof(char));
	sprintf(JVMLibFullName,"%s%s%s%s%s%s",SCILAB_PATH,JRE_PATH,"/bin/",JVM_TYPE,"/libjava",SHARED_LIB_EXT);

	FreeDynLibJVM();
	
	if (LoadFunctionsJVM(JVMLibFullName))
	{
		res = SciJNI_GetCreatedJavaVMs (&jvm, 1, &jvm_count);

		if ( jvm_count == 1 ) 
		{
			if (JVMLibFullName){FREE(JVMLibFullName);JVMLibFullName=NULL;}
			return jvm;
		}
		else jvm = NULL;
	}
	if (JVMLibFullName){FREE(JVMLibFullName);JVMLibFullName=NULL;}
	return jvm;
}
/*--------------------------------------------------------------------------*/ 
static JavaVM *SearchCreatedJavaVMPath(void)
{
	JavaVM *jvm = NULL;
	jsize jvm_count = 0;
	jint res=0;
	char *JVMLibFullName=NULL;
	
	FreeDynLibJVM();

	JVMLibFullName=(char*)MALLOC( (strlen("libjava")+strlen(SHARED_LIB_EXT)+1)*sizeof(char));
	sprintf(JVMLibFullName,"%s%s","libjava",SHARED_LIB_EXT);

	if (LoadFunctionsJVM(JVMLibFullName))
	{
		res = SciJNI_GetCreatedJavaVMs (&jvm, 1, &jvm_count);
		if ( jvm_count == 1 ) 
		{
			if (JVMLibFullName){FREE(JVMLibFullName);JVMLibFullName=NULL;};		
			return jvm;
		}
		else jvm = NULL;
	}
	if (JVMLibFullName){FREE(JVMLibFullName);JVMLibFullName=NULL;};		
	return jvm;
}
/*--------------------------------------------------------------------------*/ 
JavaVM *FindCreatedJavaVM(char *SCILAB_PATH)
{
	JavaVM *jvm = NULL;
#ifndef __APPLE__
/* Under Mac OS X, we are using the JVM provided by the distribution.
 * However, this might change with Java 7 since Apple will no longer provide
 * Java with Mac OS X */
	jvm = SearchCreatedJavaVMEmbedded(SCILAB_PATH);
	if (jvm) return jvm;
	else
	{
#endif
			jvm = SearchCreatedJavaVMPath();
			if (jvm) return jvm;
#ifndef __APPLE__
	}
#endif
	return NULL;
}
/*--------------------------------------------------------------------------*/ 
