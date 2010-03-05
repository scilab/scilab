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
#include "gw_output_stream.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "api_scilab.h"
#include "callFunctionFromGateway.h"
#include "recursionFunction.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{ 
	{sci_print, "print"},
	{sci_mprintf, "mprintf"},
	{sci_msprintf, "msprintf"},
	{NULL, ""}, //disp
	{NULL, ""} //diary
};
/*--------------------------------------------------------------------------*/
int gw_output_stream(void)
{
	Rhs = Max(0, Rhs);

	/* Recursion from disp */
	if ( isRecursionCallToFunction() )
	{
		if ( getRecursionFunctionToCall() == RECURSION_CALL_DISP)
		{
			#define disp_fname "disp"
//			sci_disp(disp_fname,(unsigned long)strlen(disp_fname));
			return 0;
		}
	}
	else
	{
		callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
