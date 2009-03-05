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
#include "mgetstr.h"
/*--------------------------------------------------------------------------*/
#define ALL_FILES_DESCRIPTOR -1
/*--------------------------------------------------------------------------*/
int sci_mgetstr(char *fname,unsigned long fname_len)
{
	int m1 = 0, n1 = 0, l1 = 0;
	int m2 = 0, n2 = 0, l2 = 0;
	int	l3 = 0;
	int err = 0;
	int fd = ALL_FILES_DESCRIPTOR;
	int n = 1;
	int one = 1;

	Nbvars = 0;
	CheckRhs(1,2);
	CheckLhs(1,1);

	/* @TODO Add check about input type */

	if ( Rhs >= 1)
	{
		GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
		n  = *istk(l1);
	}

	if ( Rhs >= 2)
	{
		GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
		fd = *istk(l2);
	}

	CreateVar(Rhs+1,STRING_DATATYPE,&n,&one,&l3);

	C2F(mgetstr1)(&fd, cstk(l3), &n, &err);

	LhsVar(1) = Rhs+1;
	if (err >  0)
	{
		Error(10000);
		return 0;
	}
	else if ( err < 0)
	{
		int n5 = 0, l5 = 0;
		/* n contains now the effectively read data */
		n5 = -err -1;
		if ( n5 < n )
		{
			CreateVar(Rhs+2,STRING_DATATYPE,&one,&n5,&l5);
			strcpy(cstk(l5),cstk(l3));
			LhsVar(1) = Rhs+2;
		}
	}

	PutLhsVar();

	return 0;
}
/*--------------------------------------------------------------------------*/
