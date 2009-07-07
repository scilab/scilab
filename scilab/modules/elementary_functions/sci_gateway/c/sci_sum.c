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

/*--------------------------------------------------------------------------*/
int sum_matrix(int _iMode);
int sum_sparse();

/*--------------------------------------------------------------------------*/
int C2F(sci_sum) (char *fname,unsigned long fname_len)
{
	int iType				= 0;
	int iMode				= 0;


	CheckRhs(1,2);
	CheckLhs(1,1);

	iType = GetType(1);

	switch(iType)
	{
	case sci_matrix :
		if(Rhs == 2)
			iMode = iGetOrient(2);
		sum_matrix(iMode);
		break;
	case sci_poly :
		break;
	case sci_sparse :
		if(Rhs == 2)
			iMode = iGetOrient(2);
		if(iMode == 0)
			sum_sparse();
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
	return 0;
}

int sum_sparse()
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

	LhsVar(1) = Rhs + 1;
	PutLhsVar();

	free(piElemByRow);
	free(piColByRow);
	return 0;
}

int sum_matrix(int _iMode)
{
	int iRows				= 0;
	int iCols				= 0;
	int iReturnRows			= 0;
	int iReturnCols			= 0;
	int iRealData			= 0;
	int iImgData			= 0;
	int iIndex				= 0;

	double *pdblReal		= NULL;
	double *pdblImg			= NULL;
	double *pReturnRealData	= NULL;
	double *pReturnImgData	= NULL;

	if(iIsComplex(1))
	{
		int iComplex	= 1;
		GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &iRealData, &iImgData);
		pdblReal		= stk(iRealData);
		pdblImg			= stk(iImgData);
	}
	else
	{
		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
		pdblReal		= stk(iRealData);
	}

	if(iRows * iCols == 0)
	{
		if(_iMode == BY_ALL)
		{
			iAllocMatrixOfDouble(Rhs + 1, 1, 1, &pReturnRealData);
			pReturnRealData[0]	= 0;
			LhsVar(1)			= Rhs + 1;
			PutLhsVar();
			return 0;
		}
		else
		{
			iAllocMatrixOfDouble(Rhs + 1, 0, 0, &pReturnRealData);
			LhsVar(1) = Rhs + 1;
			PutLhsVar();
			return 0;
		}
	}

	switch(_iMode)
	{
	case BY_ROWS :
		iReturnRows	= 1;
		iReturnCols	= iCols;
		break;
	case BY_COLS :
		iReturnRows	= iRows;
		iReturnCols	= 1;
		break;
	case BY_ALL :
		iReturnRows	= 1;
		iReturnCols	= 1;
		break;
	default :
		Error(17);
		return 0;
	}

	if(iIsComplex(1))
	{
		iAllocComplexMatrixOfDouble(Rhs + 1, iReturnRows, iReturnCols, &pReturnRealData, &pReturnImgData);
		zdmsums(_iMode, pdblReal, pdblImg, iRows, iCols, pReturnRealData, pReturnImgData);
	}
	else
	{
		iAllocMatrixOfDouble(Rhs + 1, iReturnRows, iReturnCols, &pReturnRealData);
		ddmsums(_iMode, pdblReal, iRows, iCols, pReturnRealData);
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
