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
#include "Scierror.h"
#include "api_scilab.h"
#include "api_oldstack.h"

/*--------------------------------------------------------------------------*/
int sum_matrix(int* _piAddress, int _iMode, int* _piKey);
int sum_sparse(int* _piAddress, int* _piKey);

/*--------------------------------------------------------------------------*/
int sci_sum(char *fname, int* _piKey)
{
	int iRet		= 0;
	int iMode		= 0;
	int* piAddr = NULL;

	CheckRhs(1,2);
	CheckLhs(1,1);

	iRet = getVarAddressFromPosition(1, &piAddr, _piKey);
	if(iRet)
	{
		return 1;
	}

	//manage "c", "r", "m"
	if(Rhs == 2)
	{
		int *piAddr2		= NULL;
		iRet = getVarAddressFromPosition(2, &piAddr2, _piKey);
		if(iRet)
		{
			return 1;
		}

		iRet = getProcessMode(piAddr2, piAddr, &iMode);
		if(iRet)
		{
			return 1;
		}
	}

	switch(getVarType(piAddr))
	{
	case sci_matrix :
		iRet = sum_matrix(piAddr, iMode, _piKey);
		break;
	case sci_poly :
		//call psum in poly module
		break;
	case sci_sparse :
		if(iMode == 0)
		{
			iRet = sum_sparse(piAddr, _piKey);
		}
		else
		{
			OverLoad(1);
			return 0;
		}
		break;
	default :
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

int sum_sparse(int* _piAddress, int* _piKey)
{
	int iRows				= 0;
	int iCols				= 0;
	int iRealData			= 0;
	int iImgData			= 0;

	int iTotalElem		= 0;
	int *piElemByRow	= NULL;
	int *piColByRow		= NULL;

	double *pdblReal		= NULL;
	double *pdblImg			= NULL;
	double *pReturnRealData	= NULL;
	double *pReturnImgData	= NULL;

	if(iIsComplex(1))
	{
		GetRhsCSparseVar(1, &iRows, &iCols, &iTotalElem, NULL, NULL, &iRealData, &iImgData);
		piElemByRow	= (int*)malloc(iRows * sizeof(int));
		piColByRow	= (int*)malloc(iTotalElem * sizeof(int));
		GetRhsCSparseVar(1, &iRows, &iCols, &iTotalElem, piElemByRow, piColByRow, &iRealData, &iImgData);

		pdblReal	= stk(iRealData);
		pdblImg		= stk(iImgData);

		iAllocComplexMatrixOfDouble(Rhs + 1, 1, 1, &pReturnRealData, &pReturnImgData);
		zdmsums(BY_ALL, pdblReal, pdblImg, iTotalElem, 1, pReturnRealData, pReturnImgData);
	}
	else
	{
		GetRhsSparseVar(1, &iRows, &iCols, &iTotalElem, NULL, NULL, &iRealData);
		piElemByRow	= (int*)malloc(iRows * sizeof(int));
		piColByRow	= (int*)malloc(iTotalElem * sizeof(int));
		GetRhsSparseVar(1, &iRows, &iCols, &iTotalElem, piElemByRow, piColByRow, &iRealData);

		pdblReal	= stk(iRealData);

		iAllocMatrixOfDouble(Rhs + 1, 1, 1, &pReturnRealData);
		ddmsums(BY_ALL, pdblReal, iTotalElem, 1, pReturnRealData);
	}

	free(piElemByRow);
	free(piColByRow);
	return 0;
}

int sum_matrix(int* _piAddress, int _iMode, int* _piKey)
{
	int iRet						= 0;
	int iRows						= 0;
	int iCols						= 0;

	double *pdblReal		= NULL;
	double *pdblImg			= NULL;

	int iRowsOut				= 0;
	int iColsOut				= 0;

	double *pdblRealRet	= NULL;
	double *pdblImgRet	= NULL;

	iRet = getVarDimension(_piAddress, &iRows, &iCols);
	if(iRet)
	{
		return 1;
	}

	if(iRows * iCols == 0)
	{
		if(_iMode == BY_ALL)
		{
			iRet = allocMatrixOfDouble(Rhs + 1, 1, 1, &pdblRealRet, _piKey);
			if(iRet)
			{
				return 1;
			}

			pdblRealRet[0]	= 0;
			return 0;
		}
		else
		{
			iRet = allocMatrixOfDouble(Rhs + 1, 0,0, &pdblRealRet, _piKey);
			if(iRet)
			{
				return 1;
			}
			return 0;
		}
	}

	if(isVarComplex(_piAddress))
	{
		iRet = getComplexMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
	}
	else
	{
		iRet = getMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblReal);
	}

	if(iRet)
	{
		return 1;
	}
	
	switch(_iMode)
	{
	case BY_ROWS :
		iRowsOut	= 1;
		iColsOut	= iCols;
		break;
	case BY_COLS :
		iRowsOut	= iRows;
		iColsOut	= 1;
		break;
	case BY_ALL :
		iRowsOut	= 1;
		iColsOut	= 1;
		break;
	default :
		Error(17);
		return 0;
	}

	if(isVarComplex(_piAddress))
	{
		iRet = allocComplexMatrixOfDouble(Rhs + 1, iRowsOut, iColsOut, &pdblRealRet, &pdblImgRet, _piKey);
		if(iRet)
		{
			return 1;
		}

		zdmsums(_iMode, pdblReal, pdblImg, iRows, iCols, pdblRealRet, pdblImgRet);
	}
	else
	{
		iRet = allocMatrixOfDouble(Rhs + 1, iRowsOut, iColsOut, &pdblRealRet, _piKey);
		if(iRet)
		{
			return 1;
		}

		ddmsums(_iMode, pdblReal, iRows, iCols, pdblRealRet);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
