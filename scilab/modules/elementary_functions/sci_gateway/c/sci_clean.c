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
#include "api_oldstack.h"

/*Some call to other module ( polynomial et sparse*/
extern int sci_cleanp(char *fname,unsigned long fname_len);
extern int sci_spclean (char *fname,unsigned long fname_len);
extern int C2F(ref2val) (void);
extern double C2F(dasum)();

SciErr clean_double(int* _piKey, int* _piAddress);
int clean_poly(int* _piKey, int* _piAddress);
int clean_sparse(int* _piKey, int* _piAddress);

int sci_clean(char *fname, int*_piKey)
{
	SciErr sciErr;

	int* piAddr1		= NULL;
	int iType				= 0;

	CheckRhs(1,3);
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

	switch(iType)
	{
	case sci_matrix :
		sciErr = clean_double(_piKey, piAddr1);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		PutLhsVar();
		break;
	case sci_poly :
		clean_poly(_piKey, piAddr1);
		break;
	case sci_sparse :
		clean_sparse(_piKey, piAddr1);
		break;
	default :
		OverLoad(1);
		return 0;
	}

	return 0;
}

int clean_poly(int* _piKey, int* _piAddress)
{
	sci_cleanp("clean", 5);
	return 0;
}

int clean_sparse(int* _piKey, int* _piAddress)
{
	C2F(ref2val)();
	sci_spclean("clean", 5);
	return 0;
}

SciErr clean_double(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int i;

	int iRows1						= 0;
	int iCols1						= 0;
	double *pdblReal1			= NULL;
	double *pdblImg1			= NULL;

	int iRows2						= 0;
	int iCols2						= 0;
	double *pdblReal2			= NULL;

	int iRows3						= 0;
	int iCols3						= 0;
	double *pdblReal3			= NULL;

	double	*pdblRealRet	= NULL;
	double	*pdblImgRet		= NULL;

	double dblEpsR				= 1E-10;
	double dblEpsA				= 1E-10;
	double dblEps					= 0;
	double dblNorm				= 0;

	int* piAddr2					= NULL;
	int* piAddr3					= NULL;

	if(Rhs == 3)
	{
		sciErr = getVarAddressFromPosition(_piKey, 3, &piAddr3);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = getMatrixOfDouble(_piKey, piAddr3, &iRows3, &iCols3, &pdblReal3);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		if(iRows3 != 1 || iCols3 != 1)
		{
			SciError(60);
			return sciErr;
		}
		dblEpsR = pdblReal3[0];
	}

	if(Rhs >= 2)
	{
		sciErr = getVarAddressFromPosition(_piKey, 2, &piAddr2);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = getMatrixOfDouble(_piKey, piAddr2, &iRows2, &iCols2, &pdblReal2);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		if(iRows2 != 1 || iCols2 != 1)
		{
			SciError(60);
			return sciErr;
		}
		dblEpsA = pdblReal2[0];
	}

	if(isVarComplex(_piKey, _piAddress))
	{
		sciErr = getComplexMatrixOfDouble(_piKey, _piAddress, &iRows1, &iCols1, &pdblReal1, &pdblImg1);
		if(sciErr.iErr)
		{
			return sciErr;
		}


		dblNorm = wasums(iRows1 * iCols1, pdblReal1, pdblImg1);

		sciErr = allocComplexMatrixOfDouble(_piKey, Rhs + 1, iRows1, iCols1, &pdblRealRet, &pdblImgRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}


		dblEps = Max(dblEpsA, dblEpsR * dblNorm);
		for(i = 0 ; i < iRows1 * iCols1 ; i++)
		{
			pdblRealRet[i]	= dabss(pdblReal1[i]) <= dblEps ? 0 : pdblReal1[i];
			pdblImgRet[i]		= dabss(pdblImg1[i]) <= dblEps ? 0 : pdblImg1[i];
		}
	}
	else
	{
		int iOne	= 1;
		int iSize1	= 0;

		sciErr = getMatrixOfDouble(_piKey, _piAddress, &iRows1, &iCols1, &pdblReal1);
		if(sciErr.iErr)
		{
			return sciErr;
		}


		iSize1			= iRows1 * iCols1;
		dblNorm			= C2F(dasum)(&iSize1, pdblReal1, &iOne);

		dblEps = Max(dblEpsA, dblEpsR * dblNorm);
		sciErr = allocMatrixOfDouble(_piKey, Rhs + 1, iRows1, iCols1, &pdblRealRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}


		for(i = 0 ; i < iRows1 * iCols1 ; i++)
		{
			pdblRealRet[i] = dabss(pdblReal1[i]) <= dblEps ? 0 : pdblReal1[i];
		}
	}

	LhsVar(1) = Rhs + 1;
	return sciErr;
}
/*--------------------------------------------------------------------------*/
