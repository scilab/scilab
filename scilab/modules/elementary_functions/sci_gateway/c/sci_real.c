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
#include "Scierror.h"

SciErr real_poly(int* _piKey, int* _piAddress);
SciErr real_sparse(int* _piKey, int* _piAddress);
SciErr real_double(int* _piKey, int* _piAddress);

/*--------------------------------------------------------------------------*/
int sci_real(char *fname, int* _piKey)
{
	SciErr sciErr;
	int iType 		= 0;
	int* piAddr		= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarType(_piKey, piAddr, &iType);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	switch(iType)
	{
	case sci_matrix :
		sciErr = real_double(_piKey, piAddr);
		break;
	case sci_poly :
		sciErr = real_poly(_piKey, piAddr);
		break;
	case sci_sparse :
		sciErr = real_sparse(_piKey, piAddr);
		break;
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

SciErr real_poly(int* _piKey, int* _piAddress)
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

	sciErr = getPolyVariableName(_piKey, _piAddress, pstVarName, &iLen);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	if(isVarComplex(_piKey, _piAddress))
	{
		sciErr = getComplexMatrixOfPoly(_piKey, _piAddress, &iRows, &iCols, NULL, NULL, NULL);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		piCoeff	= (int*)malloc(iRows * iCols * sizeof(int));
		sciErr = getComplexMatrixOfPoly(_piKey, _piAddress, &iRows, &iCols, piCoeff, NULL, NULL);
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

		sciErr = getComplexMatrixOfPoly(_piKey, _piAddress, &iRows, &iCols, piCoeff, pdblReal, pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			int iCoeff = piCoeff[i];
			for(j = iCoeff - 1 ; j >= 0 ; j--)
			{
				if(pdblReal[i][j] == 0)
				{
					piCoeff[i]--;
				}
				else
				{
					break;
				}
			}
		}

		sciErr = createMatrixOfPoly(_piKey, Rhs + 1, pstVarName, iRows, iCols, piCoeff, pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}
	else
	{
		sciErr  = getMatrixOfPoly(_piKey, _piAddress, &iRows, &iCols, NULL, NULL);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		piCoeff	= (int*)malloc(iRows * iCols * sizeof(int));
		sciErr = getMatrixOfPoly(_piKey, _piAddress, &iRows, &iCols, piCoeff, NULL);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		pdblReal = (double**)malloc(sizeof(double*) * iRows * iCols);

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblReal[i] = (double*)malloc(sizeof(double) * piCoeff[i]);
		}

		sciErr = getMatrixOfPoly(_piKey, _piAddress, &iRows, &iCols, piCoeff, pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = createMatrixOfPoly(_piKey, Rhs + 1, pstVarName, iRows, iCols, piCoeff, pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}
	return sciErr;
}

SciErr real_sparse(int* _piKey, int* _piAddress)
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

	if(isVarComplex(_piKey, _piAddress))
	{
		sciErr = getComplexSparseMatrix(_piKey, _piAddress, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		for(i = 0 ; i < iNbItem ; i++)
		{
			if(pdblReal[i] != 0)
			{
				iNbItemNew++;
			}
		}

		sciErr = allocSparseMatrix(_piKey, Rhs + 1, iRows, iCols, iNbItemNew, &piNbItemRowNew, &piColPosNew, &pdblRealRet);
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
				if(pdblReal[x] != 0)
				{
					piNbItemRowNew[i]++;
					piColPosNew[y] = piColPos[x];
					pdblRealRet[y] = pdblReal[x];
					y++;
				}
				x++;
			}
		}
	}
	else
	{
		sciErr = getSparseMatrix(_piKey, _piAddress, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = createSparseMatrix(_piKey, Rhs + 1, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}
	return sciErr;
}

SciErr real_double(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int iRows						= 0;
	int iCols						= 0;
	double *pdblReal		= NULL;
	double *pdblImg			= NULL;
	double *pdblRealRet = NULL;

	if(isVarComplex(_piKey, _piAddress))
	{
		sciErr = getComplexMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}
	else
	{
		sciErr = getMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}

	sciErr = createMatrixOfDouble(_piKey, Rhs + 1, iRows, iCols, pdblReal);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	return sciErr;
}
/*--------------------------------------------------------------------------*/
