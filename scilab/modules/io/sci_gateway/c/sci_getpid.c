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

#include "gw_io.h"
#include "stack-c.h"
#include "getpidc.h"
/*--------------------------------------------------------------------------*/
int sci_getpid(char *fname,unsigned long fname_len)
{
	int one = 1;
	int l1 = 0;
	int pid = 0;

	CheckRhs(0,0);
	CheckLhs(0,1);

	C2F(getpidc)(&pid);
	CreateVar(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &one,&one, &l1);
	*istk(l1) = (int) pid;

	LhsVar(1)= Rhs+1;
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
