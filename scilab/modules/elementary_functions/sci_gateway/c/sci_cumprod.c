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

int sci_cumprod(char *fname,unsigned long fname_len)
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

	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarType(pvApiCtx, piAddr1, &iType);
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
		sciErr = getProcessMode(pvApiCtx, 2, piAddr1, &iMode);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
	}

	if(isVarComplex(pvApiCtx, piAddr1))
	{
		sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddr1, &iRows, &iCols, &pdblReal, &pdblImg);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = allocComplexMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealRet, &pdblImgRet);
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
				vCuproi(iRows, pdblReal + iRows * i, pdblImg + iRows * i, pdblRealRet + iRows * i, pdblImgRet + iRows * i);
			}
		}
		else if(iMode == BY_COLS)
		{
			int iOffset = 0;

			for(i = 0 ; i < iCols - 1; i++)
			{
				vWvmul(
						iRows, pdblRealRet + iOffset,
						pdblImgRet + iOffset,
						pdblReal + iOffset + iRows,
						pdblImg + iOffset + iRows,
						1, 1,
						pdblRealRet + iOffset + iRows,
						pdblImgRet + iOffset + iRows);
				iOffset += iRows;
			}
		}
		else if(iMode == BY_ALL)
		{
			vCuproi(iRows * iCols, pdblReal, pdblImg, pdblRealRet, pdblImgRet);
		}
		else
		{
			SciError(44);
			return 2;
		}
	}
	else
	{
		sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &iRows, &iCols, &pdblReal);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealRet);
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
				vCupro(iRows, pdblReal + iRows * i, pdblRealRet + iRows * i);
			}
		}
		else if(iMode == BY_COLS)
		{
			int iOffset = 0;

			for(i = 0 ; i < iCols - 1; i++)
			{
				vDvmul(iRows, pdblRealRet + iOffset, pdblReal + iOffset + iRows, 1, 1, pdblRealRet + iOffset + iRows);
				iOffset += iRows;
			}
		}
		else if(iMode == BY_ALL)
		{
			vCupro(iRows * iCols, pdblReal, pdblRealRet);
		}
		else
		{
			SciError(44);
			return 2;
		}
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
