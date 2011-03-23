/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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
                jclass cls=NULL;
                /* Boot loader for scilab and javasci
                *  We rely on the fact that the javasci jar has an explicit
                * dependency on the jvm module. Class-Path in the manifest
                */
                cls = (*currentENV)->FindClass(currentENV, "org/scilab/modules/jvm/LibraryPath");
                
                if (cls)
                {
                    jmethodID mid=NULL;
                    mid = (*currentENV)->GetStaticMethodID(currentENV, cls,"addPath","(Ljava/lang/String;)V");
                    if (mid)
                    {
                        jstring jstr;
                        jstr = (*currentENV)->NewStringUTF(currentENV,librarypathstring);
                        (*currentENV)->CallStaticObjectMethod(currentENV,cls, mid,jstr);
                        return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/ 
