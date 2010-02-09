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

SciErr floor_poly(int* _piKey, int* _piAddress);
SciErr floor_double(int* _piKey, int* _piAddress);
SciErr floor_int(int* _piKey, int* _piAddress);

int sci_floor(char *fname,int* _piKey)
{
	SciErr sciErr;

	int iType			= 0;
	int* piAddr		= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarType(_piKey, piAddr, &iType);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	switch(iType)
	{
	case sci_matrix :
		sciErr = floor_double(_piKey, piAddr);
		break;
	case sci_poly :
		sciErr = floor_poly(_piKey, piAddr);
		break;
	case sci_ints :
		sciErr = floor_int(_piKey, piAddr);
		break;
	default:
		OverLoad(1);
		return 0;
	}

	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

SciErr floor_double(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRows							= 0;
	int iCols							= 0;

	double *pdblReal			= NULL;
	double *pdblImg				= NULL;
	double *pdblRealRet		= NULL;
	double *pdblImgRet		= NULL;

	if(isVarComplex(_piKey, _piAddress))
	{
		sciErr = getComplexMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = allocComplexMatrixOfDouble(_piKey, Rhs + 1, iRows, iCols, &pdblRealRet, &pdblImgRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblRealRet[i] = dfloors(pdblReal[i]);
			pdblImgRet[i]	 = dfloors(pdblImg[i]);
		}
	}
	else
	{
		sciErr = getMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = allocMatrixOfDouble(_piKey, Rhs + 1, iRows, iCols, &pdblRealRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblRealRet[i] = dfloors(pdblReal[i]);
		}
	}
	return sciErr;
}

SciErr floor_poly(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int i,j;

	int iRows							= 0;
	int iCols							= 0;
	int iLen							= 0;

	int *piCoeff					= NULL;

	double** pdblReal			= NULL;
	double** pdblImg			= NULL;
	double** pdblRealRet	= NULL;
	double** pdblImgRet		= NULL;

	char pstVarName[16];

	sciErr = getPolyVariableName(_piKey, _piAddress, pstVarName, &iLen);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	if(isVarComplex(_piKey, _piAddress))
	{
		sciErr = getComplexMatrixOfPoly(_piKey, _piAddress, &iRows, &iCols, NULL, NULL, NULL);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		piCoeff	= (int*)malloc(iRows * iCols * sizeof(int));
		sciErr = getComplexMatrixOfPoly(_piKey, _piAddress, &iRows, &iCols, piCoeff, NULL, NULL);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		pdblReal		= (double**)malloc(sizeof(double*) * iRows * iCols);
		pdblImg			= (double**)malloc(sizeof(double*) * iRows * iCols);
		pdblRealRet	= (double**)malloc(sizeof(double*) * iRows * iCols);
		pdblImgRet	= (double**)malloc(sizeof(double*) * iRows * iCols);

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblReal[i]			= (double*)malloc(sizeof(double) * piCoeff[i]);
			pdblImg[i]			= (double*)malloc(sizeof(double) * piCoeff[i]);
			pdblRealRet[i]	= (double*)malloc(sizeof(double) * piCoeff[i]);
			pdblImgRet[i]		= (double*)malloc(sizeof(double) * piCoeff[i]);
		}

		sciErr = getComplexMatrixOfPoly(_piKey, _piAddress, &iRows, &iCols, piCoeff, pdblReal, pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			for(j = 0 ; j < piCoeff[i] ; j++)
			{
				pdblRealRet[i][j] = dfloors(pdblReal[i][j]);
				pdblImgRet[i][j] = dfloors(pdblImg[i][j]);
			}
		}

		sciErr = createComplexMatrixOfPoly(_piKey, Rhs + 1, pstVarName, iRows, iCols, piCoeff, pdblRealRet, pdblImgRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}
	else
	{
		sciErr = getMatrixOfPoly(_piKey, _piAddress, &iRows, &iCols, NULL, NULL);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		piCoeff	= (int*)malloc(iRows * iCols * sizeof(int));
		sciErr = getMatrixOfPoly(_piKey, _piAddress, &iRows, &iCols, piCoeff, NULL);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		pdblReal		= (double**)malloc(sizeof(double*) * iRows * iCols);
		pdblRealRet	= (double**)malloc(sizeof(double*) * iRows * iCols);

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblReal[i]			= (double*)malloc(sizeof(double) * piCoeff[i]);
			pdblRealRet[i]	= (double*)malloc(sizeof(double) * piCoeff[i]);
		}

		sciErr = getMatrixOfPoly(_piKey, _piAddress, &iRows, &iCols, piCoeff, pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		for(i = 0 ; i < iRows * iCols ; i++)
		{
			for(j = 0 ; j < piCoeff[i] ; j++)
			{
				pdblRealRet[i][j] = dfloors(pdblReal[i][j]);
			}
		}

		sciErr = createMatrixOfPoly(_piKey, Rhs + 1, pstVarName, iRows, iCols, piCoeff, pdblRealRet);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}

	return sciErr;
}

SciErr floor_int(int* _piKey, int* _piAddress)
{
	SciErr sciErr;

	int iRows = 0;
	int iCols = 0;
	int iPrec = 0;

	sciErr = getMatrixOfIntegerPrecision(_piKey, _piAddress, &iPrec);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	switch(iPrec)
	{
	case SCI_INT8 :
		{
			char* pcData = NULL;
			sciErr = getMatrixOfInteger8(_piKey, _piAddress, &iRows, &iCols, &pcData);
			if(sciErr.iErr)
			{
				return sciErr;
			}

			sciErr = createMatrixOfInteger8(_piKey, Rhs + 1, iRows, iCols, pcData);
			if(sciErr.iErr)
			{
				return sciErr;
			}
			break;
		}
	case SCI_UINT8 :
		{
			unsigned char* pucData = NULL;
			sciErr = getMatrixOfUnsignedInteger8(_piKey, _piAddress, &iRows, &iCols, &pucData);
			if(sciErr.iErr)
			{
				return sciErr;
			}

			sciErr = createMatrixOfUnsignedInteger8(_piKey, Rhs + 1, iRows, iCols, pucData);
			if(sciErr.iErr)
			{
				return sciErr;
			}
			break;
		}
	case SCI_INT16 :
		{
			short* psData = NULL;
			sciErr = getMatrixOfInteger16(_piKey, _piAddress, &iRows, &iCols, &psData);
			if(sciErr.iErr)
			{
				return sciErr;
			}

			sciErr = createMatrixOfInteger16(_piKey, Rhs + 1, iRows, iCols, psData);
			if(sciErr.iErr)
			{
				return sciErr;
			}
			break;
		}
	case SCI_UINT16 :
		{
			unsigned short* pusData = NULL;
			sciErr = getMatrixOfUnsignedInteger16(_piKey, _piAddress, &iRows, &iCols, &pusData);
			if(sciErr.iErr)
			{
				return sciErr;
			}

			sciErr = createMatrixOfUnsignedInteger16(_piKey, Rhs + 1, iRows, iCols, pusData);
			if(sciErr.iErr)
			{
				return sciErr;
			}
			break;
		}
	case SCI_INT32 :
		{
			int* piData = NULL;
			sciErr = getMatrixOfInteger32(_piKey, _piAddress, &iRows, &iCols, &piData);
			if(sciErr.iErr)
			{
				return sciErr;
			}

			sciErr = createMatrixOfInteger32(_piKey, Rhs + 1, iRows, iCols, piData);
			if(sciErr.iErr)
			{
				return sciErr;
			}
			break;
		}
	case SCI_UINT32 :
		{
			unsigned int* puiData = NULL;
			sciErr = getMatrixOfUnsignedInteger32(_piKey, _piAddress, &iRows, &iCols, &puiData);
			if(sciErr.iErr)
			{
				return sciErr;
			}

			sciErr = createMatrixOfUnsignedInteger32(_piKey, Rhs + 1, iRows, iCols, puiData);
			if(sciErr.iErr)
			{
				return sciErr;
			}
			break;
		}
	}

	return sciErr;
}
/*--------------------------------------------------------------------------*/
