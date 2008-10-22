/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* desc : interface for historysize primitive                             */
/*------------------------------------------------------------------------*/
#include "gw_history_manager.h"
#include "stack-c.h"
#include "HistoryManager.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_historysize)(char *fname,unsigned long fname_len)
{
	int *paramoutINT=NULL;
	int n = 1;
	Rhs = Max(Rhs,0);
	CheckRhs(0,0);
	CheckLhs(0,1);

	paramoutINT=(int*)MALLOC(sizeof(int));

	*paramoutINT = getSizeScilabHistory();

	CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &n, &n, &paramoutINT);
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();

	if (paramoutINT) {FREE(paramoutINT);paramoutINT = NULL;}
	return 0 ;
}
/*--------------------------------------------------------------------------*/
