/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_dynamic_link.h"
#include "stack-c.h"
#include "localization.h"
#include "Scierror.h"
#include "dynamic_link.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_ulink)(char *fname,unsigned long fname_len)
{
	static int l1,n1,m1;

	Rhs = Max(0, Rhs);
	CheckRhs(0,1);
	CheckLhs(1,1);

	if (Rhs == 0)
	{
		unlinkallsharedlib();
	}
	else
	{
		if ( GetType(1) == sci_matrix )
		{
			double *params=NULL;
			int i=0;
			GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
			params=stk(l1);
			for(i=0;i<m1*n1;i++)
			{
				int ilib = (int) params[i];
				unlinksharedlib(&ilib);
			}
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Integer expected.\n"), fname,1);
			return 0;
		}
	}

	LhsVar(1)=0;
	C2F(putlhsvar)();

	return 0;

}
/*--------------------------------------------------------------------------*/
