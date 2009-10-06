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
#include "FileExist.h"
#include "isdir.h"
#include "expandPathVariable.h"
#include "freeArrayOfString.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
int sci_isfile(char *fname,unsigned long fname_len)
{
	StrErr strErr;
	int *piAddressVarOne = NULL;
	wchar_t **pStVarOne = NULL;
	int iType = 0;
	int *lenStVarOne = NULL;
	int m1 = 0, n1 = 0;

	BOOL *results = NULL;
	int m_out = 0, n_out = 0;
	int i = 0;

	/* Check Input & Output parameters */
	CheckRhs(1,1);
	CheckLhs(1,1);

	strErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	strErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	if (iType != sci_strings)
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
		return 0;
	}

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

	results = (BOOL*)MALLOC(sizeof(BOOL) * (m1 * n1));
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

	for (i=0;i< m1 * n1; i++)
	{
		wchar_t *expandedPath = expandPathVariableW(pStVarOne[i]);
		if (expandedPath)
		{
			results[i] = !isdirW(expandedPath) && FileExistW(expandedPath);
			FREE(expandedPath); expandedPath = NULL;
		}
		else
		{
			results[i] = FALSE;
		}
	}

	if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
	freeArrayOfWideString(pStVarOne, m1 * n1);

	strErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, m1, n1, results);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	LhsVar(1) = Rhs + 1;

	if (results) {FREE(lenStVarOne); lenStVarOne = NULL;}
	
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
