/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */
/*--------------------------------------------------------------------------*/ 
#include "gw_integer.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "api_oldstack.h"

int get_int(int* _piAddress, int* _piRows, int* _piCols, int* _piPrecision, void** pvData);

int int8_from_double(int* _piAddress, int* _piKey);
int uint8_from_double(int* _piAddress, int* _piKey);
int int16_from_double(int* _piAddress, int* _piKey);
int uint16_from_double(int* _piAddress, int* _piKey);
int int32_from_double(int* _piAddress, int* _piKey);
int uint32_from_double(int* _piAddress, int* _piKey);
int int64_from_double(int* _piAddress, int* _piKey);
int uint64_from_double(int* _piAddress, int* _piKey);

int int8_from_int(int* _piAddress, int* _piKey);
int uint8_from_int(int* _piAddress, int* _piKey);
int int16_from_int(int* _piAddress, int* _piKey);
int uint16_from_int(int* _piAddress, int* _piKey);
int int32_from_int(int* _piAddress, int* _piKey);
int uint32_from_int(int* _piAddress, int* _piKey);
int int64_from_int(int* _piAddress, int* _piKey);
int uint64_from_int(int* _piAddress, int* _piKey);

/*--------------------------------------------------------------------------*/
int sci_int8(char *fname, int* _piKey)
{
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int* piAddr			= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	iRet = getVarAddressFromPosition(1, &piAddr, _piKey);
	if(iRet)
	{
		return 1;
	}

	iRet = getVarDimension(piAddr, &iRows, &iCols);
	if(iRet)
	{
		return 1;
	}

	if(iRows == 0 && iCols == 0)
	{
		double dbl = 0;
		iRet = createMatrixOfDouble(Rhs + 1, 0, 0, &dbl, _piKey);
		if(iRet)
		{
			return 1;
		}
		return 0;
	}

	switch(getVarType(piAddr))
	{
	case sci_matrix :
		iRet = int8_from_double(piAddr, _piKey);
		break;
	case sci_ints : 
		iRet = int8_from_int(piAddr, _piKey);
		break;
	default :
		OverLoad(1);
		return 0;
	}

	if(iRet)
	{
		return 1;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

int sci_uint8(char *fname, int* _piKey)
{
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int* piAddr			= NULL;

	iRet = getVarAddressFromPosition(1, &piAddr, _piKey);
	if(iRet)
	{
		return 1;
	}

	iRet = getVarDimension(piAddr, &iRows, &iCols);
	if(iRet)
	{
		return 1;
	}

	if(iRows == 0 && iCols == 0)
	{
		double dbl = 0;
		iRet = createMatrixOfDouble(Rhs + 1, 0, 0, &dbl, _piKey);
		if(iRet)
		{
			return 1;
		}
		return 0;
	}

	switch(getVarType(piAddr))
	{
	case sci_matrix :
		iRet = uint8_from_double(piAddr, _piKey);
		break;
	case sci_ints : 
		iRet = uint8_from_int(piAddr, _piKey);
		break;
	default :
		OverLoad(1);
		return 0;
	}

	if(iRet)
	{
		return 1;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

int sci_int16(char *fname, int* _piKey)
{
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int* piAddr			= NULL;

	iRet = getVarAddressFromPosition(1, &piAddr, _piKey);
	if(iRet)
	{
		return 1;
	}

	iRet = getVarDimension(piAddr, &iRows, &iCols);
	if(iRet)
	{
		return 1;
	}

	if(iRows == 0 && iCols == 0)
	{
		double dbl = 0;
		iRet = createMatrixOfDouble(Rhs + 1, 0, 0, &dbl, _piKey);
		if(iRet)
		{
			return 1;
		}
		return 0;
	}

	switch(getVarType(piAddr))
	{
	case sci_matrix :
		iRet = int16_from_double(piAddr, _piKey);
		break;
	case sci_ints : 
		iRet = int16_from_int(piAddr, _piKey);
		break;
	default :
		OverLoad(1);
		return 0;
	}

	if(iRet)
	{
		return 1;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

int sci_uint16(char *fname, int* _piKey)
{
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int* piAddr			= NULL;

	iRet = getVarAddressFromPosition(1, &piAddr, _piKey);
	if(iRet)
	{
		return 1;
	}

	iRet = getVarDimension(piAddr, &iRows, &iCols);
	if(iRet)
	{
		return 1;
	}

	if(iRows == 0 && iCols == 0)
	{
		double dbl = 0;
		iRet = createMatrixOfDouble(Rhs + 1, 0, 0, &dbl, _piKey);
		if(iRet)
		{
			return 1;
		}
		return 0;
	}

	switch(getVarType(piAddr))
	{
	case sci_matrix :
		iRet = uint16_from_double(piAddr, _piKey);
		break;
	case sci_ints : 
		iRet = uint16_from_int(piAddr, _piKey);
		break;
	default :
		OverLoad(1);
		return 0;
	}

	if(iRet)
	{
		return 1;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

int sci_int32(char *fname, int* _piKey)
{
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int* piAddr			= NULL;

	iRet = getVarAddressFromPosition(1, &piAddr, _piKey);
	if(iRet)
	{
		return 1;
	}

	iRet = getVarDimension(piAddr, &iRows, &iCols);
	if(iRet)
	{
		return 1;
	}

	if(iRows == 0 && iCols == 0)
	{
		double dbl = 0;
		iRet = createMatrixOfDouble(Rhs + 1, 0, 0, &dbl, _piKey);
		if(iRet)
		{
			return 1;
		}
		return 0;
	}

	switch(getVarType(piAddr))
	{
	case sci_matrix :
		iRet = int32_from_double(piAddr, _piKey);
		break;
	case sci_ints : 
		iRet = int32_from_int(piAddr, _piKey);
		break;
	default :
		OverLoad(1);
		return 0;
	}

	if(iRet)
	{
		return 1;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

int sci_uint32(char *fname, int* _piKey)
{
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int* piAddr			= NULL;

	iRet = getVarAddressFromPosition(1, &piAddr, _piKey);
	if(iRet)
	{
		return 1;
	}

	iRet = getVarDimension(piAddr, &iRows, &iCols);
	if(iRet)
	{
		return 1;
	}

	if(iRows == 0 && iCols == 0)
	{
		double dbl = 0;
		iRet = createMatrixOfDouble(Rhs + 1, 0, 0, &dbl, _piKey);
		if(iRet)
		{
			return 1;
		}
		return 0;
	}

	switch(getVarType(piAddr))
	{
	case sci_matrix :
		iRet = uint32_from_double(piAddr, _piKey);
		break;
	case sci_ints : 
		iRet = uint32_from_int(piAddr, _piKey);
		break;
	default :
		OverLoad(1);
		return 0;
	}

	if(iRet)
	{
		return 1;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

int sci_int64(char *fname, int* _piKey)
{
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int* piAddr			= NULL;

	iRet = getVarAddressFromPosition(1, &piAddr, _piKey);
	if(iRet)
	{
		return 1;
	}

	iRet = getVarDimension(piAddr, &iRows, &iCols);
	if(iRet)
	{
		return 1;
	}

	if(iRows == 0 && iCols == 0)
	{
		double dbl = 0;
		iRet = createMatrixOfDouble(Rhs + 1, 0, 0, &dbl, _piKey);
		if(iRet)
		{
			return 1;
		}
		return 0;
	}

	switch(getVarType(piAddr))
	{
	case sci_matrix :
		iRet = int64_from_double(piAddr, _piKey);
		break;
	case sci_ints : 
		iRet = int64_from_int(piAddr, _piKey);
		break;
	default :
		OverLoad(1);
		return 0;
	}

	if(iRet)
	{
		return 1;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

int sci_uint64(char *fname, int* _piKey)
{
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int* piAddr			= NULL;

	iRet = getVarAddressFromPosition(1, &piAddr, _piKey);
	if(iRet)
	{
		return 1;
	}

	iRet = getVarDimension(piAddr, &iRows, &iCols);
	if(iRet)
	{
		return 1;
	}

	if(iRows == 0 && iCols == 0)
	{
		double dbl = 0;
		iRet = createMatrixOfDouble(Rhs + 1, 0, 0, &dbl, _piKey);
		if(iRet)
		{
			return 1;
		}
		return 0;
	}

	switch(getVarType(piAddr))
	{
	case sci_matrix :
		iRet = uint64_from_double(piAddr, _piKey);
		break;
	case sci_ints : 
		iRet = uint64_from_int(piAddr, _piKey);
		break;
	default :
		OverLoad(1);
		return 0;
	}

	if(iRet)
	{
		return 1;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

int get_int(int* _piAddress, int* _piRows, int* _piCols, int* _piPrecision, void** pvData)
{
	int iRet				= 0;
	void* pvIn			= NULL;

	char* pcOut			= NULL;

	iRet = getMatrixOfIntegerPrecision(_piAddress, _piPrecision);
	if(iRet)
	{
		return 1;
	}

	iRet = getVarDimension(_piAddress, _piRows, _piCols);
	if(iRet)
	{
		return 1;
	}

	switch(*_piPrecision)
	{
	case SCI_INT8 : 
		{
			iRet = getMatrixOfInteger8(_piAddress, _piRows, _piCols, (char**)pvData);
		}
	case SCI_UINT8 : 
		{
			iRet = getMatrixOfUnsignedInteger8(_piAddress, _piRows, _piCols, (unsigned char**)pvData);
		}
	case SCI_INT16 : 
		{
			iRet = getMatrixOfInteger16(_piAddress, _piRows, _piCols, (short**)pvData);
		}
	case SCI_UINT16 : 
		{
			iRet = getMatrixOfUnsignedInteger16(_piAddress, _piRows, _piCols, (unsigned short**)pvData);
		}
	case SCI_INT32 : 
		{
			iRet = getMatrixOfInteger32(_piAddress, _piRows, _piCols, (int**)pvData);
		}
	case SCI_UINT32 : 
		{
			iRet = getMatrixOfUnsignedInteger32(_piAddress, _piRows, _piCols, (unsigned int**)pvData);
		}
	case SCI_INT64 : 
		{
			iRet = getMatrixOfInteger64(_piAddress, _piRows, _piCols, (long long**)pvData);
		}
	case SCI_UINT64 : 
		{
			iRet = getMatrixOfUnsignedInteger64(_piAddress, _piRows, _piCols, (unsigned long long**)pvData);
		}
	}

	if(iRet)
	{
		return 1;
	}
	return 0;
}


int int8_from_double(int* _piAddress, int* _piKey)
{
	int i;
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	double* pdblIn	= NULL;

	char* pcOut			= NULL;

	if(isVarComplex(_piAddress))
	{
		return 1;
	}

	iRet = getMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblIn);
	if(iRet)
	{
		return 1;
	}

	iRet = allocMatrixOfInteger8(Rhs + 1, iRows, iCols, &pcOut, _piKey);
	if(iRet)
	{
		return 1;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pcOut[i] = (char)pdblIn[i];
	}

	return 0;
}

int uint8_from_double(int* _piAddress, int* _piKey)
{
	int i;
	int iRet							= 0;
	int iRows							= 0;
	int iCols							= 0;
	double* pdblIn				= NULL;

	unsigned char* pucOut	= NULL;

	if(isVarComplex(_piAddress))
	{
		return 1;
	}

	iRet = getMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblIn);
	if(iRet)
	{
		return 1;
	}

	iRet = allocMatrixOfUnsignedInteger8(Rhs + 1, iRows, iCols, &pucOut, _piKey);
	if(iRet)
	{
		return 1;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pucOut[i] = (unsigned char)pdblIn[i];
	}

	return 0;
}

int int16_from_double(int* _piAddress, int* _piKey)
{
	int i;
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	double* pdblIn	= NULL;

	short* psOut		= NULL;

	if(isVarComplex(_piAddress))
	{
		return 1;
	}

	iRet = getMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblIn);
	if(iRet)
	{
		return 1;
	}

	iRet = allocMatrixOfInteger16(Rhs + 1, iRows, iCols, &psOut, _piKey);
	if(iRet)
	{
		return 1;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		psOut[i] = (short)pdblIn[i];
	}

	return 0;
}

int uint16_from_double(int* _piAddress, int* _piKey)
{
	int i;
	int iRet								= 0;
	int iRows								= 0;
	int iCols								= 0;
	double* pdblIn					= NULL;

	unsigned short* pusOut	= NULL;

	if(isVarComplex(_piAddress))
	{
		return 1;
	}

	iRet = getMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblIn);
	if(iRet)
	{
		return 1;
	}

	iRet = allocMatrixOfUnsignedInteger16(Rhs + 1, iRows, iCols, &pusOut, _piKey);
	if(iRet)
	{
		return 1;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pusOut[i] = (unsigned short)pdblIn[i];
	}

	return 0;
}

int int32_from_double(int* _piAddress, int* _piKey)
{
	int i;
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	double* pdblIn	= NULL;

	int* piOut			= NULL;

	if(isVarComplex(_piAddress))
	{
		return 1;
	}

	iRet = getMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblIn);
	if(iRet)
	{
		return 1;
	}

	iRet = allocMatrixOfInteger32(Rhs + 1, iRows, iCols, &piOut, _piKey);
	if(iRet)
	{
		return 1;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		piOut[i] = (int)pdblIn[i];
	}

	return 0;
}

int uint32_from_double(int* _piAddress, int* _piKey)
{
	int i;
	int iRet							= 0;
	int iRows							= 0;
	int iCols							= 0;
	double* pdblIn				= NULL;

	unsigned int* puiOut	= NULL;

	if(isVarComplex(_piAddress))
	{
		return 1;
	}

	iRet = getMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblIn);
	if(iRet)
	{
		return 1;
	}

	iRet = allocMatrixOfUnsignedInteger32(Rhs + 1, iRows, iCols, &puiOut, _piKey);
	if(iRet)
	{
		return 1;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		puiOut[i] = (unsigned int)pdblIn[i];
	}

	return 0;
}

int int64_from_double(int* _piAddress, int* _piKey)
{
	int i;
	int iRet					= 0;
	int iRows					= 0;
	int iCols					= 0;
	double* pdblIn		= NULL;

	long long* pllOut	= NULL;

	if(isVarComplex(_piAddress))
	{
		return 1;
	}

	iRet = getMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblIn);
	if(iRet)
	{
		return 1;
	}

	iRet = allocMatrixOfInteger64(Rhs + 1, iRows, iCols, &pllOut, _piKey);
	if(iRet)
	{
		return 1;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pllOut[i] = (long long)pdblIn[i];
	}

	return 0;
}

int uint64_from_double(int* _piAddress, int* _piKey)
{
	int i;
	int iRet										= 0;
	int iRows										= 0;
	int iCols										= 0;
	double* pdblIn							= NULL;

	unsigned long long* pullOut	= NULL;

	if(isVarComplex(_piAddress))
	{
		return 1;
	}

	iRet = getMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblIn);
	if(iRet)
	{
		return 1;
	}

	iRet = allocMatrixOfUnsignedInteger64(Rhs + 1, iRows, iCols, &pullOut, _piKey);
	if(iRet)
	{
		return 1;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pullOut[i] = (unsigned long long)pdblIn[i];
	}

	return 0;
}

/*from int*/
int int8_from_int(int* _piAddress, int* _piKey)
{
	int i;
	int iRet			= 0;
	int iRows			= 0;
	int iCols			= 0;
	int iPrec			= 0;
	void* pvIn		= NULL;

	char* pcOut		= NULL;

	iRet = get_int(_piAddress, &iRows, &iCols, &iPrec, &pvIn);
	if(iRet)
	{
		return 1;
	}

	iRet = allocMatrixOfInteger8(Rhs + 1, iRows, iCols, &pcOut, _piKey);
	if(iRet)
	{
		return 1;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pcOut[i] = (char)*((char*)pvIn + (iPrec % 10) * i);
	}

	return 0;
}

int uint8_from_int(int* _piAddress, int* _piKey)
{
	int i;
	int iRet								= 0;
	int iRows								= 0;
	int iCols								= 0;
	int iPrec								= 0;
	void* pvIn							= NULL;

	unsigned char* pucOut		= NULL;

	iRet = get_int(_piAddress, &iRows, &iCols, &iPrec, &pvIn);
	if(iRet)
	{
		return 1;
	}

	iRet = allocMatrixOfUnsignedInteger8(Rhs + 1, iRows, iCols, &pucOut, _piKey);
	if(iRet)
	{
		return 1;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pucOut[i] = (unsigned char)*((unsigned char*)pvIn + (iPrec % 10) * i);
	}

	return 0;
}

int int16_from_int(int* _piAddress, int* _piKey)
{
	int i;
	int iRet			= 0;
	int iRows			= 0;
	int iCols			= 0;
	int iPrec			= 0;
	void* pvIn		= NULL;

	short* psOut	= NULL;

	iRet = get_int(_piAddress, &iRows, &iCols, &iPrec, &pvIn);
	if(iRet)
	{
		return 1;
	}

	iRet = allocMatrixOfInteger16(Rhs + 1, iRows, iCols, &psOut, _piKey);
	if(iRet)
	{
		return 1;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		psOut[i] = (short)*((short*)pvIn + (iPrec % 10) * i);
	}

	return 0;
}

int uint16_from_int(int* _piAddress, int* _piKey)
{
	int i;
	int iRet								= 0;
	int iRows								= 0;
	int iCols								= 0;
	int iPrec								= 0;
	void* pvIn							= NULL;

	unsigned short* pusOut	= NULL;

	iRet = get_int(_piAddress, &iRows, &iCols, &iPrec, &pvIn);
	if(iRet)
	{
		return 1;
	}

	iRet = allocMatrixOfUnsignedInteger16(Rhs + 1, iRows, iCols, &pusOut, _piKey);
	if(iRet)
	{
		return 1;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pusOut[i] = (unsigned short)*((unsigned short*)pvIn + (iPrec % 10) * i);
	}

	return 0;
}

int int32_from_int(int* _piAddress, int* _piKey)
{
	int i;
	int iRet			= 0;
	int iRows			= 0;
	int iCols			= 0;
	int iPrec			= 0;
	void* pvIn		= NULL;

	int* piOut		= NULL;

	iRet = get_int(_piAddress, &iRows, &iCols, &iPrec, &pvIn);
	if(iRet)
	{
		return 1;
	}

	iRet = allocMatrixOfInteger32(Rhs + 1, iRows, iCols, &piOut, _piKey);
	if(iRet)
	{
		return 1;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		piOut[i] = (int)*((int*)pvIn + (iPrec % 10) * i);
	}

	return 0;
}

int uint32_from_int(int* _piAddress, int* _piKey)
{
	int i;
	int iRet								= 0;
	int iRows								= 0;
	int iCols								= 0;
	int iPrec								= 0;
	void* pvIn							= NULL;

	unsigned int* puiOut		= NULL;

	iRet = get_int(_piAddress, &iRows, &iCols, &iPrec, &pvIn);
	if(iRet)
	{
		return 1;
	}

	iRet = allocMatrixOfUnsignedInteger32(Rhs + 1, iRows, iCols, &puiOut, _piKey);
	if(iRet)
	{
		return 1;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		puiOut[i] = (unsigned int)*((unsigned int*)pvIn + (iPrec % 10) * i);
	}

	return 0;
}

int int64_from_int(int* _piAddress, int* _piKey)
{
	int i;
	int iRet					= 0;
	int iRows					= 0;
	int iCols					= 0;
	int iPrec					= 0;
	void* pvIn				= NULL;

	long long* pllOut	= NULL;

	iRet = get_int(_piAddress, &iRows, &iCols, &iPrec, &pvIn);
	if(iRet)
	{
		return 1;
	}

	iRet = allocMatrixOfInteger64(Rhs + 1, iRows, iCols, &pllOut, _piKey);
	if(iRet)
	{
		return 1;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pllOut[i] = (long long)*((long long*)pvIn + (iPrec % 10) * i);
	}

	return 0;
}

int uint64_from_int(int* _piAddress, int* _piKey)
{
	int i;
	int iRet										= 0;
	int iRows										= 0;
	int iCols										= 0;
	int iPrec										= 0;
	void* pvIn									= NULL;

	unsigned long long* pullOut	= NULL;

	iRet = get_int(_piAddress, &iRows, &iCols, &iPrec, &pvIn);
	if(iRet)
	{
		return 1;
	}

	iRet = allocMatrixOfUnsignedInteger64(Rhs + 1, iRows, iCols, &pullOut, _piKey);
	if(iRet)
	{
		return 1;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pullOut[i] = (unsigned long long)*((unsigned long long*)pvIn + (iPrec % 10) * i);
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
