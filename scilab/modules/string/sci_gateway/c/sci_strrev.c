
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*----------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "gw_string.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "stringsstrrev.h"
/*----------------------------------------------------------------------------*/
int C2F(sci_strrev)(char *fname,unsigned long fname_len)
{
	CheckRhs(1,1);
	CheckLhs(0,1);

	if (GetType(1) == sci_strings)
	{
		int m1 = 0; int n1 = 0;
		char **InputStrings = NULL;
		int m1n1 = 0; /* m1 * n1 */

		char **OutputStrings = NULL;

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&InputStrings);
		m1n1 = m1 * n1;

		OutputStrings = strings_strrev(InputStrings,m1n1);

		if (OutputStrings)
		{
			CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,OutputStrings);
			LhsVar(1) = Rhs+1 ;
			C2F(putlhsvar)();
			freeArrayOfString(InputStrings,m1n1);
			freeArrayOfString(OutputStrings,m1n1);
		}
		else
		{
			freeArrayOfString(InputStrings,m1n1);
			Scierror(999,_("%s: No more memory.\n"),fname);
			return 0;
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"),fname,1);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
