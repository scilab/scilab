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
int tril_matrix(int _iOffset);

extern int C2F(sci_ptril) (char *fname,unsigned long fname_len);
extern int C2F(inttril)	(int *id);
/*--------------------------------------------------------------------------*/
int C2F(sci_tril) (char *fname,unsigned long fname_len)
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iRows				= 0;
	int iCols				= 0;
	int iRealData			= 0;
	int iOffset				= 0;

	CheckRhs(1,2);
	CheckLhs(1,1);

	if(Rhs == 2)
	{//Get offset
		if(GetType(2) != sci_matrix)
		{
			Error(53);
			return 0;
		}

		if(iIsComplex(2))
		{
			Error(52);
			return 0;
		}

		GetVarDimension(2, &iRows, &iCols);
		if(iRows * iCols != 1)
		{
			Error(89);
			return 0;
		}

		GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
		iOffset = (int)stk(iRealData)[0];
	}

	switch(GetType(1))
	{
	case sci_matrix :
		tril_matrix(iOffset);
		break;
	case sci_poly :
		C2F(sci_ptril)(fname, fname_len);
	default : 
		OverLoad(1);
		break;
	}
#else
	C2F(inttril)(id);
#endif
	return 0;
}

int tril_matrix(int _iOffset)
{
	int iIndex				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int iRealData			= 0;
	int iImgData			= 0;

	double *pReal			= NULL;
	double *pImg			= NULL;

	double *pReturnReal		= NULL;
	double *pReturnImg		= NULL;

	if(iIsComplex(1))
	{
		int iComplex	= 1;
		GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &iRealData, &iImgData);
		pReal			= stk(iRealData);
		pImg			= stk(iImgData);

		iAllocComplexMatrixOfDouble(Rhs + 1, iRows, iCols, &pReturnReal, &pReturnImg);
		memcpy(pReturnReal	, pReal	, sizeof(double) * iRows * iCols);
		memcpy(pReturnImg	, pImg	, sizeof(double) * iRows * iCols);

		for(iIndex = 0 ; iIndex < iCols ; iIndex++)
		{
			int iSize	= Min(Max(iIndex - _iOffset, 0), iRows);
			memset(&pReturnReal[iIndex * iRows], 0x00, sizeof(double) * iSize);
			memset(&pReturnImg[iIndex * iRows], 0x00, sizeof(double) * iSize);
		}
	}
	else
	{
		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
		pReal			= stk(iRealData);

		iAllocMatrixOfDouble(Rhs + 1, iRows, iCols, &pReturnReal);
		memcpy(pReturnReal	, pReal	, sizeof(double) * iRows * iCols);

		for(iIndex = 0 ; iIndex < iCols ; iIndex++)
		{
			int iSize	= Min(Max(iIndex - _iOffset, 0), iRows);
			memset(&pReturnReal[iIndex * iRows], 0x00, sizeof(double) * iSize);
		}
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}


/*--------------------------------------------------------------------------*/
