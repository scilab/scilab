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
#include "setlookandfeel.h"
#include "MALLOC.h"
#include "getScilabJNIEnv.h"
/*--------------------------------------------------------------------------*/ 
BOOL setlookandfeel(char *lookandfeekstr)
{
	BOOL bOK=FALSE;
	JNIEnv *env = getScilabJNIEnv();

	if (env)
	{
		jclass classLookAndFeel = (*env)->FindClass(env, "org/scilab/modules/gui/utils/LookAndFeelManager");
		if (classLookAndFeel)
		{
			jmethodID methodgetInstalledLookAndFeels = (*env)->GetStaticMethodID(env,classLookAndFeel, "setLookAndFeel", "(Ljava/lang/String;)"); 
			jobjectArray jStrings = (*env)->CallStaticObjectMethod(env,classLookAndFeel,methodgetInstalledLookAndFeels,NULL);
			jstring jstr = (*env)->NewStringUTF(env,lookandfeekstr);
			jboolean jbool = (*env)->CallStaticBooleanMethod(env,classLookAndFeel,methodgetInstalledLookAndFeels,jstr);
			bOK = (BOOL) jbool;
		}
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/ 
BOOL setsystemlookandfeel(void)
{
	BOOL bOK=FALSE;
	JNIEnv *env = getScilabJNIEnv();

	if (env)
	{
		jclass classLookAndFeel = (*env)->FindClass(env, "org/scilab/modules/gui/utils/LookAndFeelManager");
		if (classLookAndFeel)
		{
			jmethodID methodgetInstalledLookAndFeels = (*env)->GetStaticMethodID(env,classLookAndFeel, "setSystemLookAndFeel", "()"); 
			jboolean jbool = (*env)->CallStaticBooleanMethod(env,classLookAndFeel,methodgetInstalledLookAndFeels);
			bOK = (BOOL) jbool;
		}
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/ 
