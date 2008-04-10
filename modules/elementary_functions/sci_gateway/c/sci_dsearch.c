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
#include "sciprint.h"
#include "localization.h"

#define _NEW_TONIO_

/*--------------------------------------------------------------------------*/
extern int C2F(intdsearch) _PARAMS((int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_dsearch) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_

	int iRows				= 0;
	int iCols				= 0;
	int iRealData			= 0;
	int iImgData			= 0;
	int iIndex				= 0;
	char cMode				= 0;
	int iRowOcc				= 0;
	int iColOcc				= 0;
	int iRowVal				= 0;
	int iColVal				= 0;
	double *pdblRealData	= 0;
	double *pdblRealVal		= 0;


	double *pdblRealOccData		= NULL;
	double *pdblRealIndData		= NULL;
	double *pdblRealInfoData	= NULL;

	CheckRhs(2,3);
	CheckLhs(1,3);

	if(GetType(1) != sci_matrix || GetType(2) != sci_matrix)
	{
		OverLoad(1);
		return 0;
	}

	//get ch
	if(Rhs == 3)
	{
		GetRhsVar(3, STRING_DATATYPE, &iRows, &iCols, &iRealData);
		cMode = *cstk(iRealData);
	}
	else
		cMode = 'c';

	if(cMode != 'c' && cMode != 'd')
	{
		sciprint(_("%s: unknown char specifier (must be ''c'' or ''d'').\n"), fname);
		Error(999);
		return 0;
	}

	if(iIsComplex(2) || iIsComplex(1))
	{
		Error(202);
		return 0;
	}
	else
	{
		GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &iRowVal, &iColVal, &iRealData);
		pdblRealVal = stk(iRealData);


		//verif that val is in strict increasing order
		for(iIndex = 1 ; iIndex < iRowVal * iColVal ; iIndex++)
		{
			if(pdblRealVal[iIndex] < pdblRealVal[iIndex - 1])
			{
				sciprint(_("%s: the array val (arg 2) is not well ordered.\n"), fname);
				Error(999);
				return 0;
			}
		}
	}

	iRowOcc = iRowVal;
	iColOcc = iColVal;
	if(cMode == 'd')
	{
		if( iRowOcc * iColOcc < 1)
		{
			sciprint(_("%s: Argument 2: argument 2 must not be an empty vector.\n"), fname);
			Error(999);
			return 0;
		}
	}
	else
	{// cMode == 'c'
		if( iRowOcc * iColOcc < 2)
		{
			sciprint(_("%s: in the interval case, argument 2 must be a vector with length > 1.\n"), fname);
			Error(999);
			return 0;
		}
		if(iRowOcc == 1)
			iColOcc--;
		else
			iRowOcc--;
	}

	//Get X
	GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
	pdblRealData		= stk(iRealData);

	pdblRealIndData		= (double*)malloc(iRows * iCols * sizeof(double));
	pdblRealOccData		= (double*)malloc(iRowOcc * iColOcc * sizeof(double));
	pdblRealInfoData	= (double*)malloc(sizeof(double));

	if(iRows == 0 || iCols == 0)
	{
		pdblRealInfoData[0] = 0;
		memset(pdblRealOccData, 0x00, iRowOcc * iColOcc * sizeof(double));
	}
	else
	{
		//go on for the computation
		if(cMode == 'c')
		{
			vDsearchC(pdblRealData, iRows*iCols, pdblRealVal, iRowVal * iColVal - 1, pdblRealIndData, pdblRealOccData, pdblRealInfoData);
		}
		else
		{
			vDsearchD(pdblRealData, iRows*iCols, pdblRealVal, iRowVal * iColVal, pdblRealIndData, pdblRealOccData, pdblRealInfoData);
		}
	}

	if(Lhs >= 1)
	{
		CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pdblRealIndData);
		LhsVar(1) = Rhs + 1;
	}
	if(Lhs >= 2)
	{
		CreateVarFromPtr(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &iRowOcc, &iColOcc, &pdblRealOccData);
		LhsVar(2) = Rhs + 2;
	}
	if(Lhs >= 3)
	{
		int iRow = 1;
		int iCol = 1;
		CreateVarFromPtr(Rhs + 3, MATRIX_OF_DOUBLE_DATATYPE, &iRow, &iCol, &pdblRealInfoData);
		LhsVar(3) = Rhs + 3;
	}

	free(pdblRealIndData);
	free(pdblRealOccData);
	free(pdblRealInfoData);
	PutLhsVar();
#else
	C2F(intdsearch)(id);
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
