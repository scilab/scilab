/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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

#include <jni.h>
#include <string.h>
#include "getMainWindowTitle.h"
#include "getScilabJNIEnv.h"
#include "getScilabObject.h"
#include "sci_malloc.h"
#include "os_string.h"
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
            jfieldID id_Window =  (*env)->GetFieldID(env, class_Mine, "mainView", "Lorg/scilab/modules/gui/window/Window;");

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
                                strValue = (*env)->GetStringUTFChars(env, jstr, 0);
                                if (strValue)
                                {
                                    title = os_strdup(strValue);
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
