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
extern int C2F(inttan) _PARAMS((int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_tan) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
	int iRows1 = 0;
	int iCols1 = 0;
	int iRows2 = 0;
	int iCols2 = 0;
	int iRealData1 = 0;
	int iRealData2 = 0;
	int iImgData1 = 0;
	int iIndex;
	

	CheckRhs(1,1);
	CheckLhs(1,1);

	if(GetType(1) != sci_matrix)
	{
		OverLoad(1);
		return 0;
	}

	if(iIsComplex(1))
	{// case complex
		double *pdblRealData	= 0;
		double *pdblImgData		= 0;
		double *pReturnRealData = NULL;
		double *pReturnImgData	= NULL;
		int iComplex			= 1;

		GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows1, &iCols1, &iRealData1, &iImgData1);
		pdblRealData	= stk(iRealData1);
		pdblImgData		= stk(iImgData1);
		pReturnRealData	= (double*)malloc(iRows1 * iCols1 * sizeof(double));
		pReturnImgData	= (double*)malloc(iRows1 * iCols1 * sizeof(double));

		for(iIndex = 0 ; iIndex < iRows1 * iCols1 ; iIndex++)
		{
			watan(pdblRealData[iIndex], pdblImgData[iIndex], &pReturnRealData[iIndex], &pReturnImgData[iIndex]);

			CreateCVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows1, &iCols1, &pReturnRealData, &pReturnImgData);
			LhsVar(1) = Rhs + 1;
			PutLhsVar();
			free(pReturnRealData);
			free(pReturnImgData);
		}

	}
	else
	{// case real
		double *pdblRealData	= 0;
		double *pReturnRealData = NULL;

		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows1, &iCols1, &iRealData1);
		pdblRealData		= stk(iRealData1);
		pReturnRealData		= (double*)malloc(iRows1 * iCols1 * sizeof(double));
		for(iIndex = 0 ; iIndex < iRows1 * iCols1 ; iIndex++)
			pReturnRealData[iIndex] = datans(pdblRealData[iIndex]);

		CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows1, &iCols1, &pReturnRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		free(pReturnRealData);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
