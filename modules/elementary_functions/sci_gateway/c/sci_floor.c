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
/*--------------------------------------------------------------------------*/
int floor_poly();
int floor_double();

extern int C2F(intfloor) _PARAMS((int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_floor) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iType = 0;

	CheckRhs(1,1);
	CheckLhs(1,1);

	iType = GetType(1);

	switch(iType)
	{
	case sci_matrix :
		floor_double();
		break;
	case sci_poly :
		floor_poly();
		break;
	default:
		OverLoad(1);
		return 0;
	}

#else //_NEW_TONIO_
	C2F(intfloor)(id);
#endif
	return 0;
}

int floor_double()
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
		double *pReturnImgData	= NULL;
		int		iComplex		= 1;

		GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &iRealData, &iImgData);

		pdblRealData	= stk(iRealData);
		pdblImgData		= stk(iImgData);

		pReturnRealData = (double*)malloc(iRows * iCols * sizeof(double));
		pReturnImgData	= (double*)malloc(iRows * iCols * sizeof(double));

		for(iIndex = 0 ; iIndex < iRows * iCols ; iIndex++)
		{
			pReturnRealData[iIndex] = dfloors(pdblRealData[iIndex]);
			pReturnImgData[iIndex] = dfloors(pdblImgData[iIndex]);
		}

		CreateCVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &pReturnRealData, pReturnImgData);
		LhsVar(1) = Rhs + 1;
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
			pReturnRealData[iIndex] = dfloors(pdblRealData[iIndex]);

		CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		free(pReturnRealData);
	}

	return 0;
}

int floor_poly()
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
		pReturnImgData		= (double*)malloc(iMaxData * sizeof(double));

		for(iIndex = 0 ; iIndex < iMaxData ; iIndex++)
		{
			pReturnRealData[iIndex] = dfloors(pdblRealData[iIndex]);
			pReturnImgData[iIndex] = dfloors(pdblImgData[iIndex]);
		}

		CreateCPolyVarFromPtr(Rhs + 1, &piVarName, iRows, iCols, piPow, pReturnRealData, pReturnImgData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		free(pReturnRealData);
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

		for(iIndex = 0 ; iIndex < iMaxData ; iIndex++)
			pReturnRealData[iIndex] = dfloors(pdblRealData[iIndex]);

		CreatePolyVarFromPtr(Rhs + 1, &piVarName, iRows, iCols, piPow, pReturnRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		free(pReturnRealData);
		free(piPow);
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
