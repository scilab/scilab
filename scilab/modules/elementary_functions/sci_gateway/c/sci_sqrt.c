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
#include "api_oldstack.h"


int sci_sqrt(char *fname,int* _piKey)
{
	SciErr sciErr;
	int i;
	int iRet						= 0;
	int iComplex				= 0;
	int iRows						= 0;
	int iCols						= 0;
	int* piAddr					= NULL;

	double* pdblReal		= NULL;
	double* pdblImg			= NULL;
	double* pdblRealRet	= NULL;
	double* pdblImgRet	= NULL;


	CheckRhs(1,1);
	CheckLhs(1,1);

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(!isDoubleType(_piKey, piAddr))
	{
		OverLoad(1);
		return 0;
	}

	if(isVarComplex(_piKey, piAddr))
	{
		iComplex = 1;
		sciErr = getComplexMatrixOfDouble(_piKey, piAddr, &iRows, &iCols, &pdblReal, &pdblImg);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
	}
	else
	{
		iComplex = 0;
		sciErr = getMatrixOfDouble(_piKey, piAddr, &iRows, &iCols, &pdblReal);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
	}

	for(i = 0 ; i < iRows * iCols && iComplex == 0 ; i++)
	{
		if(pdblReal[i] < 0)
		{
			iComplex = 1;
		}
	}

	if(iComplex)
	{
		sciErr = allocComplexMatrixOfDouble(_piKey, Rhs + 1, iRows, iCols, &pdblRealRet, &pdblImgRet);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		if(isVarComplex(_piKey, piAddr))
		{
			for(i = 0 ; i < iRows * iCols ; i++)
			{
				zsqrts(pdblReal[i], pdblImg[i], &pdblRealRet[i], &pdblImgRet[i]);
			}
		}
		else
		{
			for(i = 0 ; i < iRows * iCols ; i++)
			{
				zsqrts(pdblReal[i], 0, &pdblRealRet[i], &pdblImgRet[i]);
			}
		}
	}
	else
	{
		sciErr = allocMatrixOfDouble(_piKey, Rhs + 1, iRows, iCols, &pdblRealRet);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblRealRet[i] = dsqrts(pdblReal[i]);
		}
	}
	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
