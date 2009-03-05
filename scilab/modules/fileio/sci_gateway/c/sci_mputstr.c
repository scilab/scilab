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
#include "mputstr.h"
/*--------------------------------------------------------------------------*/
#define ALL_FILES_DESCRIPTOR -1
/*--------------------------------------------------------------------------*/
int sci_mputstr(char *fname,unsigned long fname_len)
{
	int m1 = 0, n1 = 0, l1 = 0;
	int	m2 = 0, n2 = 0, l2 = 0;
	int	l3 = 0;
	int	err = 0;
	int one = 1;
	int fd = ALL_FILES_DESCRIPTOR;

	Nbvars = 0 ;
	CheckRhs(1,2);
	CheckLhs(1,1);

	/* @TODO Add check about input type */

	/*  checking variable file */
	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);

	if ( Rhs >= 2)
	{
		GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
		fd = *istk(l2);
	}

	CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &one,&one, &l3);

	C2F(mputstr)(&fd,cstk(l1),stk(l3),&err);

	if (err >  0)
	{
		Error(10000);
		return 0;
	}

	LhsVar(1) = Rhs+1;
	PutLhsVar();

	return 0;
}
/*--------------------------------------------------------------------------*/
