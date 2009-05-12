
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "gw_functions.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
#include "recursionFunction.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
	{C2F(sci_lib),"lib"},
	{C2F(sci_comp),"comp"},
	{C2F(sci_deff),"deff"}
};
/*--------------------------------------------------------------------------*/
int gw_functions(void)
{  
	Rhs = Max(0, Rhs);

	/**
	* recursion from sci_deff
	*/
	if ( isRecursionCallToFunction() )
	{
		if (getRecursionGatewayToCall() == GW_FUNCTIONS_ID )
		{
			/* We call "comp" */
			Fin = 2;
		}
		else
		{
			switch ( getRecursionFunctionToCall() )
			{
				case RECURSION_CALL_DEFF:
					#define deff_fname "deff"
					C2F(sci_deff)(deff_fname,strlen(deff_fname));
					return 0;
				default:
					return 0;
			}
		}
	}
	
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/
