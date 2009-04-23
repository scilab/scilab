/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/ 
#include "gw_dynamic_link.h"
#include "stack-c.h"
#include "ilib_verbose.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_ilib_verbose(char *fname,unsigned long fname_len)
{
	Rhs = Max(0, Rhs);

	CheckRhs(0,1);
	CheckLhs(1,1);

	if (Rhs == 0)
	{
		int n = 1 ,l = 0;
		ilib_verbose_level currentLevel = getIlibVerboseLevel();
		
		CreateVar(Rhs+1, MATRIX_OF_INTEGER_DATATYPE, &n, &n,&l);
		*istk(l) = (int)currentLevel;
		LhsVar(1) = Rhs + 1;
	}
	else
	{
		if ( VarType(1)== sci_matrix )
		{
			int m1 = 0, n1 = 0, l1 = 0;
			GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
			if ( (m1 == n1) && (n1 == 1) )
			{
				double dlevel = (double)*stk(l1);
				int level = (int) dlevel;

				if (dlevel != (double)level)
				{
					Scierror(999,_("%s: Wrong value for input argument #%d: A int expected.\n"),fname,1);
					return 0;
				}

				if ( !setIlibVerboseLevel((ilib_verbose_level)level) )
				{
					Scierror(999,_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),fname,1,"0,1,2");
					return 0;
				}
				else
				{
					LhsVar(1) = 0;
				}
			}
			else
			{
				Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,1);
				return 0;
			}
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A int expected.\n"),fname,1);
			return 0;
		}
	}
	
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
