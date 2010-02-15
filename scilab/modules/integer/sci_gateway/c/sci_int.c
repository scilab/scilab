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

int get_int(int* _piKey, int* _piAddress, int* _piRows, int* _piCols, int* _piPrecision, void** pvData);

int int8_from_double(int* _piKey, int* _piAddress);
int uint8_from_double(int* _piKey, int* _piAddress);
int int16_from_double(int* _piKey, int* _piAddress);
int uint16_from_double(int* _piKey, int* _piAddress);
int int32_from_double(int* _piKey, int* _piAddress);
int uint32_from_double(int* _piKey, int* _piAddress);
int int64_from_double(int* _piKey, int* _piAddress);
int uint64_from_double(int* _piKey, int* _piAddress);

int int8_from_int(int* _piKey, int* _piAddress);
int uint8_from_int(int* _piKey, int* _piAddress);
int int16_from_int(int* _piKey, int* _piAddress);
int uint16_from_int(int* _piKey, int* _piAddress);
int int32_from_int(int* _piKey, int* _piAddress);
int uint32_from_int(int* _piKey, int* _piAddress);
int int64_from_int(int* _piKey, int* _piAddress);
int uint64_from_int(int* _piKey, int* _piAddress);

/*--------------------------------------------------------------------------*/
int sci_int8(char *fname, int* _piKey)
{
	SciErr sciErr;
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int* piAddr			= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(isEmptyMatrix(_piKey, piAddr))
	{
		iRet = createEmptyMatrix(_piKey, Rhs + 1);
		if(iRet)
		{
			return 0;
		}
	}

	if(isDoubleType(_piKey, piAddr))
	{
		iRet = int8_from_double(_piKey, piAddr);
	}
	else if(isIntegerType(_piKey, piAddr))
	{
		iRet = int8_from_int(_piKey, piAddr);
	}
	else
	{
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
	SciErr sciErr;
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int* piAddr			= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(isEmptyMatrix(_piKey, piAddr))
	{
		iRet = createEmptyMatrix(_piKey, Rhs + 1);
		if(iRet)
		{
			return 0;
		}
	}

	if(isDoubleType(_piKey, piAddr))
	{
		iRet = uint8_from_double(_piKey, piAddr);
	}
	else if(isIntegerType(_piKey, piAddr))
	{
		iRet = uint8_from_int(_piKey, piAddr);
	}
	else
	{
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
	SciErr sciErr;
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int* piAddr			= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(isEmptyMatrix(_piKey, piAddr))
	{
		iRet = createEmptyMatrix(_piKey, Rhs + 1);
		if(iRet)
		{
			return 0;
		}
	}

	if(isDoubleType(_piKey, piAddr))
	{
		iRet = int16_from_double(_piKey, piAddr);
	}
	else if(isIntegerType(_piKey, piAddr))
	{
		iRet = int16_from_int(_piKey, piAddr);
	}
	else
	{
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
	SciErr sciErr;
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int* piAddr			= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(isEmptyMatrix(_piKey, piAddr))
	{
		iRet = createEmptyMatrix(_piKey, Rhs + 1);
		if(iRet)
		{
			return 0;
		}
	}

	if(isDoubleType(_piKey, piAddr))
	{
		iRet = uint16_from_double(_piKey, piAddr);
	}
	else if(isIntegerType(_piKey, piAddr))
	{
		iRet = uint16_from_int(_piKey, piAddr);
	}
	else
	{
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
	SciErr sciErr;
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int* piAddr			= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(isEmptyMatrix(_piKey, piAddr))
	{
		iRet = createEmptyMatrix(_piKey, Rhs + 1);
		if(iRet)
		{
			return 0;
		}
	}

	if(isDoubleType(_piKey, piAddr))
	{
		iRet = int32_from_double(_piKey, piAddr);
	}
	else if(isIntegerType(_piKey, piAddr))
	{
		iRet = int32_from_int(_piKey, piAddr);
	}
	else
	{
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
	SciErr sciErr;
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int* piAddr			= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(isEmptyMatrix(_piKey, piAddr))
	{
		iRet = createEmptyMatrix(_piKey, Rhs + 1);
		if(iRet)
		{
			return 0;
		}
	}

	if(isDoubleType(_piKey, piAddr))
	{
		iRet = uint32_from_double(_piKey, piAddr);
	}
	else if(isIntegerType(_piKey, piAddr))
	{
		iRet = uint32_from_int(_piKey, piAddr);
	}
	else
	{
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
	SciErr sciErr;
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int* piAddr			= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(isEmptyMatrix(_piKey, piAddr))
	{
		iRet = createEmptyMatrix(_piKey, Rhs + 1);
		if(iRet)
		{
			return 0;
		}
	}

	if(isDoubleType(_piKey, piAddr))
	{
		iRet = int64_from_double(_piKey, piAddr);
	}
	else if(isIntegerType(_piKey, piAddr))
	{
		iRet = int64_from_int(_piKey, piAddr);
	}
	else
	{
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
	SciErr sciErr;
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int* piAddr			= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(isEmptyMatrix(_piKey, piAddr))
	{
		iRet = createEmptyMatrix(_piKey, Rhs + 1);
		if(iRet)
		{
			return 0;
		}
	}

	if(isDoubleType(_piKey, piAddr))
	{
		iRet = uint64_from_double(_piKey, piAddr);
	}
	else if(isIntegerType(_piKey, piAddr))
	{
		iRet = uint64_from_int(_piKey, piAddr);
	}
	else
	{
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

int get_int(int* _piKey, int* _piAddress, int* _piRows, int* _piCols, int* _piPrecision, void** pvData)
{
	SciErr sciErr;
	int iRet				= 0;
	void* pvIn			= NULL;

	char* pcOut			= NULL;

	sciErr = getMatrixOfIntegerPrecision(_piKey, _piAddress, _piPrecision);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	switch(*_piPrecision)
	{
	case SCI_INT8 : 
		{
			sciErr = getMatrixOfInteger8(_piKey, _piAddress, _piRows, _piCols, (char**)pvData);
		}
	case SCI_UINT8 : 
		{
			sciErr = getMatrixOfUnsignedInteger8(_piKey, _piAddress, _piRows, _piCols, (unsigned char**)pvData);
		}
	case SCI_INT16 : 
		{
			sciErr = getMatrixOfInteger16(_piKey, _piAddress, _piRows, _piCols, (short**)pvData);
		}
	case SCI_UINT16 : 
		{
			sciErr = getMatrixOfUnsignedInteger16(_piKey, _piAddress, _piRows, _piCols, (unsigned short**)pvData);
		}
	case SCI_INT32 : 
		{
			sciErr = getMatrixOfInteger32(_piKey, _piAddress, _piRows, _piCols, (int**)pvData);
		}
	case SCI_UINT32 : 
		{
			sciErr = getMatrixOfUnsignedInteger32(_piKey, _piAddress, _piRows, _piCols, (unsigned int**)pvData);
		}
	case SCI_INT64 : 
		{
			sciErr = getMatrixOfInteger64(_piKey, _piAddress, _piRows, _piCols, (long long**)pvData);
		}
	case SCI_UINT64 : 
		{
			sciErr = getMatrixOfUnsignedInteger64(_piKey, _piAddress, _piRows, _piCols, (unsigned long long**)pvData);
		}
	default :
		{
			*_piRows = 0;
			*_piCols = 0;
			*pvData = NULL;
			return 1;
		}
	}

	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}
	return 0;
}


int int8_from_double(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRows				= 0;
	int iCols				= 0;
	double* pdblIn	= NULL;

	char* pcOut			= NULL;

	sciErr = getMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblIn);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	sciErr = allocMatrixOfInteger8(_piKey, Rhs + 1, iRows, iCols, &pcOut);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pcOut[i] = (char)pdblIn[i];
	}

	return 0;
}

int uint8_from_double(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRows							= 0;
	int iCols							= 0;
	double* pdblIn				= NULL;

	unsigned char* pucOut	= NULL;

	sciErr = getMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblIn);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	sciErr = allocMatrixOfUnsignedInteger8(_piKey, Rhs + 1, iRows, iCols, &pucOut);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pucOut[i] = (unsigned char)pdblIn[i];
	}

	return 0;
}

int int16_from_double(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRows				= 0;
	int iCols				= 0;
	double* pdblIn	= NULL;

	short* psOut		= NULL;

	sciErr = getMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblIn);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	sciErr = allocMatrixOfInteger16(_piKey, Rhs + 1, iRows, iCols, &psOut);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		psOut[i] = (short)pdblIn[i];
	}

	return 0;
}

int uint16_from_double(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRows								= 0;
	int iCols								= 0;
	double* pdblIn					= NULL;

	unsigned short* pusOut	= NULL;

	sciErr = getMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblIn);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	sciErr = allocMatrixOfUnsignedInteger16(_piKey, Rhs + 1, iRows, iCols, &pusOut);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pusOut[i] = (unsigned short)pdblIn[i];
	}

	return 0;
}

int int32_from_double(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRows				= 0;
	int iCols				= 0;
	double* pdblIn	= NULL;

	int* piOut			= NULL;

	sciErr = getMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblIn);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	sciErr = allocMatrixOfInteger32(_piKey, Rhs + 1, iRows, iCols, &piOut);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		piOut[i] = (int)pdblIn[i];
	}

	return 0;
}

int uint32_from_double(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRows							= 0;
	int iCols							= 0;
	double* pdblIn				= NULL;

	unsigned int* puiOut	= NULL;

	sciErr = getMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblIn);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	sciErr = allocMatrixOfUnsignedInteger32(_piKey, Rhs + 1, iRows, iCols, &puiOut);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		puiOut[i] = (unsigned int)pdblIn[i];
	}

	return 0;
}

int int64_from_double(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRows					= 0;
	int iCols					= 0;
	double* pdblIn		= NULL;

	long long* pllOut	= NULL;

	sciErr = getMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblIn);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	sciErr = allocMatrixOfInteger64(_piKey, Rhs + 1, iRows, iCols, &pllOut);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pllOut[i] = (long long)pdblIn[i];
	}

	return 0;
}

int uint64_from_double(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRows										= 0;
	int iCols										= 0;
	double* pdblIn							= NULL;

	unsigned long long* pullOut	= NULL;

	sciErr = getMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblIn);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	sciErr = allocMatrixOfUnsignedInteger64(_piKey, Rhs + 1, iRows, iCols, &pullOut);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pullOut[i] = (unsigned long long)pdblIn[i];
	}

	return 0;
}

/*from int*/
int int8_from_int(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRet			= 0;
	int iRows			= 0;
	int iCols			= 0;
	int iPrec			= 0;
	void* pvIn		= NULL;

	char* pcOut		= NULL;

	iRet = get_int(_piKey, _piAddress, &iRows, &iCols, &iPrec, &pvIn);
	if(iRet)
	{
		return 1;
	}

	sciErr = allocMatrixOfInteger8(_piKey, Rhs + 1, iRows, iCols, &pcOut);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pcOut[i] = (char)*((char*)pvIn + (iPrec % 10) * i);
	}

	return 0;
}

int uint8_from_int(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRet								= 0;
	int iRows								= 0;
	int iCols								= 0;
	int iPrec								= 0;
	void* pvIn							= NULL;

	unsigned char* pucOut		= NULL;

	iRet = get_int(_piKey, _piAddress, &iRows, &iCols, &iPrec, &pvIn);
	if(iRet)
	{
		return 1;
	}

	sciErr = allocMatrixOfUnsignedInteger8(_piKey, Rhs + 1, iRows, iCols, &pucOut);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pucOut[i] = (unsigned char)*((unsigned char*)pvIn + (iPrec % 10) * i);
	}

	return 0;
}

int int16_from_int(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRet			= 0;
	int iRows			= 0;
	int iCols			= 0;
	int iPrec			= 0;
	void* pvIn		= NULL;

	short* psOut	= NULL;

	iRet = get_int(_piKey, _piAddress, &iRows, &iCols, &iPrec, &pvIn);
	if(iRet)
	{
		return 1;
	}

	sciErr = allocMatrixOfInteger16(_piKey, Rhs + 1, iRows, iCols, &psOut);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		psOut[i] = (short)*((short*)pvIn + (iPrec % 10) * i);
	}

	return 0;
}

int uint16_from_int(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRet								= 0;
	int iRows								= 0;
	int iCols								= 0;
	int iPrec								= 0;
	void* pvIn							= NULL;

	unsigned short* pusOut	= NULL;

	iRet = get_int(_piKey, _piAddress, &iRows, &iCols, &iPrec, &pvIn);
	if(iRet)
	{
		return 1;
	}

	sciErr = allocMatrixOfUnsignedInteger16(_piKey, Rhs + 1, iRows, iCols, &pusOut);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pusOut[i] = (unsigned short)*((unsigned short*)pvIn + (iPrec % 10) * i);
	}

	return 0;
}

int int32_from_int(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRet			= 0;
	int iRows			= 0;
	int iCols			= 0;
	int iPrec			= 0;
	void* pvIn		= NULL;

	int* piOut		= NULL;

	iRet = get_int(_piKey, _piAddress, &iRows, &iCols, &iPrec, &pvIn);
	if(iRet)
	{
		return 1;
	}

	sciErr = allocMatrixOfInteger32(_piKey, Rhs + 1, iRows, iCols, &piOut);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		piOut[i] = (int)*((int*)pvIn + (iPrec % 10) * i);
	}

	return 0;
}

int uint32_from_int(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRet								= 0;
	int iRows								= 0;
	int iCols								= 0;
	int iPrec								= 0;
	void* pvIn							= NULL;

	unsigned int* puiOut		= NULL;

	iRet = get_int(_piKey, _piAddress, &iRows, &iCols, &iPrec, &pvIn);
	if(iRet)
	{
		return 1;
	}

	sciErr = allocMatrixOfUnsignedInteger32(_piKey, Rhs + 1, iRows, iCols, &puiOut);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		puiOut[i] = (unsigned int)*((unsigned int*)pvIn + (iPrec % 10) * i);
	}

	return 0;
}

int int64_from_int(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRet					= 0;
	int iRows					= 0;
	int iCols					= 0;
	int iPrec					= 0;
	void* pvIn				= NULL;

	long long* pllOut	= NULL;

	iRet = get_int(_piKey, _piAddress, &iRows, &iCols, &iPrec, &pvIn);
	if(iRet)
	{
		return 1;
	}

	sciErr = allocMatrixOfInteger64(_piKey, Rhs + 1, iRows, iCols, &pllOut);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pllOut[i] = (long long)*((long long*)pvIn + (iPrec % 10) * i);
	}

	return 0;
}

int uint64_from_int(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int i;
	int iRet										= 0;
	int iRows										= 0;
	int iCols										= 0;
	int iPrec										= 0;
	void* pvIn									= NULL;

	unsigned long long* pullOut	= NULL;

	iRet = get_int(_piKey, _piAddress, &iRows, &iCols, &iPrec, &pvIn);
	if(iRet)
	{
		return 1;
	}

	sciErr = allocMatrixOfUnsignedInteger64(_piKey, Rhs + 1, iRows, iCols, &pullOut);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pullOut[i] = (unsigned long long)*((unsigned long long*)pvIn + (iPrec % 10) * i);
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
