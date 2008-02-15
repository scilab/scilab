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
#include "gw_jvm.h"
#include "MALLOC.h"
#include "machine.h"
#include "stack-c.h"
#include "getScilabJavaVM.h"
#include "getScilabJavaVM.h"
#include "jni.h"
#include "sciprint.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_jvm_version) _PARAMS((char *fname,unsigned long fname_len))
{

/*	JavaVM* currentJVM =getScilabJavaVM();
	JNIEnv * currentENV ;

	jint res;

	jobject JvmVersionObject;
	jclass cls=NULL;
	jmethodID mid=NULL;


	res = (*currentJVM)->GetEnv(currentJVM, (void **)&currentENV, JNI_VERSION_1_6);
	if (!res)
	{
		cls = (*currentENV)->FindClass(currentENV, "JvmVersion");
		if (cls)
		{
			mid = (*currentENV)->GetMethodID(currentENV, cls, "<init>","()V");
			if (mid)
			{
				JvmVersionObject = (*currentENV)->NewObject(currentENV,cls,mid);
				if (JvmVersionObject)
				{
					(*currentENV)->CallObjectMethod(currentENV,(jobject)JvmVersionObject, mid,NULL);

					{
						const char *cname;
						jclass class_Mine = (*currentENV)->GetObjectClass(currentENV, JvmVersionObject);
						jfieldID id_vmVersion =  (*currentENV)->GetFieldID(currentENV, class_Mine, "vmVersion","Ljava/lang/String;");
						jstring vmVersion = (jstring) (*currentENV)->GetObjectField(currentENV, JvmVersionObject, id_vmVersion);
						cname = (*currentENV)->GetStringUTFChars(currentENV, vmVersion, NULL);
						sciprint("%s",cname);
						(*currentENV)->ReleaseStringUTFChars(currentENV, vmVersion , cname);
					}
				}
			}
		}
	}
*/
	LhsVar(0) = 0;
	C2F(putlhsvar)();	
	return 0;
}
/*--------------------------------------------------------------------------*/
