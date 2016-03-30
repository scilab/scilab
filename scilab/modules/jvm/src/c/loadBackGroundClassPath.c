/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#include "loadBackGroundClassPath.h"
#include "addToClasspath.h"
#include "getScilabJNIEnv.h"
#include "fromjava.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
BOOL loadBackGroundClassPath(void)
{
    JNIEnv * currentENV = getScilabJNIEnv();
    if (currentENV)
    {
        jclass cls = NULL;
        cls = (*currentENV)->FindClass(currentENV, "org/scilab/modules/jvm/ClassPath");

        if (cls)
        {
            jmethodID mid = (*currentENV)->GetStaticMethodID(currentENV, cls, "loadBackGroundClassPath", "()V");
            if (mid)
            {
                (*currentENV)->CallStaticObjectMethod(currentENV, cls, mid);
                return TRUE;
            }
        }
    }

    return FALSE;
}
/*--------------------------------------------------------------------------*/
