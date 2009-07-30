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

int int_double(int* _piAddress);
int int_poly(int* _piAddress);
/*--------------------------------------------------------------------------*/
int C2F(sci_int) (char *fname,unsigned long fname_len)
{
	int iRet						= 0;

	int* piAddr					= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	iRet = getVarAddressFromPosition(1, &piAddr);
	if(iRet)
	{
		return 1;
	}

	switch(getVarType(piAddr))
	{
	case sci_matrix :
		iRet = int_double(piAddr);
		break;
	case sci_poly :
		iRet = int_poly(piAddr);
	default :
		OverLoad(1);
		return 0;
	}

	if(iRet)
	{
		return 1;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

int int_double(int* _piAddress)
{
	int i;
	int iRet						= 0;
	int iRows						= 0;
	int iCols						= 0;

	double *pdblReal		= NULL;
	double *pdblImg			= NULL;
	double *pdblRealRet = NULL;
	double *pdblImgRet	= NULL;

	if(isVarComplex(_piAddress))
	{
		iRet = getComplexMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
		if(iRet)
		{
			return 1;
		}

		iRet = allocComplexMatrixOfDouble(Rhs + 1, iRows, iCols, &pdblRealRet, &pdblImgRet);
		if(iRet)
		{
			return 1;
		}

		for(i = 0 ; i < iCols * iRows ; i++)
		{
			pdblRealRet[i]	= (double)(int)pdblReal[i];
			pdblImgRet[i]		= (double)(int)pdblImg[i];
		}
	}
	else
	{
		iRet = getMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblReal);
		if(iRet)
		{
			return 1;
		}

		iRet = allocMatrixOfDouble(Rhs + 1, iRows, iCols, &pdblRealRet);
		if(iRet)
		{
			return 1;
		}

		for(i = 0 ; i < iCols * iRows ; i++)
		{
			pdblRealRet[i]	= (double)(int)pdblReal[i];
		}
	}
	return 0;
}

int int_poly(int* _piAddress)
{
	int i,j;
	int iRet							= 0;
	int iRows							= 0;
	int iCols							= 0;
	int iLen							= 0;
	int* piCoeff					= NULL;

	char pstVarName[16];

	double** pdblReal			= NULL;
	double** pdblImg			= NULL;
	double** pdblRealRet	= NULL;
	double** pdblImgRet		= NULL;

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
		pdblImgRet	= (double**)malloc(sizeof(double*) * iRows * iCols);

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblReal[i]			= (double*)malloc(sizeof(double) * piCoeff[i]);
			pdblImg[i]			= (double*)malloc(sizeof(double) * piCoeff[i]);
			pdblRealRet[i]	= (double*)malloc(sizeof(double) * piCoeff[i]);
			pdblImgRet[i]		= (double*)malloc(sizeof(double) * piCoeff[i]);
		}

		iRet = getComplexMatrixOfPoly(_piAddress, &iRows, &iCols, piCoeff, pdblReal, pdblImg);
		if(iRet)
		{
			return 1;
		}

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			for(j = 0 ; j < piCoeff[i] ; j++)
			{
				pdblRealRet[i][j] = pdblReal[i][j];
				pdblImgRet[i][j]	= pdblImg[i][j];
			}
		}

		iRet = createComplexMatrixOfPoly(Rhs + 1, pstVarName, iRows, iCols, piCoeff, pdblRealRet, pdblImgRet);
		if(iRet)
		{
			return 1;
		}
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

		iRet = getMatrixOfPoly(_piAddress, &iRows, &iCols, piCoeff, pdblReal);
		if(iRet)
		{
			return 1;
		}

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			for(j = 0 ; j < piCoeff[i] ; j++)
			{
				pdblRealRet[i][j] = pdblReal[i][j];
			}
		}

		iRet = createMatrixOfPoly(Rhs + 1, pstVarName, iRows, iCols, piCoeff, pdblRealRet);
		if(iRet)
		{
			return 1;
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

	if(isVarComplex(_piAddress))
	{
		for(i = 0 ; i < iRows * iCols ; i++)
		{
			free(pdblImg[i]);
			free(pdblImgRet[i]);
		}
		free(pdblImg);
		free(pdblImgRet);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
