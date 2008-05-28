/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#include "gw_fileio.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "deleteafile.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_deletefile)(char *fname,unsigned long fname_len)
{
	CheckRhs(1,1);
	CheckLhs(1,1);

	if (GetType(1) == sci_strings)
	{
		int m1,n1,l1;
		char *VarName=NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		VarName=cstk(l1);

		n1=1;
		if ( deleteafile(VarName) )
		{
			CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
			*istk(l1)=(int)(TRUE);
		}
		else
		{
			CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
			*istk(l1)=(int)(FALSE);
		}

		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument: A string expected.\n"),fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
