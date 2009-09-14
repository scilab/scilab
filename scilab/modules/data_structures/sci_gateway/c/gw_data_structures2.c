/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_data_structures2.h"
#include "callFunctionFromGateway.h"
#include "stack-c.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
	{C2F(sci_getfield),"getfield"},
	{C2F(sci_setfield),"setfield"}
};
/*--------------------------------------------------------------------------*/
int gw_data_structures2(void)
{
	Rhs=Max(Rhs,0);
	if (Top - Rhs + Lhs + 1 >= Bot)
	{
		static int codeerror = 18;
		Error(codeerror);
		return 0;
	}

	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
