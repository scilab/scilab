/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2010 - Allan CORNET
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
#include "createMainScilabObject.h"
#include "getScilabObject.h"
#include "getScilabJNIEnv.h"
#include "getScilabJavaVM.h"
#include "catchIfJavaException.h"
#include "configvariable_interface.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
static jobject ScilabObject = NULL;
/*--------------------------------------------------------------------------*/
BOOL createMainScilabObject(void)
{
    BOOL bOK = FALSE;
    JNIEnv * currentENV = getScilabJNIEnv();

    if (currentENV)
    {
        jclass cls = NULL;
        cls = (*currentENV)->FindClass(currentENV, "org/scilab/modules/core/Scilab");
        bOK = catchIfJavaException(_("Could not access to the Main Scilab Class:\n"));
        if (cls)
        {
            jmethodID mid = NULL;
            mid = (*currentENV)->GetMethodID(currentENV, cls, "<init>", "(I)V");
            bOK = catchIfJavaException(_("Could not access to the constructor of the Main Scilab Class:\n"));
            if (mid)
            {
                jint ScilabMode = getScilabMode();
                jobject localScilabObject = (*currentENV)->NewObject(currentENV, cls, mid, ScilabMode);
                bOK = catchIfJavaException(_("Could not create a Scilab main class. Error:\n"));
                if (bOK == TRUE)
                {
                    ScilabObject = (*currentENV)->NewGlobalRef(currentENV, localScilabObject);
                    /* Catch the exception and display an human-reading error message */
                    bOK = catchIfJavaException(_("Could not create a Scilab main class. Error:\n"));
                }
            }
        }
    }

    return bOK;
}
/*--------------------------------------------------------------------------*/
jobject getScilabObject(void)
{
    return ScilabObject;
}
/*--------------------------------------------------------------------------*/
BOOL finishMainScilabObject(void)
{
    BOOL retValue = FALSE;
    jint result = 1;
    JNIEnv * currentENV = getScilabJNIEnv();
    JavaVM * currentJVM = getScilabJavaVM();

    JavaVMAttachArgs args;
    args.version = (*currentENV)->GetVersion(currentENV);
    args.name = (char *) "Scilab - Finish";
    args.group = NULL;

    result = (*currentJVM)->AttachCurrentThread(currentJVM, (void **) &currentENV, (void*) &args) ;
    if (result == 0)
    {
        jclass cls = NULL;
        cls = (*currentENV)->FindClass(currentENV, "org/scilab/modules/core/Scilab");
        catchIfJavaException(_("Could not access to the Main Scilab Class:\n"));
        if (cls)
        {
            jmethodID mid = NULL;
            mid = (*currentENV)->GetStaticMethodID(currentENV, cls, "executeFinalHooks", "()V");
            if (mid)
            {
                (*currentENV)->CallStaticVoidMethod(currentENV, cls, mid);
            }
            catchIfJavaException(_("Cannot execute final hooks. Error:\n"));

            (*currentENV)->DeleteGlobalRef(currentENV, ScilabObject);
            ScilabObject = NULL;
            retValue = TRUE;
        }

        (*currentJVM)->DetachCurrentThread(currentJVM);
    }
    return retValue;
}
/*--------------------------------------------------------------------------*/
BOOL canCloseMainScilabObject(void)
{
    BOOL retValue = FALSE;
    JNIEnv * currentENV = getScilabJNIEnv();
    JavaVM * currentJVM = getScilabJavaVM();
    jint result = 1;

    JavaVMAttachArgs args;
    args.version = (*currentENV)->GetVersion(currentENV);
    args.name = (char *) "Scilab - Try finish";
    args.group = NULL;

    result = (*currentJVM)->AttachCurrentThread(currentJVM, (void **) &currentENV, (void*) &args) ;
    if (result == 0)
    {
        jclass cls = NULL;
        cls = (*currentENV)->FindClass(currentENV, "org/scilab/modules/core/Scilab");
        catchIfJavaException(_("Could not access to the Main Scilab Class:\n"));
        if (cls)
        {
            jmethodID mid = NULL;
            mid = (*currentENV)->GetStaticMethodID(currentENV, cls, "canClose", "()Z");
            if (mid)
            {
                retValue = (*currentENV)->CallStaticBooleanMethod(currentENV, cls, mid);
            }
            catchIfJavaException(_("Error with Scilab.canClose():\n"));
        }

        (*currentJVM)->DetachCurrentThread(currentJVM);
    }

    return retValue;
}
/*--------------------------------------------------------------------------*/
void forceCloseMainScilabObject(void)
{
    JNIEnv * currentENV = getScilabJNIEnv();
    JavaVM * currentJVM = getScilabJavaVM();
    jint result = 1;

    JavaVMAttachArgs args;
    args.version = (*currentENV)->GetVersion(currentENV);
    args.name = (char *) "Scilab - Force finish";
    args.group = NULL;

    result = (*currentJVM)->AttachCurrentThread(currentJVM, (void **) &currentENV, (void*) &args) ;
    if (result == 0)
    {
        jclass cls = NULL;
        cls = (*currentENV)->FindClass(currentENV, "org/scilab/modules/core/Scilab");
        catchIfJavaException(_("Could not access to the Main Scilab Class:\n"));
        if (cls)
        {
            jmethodID mid = NULL;
            mid = (*currentENV)->GetStaticMethodID(currentENV, cls, "forceClose", "()V");
            if (mid)
            {
                (*currentENV)->CallStaticVoidMethod(currentENV, cls, mid);
            }
            catchIfJavaException(_("Error with Scilab.forceClose():\n"));
        }
    }
}
/*--------------------------------------------------------------------------*/

