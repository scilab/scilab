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

int abs_double();
int abs_poly();
int abs_sparse();

/*--------------------------------------------------------------------------*/
extern int C2F(intabs) _PARAMS((int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_abs) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
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
	default:
		OverLoad(1);
		break;
	}
	return 0;
}

/*Absolute value for a double*/
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

		for(iIndex = 0 ; iIndex < iRows * iCols ; iIndex++)
			pReturnRealData[iIndex] = dabsz(pdblRealData[iIndex], pdblImgData[iIndex]);

		CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		free(pReturnRealData);
	}
	else
	{
		double *pdblRealData	= 0;
		double *pReturnRealData = NULL;
		int		itr				= 0;

		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
		pdblRealData		= stk(iRealData);
		pReturnRealData		= (double*)malloc(iRows * iCols * sizeof(double));

		for(iIndex = 0 ; (iIndex < iRows * iCols) && (itr == 0) ; iIndex++)
			pReturnRealData[iIndex] = dabss(pdblRealData[iIndex]);

		CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		free(pReturnRealData);
	}

	return 0;
}

/*Absolute value for a polynomial ( absolute value of each coefficient )*/
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

	if(iIsComplex(1))
	{
		GetRhsCPolyVar(1, &piVarName, &iRows, &iCols, NULL, &iRealData, &iImgData);
		piPow				= (int*)malloc(iRows * iCols * sizeof(int));
		GetRhsCPolyVar(1, &piVarName, &iRows, &iCols, piPow, &iRealData, &iImgData);
		iMaxData			= iArraySum(piPow, 0, iRows * iCols);

		pdblRealData		= stk(iRealData);
		pdblImgData			= stk(iImgData);
		pReturnRealData		= (double*)malloc(iMaxData * sizeof(double));

		for(iIndex = 0 ; iIndex < iMaxData ; iIndex++)
		{
			pReturnRealData[iIndex] = dabsz(pdblRealData[iIndex], pdblImgData[iIndex]);
		}
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

	}

	CreatePolyVarFromPtr(Rhs + 1, &piVarName, iRows, iCols, piPow, pReturnRealData);
	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	free(pReturnRealData);
	free(piPow);

	return 0;
}

/*Absolute value for a sparse ( absolute value of each element )*/
int abs_sparse()
{
	int iRows			= 0;
	int iCols			= 0;
	int iRealData		= 0;
	int iImgData		= 0;
	int iTotalElem		= 0;
	int *piElemByRow	= NULL;
	int *piColByRow		= NULL;

	int iIndex			= 0;
	int iIndex2			= 0;
	int iIndex3			= 0;

	double *pdblRealData	= 0;
	double *pdblImgData		= 0;
	double *pReturnRealData	= NULL;
	double *pReturnImgData	= NULL;

	if(iIsComplex(1))
	{
		GetRhsCSparseVar(1, &iRows, &iCols, &iTotalElem, NULL, NULL, &iRealData, &iImgData);
		piElemByRow	= (int*)malloc(iTotalElem * sizeof(int));
		piColByRow	= (int*)malloc(iTotalElem * sizeof(int));
		GetRhsCSparseVar(1, &iRows, &iCols, &iTotalElem, piElemByRow, piColByRow, &iRealData, &iImgData);

		pdblRealData	= stk(iRealData);
		pdblImgData		= stk(iImgData);
		pReturnRealData	= (double*)malloc(iTotalElem * sizeof(double));

		for(iIndex = 0 ; iIndex < iTotalElem ; iIndex++)
		{
			pReturnRealData[iIndex] = dabsz(pdblRealData[iIndex],pdblImgData[iIndex]);
		}
	}
	else
	{//void GetRhsSparseVar(int _iVarNum, int* _piRows, int* _piCols, int* _piRowsElem, int* _piColsElem, int* _piReal);
		GetRhsSparseVar(1, &iRows, &iCols, &iTotalElem, NULL, NULL, &iRealData);
		piElemByRow	= (int*)malloc(iRows * sizeof(int));
		piColByRow	= (int*)malloc(iTotalElem * sizeof(int));
		GetRhsSparseVar(1, &iRows, &iCols, &iTotalElem, piElemByRow, piColByRow, &iRealData);

		pdblRealData	= stk(iRealData);

		pReturnRealData	= (double*)malloc(iTotalElem * sizeof(double));

		/*This is the "elegante" version, parse two arrays and compute abs(Data)*/
		/*
		for(iIndex = 0 ; iIndex < iTotalElem ; iIndex++)
		{
			for(iIndex2 = 0 ; iIndex2 < piElemByRow[iIndex] ; iIndex2++)
			{
				pReturnRealData[iIndex3] = fabs(pdblRealData[iIndex3]);
				iIndex3++;
			}
		}
		*/

		/*This is the faster version, don't care about position in the matrix, juste compute de value*/
		for(iIndex = 0 ; iIndex < iTotalElem ; iIndex++)
			pReturnRealData[iIndex] = fabs(pdblRealData[iIndex]);
	}

	CreateSparseVarFromPtr(Rhs + 1, iRows, iCols, iTotalElem, piElemByRow, piColByRow, pReturnRealData);
	LhsVar(1) = Rhs + 1;
	PutLhsVar();

	free(piElemByRow);
	free(piColByRow);
	free(pReturnRealData);

	return 0;
}

/*--------------------------------------------------------------------------*/
