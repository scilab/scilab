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
#include "gw_fileio.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "localization.h"
#include "api_common.h"
#include "api_string.h"
#include "api_double.h"
#include "api_boolean.h"
#include "basename.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_basename(char *fname,unsigned long fname_len)
{
	BOOL flag = TRUE; /* default */
	BOOL flagexpand = TRUE; /* default */

	int *piAddressVarOne = NULL;
	wchar_t **pStVarOne = NULL;
	int *lenStVarOne = NULL;
	int m1 = 0, n1 = 0;

	wchar_t **pStResult = NULL;

	int i = 0;

	/* Check Input & Output parameters */
	CheckRhs(1,3);
	CheckLhs(1,1);

	if (Rhs > 2)
	{
		int *piAddressVarThree = NULL;
		int *piData = NULL;
		int m3 = 0, n3 = 0;

		getVarAddressFromPosition(3, &piAddressVarThree);
		if (getVarType(piAddressVarThree) != sci_boolean)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 3);
			return 0;
		}

		getVarDimension(piAddressVarThree, &m3, &n3);

		if ( (m3 != n3) && (n3 != 1) ) 
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 3);
			return 0;
		}

		getMatrixOfBoolean(piAddressVarThree, &m3, &n3,  &piData);
		flagexpand = piData[0];

	}

	if (Rhs > 1)
	{
		int *piAddressVarTwo = NULL;
		int *piData = NULL;
		int m2 = 0, n2 = 0;

		getVarAddressFromPosition(2, &piAddressVarTwo);
		if (getVarType(piAddressVarTwo) != sci_boolean)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 2);
			return 0;
		}

		getVarDimension(piAddressVarTwo, &m2, &n2);

		if ( (m2 != n2) && (n2 != 1) ) 
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 2);
			return 0;
		}

		getMatrixOfBoolean(piAddressVarTwo, &m2, &n2,  &piData);
		flag = piData[0];
	}

	getVarAddressFromPosition(1, &piAddressVarOne);

	if (getVarType(piAddressVarOne) == sci_matrix)
	{
		getVarDimension(piAddressVarOne, &m1, &n1);
		if ( (m1 == n1) && (m1 == 0) )
		{
			createMatrixOfDouble(Rhs + 1, m1, n1, NULL);
			LhsVar(1) = Rhs + 1;
			C2F(putlhsvar)();
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: String array expected.\n"), fname, 1);
		}
	}
	else if (getVarType(piAddressVarOne) == sci_strings)
	{
		getVarDimension(piAddressVarOne, &m1, &n1);

		lenStVarOne = (int*)MALLOC(sizeof(int) * (m1 * n1));
		if (lenStVarOne == NULL)
		{
			Scierror(999,_("%s : Memory allocation error.\n"),fname);
			return 0;
		}

		pStVarOne = (wchar_t**)MALLOC(sizeof(wchar_t*) * (m1 * n1));
		if (pStVarOne == NULL)
		{
			if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
			Scierror(999,_("%s : Memory allocation error.\n"),fname);
			return 0;
		}

		getMatrixOfWideString(piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
		pStResult = (wchar_t**)MALLOC(sizeof(wchar_t*) * (m1 * n1));

		if (pStResult == NULL)
		{
			if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
			Scierror(999,_("%s : Memory allocation error.\n"),fname);
			return 0;
		}

		for (i=0;i< m1 * n1; i++)
		{
			pStResult[i] = basenameW(pStVarOne[i], flagexpand);
		}

		createMatrixOfWideString(Rhs + 1, m1, n1, pStResult);
		LhsVar(1) = Rhs + 1;
		C2F(putlhsvar)();

		if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
		freeArrayOfWideString(pStResult, m1 * n1);
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: String array expected.\n"), fname, 1);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
