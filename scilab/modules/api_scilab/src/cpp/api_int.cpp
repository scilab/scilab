/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 * Please note that piece of code will be rewrited for the Scilab 6 family
 * However, the API (profile of the functions in the header files) will be 
 * still available and supported in Scilab 6.
 */

#include "api_common.h"
#include "api_internal_common.h"
#include "api_internal_int.h"
#include "api_int.h"
#include "localization.h"

#include "call_scilab.h"
#include "stack-c.h"

StrErr getMatrixOfIntegerPrecision(void* _pvCtx, int* _piAddress, int* _piPrecision)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	if(_piAddress == NULL)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getMatrixOfIntegerPrecision");
		return strErr;
	}

	if(_piAddress[0] != sci_ints)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), "getMatrixOfIntegerPrecision", _("int matrix"));
		return strErr;
	}

	*_piPrecision = _piAddress[3];
	return strErr;
}

StrErr getMatrixOfUnsignedInteger8(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, unsigned char** _pucData8)
{
	return getCommonMatrixOfInteger(_pvCtx, _piAddress, SCI_UINT8, _piRows, _piCols, (void**)_pucData8);
}

StrErr getMatrixOfUnsignedInteger16(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, unsigned short** _pusData16)
{
	return getCommonMatrixOfInteger(_pvCtx, _piAddress, SCI_UINT16, _piRows, _piCols, (void**)_pusData16);
}

StrErr getMatrixOfUnsignedInteger32(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, unsigned int** _puiData32)
{
	return getCommonMatrixOfInteger(_pvCtx, _piAddress, SCI_UINT32, _piRows, _piCols, (void**)_puiData32);
}

StrErr getMatrixOfInteger8(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, char** _pcData8)
{
	return getCommonMatrixOfInteger(_pvCtx, _piAddress, SCI_INT8, _piRows, _piCols, (void**)_pcData8);
}

StrErr getMatrixOfInteger16(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, short** _psData16)
{
	return getCommonMatrixOfInteger(_pvCtx, _piAddress, SCI_INT16, _piRows, _piCols, (void**)_psData16);
}

StrErr getMatrixOfInteger32(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piData32)
{
	return getCommonMatrixOfInteger(_pvCtx, _piAddress, SCI_INT32, _piRows, _piCols, (void**)_piData32);
}

StrErr getCommonMatrixOfInteger(void* _pvCtx, int* _piAddress, int _iPrecision, int* _piRows, int* _piCols, void** _piData)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iPrec		= 0;

	if(_piAddress == NULL)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getMatrixOfInteger");
		return strErr;
	}

	strErr = getMatrixOfIntegerPrecision(_pvCtx, _piAddress, &iPrec);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_INT, _("%s: Unable to get argument #%d"), "getMatrixOfInteger", getRhsFromAddress(_pvCtx, _piAddress));
		return strErr;
	}

	if(iPrec != _iPrecision)
	{
		addErrorMessage(&strErr, API_ERROR_GET_INT, _("%s: Unable to get argument #%d"), "getMatrixOfInteger", getRhsFromAddress(_pvCtx, _piAddress));
		return strErr;
	}

	*_piRows = _piAddress[1];
	*_piCols = _piAddress[2];

	*_piData	= (void*)(_piAddress + 4);
	return strErr;
}

StrErr createMatrixOfUnsignedInteger8(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned char* _pucData8)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	unsigned char *pucData8		= NULL;
	int iSize									= _iRows * _iCols;

	strErr = allocMatrixOfUnsignedInteger8(_pvCtx, _iVar, _iRows, _iCols, &pucData8);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfUnsignedInteger8");
		return strErr;
	}

	memcpy(pucData8, _pucData8, sizeof(unsigned char) * iSize);
	return strErr;
}

StrErr createMatrixOfUnsignedInteger16(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned short* _pusData16)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	unsigned short *psData16	= NULL;
	int iSize									= _iRows * _iCols;

	strErr = allocMatrixOfUnsignedInteger16(_pvCtx, _iVar, _iRows, _iCols, &psData16);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfUnsignedInteger16");
		return strErr;
	}

	memcpy(psData16, _pusData16, sizeof(unsigned short) * iSize);
	return strErr;
}

StrErr createMatrixOfUnsignedInteger32(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned int* _puiData32)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	unsigned int *piData32	= NULL;
	int iSize			= _iRows * _iCols;

	strErr = allocMatrixOfUnsignedInteger32(_pvCtx, _iVar, _iRows, _iCols, &piData32);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfUnsignedInteger32");
		return strErr;
	}

	memcpy(piData32, _puiData32, sizeof(unsigned int) * iSize);
	return strErr;
}

StrErr createMatrixOfInteger8(void* _pvCtx, int _iVar, int _iRows, int _iCols, char* _pcData8)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	char *pcData8	= NULL;
	int iSize			= _iRows * _iCols;

	strErr = allocMatrixOfInteger8(_pvCtx, _iVar, _iRows, _iCols, &pcData8);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfInteger8");
		return strErr;
	}

	memcpy(pcData8, _pcData8, sizeof(char) * iSize);
	return strErr;
}

StrErr createMatrixOfInteger16(void* _pvCtx, int _iVar, int _iRows, int _iCols, short* _psData16)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	short *psData16	= NULL;
	int iSize			= _iRows * _iCols;

	strErr = allocMatrixOfInteger16(_pvCtx, _iVar, _iRows, _iCols, &psData16);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfInteger16");
		return strErr;
	}

	memcpy(psData16, _psData16, sizeof(short) * iSize);
	return strErr;
}

StrErr createMatrixOfInteger32(void* _pvCtx, int _iVar, int _iRows, int _iCols, int* _piData32)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int *piData32	= NULL;
	int iSize			= _iRows * _iCols;

	strErr = allocMatrixOfInteger32(_pvCtx, _iVar, _iRows, _iCols, &piData32);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfInteger32");
		return strErr;
	}

	memcpy(piData32, _piData32, sizeof(int) * iSize);
	return strErr;
}

#ifdef __SCILAB_INT64__
StrErr createMatrixOfInteger64(void* _pvCtx, int _iVar, int _iRows, int _iCols, long long* _pllData64)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	long long  *pllData64	= NULL;
	int iSize							= _iRows * _iCols;

	strErr = allocMatrixOfInteger64(_iVar, _iRows, _iCols, &pllData64);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfInteger64");
		return strErr;
	}

	memcpy(pllData64, _pllData64, sizeof(long long) * iSize);
	return strErr;
}
#endif

StrErr allocMatrixOfInteger8(void* _pvCtx, int _iVar, int _iRows, int _iCols, char** _pcData8)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int *piAddr		= NULL;
	char *pcData8	= NULL;
	int iNewPos		= Top - Rhs + _iVar;

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	strErr = allocCommonMatrixOfInteger(_pvCtx, _iVar, piAddr, SCI_INT8, _iRows, _iCols, (void**)&pcData8);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfInteger8");
		return strErr;
	}

	*_pcData8		= pcData8;
	return strErr;
}

StrErr allocMatrixOfInteger16(void* _pvCtx, int _iVar, int _iRows, int _iCols, short** _psData16)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int *piAddr			= NULL;
	short *psData16	= NULL;
	int iNewPos			= Top - Rhs + _iVar;

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	strErr = allocCommonMatrixOfInteger(_pvCtx, _iVar, piAddr, SCI_INT16, _iRows, _iCols, (void**)&psData16);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfInteger16");
		return strErr;
	}

	*_psData16	= psData16;
	return strErr;
}

StrErr allocMatrixOfInteger32(void* _pvCtx, int _iVar, int _iRows, int _iCols, int** _piData32)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int *piAddr		= NULL;
	int *piData32	= NULL;
	int iNewPos		= Top - Rhs + _iVar;

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	strErr = allocCommonMatrixOfInteger(_pvCtx, _iVar, piAddr, SCI_INT32, _iRows, _iCols, (void**)&piData32);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfInteger32");
		return strErr;
	}

	*_piData32	= piData32;
	return strErr;
}

#ifdef __SCILAB_INT64__
StrErr allocMatrixOfInteger64(void* _pvCtx, int _iVar, int _iRows, int _iCols, long long** _piData64)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int *piAddr						= NULL;
	long long *pllData64	= NULL;
	int iNewPos						= Top - Rhs + _iVar;

	getNewVarAddressFromPosition(iNewPos, &piAddr);

	strErr = allocCommonMatrixOfInteger(_iVar, piAddr, SCI_INT64, _iRows, _iCols, (void**)&piData32);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfInteger64");
		return strErr;
	}

	*_pllData64	= pllData64;
	return strErr;
}
#endif

StrErr allocMatrixOfUnsignedInteger8(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned char** _pucData8)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int *piAddr							= NULL;
	unsigned char *pucData8	= NULL;
	int iNewPos							= Top - Rhs + _iVar;

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	strErr = allocCommonMatrixOfInteger(_pvCtx, _iVar, piAddr, SCI_UINT8, _iRows, _iCols, (void**)&pucData8);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfUnsignedInteger8");
		return strErr;
	}

	*_pucData8		= pucData8;
	return strErr;
}

StrErr allocMatrixOfUnsignedInteger16(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned short** _pusData16)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int *piAddr								= NULL;
	unsigned short *pusData16	= NULL;
	int iNewPos								= Top - Rhs + _iVar;

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	strErr = allocCommonMatrixOfInteger(_pvCtx, _iVar, piAddr, SCI_UINT16, _iRows, _iCols, (void**)&pusData16);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfUnsignedInteger16");
		return strErr;
	}

	*_pusData16	= pusData16;
	return strErr;
}

StrErr allocMatrixOfUnsignedInteger32(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned int** _puiData32)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int *piAddr							= NULL;
	unsigned int *puiData32	= NULL;
	int iNewPos							= Top - Rhs + _iVar;

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	strErr = allocCommonMatrixOfInteger(_pvCtx, _iVar, piAddr, SCI_INT32, _iRows, _iCols, (void**)&puiData32);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfUnsignedInteger32");
		return strErr;
	}

	*_puiData32	= puiData32;
	return strErr;
}

#ifdef __SCILAB_INT64__
StrErr allocMatrixOfUnsignedInteger64(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned long long** _pullData64)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int *piAddr											= NULL;
	unsigned long long *pullData64	= NULL;
	int iNewPos											= Top - Rhs + _iVar;

	getNewVarAddressFromPosition(iNewPos, &piAddr);

	strErr = allocCommonMatrixOfInteger(_iVar, piAddr, SCI_UINT64, _iRows, _iCols, (void**)&puiData32);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_INT, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfUnsignedInteger64");
		return strErr;
	}

	*_pullData64	= pullData64;
	return strErr;
}
#endif

StrErr allocCommonMatrixOfInteger(void* _pvCtx, int _iVar, int *_piAddress, int _iPrecision, int _iRows, int _iCols, void** pvData)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);

	int iRate				= (sizeof(double) / (_iPrecision % 10));
	int iSize				= _iRows * _iCols;
	int iDouble			= iSize / iRate;
	int iMod				= (iSize % iRate) == 0 ? 0 : 1;
	int iTotalSize	= iDouble + iMod;

	int iMemSize = iTotalSize + 2;
	int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(iAddr));
	if (iMemSize > iFreeSpace)
	{
		addStackSizeError(&strErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
		return strErr;
	}

	fillCommonMatrixOfInteger(_pvCtx, _piAddress, _iPrecision, _iRows, _iCols, pvData);
	updateInterSCI(_iVar, '$', iAddr, iAddr + 4);
	updateLstk(iNewPos, iAddr + 4, iTotalSize );
	return strErr;
}

StrErr fillCommonMatrixOfInteger(void* _pvCtx, int* _piAddress, int _iPrecision, int _iRows, int _iCols, void** _pvData)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	_piAddress[0]		= sci_ints;
	_piAddress[1]		= Min(_iRows, _iRows * _iCols);
	_piAddress[2]		= Min(_iCols, _iRows * _iCols);
	_piAddress[3]		= _iPrecision;


	*_pvData		= (void*)(_piAddress + 4);

	return strErr;
}

StrErr createNamedMatrixOfUnsignedInteger8(void* _pvCtx, char* _pstName, int _iRows, int _iCols, unsigned char* _pucData8)
{
	return createCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_UINT8, _iRows, _iCols, _pucData8);
}

StrErr createNamedMatrixOfUnsignedInteger16(void* _pvCtx, char* _pstName, int _iRows, int _iCols, unsigned short* _pusData16)
{
	return createCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_UINT16, _iRows, _iCols, _pusData16);
}

StrErr createNamedMatrixOfUnsignedInteger32(void* _pvCtx, char* _pstName, int _iRows, int _iCols, unsigned int* _puiData32)
{
	return createCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_UINT32, _iRows, _iCols, _puiData32);
}

StrErr createNamedMatrixOfInteger8(void* _pvCtx, char* _pstName, int _iRows, int _iCols, char* _pcData8)
{
	return createCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_INT8, _iRows, _iCols, _pcData8);
}

StrErr createNamedMatrixOfInteger16(void* _pvCtx, char* _pstName, int _iRows, int _iCols, short* _psData16)
{
	return createCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_INT16, _iRows, _iCols, _psData16);
}

StrErr createNamedMatrixOfInteger32(void* _pvCtx, char* _pstName, int _iRows, int _iCols, int* _piData32)
{
	return createCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_INT32, _iRows, _iCols, _piData32);
}

#ifdef __SCILAB_INT64__
StrErr createNamedMatrixOfInteger64(void* _pvCtx, char* _pstName, int _iRows, int _iCols, long long* _pllData64)
{
	return -1;
}
#endif

StrErr createCommonNamedMatrixOfInteger(void* _pvCtx, char* _pstName, int _iPrecision, int _iRows, int _iCols, void* _pvData)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int *piAddr				= NULL;
	void *pvData			= NULL;

	int iRate				= (sizeof(double) / (_iPrecision % 10));
	int iSize				= _iRows * _iCols;
	int iDouble			= iSize / iRate;
	int iMod				= (iSize % iRate) == 0 ? 0 : 1;
	int iTotalSize	= iDouble + iMod;

	C2F(str2name)(_pstName, iVarID, (int)strlen(_pstName));
  Top = Top + Nbvars + 1;

	int iMemSize = iTotalSize + 2;
	int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(Top));
	if (iMemSize > iFreeSpace)
	{
		addStackSizeError(&strErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
		return strErr;
	}

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

	//write matrix information
	fillCommonMatrixOfInteger(_pvCtx, piAddr, _iPrecision, _iRows, _iCols, &pvData);
	//copy data in stack
	memcpy(pvData, _pvData, (_iPrecision % 10) * iSize);

	//update "variable index"
	updateLstk(Top, *Lstk(Top) + 4, iTotalSize);

	Rhs = 0;
	//Add name in stack reference list
	createNamedVariable(iVarID);

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return strErr;
}

StrErr getNamedMatrixOfIntegerPrecision(void* _pvCtx, char* _pstName, int* _piPrecision)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr				= NULL;

	strErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_NAMED_INT_PRECISION, _("%s: Unable to get precision of variable \"%s\""), "getNamedMatrixOfIntegerPrecision", _pstName);
		return strErr;
	}

	//check variable type
	if(piAddr[0] != sci_ints)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), "getNamedMatrixOfIntegerPrecision", _("int matrix"));
		return strErr;
	}

	*_piPrecision = piAddr[3];
	return strErr;
}

StrErr readNamedMatrixOfUnsignedInteger8(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, unsigned char* _pucData8)
{
	return readCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_UINT8, _piRows, _piCols, _pucData8);
}

StrErr readNamedMatrixOfUnsignedInteger16(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, unsigned short* _pusData16)
{
	return readCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_UINT16, _piRows, _piCols, _pusData16);
}

StrErr readNamedMatrixOfUnsignedInteger32(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, unsigned int* _puiData32)
{
	return readCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_UINT32, _piRows, _piCols, _puiData32);
}

StrErr readNamedMatrixOfInteger8(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, char* _pcData8)
{
	return readCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_INT8, _piRows, _piCols, _pcData8);
}

StrErr readNamedMatrixOfInteger16(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, short* _psData16)
{
	return readCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_INT16, _piRows, _piCols, _psData16);
}

StrErr readNamedMatrixOfInteger32(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piData32)
{
	return readCommonNamedMatrixOfInteger(_pvCtx, _pstName, SCI_INT32, _piRows, _piCols, _piData32);
}

#ifdef __SCILAB_INT64__
StrErr readNamedMatrixOfInteger64(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, long long* _pllData64)
{
	return 1;
}
#endif

StrErr readCommonNamedMatrixOfInteger(void* _pvCtx, char* _pstName, int _iPrecision, int* _piRows, int* _piCols, void* _pvData)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr				= NULL;
	int iSize					= 0;
	void* pvData			= NULL;

	strErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_NAMED_INT, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfInteger", _pstName);
		return strErr;
	}

	strErr = getCommonMatrixOfInteger(_pvCtx, piAddr, _iPrecision, _piRows, _piCols, &pvData);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_NAMED_INT, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfInteger", _pstName);
		return strErr;
	}
	iSize = *_piRows * *_piCols;

	if(pvData == NULL || _pvData == NULL)
	{
		return strErr;
	}

	memcpy(_pvData, pvData, (_iPrecision % 10) * iSize);
	return strErr;
}
/*--------------------------------------------------------------------------*/
