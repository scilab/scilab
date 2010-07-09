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

SciErr isreal_double(int* _piAddress, double _dblRef, int* _piIsReal);
SciErr isreal_poly(int* _piAddress, double _dblRef, int* _piIsReal);
SciErr isreal_sparse(int* _piAddress, double _dblRef, int* _piIsReal);
SciErr isreal_common(double *_pdblData, int _iSize, double _dblRef, int* _piIsReal);

/*--------------------------------------------------------------------------*/
int sci_isreal(char *fname,unsigned long fname_len)
{
	SciErr sciErr;
	int iRows					= 0;
	int iCols					= 0;
	int iType1				= 0;
	int* piAddr1			= NULL;

	int iType2 				= 0;
	int* piAddr2			= NULL;
	int* piBool				= NULL;

	double *pdblPrec	= NULL;

	CheckRhs(1,2);
	CheckLhs(1,1);

	sciErr  = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarType(pvApiCtx, piAddr1, &iType1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(iType1 != sci_matrix && iType1 != sci_poly && iType1 != sci_sparse)
	{
		OverLoad(1);
		return 0;
	}

	sciErr = allocMatrixOfBoolean(pvApiCtx, Rhs + 1, 1, 1, &piBool);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(Rhs == 1)
	{
		if(isVarComplex(pvApiCtx, piAddr1))
			piBool[0] = 0;
		else
			piBool[0] = 1;
	}
	else //Rhs == 2
	{
		double dblPrec = 0;
		sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = getVarType(pvApiCtx, piAddr2, &iType2);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		if(iType2 != sci_matrix)
		{
			SciError(53);
			return 0;
		}

		sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols, &pdblPrec);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		if(!isVarComplex(pvApiCtx, piAddr1)) //Not complex
		{
			piBool[0] = 1;
		}
		else //Complex
		{
			switch(iType1)
			{
			case sci_matrix :
				sciErr = isreal_double(piAddr1, pdblPrec[0], piBool);
				break;
			case sci_poly :
				sciErr = isreal_poly(piAddr1, pdblPrec[0], piBool);
				break;
			case sci_sparse :
				sciErr = isreal_sparse(piAddr1, pdblPrec[0], piBool);
				break;
			default: //never come here
				break;
			}
		}
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

SciErr isreal_double(int* _piAddress, double _dblRef, int* _piIsReal)
{
	SciErr sciErr;
	int iRows					= 0;
	int iCols					= 0;

	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	sciErr = getComplexMatrixOfDouble(pvApiCtx, _piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	sciErr = isreal_common(pdblImg, iCols * iRows, _dblRef, _piIsReal);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	return sciErr;
}

SciErr isreal_poly(int* _piAddress, double _dblRef, int* _piIsReal)
{
	SciErr sciErr;
	int i;
	int iRows					= 0;
	int iCols					= 0;
	int *piCoeff			= NULL;

	double** pdblReal	= NULL;
	double** pdblImg	= NULL;

	sciErr = getComplexMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, NULL, NULL, NULL);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	piCoeff = (int*)malloc(sizeof(int) * iRows * iCols);
	sciErr = getComplexMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, piCoeff, NULL, NULL);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	pdblReal	= (double**)malloc(sizeof(double*) * iRows * iCols);
	pdblImg		= (double**)malloc(sizeof(double*) * iRows * iCols);

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pdblReal[i]	= (double*)malloc(sizeof(double) * piCoeff[i]);
		pdblImg[i]	= (double*)malloc(sizeof(double) * piCoeff[i]);
	}

	sciErr = getComplexMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, piCoeff, pdblReal, pdblImg);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		sciErr = isreal_common(pdblImg[i], piCoeff[i], _dblRef, _piIsReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		if(*_piIsReal == 0)
		{
			break;
		}
	}
	return sciErr;
}

SciErr isreal_sparse(int* _piAddress, double _dblRef, int* _piIsReal)
{
	SciErr sciErr;
	int iRet					= 0;
	int iRows					= 0;
	int iCols					= 0;
	int iNbItem				= 0;
	int *piNbItemRow	= NULL;
	int *piColPos			= NULL;

	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	sciErr = getComplexSparseMatrix(pvApiCtx, _piAddress, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	sciErr = isreal_common(pdblImg, iNbItem, _dblRef, _piIsReal);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	return sciErr;
}

SciErr isreal_common(double *_pdblData, int _iSize, double _dblRef, int* _piIsReal)
{
	int i;
	SciErr sciErr;sciErr.iErr = 0;

	*_piIsReal = 1;
	for(i = 0 ; i < _iSize ; i++)
	{
		if(_pdblData[i] > _dblRef)
		{
			*_piIsReal = 0;
			break;
		}
	}
	return sciErr;
}
/*--------------------------------------------------------------------------*/
