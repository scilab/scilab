/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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
#include <string.h>
#include "system_setproperty.h"
#include "getScilabJNIEnv.h"
#include "sci_malloc.h"
#include "catchIfJavaException.h"
#include "os_string.h"
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
                            retValue = os_strdup(strPreviousValue);
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
