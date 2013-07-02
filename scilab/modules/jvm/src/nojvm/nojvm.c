/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2010 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "Scierror.h"
#include "gw_jvm.h"
#include "dynlib_jvm.h"
#include "InitializeJVM.h"
#include "TerminateJVM.h"
#include "loadBackGroundClassPath.h"
#include "loadOnUseClassPath.h"
#include "localization.h"

#ifdef _MSC_VER
#include "getScilabJNIEnv.h"
#include "getScilabJavaVM.h"
#include "getScilabObject.h"
#include "addToClasspath.h"
#include "addToLibrarypath.h"
#include "catchIfJavaException.h"
#endif

/*--------------------------------------------------------------------------*/
JVM_IMPEXP int gw_jvm(void)
{
    Scierror(999, _("Scilab Java module not installed.\n"));
    return 0;
}

#ifdef _MSC_VER
JNIEnv *getScilabJNIEnv(void)
{
    return NULL;
}

JavaVM *getScilabJavaVM(void)
{
    return NULL;
}

jobject getScilabObject(void)
{
    return NULL;
}

BOOL addToClasspath(char *classpathstring, typeOfLoad load)
{
    return FALSE;
}

BOOL addToLibrarypath(char *librarypathstring)
{
    return FALSE;
}

BOOL catchIfJavaException(char *errorMsg)
{
    return FALSE;
}
#endif
/*--------------------------------------------------------------------------*/
JVM_IMPEXP BOOL InitializeJVM(void)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
JVM_IMPEXP BOOL TerminateJVM(void)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
JVM_IMPEXP BOOL loadBackGroundClassPath(void)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
JVM_IMPEXP BOOL loadOnUseClassPath(char const* tag)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
/* BUG 10325: FORCE EXPORT canCloseMainScilabObject on Windows */
BOOL canCloseMainScilabObject(void)
{
    return TRUE;
}

/*--------------------------------------------------------------------------*/
/* BUG 10325: FORCE EXPORT forceCloseMainScilabObject on Windows */
void forceCloseMainScilabObject(void)
{
}

/*--------------------------------------------------------------------------*/
JVM_IMPEXP BOOL ExecuteInitialHooks(void)
{
    return TRUE;
}

/*--------------------------------------------------------------------------*/
JVM_IMPEXP BOOL isItTheDisabledLib(void)
{
    return TRUE;
}

/*--------------------------------------------------------------------------*/
