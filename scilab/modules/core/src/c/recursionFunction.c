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
/*--------------------------------------------------------------------------*/
#include "recursionFunction.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
/**
* PLEASE DO NOT USE RECURSION CALL TO A FUNCTION.
* ONLY FOR COMPATIBILITY
* see also callinter.h (fortran part)
*/
/*--------------------------------------------------------------------------*/
BOOL isRecursionCallToFunction(void)
{
	return (BOOL)(C2F(recu).rstk[C2F(recu).pt-1] / 100 == 9);
}
/*--------------------------------------------------------------------------*/
int getRecursionGatewayToCall(void)
{
	int gw = ERROR_GW_ID;

	if ( isRecursionCallToFunction() )
	{
		int ir = C2F(recu).rstk[C2F(recu).pt-1] - 900;
		if (ir == 1)
		{
			gw = GW_CORE_ID; /* gw_core */
		}
		else if ( (ir > 2) && (ir < 9) )
		{
			gw = GW_IO_ID; /* gw_io */
		}
		else if (ir > 20)
		{
			gw = GW_USER_ID; /* gw_user */
		}
		else if (ir > 40)
		{
			gw = GW_USER2_ID; /* gw_user2 */
		}
		else
		{
			/* overloaded function  */
		}
	}
	return gw;
}
/*--------------------------------------------------------------------------*/
recursion_function_called getRecursionFunctionToCall(void)
{
	int function_number = RECURSION_ERROR;

	if (isRecursionCallToFunction())
	{
		int ir = (int)(C2F(recu).rstk[C2F(recu).pt-1] - 901);
		if ( (ir > 0) || (ir < 9) )
		{
			function_number = (recursion_function_called) ir;
		}
		else function_number = RECURSION_ERROR;
	}

	return function_number;
}
/*--------------------------------------------------------------------------*/
