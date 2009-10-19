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
 */

#include "gw_functions.h"
#include "api_scilab.h"
#include "stack-c.h"
#include "recursionFunction.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intexecstr)(); /* fortran */
/*--------------------------------------------------------------------------*/
#define ERRCATCH_KEYWORD "errcatch"
#define MESSAGE_KEYWORD "m"
#define MESSAGE_DEFAULT_KEYWORD "n"
/*--------------------------------------------------------------------------*/
int C2F(sci_execstr)(char *fname,unsigned long fname_len)
{
	if ( isRecursionCallToFunction() )
	{
		C2F(intexecstr)(fname, fname_len);
	}
	else
	{
		StrErr strErr;

		int *piAddressVarOne = NULL;
		int iType1 = 0;

		CheckRhs(1,3);
		CheckLhs(0,1);

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

		/* execstr([])*/
		if (iType1 == sci_matrix)
		{
			int m1 = 0, n1 = 0;

			strErr = getVarDimension(pvApiCtx, piAddressVarOne, &m1, &n1);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			if ((m1 == n1) && (m1 == 0)) /* [] */
			{
				strErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 0, 0, NULL);
				if(strErr.iErr)
				{
					printError(&strErr, 0);
					return 0;
				}

				LhsVar(1) = Rhs + 1;

				C2F(putlhsvar)();
				return 0;
			}
			else
			{
				Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
				return 0;
			}
		}

		if (iType1 != sci_strings)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
			return 0;
		}

		if (Rhs > 1)
		{
			int m2 = 0, n2 = 0;
			int *piAddressVarTwo = NULL;
			char *pStVarTwo = NULL;
			int lenStVarTwo = 0;
			int iType2 = 0;

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

			if (iType2 != sci_strings)
			{
				Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,2);
				return 0;
			}

			strErr = getMatrixOfString(pvApiCtx, piAddressVarTwo,&m2,&n2,&lenStVarTwo,&pStVarTwo);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			if (m2 * n2 != 1)
			{
				Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,2);
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

				if (strcmp(pStVarTwo,ERRCATCH_KEYWORD))
				{
					Scierror(999,_("%s: Wrong value for input argument #%d: 'errcatch' expected.\n"),fname,2);
					FREE(pStVarTwo);
					pStVarTwo = NULL;
					return 0;
				}

				FREE(pStVarTwo);
				pStVarTwo = NULL;
			}
			else
			{
				Scierror(999,_("%s : Memory allocation error.\n"),fname);
				return 0;
			}
		}

		if (Rhs > 2)
		{
			int m3 = 0, n3 = 0;
			int *piAddressVarThree = NULL;
			char *pStVarThree = NULL;
			int lenStVarThree = 0;
			int iType3 = 0;

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

			if (iType3 != sci_strings)
			{
				Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,3);
				return 0;
			}

			strErr = getMatrixOfString(pvApiCtx, piAddressVarThree,&m3,&n3,&lenStVarThree,&pStVarThree);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			if (m3 * n3 != 1)
			{
				Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,3);
				return 0;
			}

			pStVarThree = (char*)MALLOC(sizeof(char)*(lenStVarThree + 1));
			if (pStVarThree)
			{
				strErr = getMatrixOfString(pvApiCtx, piAddressVarThree,&m3,&n3,&lenStVarThree,&pStVarThree);
				if(strErr.iErr)
				{
					printError(&strErr, 0);
					return 0;
				}

				if ( strcmp(pStVarThree, MESSAGE_KEYWORD) && strcmp(pStVarThree, MESSAGE_DEFAULT_KEYWORD) )
				{
					Scierror(999,_("%s: Wrong value for input argument #%d: 'm' or 'n' expected.\n"),fname,3);
					FREE(pStVarThree);
					pStVarThree = NULL;
					return 0;
				}

				FREE(pStVarThree);
				pStVarThree = NULL;
			}
			else
			{
				Scierror(999,_("%s : Memory allocation error.\n"),fname);
				return 0;
			}
		}

		C2F(intexecstr)(fname, fname_len);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
