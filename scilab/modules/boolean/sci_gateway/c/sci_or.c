/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string.h>
#include <stdlib.h>
#include "stack-c.h"
#include "gw_boolean.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "vect_or.h"
#include "api_scilab.h"
#include "api_oldstack.h"
/*--------------------------------------------------------------------------*/
/* SCILAB function : or */
/*--------------------------------------------------------------------------*/
int sci_or(char *fname, void* pvApiCtx)
{
	SciErr sciErr;
	int iRet			= 0;
	int iMode			= 0;

	int iRows			= 0;
	int iCols			= 0;

	int* piAddr1	= NULL;
	int* piAddr2	= NULL;

	int* piBool1		= NULL;
	int* piBool3		= NULL;

	CheckRhs(1,2);
	CheckLhs(1,1);


	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(!isBooleanType(pvApiCtx, piAddr1))
	{
		OverLoad(1);
		return 0;
	}

	if(Rhs == 2)
	{
		sciErr = getProcessMode(pvApiCtx, 2, piAddr1, &iMode);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
	}

	sciErr = getMatrixOfBoolean(pvApiCtx, piAddr1, &iRows, &iCols, &piBool1);
	if(iRet)
	{
		return 1;
	}

	switch(iMode)
	{
	case BY_ALL :
		piBool3 = (int*)MALLOC(sizeof(int));
        memset(piBool3, 0x00, sizeof(int));
		break;
	case BY_ROWS :
		piBool3 = (int*)MALLOC(sizeof(int) * iCols);
        memset(piBool3, 0x00, sizeof(int)* iCols);
		break;
	case BY_COLS :
		piBool3 = (int*)MALLOC(sizeof(int) * iRows);
        memset(piBool3, 0x00, sizeof(int) * iRows);
		break;
	}

	vect_or(piBool1, iRows, iCols, piBool3, iMode);

	switch(iMode)
	{
	case BY_ALL :
		iRet = createScalarBoolean(pvApiCtx, Rhs + 1, *piBool3);
		if(iRet)
		{
			return 0;
		}
		break;
	case BY_ROWS :
		sciErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, 1, iCols, piBool3);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		break;
	case BY_COLS :
		sciErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, iRows, 1, piBool3);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		break;
	}

	if(iRet)
	{
		return 1;
	}


	LhsVar(1)= Rhs + 1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
