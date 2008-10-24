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
extern int C2F(intsign) _PARAMS((int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_sign) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iRows				= 0;
	int iCols				= 0;
	int iRealData			= 0;
	int iImgData			= 0;
	int iIndex				= 0;
	
	double *pRealData		= NULL;
	double *pImgData		= NULL;
	double *pReturnRealData	= NULL;
	double *pReturnImgData	= NULL;


	CheckRhs(1,1);
	CheckLhs(1,1);

	if(GetType(1) != sci_matrix)
	{
		OverLoad(1);
		return 0;
	}

	if(iIsComplex(1))
	{
		int iComplex	= 1;
		GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &iRealData, &iImgData);
		pRealData		= stk(iRealData);
		pImgData		= stk(iImgData);

		iAllocComplexMatrixOfDouble(Rhs + 1, 1, iRows, iCols, &pReturnRealData, &pReturnImgData);
		for(iIndex = 0 ; iIndex < iRows * iCols ; iIndex++)
		{
			double dblTemp = dpythags(pRealData[iIndex], pImgData[iIndex]);
			if(dblTemp == 0)
			{
				pReturnRealData[iIndex] = 0;
				pReturnImgData[iIndex] = 0;
			}
			else
			{
				pReturnRealData[iIndex] = pRealData[iIndex] / dblTemp;
				pReturnImgData[iIndex] = pImgData[iIndex] / dblTemp;;
			}
		}
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
	}
	else
	{
		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
		pRealData		= stk(iRealData);

		iAllocMatrixOfDouble(Rhs + 1, iRows, iCols, &pReturnRealData);
		for(iIndex = 0 ; iIndex < iRows * iCols ; iIndex++)
			pReturnRealData[iIndex] = dsignsEx(pRealData[iIndex]);

		LhsVar(1) = Rhs + 1;
		PutLhsVar();
	}
#else
	C2F(intsign)(id);
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
