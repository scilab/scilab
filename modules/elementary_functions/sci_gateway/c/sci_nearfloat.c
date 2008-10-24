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
extern int C2F(intnearfl) _PARAMS((int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_nearfloat) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iRows1			= 0;
	int iCols1			= 0;
	int iRows2			= 0;
	int iCols2			= 0;
	int iRealData		= 0;
	int iImgData		= 0;
	int iIndex			= 0;
	double dblMode		= 0;
	char **szRealData	= 0;

	double *pReturnData = NULL;
	double *pdblData	= NULL;
	

	CheckRhs(2,2);
	CheckLhs(1,1);

	if(GetType(1) != sci_strings)
	{
		Err = 2;
		Error(55);
		return 0;
	}

	if(GetType(2) != sci_matrix)
	{
		Err = 1;
		Error(53);
		return 0;
	}

	GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &iRows1, &iCols1, &szRealData);
	GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &iRows2, &iCols2, &iRealData);
	pdblData = stk(iRealData);

	if(strcmp(*szRealData, "succ") == 0)
	{
		dblMode = 1.0;
	}
	else if(strcmp(*szRealData, "pred") == 0)
	{
		dblMode = -1.0;
	}
	else
	{
		Error(999);
		return 0;
	}

	iAllocMatrixOfDouble(Rhs + 1, iRows2, iCols2, &pReturnData);
	//pReturnData = (double*)malloc(iRows2 * iCols2 * sizeof(double));
	for(iIndex = 0 ; iIndex < iRows2 * iCols2 ; iIndex++)
		pReturnData[iIndex] = dblNearFloat(pdblData[iIndex], dblMode);
		//pReturnData[iIndex] = C2F(nearfloat)(&pdblData[iIndex], &dblMode);

	//CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows2, &iCols2, &pReturnData);
	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	//free(pReturnData);
#else // _NEW_TONIO_
	C2F(intnearfl)(id);
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
