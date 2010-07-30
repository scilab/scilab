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

int read_sparse(char *fname,unsigned long fname_len)
{
	SciErr sciErr;
	int i,j,k;
	int* piAddr			= NULL;
	int iRows			= 0;
	int iCols			= 0;
	int iNbItem			= 0;
	int* piNbItemRow	= NULL;
	int* piColPos		= NULL;
	double* pdblReal	= NULL;
	double* pdblImg		= NULL;
	CheckRhs(1,1);
	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	if(isVarComplex(pvApiCtx, piAddr))
	{
		sciErr = getComplexSparseMatrix(pvApiCtx, piAddr, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
	}
	else
	{
		sciErr = getSparseMatrix(pvApiCtx, piAddr, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal);
	}
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	sciprint("Sparse %d item(s)\n", iNbItem);
	k = 0;
	for(i = 0 ; i < iRows ; i++)
	{
		for(j = 0 ; j < piNbItemRow[i] ; j++)
		{
			sciprint("(%d,%d) = %f", i+1, piColPos[k], pdblReal[k]);
			if(isVarComplex(pvApiCtx, piAddr))
			{
				sciprint(" %+fi", pdblImg[k]);
			}
			sciprint("\n");
			k++;
		}
	}
	//assign allocated variables to Lhs position
	LhsVar(1) = 0;
	return 0;
}
