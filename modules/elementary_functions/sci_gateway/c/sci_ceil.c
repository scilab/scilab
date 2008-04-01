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
#include "basic_functions.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intceil) _PARAMS((int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_ceil) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
	int iRows = 0;
	int iCols = 0;
	int iRealData = 0;
	int iImgData = 0;
	int iIndex = 0;
	int *piPow = NULL;
	int *piVarName = NULL;
	int iMaxData = 0;
	
	double *pdblRealData = 0;
	double *pdblImgData = 0;
	double *pReturnRealData = NULL;
	double *pReturnImgData = NULL;
	int iComplex = 1;

	CheckRhs(1,1);
	CheckLhs(1,1);

	if(GetType(1) == sci_matrix)
	{
		if(iIsComplex(1))
		{

			GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &iRealData, &iImgData);
			pdblRealData	= stk(iRealData);
			pdblImgData		= stk(iImgData);

			pReturnRealData = (double*)malloc(iRows * iCols * sizeof(double));
			pReturnImgData	= (double*)malloc(iRows * iCols * sizeof(double));

			for(iIndex = 0 ; iIndex < iCols * iRows ; iIndex++)
			{
				pReturnRealData[iIndex] = dceils(pdblRealData[iIndex]);
				pReturnImgData[iIndex] = dceils(pdblImgData[iIndex]);
			}

			CreateCVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &pReturnRealData, &pReturnImgData);
			LhsVar(1) = Rhs + 1;
			PutLhsVar();
			free(pReturnRealData);
			free(pReturnImgData);
		}
		else
		{
			GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
			pdblRealData	= stk(iRealData);

			pReturnRealData = (double*)malloc(iRows * iCols * sizeof(double));

			for(iIndex = 0 ; iIndex < iCols * iRows ; iIndex++)
				pReturnRealData[iIndex] = dceils(pdblRealData[iIndex]);

			CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
			LhsVar(1) = Rhs + 1;
			PutLhsVar();
			free(pReturnRealData);
		}
	}
	else if(GetType(1) == sci_poly)
	{
		if(iIsComplex(1))
		{
			GetRhsCPolyVar(1, &piVarName, &iRows, &iCols, NULL, &iRealData, &iImgData);
			piPow	= (int*)malloc(iRows * iCols * sizeof(int));
			GetRhsCPolyVar(1, &piVarName, &iRows, &iCols, piPow, &iRealData, &iImgData);
			iMaxData = iArraySum(piPow, 0, iRows * iCols);

			pdblRealData	= stk(iRealData);
			pdblImgData		= stk(iImgData);

			pReturnRealData = (double*)malloc(iMaxData * sizeof(double));
			pReturnImgData	= (double*)malloc(iMaxData * sizeof(double));

			for(iIndex = 0 ; iIndex < iMaxData ; iIndex++)
			{
				pReturnRealData[iIndex] = dceils(pdblRealData[iIndex]);
				pReturnImgData[iIndex] = dceils(pdblImgData[iIndex]);
			}

			CreateCPolyVarFromPtr(Rhs + 1, &piVarName, iRows, iCols, piPow, pReturnRealData, pReturnImgData);
			LhsVar(1) = Rhs + 1;
			PutLhsVar();
			free(pReturnRealData);
			free(pReturnImgData);
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
				pReturnRealData[iIndex] = dceils(pdblRealData[iIndex]);

			CreatePolyVarFromPtr(Rhs + 1, &piVarName, iRows, iCols, piPow, pReturnRealData);
			LhsVar(1) = Rhs + 1;
			PutLhsVar();
			free(pReturnRealData);
			free(piPow);

		}
	}
	else if(GetType(1) == sci_ints)
	{//No change for integer
		LhsVar(1) = 1;
		PutLhsVar();
		return 0;
	}
	else
	{
		OverLoad(1);
		return 0;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
