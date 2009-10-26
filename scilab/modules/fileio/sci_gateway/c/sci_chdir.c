/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
/*--------------------------------------------------------------------------*/
#include "gw_fileio.h"
#include "stack-c.h"
#include "Scierror.h"
#include "scicurdir.h"
#include "localization.h"
#include "expandPathVariable.h"
#include "MALLOC.h"
#include "localization.h"
#include "api_scilab.h"
#include "isdir.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
int sci_chdir(char *fname,unsigned long fname_len)
{
	StrErr strErr;
	int *piAddressVarOne = NULL;
	wchar_t *pStVarOne = NULL;
	int iType1	= 0;
	int lenStVarOne = 0;
	int m1 = 0, n1 = 0;

	wchar_t *expandedPath = NULL;

	Rhs = Max(0, Rhs);
	CheckRhs(0,1);
	CheckLhs(1,1);

	if (Rhs == 0)
	{
		pStVarOne = (wchar_t*)MALLOC(sizeof(wchar_t) * ((int)wcslen(L"home")+1));
		if (pStVarOne)
		{
			wcscpy(pStVarOne, L"home");
		}
	}
	else
	{
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

		if (iType1 != sci_strings )
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
			return 0;
		}

		strErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne,&m1,&n1,&lenStVarOne,&pStVarOne);
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

		pStVarOne = (wchar_t*)MALLOC(sizeof(wchar_t)*(lenStVarOne + 1));
		if (pStVarOne == NULL)
		{
			Scierror(999,_("%s : Memory allocation error.\n"),fname);
			return 0;
		}

		strErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}
	}

	expandedPath = expandPathVariableW(pStVarOne);
	if (pStVarOne) {FREE(pStVarOne); pStVarOne = NULL;}

	if (expandedPath)
	{
		/* get value of PWD scilab variable (compatiblity scilab 4.x) */
		if (wcscmp(expandedPath, L"PWD") == 0)
		{
			strErr = getNamedVarType(pvApiCtx, "PWD", &iType1);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			if (iType1 == sci_strings)
			{
				wchar_t *VARVALUE = NULL;
				int VARVALUElen = 0;
				int m = 0, n = 0;
				strErr = readNamedMatrixOfWideString(pvApiCtx, "PWD", &m, &n, &VARVALUElen, &VARVALUE);
				if(strErr.iErr)
				{
					printError(&strErr, 0);
					return 0;
				}

				if ( (m == 1) && (n == 1) )
				{
					VARVALUE = (wchar_t*)MALLOC(sizeof(wchar_t)*(VARVALUElen + 1));
					if (VARVALUE)
					{
						readNamedMatrixOfWideString(pvApiCtx, "PWD", &m, &n, &VARVALUElen, &VARVALUE);
						FREE(expandedPath);
						expandedPath = VARVALUE;
					}
				}
			}
		}

		if (strcmp(fname, "chdir") == 0) /* chdir output boolean */
		{	
			BOOL *bOuput = (BOOL*)MALLOC(sizeof(BOOL));

			int ierr = scichdirW(expandedPath);

			if (ierr) bOuput[0] = FALSE;
			else bOuput[0] = TRUE; 

			strErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, 1, 1, bOuput);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			LhsVar(1) = Rhs + 1;
			C2F(putlhsvar)();
			
		}
		else /* cd output string current path */
		{
			if ( isdirW(expandedPath) || (wcscmp(expandedPath,L"/") == 0) ||
				 (wcscmp(expandedPath,L"\\") == 0) )
			{
				int ierr = scichdirW(expandedPath);
				wchar_t *currentDir = scigetcwdW(&ierr);
				if ( (ierr == 0) && currentDir)
				{
					strErr = createMatrixOfWideString(pvApiCtx, Rhs + 1, 1, 1, &currentDir);
				}
				else
				{
					strErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 0, 0, NULL);
				}

				if(strErr.iErr)
				{
					printError(&strErr, 0);
					return 0;
				}

				LhsVar(1) = Rhs + 1;
				C2F(putlhsvar)();

				if (currentDir) {FREE(currentDir); currentDir = NULL;}
			}
			else
			{
				char *path = wide_string_to_UTF8(expandedPath);
				if (path)
				{
					Scierror(998, _("%s: Cannot go to directory %s\n"), fname, path);
					FREE(path);
					path = NULL;
				}
				else
				{
					Scierror(998, _("%s: Cannot go to directory.\n"), fname);
				}
			}
		}

		FREE(expandedPath); expandedPath = NULL;
	}
	else
	{
		Scierror(999,_("%s : Memory allocation error.\n"),fname);
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
