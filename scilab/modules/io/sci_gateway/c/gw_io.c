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
/*--------------------------------------------------------------------------*/
#include "gw_io.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "callFunctionFromGateway.h"
#include "recursionFunction.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
{C2F(sci_setenv),"setenv"},
{C2F(sci_read),"read"},
{C2F(sci_getenv),"getenv"},
{C2F(sci_getio),"getio"},
{NULL,""},
{C2F(sci_mgetl),"mgetl"},
{C2F(sci_write),"write"},
{C2F(sci_rat),"rat"},
{C2F(sci_file),"file"},
{C2F(sci_host),"host"},
{C2F(sci_unix),"unix"},
{C2F(sci_readb),"readb"},
{C2F(sci_writb),"writb"},
{C2F(sci_getpid),"getpid"},
{C2F(sci_read4b),"read4b"},
{C2F(sci_write4b),"write4b"},
{C2F(sci_save),"save"},
{C2F(sci_load),"load"}
};
/*--------------------------------------------------------------------------*/
int gw_io(void)
{  
	/* Recursion from a function */
	if(pvApiCtx == NULL)
	{
		pvApiCtx = (StrCtx*)MALLOC(sizeof(StrErr));
	}

	if ( isRecursionCallToFunction() )
	{
		switch ( getRecursionFunctionToCall() )
		{
			case RECURSION_CALL_SAVE:
				{
					pvApiCtx->pstName = "save";
					C2F(intsave)(); 
					return 0;
				}
				break;
			case RECURSION_CALL_LOAD:
				{
					pvApiCtx->pstName = "load";
					C2F(sci_load)("load",(unsigned long)strlen("load"));
					return 0;
				}
				break;
			default:
				break;
		}
	}
	else
	{
		Rhs = Max(0, Rhs);
		pvApiCtx->pstName = (char*)Tab[Fin-1].name;
		callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
