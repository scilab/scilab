/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#include <stdio.h>
#include <string.h>
#include "gw_io.h"
#include "api_common.h"
#include "api_string.h"
#include "api_boolean.h"
#include "setenvc.h"
#include "MALLOC.h" /* MALLOC */
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_setenv)(char *fname,unsigned long fname_len)
{
	int m1 = 0, n1 = 0;
	int *piAddressVarOne = NULL;
	char *pStVarOne = NULL;
	int lenStVarOne = 0;

	int m2 = 0, n2 = 0;
	int *piAddressVarTwo = NULL;
	char *pStVarTwo = NULL;
	int lenStVarTwo = 0;

	int m_out1 = 0, n_out1 = 0;
	int *pbAddressOut1 = NULL;

	int result = 0;

	Rhs = Max(0,Rhs);
	CheckRhs(2,2);
	CheckLhs(0,1);

	getVarAddressFromPosition(1, &piAddressVarOne);
	getVarAddressFromPosition(2, &piAddressVarTwo);

	if ( getVarType(piAddressVarOne) != sci_strings )
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
		return 0;
	}

	if ( getVarType(piAddressVarTwo) != sci_strings )
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,2);
		return 0;
	}

	getMatrixOfString(piAddressVarOne,&m1,&n1,&lenStVarOne,&pStVarOne);
	if ( (m1 != n1) && (n1 != 1) ) 
	{
		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
		return 0;
	}

	getMatrixOfString(piAddressVarTwo,&m2,&n2,&lenStVarTwo,&pStVarTwo);
	if ( (m2 != n2) && (n2 != 1) ) 
	{
		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,2);
		return 0;
	}

	pStVarOne = (char*)MALLOC(sizeof(char)*(lenStVarOne + 1));
	if (pStVarOne)
	{
		getMatrixOfString(piAddressVarOne,&m1,&n1,&lenStVarOne,&pStVarOne);
	}
	else
	{
		Scierror(999,_("%s : Memory allocation error.\n"),fname);
		return 0;
	}

	pStVarTwo = (char*)MALLOC(sizeof(char)*(lenStVarTwo + 1));
	if (pStVarTwo)
	{
		getMatrixOfString(piAddressVarTwo,&m2,&n2,&lenStVarTwo,&pStVarTwo);
	}
	else
	{
		FREE(pStVarOne);
		Scierror(999,_("%s : Memory allocation error.\n"),fname);
		return 0;
	}

	result = setenvc(pStVarOne, pStVarTwo);

	FREE(pStVarOne); pStVarOne = NULL;
	FREE(pStVarTwo); pStVarTwo = NULL;

	m_out1 = 1; n_out1 = 1;
	createMatrixOfBoolean(Rhs + 1, m_out1, n_out1, &result);
	LhsVar(1) = Rhs + 1; 

	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
