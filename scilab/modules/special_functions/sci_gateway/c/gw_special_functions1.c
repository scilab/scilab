/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
/*--------------------------------------------------------------------------*/
#include "gw_special_functions1.h"
#include "callFunctionFromGateway.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] = 
{ 
    {sci_oldbesseli,"oldbesseli"},
    {sci_oldbesselj, "oldbesselj"},
    {sci_oldbesselk,"oldbesselk"},
    {sci_oldbessely,"oldbessely"},
    {sci_gamma,"gamma"},
    {sci_lgamma,"gammaln"},
    {sci_dlgamma,"dlgamma"},
    {sci_calerf,"calerf"}
};
/*--------------------------------------------------------------------------*/
int gw_special_functions1(void)
{
    Rhs = Max(0, Rhs);

    callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    return 0;
}
/*--------------------------------------------------------------------------*/
