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
/* desc : interface for clc primitive                                       */
/*--------------------------------------------------------------------------*/
#include "gw_shell.h"
#include "stack-c.h"
#include "clc.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_clc)(char *fname,unsigned long fname_len)
{
	BOOL res = FALSE;
	int nblines = -1;

	Rhs = Max(Rhs,0);
	CheckRhs(0,1);
	CheckLhs(0,1);

	if (Rhs == 1)
	{
		if ( GetType(1) == sci_matrix )
		{
			int l1,m1=1,n1=1;
			GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
			nblines=*istk(l1);

			if (nblines < 0)
			{
				Scierror(999,_("%s: Wrong value for input argument #%d.\n"),fname,1);
				return 0;
			}
		}
		else
		{
			Scierror(999,_("%s: This feature has not been implemented.\n"),fname);
			return 0;
		}
	}
	else
	{
		/* Clear console full */
		nblines = -1;
	}

	res = clc(nblines);
	if (!res)
	{
		sciprint(_("%s: This feature has not been implemented in this mode.\n"),fname);
	}

	LhsVar(1) = 0;
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
