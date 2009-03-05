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
#include <stdio.h>
#include "stack-c.h"
#include "gw_fileio.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "PATH_MAX.h"
#include "cluni0.h"
#include "mopen.h"
/*--------------------------------------------------------------------------*/
int sci_mopen(char *fname,unsigned long fname_len)
{
	int m1 = 0, n1 = 0, l1 = 0;
	int m2 = 0, n2 = 0, l2 = 0;
	int m3 = 0, n3 = 0, l3 = 0;
	int l4 = 0, l5 = 0, err = 0;
	int swap = 1, one = 1;
	char *status = NULL;
	char filename[PATH_MAX + FILENAME_MAX];
	int out_n = 0;
	long int lout = 0;

	Nbvars = 0;
	CheckRhs(1,3);
	CheckLhs(1,2);

	/* @TODO Add check about input type */

	/*  checking variable file */
	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);

	if ( Rhs >= 2)
	{
		GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
		status = cstk(l2);
	}
	else
	{
		status = "rb";
	}

	if ( Rhs >= 3)
	{
		GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&m3,&n3,&l3);
		swap = *istk(l3);
	}

	CreateVar(Rhs+1, MATRIX_OF_INTEGER_DATATYPE, &one,&one, &l4);
	CreateVar(Rhs+2, MATRIX_OF_DOUBLE_DATATYPE, &one,&one, &l5);
	lout = PATH_MAX + FILENAME_MAX;
	C2F(cluni0)(cstk(l1), filename, &out_n,m1*n1,lout);

	C2F(mopen)(istk(l4),filename,status,&swap,stk(l5),&err);

	if (err >  0)
	{
		if ( Lhs == 1)
		{
			if ( err == 1) 
			{
				Error(66);/* no more logical units */
				return 0;
			}
			else if ( err == 2)
			{
				Scierror(999,_("%s: Cannot open file %s.\n"),fname,filename);
				return 0;
			}
			else
			{
				Scierror(999,_("%s: No more memory.\n"),fname);
				return 0;
			}
		}
		else
		{
			*stk(l5) = - err;
		}
	}

	LhsVar(1) = Rhs+1;
	LhsVar(2) = Rhs+2;
	PutLhsVar();

	return 0;
}
/*--------------------------------------------------------------------------*/
