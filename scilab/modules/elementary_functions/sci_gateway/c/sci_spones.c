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
extern int C2F(intspones) (char *fname,int *id);
/*--------------------------------------------------------------------------*/
int C2F(sci_spones) (char *fname,unsigned long fname_len)
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iRows				= 0;
	int iCols				= 0;
	int iRealData			= 0;
	int iImgData			= 0;
	int iIndex				= 0;
	int iTotalElem			= 0;
	
	int *piElemByRow		= NULL;
	int *piColByRow			= NULL;

	double *pReturnRealData	= NULL;
	double *pReturnImgdata	= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	if(GetType(1) != sci_sparse && GetType(1) != sci_boolean_sparse)
	{
		if(GetType(1) == sci_matlab_sparse)
		{
			OverLoad(1);
			return 0;
		}
		else
		{
			sciprint(_("%s: argument must be a sparse matrix.\n"), fname);
			Error(999);
			return 0;
		}
	}

	if(iIsComplex(1))
	{
		GetRhsCSparseVar(1, &iRows, &iCols, &iTotalElem, NULL, NULL, &iRealData, &iImgData);

		//Warning, tips to use same variable in out and in !!!
		iAllocSparseMatrix(Rhs + 1, iRows, iCols, iTotalElem, &piElemByRow, &piColByRow, &pReturnRealData);
		GetRhsCSparseVar(1, &iRows, &iCols, &iTotalElem, piElemByRow, piColByRow, &iRealData, &iImgData);
		vDset(iTotalElem, 1, pReturnRealData, 1);
	}
	else
	{
		GetRhsSparseVar(1, &iRows, &iCols, &iTotalElem, NULL, NULL, &iRealData);

		//Warning, tips to use same variable in out and in !!!
		iAllocSparseMatrix(Rhs + 1, iRows, iCols, iTotalElem, &piElemByRow, &piColByRow, &pReturnRealData);

		GetRhsSparseVar(1, &iRows, &iCols, &iTotalElem, piElemByRow, piColByRow, &iRealData);
		vDset(iTotalElem, 1, pReturnRealData, 1);
	}
	LhsVar(1) = Rhs + 1;
	PutLhsVar();


#else
	C2F(intspones)(fname,id);
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
