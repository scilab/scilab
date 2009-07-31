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

extern int C2F(dscal)();

int conj_double(int* _piAddress);
int conj_poly(int* _piAddress);

int C2F(sci_conj) (char *fname,unsigned long fname_len)
{
	int iRet		= 0;
	int* piAddr	= NULL;

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
		iRet = conj_double(piAddr);
		break;
	case sci_poly : 
		iRet = conj_poly(piAddr);
		break;
	default : 
		OverLoad(1);
		return 0;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

int conj_double(int* _piAddress)
{
	int iRet						= 0;
	int iRows						= 0;
	int iCols						= 0;
	double *pdblReal		= NULL;
	double *pdblImg			= NULL;
	double *pdblRealRet	= NULL;
	double *pdblImgRet	= NULL;

	int iOne						= 1;
	int iSize						= 0;
	double dblCoef			= -1;

	if(isVarComplex(_piAddress))
	{
		iRet = getComplexMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
		if(iRet)
		{
			return 1;
		}

		iSize = iRows * iCols;

		iRet = allocComplexMatrixOfDouble(Rhs + 1, iRows, iCols, &pdblRealRet, &pdblImgRet);
		if(iRet)
		{
			return 1;
		}

		memcpy(pdblRealRet, pdblReal, iSize * sizeof(double));
		memcpy(pdblImgRet, pdblImg, iSize * sizeof(double));

		C2F(dscal)(&iSize, &dblCoef, pdblImgRet, &iOne);
	}
	else
	{//do nothing, just copy in a new variable
		iRet = getMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblReal);
		if(iRet)
		{
			return 1;
		}

		iRet = createMatrixOfDouble(Rhs + 1, iRows, iCols, pdblReal);
		if(iRet)
		{
			return 1;
		}
	}
	return 0;
}

int conj_poly(int* _piAddress)
{
	int i;
	int iRet							= 0;
	int iRows							= 0;
	int iCols							= 0;
	int *piCoeff					= NULL;
	double** pdblReal			= NULL;
	double** pdblImg			= NULL;
	double** pdblRealRet	= NULL;
	double** pdblImgRet		= NULL;
	char pstVarName[16];

	int iOne							= 1;
	int iSize							= 0;
	double dblCoef				= -1;

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
			memcpy(pdblRealRet[i], pdblReal[i], piCoeff[i] * sizeof(double));
			memcpy(pdblImgRet[i], pdblImg[i], piCoeff[i] * sizeof(double));
			C2F(dscal)(&piCoeff[i], &dblCoef, pdblImgRet[i], &iOne);
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
		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblReal[i] = (double*)malloc(sizeof(double) * piCoeff[i]);
		}

		iRet = getMatrixOfPoly(_piAddress, &iRows, &iCols, piCoeff, pdblReal);
		if(iRet)
		{
			return 1;
		}

		iRet = createMatrixOfPoly(Rhs + 1, pstVarName, iRows, iCols, piCoeff, pdblReal);
		if(iRet)
		{
			return 1;
		}
	}

	//free memory
	free(piCoeff);

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		free(pdblReal[i]);
	}
	free(pdblReal);

	if(isVarComplex(_piAddress))
	{
		for(i = 0 ; i < iRows * iCols ; i++)
		{
			free(pdblRealRet[i]);
			free(pdblImg[i]);
			free(pdblImgRet[i]);
		}
		free(pdblRealRet);
		free(pdblImg);
		free(pdblImgRet);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
