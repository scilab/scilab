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
#include "with_module.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_with_module)(char *fname,unsigned long fname_len)
{
	static int l1,n1,m1;
	int *Status=NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	if (GetType(1) == sci_strings)
	{
		char *VarName=NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		VarName=cstk(l1);

		Status=(int*)MALLOC(sizeof(int));
		*Status=with_module(VarName);

		m1=1;n1=1;
		CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &Status);
		LhsVar(1)=Rhs+1;

		if (Status) {FREE(Status);Status=NULL;}

        PutLhsVar();
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"), fname,1);
		return 0;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/


