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
extern int C2F(inteye) _PARAMS((int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_eye) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iRows = 0;
	int iCols = 0;
	int iRealData = 0;
	
	double *pReturnRealData;

	CheckRhs(-1,2);
	CheckLhs(1,1);

	if(Rhs == 0 || Rhs == -1)//eye or eye()
	{
		Rhs = Max(Rhs,0);
		iRows = -1;
		iCols = -1;
		pReturnRealData = (double*)malloc(sizeof(double));
		pReturnRealData[0] = 1;
		CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		free(pReturnRealData);
		return 0;
	}
	else if(Rhs == 1)
	{
		int iType = GetType(1);

		if(iType != sci_matrix && iType != sci_ints && iType != sci_poly)
		{
			OverLoad(1);
			return 0;
		}

		switch(iType)
		{
		case sci_poly :
			{
				int *piVarName = NULL;
				int *piPow = NULL;
				GetRhsPolyVar(1, &piVarName, &iRows, &iCols, NULL, &iRealData);
				piPow = (int*)malloc(iRows * iCols * sizeof(int));
				GetRhsPolyVar(1, &piVarName, &iRows, &iCols, piPow, &iRealData);
				free(piPow);
				break;
			}
		default:
			GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
			break;
		}
	}
	else if(Rhs == 2)
	{
		GetDimFromVar(1, 1, &iRows);
		GetDimFromVar(2, 2, &iCols);
	}

	if(iRows == 0 || iCols == 0)
	{
		iRows = 0;
		iCols = 0;
	}

	//ca marche si iRows = 0 et iCols = 0 ???
	if(iRows * iCols != 0)
	{
		iRows = (int)dabss(iRows);
		iCols = (int)dabss(iCols);

		pReturnRealData = (double*)malloc(sizeof(double) * iRows * iCols);
		vDset(iRows * iCols, 0, pReturnRealData, 1);
		vDset(Min(iRows, iCols), 1, pReturnRealData, iRows + 1);

		CreateVarFromPtr(Rhs +1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
		LhsVar(1) = Rhs +1;
		PutLhsVar();
		free(pReturnRealData);
	}
#else
	C2F(inteye)(id);
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
