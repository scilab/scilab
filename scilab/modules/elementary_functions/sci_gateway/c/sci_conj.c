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
#include "api_oldstack.h"

extern int C2F(dscal)();

SciErr conj_double(int*_piKey, int* _piAddress);
SciErr conj_poly(int*_piKey, int* _piAddress);

int sci_conj(char *fname, int* _piKey)
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
		sciErr = conj_double(_piKey, piAddr);
		break;
	case sci_poly :
		sciErr = conj_poly(_piKey, piAddr);
		break;
	default :
		OverLoad(1);
		return 0;
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

SciErr conj_double(int*_piKey, int* _piAddress)
{
	SciErr sciErr;
	int iRows						= 0;
	int iCols						= 0;
	double *pdblReal		= NULL;
	double *pdblImg			= NULL;
	double *pdblRealRet	= NULL;
	double *pdblImgRet	= NULL;

	int iOne						= 1;
	int iSize						= 0;
	double dblCoef			= -1;

	if(isVarComplex(_piKey, _piAddress))
	{
		sciErr = getComplexMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		iSize = iRows * iCols;

		sciErr = allocComplexMatrixOfDouble(_piKey, Rhs + 1, iRows, iCols, &pdblRealRet, &pdblImgRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		memcpy(pdblRealRet, pdblReal, iSize * sizeof(double));
		memcpy(pdblImgRet, pdblImg, iSize * sizeof(double));

		C2F(dscal)(&iSize, &dblCoef, pdblImgRet, &iOne);
	}
	else
	{//do nothing, just copy in a new variable
		sciErr = getMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = createMatrixOfDouble(_piKey, Rhs + 1, iRows, iCols, pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}
	return sciErr;
}

SciErr conj_poly(int*_piKey, int* _piAddress)
{
	SciErr sciErr;
	int i;
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
		pdblRealRet	= (double**)malloc(sizeof(double*) * iRows * iCols);
		pdblImgRet	= (double**)malloc(sizeof(double*) * iRows * iCols);

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblReal[i]			= (double*)malloc(sizeof(double) * piCoeff[i]);
			pdblImg[i]			= (double*)malloc(sizeof(double) * piCoeff[i]);
			pdblRealRet[i]	= (double*)malloc(sizeof(double) * piCoeff[i]);
			pdblImgRet[i]		= (double*)malloc(sizeof(double) * piCoeff[i]);
		}

		sciErr = getComplexMatrixOfPoly(_piKey, _piAddress, &iRows, &iCols, piCoeff, pdblReal, pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			memcpy(pdblRealRet[i], pdblReal[i], piCoeff[i] * sizeof(double));
			memcpy(pdblImgRet[i], pdblImg[i], piCoeff[i] * sizeof(double));
			C2F(dscal)(&piCoeff[i], &dblCoef, pdblImgRet[i], &iOne);
		}

		sciErr = createComplexMatrixOfPoly(_piKey, Rhs + 1, pstVarName, iRows, iCols, piCoeff, pdblRealRet, pdblImgRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}
	else
	{
		sciErr = getMatrixOfPoly(_piKey, _piAddress, &iRows, &iCols, NULL, NULL);
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

		pdblReal		= (double**)malloc(sizeof(double*) * iRows * iCols);
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

	//free memory
	free(piCoeff);

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		free(pdblReal[i]);
	}
	free(pdblReal);

	if(isVarComplex(_piKey, _piAddress))
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
	return sciErr;
}
/*--------------------------------------------------------------------------*/
