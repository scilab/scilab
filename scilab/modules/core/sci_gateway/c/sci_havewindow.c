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
#include "gw_core.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "scilabmode.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_havewindow)(char *fname,unsigned long fname_len)
{
	static int n1,m1;
	int *Status=NULL;

	CheckRhs(0,1);
	CheckLhs(1,1);

	Status=(int*)MALLOC(sizeof(int));
	*Status = (int) ( getScilabMode() == SCILAB_STD );

	m1=1;n1=1;
	CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &Status);
	LhsVar(1)=Rhs+1;

	C2F(putlhsvar)();

	if (Status) {FREE(Status);Status=NULL;}

	return 0;
}
/*--------------------------------------------------------------------------*/
