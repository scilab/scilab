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
#define Pt (C2F(recu).pt)
/*--------------------------------------------------------------------------*/
extern int C2F(getrstkpt)();
/*--------------------------------------------------------------------------*/
BOOL isRecursionCallToFunction(void)
{
	int *Rstk    = C2F(recu).rstk - 1;
	return (BOOL)( (int)(Rstk[Pt] / 100) == 9 );
}
/*--------------------------------------------------------------------------*/
int getRecursionGatewayToCall(void)
{
	int gw = ERROR_GW_ID;

	if ( isRecursionCallToFunction() )
	{
		int ir = C2F(recu).rstk[C2F(recu).pt-1] - 900;
		gw = C2F(getrecursiongatewaytocall)(&ir);
	}

	return gw;
}
/*--------------------------------------------------------------------------*/
recursion_function_called getRecursionFunctionToCall(void)
{
	int function_number = RECURSION_ERROR;

	if ( isRecursionCallToFunction() )
	{
		int ir = (int)(C2F(recu).rstk[C2F(recu).pt-1] - 900);

		if ( (ir > (int) RECURSION_CALL_EXEC2) || (ir < (int) RECURSION_CALL_COMP) )
		{
			function_number = RECURSION_ERROR;
		}
		else
		{
			function_number = (recursion_function_called) ir;
		}
	}
	return function_number;
}
/*--------------------------------------------------------------------------*/
int C2F(getrecursiongatewaytocall)(int *callID)
{
	int  gw = ERROR_GW_ID;

	switch (*callID)
	{
	case RECURSION_CALL_COMP:
		/* see comp (sci_comp.f) */
		gw = GW_CORE_ID; /* gw_core */
		break;
	case RECURSION_CALL_EXEC1:
		/* see exec (intexec.f) */
		gw = GW_FUNCTIONS_ID; /* gw_functions */
		break;
	case RECURSION_CALL_EXECSTR:
		/* see execstr (intexecstr.f) */
		gw = GW_FUNCTIONS_ID; /* gw_functions */
		break;
	case RECURSION_CALL_GETF:
		/* see getf (intgetf.f) */
		gw = GW_FUNCTIONS_ID; /* gw_functions */
		break;
	case RECURSION_CALL_SAVE:
		/* see save (newsave.f) */
		gw = GW_IO_ID; /* gw_io */
		break;
	case RECURSION_CALL_LOAD:
		/* see load (newsave.f) */
		gw = GW_IO_ID; /* gw_io */
		break;
	case RECURSION_CALL_DEFF:
		/* see deff (intdeff.f) */
		gw = GW_FUNCTIONS_ID; /* gw_functions */
		break;
	case RECURSION_CALL_DISP:
		/* see disp (intdisp.f) */
		gw = GW_OUTPUT_STREAM_ID; /* gw_output_stream */
		break;
	case RECURSION_CALL_EXEC2:
		/* see exec (intexec.f) */
		gw = GW_FUNCTIONS_ID; /* gw_functions */
		break;
	case RECURSION_OVERLOAD:
		/* overloaded function  */
		gw = END_OVERLOAD;
		break;
	default:
		{
			if (*callID > 20)
			{
				gw = GW_USER_ID; /* gw_user */
			}
			else if (*callID > 40)
			{
				gw = GW_USER2_ID; /* gw_user2 */
			}
		}
		break;
	}

	return gw; 
}
/*--------------------------------------------------------------------------*/
int C2F(isrecursioncalltofunction)(void)
{
	return isRecursionCallToFunction();
}
/*--------------------------------------------------------------------------*/
