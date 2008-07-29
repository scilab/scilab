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

extern int C2F(dscal)();

int C2F(sci_conj) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
	int iRows		= 0;
	int iCols		= 0;
	int iRealData	= 0;
	int iImgData	= 0;
	int iIndex		= 0;
	int iDataType	= 0;

	double *pdblRealData = 0;
	double *pdblImgData = 0;
	double *pReturnRealData = NULL;
	double *pReturnImgData = NULL;
	int iComplex = 1;

	CheckRhs(1,1);
	CheckLhs(1,1);


	iDataType = GetType(1);
	if(iDataType != sci_matrix && iDataType != sci_poly)
	{
		OverLoad(1);
		return 0;
	}

	if(iDataType == sci_matrix)
	{
		if(iIsComplex(1))
		{
			int iOne		= 1;
			int iSize		= 0;
			double dblCoef	= -1;
			GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &iRealData, &iImgData);
			pdblRealData	= stk(iRealData);
			pdblImgData		= stk(iImgData);
			iSize			= iRows * iCols;

			iAllocComplexMatrixOfDouble(Rhs + 1, 1, iRows, iCols, &pReturnRealData, &pReturnImgData);
			//pReturnRealData = (double*)malloc(iRows * iCols * sizeof(double));
			//pReturnImgData	= (double*)malloc(iRows * iCols * sizeof(double));

			memcpy(pReturnRealData, pdblRealData, iSize * sizeof(double));

			C2F(dscal)(&iSize, &dblCoef, pdblImgData, pReturnImgData);

			//CreateCVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &pReturnRealData, &pReturnImgData);
			LhsVar(1) = Rhs + 1;
			PutLhsVar();
			//free(pReturnRealData);
			//free(pReturnImgData);

		}
		else
		{//do nothing, just copy in a new variable
			double *pdblRealData = 0;
			double *pReturnRealData = NULL;

			GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
			pdblRealData		= stk(iRealData);

			iAllocMatrixOfDouble(Rhs + 1, iRows, iCols, &pReturnRealData);
			//pReturnRealData		= (double*)malloc(iRows * iCols * sizeof(double));
			memcpy(pReturnRealData, pdblRealData, iRows * iCols * sizeof(double));
			//CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
			LhsVar(1) = Rhs + 1;
			PutLhsVar();
			//free(pReturnRealData);
		}
	}
	else if(iDataType == sci_poly)
	{
		int *piPow		= NULL;
		int *piVarName	= NULL;
		int iMaxData	= 0;

		if(iIsComplex(1))
		{
			int iOne		= 1;
			int iSize		= 0;
			double dblCoef	= -1;
			GetRhsCPolyVar(1, &piVarName, &iRows, &iCols, NULL, &iRealData, &iImgData);
			piPow				= (int*)malloc(iRows * iCols * sizeof(int));
			GetRhsCPolyVar(1, &piVarName, &iRows, &iCols, piPow, &iRealData, &iImgData);
			iMaxData			= iArraySum(piPow, 0, iRows * iCols);
			iSize				= iRows * iCols;

			pdblRealData		= stk(iRealData);
			pdblImgData			= stk(iImgData);
			iAllocComplexMatrixOfPoly(Rhs + 1, 1, &piVarName, iRows, iCols, piPow, &pReturnRealData, &pReturnImgData);
			//pReturnRealData		= (double*)malloc(iMaxData * sizeof(double));
			//pReturnImgData		= (double*)malloc(iMaxData * sizeof(double));

			memcpy(pReturnRealData, pdblRealData, iMaxData * sizeof(double));
			C2F(dscal)(&iSize, &dblCoef, pdblImgData, pReturnImgData);
			//CreateCPolyVarFromPtr(Rhs + 1, &piVarName, iRows, iCols, piPow, pReturnRealData, pReturnImgData);
			LhsVar(1) = Rhs + 1;
			PutLhsVar();
			//free(pReturnRealData);
			//free(pReturnImgData);
			free(piPow);
		}
		else
		{
			GetRhsPolyVar(1, &piVarName, &iRows, &iCols, NULL, &iRealData);
			piPow				= (int*)malloc(iRows * iCols * sizeof(int));
			GetRhsPolyVar(1, &piVarName, &iRows, &iCols, piPow, &iRealData);
			iMaxData			= iArraySum(piPow, 0, iRows * iCols);

			pdblRealData		= stk(iRealData);
			iAllocMatrixOfPoly(Rhs + 1, &piVarName, iRows, iCols, piPow, &pReturnRealData);
			//pReturnRealData		= (double*)malloc(iMaxData * sizeof(double));

			memcpy(pReturnRealData, pdblRealData, iMaxData * sizeof(double));
			//CreatePolyVarFromPtr(Rhs + 1, &piVarName, iRows, iCols, piPow, pReturnRealData);
			LhsVar(1) = Rhs + 1;
			PutLhsVar();
			//free(pReturnRealData);
			free(piPow);
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
