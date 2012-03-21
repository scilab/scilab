
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
#include "api_scilab.h"
#include "MALLOC.h"
#include "callFunctionFromGateway.h"
#include "recursionFunction.h"
/*--------------------------------------------------------------------------*/
static int sci_getf(char *fname,unsigned long fname_len)
{
	// Because we do not want change order in gateway
	// empty function
	return 0;
}
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
	{C2F(sci_lib),"lib"},
	{C2F(sci_deff),"deff"},
	{sci_getf,"getf"},
	{C2F(sci_exec),"exec"},
	{C2F(sci_execstr),"execstr"},
	{sci_librarieslist,"librarieslist"},
	{sci_libraryinfo,"libraryinfo"},
	{sci_whereis,"whereis"}
};
/*--------------------------------------------------------------------------*/
int gw_functions(void)
{  
	Rhs = Max(0, Rhs);

	if ( isRecursionCallToFunction() )
	{
		switch ( getRecursionFunctionToCall() )
		{
			case RECURSION_CALL_DEFF:
				#define deff_fname "deff"
				C2F(sci_deff)(deff_fname,(unsigned long)strlen(deff_fname));
				return 0;

			case RECURSION_CALL_EXEC1: case RECURSION_CALL_EXEC2:
				#define exec_fname "exec"
				C2F(sci_exec)(exec_fname,(unsigned long)strlen(exec_fname));
				return 0;

			case RECURSION_CALL_EXECSTR:
				#define execstr_fname "execstr"
				C2F(sci_execstr)(execstr_fname,(unsigned long)strlen(execstr_fname));
				return 0;

			default:
				return 0;
		}
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
