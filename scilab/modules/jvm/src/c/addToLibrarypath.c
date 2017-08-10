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
#include "addToLibrarypath.h"
#include "getScilabJNIEnv.h"
#include "isdir.h"
#include "fromjava.h"
#
/*--------------------------------------------------------------------------*/
BOOL addToLibrarypath(char *librarypathstring)
{

    if (librarypathstring)
    {
        if ( isdir(librarypathstring) )
        {
            JNIEnv * currentENV = getScilabJNIEnv();
            if (currentENV)
            {
                jclass cls = NULL;
                /* Boot loader for scilab and javasci
                *  We rely on the fact that the javasci jar has an explicit
                * dependency on the jvm module. Class-Path in the manifest
                */
                cls = (*currentENV)->FindClass(currentENV, "org/scilab/modules/jvm/LibraryPath");

                if (cls)
                {
                    jmethodID mid = NULL;
                    mid = (*currentENV)->GetStaticMethodID(currentENV, cls, "addPath", "(Ljava/lang/String;)V");
                    if (mid)
                    {
                        jstring jstr;
                        jstr = (*currentENV)->NewStringUTF(currentENV, librarypathstring);
                        (*currentENV)->CallStaticObjectMethod(currentENV, cls, mid, jstr);
                        return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
