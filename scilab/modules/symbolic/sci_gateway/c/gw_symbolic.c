/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include "gw_symbolic.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
{sci_addf,"addf"},
{sci_subf,"subf"},
{sci_mulf,"mulf"},
{sci_ldivf,"ldivf"},
{sci_rdivf,"rdivf"}
};
/*--------------------------------------------------------------------------*/
int gw_symbolic(void)
{
	if (Rhs != 2)
	{
		Scierror(39,_("%s: Wrong number of input arguments: %d expected.\n"),Tab[Fin-1].name, 2);
		return 0;
	}
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
