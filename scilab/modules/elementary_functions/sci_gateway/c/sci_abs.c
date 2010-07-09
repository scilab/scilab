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
#include "api_scilab.h"
#include "basic_functions.h"

SciErr abs_double(int* _piAddress);
SciErr abs_poly(int* _piAddress);
SciErr abs_sparse(int* _piAddress);

/*--------------------------------------------------------------------------*/
int sci_abs(char *fname,unsigned long fname_len)
{
	SciErr sciErr;
	int iType			= 0;
	int* piAddr		= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarType(pvApiCtx, piAddr, &iType);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	switch(iType)
	{
	case sci_matrix:
		sciErr = abs_double(piAddr);
		break;
	case sci_poly:
		sciErr = abs_poly(piAddr);
		break;
	case sci_sparse:
		sciErr = abs_sparse(piAddr);
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

/*Absolute value for a double*/
SciErr abs_double(int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRows						= 0;
	int iCols						= 0;
	double *pdblReal		= NULL;
	double* pdblImg			= NULL;

	double* pdblRealRet	= NULL;

	if(isVarComplex(pvApiCtx, _piAddress))
	{
		sciErr = getComplexMatrixOfDouble(pvApiCtx, _piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblRealRet[i] = dabsz(pdblReal[i], pdblImg[i]);
		}
	}
	else
	{
		sciErr = getMatrixOfDouble(pvApiCtx, _piAddress, &iRows, &iCols, &pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblRealRet[i] = dabss(pdblReal[i]);
		}
	}
	return sciErr;
}

/*Absolute value for a polynomial ( absolute value of each coefficient )*/
SciErr abs_poly(int* _piAddress)
{
	SciErr sciErr;
	int i,j;
	int iLen							= 0;
	int iRows							= 0;
	int iCols							= 0;
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
		pdblRealRet	= (double**)malloc(sizeof(double*) * iRows * iCols);

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblReal[i]			= (double*)malloc(sizeof(double) * piCoeff[i]);
			pdblImg[i]			= (double*)malloc(sizeof(double) * piCoeff[i]);
			pdblRealRet[i]	= (double*)malloc(sizeof(double) * piCoeff[i]);
		}

		sciErr = getComplexMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, piCoeff, pdblReal, pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			for(j = 0 ; i < piCoeff[i] ; j++)
			{
				pdblRealRet[i][j] = dabsz(pdblReal[i][j], pdblImg[i][j]);
			}
		}

		sciErr = createMatrixOfPoly(pvApiCtx, Rhs + 1, pstVarName, iRows, iCols, piCoeff, pdblRealRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}
	else
	{
		sciErr = getMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, NULL, NULL);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		piCoeff	= (int*)malloc(iRows * iCols * sizeof(int));
		sciErr = getMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, piCoeff, NULL);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		pdblReal		= (double**)malloc(sizeof(double*) * iRows * iCols);
		pdblRealRet	= (double**)malloc(sizeof(double*) * iRows * iCols);

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblReal[i]			= (double*)malloc(sizeof(double) * piCoeff[i]);
			pdblRealRet[i]	= (double*)malloc(sizeof(double) * piCoeff[i]);
		}

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			for(j = 0 ; i < piCoeff[i] ; j++)
			{
				pdblRealRet[i][j] = dabss(pdblReal[i][j]);
			}
		}

		sciErr = createMatrixOfPoly(pvApiCtx, Rhs + 1, pstVarName, iRows, iCols, piCoeff, pdblRealRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}

	free(piCoeff);
	for(i = 0 ; i < iRows * iCols ; i++)
	{
		free(pdblReal[i]);
		free(pdblRealRet[i]);
	}
	free(pdblReal);
	free(pdblRealRet);

	if(isVarComplex(pvApiCtx, _piAddress))
	{
		for(i = 0 ; i < iRows * iCols ; i++)
		{
			free(pdblImg[i]);
		}
		free(pdblImg);
	}
	return sciErr;
}

/*Absolute value for a sparse ( absolute value of each element )*/
SciErr abs_sparse(int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRows						= 0;
	int iCols						= 0;
	int iNbItem					= 0;
	int *piNbItemRow		= NULL;
	int *piColPos				= NULL;

	int iIndex					= 0;
	int iIndex2					= 0;
	int iIndex3					= 0;

	double *pdblReal		= NULL;
	double *pdblImg			= NULL;
	double *pdblRealRet	= NULL;

	if(isVarComplex(pvApiCtx, _piAddress))
	{
		sciErr = getComplexSparseMatrix(pvApiCtx, _piAddress, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		pdblRealRet = (double*)malloc(sizeof(double) * iNbItem);

		for(i = 0 ; i < iNbItem ; i++)
		{
			pdblRealRet[i] = dabsz(pdblReal[i], pdblImg[i]);
		}

		sciErr = createSparseMatrix(pvApiCtx, Rhs + 1, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblRealRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}
	else
	{
		sciErr = getSparseMatrix(pvApiCtx, _piAddress, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		pdblRealRet = (double*)malloc(sizeof(double) * iNbItem);

		for(i = 0 ; i < iNbItem ; i++)
		{
			pdblRealRet[i] = dabss(pdblReal[i]);
		}

		sciErr = createSparseMatrix(pvApiCtx, Rhs + 1, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblRealRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}

	return sciErr;
}

/*--------------------------------------------------------------------------*/
