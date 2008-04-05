/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) {2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
/* desc : interface for prompt primitive                                    */
/*--------------------------------------------------------------------------*/
#include "gw_shell.h"
#include "stack-c.h"
#include "prompt.h"
/*--------------------------------------------------------------------------*/
#define PROMPT_SIZE_MAX 64
/*--------------------------------------------------------------------------*/
int C2F(sci_prompt)(char *fname,unsigned long fname_len)
{
	char currentPrompt[PROMPT_SIZE_MAX];
	static int n1 = 0, m1 = 0;
	int outIndex = 0 ;

	CheckRhs(0,0);
	CheckLhs(0,1);

	GetCurrentPrompt(currentPrompt);

	m1= (int)strlen(currentPrompt);
	n1=1;

	CreateVar( Rhs+1,STRING_DATATYPE,&m1,&n1,&outIndex);
	strcpy(cstk(outIndex), currentPrompt);

	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
