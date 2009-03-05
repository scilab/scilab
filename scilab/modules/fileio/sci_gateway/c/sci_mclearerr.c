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
#include "gw_fileio.h"
#include "stack-c.h"
#include "mclearerr.h"
/*--------------------------------------------------------------------------*/
#define ALL_FILES_DESCRIPTOR -1
/*--------------------------------------------------------------------------*/
int sci_mclearerr(char *fname,unsigned long fname_len)
{
	int m1 = 0, n1 = 0, l1 = 0;
	int fd = ALL_FILES_DESCRIPTOR;

	Nbvars = 0;
	CheckRhs(0,1);
	CheckLhs(1,1);

	/* @TODO Add check about input type */

	/*  checking variable file */
	if ( Rhs >= 1)
	{
		GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
		fd = *istk(l1);
	}

	C2F(mclearerr)(&fd);

	LhsVar(1) = 0;
	PutLhsVar();

	return 0;
}
/*--------------------------------------------------------------------------*/
