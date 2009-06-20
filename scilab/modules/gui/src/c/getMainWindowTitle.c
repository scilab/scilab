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
#include "getMainWindowTitle.h"
#include "getScilabJNIEnv.h"
#include "getScilabObject.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/ 
char * getMainWindowTitle(void)
{
	char *title = NULL;

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
						jmethodID mid = (*env)->GetMethodID(env, cls, "getTitle", "()Ljava/lang/String;");
						if (mid)
						{
							jstring jstr;
							jstr = (*env)->CallObjectMethod(env, jMainView, mid);
							if (jstr)
							{
								const char *strValue = NULL;
								strValue = (*env)->GetStringUTFChars(env,jstr, 0);
								if (strValue)
								{
									title = strdup(strValue);
								}
								(*env)->ReleaseStringUTFChars(env, jstr , strValue);
							}
						}
					}
				}
			}
		}
	}

	return title;
}
/*--------------------------------------------------------------------------*/ 
