/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#include "stack-c.h"
#include "gw_fileio.h"
#include "meof.h"
/*--------------------------------------------------------------------------*/
#define ALL_FILES_DESCRIPTOR -1
/*--------------------------------------------------------------------------*/
int sci_meof(char *fname,unsigned long fname_len)
{
	int m1 = 0, n1 = 0, l1 = 0;
	int one = 1;
	int lr = 0;
	int fd = ALL_FILES_DESCRIPTOR;

	Nbvars = 0;
	CheckRhs(0,1);
	CheckLhs(1,1);

	/* @TODO Add check about input type */

	if ( Rhs >= 1)
	{
		GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1);
		fd  = *istk(l1);
	}

	CreateVar(Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &lr);

	C2F(meof)(&fd,stk(lr));

	LhsVar(1)= Rhs+1;
	PutLhsVar();

	return 0;
}
/*--------------------------------------------------------------------------*/
