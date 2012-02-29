/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <math.h>
/*--------------------------------------------------------------------------*/
#include "gw_special_functions.h"
#include "callFunctionFromGateway.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "xerhlt.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] = {
    {sci_legendre, "legendre"},
    {sci_beta, "beta"},
    {sci_besseli, "besseli"},
    {sci_besselj, "besselj"},
    {sci_besselk, "besselk"},
    {sci_bessely, "bessely"},
    {sci_besselh, "besselh"},
    {sci_gamma,"gamma"},
    {sci_lgamma,"gammaln"},
    {sci_dlgamma,"dlgamma"},
    {sci_calerf,"calerf"}
};
/*--------------------------------------------------------------------------*/
int gw_special_functions(void)
{
    Rhs = Max(0, Rhs);
    if (setjmp_slatec_jmp_env())
    {
        Scierror(999,"%s: Wrong value for input argument: Positive expected.\n", Tab[Fin-1].name);
        return 0;
    }

    if(pvApiCtx == NULL)
	{
		pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
	}

	pvApiCtx->pstName = (char*)Tab[Fin-1].name;
    callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    return 0;
}
/*--------------------------------------------------------------------------*/
