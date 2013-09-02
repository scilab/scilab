/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <jni.h>
#include <string.h>
#include "system_setproperty.h"
#include "getScilabJNIEnv.h"
#include "MALLOC.h"
#include "catchIfJavaException.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
char * system_setproperty(char *property, char *value)
{
    char *retValue = NULL;

    JNIEnv * currentENV = getScilabJNIEnv();

    if (currentENV)
    {
        jclass cls = NULL;
        cls = (*currentENV)->FindClass(currentENV, "java/lang/System");
        if (cls)
        {
            jmethodID mid = NULL;
            mid = (*currentENV)->GetStaticMethodID(currentENV, cls, "setProperty", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
            if (mid)
            {
                BOOL bOK = FALSE;
                const char *strPreviousValue = NULL;
                jstring jstrProperty;
                jstring jstrValue;
                jstring jstrPreviousValue;

                jstrProperty = (*currentENV)->NewStringUTF(currentENV, property);
                jstrValue = (*currentENV)->NewStringUTF(currentENV, value);

                jstrPreviousValue = (*currentENV)->CallStaticObjectMethod(currentENV, cls, mid, jstrProperty, jstrValue);
                bOK = catchIfJavaException("");

                if (bOK)
                {
                    if (jstrPreviousValue)
                    {
                        strPreviousValue = (*currentENV)->GetStringUTFChars(currentENV, jstrPreviousValue, 0);
                        if (strPreviousValue)
                        {
                            retValue = strdup(strPreviousValue);
                        }
                        (*currentENV)->ReleaseStringUTFChars(currentENV, jstrPreviousValue , strPreviousValue);
                    }
                }
            }
        }
    }
    return retValue;
}
/*--------------------------------------------------------------------------*/
