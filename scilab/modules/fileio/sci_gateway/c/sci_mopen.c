/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* ...
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "stack-c.h"
#include "gw_fileio.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "PATH_MAX.h"
#include "expandPathVariable.h"
#include "mopen.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_mopen(char *fname,unsigned long fname_len)
{
	int m1 = 0, n1 = 0, l1 = 0;
	int m2 = 0, n2 = 0, l2 = 0;
	int m3 = 0, n3 = 0, l3 = 0;
	int l4 = 0, l5 = 0, err = 3;
	int swap = 1, one = 1;
	char *status = NULL;
	char *filename = NULL;

	Nbvars = 0;
	CheckRhs(1,3);
	CheckLhs(1,2);

	if (GetType(1) != sci_strings)
	{
		Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname,1);
		return 0;
	}

	/*  checking variable file */
	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);

	if ( Rhs >= 2)
	{
		if (GetType(2) != sci_strings)
		{
			Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname,2);
			return 0;
		}

		GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
		status = cstk(l2);
	}
	else
	{
		status = "rb";
	}

	if ( Rhs >= 3)
	{
		if (GetType(3) == sci_matrix)
		{
			GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&m3,&n3,&l3);
			if (m3 * n3 == 1)
			{
				swap = *istk(l3);
			}
			else
			{
				Scierror(999, _("%s: Wrong size for input argument #%d: A integer expected.\n"), fname,3);
				return 0;
			}
		}
		else
		{
			Scierror(999, _("%s: Wrong type for input argument #%d: A integer expected.\n"), fname,3);
			return 0;
		}
	}

	CreateVar(Rhs+1, MATRIX_OF_INTEGER_DATATYPE, &one,&one, &l4);
	CreateVar(Rhs+2, MATRIX_OF_DOUBLE_DATATYPE, &one,&one, &l5);

	filename = expandPathVariable(cstk(l1));
	if (filename)
	{
		C2F(mopen)(istk(l4),filename,status,&swap,stk(l5),&err);
	}

	if (err > (int)MOPEN_NO_ERROR)
	{
		if ( Lhs == 1)
		{
			switch (err)
			{
				case (int)MOPEN_NO_MORE_LOGICAL_UNIT : 
				{
					Error(66);/* no more logical units */
					FREE(filename);
					filename = NULL;
					return 0;
				}
				case (int)MOPEN_CAN_NOT_OPEN_FILE:
				{
					Scierror(999,_("%s: Cannot open file %s.\n"),fname,filename);
					FREE(filename);
					filename = NULL;
					return 0;
				}
				case (int)MOPEN_NO_MORE_MEMORY:
				{
					FREE(filename);
					filename = NULL;
					Scierror(999,_("%s: No more memory.\n"),fname);
					return 0;
				}
				case (int)MOPEN_INVALID_FILENAME:
				{
					FREE(filename);
					filename = NULL;
					Scierror(999,_("%s: invalid filename.\n"),fname);
					return 0;
				}
				case (int)MOPEN_INVALID_STATUS:
				{
					FREE(filename);
					filename = NULL;
					Scierror(999,_("%s: invalid status.\n"),fname);
					return 0;
				}
			}
		}
		else
		{
			*stk(l5) = - err;
		}
	}

	if (filename)
	{
		FREE(filename);
		filename = NULL;
	}

	LhsVar(1) = Rhs + 1;
	LhsVar(2) = Rhs + 2;
	PutLhsVar();

	return 0;
}
/*--------------------------------------------------------------------------*/
