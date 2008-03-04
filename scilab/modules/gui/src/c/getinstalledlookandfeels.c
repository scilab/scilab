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
/* @TODO: rewrite using GIWS ! */
#include <jni.h>
#include <string.h>
#include "MALLOC.h"
#include "getinstalledlookandfeels.h"
#include "getScilabJNIEnv.h"
/*--------------------------------------------------------------------------*/ 
char ** getinstalledlookandfeels(int *sizeReturned)
{
	char **ListLookAndFeels = NULL;
	JNIEnv *env = getScilabJNIEnv();
	jmethodID constructObject = NULL ;
	jobject localInstance ;

	const char* construct="<init>";
	const char* param="()V";
	if (env)
	{
		jclass classLookAndFeel = (*env)->FindClass(env, "org/scilab/modules/gui/utils/LookAndFeelManager");
		if (classLookAndFeel)
		{
			constructObject = (*env)->GetMethodID(env, classLookAndFeel, construct , param ) ;
			localInstance = (*env)->NewObject(env, classLookAndFeel, constructObject ) ;

			jmethodID methodgetInstalledLookAndFeels = (*env)->GetMethodID(env,classLookAndFeel, "getInstalledLookAndFeels", "()[Ljava/lang/String;"); 
			jobjectArray jStrings = (*env)->CallObjectMethod(env,localInstance,methodgetInstalledLookAndFeels,NULL);

			jobjectArray objStrInstalledLookAndFeels = (*env)->CallObjectMethod(env,localInstance,methodgetInstalledLookAndFeels);
			*sizeReturned=(*env)->GetArrayLength(env,objStrInstalledLookAndFeels);
			if (*sizeReturned > 0)
			{
				int i=0;
				ListLookAndFeels=(char**)MALLOC(sizeof(char*)*(*sizeReturned));
				for (i=0;i<*sizeReturned;i++)
				{
					jstring jelement;
					const char *str = NULL;
					char *string = NULL;

					jelement = (jstring)(*env)->GetObjectArrayElement(env, jStrings, i);
					str = (*env)->GetStringUTFChars(env,jelement, 0);
					string = (char*)MALLOC(sizeof(char)*(strlen(str)+1));

					strcpy(string,str);

					ListLookAndFeels[i]=string;
					(*env)->ReleaseStringUTFChars(env, jelement, str);
				}
			}
		}
	}
	return ListLookAndFeels;
}
/*--------------------------------------------------------------------------*/ 
