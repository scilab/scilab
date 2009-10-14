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
#include "api_scilab.h"
#include "Scierror.h"
#include "pathconvert.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_pathconvert(char *fname,unsigned long fname_len)
{
	StrErr strErr;
	int *piAddressVarOne = NULL;
	wchar_t **pStVarOne = NULL;
	int iType1 = 0;
	int *lenStVarOne = NULL;
	int m1 = 0, n1 = 0;

	wchar_t **results = NULL;
	int i = 0;

	BOOL flagtrail = TRUE;
	BOOL flagexpand = TRUE;

	PathConvertType PType = AUTO_STYLE;

	/* Check Input & Output parameters */
	CheckRhs(1,4);
	CheckLhs(1,1);


	if (Rhs > 3)
	{
		int *piAddressVarFour = NULL;
		wchar_t *pStVarFour = NULL;
		int iType4 = 0;
		int lenStVarFour = 0;
		int m4 = 0, n4 = 0;

		strErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddressVarFour);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		strErr = getVarType(pvApiCtx, piAddressVarFour, &iType4);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if (iType4 != sci_strings)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 4);
			return 0;
		}

		strErr = getVarDimension(pvApiCtx, piAddressVarFour, &m4, &n4);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if ( (m4 != n4) && (n4 != 1) ) 
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 4);
			return 0;
		}

		strErr = getMatrixOfWideString(pvApiCtx, piAddressVarFour, &m4, &n4, &lenStVarFour, &pStVarFour);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}


		pStVarFour = (wchar_t*)MALLOC(sizeof(wchar_t)*(lenStVarFour + 1));
		if (pStVarFour == NULL)
		{
			Scierror(999,_("%s : Memory allocation error.\n"),fname);
			return 0;
		}

		strErr = getMatrixOfWideString(pvApiCtx, piAddressVarFour, &m4, &n4, &lenStVarFour, &pStVarFour);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}


		if ( (wcscmp(pStVarFour, L"w") == 0) || (wcscmp(pStVarFour, L"u") == 0) )
		{
			if (wcscmp(pStVarFour, L"w") == 0)
			{
				PType = WINDOWS_STYLE;
			}
			else
			{
				PType = UNIX_STYLE;
			}
			if (pStVarFour) {FREE(pStVarFour); pStVarFour = NULL;}
		}
		else
		{
			if (pStVarFour) {FREE(pStVarFour); pStVarFour = NULL;}
			Scierror(999,_("%s: Wrong value for input argument #%d: ''w'' or ''u'' string expected.\n"), fname, 4);
			return 0;
		}
	}

	if (Rhs > 2)
	{
		int *piAddressVarThree = NULL;
		int *piData = NULL;
		int iType3	= 0;
		int m3 = 0, n3 = 0;

		strErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		strErr = getVarType(pvApiCtx, piAddressVarThree, &iType3);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if (iType3 != sci_boolean)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 3);
			return 0;
		}

		strErr = getVarDimension(pvApiCtx, piAddressVarThree, &m3, &n3);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}


		if ( (m3 != n3) && (n3 != 1) ) 
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 3);
			return 0;
		}

		strErr = getMatrixOfBoolean(pvApiCtx, piAddressVarThree, &m3, &n3,  &piData);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		flagexpand = piData[0];
	}

	if (Rhs > 1)
	{
		int *piAddressVarTwo = NULL;
		int *piData = NULL;
		int iType2	= 0;
		int m2 = 0, n2 = 0;

		strErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		strErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if (iType2 != sci_boolean)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 2);
			return 0;
		}

		strErr = getVarDimension(pvApiCtx, piAddressVarTwo, &m2, &n2);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if ( (m2 != n2) && (n2 != 1) ) 
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 2);
			return 0;
		}

		strErr = getMatrixOfBoolean(pvApiCtx, piAddressVarTwo, &m2, &n2,  &piData);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		flagtrail = piData[0];
	}

	strErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
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

	if (iType1 == sci_matrix)
	{
		strErr = getVarDimension(pvApiCtx, piAddressVarOne, &m1, &n1);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if ( (m1 == n1) && (m1 == 0) )
		{
			strErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m1, n1, NULL);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			LhsVar(1) = Rhs + 1;
			C2F(putlhsvar)();
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: String array expected.\n"), fname, 1);
		}
	}
	else if (iType1 == sci_strings)
	{
		strErr = getVarDimension(pvApiCtx, piAddressVarOne, &m1, &n1);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

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

		results = (wchar_t **)MALLOC(sizeof(wchar_t*) * (m1 * n1));

		if (results == NULL)
		{
			if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
			freeArrayOfWideString(pStVarOne, m1 * n1);
			Scierror(999,_("%s : Memory allocation error.\n"),fname);
			return 0;
		}

		strErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		for( i = 0; i < m1 * n1; i++)
		{
			results[i] = pathconvertW(pStVarOne[i], flagtrail, flagexpand, PType);
		}

		strErr = createMatrixOfWideString(pvApiCtx, Rhs + 1, m1, n1, results);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		LhsVar(1) = Rhs + 1;
		C2F(putlhsvar)();

		if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
		freeArrayOfWideString(results, m1 * n1);
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
		return 0;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
