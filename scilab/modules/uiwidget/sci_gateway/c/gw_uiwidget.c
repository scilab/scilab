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
#include "gw_uiwidget.h"
#include "api_scilab.h"
#include "callFunctionFromGateway.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
/*  interface function */
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
    {sci_uiwidget, "uiwidget"},
    {sci_uiwidget, "%UIWidget_uicontrol"},
    {sci_uiwidget, "%c_uicontrol"},
    {sci_uiget, "uiget"},
    {sci_uiset, "uiset"},
    {sci_uiget, "%UIWidget_get"},
    {sci_uiset, "%UIWidget_set"},
    {sci_percent_UIWidget_e, "%UIWidget_e"},
    {sci_percent_foo_i_UIWidget, "%c_i_UIWidget"},
    {sci_percent_foo_i_UIWidget, "%s_i_UIWidget"},
    {sci_percent_foo_i_UIWidget, "%i_i_UIWidget"},
    {sci_percent_foo_i_UIWidget, "%mc_i_UIWidget"},
    {sci_percent_foo_i_UIWidget, "%sp_i_UIWidget"},
    {sci_percent_foo_i_UIWidget, "%b_i_UIWidget"},
    {sci_percent_foo_i_UIWidget, "%l_i_UIWidget"},
    {sci_percent_foo_i_UIWidget, "%st_i_UIWidget"}
};

/*--------------------------------------------------------------------------*/
int gw_uiwidget(void)
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
