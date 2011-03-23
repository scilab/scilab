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

int polyExample(char *fname,unsigned long fname_len)
{
	SciErr sciErr;
	int* piAddr		= NULL;
	int iType			= 0;
	int iRet			= 0;
	CheckRhs(1,1);
	CheckLhs(0,1);
	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	if(isPolyType(pvApiCtx, piAddr))
	{
		char pstVarName[64];
		int iLen		= 0;
		sciErr = getPolyVariableName(pvApiCtx, piAddr, pstVarName, &iLen);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return sciErr.iErr;
		}
		if(isScalar(pvApiCtx, piAddr))
		{
			int iNbCoef				= 0;
			double* pdblReal	= NULL;
			double* pdblImg		= NULL;
			if(isVarComplex(pvApiCtx, piAddr))
			{
				iRet = getAllocatedSingleComplexPoly(pvApiCtx, piAddr, &iNbCoef, &pdblReal, &pdblImg);
				if(iRet)
				{
					freeAllocatedSingleComplexPoly(pdblReal, pdblImg);
					return iRet;
				}
				sciErr = createComplexMatrixOfPoly(pvApiCtx, Rhs + 1, pstVarName, 1, 1, &iNbCoef, &pdblReal, &pdblImg);
				if(sciErr.iErr)
				{
					freeAllocatedSingleComplexPoly(pdblReal, pdblImg);
					printError(&sciErr, 0);
					return sciErr.iErr;
				}
				freeAllocatedSingleComplexPoly(pdblReal, pdblImg);
			}
			else
			{
				iRet = getAllocatedSinglePoly(pvApiCtx, piAddr, &iNbCoef, &pdblReal);
				if(iRet)
				{
					freeAllocatedSinglePoly(pdblReal);
					return iRet;
				}
				sciErr = createMatrixOfPoly(pvApiCtx, Rhs + 1, pstVarName, 1, 1, &iNbCoef, &pdblReal);
				if(sciErr.iErr)
				{
					freeAllocatedSinglePoly(pdblReal);
					printError(&sciErr, 0);
					return sciErr.iErr;
				}
				freeAllocatedSinglePoly(pdblReal);
			}
		}
		else
		{
			int iRows					= 0;
			int iCols					= 0;
			int* piNbCoef			= NULL;
			double** pdblReal	= NULL;
			double** pdblImg	= NULL;
			if(isVarComplex(pvApiCtx, piAddr))
			{
				iRet = getAllocatedMatrixOfComplexPoly(pvApiCtx, piAddr, &iRows, &iCols, &piNbCoef, &pdblReal, &pdblImg);
				if(iRet)
				{
					freeAllocatedMatrixOfComplexPoly(iRows, iCols, piNbCoef, pdblReal, pdblImg);
					return iRet;
				}
				sciErr = createComplexMatrixOfPoly(pvApiCtx, Rhs + 1, pstVarName, iRows, iCols, piNbCoef, pdblReal, pdblImg);
				if(sciErr.iErr)
				{
					freeAllocatedMatrixOfComplexPoly(iRows, iCols, piNbCoef, pdblReal, pdblImg);
					printError(&sciErr, 0);
					return sciErr.iErr;
				}
				freeAllocatedMatrixOfComplexPoly(iRows, iCols, piNbCoef, pdblReal, pdblImg);
			}
			else
			{
				iRet = getAllocatedMatrixOfPoly(pvApiCtx, piAddr, &iRows, &iCols, &piNbCoef, &pdblReal);
				if(iRet)
				{
					freeAllocatedMatrixOfPoly(iRows, iCols, piNbCoef, pdblReal);
					return iRet;
				}
				sciErr = createMatrixOfPoly(pvApiCtx, Rhs + 1, pstVarName, iRows, iCols, piNbCoef, pdblReal);
				if(sciErr.iErr)
				{
					freeAllocatedMatrixOfPoly(iRows, iCols, piNbCoef, pdblReal);
					printError(&sciErr, 0);
					return sciErr.iErr;
				}
				freeAllocatedMatrixOfPoly(iRows, iCols, piNbCoef, pdblReal);
			}
		}
		LhsVar(1) = Rhs + 1;
	}
	else
	{
		LhsVar(1) = 0;
	}
	return 0;
}
