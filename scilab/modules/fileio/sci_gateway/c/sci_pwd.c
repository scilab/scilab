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
#include "gw_fileio.h"
#include "stack-c.h"
#include "scicurdir.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_pwd(char *fname,unsigned long fname_len)
{
	int ierr = 0;
	int lpath = 0;
	char *path = NULL;

	Rhs = Max(Rhs,0);

	CheckRhs(0,0);
	CheckLhs(0,1);

	scigetcwd(&path,&lpath,&ierr);

	if (ierr)
	{
		Scierror(998,_("%s: An error occurred.\n"), fname);
		return 0;
	}
	else
	{
		int n1 = 1;
		int m1 = (int)strlen(path);

		n1 = 1;
		CreateVarFromPtr(Rhs+1, STRING_DATATYPE, &m1, &n1, &path);
		LhsVar(1) = Rhs+1;
		C2F(putlhsvar)();
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
