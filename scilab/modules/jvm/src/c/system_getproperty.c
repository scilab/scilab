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
#include <jni.h>
#include <string.h>
#include "system_getproperty.h"
#include "getScilabJNIEnv.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/ 
char * system_getproperty(char *property,char *defaultproperty)
{
	char *retValue = NULL;
	JNIEnv * currentENV = getScilabJNIEnv();

	if (currentENV)
	{
		jclass cls=NULL;
		cls = (*currentENV)->FindClass(currentENV, "java/lang/System");
		if (cls)
		{
			jmethodID mid=NULL;
			mid = (*currentENV)->GetStaticMethodID(currentENV, cls, "getProperty","(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
			if (mid)
			{
				const char *strValue = NULL;
				jstring jstrProperty;
				jstring jstrDefaultValue;
				jstring jstrValue;
				jstrProperty = (*currentENV)->NewStringUTF(currentENV,property);
				jstrDefaultValue = (*currentENV)->NewStringUTF(currentENV,defaultproperty);

				jstrValue=(*currentENV)->CallStaticObjectMethod(currentENV,cls, mid,jstrProperty,jstrDefaultValue);

				strValue = (*currentENV)->GetStringUTFChars(currentENV,jstrValue, 0);
				if (strValue)
				{
					retValue = strdup(strValue);
				}
				(*currentENV)->ReleaseStringUTFChars(currentENV, jstrValue , strValue);
			}
		}
	}
	return retValue;
}
/*--------------------------------------------------------------------------*/ 
