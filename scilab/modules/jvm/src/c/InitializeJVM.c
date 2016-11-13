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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef _MSC_VER
#include <windows.h>
#endif
#include "InitializeJVM.h"
#include "loadClasspath.h"
#include "loadLibrarypath.h"
#include "sci_path.h"
#include "getScilabJNIEnv.h"
#include "getScilabJavaVM.h"
#include "sci_malloc.h"
#include "JVM.h"
#include "createMainScilabObject.h"
#include "scilabDefaults.h"
#include "localization.h"
#include "fromjava.h"
#include "os_string.h"
#include "catchIfJavaException.h"
/*--------------------------------------------------------------------------*/
static void DoLoadClasspathInEtc(char *sciPath);
static void DoLoadLibrarypathInEtc(char *sciPath);
/*--------------------------------------------------------------------------*/
BOOL InitializeJVM(void)
{
    BOOL bOK = FALSE;
    char *sciPath = NULL;

    sciPath = getSCI();

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
            char *errorMsg = os_strdup(gettext("\nScilab cannot create Scilab Java Main-Class (we have not been able to find the main Scilab class. Check if the Scilab and thirdparty packages are available).\n"));

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

    FREE(sciPath);
    sciPath = NULL;

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
    FREE(classpathfile);
    classpathfile = NULL;
}
/*--------------------------------------------------------------------------*/
static void DoLoadLibrarypathInEtc(char *sciPath)
{
    char *librarypathfile = (char*)MALLOC(sizeof(char) * (strlen(sciPath) + strlen(XMLLIBRARYPATH) + 1));
    sprintf(librarypathfile, XMLLIBRARYPATH, sciPath);
    LoadLibrarypath(librarypathfile);
    FREE(librarypathfile);
    librarypathfile = NULL;
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


