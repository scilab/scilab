/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "scilines.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
#include "gw_shell.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_lines)(char *fname,unsigned long fname_len)
{
	CheckRhs(0,2);
	CheckLhs(1,1);

	if (Rhs ==  0)
	{
		int n1 = 0,m1 = 0;
		int *paramoutINT = NULL;
		paramoutINT=(int*)MALLOC(sizeof(int)*2);

		paramoutINT[0] = getColumnsSize();
		paramoutINT[1] = getLinesSize();

		n1=1; m1=2;
		CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &n1, &m1, &paramoutINT);

		LhsVar(1) = Rhs+1;
		C2F(putlhsvar)();

		if (paramoutINT) { FREE(paramoutINT); paramoutINT = NULL;}
	}
	else
	{
		if (Rhs == 2)
		{
			if ( GetType(2) == sci_matrix )
			{
				int l2 = 0,n2 = 0, m2 = 0;

				GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);
				setColumnsSize((int)*stk(l2));
			}
			else
			{
				Scierror(999,_("%s: Wrong type for input argument #%d: Matrix expected.\n"),fname,2);
				return 0;
			}
		}

		if ( GetType(1) == sci_matrix )
		{
			int l1 = 0,n1 = 0, m1 = 0;

			GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
			setLinesSize((int)*stk(l1));
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Matrix expected.\n"),fname,1);
			return 0;
		}

		LhsVar(1) = 0;
		C2F(putlhsvar)();
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
