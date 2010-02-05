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
int sci_cumsum(char *fname, int*_piKey)
{
	SciErr sciErr;
	int i;
	int iRows							= 0;
	int iCols							= 0;
	int iType							= 0;
	int* piAddr1					= NULL;

	double *pdblReal		 = NULL;
	double *pdblImg			 = NULL;
	double *pdblRealRet	 = NULL;
	double *pdblImgRet	 = NULL;

	int iMode						= 0;

	CheckRhs(1,2);
	CheckLhs(1,1);

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarType(_piKey, piAddr1, &iType);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(iType != sci_matrix)
	{
		OverLoad(1);
		return 0;
	}

	if(Rhs == 2)
	{
		sciErr = getProcessMode(_piKey, 2, piAddr1, &iMode);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
	}

	if(isVarComplex(_piKey, piAddr1))
	{
		sciErr = getComplexMatrixOfDouble(_piKey, piAddr1, &iRows, &iCols, &pdblReal, &pdblImg);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = allocComplexMatrixOfDouble(_piKey, Rhs + 1, iRows, iCols, &pdblRealRet, &pdblImgRet);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		/*Set the first column of returned matrix at the same value of the input matrix*/
		memcpy(pdblRealRet, pdblReal, iRows * sizeof(double));
		memcpy(pdblImgRet, pdblImg, iRows * sizeof(double));

		if(iMode == BY_ROWS)
		{
			for(i = 0 ; i < iCols ; i++)
			{
				vCusum(iRows, pdblReal + iRows * i, pdblRealRet + iRows * i);
				vCusum(iRows, pdblImg + iRows * i, pdblImgRet + iRows * i);
			}
		}
		else if(iMode == BY_COLS)
		{
			int iOffset = 0;

			for(i = 0 ; i < iCols - 1; i++)
			{
				vDadd(iRows, pdblRealRet + iOffset, pdblReal + iOffset + iRows, 1, 1, pdblRealRet + iOffset + iRows);
				vDadd(iRows, pdblImgRet + iOffset, pdblImg + iOffset + iRows, 1, 1, pdblImgRet + iOffset + iRows);
				iOffset += iRows;
			}
		}
		else if(iMode == BY_ALL)
		{
			vCusum(iRows * iCols, pdblReal, pdblRealRet);
			vCusum(iRows * iCols, pdblImg, pdblImgRet);
		}
		else
		{
			Error(44);
			return 2;
		}
	}
	else
	{
		sciErr = getMatrixOfDouble(_piKey, piAddr1, &iRows, &iCols, &pdblReal);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = allocMatrixOfDouble(_piKey, Rhs + 1, iRows, iCols, &pdblRealRet);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		memcpy(pdblRealRet, pdblReal, iRows * sizeof(double));

		if(iMode == BY_ROWS)
		{
			for(i = 0 ; i < iCols ; i++)
			{
				vCusum(iRows, pdblReal + iRows * i, pdblRealRet + iRows * i);
			}
		}
		else if(iMode == BY_COLS)
		{
			int iOffset = 0;

			for(i = 0 ; i < iCols - 1; i++)
			{
				vDadd(iRows, pdblRealRet + iOffset, pdblReal + iOffset + iRows, 1, 1, pdblRealRet + iOffset + iRows);
				iOffset += iRows;
			}
		}
		else if(iMode == BY_ALL)
		{
			vCusum(iRows * iCols, pdblReal, pdblRealRet);
		}
		else
		{
			Error(44);
			return 2;
		}
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
