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

#include "gw_io.h"
#include "stack-c.h"
#include "systemc.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_host)(char *fname,unsigned long fname_len)
{
	CheckRhs(1,1);
	CheckLhs(1,1);

	if (VarType(1) == sci_strings)
	{
		int m1 = 0, n1 = 0;
		char **Str=NULL;

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);

		if ( (m1 != 1) && (n1 != 1) )
		{
			freeArrayOfString(Str,m1*n1);
			Scierror(89,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
			return 0;
		}
		else
		{
			int stat = 0;
			int one = 1;
			int l1 = 0;

			C2F(systemc)(Str[0], &stat);
			CreateVar(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &one,&one, &l1);
			*istk(l1) = (int) stat;

			LhsVar(1)= Rhs+1;
			C2F(putlhsvar)();
			freeArrayOfString(Str,m1*n1);
		}
	}
	else
	{
		Scierror(55,_("%s: Wrong type for input argument #%d: String expected.\n"),fname,1);
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
