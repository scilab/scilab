/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA  - Allan CORNET
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
#include "../../core/src/c/parse.h"

#define _NEW_TONIO_

/*--------------------------------------------------------------------------*/
extern int C2F(intlog) _PARAMS((int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_log) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_

	int iRows			= 0;
	int iCols			= 0;
	int iRealData		= 0;
	int iImgData		= 0;
	int iIndex			= 0;

	double *pdblRealData	= NULL;
	double *pdblImgData		= NULL;
	double* pReturnRealData	= NULL;
	double* pReturnImgData	= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	if(GetType(1) != sci_matrix)
	{
		OverLoad(1);
		return 0;
	}

	if(iIsComplex(1))
	{
		int iComplex = 1;
		GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &iRealData, &iImgData);
		pdblRealData	= stk(iRealData);
		pdblImgData		= stk(iImgData);

		pReturnRealData = (double*)malloc(iRows * iCols * sizeof(double));
		pReturnImgData	= (double*)malloc(iRows * iCols * sizeof(double));


		for(iIndex = 0 ; iIndex < iRows * iCols ; iIndex++)
		{
			if(pdblRealData[iIndex] == 0 && pdblImgData[iIndex] == 0)
			{
				if(C2F(errgst).ieee == 0)
				{
					Error(32);
					return 0;
				}
				else if(C2F(errgst).ieee == 1)
				{
					Msgs(64,0);
				}
			}

			wlog(pdblRealData[iIndex], pdblImgData[iIndex], &pReturnRealData[iIndex], &pReturnImgData[iIndex]);
		}
		CreateCVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &pReturnRealData, &pReturnImgData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		free(pReturnRealData);
		free(pReturnImgData);
	}
	else
	{
		int iLessZero	= 0;
		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
		pdblRealData	= stk(iRealData);

		for(iIndex = 0; iIndex < iRows * iCols ; iIndex++)
		{
			if(pdblRealData[iIndex] < 0)
			{
				iLessZero = 1;
			}
			else if(pdblRealData[iIndex] == 0)
			{
				if(C2F(errgst).ieee == 0)
				{
					Error(32);
					return 0;
				}
				else if(C2F(errgst).ieee == 1)
				{
					Msgs(64,0);
				}
			}
		}

		if(iLessZero == 0)
		{//All values > 0
			pReturnRealData = (double*)malloc(iRows * iCols * sizeof(double));

			for(iIndex = 0; iIndex < iRows * iCols ; iIndex++)
				pReturnRealData[iIndex] = dlogs(pdblRealData[iIndex]);

			CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
			LhsVar(1) = Rhs + 1;
			PutLhsVar();
			free(pReturnRealData);
		}
		else
		{
			int iComplex = 1;
			pReturnRealData = (double*)malloc(iRows * iCols * sizeof(double));
			pReturnImgData	= (double*)malloc(iRows * iCols * sizeof(double));

			for(iIndex = 0; iIndex < iRows * iCols ; iIndex++)
				wlog(pdblRealData[iIndex], 0, &pReturnRealData[iIndex], &pReturnImgData[iIndex]);

			CreateCVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &pReturnRealData, &pReturnImgData);
			LhsVar(1) = Rhs + 1;
			PutLhsVar();
			free(pReturnRealData);
			free(pReturnImgData);
		}
	}

#else //_NEW_TONIO_
	C2F(intlog)(id);
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
