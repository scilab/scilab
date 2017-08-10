/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/
/*--------------------------------------------------------------------------*/
#include <jni.h>
#include "setVisibleMainWindow.h"
#include "getScilabJNIEnv.h"
#include "getScilabObject.h"
/*--------------------------------------------------------------------------*/
void setVisibleMainWindow(BOOL newVisibleState)
{
    JNIEnv *env = getScilabJNIEnv();
    jobject  ScilabObj = getScilabObject();

    if (env)
    {
        jclass class_Mine = (*env)->GetObjectClass(env, ScilabObj);
        if (class_Mine)
        {
            jfieldID id_Window =  (*env)->GetFieldID(env, class_Mine, "mainView", "Lorg/scilab/modules/gui/bridge/window/SwingScilabWindow;");
            if (id_Window)
            {
                jobject jMainView = (*env)->GetObjectField(env, ScilabObj, id_Window);
                if (jMainView)
                {
                    jclass cls = (*env)->GetObjectClass(env, jMainView);
                    if (cls)
                    {
                        jmethodID mid = (*env)->GetMethodID(env, cls, "setVisible", "(Z)V");
                        if (mid)
                        {
                            jboolean newVisibleState_ = (newVisibleState ? JNI_TRUE : JNI_FALSE);

                            (*env)->CallVoidMethod(env, jMainView, mid, newVisibleState_);
                        }
                    }
                }
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
BOOL isVisibleMainWindow(void)
{
    JNIEnv *env = getScilabJNIEnv();
    jobject  ScilabObj = getScilabObject();

    if (env)
    {
        jclass class_Mine = (*env)->GetObjectClass(env, ScilabObj);
        if (class_Mine)
        {
            jfieldID id_Window =  (*env)->GetFieldID(env, class_Mine, "mainView", "Lorg/scilab/modules/gui/bridge/window/SwingScilabWindow;");
            if (id_Window)
            {
                jobject jMainView = (*env)->GetObjectField(env, ScilabObj, id_Window);
                if (jMainView)
                {
                    jclass cls = (*env)->GetObjectClass(env, jMainView);
                    if (cls)
                    {
                        jmethodID mid = (*env)->GetMethodID(env, cls, "isVisible", "(V)Z");
                        if (mid)
                        {
                            jboolean jbVisible = (*env)->CallBooleanMethod(env, jMainView, mid);
                            return (jbVisible ? TRUE : FALSE);
                        }
                    }
                }
            }
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
