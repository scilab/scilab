/*--------------------------------------------------------------------------*/ 
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
/*--------------------------------------------------------------------------*/ 
static BOOL EMBEDDED_JRE=FALSE;
/*--------------------------------------------------------------------------*/ 
BOOL LoadDynLibJVM(char *SCILAB_PATH)
{
	/* 1] search in SCI/java/jre */
	/* 2] search in PATH */
	/* else ERROR Java not found */

	BOOL bOK=FALSE;
	char *JVMLIBFULLNAME=NULL;
	
	/* 1] search in SCI/java/jre */
	JVMLIBFULLNAME=(char*)MALLOC( (strlen(SCILAB_PATH)+strlen(JRE_PATH)+strlen("/bin/")+strlen(JVM_TYPE)+strlen("/libjava")+strlen(SHARED_LIB_EXT)+1)*sizeof(char));
	sprintf(JVMLIBFULLNAME,"%s%s%s%s%s%s",SCILAB_PATH,JRE_PATH,"/bin/",JVM_TYPE,"/libjava",SHARED_LIB_EXT);

	if (LoadFuntionsJVM(JVMLIBFULLNAME)==NULL)
	{
		  /* 2] search in PATH */
			if (JVMLIBFULLNAME){FREE(JVMLIBFULLNAME);JVMLIBFULLNAME=NULL;};
			JVMLIBFULLNAME=(char*)MALLOC( (strlen("libjava")+strlen(SHARED_LIB_EXT)+1)*sizeof(char));
			sprintf(JVMLIBFULLNAME,"%s%s","libjava",SHARED_LIB_EXT);
			if (LoadFuntionsJVM(JVMLIBFULLNAME)) bOK=TRUE;
	}
	else 
	{
		EMBEDDED_JRE=TRUE;
		bOK=TRUE;
	}

	if (JVMLIBFULLNAME){FREE(JVMLIBFULLNAME);JVMLIBFULLNAME=NULL;};
	
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
	char *JVMLIBFULLNAME=NULL;

	/* search in SCI/java/jre */
	JVMLIBFULLNAME=(char*)MALLOC( (strlen(SCILAB_PATH)+strlen(JRE_PATH)+strlen("/bin/")+strlen(JVM_TYPE)+strlen("/libjava")+strlen(SHARED_LIB_EXT)+1)*sizeof(char));
	sprintf(JVMLIBFULLNAME,"%s%s%s%s%s%s",SCILAB_PATH,JRE_PATH,"/bin/",JVM_TYPE,"/libjava",SHARED_LIB_EXT);

	FreeDynLibJVM();
	
	if (LoadFuntionsJVM(JVMLIBFULLNAME))
	{
		res = SciJNI_GetCreatedJavaVMs (&jvm, 1, &jvm_count);

		if ( jvm_count == 1 ) 
		{
			if (JVMLIBFULLNAME){FREE(JVMLIBFULLNAME);JVMLIBFULLNAME=NULL;}
			return jvm;
		}
		else jvm = NULL;
	}
	if (JVMLIBFULLNAME){FREE(JVMLIBFULLNAME);JVMLIBFULLNAME=NULL;}
	return jvm;
}
/*--------------------------------------------------------------------------*/ 
static JavaVM *SearchCreatedJavaVMPath(void)
{
	JavaVM *jvm = NULL;
	jsize jvm_count = 0;
	jint res=0;
	char *JVMLIBFULLNAME=NULL;
	
	FreeDynLibJVM();

	JVMLIBFULLNAME=(char*)MALLOC( (strlen("libjava")+strlen(SHARED_LIB_EXT)+1)*sizeof(char));
	sprintf(JVMLIBFULLNAME,"%s%s","libjava",SHARED_LIB_EXT);

	if (LoadFuntionsJVM(JVMLIBFULLNAME))
	{
		res = SciJNI_GetCreatedJavaVMs (&jvm, 1, &jvm_count);
		if ( jvm_count == 1 ) 
		{
			if (JVMLIBFULLNAME){FREE(JVMLIBFULLNAME);JVMLIBFULLNAME=NULL;};		
			return jvm;
		}
		else jvm = NULL;
	}
	if (JVMLIBFULLNAME){FREE(JVMLIBFULLNAME);JVMLIBFULLNAME=NULL;};		
	return jvm;
}
/*--------------------------------------------------------------------------*/ 
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
/*--------------------------------------------------------------------------*/ 
