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

#include "gw_io.h"
#include "stack-c.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_getio)(char *fname,unsigned long fname_len)
{
	int one = 1;
	int four = 4;
	int *values = NULL;

	CheckRhs(0,0);
	CheckLhs(0,1);

	values = (int*)MALLOC(sizeof(int)*(one*four));

	values[0] =  C2F(iop).rio;
	values[1] =  C2F(iop).rte;
	values[2] =  0;
	values[3] =  C2F(iop).wte;

	CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE,&one,&four,&values);
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();

	if (values) {FREE(values);values = NULL;}

	return 0;
}
/*--------------------------------------------------------------------------*/
