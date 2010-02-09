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

/*--------------------------------------------------------------------------*/
int sum_matrix(int* _piKey, int* _piAddress, int _iMode);
int sum_sparse(int* _piKey, int* _piAddress);

/*--------------------------------------------------------------------------*/
int sci_sum(char *fname, int* _piKey)
{
	SciErr sciErr;
	int iRet		= 0;
	int iMode		= 0;
	int iType		= 0;
	int* piAddr = NULL;

	CheckRhs(1,2);
	CheckLhs(1,1);

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	//manage "c", "r", "m"
	if(Rhs == 2)
	{
		sciErr = getProcessMode(_piKey, 2, piAddr, &iMode);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
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
		iRet = sum_matrix(_piKey, piAddr, iMode);
		break;
	case sci_poly :
		//call psum in poly module
		break;
	case sci_sparse :
		if(iMode == 0)
		{
			iRet = sum_sparse(_piKey, piAddr);
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
	int iRet						= 0;

	int iRows						= 0;
	int iCols						= 0;
	int iTotalElem			= 0;
	int *piElemByRow		= NULL;
	int *piColByRow			= NULL;
	double *pdblReal		= NULL;
	double *pdblImg			= NULL;
	
	double dblRealOut		= 0;
	double dblImgOut		=	0;

	if(isVarComplex(_piKey, _piAddress))
	{
		iRet = getAllocatedComplexSparseMatrix(_piKey, _piAddress, &iRows, &iCols, &iTotalElem, &piElemByRow, &piColByRow, &pdblReal, &pdblImg);
		if(iRet)
		{
			freeAllocatedComplexSparseMatrix(piElemByRow, piColByRow, pdblReal, pdblImg);
			return 0;
		}

		zdmsums(BY_ALL, pdblReal, pdblImg, iTotalElem, 1, &dblRealOut, &dblImgOut);

		createScalarComplexDouble(_piKey, Rhs + 1, dblRealOut, dblImgOut);
		if(iRet)
		{
			return 0;
		}
		freeAllocatedComplexSparseMatrix(piElemByRow, piColByRow, pdblReal, pdblImg);
	}
	else
	{
		iRet = getAllocatedSparseMatrix(_piKey, _piAddress, &iRows, &iCols, &iTotalElem, &piElemByRow, &piColByRow, &pdblReal);
		if(iRet)
		{
			freeAllocatedSparseMatrix(piElemByRow, piColByRow, pdblReal);
			return 0;
		}

		ddmsums(BY_ALL, pdblReal, iTotalElem, 1, &dblRealOut);

		createScalarDouble(_piKey, Rhs + 1, dblRealOut);
		if(iRet)
		{
			return 0;
		}

		freeAllocatedSparseMatrix(piElemByRow, piColByRow, pdblReal);
	}

	free(piElemByRow);
	free(piColByRow);
	return 0;
}

int sum_matrix(int* _piKey, int* _piAddress, int _iMode)
{
	SciErr sciErr;
	int iRet						= 0;
	int iRows						= 0;
	int iCols						= 0;

	double *pdblReal		= NULL;
	double *pdblImg			= NULL;

	int iRowsOut				= 0;
	int iColsOut				= 0;

	double *pdblRealRet	= NULL;
	double *pdblImgRet	= NULL;

	sciErr = getVarDimension(_piKey, _piAddress, &iRows, &iCols);
	if(iRet)
	{
		return 1;
	}

	if(isEmptyMatrix(_piKey, _piAddress))
	{
		if(_iMode == BY_ALL)
		{
			iRet = createScalarDouble(_piKey, Rhs + 1, 0);
			if(iRet)
			{
				return 1;
			}
		}
		else
		{
			iRet = createEmptyMatrix(_piKey, Rhs + 1);
			if(iRet)
			{
				return 1;
			}
		}
		return 0;
	}

	if(isVarComplex(_piKey, _piAddress))
	{
		sciErr = getComplexMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
	}
	else
	{
		sciErr = getMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblReal);
	}

	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
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

	if(isVarComplex(_piKey, _piAddress))
	{
		sciErr = allocComplexMatrixOfDouble(_piKey, Rhs + 1, iRowsOut, iColsOut, &pdblRealRet, &pdblImgRet);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		zdmsums(_iMode, pdblReal, pdblImg, iRows, iCols, pdblRealRet, pdblImgRet);
	}
	else
	{
		sciErr = allocMatrixOfDouble(_piKey, Rhs + 1, iRowsOut, iColsOut, &pdblRealRet);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		ddmsums(_iMode, pdblReal, iRows, iCols, pdblRealRet);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
