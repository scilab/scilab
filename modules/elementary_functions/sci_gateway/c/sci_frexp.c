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
extern int C2F(intfrexp) _PARAMS((int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_frexp) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iRows		= 0;
	int iCols		= 0;
	int iRealData	= 0;
	int iImgData	= 0;
	int iIndex		= 0;


	double *pdblRealData	= 0;
	double *pReturnCoefData = NULL;
	double *pReturnExpData	= NULL;
	
	CheckRhs(1,1);
	CheckLhs(2,2);

	if(GetType(1) != sci_matrix)
	{
		OverLoad(1);
		return 0;
	}

	if(iIsComplex(1))
	{
		Error(9999);
		sciprint(_("%s: real data only.\n"), fname);
	}

	GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
	pdblRealData		= stk(iRealData);
	pReturnCoefData		= (double*)malloc(iRows * iCols * sizeof(double));
	pReturnExpData		= (double*)malloc(iRows * iCols * sizeof(double));

	for(iIndex = 0 ; iIndex < iRows * iCols; iIndex++)
	{
//		int iTemp;
		pReturnCoefData[iIndex] = dfrexps(pdblRealData[iIndex], &pReturnExpData[iIndex]);
//		pReturnCoefData[iIndex] = dfrexps(pdblRealData[iIndex], &iTemp);
//		pReturnExpData[iIndex] = iTemp;
	}

	CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnCoefData);
	LhsVar(1) = Rhs + 1;

	CreateVarFromPtr(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnExpData);
	LhsVar(2) = Rhs + 2;

	PutLhsVar();
	free(pReturnCoefData);
	free(pReturnExpData);
#else
	C2F(intfrexp)(id);
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
