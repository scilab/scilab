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

int abs_double(int* _piAddress);
int abs_poly(int* _piAddress);
int abs_sparse(int* _piAddress);

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
int C2F(sci_abs) (char *fname,unsigned long fname_len)
{
	int iRet			= 0;
	int* piAddr		= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	iRet = getVarAddressFromPosition(1, &piAddr);
	if(iRet)
	{
		return 1;
	}

	switch(getVarType(piAddr))
	{
	case sci_matrix:
		iRet = abs_double(piAddr);
		break;
	case sci_poly:
		iRet = abs_poly(piAddr);
		break;
	case sci_sparse:
		iRet = abs_sparse(piAddr);
		break;
	default:
		OverLoad(1);
		break;
	}

	if(iRet)
	{
		return 1;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

/*Absolute value for a double*/
int abs_double(int* _piAddress)
{
	int i;
	int iRet						= 0;
	int iRows						= 0;
	int iCols						= 0;
	double *pdblReal		= NULL;
	double* pdblImg			= NULL;

	double* pdblRealRet	= NULL;

	if(isVarComplex(_piAddress))
	{
		iRet = getComplexMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
		if(iRet)
		{
			return 1;
		}

		allocMatrixOfDouble(Rhs + 1, iRows, iCols, &pdblRealRet);
		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblRealRet[i] = dabsz(pdblReal[i], pdblImg[i]);
		}
	}
	else
	{
		iRet = getMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblReal);
		if(iRet)
		{
			return 1;
		}

		allocMatrixOfDouble(Rhs + 1, iRows, iCols, &pdblRealRet);
		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblRealRet[i] = dabss(pdblReal[i]);
		}
	}
	return 0;
}

/*Absolute value for a polynomial ( absolute value of each coefficient )*/
int abs_poly(int* _piAddress)
{
	int i,j;
	int iLen							= 0;
	int iRet							= 0;
	int iRows							= 0;
	int iCols							= 0;
	int *piCoeff					= NULL;
	char pstVarName[16];

	double** pdblReal			= NULL;
	double** pdblImg			= NULL;
	double** pdblRealRet	= NULL;

	iRet = getPolyVariableName(_piAddress, pstVarName, &iLen);
	if(iRet)
	{
		return 1;
	}

	if(isVarComplex(_piAddress))
	{
		iRet = getComplexMatrixOfPoly(_piAddress, &iRows, &iCols, NULL, NULL, NULL);
		if(iRet)
		{
			return 1;
		}

		piCoeff	= (int*)malloc(iRows * iCols * sizeof(int));
		iRet = getComplexMatrixOfPoly(_piAddress, &iRows, &iCols, piCoeff, NULL, NULL);
		if(iRet)
		{
			return 1;
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

		iRet = getComplexMatrixOfPoly(_piAddress, &iRows, &iCols, piCoeff, pdblReal, pdblImg);
		for(i = 0 ; i < iRows * iCols ; i++)
		{
			for(j = 0 ; i < piCoeff[i] ; j++)
			{
				pdblRealRet[i][j] = dabsz(pdblReal[i][j], pdblImg[i][j]);
			}
		}

		iRet = createMatrixOfPoly(Rhs + 1, pstVarName, iRows, iCols, piCoeff, pdblRealRet);
	}
	else
	{
		iRet = getMatrixOfPoly(_piAddress, &iRows, &iCols, NULL, NULL);
		if(iRet)
		{
			return 1;
		}

		piCoeff	= (int*)malloc(iRows * iCols * sizeof(int));
		iRet = getMatrixOfPoly(_piAddress, &iRows, &iCols, piCoeff, NULL);
		if(iRet)
		{
			return 1;
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

		iRet = createMatrixOfPoly(Rhs + 1, pstVarName, iRows, iCols, piCoeff, pdblRealRet);
	}

	if(iRet)
	{
		return 1;
	}

	free(piCoeff);
	for(i = 0 ; i < iRows * iCols ; i++)
	{
		free(pdblReal[i]);
		free(pdblRealRet[i]);
	}
	free(pdblReal);
	free(pdblRealRet);

	if(isVarComplex(_piAddress))
	{
		for(i = 0 ; i < iRows * iCols ; i++)
		{
			free(pdblImg[i]);
		}
		free(pdblImg);
	}
	return 0;
}

/*Absolute value for a sparse ( absolute value of each element )*/
int abs_sparse(int* _piAddress)
{
	int i;
	int iRet						= 0;
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

	if(isVarComplex(_piAddress))
	{
		iRet = getComplexSparseMatrix(_piAddress, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
		if(iRet)
		{
			return 1;
		}

		pdblRealRet = (double*)malloc(sizeof(double) * iNbItem);

		for(i = 0 ; i < iNbItem ; i++)
		{
			pdblRealRet[i] = dabsz(pdblReal[i], pdblImg[i]);
		}

		iRet = createSparseMatrix(Rhs + 1, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblRealRet);
	}
	else
	{
		iRet = getSparseMatrix(_piAddress, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal);
		if(iRet)
		{
			return 1;
		}

		pdblRealRet = (double*)malloc(sizeof(double) * iNbItem);

		for(i = 0 ; i < iNbItem ; i++)
		{
			pdblRealRet[i] = dabss(pdblReal[i]);
		}

		iRet = createSparseMatrix(Rhs + 1, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblRealRet);
	}

	if(iRet)
	{
		return 1;
	}
	return 0;
}

/*--------------------------------------------------------------------------*/
