/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_io.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "callFunctionFromGateway.h"
#include "recursionFunction.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intsave)(); /* fortran subroutine */
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
    {sci_setenv, "setenv"},
    {sci_read, "read"},
    {sci_getenv, "getenv"},
    {sci_getio, "getio"},
    {NULL, ""},
    {NULL, ""},
    {sci_write, "write"},
    {NULL, ""},
    {sci_file, "file"},
    {sci_host, "host"},
    {sci_unix, "unix"},
    {sci_readb, "readb"},
    {sci_writb, "writb"},
    {sci_getpid, "getpid"},
    {sci_read4b, "read4b"},
    {sci_write4b, "write4b"},
    {sci_save, "save"},
    {sci_load, "load"},
    {sci_percent_load, "%_load"}
};
/*--------------------------------------------------------------------------*/
int gw_io(void)
{
    /* Recursion from a function */
    if (pvApiCtx == NULL)
    {
        pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
    }

    if ( isRecursionCallToFunction() )
    {
        switch ( getRecursionFunctionToCall() )
        {
            case RECURSION_CALL_SAVE:
            {
                pvApiCtx->pstName = "save";
                C2F(intsave)();
                return 0;
            }
            break;
            case RECURSION_CALL_LOAD:
            {
                pvApiCtx->pstName = "load";
                sci_load("load", (unsigned long)strlen("load"));
                return 0;
            }
            break;
            default:
                break;
        }
    }
    else
    {
        Rhs = Max(0, Rhs);
        pvApiCtx->pstName = (char*)Tab[Fin - 1].name;
        callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
