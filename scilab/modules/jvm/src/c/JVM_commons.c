/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#include <stdio.h>
#include "JVM_commons.h"
#include "dynamiclibrary.h"
#include "localization.h"
#include "getshortpathname.h"
#include "BOOL.h"
#include "sci_malloc.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
static DynLibHandle hLibJVM = NULL;
static BOOL bSymbolsLoaded = FALSE;
/*--------------------------------------------------------------------------*/
typedef jint (JNICALL *JNI_CreateJavaVMPROC) (JavaVM **jvm, JNIEnv **penv, JavaVMInitArgs *args);
typedef jint (JNICALL *JNI_GetCreatedJavaVMsPROC)(JavaVM **vmBuf, jsize BufLen, jsize *nVMs);
typedef jint (JNICALL *JNI_GetDefaultJavaVMInitArgsPROC)(JavaVMInitArgs *args);
/*--------------------------------------------------------------------------*/
static JNI_GetDefaultJavaVMInitArgsPROC ptr_JNI_GetDefaultJavaVMInitArgs = NULL;
static JNI_CreateJavaVMPROC ptr_JNI_CreateJavaVM  = NULL;
static JNI_GetCreatedJavaVMsPROC ptr_JNI_GetCreatedJavaVMs = NULL;
/*--------------------------------------------------------------------------*/
jint SciJNI_GetDefaultJavaVMInitArgs(JavaVMInitArgs *args)
{
    if (ptr_JNI_GetDefaultJavaVMInitArgs)
    {
        return (ptr_JNI_GetDefaultJavaVMInitArgs)(args);
    }
    return JNI_ERR;
}
/*--------------------------------------------------------------------------*/
jint SciJNI_CreateJavaVM(JavaVM **jvm, JNIEnv **penv, JavaVMInitArgs *args)
{
    if (ptr_JNI_CreateJavaVM)
    {
        return (ptr_JNI_CreateJavaVM)(jvm, penv, args);
    }
    return JNI_ERR;
}
/*--------------------------------------------------------------------------*/
jint SciJNI_GetCreatedJavaVMs(JavaVM **vmBuf, jsize BufLen, jsize *nVMs)
{
    if (ptr_JNI_GetCreatedJavaVMs)
    {
        return (ptr_JNI_GetCreatedJavaVMs)(vmBuf, BufLen, nVMs);
    }
    return JNI_ERR;
}
/*--------------------------------------------------------------------------*/
BOOL FreeDynLibJVM(void)
{
    if (hLibJVM)
    {
        if (FreeDynLibrary(hLibJVM))
        {
            ptr_JNI_GetDefaultJavaVMInitArgs = NULL;
            ptr_JNI_CreateJavaVM = NULL;
            ptr_JNI_GetCreatedJavaVMs = NULL;
            hLibJVM = NULL;
            bSymbolsLoaded = FALSE;
            return TRUE;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL LoadFunctionsJVM(char *filedynlib)
{
#ifdef _MSC_VER
    if (filedynlib == NULL)
    {
        hLibJVM = LoadDynLibraryW(L"jvm.dll");
    }
    else
    {
        wchar_t * wcfiledynlib = to_wide_string(filedynlib);
        if (wcfiledynlib)
        {
            hLibJVM = LoadDynLibraryW(wcfiledynlib);
            FREE(wcfiledynlib);
            wcfiledynlib = NULL;
        }
    }
#else
#ifdef __APPLE__
    /*
    ** After MacOSX 10.6.8 manually load libjava.jnilib make JNI_* functions crash
    ** Rely on OS by using dlopen(NULL) to find correct symbol with dlsym
    */
    hLibJVM = LoadDynLibrary(NULL);
#else
    if (filedynlib == NULL)
    {
        hLibJVM = LoadDynLibrary(NULL);
    }
    else
    {
        hLibJVM = LoadDynLibrary(filedynlib);
    }
#endif
#endif

    if (hLibJVM)
    {
        ptr_JNI_GetDefaultJavaVMInitArgs = (JNI_GetDefaultJavaVMInitArgsPROC) GetDynLibFuncPtr(hLibJVM, "JNI_GetDefaultJavaVMInitArgs" );
        ptr_JNI_CreateJavaVM = (JNI_CreateJavaVMPROC) GetDynLibFuncPtr(hLibJVM, "JNI_CreateJavaVM" );
        ptr_JNI_GetCreatedJavaVMs = (JNI_GetCreatedJavaVMsPROC) GetDynLibFuncPtr(hLibJVM, "JNI_GetCreatedJavaVMs" );

        if (ptr_JNI_GetDefaultJavaVMInitArgs && ptr_JNI_CreateJavaVM && ptr_JNI_GetCreatedJavaVMs)
        {
            bSymbolsLoaded = TRUE;
            return TRUE;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
char *getJniErrorFromStatusCode(long status)
{
    switch (status)
    {
        case JNI_ERR:
            return _("Unknown JNI error");
            break;
        case JNI_EDETACHED:
            return _("Thread detached from the VM");
            break;
        case JNI_EVERSION:
            return _("JNI version error");
            break;
#ifdef JNI_ENOMEM
        /* ifdef because not defined with some version of gcj */
        case JNI_ENOMEM:
            return _("JNI: not enough memory");
            break;
#endif
#ifdef JNI_EEXIST
        /* ifdef because not defined with some version of gcj */
        case JNI_EEXIST:
            return _("VM already created");
            break;
#endif
#ifdef JNI_EINVAL
        /* ifdef because not defined with some version of gcj */
        case JNI_EINVAL:
            return _("JNI: invalid arguments");
            break;
#endif
        default:
            return _("Undefined error code in the JNI. Weird problem");
            break;
    }
}
/*--------------------------------------------------------------------------*/
BOOL hasJvmSymbolsLoaded(void)
{
    return bSymbolsLoaded;
}
/*--------------------------------------------------------------------------*/