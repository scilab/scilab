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
#include "gw_core.h"
#include "stack-c.h"
#include "localization.h"
#include "hashtable_core.h"
#include "Scierror.h"
#include "Funtab.h"
#include "stack-def.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_funptr) (char *fname,unsigned long fname_len)
{
	CheckRhs(1,1);
	CheckLhs(1,1);

	if ( GetType(1) == sci_strings )
	{
		char *VarName = NULL;
		int m1 = 0, n1 = 0, l1 = 0;
		int id[nsiz];
		int funptr = 0;
		int zero = 0;
		int one = 1;
		int job = SCI_HFUNCTIONS_FIND; /* Find function & returns fptr value */

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		VarName=cstk(l1);

		C2F(cvname)(id,VarName,&zero,(unsigned long)strlen(VarName));
		C2F(funtab)(id,&funptr,&job,"NULL_NAME",0);

		CreateVar(Rhs+1,MATRIX_OF_INTEGER_DATATYPE,&one,&one,&l1);
		*istk(l1) = funptr;

		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname,1);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
