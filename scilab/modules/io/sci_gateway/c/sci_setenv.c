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
#include "api_scilab.h"
#include "setenvc.h"
#include "MALLOC.h" /* MALLOC */
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_setenv)(char *fname,unsigned long fname_len)
{
	StrErr strErr;
	int m1 = 0, n1 = 0;
	int *piAddressVarOne = NULL;
	int iType1 = 0;
	char *pStVarOne = NULL;
	int lenStVarOne = 0;

	int m2 = 0, n2 = 0;
	int *piAddressVarTwo = NULL;
	int iType2 = 0;
	char *pStVarTwo = NULL;
	int lenStVarTwo = 0;

	int m_out1 = 0, n_out1 = 0;
	int *pbAddressOut1 = NULL;

	int result = 0;

	Rhs = Max(0,Rhs);
	CheckRhs(2,2);
	CheckLhs(0,1);

	strErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	strErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	strErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	if (iType1  != sci_strings )
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
		return 0;
	}

	strErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	if (iType2  != sci_strings )
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,2);
		return 0;
	}

	strErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&m1,&n1,&lenStVarOne,&pStVarOne);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	if ( (m1 != n1) && (n1 != 1) ) 
	{
		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
		return 0;
	}

	strErr = getMatrixOfString(pvApiCtx, piAddressVarTwo,&m2,&n2,&lenStVarTwo,&pStVarTwo);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	if ( (m2 != n2) && (n2 != 1) ) 
	{
		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,2);
		return 0;
	}

	pStVarOne = (char*)MALLOC(sizeof(char)*(lenStVarOne + 1));
	if (pStVarOne)
	{
		strErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&m1,&n1,&lenStVarOne,&pStVarOne);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

	}
	else
	{
		Scierror(999,_("%s : Memory allocation error.\n"),fname);
		return 0;
	}

	pStVarTwo = (char*)MALLOC(sizeof(char)*(lenStVarTwo + 1));
	if (pStVarTwo)
	{
		strErr = getMatrixOfString(pvApiCtx, piAddressVarTwo,&m2,&n2,&lenStVarTwo,&pStVarTwo);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

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
	strErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, m_out1, n_out1, &result);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	LhsVar(1) = Rhs + 1; 

	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
