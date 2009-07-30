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
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_eye) (char *fname,unsigned long fname_len)
{
	int iRet							= 0;
	int iRows							= 0;
	int iCols							= 0;
	
	int* piAddr1					= NULL;
	int* piAddr2					= NULL;

	double *pdblRealRet		= NULL;

	Rhs = Max(Rhs,0);

	CheckRhs(0,2);
	CheckLhs(1,1);


	if(Rhs == 0)
	{
		iRows = -1;
		iCols = -1;
		iRet = allocMatrixOfDouble(Rhs + 1, -1, -1, &pdblRealRet);
		if(iRet)
		{
			return 1;
		}
		pdblRealRet[0] = 1;
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		return 0;
	}
	else if(Rhs == 1)
	{
		iRet = getVarAddressFromPosition(1, &piAddr1);
		if(iRet)
		{
			return 1;
		}

		switch(getVarType(piAddr1))
		{
		case sci_matrix : 
		case sci_poly : 
		case sci_boolean : 
		case sci_ints : 
		case sci_handles : 
		case sci_strings : 
			getVarDimension(piAddr1, &iRows, &iCols);
			break;
		default :
			OverLoad(1);
			return 0;
		}
	}
	else if(Rhs == 2)
	{
		getDimFromVar(piAddr1, &iRows);
		getDimFromVar(piAddr2, &iCols);
	}

	if(iRows == 0 || iCols == 0)
	{
		iRows = 0;
		iCols = 0;
	}
	else
	{
		iRows = (int)dabss(iRows);
		iCols = (int)dabss(iCols);
	}

	iRet = allocMatrixOfDouble(Rhs + 1, iRows, iCols, &pdblRealRet);


	if(iRows * iCols != 0)
	{
		deyes(pdblRealRet, iRows, iCols);
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
