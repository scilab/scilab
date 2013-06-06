/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_external_objects_java.h"
#include "api_scilab.h"
#include "callFunctionFromGateway.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
/*  interface function */
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
    {sci_jimport, "jimport"},
    {sci_jgetclassname, "jgetclassname"},
    {sci_jwrap, "jwrap"},
    {sci_jgetmethods, "jgetmethods"},
    {sci_jgetfields, "jgetfields"},
    {sci_jgetfield, "jgetfield"},
    {sci_jcompile, "jcompile"},
    {sci_jnewInstance, "jnewInstance"},
    {sci_jremove, "jremove"},
    {sci_jexists, "jexists"},
    {sci_junwrap, "junwrap"},
    {sci_junwraprem, "junwraprem"},
    {sci_jenableTrace, "jenableTrace"},
    {sci_jdisableTrace, "jdisableTrace"},
    {sci_jdeff, "jdeff"},
    {sci_jvoid, "jvoid"},
    {sci_jarray, "jarray"},
    {sci_jcast, "jcast"},
    {sci_jallowClassReloading, "jallowClassReloading"}
};

/*--------------------------------------------------------------------------*/
int gw_external_objects_java(void)
{
    Rhs = Max(0, Rhs);

    if (pvApiCtx == NULL)
    {
        pvApiCtx = (StrCtx *) MALLOC(sizeof(StrCtx));
    }

    pvApiCtx->pstName = (char *)Tab[Fin - 1].name;
    callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    return 0;
}

/*--------------------------------------------------------------------------*/
