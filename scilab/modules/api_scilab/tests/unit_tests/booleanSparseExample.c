/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Scilab Consortium Operational Team
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "api_scilab.h"
#include "MALLOC.h"

int booleanSparseExample(char *fname,unsigned long fname_len)
{
	SciErr sciErr;
	int* piAddr		= NULL;
	int iType		= 0;
	int iRet		= 0;
	CheckRhs(1,1);
	CheckLhs(0,1);
	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
	if(sciErr.iErr)
	{
			printError(&sciErr, 0);
			return 0;
	}
	if(isBooleanSparseType(pvApiCtx, piAddr))
	{
		int iRows			= 0;
		int iCols			= 0;
		int iNbItem			= 0;
		int* piNbItemRow	= NULL;
		int* piColPos		= NULL;
		iRet = getAllocatedBooleanSparseMatrix(pvApiCtx, piAddr, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos);
		if(iRet)
		{
			freeAllocatedBooleanSparse(piNbItemRow, piColPos);
			return iRet;
		}
		sciErr = createBooleanSparseMatrix(pvApiCtx, Rhs + 1, iRows, iCols, iNbItem, piNbItemRow, piColPos);
		if(sciErr.iErr)
		{
			freeAllocatedBooleanSparse(piNbItemRow, piColPos);
			printError(&sciErr, 0);
			return sciErr.iErr;
		}
		freeAllocatedBooleanSparse(piNbItemRow, piColPos);
		LhsVar(1) = Rhs + 1;
	}
	return 0;
}
