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
extern int C2F(intprod) _PARAMS((int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_prod) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iRows		= 0;
	int iCols		= 0;
	int iRowsRet	= 0;
	int iColsRet	= 0;
	int iRealData	= 0;
	int iImgData	= 0;
	int iIndex		= 0;
	int iMode		= 0;

	CheckRhs(1,2);
	CheckLhs(1,1);

	if(Rhs == 2)
	{
		iMode = iGetOrient(2);
	}

	GetVarDimension(1, &iRows, &iCols);

	if(iRows * iCols == 0)
	{
		double *pReturnRealData	= NULL;
		iAllocMatrixOfDouble(Rhs + 1, 1, 1, &pReturnRealData);
		//pReturnRealData = (double*)malloc(sizeof(double));

		if(iMode == 0)
		{
			iRows = 1;
			iCols = 1;
			pReturnRealData[0] = 1;
		}
		else
		{
			iRows = 0;
			iCols = 0;
		}
		//CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		//free(pReturnRealData);
		return 0;
	}

	switch(iMode)
	{
	case BY_ROWS :
		iRowsRet = 1;
		iColsRet = iCols;
		break;
	case BY_COLS :
		iRowsRet = iRows;
		iColsRet = 1;
		break;
	default : //BY_ALL
		iRowsRet = 1;
		iColsRet = 1;
		break;
	}
	
	
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

		iAllocComplexMatrixOfDouble(Rhs + 1, 1, iRowsRet, iColsRet, &pReturnRealData, &pReturnImgData);
		//pReturnRealData	= (double*)malloc(iRowsRet * iColsRet * sizeof(double));
		//pReturnImgData	= (double*)malloc(iRowsRet * iColsRet * sizeof(double));

		vWDmProd(iMode, pdblRealData, pdblImgData, iRows, iRows, iCols, pReturnRealData, pReturnImgData, 1);

		//CreateCVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRowsRet, &iColsRet, &pReturnRealData, &pReturnImgData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		//free(pReturnRealData);
	}
	else
	{
		double *pdblRealData	= 0;
		double *pReturnRealData = NULL;
		int		itr				= 0;

		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
		pdblRealData		= stk(iRealData);

		iAllocMatrixOfDouble(Rhs + 1, iRowsRet, iColsRet, &pReturnRealData);
		//pReturnRealData		= (double*)malloc(iRowsRet * iColsRet * sizeof(double));

		vDmProd(iMode, pdblRealData, iRows, iRows, iCols, pReturnRealData, 1);

		//CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRowsRet, &iColsRet, &pReturnRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		//free(pReturnRealData);
	}
#else
	C2F(intprod)(id);
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
