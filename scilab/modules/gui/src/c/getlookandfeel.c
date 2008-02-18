/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#include <jni.h>
#include <string.h>
#include "getlookandfeel.h"
#include "MALLOC.h"
#include "getScilabJNIEnv.h"
/*--------------------------------------------------------------------------*/ 
char * getlookandfeel(void)
{
	char *lookstr = NULL;
	JNIEnv *env = getScilabJNIEnv();

	if (env)
	{
		jclass classLookAndFeel = (*env)->FindClass(env, "org/scilab/modules/gui/utils/LookAndFeelManager");
		if (classLookAndFeel)
		{
			jmethodID methodgetLookAndFeel = (*env)->GetStaticMethodID(env,classLookAndFeel, "getCurrentLookAndFeel", "()Ljava/lang/String;");
			if (methodgetLookAndFeel)
			{
				jstring jstrValue = (*env)->CallStaticObjectMethod(env,classLookAndFeel, methodgetLookAndFeel,NULL);
				if (jstrValue)
				{
					const char *strValue = NULL;
					strValue = (*env)->GetStringUTFChars(env,jstrValue, 0);
					if (strValue)
					{
						lookstr = (char*)MALLOC(sizeof(char)*(strlen(strValue)+1));
						strcpy(lookstr,strValue);
					}
					(*env)->ReleaseStringUTFChars(env, jstrValue , strValue);
				}
			}
		}
	}
	return lookstr;
}
/*--------------------------------------------------------------------------*/ 
