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
extern int C2F(intsqrt) (int *id);
/*--------------------------------------------------------------------------*/
int C2F(sci_sqrt) (char *fname,unsigned long fname_len)
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iRows			= 0;
	int iCols			= 0;
	int iRealData		= 0;
	int iImgData		= 0;
	int iComplex		= 0;
	int iIndex			= 0;
	int iComplexCase	= 0;

	double *pRealData		= NULL;
	double *pImgData		= NULL;
	double *pReturnRealData	= NULL;
	double *pReturnImgdata	= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	if(GetType(1) != sci_matrix)
	{
		OverLoad(1);
		return 0;
	}

	iComplex = iIsComplex(Rhs);
	if(iComplex)
	{
		GetRhsCVar(Rhs, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &iRealData, &iImgData);
		pRealData	= stk(iRealData);
		pImgData	= stk(iImgData);
		iComplexCase = 1;
	}
	else
	{
		GetRhsVar(Rhs, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
		pRealData	= stk(iRealData);
		iComplexCase = 0;
	}

	for(iIndex = 0 ; iIndex < iRows * iCols && iComplexCase == 0 ; iIndex++)
	{
		if(pRealData[iIndex] < 0)
			iComplexCase = 1;
	}

	if(iComplexCase == 0)
	{
		iAllocMatrixOfDouble(Rhs + 1, iRows, iCols, &pReturnRealData);
		//pReturnRealData = (double*)malloc(sizeof(double) * iRows * iCols);

		for(iIndex = 0 ; iIndex < iRows * iCols ; iIndex++)
			pReturnRealData[iIndex] = dsqrts(pRealData[iIndex]);

		//CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		//free(pReturnRealData);
	}
	else
	{
		iAllocComplexMatrixOfDouble(Rhs + 1, iRows, iCols, &pReturnRealData, &pReturnImgdata);
		//pReturnRealData = (double*)malloc(sizeof(double) * iRows * iCols);
		//pReturnImgdata	= (double*)malloc(sizeof(double) * iRows * iCols);

		if(iComplex == 0)
			for(iIndex = 0 ; iIndex < iRows * iCols ; iIndex++)
				zsqrts(pRealData[iIndex], 0, &pReturnRealData[iIndex], &pReturnImgdata[iIndex]);
		else
			for(iIndex = 0 ; iIndex < iRows * iCols ; iIndex++)
				zsqrts(pRealData[iIndex], pImgData[iIndex], &pReturnRealData[iIndex], &pReturnImgdata[iIndex]);

		//CreateCVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iComplexCase, &iRows, &iCols, &pReturnRealData, &pReturnImgdata);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		//free(pReturnRealData);
		//free(pReturnImgdata);
	}
#else
	C2F(intsqrt)(id);
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
