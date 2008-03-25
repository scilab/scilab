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

/*--------------------------------------------------------------------------*/
extern int C2F(intsin) _PARAMS((int *id));

extern int iIsComplex(int _iVar);
extern double dcoss(double _dblVal);
extern double dsins(double _dblVal);
extern double dcoshs(double _dblVal);
extern double dsinhs(double _dblVal);
/*--------------------------------------------------------------------------*/
int C2F(sci_sin) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iRows = 0;
	int iCols = 0;
	int iRealData = 0;
	int iImgData = 0;
	int iIndex;
	
	CheckRhs(1,1);
	CheckLhs(1,1);
	if(GetType(1) != sci_matrix)
	{
		OverLoad(1);
		return 0;
	}

	if(iIsComplex(1))
	{
		double *pdblRealData = 0;
		double *pdblImgData = 0;
		double *pReturnRealData = NULL;
		double *pReturnImgData = NULL;
		int iComplex = 1;

		GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &iRealData, &iImgData);
		pdblRealData	= stk(iRealData);
		pdblImgData		= stk(iImgData);

		pReturnRealData = (double*)malloc(iRows * iCols * sizeof(double));
		pReturnImgData	= (double*)malloc(iRows * iCols * sizeof(double));

		for(iIndex = 0 ; iIndex < iCols * iRows ; iIndex++)
		{
			pReturnRealData[iIndex] = dsins(pdblRealData[iIndex]) * dcoshs(pdblImgData[iIndex]);
			pReturnImgData[iIndex]	= dcoss(pdblRealData[iIndex]) * dsinhs(pdblImgData[iIndex]);
		}

		CreateCVarFromPtr(2, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &pReturnRealData, &pReturnImgData);
		LhsVar(1) = 2;
		PutLhsVar();
		free(pReturnRealData);
		free(pReturnImgData);
	}
	else
	{
		double *pdblRealData = 0;
		double *pReturnRealData = NULL;

		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
		pdblRealData		= stk(iRealData);
		pReturnRealData		= (double*)malloc(iRows * iCols * sizeof(double));

		for(iIndex = 0 ; iIndex < iCols * iRows ; iIndex++)
		{
			pReturnRealData[iIndex] = dsins(pdblRealData[iIndex]);
		}
		
		CreateVarFromPtr(2, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
		LhsVar(1) = 2;
		PutLhsVar();
		free(pReturnRealData);
	}
#else
	C2F(intsin)(id);
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
