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

int iIsRealSparse(int _iVar, double _dblPrecision);
int iIsRealPoly(int _iVar, double _dblPrecision);
int iIsRealDouble(int _iVar, double _dblPrecision);
int iIsRealRaw(double *_pdblVal, int _iSize, double _dblPrecision);
/*--------------------------------------------------------------------------*/
extern int C2F(intisreal) _PARAMS((int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_isreal) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iRows				= 0;
	int iCols				= 0;
	int iRealData			= 0;
	int iRealEsp			= 0;
	double *pdblRealData	= NULL;
	int iType				= 0;
	int iVal				= 0;

	CheckRhs(1,2);
	CheckLhs(1,1);

	iType = GetType(1);
	if(iType != sci_matrix && iType != sci_poly && iType != sci_sparse)
	{
		OverLoad(1);
		return 0;
	}

	if(Rhs == 1)
	{
		if(iIsComplex(1))
			iVal = 0;
		else
			iVal = 1;
		CheckAllVarUsed(-1, -1);
	}
	else //Rhs == 2
	{
		double dblEsp = 0;
		if(GetType(2) != sci_matrix)
		{
			Error(53);
			return 0;
		}

		GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealEsp);
		dblEsp = stk(iRealEsp)[0];
	
		if(iIsComplex(1) == 0) //Not complex
		{
			iVal = 1;
		}
		else //Complex
		{
			iType = GetType(1);
			switch(iType)
			{
			case sci_matrix :
				iVal = iIsRealDouble(1, dblEsp);
				break;
			case sci_poly :
				iVal = iIsRealPoly(1, dblEsp);
				break;
			case sci_sparse :
				iVal = iIsRealSparse(1, dblEsp);
				break;
			default: //never pass here
				break;
			}
		}
	}
	CreateBooleanVarFromPtr(Rhs + 1, 1, 1, &iVal);
	LhsVar(1) = Rhs + 1;
	PutLhsVar();
#else
	C2F(intisreal)(id);
#endif
	return 0;
}

int iIsRealDouble(int _iVar, double _dblPrecision)
{
	int iRows		= 0;
	int iCols		= 0;
	int iRealData	= 0;
	int iImgData	= 0;
	int iComplex	= 1;

	double *pdblImgData		= 0;

	GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &iRealData, &iImgData);

	pdblImgData	= stk(iImgData);
	return iIsRealRaw(pdblImgData, iCols * iRows, _dblPrecision);
}

int iIsRealPoly(int _iVar, double _dblPrecision)
{
	int iRows		= 0;
	int iCols		= 0;
	int iRealData	= 0;
	int iImgData	= 0;
	int *piPow		= NULL;
	int *piVarName	= NULL;
	int iMaxData	= 0;

	double *pdblImgData		= 0;

	GetRhsCPolyVar(1, &piVarName, &iRows, &iCols, NULL, &iRealData, &iImgData);
	piPow	= (int*)malloc(iRows * iCols * sizeof(int));
	GetRhsCPolyVar(1, &piVarName, &iRows, &iCols, piPow, &iRealData, &iImgData);
	iMaxData = iArraySum(piPow, 0, iRows * iCols);

	pdblImgData	= stk(iImgData);
	return iIsRealRaw(pdblImgData, iMaxData, _dblPrecision);
}

int iIsRealSparse(int _iVar, double _dblPrecision)
{
	int iRows			= 0;
	int iCols			= 0;
	int iRealData		= 0;
	int iImgData		= 0;
	int iTotalElem		= 0;
	int *piElemByRow	= NULL;
	int *piColByRow		= NULL;

	double *pdblImgData		= 0;

	GetRhsCSparseVar(1, &iRows, &iCols, &iTotalElem, NULL, NULL, &iRealData, &iImgData);
	piElemByRow	= (int*)malloc(iRows * sizeof(int));
	piColByRow	= (int*)malloc(iTotalElem * sizeof(int));
	GetRhsCSparseVar(1, &iRows, &iCols, &iTotalElem, piElemByRow, piColByRow, &iRealData, &iImgData);

	pdblImgData	= stk(iImgData);
	return iIsRealRaw(pdblImgData, iTotalElem, _dblPrecision);
}

int iIsRealRaw(double *_pdblVal, int _iSize, double _dblPrecision)
{
	int iIndex = 0;
	double dblMaxVal = 0;
	for(iIndex = 0 ; iIndex < _iSize ; iIndex++)
		dblMaxVal = Max(dblMaxVal, dabss(_pdblVal[iIndex]));

	if(dblMaxVal > _dblPrecision)
		return 0;
	else
		return 1;
}
/*--------------------------------------------------------------------------*/
