/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "stack-c.h"
#include "fileio.h"
/*--------------------------------------------------------------------------*/ 
int int_objnumTokens(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
int int_objnumTokens(char *fname,unsigned long fname_len)
{
	static int l1,m1,n1,l2,un=1;
	Nbvars = 0;
	CheckRhs(1,1);
	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
	StringConvert(cstk(l1));  /* conversion */
	CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE, &un, &un, &l2);
	*stk(l2) = (double) NumTokens(cstk(l1));
	LhsVar(1) = Rhs+2;
	PutLhsVar();
	return 0;
}  
/*--------------------------------------------------------------------------*/ 
