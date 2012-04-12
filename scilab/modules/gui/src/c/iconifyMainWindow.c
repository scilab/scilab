/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#include "iconifyMainWindow.h"
#include "getScilabJNIEnv.h"
#include "getScilabObject.h"
/*--------------------------------------------------------------------------*/ 
typedef enum { ICONIFY_MAINWINDOW = 3,
DEICONIFY_MAINWINDOW = 2,
MAXIMIZE_MAINWINDOW = 1,
NORMAL_MAINWINDOW = 0
} windowState_enum;
/*--------------------------------------------------------------------------*/ 
static void setMainWindowState(windowState_enum windowState);
/*--------------------------------------------------------------------------*/ 
void deiconifyMainWindow(void)
{
	setMainWindowState(DEICONIFY_MAINWINDOW);
}
/*--------------------------------------------------------------------------*/ 
void iconifyMainWindow(void)
{
	setMainWindowState(ICONIFY_MAINWINDOW);
}
/*--------------------------------------------------------------------------*/ 
void maximizeMainWindow(void)
{
	setMainWindowState(MAXIMIZE_MAINWINDOW);
}
/*--------------------------------------------------------------------------*/ 
void normalMainWindow(void)
{
	setMainWindowState(NORMAL_MAINWINDOW);
}
/*--------------------------------------------------------------------------*/ 
static void setMainWindowState(windowState_enum windowState)
{
	JNIEnv *env = getScilabJNIEnv();
	jobject  ScilabObj = getScilabObject();

	if (env)
	{
		jclass class_Mine = (*env)->GetObjectClass(env, ScilabObj);
		if (class_Mine)
		{
			jfieldID id_Window =  (*env)->GetFieldID(env, class_Mine, "mainView","Lorg/scilab/modules/gui/bridge/window/SwingScilabWindow;");
			if (id_Window)
			{
				jobject jMainView = (*env)->GetObjectField(env, ScilabObj, id_Window);
				if (jMainView)
				{
					jclass cls = (*env)->GetObjectClass(env, jMainView);
					if (cls)
					{
						jmethodID mid = 0;
						switch (windowState)
						{
						case DEICONIFY_MAINWINDOW:
							mid = (*env)->GetMethodID(env, cls, "windowDeiconified", "()V");
							break;
						case ICONIFY_MAINWINDOW :
							mid = (*env)->GetMethodID(env, cls, "windowIconified", "()V");
							break;
						case MAXIMIZE_MAINWINDOW:
							mid = (*env)->GetMethodID(env, cls, "windowMaximized", "()V");
							break;
						case NORMAL_MAINWINDOW:default:
							mid = (*env)->GetMethodID(env, cls, "windowNormal", "()V");
							break;
						}
						
						if (mid)
						{
							(*env)->CallVoidMethod(env, jMainView, mid);
						}
					}
				}
			}
		}
	}
}
/*--------------------------------------------------------------------------*/ 
