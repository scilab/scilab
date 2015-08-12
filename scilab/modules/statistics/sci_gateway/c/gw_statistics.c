/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_statistics.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
    {cdfbetI, "cdfbet"},
    {cdfbinI, "cdfbin"},
    {cdfchiI, "cdfchi"},
    {cdfchnI, "cdfchn"},
    {cdffI, "cdff"},
    {cdffncI, "cdffnc"},
    {cdfgamI, "cdfgam"},
    {cdfnbnI, "cdfnbn"},
    {cdfnorI, "cdfnor"},
    {cdfpoiI, "cdfpoi"},
    {cdftI, "cdft"}
};
/*--------------------------------------------------------------------------*/
int gw_statistics()
{
    Rhs = Max(0, Rhs);

    if (pvApiCtx == NULL)
    {
        pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
    }

    pvApiCtx->pstName = (char*)Tab[Fin - 1].name;
    callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    return 0;
}
/*--------------------------------------------------------------------------*/
