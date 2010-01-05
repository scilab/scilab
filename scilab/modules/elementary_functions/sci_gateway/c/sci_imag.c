/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_elementary_functions.h"
#include "stack-c.h"
#include "basic_functions.h"
#include "api_scilab.h"

SciErr img_double(int* _piAddress);
SciErr img_poly(int* _piAddress);
SciErr img_sparse(int* _piAddress);

/*--------------------------------------------------------------------------*/
int C2F(sci_imag) (char *fname,unsigned long fname_len)
{
	SciErr sciErr;
	int* piAddr		= NULL;
	int iType = 0;

	CheckRhs(1,1);
	CheckLhs(1,1);

	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarType(pvApiCtx, piAddr, &iType);

	switch(iType)
	{
	case sci_matrix:
		{
			sciErr = img_double(piAddr);
			break;
		}
	case sci_poly:
		{
			sciErr = img_poly(piAddr);
			break;
		}
	case sci_sparse:
		{
			sciErr = img_sparse(piAddr);
			break;
		}
	default:
		OverLoad(1);
		break;
	}

	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

SciErr img_double(int* _piAddress)
{
	SciErr sciErr;
	int iRows						= 0;
	int iCols						= 0;

	double *pdblReal		= NULL;
	double *pdblImg			= NULL;
	double *pdblRealRet	= NULL;

	if(isVarComplex(pvApiCtx, _piAddress))
	{
		sciErr = getComplexMatrixOfDouble(pvApiCtx, _piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}
	else
	{
		sciErr = getVarDimension(pvApiCtx, _piAddress, &iRows, &iCols);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		memset(pdblRealRet, 0x00, iRows * iCols * sizeof(double));
	}

	return sciErr;
}

SciErr img_poly(int* _piAddress)
{
	SciErr sciErr;
	int i,j;
	int iRows							= 0;
	int iCols							= 0;
	int iLen							= 0;
	int *piCoeff					= NULL;
	char pstVarName[16];

	double** pdblReal			= NULL;
	double** pdblImg			= NULL;
	double** pdblRealRet	= NULL;

	sciErr = getPolyVariableName(pvApiCtx, _piAddress, pstVarName, &iLen);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	if(isVarComplex(pvApiCtx, _piAddress))
	{
		sciErr = getComplexMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, NULL, NULL, NULL);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		piCoeff	= (int*)malloc(iRows * iCols * sizeof(int));
		sciErr = getComplexMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, piCoeff, NULL, NULL);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		pdblReal		= (double**)malloc(sizeof(double*) * iRows * iCols);
		pdblImg			= (double**)malloc(sizeof(double*) * iRows * iCols);

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblReal[i]			= (double*)malloc(sizeof(double) * piCoeff[i]);
			pdblImg[i]			= (double*)malloc(sizeof(double) * piCoeff[i]);
		}

		sciErr = getComplexMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, piCoeff, pdblReal, pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			int iCoeff = piCoeff[i];
			for(j = iCoeff - 1 ; j >= 0 ; j--)
			{
				if(pdblImg[i][j] == 0)
				{
					piCoeff[i]--;
				}
				else
				{
					break;
				}
			}
		}
		sciErr = createMatrixOfPoly(pvApiCtx, Rhs + 1, pstVarName, iRows, iCols, piCoeff, pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}
	else
	{
		sciErr = getVarDimension(pvApiCtx, _piAddress, &iRows, &iCols);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		piCoeff = (int*)malloc(sizeof(int) * iRows * iCols);
		memset(piCoeff, 0x00, sizeof(int) * iRows * iCols);
		pdblRealRet = (double**)malloc(sizeof(double*) * iRows * iCols);
		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblRealRet[i] = (double*)malloc(sizeof(double) * 1);
			pdblRealRet[i] = 0;
		}
		sciErr = createMatrixOfPoly(pvApiCtx, Rhs + 1, pstVarName, iRows, iCols, piCoeff, pdblRealRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}
	return sciErr;
}

SciErr img_sparse(int* _piAddress)
{
	SciErr sciErr;
	int i,j,x,y;
	int iRows						= 0;
	int iCols						= 0;
	int iNbItem					= 0;
	int *piNbItemRow		= NULL;
	int *piColPos				= NULL;

	int iNbItemNew			= 0;
	int* piNbItemRowNew	= NULL;
	int* piColPosNew		= NULL;

	double *pdblReal		= 0;
	double *pdblImg			= 0;
	double *pdblRealRet	= NULL;

	if(isVarComplex(pvApiCtx, _piAddress))
	{
		sciErr = getComplexSparseMatrix(pvApiCtx, _piAddress, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		for(i = 0 ; i < iNbItem ; i++)
		{
			if(pdblImg[i] != 0)
			{
				iNbItemNew++;
			}
		}

		sciErr = allocSparseMatrix(pvApiCtx, Rhs + 1, iRows, iCols, iNbItemNew, &piNbItemRowNew, &piColPosNew, &pdblRealRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		x = 0;
		y = 0;
		for(i = 0 ; i < iRows ; i++)
		{
			piNbItemRowNew[i] = 0;
			for(j = 0 ; j < piNbItemRow[i] ; j++)
			{
				if(pdblImg[x] != 0)
				{
					piNbItemRowNew[i]++;
					piColPosNew[y] = piColPos[x];
					pdblRealRet[y] = pdblImg[x];
					y++;
				}
				x++;
			}
		}
	}
	else
	{
		sciErr = getVarDimension(pvApiCtx, _piAddress, &iRows, &iCols);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = allocSparseMatrix(pvApiCtx, Rhs + 1, iRows, iCols, 0, &piNbItemRowNew, &piColPosNew, &pdblRealRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}
	return sciErr;
}
/*--------------------------------------------------------------------------*/
