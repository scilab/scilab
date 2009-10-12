/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Vincent COUVERT
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "XcosDiagram.hxx"

extern "C"
{
#include "gw_xcos.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "getScilabJavaVM.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_xcos;
/*--------------------------------------------------------------------------*/
int sci_setBlockTextValue(char *fname,unsigned long fname_len)
{
	CheckRhs(2,2);
	CheckLhs(0,1);

	int m1 = 0, n1 = 0;
	int *piAddressVarOne = NULL;
	int *piAddressVarTwo = NULL;
	double *pStVarOne = NULL;
	char **pStVarTwo = NULL;
	int *lenStVarTwo = NULL;
        int i = 0;
	int iType = 0;
        int iBlockID = 0;
	StrErr strErr;

        /* Read bloc ID */
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

	if ( iType != sci_matrix )
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A Real Scalar expected.\n"),fname,1);
		return 0;
	}

        strErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne, &m1, &n1, &pStVarOne); 
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

        if (m1 * n1 != 1)
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A Real Scalar expected.\n"),fname,1);
		return 0;
	}

        iBlockID = (int) pStVarOne[0];

        /* Read text value */
	strErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	strErr = getVarType(pvApiCtx, piAddressVarTwo, &iType);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	if ( iType != sci_strings )
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,2);
		return 0;
	}

	/* get dimensions */
	strErr = getMatrixOfString(pvApiCtx, piAddressVarTwo, &m1, &n1, lenStVarTwo, pStVarTwo);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	lenStVarTwo = (int*)MALLOC(sizeof(int));
	if (lenStVarTwo == NULL)
	{
		Scierror(999,_("%s: No more memory.\n"), fname);
		return 0;
	}

	/* get lengths */
	strErr = getMatrixOfString(pvApiCtx, piAddressVarTwo, &m1, &n1, lenStVarTwo, pStVarTwo);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	pStVarTwo = (char **)MALLOC(sizeof(char*)*(m1*n1));
	if (pStVarTwo == NULL)
	{
		Scierror(999,_("%s: No more memory.\n"), fname);
		return 0;
	}

        for(i = 0; i < m1 * n1; i++)
          {
            pStVarTwo[i] = (char*)MALLOC(sizeof(char*) * (lenStVarTwo[i] + 1));
          }

	/* get strings */
	strErr = getMatrixOfString(pvApiCtx, piAddressVarTwo, &m1, &n1, lenStVarTwo, pStVarTwo);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	XcosDiagram::setBlockTextValue(getScilabJavaVM(), iBlockID, pStVarTwo, m1*n1, m1, n1);
	freeArrayOfString(pStVarTwo, 1);

	LhsVar(1) = 0;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
