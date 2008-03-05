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
#include "setlookandfeel.h"
#include "MALLOC.h"
#include "getScilabJNIEnv.h"
/*--------------------------------------------------------------------------*/ 
BOOL setlookandfeel(char *lookandfeekstr)
{
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
			jmethodID methodSetLookAndFeel;
			jstring jstr;
			constructObject = (*env)->GetMethodID(env, classLookAndFeel, construct , param ) ;
			localInstance = (*env)->NewObject(env, classLookAndFeel, constructObject ) ;
			methodSetLookAndFeel = (*env)->GetMethodID(env,classLookAndFeel, "setLookAndFeel", "(Ljava/lang/String;)Z"); 
			jstr = (*env)->NewStringUTF(env,lookandfeekstr);
			return (BOOL) (*env)->CallBooleanMethod(env,localInstance,methodSetLookAndFeel,jstr);
		}
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/ 
BOOL setsystemlookandfeel(void)
{
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
			jmethodID methodSetSystemLookAndFeels;
			constructObject = (*env)->GetMethodID(env, classLookAndFeel, construct , param ) ;
			localInstance = (*env)->NewObject(env, classLookAndFeel, constructObject ) ;
			methodSetSystemLookAndFeels = (*env)->GetMethodID(env,classLookAndFeel, "setSystemLookAndFeel", "()Z"); 
			return (BOOL) (*env)->CallBooleanMethod(env,localInstance,methodSetSystemLookAndFeels);
		}
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/ 
