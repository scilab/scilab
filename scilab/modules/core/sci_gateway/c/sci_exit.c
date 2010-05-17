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
#include "gw_core.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "sciquit.h"
#include "localization.h"
#include "Scierror.h"

/*--------------------------------------------------------------------------*/
extern int C2F(compil)(int *, int *, int *, int *, int *);
/*--------------------------------------------------------------------------*/
int C2F(sci_exit)(char *fname,unsigned long fname_len)
{
	int twenty = 20;
	int zero = 0;
	int exitCode = 0;

	/* if special compilation mode skip commands */
	if ( C2F(com).comp[2] == 1 )
	{
		C2F(com).fin = 0;
		C2F(com).fun = 0;

		LhsVar(1) = 0;
		C2F(putlhsvar)();

		return 0;
	}

	/* compilation exit:<20> */
	if (C2F(compil)(&twenty, &zero, &zero, &zero, &zero)) 
	{
		return 0;
	}

	Rhs = Max(0, Rhs);
	CheckRhs(0,1);

	if (Rhs == 1)
	{
		if ( VarType(1) == sci_matrix )
		{
			int m1 = 0, n1 = 0, l1 = 0;
			GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
			if ( (m1 == n1) && (n1 == 1) )
			{
				double dexitcode = *stk(l1);
				exitCode = (int) dexitcode;

				if (dexitcode != (double)exitCode)
				{
					Scierror(999,_("%s: Wrong value for input argument #%d: A int expected.\n"),fname,1);
					return 0;
				}
			}
			else
			{
				Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,1);
				return 0;
			}
		}
	}

	LhsVar(1) = 0;
	C2F(putlhsvar)();

	ExitWithCodeFromScilab(exitCode);


	return 0;
}
/*--------------------------------------------------------------------------*/
