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
#include "Scierror.h"

/*--------------------------------------------------------------------------*/
int C2F(sci_prod) (char *fname,unsigned long fname_len)
{
	int iRet						= 0;

	int iRows						= 0;
	int iCols						= 0;
	int*piAddr					= NULL;
	double* pdblReal		= NULL;
	double* pdblImg			= NULL;

	int iRowsRet				= 0;
	int iColsRet				= 0;
	double* pdblRealRet	= NULL;
	double* pdblImgRet	= NULL;
	int iMode						= 0;

	CheckRhs(1,2);
	CheckLhs(1,1);

	iRet = getVarAddressFromPosition(1, &piAddr);
	if(iRet)
	{
		return 1;
	}

	if(Rhs == 2)
	{
		iRet = getProcessMode(2, piAddr, &iMode);
		if(iRet)
		{
			return 1;
		}
	}

	iRet = getVarDimension(piAddr, &iRows, &iCols);
	if(iRet)
	{
		return 1;
	}

	if(iRows * iCols == 0)
	{
		double dblVal	= 0;
		if(iMode == 0)
		{
			iRows = 1;
			iCols = 1;
			dblVal = 1;
		}
		else
		{
			iRows = 0;
			iCols = 0;
		}
		iRet = createMatrixOfDouble(Rhs + 1, 1, 1, &dblVal);
		if(iRet)
		{
			return 1;
		}

		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		return 0;
	}

	switch(iMode)
	{
	case BY_ROWS :
		iRowsRet = 1;
		iColsRet = iCols;
		break;
	case BY_COLS :
		iRowsRet = iRows;
		iColsRet = 1;
		break;
	default : //BY_ALL
		iRowsRet = 1;
		iColsRet = 1;
		break;
	}
	
	
	if(isVarComplex(piAddr))
	{
		iRet = getComplexMatrixOfDouble(piAddr, &iRows, &iCols, &pdblReal, &pdblImg);
		if(iRet)
		{
			return 1;
		}

		iRet = allocComplexMatrixOfDouble(Rhs + 1, iRowsRet, iColsRet, &pdblRealRet, &pdblImgRet);
		if(iRet)
		{
			return 1;
		}

		vWDmProd(iMode, pdblReal, pdblImg, iRows, iRows, iCols, pdblRealRet, pdblImgRet, 1);
	}
	else
	{
		iRet = getMatrixOfDouble(piAddr, &iRows, &iCols, &pdblReal);
		if(iRet)
		{
			return 1;
		}

		iRet = allocMatrixOfDouble(Rhs + 1, iRowsRet, iColsRet, &pdblRealRet);
		if(iRet)
		{
			return 1;
		}

		vDmProd(iMode, pdblReal, iRows, iRows, iCols, pdblRealRet, 1);
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
