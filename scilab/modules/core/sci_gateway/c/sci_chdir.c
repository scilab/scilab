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
#include <stdio.h>
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "Scierror.h"
#include "scicurdir.h"
#include "localization.h"
#include "cluni0.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_chdir)(char *fname,unsigned long fname_len)
{
	Rhs = Max(0, Rhs);
	CheckRhs(0,1);
	CheckLhs(1,1);

	if ( (Rhs == 0) || (GetType(1) == sci_strings) )
	{
		int ierr = 0;
		static int l1,n1,m1;
		char shortpath[PATH_MAX];
		char path[PATH_MAX];
		int out_n = 0;

		if (Rhs == 0)
		{
			strcpy(shortpath,"home/");
		}
		else
		{
			GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
			strcpy(shortpath,cstk(l1));
		}

		C2F(cluni0)(shortpath,path,&out_n,(long)strlen(shortpath),PATH_MAX);
		C2F(scichdir)(path,&ierr);

		n1=1;
		CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);

		if (ierr == 0) *istk(l1)=(int)(TRUE);
		else *istk(l1)=(int)(FALSE);

		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname, 1);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
