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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef _MSC_VER
#include <windows.h>
#endif
#include "InitializeJVM.h"
#include "loadClasspath.h"
#include "loadLibrarypath.h"
#include "setgetSCIpath.h"
#include "getScilabJNIEnv.h"
#include "getScilabJavaVM.h"
#include "MALLOC.h"
#include "JVM.h"
#include "createMainScilabObject.h"
#include "scilabDefaults.h"
#include "localization.h"
#include "fromjava.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "catchIfJavaException.h"
/*--------------------------------------------------------------------------*/
static void DoLoadClasspathInEtc(char *sciPath);
static void DoLoadLibrarypathInEtc(char *sciPath);
/*--------------------------------------------------------------------------*/
BOOL InitializeJVM(void)
{
    BOOL bOK = FALSE;
    char *sciPath = NULL;

    sciPath = getSCIpath();

    if (!startJVM(sciPath))
    {
#ifdef _MSC_VER
        MessageBox(NULL, gettext("\nScilab cannot open JVM library.\n"), gettext("Error"), MB_ICONEXCLAMATION | MB_OK);
#else
        fprintf(stderr, _("\nScilab cannot open JVM library.\n"));
#endif
    }
    else
    {
        DoLoadLibrarypathInEtc(sciPath);
        DoLoadClasspathInEtc(sciPath);

        if (!createMainScilabObject())
        {
            char *errorMsg = strdup(gettext("\nScilab cannot create Scilab Java Main-Class (we have not been able to find the main Scilab class. Check if the Scilab and thirdparty packages are available).\n"));

            if (IsFromJava())
            {
                char *errorMsg2 = gettext("If Scilab is used from Java, make sure that your IDE (ex: Netbeans, etc) is not adding extra dependencies which could not be found at runtime.\n");
                char *tempMsg = (char*)MALLOC(sizeof(char) * (strlen(errorMsg) + strlen(errorMsg2) + 1));
                if (tempMsg)
                {
                    strcpy(tempMsg, errorMsg);
                    strcat(tempMsg, errorMsg2);
                    FREE(errorMsg);
                    errorMsg = tempMsg;
                }
            }
#ifdef _MSC_VER
            MessageBox(NULL, errorMsg, gettext("Error"), MB_ICONEXCLAMATION | MB_OK);
#else
            fprintf(stderr, "%s", errorMsg);
#endif
            if (errorMsg)
            {
                FREE(errorMsg);
                errorMsg = NULL;
            }
        }
        else
        {
            bOK = TRUE;
        }
    }

    if (sciPath)
    {
        FREE(sciPath);
        sciPath = NULL;
    }

    if (!bOK)
    {
        exit(1);
    }

    return TRUE;
}
/*--------------------------------------------------------------------------*/
static void DoLoadClasspathInEtc(char *sciPath)
{
    char *classpathfile = (char*)MALLOC(sizeof(char) * (strlen(sciPath) + strlen(XMLCLASSPATH) + 1));
    sprintf(classpathfile, XMLCLASSPATH, sciPath);
    LoadClasspath(classpathfile);
    if (classpathfile)
    {
        FREE(classpathfile);
        classpathfile = NULL;
    }
}
/*--------------------------------------------------------------------------*/
static void DoLoadLibrarypathInEtc(char *sciPath)
{
    char *librarypathfile = (char*)MALLOC(sizeof(char) * (strlen(sciPath) + strlen(XMLLIBRARYPATH) + 1));
    sprintf(librarypathfile, XMLLIBRARYPATH, sciPath);
    LoadLibrarypath(librarypathfile);
    if (librarypathfile)
    {
        FREE(librarypathfile);
        librarypathfile = NULL;
    }
}
/*--------------------------------------------------------------------------*/
BOOL ExecuteInitialHooks(void)
{
    JNIEnv * currentENV = getScilabJNIEnv();
    JavaVM * currentJVM = getScilabJavaVM();

    jint result = (*currentJVM)->AttachCurrentThread(currentJVM, (void **) &currentENV, NULL) ;
    if (result == 0)
    {
        jclass cls = NULL;
        cls = (*currentENV)->FindClass(currentENV, "org/scilab/modules/core/Scilab");
        catchIfJavaException(_("Could not access to the Main Scilab Class:\n"));
        if (cls)
        {
            jmethodID mid = NULL;
            mid = (*currentENV)->GetStaticMethodID(currentENV, cls, "executeInitialHooks", "()V");
            if (mid)
            {
                (*currentENV)->CallStaticVoidMethod(currentENV, cls, mid);
            }
            catchIfJavaException(_("Cannot execute initial hooks. Error:\n"));

            return TRUE;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/


