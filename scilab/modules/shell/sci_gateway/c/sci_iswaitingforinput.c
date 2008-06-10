/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
/* desc: interface for iswaitingforinput primitive                          */
/*--------------------------------------------------------------------------*/
#include "gw_shell.h"
#include "stack-c.h"
#include "ConsoleIsWaitingForInput.h"
#include "scilabmode.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
/*--------------------------------------------------------------------------*/
int sci_iswaitingforinput(char *fname,unsigned long fname_len)
{
	BOOL res = FALSE;
	int un = 1, outIndex = 0;

	Rhs = Max(Rhs,0);
	CheckRhs(0,1);
	CheckLhs(0,1);

	if (getScilabMode() == SCILAB_STD) 
	{
		res = ConsoleIsWaitingForInput();
    } 
	else 
	{
		sciprint(_("%s: Not implemented in this mode.\n"),fname);
	}

	CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE,&un,&un,&outIndex);
	*istk(outIndex) = res;
	
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
