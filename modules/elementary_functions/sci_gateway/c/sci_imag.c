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

#define _NEW_TONIO_

int img_double();
int img_poly();
int img_sparse();
/*--------------------------------------------------------------------------*/
extern int C2F(intimag) _PARAMS((int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_imag) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_
	CheckRhs(1,1);
	CheckLhs(1,1);
	switch(GetType(1))
	{
	case sci_matrix:
		{
			img_double();
			break;
		}
	case sci_poly:
		{
			img_poly();
			break;
		}
	case sci_sparse:
		{
			img_sparse();
			break;
		}
	default:
		OverLoad(1);
		break;
	}
#else
	C2F(intimag)(id);
#endif
	return 0;
}

int img_double()
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

		CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pdblImgData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
	}
	else
	{
		double *pdblRealData	= 0;
		double *pReturnRealData = NULL;
		int		itr				= 0;

		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
		pdblRealData		= stk(iRealData);
		pReturnRealData		= (double*)malloc(iRows * iCols * sizeof(double));

		memset(pReturnRealData, 0x00, iRows * iCols * sizeof(double));

		CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		free(pReturnRealData);
	}

	return 0;
}

int img_poly()
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

	if(iIsComplex(1))
	{
		GetRhsCPolyVar(1, &piVarName, &iRows, &iCols, NULL, &iRealData, &iImgData);
		piPow				= (int*)malloc(iRows * iCols * sizeof(int));
		GetRhsCPolyVar(1, &piVarName, &iRows, &iCols, piPow, &iRealData, &iImgData);
		iMaxData			= iArraySum(piPow, 0, iRows * iCols);

		pdblRealData		= stk(iRealData);
		pdblImgData			= stk(iImgData);

		CreatePolyVarFromPtr(Rhs + 1, &piVarName, iRows, iCols, piPow, pdblImgData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		free(piPow);
	}
	else
	{
		GetRhsPolyVar(1, &piVarName, &iRows, &iCols, NULL, &iRealData);
		piPow	= (int*)malloc(iRows * iCols * sizeof(int));
		GetRhsPolyVar(1, &piVarName, &iRows, &iCols, piPow, &iRealData);
		iMaxData = iArraySum(piPow, 0, iRows * iCols);

		pdblRealData	= stk(iRealData);
		pReturnRealData	= (double*)malloc(iMaxData * sizeof(double));
		memset(pReturnRealData, 0x00, iMaxData * sizeof(double));

		CreatePolyVarFromPtr(Rhs + 1, &piVarName, iRows, iCols, piPow, pReturnRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		free(pReturnRealData);
		free(piPow);
	}
	return 0;
}

int img_sparse()
{
	int iRows			= 0;
	int iCols			= 0;
	int iRealData		= 0;
	int iImgData		= 0;
	int iTotalElem		= 0;
	int *piElemByRow	= NULL;
	int *piColByRow		= NULL;

	double *pdblRealData	= 0;
	double *pdblImgData		= 0;
	double *pReturnRealData	= NULL;
	double *pReturnImgData	= NULL;

	int iIndex1 = 0;
	int iIndex2 = 0;
	int iIndex3 = 0;

	if(iIsComplex(1))
	{
		int iNewTotalItem = 0;
		int *piNewElemByRow = NULL;
		int *piNewColByRow = NULL;

		GetRhsCSparseVar(1, &iRows, &iCols, &iTotalElem, NULL, NULL, &iRealData, &iImgData);
		piElemByRow	= (int*)malloc(iRows * sizeof(int));
		piColByRow	= (int*)malloc(iTotalElem * sizeof(int));
		GetRhsCSparseVar(1, &iRows, &iCols, &iTotalElem, piElemByRow, piColByRow, &iRealData, &iImgData);

		pdblImgData		= stk(iImgData);

		piNewElemByRow		= (int*)malloc(iRows * sizeof(int));
		piNewColByRow		= (int*)malloc(iTotalElem * sizeof(int));
		pReturnRealData		= (double*)malloc(iTotalElem * sizeof(double));
		memset(piNewElemByRow, 0x00, iRows * sizeof(int));

		for(iIndex1 = 0 ; iIndex1 < iRows ; iIndex1++)
		{
			for(iIndex2 = 0 ; iIndex2 < piElemByRow[iIndex1] ; iIndex2++)
			{
				if(pdblImgData[iIndex3] != 0)
				{
					pReturnRealData[iNewTotalItem] = pdblImgData[iIndex3];
					(piNewElemByRow[iIndex1])++;
					piNewColByRow[iNewTotalItem] = piColByRow[iIndex3];
					iNewTotalItem++;
				}
				iIndex3++;
			}
		}

		CreateSparseVarFromPtr(Rhs + 1, iRows, iCols, iNewTotalItem, piNewElemByRow, piNewColByRow, pReturnRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		free(piElemByRow);
		free(piColByRow);
		free(piNewElemByRow);
		free(piNewColByRow);
		free(pReturnRealData);
	}
	else
	{
		GetRhsSparseVar(1, &iRows, &iCols, &iTotalElem, NULL, NULL, &iRealData);
		piElemByRow	= (int*)malloc(iRows * sizeof(int));
		piColByRow	= (int*)malloc(iTotalElem * sizeof(int));
		GetRhsSparseVar(1, &iRows, &iCols, &iTotalElem, piElemByRow, piColByRow, &iRealData);

		pdblRealData	= stk(iRealData);

		pReturnRealData	= (double*)malloc(iTotalElem * sizeof(double));

		iTotalElem = 0;
		memset(piElemByRow, 0x00, iRows);
		memset(pReturnRealData, 0x00, iTotalElem);

		CreateSparseVarFromPtr(Rhs + 1, iRows, iCols, iTotalElem, piElemByRow, piColByRow, pReturnRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();

		free(piElemByRow);
		free(piColByRow);
		free(pReturnRealData);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/