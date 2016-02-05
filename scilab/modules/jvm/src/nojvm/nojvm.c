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
#include "Scierror.h"
#include "gw_jvm.h"
#include "localization.h"

#ifdef _MSC_VER
#include "getScilabObject.h"
#include "addToClasspath.h"
#endif

/*--------------------------------------------------------------------------*/
int gw_jvm(void)
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
BOOL InitializeJVM(void)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
BOOL TerminateJVM(void)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
BOOL loadBackGroundClassPath(void)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
BOOL loadOnUseClassPath(char const* tag)
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
BOOL ExecuteInitialHooks(void)
{
    return TRUE;
}

/*--------------------------------------------------------------------------*/
BOOL isItTheDisabledLib(void)
{
    return TRUE;
}

/*--------------------------------------------------------------------------*/
