/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "gw_cacsd1.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
	{C2F(scilyap),"lyap"},
	{C2F(scisylv),"sylv"},
	{C2F(intricc),"ricc"}
};
/*--------------------------------------------------------------------------*/
int gw_cacsd1(void)
{  
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
