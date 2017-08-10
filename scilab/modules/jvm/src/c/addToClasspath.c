/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) 2008-2008 - INRIA - Sylvestre LEDRU
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
#include <stdio.h>
#include "addToClasspath.h"
#include "getScilabJNIEnv.h"
#include "../../fileio/includes/isdir.h"
#include "../../fileio/includes/FileExist.h"
#include "fromjava.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
BOOL addToClasspath(char *classpathstring, typeOfLoad load)
{
    if (classpathstring)
    {
        if ( isdir(classpathstring) || FileExist(classpathstring) )
        {
            JNIEnv * currentENV = getScilabJNIEnv();
            if (currentENV)
            {
                jclass cls = NULL;
                /* Boot loader for scilab and javasci
                *  We rely on the fact that the javasci jar has an explicit
                * dependency on the jvm module. Class-Path in the manifest
                */
                cls = (*currentENV)->FindClass(currentENV, "org/scilab/modules/jvm/ClassPath");

                if (cls)
                {
                    jmethodID mid = (*currentENV)->GetStaticMethodID(currentENV, cls, "addFile", "(Ljava/lang/String;I)V");
                    if (mid)
                    {
                        jstring jstr = (*currentENV)->NewStringUTF(currentENV, classpathstring);
                        (*currentENV)->CallStaticObjectMethod(currentENV, cls, mid, jstr, (int)load);
                        return TRUE;
                    }
                }
            }
        }
        else
        {
            fprintf(stderr, _("Warning: Could not find Java package '%s'.\n"), classpathstring);
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
