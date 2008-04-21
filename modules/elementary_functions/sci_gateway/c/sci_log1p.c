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
#include "../../core/src/c/parse.h"

#define _NEW_TONIO_
/*--------------------------------------------------------------------------*/
extern int C2F(intlog1p) _PARAMS((int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_log1p) _PARAMS((char *fname,unsigned long fname_len))
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
		Error(43);
		return 0;
	}
	else
	{
		int iLessZero	= 0;
		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
		pdblRealData	= stk(iRealData);

		for(iIndex = 0; iIndex < iRows * iCols ; iIndex++)
		{
			if(pdblRealData[iIndex] <= -1)
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

		pReturnRealData = (double*)malloc(iRows * iCols * sizeof(double));

		for(iIndex = 0; iIndex < iRows * iCols ; iIndex++)
			pReturnRealData[iIndex] = dlog1ps(pdblRealData[iIndex]);

		CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		free(pReturnRealData);
	}
#else // _NEW_TONIO_
	C2F(intlog1p)(id);
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
