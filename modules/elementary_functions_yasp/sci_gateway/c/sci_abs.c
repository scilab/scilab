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
#include "stdlib.h"

int abs_double();
int abs_poly();
int abs_sparse();

/*--------------------------------------------------------------------------*/
extern int C2F(intabs) _PARAMS((int *id));
extern double dabss(double _dblVal);
extern double dabsz(double _dblRealVal, double _dblImgVal);
/*--------------------------------------------------------------------------*/
int C2F(sci_abs) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_
	CheckRhs(1,1);
	CheckLhs(1,1);
	switch(GetType(1))
	{
	case sci_matrix:
		{
			abs_double();
			break;
		}
	case sci_poly:
		{
			abs_poly();
			break;
		}
	case sci_sparse:
		{
			abs_sparse();
			break;
		}
	default:;
	}


#else
	C2F(intabs)(id);
#endif
	return 0;
}

int abs_double()
{
	int iRows = 0;
	int iCols = 0;
	int iRealData = 0;
	int iImgData = 0;
	int iIndex = 0;

	if(iIsComplex(1))
	{
		double *pdblRealData	= 0;
		double *pdblImgData		= 0;
		double *pReturnRealData	= NULL;
		int		iComplex		= 1;

		GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &iRealData, &iImgData);

		pdblRealData	= stk(iRealData);
		pdblImgData		= stk(iImgData);

		pReturnRealData = (double*)malloc(iRows * iCols * sizeof(double));
//		pReturnImgData	= (double*)malloc(iRows * iCols * sizeof(double));

		for(iIndex = 0 ; iIndex < iRows * iCols ; iIndex++)
			pReturnRealData[iIndex] = dabsz(pdblRealData[iIndex], pdblImgData[iIndex]);

		CreateVarFromPtr(2, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
		LhsVar(1) = 2;
		PutLhsVar();
		free(pReturnRealData);
	}
	else
	{
		double *pdblRealData	= 0;
		double *pReturnRealData = NULL;
		double *pReturnImgData	= NULL;
		int		itr				= 0;

		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
		pdblRealData		= stk(iRealData);
		pReturnRealData		= (double*)malloc(iRows * iCols * sizeof(double));

		for(iIndex = 0 ; (iIndex < iRows * iCols) && (itr == 0) ; iIndex++)
			pReturnRealData[iIndex] = dabss(pdblRealData[iIndex]);

		CreateVarFromPtr(2, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
		LhsVar(1) = 2;
		PutLhsVar();
		free(pReturnRealData);
	}

	return 0;
}

int abs_poly()
{
	int iRows = 0;
	int iCols = 0;
	int iRealData = 0;
	int iImgData = 0;
	int iIndex = 0;
	int *piPow = NULL;
	int *piVarName = NULL;
	int iMaxData = 0;

	double *pdblRealData	= 0;
	double *pdblImgData		= 0;
	double *pReturnRealData	= NULL;
	double *pReturnImgData	= NULL;
	int		iComplex		= 1;

	if(iIsComplex(1))
	{
		GetRhsCPolyVar(1, &piVarName, &iRows, &iCols, NULL, &iRealData, &iImgData);
		piPow			= (int*)malloc(iRows * iCols * sizeof(int));
		GetRhsCPolyVar(1, &piVarName, &iRows, &iCols, piPow, &iRealData, &iImgData);
		pdblRealData	= stk(iRealData);
		pdblImgData		= stk(iImgData);
	}
	else
	{
		GetRhsPolyVar(1, &piVarName, &iRows, &iCols, NULL, &iRealData);
		piPow	= (int*)malloc(iRows * iCols * sizeof(int));
		GetRhsPolyVar(1, &piVarName, &iRows, &iCols, piPow, &iRealData);
		iMaxData = iArraySum(piPow, 0, iRows * iCols);

		pdblRealData	= stk(iRealData);
		pReturnRealData	= (double*)malloc(iMaxData * sizeof(double));
		

		for(iIndex = 0 ; iIndex < iMaxData ; iIndex++)
		{
			pReturnRealData[iIndex] = dabss(pdblRealData[iIndex]);
		}

		CreatePolyVarFromPtr(2, &piVarName, MATRIX_OF_POLYNOMIAL_DATATYPE, iRows, iCols, piPow, pReturnRealData);
		LhsVar(1) = 2;
		PutLhsVar();
		free(pReturnRealData);
		free(piPow);
	}
	return 0;
}

int abs_sparse()
{
	return 0;
}

/*--------------------------------------------------------------------------*/
