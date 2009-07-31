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


/*Some call to other module ( polynomial et sparse*/
extern int C2F(sci_cleanp) (char *fname,unsigned long fname_len);
extern int C2F(sci_spclean) (char *fname,unsigned long fname_len);
extern int C2F(ref2val) (void);
extern double C2F(dasum)();

int clean_double(int* _piAddress);
int clean_poly(int* _piAddress);
int clean_sparse(int* _piAddress);

int C2F(sci_clean) (char *fname,unsigned long fname_len)
{
	int iRet			= 0;

	int* piAddr1					= NULL;

	CheckRhs(1,3);
	CheckLhs(1,1);

	iRet = getVarAddressFromPosition(1, &piAddr1);
	if(iRet)
	{
		return 1;
	}

	switch(getVarType(piAddr1))
	{
	case sci_matrix : 
		iRet = clean_double(piAddr1);
		break;
	case sci_poly : 
		iRet = clean_poly(piAddr1);
		break;
	case sci_sparse :
		iRet = clean_sparse(piAddr1);
		break;
	default :
		OverLoad(1);
		return 0;
	}
	return 0;
}

int clean_poly(int* _piAddress)
{
	C2F(sci_cleanp)("clean", 5);
	return 0;
}

int clean_sparse(int* _piAddress)
{
	C2F(ref2val)();
	C2F(sci_spclean)("clean", 5);
	return 0;
}

int clean_double(int* _piAddress)
{
	int i;
	int iRet							= 0;

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
		iRet = getVarAddressFromPosition(3, &piAddr3);
		if(iRet)
		{
			return 1;
		}

		if(getVarType(piAddr3) != sci_matrix)
		{
			Error(52);
			return 0;
		}

		if(isVarComplex(piAddr3))
		{
			Error(52);
			return 0;
		}

		iRet = getMatrixOfDouble(piAddr3, &iRows3, &iCols3, &pdblReal3);
		if(iRet)
		{
			return 1;
		}
		if(iRows3 != 1 || iCols3 != 1)
		{
			Error(60);
			return 0;
		}
		dblEpsR = pdblReal3[0];
	}

	if(Rhs >= 2)
	{
		iRet = getVarAddressFromPosition(2, &piAddr2);
		if(iRet)
		{
			return 1;
		}

		if(getVarType(piAddr2) != sci_matrix)
		{
			Error(52);
			return 0;
		}

		if(isVarComplex(piAddr2))
		{
			Error(52);
			return 0;
		}

		iRet = getMatrixOfDouble(piAddr2, &iRows2, &iCols2, &pdblReal2);
		if(iRet)
		{
			return 1;
		}
		if(iRows2 != 1 || iCols2 != 1)
		{
			Error(60);
			return 0;
		}
		dblEpsA = pdblReal2[0];


	}

	if(isVarComplex(_piAddress))
	{
		iRet = getComplexMatrixOfDouble(_piAddress, &iRows1, &iCols1, &pdblReal1, &pdblImg1);
		if(iRet)
		{
			return 1;
		}

		dblNorm = wasums(iRows1 * iCols1, pdblReal1, pdblImg1);

		allocComplexMatrixOfDouble(Rhs + 1, iRows1, iCols1, &pdblRealRet, &pdblImgRet);

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

		iRet = getMatrixOfDouble(_piAddress, &iRows1, &iCols1, &pdblReal1);
		if(iRet)
		{
			return 1;
		}

		iSize1			= iRows1 * iCols1;
		dblNorm			= C2F(dasum)(&iSize1, pdblReal1, &iOne);

		dblEps = Max(dblEpsA, dblEpsR * dblNorm);
		iRet = allocMatrixOfDouble(Rhs + 1, iRows1, iCols1, &pdblRealRet);
		if(iRet)
		{
			return 1;
		}

		for(i = 0 ; i < iRows1 * iCols1 ; i++)
		{
			pdblRealRet[i] = dabss(pdblReal1[i]) <= dblEps ? 0 : pdblReal1[i];
		}
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
