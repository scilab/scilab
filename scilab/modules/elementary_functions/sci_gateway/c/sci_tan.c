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
extern int C2F(inttan) (int *id);
/*--------------------------------------------------------------------------*/
int C2F(sci_tan) (char *fname,unsigned long fname_len)
{
	static int id[6];
	int iRows = 0;
	int iCols = 0;
	int iRealData = 0;
	int iImgData = 0;
	int iIndex;

	double *pdblRealData	= 0;
	double *pdblImgData		= 0;
	double *pReturnRealData = NULL;
	double *pReturnImgData	= NULL;
	

	CheckRhs(1,1);
	CheckLhs(1,1);

	if(GetType(1) != sci_matrix)
	{
		OverLoad(1);
		return 0;
	}

	if(iIsComplex(1))
	{// complex case
		int iComplex			= 1;

		GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &iRealData, &iImgData);
		pdblRealData	= stk(iRealData);
		pdblImgData		= stk(iImgData);

		iAllocComplexMatrixOfDouble(Rhs + 1, iRows, iCols, &pReturnRealData, &pReturnImgData);
		//pReturnRealData	= (double*)malloc(iRows1 * iCols1 * sizeof(double));
		//pReturnImgData	= (double*)malloc(iRows1 * iCols1 * sizeof(double));

		for(iIndex = 0 ; iIndex < iRows * iCols ; iIndex++)
			ztans(pdblRealData[iIndex], pdblImgData[iIndex], &pReturnRealData[iIndex], &pReturnImgData[iIndex]);

		//CreateCVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows1, &iCols1, &pReturnRealData, &pReturnImgData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		//free(pReturnRealData);
		//free(pReturnImgData);
	}
	else
	{// real case

		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
		pdblRealData		= stk(iRealData);

		iAllocMatrixOfDouble(Rhs + 1, iRows, iCols, &pReturnRealData);
		//pReturnRealData		= (double*)malloc(iRows1 * iCols1 * sizeof(double));
		for(iIndex = 0 ; iIndex < iRows * iCols ; iIndex++)
			pReturnRealData[iIndex] = dtans(pdblRealData[iIndex]);

		//CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows1, &iCols1, &pReturnRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		//free(pReturnRealData);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
