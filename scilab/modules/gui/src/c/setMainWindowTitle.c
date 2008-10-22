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
#include "setMainWindowTitle.h"
#include "getScilabJNIEnv.h"
#include "getScilabObject.h"
/*--------------------------------------------------------------------------*/ 
BOOL setMainWindowTitle(char* newtitle)
{
	BOOL bOK = FALSE;
	if (newtitle)
	{
		JNIEnv *env = getScilabJNIEnv();
		jobject  ScilabObj = getScilabObject();

		if (env)
		{
			jclass class_Mine = (*env)->GetObjectClass(env, ScilabObj);
			if (class_Mine)
			{
				jfieldID id_Window =  (*env)->GetFieldID(env, class_Mine, "mainView","Lorg/scilab/modules/gui/window/Window;");

				if (id_Window)
				{
					jobject jMainView = (*env)->GetObjectField(env, ScilabObj, id_Window);
					if (jMainView)
					{
						jclass cls = (*env)->GetObjectClass(env, jMainView);
						if (cls)
						{
							jmethodID mid = (*env)->GetMethodID(env, cls, "setTitle", "(Ljava/lang/String;)V");
							if (mid)
							{
								jstring jstr;
								jstr = (*env )->NewStringUTF(env ,newtitle);
								(*env)->CallVoidMethod(env, jMainView, mid,jstr);
								bOK = TRUE;
							}
						}
					}
				}
			}
		}
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/ 

