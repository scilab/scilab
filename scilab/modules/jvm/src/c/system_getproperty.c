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
#include "system_getproperty.h"
#include "getScilabJNIEnv.h"
#include "sci_malloc.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
char * system_getproperty(const char * property, const char * defaultproperty)
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
            mid = (*currentENV)->GetStaticMethodID(currentENV, cls, "getProperty", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
            if (mid)
            {
                const char *strValue = NULL;
                jstring jstrProperty;
                jstring jstrDefaultValue;
                jstring jstrValue;
                jstrProperty = (*currentENV)->NewStringUTF(currentENV, property);
                jstrDefaultValue = (*currentENV)->NewStringUTF(currentENV, defaultproperty);

                jstrValue = (*currentENV)->CallStaticObjectMethod(currentENV, cls, mid, jstrProperty, jstrDefaultValue);

                strValue = (*currentENV)->GetStringUTFChars(currentENV, jstrValue, 0);
                if (strValue)
                {
                    retValue = os_strdup(strValue);
                }
                (*currentENV)->ReleaseStringUTFChars(currentENV, jstrValue , strValue);
            }
        }
    }
    return retValue;
}
/*--------------------------------------------------------------------------*/
