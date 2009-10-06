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
#include "gw_core.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "searchmacroinlibraries.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
extern int C2F(whereismacro)(); 
/*--------------------------------------------------------------------------*/
int sci_whereis(char *fname,unsigned long fname_len)
{
	StrErr strErr;
	int *piAddressVarOne = NULL;
	int iType1 = 0;

	/* Check the number of input argument */
	CheckRhs(1,1); 

	/* Check the number of output argument */
	CheckLhs(1,1);

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


	if ( (iType1 == sci_u_function) || (iType1 == sci_c_function) )
	{
		int m = 0, n = 0;

		strErr = getVarDimension(pvApiCtx, piAddressVarOne, &m, &n);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if ( (m != n) && (n != 1) ) 
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A function-name expected.\n"),fname,1);
			return 0;
		}

		/* to rewrite with new API when it will be possible */
		C2F(whereismacro)();
	}
	else if (iType1 == sci_strings)
	{
		char *pStVarOne = NULL;
		int lenStVarOne = 0;
		int m = 0, n = 0;

		char **librariesResult = NULL;
		int librariesResultSize = 0;

		strErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m, &n, &lenStVarOne, &pStVarOne);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if ( (m != n) && (n != 1) ) 
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
			return 0;
		}

		pStVarOne = (char*)MALLOC(sizeof(char)*(lenStVarOne + 1));
		if (pStVarOne == NULL)
		{
			Scierror(999,_("%s : Memory allocation error.\n"),fname);
			return 0;
		}

		strErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&m,&n,&lenStVarOne,&pStVarOne);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		librariesResult = searchmacroinlibraries(pStVarOne, &librariesResultSize);

		if ( (librariesResultSize == 0) || (librariesResult == NULL) )
		{
			// return []
			strErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 0, 0, NULL);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

		}
		else
		{
			int m_out = librariesResultSize;
			int n_out = 1;

			strErr = createMatrixOfString(pvApiCtx, Rhs + 1, m_out, n_out, librariesResult);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

		}

		LhsVar(1) = Rhs + 1;

		freeArrayOfString(librariesResult, librariesResultSize);
		if (pStVarOne){FREE(pStVarOne); pStVarOne = NULL;}

		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d.\n"),fname, 1);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
