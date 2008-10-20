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
extern int C2F(intnbprop) _PARAMS((int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_number_properties) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iIndex			= 0;
	int iRows			= 0;
	int iCols			= 0;
	char **szRealData	= 0;

	double *pReturnData = NULL;
	

	CheckRhs(1,1);
	CheckLhs(1,1);

	if(GetType(1) != sci_strings)
	{
		Err = 2;
		Error(55);
		return 0;
	}

	GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &iRows, &iCols, &szRealData);

	iAllocMatrixOfDouble(Rhs + 1, 1, 1, &pReturnData);
	//pReturnData = (double*)malloc(sizeof(double));

	if(strcmp(szRealData[0], "eps") == 0)
		pReturnData[0]	= F2C(dlamch)("e",1L);
	else if(strcmp(szRealData[0], "huge") == 0)
		pReturnData[0]	= F2C(dlamch)("oe",1L);
	else if(strcmp(szRealData[0], "tiny") == 0)
		pReturnData[0]	= F2C(dlamch)("u",1L);
	else if(strcmp(szRealData[0], "radix") == 0)
		pReturnData[0]	= F2C(dlamch)("b",1L);
	else if(strcmp(szRealData[0], "digits") == 0)
		pReturnData[0]	= F2C(dlamch)("n",1L);
	else if(strcmp(szRealData[0], "minexp") == 0)
		pReturnData[0]	= F2C(dlamch)("m",1L);
	else if(strcmp(szRealData[0], "maxexp") == 0)
		pReturnData[0]	= F2C(dlamch)("l",1L);
	else if(strcmp(szRealData[0], "denorm") == 0)
	{
		if(F2C(dlamch)("u",1L) / F2C(dlamch)("b",1L) > 0)
			pReturnData[0]	= 1;
		else
			pReturnData[0]	= 0;
	}
	else if(strcmp(szRealData[0], "tiniest") == 0)
	{
		double dblRadix	= F2C(dlamch)("b",1L);
		pReturnData[0]	= F2C(dlamch)("u",1L);

		if(pReturnData[0] / dblRadix != 0)
		{//denormalised number are used
			int iDigits = (int)F2C(dlamch)("n",1L);
			for(iIndex = 1 ; iIndex < iDigits ; iIndex++)
				pReturnData[0] /= dblRadix;
		}
	}
	else
	{
		free(pReturnData);
		sciprint(_("%s: unknown property kind.\n"), fname);
		Error(999);
		return 0;
	}

	//CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnData);
	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	//free(pReturnData);

#else
	C2F(intnbprop)(id);
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
