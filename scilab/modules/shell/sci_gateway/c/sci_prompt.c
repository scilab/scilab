/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_shell.h"
#include "stack-c.h"
#include "prompt.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_prompt(char *fname,unsigned long fname_len)
{
	char currentPrompt[PROMPT_SIZE_MAX];
	static int n1 = 0, m1 = 0, l1 = 0;
	int outIndex = 0 ;

	CheckRhs(0,1);
	CheckLhs(0,2);

	if (Rhs == 0) /* Get current Scilab prompt */
	{
		GetCurrentPrompt(currentPrompt);
            
		m1= (int)strlen(currentPrompt);
		n1=1;
		CreateVar( Rhs+1,STRING_DATATYPE,&m1,&n1,&outIndex);
		strcpy(cstk(outIndex), currentPrompt);

		if (Lhs == 2)
		{
			m1 = 1;
			n1 = 1;
			l1 = 0;

			CreateVar( Rhs+2, MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1 );
			*istk(l1) = (int)C2F(recu).paus ;
		}
            
		LhsVar(1) = Rhs+1;
		if (Lhs == 2) LhsVar(2) = Rhs+2;
		C2F(putlhsvar)();
	}
	else /* Tempory change of Scilab prompt */
	{
		if (Lhs <=1)
		{
			if (VarType(1) != sci_strings)
			{
				Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname,1);
				return FALSE;
			}

			GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);

			if (n1 != 1)
			{
				Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname,1);
				return FALSE;
			}

			SetTemporaryPrompt(cstk(l1));

			LhsVar(1) = 0;
			C2F(putlhsvar)();
		}
		else
		{
			Scierror(999, _("%s: Wrong number of output argument(s).\n"), fname);
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
