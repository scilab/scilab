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

#define MAGI_LETTER		'm'
#define FRK_LETTER		'f'
#define HILB_LETTER		'h'

/*--------------------------------------------------------------------------*/
extern int C2F(inttestmatrix) (int *id);
extern int C2F(magic)();
/*--------------------------------------------------------------------------*/
int C2F(sci_testmatrix) (char *fname,unsigned long fname_len)
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iRows				= 0;
	int iCols				= 0;
	int iRealData			= 0;
	char cMode				= 0;
	int iVal				= 0;

	double *pReturnRealData = NULL;
	char **szRealData		= NULL;
	

	CheckRhs(2,2);
	CheckLhs(1,1);

	if(GetType(1) != sci_strings)
	{
		Error(55);
		return 0;
	}

	GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &iRows, &iCols, &szRealData);
	cMode = (int)*szRealData[0];

	if(iIsComplex(2))
	{
		//voir comment on ajoute une string dans les messages d'erreur
		//"%dth argument must be a real scalar
		Error(51);
		return 0;
	}

	GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
	if(iRows != 1 || iCols != 1)
	{
		//voir comment on ajoute une string dans les messages d'erreur
		//"%dth argument must be a real scalar
		Error(51);
		return 0;
	}
	iVal = (int)dabss(stk(iRealData)[0]);

	iAllocMatrixOfDouble(Rhs + 1, iVal, iVal, &pReturnRealData);
	switch(cMode)
	{
	case FRK_LETTER :
		franck_matrix(iVal, pReturnRealData);
		break;
	case HILB_LETTER :
		hilb_matrix(iVal, pReturnRealData);
		break;
	default : //Magic case and others
		magic_matrix(iVal, pReturnRealData);
		break;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
#else
	C2F(inttestmatrix)(id);
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
