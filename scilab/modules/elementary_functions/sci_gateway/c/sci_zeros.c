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
extern int C2F(intzeros) (int *id);
/*--------------------------------------------------------------------------*/
int C2F(sci_zeros) (char *fname,unsigned long fname_len)
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iIndex				= 0;
	int iRows				= 0;
	int iCols				= 0;
	double *pReturnRealData	= 0;

	CheckLhs(1,1);

	if(Rhs > 2)
	{
		//trouver un moyen d'appeller %hm_zeros :(
	}
	else if(Rhs <= 0)
	{
		Rhs = 0;
		iRows = 1;
		iCols = 1;
	}
	else if(Rhs == 1)
	{
		if(GetType(1) > 10)
		{
			OverLoad(1);
			return 0;
		}
		GetVarDimension(1, &iRows, &iCols);
		CheckVarUsed(1);
	}
	else if(Rhs == 2)
	{
		GetDimFromVar(1, 2, &iRows);
		GetDimFromVar(2, 1, &iCols);
	}

	if(iCols == 0)
		iRows = 0;
	if(iRows == 0)
		iCols = 0;

	iAllocMatrixOfDouble(Rhs + 1, iRows, iCols, &pReturnRealData);
	//pReturnRealData = (double*)malloc(iRows * iCols * sizeof(double));
	if(iRows != 0)
		vDset(iRows * iCols, 0, pReturnRealData, 1);

	//CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	//free(pReturnRealData);
#else
	C2F(intzeros)(id);
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
