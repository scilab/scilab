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

SciErr round_double(int* _piAddress);
SciErr round_poly(int* _piAddress);

/*--------------------------------------------------------------------------*/
int sci_round(char *fname,unsigned long fname_len)
{
	SciErr sciErr;
	int iType 		= 0;
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
	case sci_matrix :
		sciErr = round_double(piAddr);
		break;
	case sci_poly :
		sciErr = round_poly(piAddr);
		break;
	default:
		OverLoad(1);
		return 0;
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

SciErr round_double(int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRows						= 0;
	int iCols						= 0;

	double *pdblReal		= NULL;
	double *pdblImg			= NULL;
	double *pdblRealRet	= NULL;
	double *pdblImgRet	= NULL;

	if(isVarComplex(pvApiCtx, _piAddress))
	{
		sciErr = getComplexMatrixOfDouble(pvApiCtx, _piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = allocComplexMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealRet, &pdblImgRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		for(i = 0 ; i < iRows * iCols; i++)
		{
			pdblRealRet[i] = danints(pdblReal[i]);
			pdblImgRet[i]	= danints(pdblImg[i]);
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

		for(i = 0 ; i < iRows * iCols; i++)
		{
			pdblRealRet[i] = danints(pdblReal[i]);
		}
	}

	return sciErr;
}

SciErr round_poly(int* _piAddress)
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
	double** pdblImgRet	= NULL;

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
		pdblImgRet	= (double**)malloc(sizeof(double*) * iRows * iCols);

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblReal[i]			= (double*)malloc(sizeof(double) * piCoeff[i]);
			pdblImg[i]			= (double*)malloc(sizeof(double) * piCoeff[i]);
			pdblRealRet[i]	= (double*)malloc(sizeof(double) * piCoeff[i]);
			pdblImgRet[i]		= (double*)malloc(sizeof(double) * piCoeff[i]);
		}

		sciErr = getComplexMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, piCoeff, pdblReal, pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			for(j = 0 ; j < piCoeff[i] ; j++)
			{
				pdblRealRet[i][j] = danints(pdblReal[i][j]);
				pdblImgRet[i][j] = danints(pdblImg[i][j]);
			}
		}

		sciErr = createComplexMatrixOfPoly(pvApiCtx, Rhs + 1, pstVarName, iRows, iCols, piCoeff, pdblRealRet, pdblImgRet);
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

		sciErr = getMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, piCoeff, pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			for(j = 0 ; j < piCoeff[i] ; j++)
			{
				pdblRealRet[i][j] = danints(pdblReal[i][j]);
			}
		}

		sciErr = createMatrixOfPoly(pvApiCtx, Rhs + 1, pstVarName, iRows, iCols, piCoeff, pdblRealRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}
	return sciErr;
}

/*--------------------------------------------------------------------------*/
