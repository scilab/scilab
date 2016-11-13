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
#include "getLibrarypath.h"
#include "getScilabJNIEnv.h"
#include "fromjava.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
char **getLibrarypath(int *sizeLibrarypathArray)
{
    char **LibrarypathArray = NULL;
    JNIEnv * currentENV = getScilabJNIEnv();
    if (currentENV)
    {
        jclass cls = NULL;
        if (IsFromJava())
        {
            /* Boot loader for scilab and javasci */
            /* if scilab is called from java (javasci), we need to update standard java.library.path */
            /* doesn't require to add -cp SCI/modules/jvm/jar/org.scilab.modules.jvm.jar when you use javasci */
            cls = (*currentENV)->FindClass(currentENV, "javasci/LibraryPath");
        }
        else
        {
            cls = (*currentENV)->FindClass(currentENV, "org/scilab/modules/jvm/LibraryPath");
        }

        if (cls)
        {
            jmethodID mid = NULL;
            mid = (*currentENV)->GetStaticMethodID(currentENV, cls, "getLibraryPath", "()[Ljava/lang/String;");
            if (mid)
            {
                jobjectArray jStrings;

                jStrings = (*currentENV)->CallStaticObjectMethod(currentENV, cls, mid, NULL);
                *sizeLibrarypathArray = (*currentENV)->GetArrayLength(currentENV, jStrings);
                if (*sizeLibrarypathArray > 0)
                {
                    int i = 0;

                    LibrarypathArray = (char**)MALLOC(sizeof(char*) * (*sizeLibrarypathArray));
                    for (i = 0; i < *sizeLibrarypathArray; i++)
                    {
                        jstring jelement;
                        const char *str = NULL;
                        char *string = NULL;

                        jelement = (jstring)(*currentENV)->GetObjectArrayElement(currentENV, jStrings, i);
                        str = (*currentENV)->GetStringUTFChars(currentENV, jelement, 0);
                        string = (char*)MALLOC(sizeof(char) * (strlen(str) + 1));
                        strcpy(string, str);
                        LibrarypathArray[i] = string;
                        (*currentENV)->ReleaseStringUTFChars(currentENV, jelement, str);
                    }
                }
            }
        }
    }
    return LibrarypathArray;
}
/*--------------------------------------------------------------------------*/
