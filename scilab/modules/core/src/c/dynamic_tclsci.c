/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "dynamic_tclsci.h"
#include "callDynamicGateway.h"
#include "gw_dynamic_generic.h"
#include "scilabmode.h"
#include "with_module.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
/* tclsci module */
#define TCLSCI_MODULE_NAME "tclsci"
static DynLibHandle hTclsciLib = NULL;
static PROC_GATEWAY ptr_gw_tclsci = NULL;
static char* dynlibname_tclsci = NULL;
static char* gatewayname_tclsci = NULL;
/*--------------------------------------------------------------------------*/
#define SETENVTCL_NAME "setenvtcl"
typedef int (*PROC_SETENVTCL) (const char *, const char *);
static PROC_SETENVTCL ptr_setenvtcl = NULL;
/*--------------------------------------------------------------------------*/
#define TERMINATETCLTK_NAME "TerminateTclTk"
typedef BOOL (*PROC_TERMINATETCLTK) (void);
static PROC_TERMINATETCLTK ptr_TerminatTclTk = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_tclsci(void)
{
    return gw_dynamic_generic(TCLSCI_MODULE_NAME,
                              &dynlibname_tclsci,
                              &gatewayname_tclsci,
                              &hTclsciLib,
                              &ptr_gw_tclsci);
}
/*--------------------------------------------------------------------------*/
int dynamic_setenvtcl(const char *string, const char *value)
{
    if (hTclsciLib)
    {
        if (ptr_setenvtcl == NULL)
        {
            ptr_setenvtcl = (PROC_SETENVTCL) GetDynLibFuncPtr(hTclsciLib,
                            SETENVTCL_NAME);
            if (ptr_setenvtcl == NULL)
            {
                return 0;
            }
        }
        return (ptr_setenvtcl)(string , value);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
BOOL dynamic_TerminateTclTk(void)
{
    if (hTclsciLib)
    {
        BOOL bResult = FALSE;
        if (ptr_TerminatTclTk == NULL)
        {
            ptr_TerminatTclTk = (PROC_TERMINATETCLTK) GetDynLibFuncPtr(hTclsciLib,
                                TERMINATETCLTK_NAME);
            if (ptr_TerminatTclTk == NULL)
            {
                return FALSE;
            }
        }
        bResult = (ptr_TerminatTclTk)();

        freeDynamicGateway(&dynlibname_tclsci,
                           &gatewayname_tclsci,
                           &hTclsciLib,
                           &ptr_gw_tclsci);

        return bResult;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
