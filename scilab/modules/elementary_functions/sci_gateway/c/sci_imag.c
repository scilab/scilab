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

int img_double(int* _piAddress);
int img_poly(int* _piAddress);
int img_sparse(int* _piAddress);

/*--------------------------------------------------------------------------*/
int C2F(sci_imag) (char *fname,unsigned long fname_len)
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
		{
			iRet = img_double(piAddr);
			break;
		}
	case sci_poly:
		{
			iRet = img_poly(piAddr);
			break;
		}
	case sci_sparse:
		{
			iRet = img_sparse(piAddr);
			break;
		}
	default:
		OverLoad(1);
		break;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

int img_double(int* _piAddress)
{
	int iRet						= 0;
	int iRows						= 0;
	int iCols						= 0;

	double *pdblReal		= NULL;
	double *pdblImg			= NULL;
	double *pdblRealRet	= NULL;

	if(isVarComplex(_piAddress))
	{
		iRet = getComplexMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
		if(iRet)
		{
			return 1;
		}

		iRet = createMatrixOfDouble(Rhs + 1, iRows, iCols, pdblImg);
		if(iRet)
		{
			return 1;
		}
	}
	else
	{
		iRet = getVarDimension(_piAddress, &iRows, &iCols);
		if(iRet)
		{
			return 1;
		}

		iRet = allocMatrixOfDouble(Rhs + 1, iRows, iCols, &pdblRealRet);
		if(iRet)
		{
			return 1;
		}

		memset(pdblRealRet, 0x00, iRows * iCols * sizeof(double));
	}

	return 0;
}

int img_poly(int* _piAddress)
{
	int i,j;
	int iRet							= 0;
	int iRows							= 0;
	int iCols							= 0;
	int iLen							= 0;
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

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblReal[i]			= (double*)malloc(sizeof(double) * piCoeff[i]);
			pdblImg[i]			= (double*)malloc(sizeof(double) * piCoeff[i]);
		}

		iRet = getComplexMatrixOfPoly(_piAddress, &iRows, &iCols, piCoeff, pdblReal, pdblImg);
		if(iRet)
		{
			return 1;
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
		iRet = createMatrixOfPoly(Rhs + 1, pstVarName, iRows, iCols, piCoeff, pdblImg);
		if(iRet)
		{
			return 1;
		}
	}
	else
	{
		iRet = getVarDimension(_piAddress, &iRows, &iCols);
		if(iRet)
		{
			return 1;
		}

		piCoeff = (int*)malloc(sizeof(int) * iRows * iCols);
		memset(piCoeff, 0x00, sizeof(int) * iRows * iCols);
		pdblRealRet = (double**)malloc(sizeof(double*) * iRows * iCols);
		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblRealRet[i] = (double*)malloc(sizeof(double) * 1);
			pdblRealRet[i] = 0;
		}
		createMatrixOfPoly(Rhs + 1, pstVarName, iRows, iCols, piCoeff, pdblRealRet);
		if(iRet)
		{
			return 1;
		}
	}
	return 0;
}

int img_sparse(int* _piAddress)
{
	int i,j,x,y;
	int iRet						= 0;
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

	if(isVarComplex(_piAddress))
	{
		iRet = getComplexSparseMatrix(_piAddress, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
		if(iRet)
		{
			return 1;
		}

		for(i = 0 ; i < iNbItem ; i++)
		{
			if(pdblImg[i] != 0)
			{
				iNbItemNew++;
			}
		}

		iRet = allocSparseMatrix(Rhs + 1, iRows, iCols, iNbItemNew, &piNbItemRowNew, &piColPosNew, &pdblRealRet);

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
		iRet = getVarDimension(_piAddress, &iRows, &iCols);
		if(iRet)
		{
			return 1;
		}

		iRet = allocSparseMatrix(Rhs + 1, iRows, iCols, 0, &piNbItemRowNew, &piColPosNew, &pdblRealRet);
		if(iRet)
		{
			return 1;
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/