
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
#include "gw_cacsd3.h"
#include "callFunctionFromGateway.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
	{C2F(sci_arl2),"arl2_ius"},
	{C2F(sci_residu),"residu"},
	{C2F(sci_ldiv),"ldiv"}
};
/*--------------------------------------------------------------------------*/
int gw_cacsd3(void)
{  
	if ( Fin-1 > 2) return 0;
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
