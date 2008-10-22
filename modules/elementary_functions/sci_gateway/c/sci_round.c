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
int round_double(void);
int round_poly(void);

extern int C2F(intround) _PARAMS((int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_round) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iType			= 0;
	int iRet			= 0;

	CheckRhs(1,1);
	CheckLhs(1,1);

	iType = GetType(1);

	switch(iType)
	{
	case sci_matrix :
		iRet = round_double();
		break;
	case sci_poly :
		iRet = round_poly();
		break;
	default:
		iRet = OverLoad(1);
		break;
	}

	if(iRet != 0)
		Error(iRet);


#else
	C2F(intround)(id);
#endif
	return 0;
}

int round_double(void)
{
	int iRows		= 0;
	int iCols		= 0;
	int iRealData	= 0;
	int iImgData	= 0;
	int iIndex		= 0;


	double *pRealData		= NULL;
	double *pImgData		= NULL;
	double *pReturnRealData	= NULL;
	double *pReturnImgData	= NULL;

	if(iIsComplex(1))
	{
		int iComplex = 1;
		GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &iRealData, &iImgData);
		pRealData	= stk(iRealData);
		pImgData	= stk(iImgData);

		iAllocComplexMatrixOfDouble(Rhs + 1, 1, iRows, iCols, &pReturnRealData, &pReturnImgData);

		for(iIndex = 0 ; iIndex < iRows * iCols; iIndex++)
		{
			pReturnRealData[iIndex] = danints(pRealData[iIndex]);
			pReturnImgData[iIndex]	= danints(pImgData[iIndex]);
		}

		LhsVar(1) = Rhs + 1;
		PutLhsVar();
	}
	else
	{
		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
		pRealData	= stk(iRealData);

		iAllocMatrixOfDouble(Rhs + 1, iRows, iCols, &pReturnRealData);

		for(iIndex = 0 ; iIndex < iRows * iCols; iIndex++)
			pReturnRealData[iIndex] = danints(pRealData[iIndex]);

		LhsVar(1) = Rhs + 1;
		PutLhsVar();
	}

	return 0;
}

int round_poly(void)
{
	int iRows		= 0;
	int iCols		= 0;
	int iRealData	= 0;
	int iImgData	= 0;
	int iIndex		= 0;
	int iMaxData	= 0;
	int *piPow = NULL;
	int *piVarName = NULL;

	double *pRealData		= NULL;
	double *pImgData		= NULL;
	double *pReturnRealData	= NULL;
	double *pReturnImgData	= NULL;

	if(iIsComplex(1))
	{
		int iComplex = 1;
		GetRhsCPolyVar(1, &piVarName, &iRows, &iCols, NULL, &iRealData, &iImgData);
		piPow	= (int*)malloc(iRows * iCols * sizeof(int));
		GetRhsCPolyVar(1, &piVarName, &iRows, &iCols, piPow, &iRealData, &iImgData);
		iMaxData = iArraySum(piPow, 0, iRows * iCols);

		pRealData	= stk(iRealData);
		pImgData	= stk(iImgData);

		iAllocComplexMatrixOfPoly(Rhs + 1, 1, &piVarName, iRows, iCols, piPow, &pReturnRealData, &pReturnImgData);
		for(iIndex = 0 ; iIndex < iMaxData ; iIndex++)
		{
			pReturnRealData[iIndex] = danints(pRealData[iIndex]);
			pReturnImgData[iIndex]	= danints(pImgData[iIndex]);
		}

		LhsVar(1) = Rhs + 1;
		PutLhsVar();
	}
	else
	{
		GetRhsPolyVar(1, &piVarName, &iRows, &iCols, NULL, &iRealData);
		piPow	= (int*)malloc(iRows * iCols * sizeof(int));
		GetRhsPolyVar(1, &piVarName, &iRows, &iCols, piPow, &iRealData);
		iMaxData = iArraySum(piPow, 0, iRows * iCols);

		pRealData	= stk(iRealData);
		pImgData	= stk(iImgData);

		iAllocMatrixOfPoly(Rhs + 1, &piVarName, iRows, iCols, piPow, &pReturnRealData);
		for(iIndex = 0 ; iIndex < iMaxData ; iIndex++)
			pReturnRealData[iIndex] = danints(pRealData[iIndex]);

		LhsVar(1) = Rhs + 1;
		PutLhsVar();
	}
	return 0;
}

/*--------------------------------------------------------------------------*/
