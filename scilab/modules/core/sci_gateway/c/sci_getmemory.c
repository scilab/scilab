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
#include "getmemory.h"
#include "gw_core.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_getmemory) (char *fname,unsigned long fname_len)
{
	static int l1,one=1;

	Rhs=Max(Rhs,0);
	CheckRhs(0,0) ;
	CheckLhs(1,2);

	CreateVar(Rhs+1,MATRIX_OF_INTEGER_DATATYPE,&one,&one,&l1);
	*istk(l1) = getfreememory();
	LhsVar(1) = Rhs+1;
	if (Lhs == 2) {
		CreateVar(Rhs+2,MATRIX_OF_INTEGER_DATATYPE,&one,&one,&l1);
		*istk(l1) = getmemorysize();
		LhsVar(2) = Rhs+2;
	}

	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
