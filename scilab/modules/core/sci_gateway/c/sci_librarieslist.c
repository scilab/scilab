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
#include "gw_core.h"
#include "stack-c.h"
#include "librarieslist.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_librarieslist)(char *fname,unsigned long fname_len)
{
	char **libraries = NULL;
	int sizelibraries = 0;

	CheckRhs(0,0);
	CheckLhs(0,1);

	libraries = getlibrarieslist(&sizelibraries);
	
	if (libraries)
	{
		int m = 0, n = 0;
		m = sizelibraries; n = 1;
		CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &m, &n, libraries);
	}
	else
	{
		int m = 0, n = 0, l = 0;
		CreateVarFromPtr(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&n,&m,&l);
	}
	LhsVar(1) = Rhs+1;

	if (libraries)
	{
		int i = 0;
		for (i=0;i < sizelibraries;i++) 
		{ 
			if (libraries[i])
			{
				FREE(libraries[i]);
				libraries[i]=NULL; 
			}
		}
		FREE(libraries);
		libraries = NULL; 
	}
	
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
