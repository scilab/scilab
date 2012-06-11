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
/*--------------------------------------------------------------------------*/
JVM_IMPEXP int gw_jvm(void)
{
    Scierror(999, _("Scilab Java module not installed.\n"));
    return 0;
}

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
JVM_IMPEXP BOOL canCloseMainScilabObject(void)
{
    return TRUE;
}

/*--------------------------------------------------------------------------*/
/* BUG 10325: FORCE EXPORT forceCloseMainScilabObject on Windows */
JVM_IMPEXP void forceCloseMainScilabObject(void)
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
