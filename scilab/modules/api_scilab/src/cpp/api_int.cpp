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

#include "CallScilab.h"
#include "stack-c.h"

int getMatrixOfIntegerPrecision(int* _piAddress, int* _piPrecision)
{
	if(_piAddress == NULL)
	{
		return 1;
	}

	if(_piAddress[0] != sci_ints)
	{
		return 1;
	}

	*_piPrecision = _piAddress[3];
	return 0;
}

int getMatrixOfUnsignedInteger8(int* _piAddress, int* _piRows, int* _piCols, unsigned char** _pucData8)
{
	return getCommonMatrixOfInteger(_piAddress, SCI_UINT8, _piRows, _piCols, (void**)_pucData8);
}

int getMatrixOfUnsignedInteger16(int* _piAddress, int* _piRows, int* _piCols, unsigned short** _pusData16)
{
	return getCommonMatrixOfInteger(_piAddress, SCI_UINT16, _piRows, _piCols, (void**)_pusData16);
}

int getMatrixOfUnsignedInteger32(int* _piAddress, int* _piRows, int* _piCols, unsigned int** _puiData32)
{
	return getCommonMatrixOfInteger(_piAddress, SCI_UINT32, _piRows, _piCols, (void**)_puiData32);
}

int getMatrixOfInteger8(int* _piAddress, int* _piRows, int* _piCols, char** _pcData8)
{
	return getCommonMatrixOfInteger(_piAddress, SCI_INT8, _piRows, _piCols, (void**)_pcData8);
}

int getMatrixOfInteger16(int* _piAddress, int* _piRows, int* _piCols, short** _psData16)
{
	return getCommonMatrixOfInteger(_piAddress, SCI_INT16, _piRows, _piCols, (void**)_psData16);
}

int getMatrixOfInteger32(int* _piAddress, int* _piRows, int* _piCols, int** _piData32)
{
	return getCommonMatrixOfInteger(_piAddress, SCI_INT32, _piRows, _piCols, (void**)_piData32);
}

int getCommonMatrixOfInteger(int* _piAddress, int _iPrecision, int* _piRows, int* _piCols, void** _piData)
{
	int iPrec		= 0;
	int iRet		= 0;

	if(_piAddress == NULL)
	{
		return 1;
	}

	iRet = getMatrixOfIntegerPrecision(_piAddress, &iPrec);
	if(iPrec != _iPrecision || iRet)
	{
		return 1;
	}

	*_piRows = _piAddress[1];
	*_piCols = _piAddress[2];

	*_piData	= (void*)(_piAddress + 4);
	return 0;
}

int createMatrixOfUnsignedInteger8(int _iVar, int _iRows, int _iCols, unsigned char* _pucData8)
{
	unsigned char *pucData8		= NULL;
	int iSize									= _iRows * _iCols;

	int iRet = allocMatrixOfUnsignedInteger8(_iVar, _iRows, _iCols, &pucData8);
	if(iRet)
	{
		return 1;
	}

	memcpy(pucData8, _pucData8, sizeof(unsigned char) * iSize);
	return 0;
}

int createMatrixOfUnsignedInteger16(int _iVar, int _iRows, int _iCols, unsigned short* _pusData16)
{
	unsigned short *psData16	= NULL;
	int iSize									= _iRows * _iCols;

	int iRet = allocMatrixOfUnsignedInteger16(_iVar, _iRows, _iCols, &psData16);
	if(iRet)
	{
		return 1;
	}

	memcpy(psData16, _pusData16, sizeof(unsigned short) * iSize);
	return 0;
}

int createMatrixOfUnsignedInteger32(int _iVar, int _iRows, int _iCols, unsigned int* _puiData32)
{
	unsigned int *piData32	= NULL;
	int iSize			= _iRows * _iCols;

	int iRet = allocMatrixOfUnsignedInteger32(_iVar, _iRows, _iCols, &piData32);
	if(iRet)
	{
		return 1;
	}

	memcpy(piData32, _puiData32, sizeof(unsigned int) * iSize);
	return 0;
}

int createMatrixOfInteger8(int _iVar, int _iRows, int _iCols, char* _pcData8)
{
	char *pcData8	= NULL;
	int iSize			= _iRows * _iCols;

	int iRet = allocMatrixOfInteger8(_iVar, _iRows, _iCols, &pcData8);
	if(iRet)
	{
		return 1;
	}

	memcpy(pcData8, _pcData8, sizeof(char) * iSize);
	return 0;
}

int createMatrixOfInteger16(int _iVar, int _iRows, int _iCols, short* _psData16)
{
	short *psData16	= NULL;
	int iSize			= _iRows * _iCols;

	int iRet = allocMatrixOfInteger16(_iVar, _iRows, _iCols, &psData16);
	if(iRet)
	{
		return 1;
	}

	memcpy(psData16, _psData16, sizeof(short) * iSize);
	return 0;
}

int createMatrixOfInteger32(int _iVar, int _iRows, int _iCols, int* _piData32)
{
	int *piData32	= NULL;
	int iSize			= _iRows * _iCols;

	int iRet = allocMatrixOfInteger32(_iVar, _iRows, _iCols, &piData32);
	if(iRet)
	{
		return 1;
	}

	memcpy(piData32, _piData32, sizeof(int) * iSize);
	return 0;
}

#ifdef __SCILAB_INT64__
int createMatrixOfInteger64(int _iVar, int _iRows, int _iCols, long long* _pllData64)
{
	long long  *pllData64	= NULL;
	int iSize							= _iRows * _iCols;

	int iRet = allocMatrixOfInteger64(_iVar, _iRows, _iCols, &pllData64);
	if(iRet)
	{
		return 1;
	}

	memcpy(pllData64, _pllData64, sizeof(long long) * iSize);
	return 0;
}
#endif

int allocMatrixOfInteger8(int _iVar, int _iRows, int _iCols, char** _pcData8)
{
	int *piAddr		= NULL;
	char *pcData8	= NULL;

	int iNewPos = Top - Rhs + _iVar;
	int iRet = getNewVarAddressFromPosition(iNewPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = allocCommonMatrixOfInteger(_iVar, piAddr, SCI_INT8, _iRows, _iCols, (void**)&pcData8);
	if(iRet)
	{
		return 1;
	}

	*_pcData8		= pcData8;
	return 0;
}

int allocMatrixOfInteger16(int _iVar, int _iRows, int _iCols, short** _psData16)
{
	int *piAddr			= NULL;
	short *psData16	= NULL;

	int iNewPos = Top - Rhs + _iVar;
	int iRet = getNewVarAddressFromPosition(iNewPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = allocCommonMatrixOfInteger(_iVar, piAddr, SCI_INT16, _iRows, _iCols, (void**)&psData16);
	if(iRet)
	{
		return 1;
	}

	*_psData16	= psData16;
	return 0;
}

int allocMatrixOfInteger32(int _iVar, int _iRows, int _iCols, int** _piData32)
{
	int *piAddr		= NULL;
	int *piData32	= NULL;

	int iNewPos = Top - Rhs + _iVar;
	int iRet = getNewVarAddressFromPosition(iNewPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = allocCommonMatrixOfInteger(_iVar, piAddr, SCI_INT32, _iRows, _iCols, (void**)&piData32);
	if(iRet)
	{
		return 1;
	}

	*_piData32	= piData32;
	return 0;
}

#ifdef __SCILAB_INT64__
int allocMatrixOfInteger64(int _iVar, int _iRows, int _iCols, long long** _piData64)
{
	return 1;
}
#endif

int allocMatrixOfUnsignedInteger8(int _iVar, int _iRows, int _iCols, unsigned char** _pucData8)
{
	int *piAddr		= NULL;
	unsigned char *pucData8	= NULL;

	int iNewPos = Top - Rhs + _iVar;
	int iRet = getNewVarAddressFromPosition(iNewPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = allocCommonMatrixOfInteger(_iVar, piAddr, SCI_UINT8, _iRows, _iCols, (void**)&pucData8);
	if(iRet)
	{
		return 1;
	}

	*_pucData8		= pucData8;
	return 0;
}

int allocMatrixOfUnsignedInteger16(int _iVar, int _iRows, int _iCols, unsigned short** _pusData16)
{
	int *piAddr			= NULL;
	unsigned short *pusData16	= NULL;

	int iNewPos = Top - Rhs + _iVar;
	int iRet = getNewVarAddressFromPosition(iNewPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = allocCommonMatrixOfInteger(_iVar, piAddr, SCI_UINT16, _iRows, _iCols, (void**)&pusData16);
	if(iRet)
	{
		return 1;
	}

	*_pusData16	= pusData16;
	return 0;
}

int allocMatrixOfUnsignedInteger32(int _iVar, int _iRows, int _iCols, unsigned int** _puiData32)
{
	int *piAddr		= NULL;
	unsigned int *puiData32	= NULL;

	int iNewPos = Top - Rhs + _iVar;
	int iRet = getNewVarAddressFromPosition(iNewPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = allocCommonMatrixOfInteger(_iVar, piAddr, SCI_INT32, _iRows, _iCols, (void**)&puiData32);
	if(iRet)
	{
		return 1;
	}

	*_puiData32	= puiData32;
	return 0;
}

#ifdef __SCILAB_INT64__
int allocMatrixOfUnsignedInteger64(int _iVar, int _iRows, int _iCols, unsigned long long** _pullData64)
{
	return 1;
}
#endif

int allocCommonMatrixOfInteger(int _iVar, int *_piAddress, int _iPrecision, int _iRows, int _iCols, void** pvData)
{
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);

	int iRate				= (sizeof(double) / (_iPrecision % 10));
	int iSize				= _iRows * _iCols;
	int iDouble			= iSize / iRate;
	int iMod				= (iSize % iRate) == 0 ? 0 : 1;
	int iTotalSize	= iDouble + iMod;


	fillCommonMatrixOfInteger(_piAddress, _iPrecision, _iRows, _iCols, pvData);
	updateInterSCI(_iVar, '$', iAddr, iAddr + 4);
	updateLstk(iNewPos, iAddr + 4, iTotalSize );
	return 0;
}

int fillCommonMatrixOfInteger(int* _piAddress, int _iPrecision, int _iRows, int _iCols, void** _pvData)
{
	_piAddress[0]		= sci_ints;
	_piAddress[1]		= Min(_iRows, _iRows * _iCols);
	_piAddress[2]		= Min(_iCols, _iRows * _iCols);
	_piAddress[3]		= _iPrecision;


	*_pvData		= (void*)(_piAddress + 4);

	return 0;
}

int createNamedMatrixOfUnsignedInteger8(char* _pstName, int _iRows, int _iCols, unsigned char* _pucData8)
{
	return createCommonNamedMatrixOfInteger(_pstName, SCI_UINT8, _iRows, _iCols, _pucData8);
}

int createNamedMatrixOfUnsignedInteger16(char* _pstName, int _iRows, int _iCols, unsigned short* _pusData16)
{
	return createCommonNamedMatrixOfInteger(_pstName, SCI_UINT16, _iRows, _iCols, _pusData16);
}

int createNamedMatrixOfUnsignedInteger32(char* _pstName, int _iRows, int _iCols, unsigned int* _puiData32)
{
	return createCommonNamedMatrixOfInteger(_pstName, SCI_UINT32, _iRows, _iCols, _puiData32);
}

int createNamedMatrixOfInteger8(char* _pstName, int _iRows, int _iCols, char* _pcData8)
{
	return createCommonNamedMatrixOfInteger(_pstName, SCI_INT8, _iRows, _iCols, _pcData8);
}

int createNamedMatrixOfInteger16(char* _pstName, int _iRows, int _iCols, short* _psData16)
{
	return createCommonNamedMatrixOfInteger(_pstName, SCI_INT16, _iRows, _iCols, _psData16);
}

int createNamedMatrixOfInteger32(char* _pstName, int _iRows, int _iCols, int* _piData32)
{
	return createCommonNamedMatrixOfInteger(_pstName, SCI_INT32, _iRows, _iCols, _piData32);
}

#ifdef __SCILAB_INT64__
int createNamedMatrixOfInteger64(char* _pstName, int _iRows, int _iCols, long long* _pllData64)
{
	return -1;
}
#endif

int createCommonNamedMatrixOfInteger(char* _pstName, int _iPrecision, int _iRows, int _iCols, void* _pvData)
{
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iRet					= 0;
	int *piAddr				= NULL;
	void *pvData			= NULL;

	int iRate				= (sizeof(double) / (_iPrecision % 10));
	int iSize				= _iRows * _iCols;
	int iDouble			= iSize / iRate;
	int iMod				= (iSize % iRate) == 0 ? 0 : 1;
	int iTotalSize	= iDouble + iMod;

	C2F(str2name)(_pstName, iVarID, (int)strlen(_pstName));
  Top = Top + Nbvars + 1;

	iRet = getNewVarAddressFromPosition(Top, &piAddr);

	//write matrix information
	fillCommonMatrixOfInteger(piAddr, _iPrecision, _iRows, _iCols, &pvData);
	//copy data in stack
	memcpy(pvData, _pvData, (_iPrecision % 10) * iSize);

	//update "variable index"
	updateLstk(Top, *Lstk(Top) + 4, iTotalSize);

	Rhs = 0;
	//Add name in stack reference list
	createNamedVariable(iVarID);

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return 0;
}

int getNamedMatrixOfIntegerPrecision(char* _pstName, int* _piPrecision)
{
	int iRet					= 0;
	int* piAddr				= NULL;

	iRet = getVarAddressFromName(_pstName, &piAddr);
	if(iRet)
	{
		return 1;
	}

	//check variable type
	if(piAddr[0] != sci_ints)
	{
		return 1;
	}

	*_piPrecision = piAddr[3];
	return 0;
}

int readNamedMatrixOfUnsignedInteger8(char* _pstName, int* _piRows, int* _piCols, unsigned char* _pucData8)
{
	return readCommonNamedMatrixOfInteger(_pstName, SCI_UINT8, _piRows, _piCols, _pucData8);
}

int readNamedMatrixOfUnsignedInteger16(char* _pstName, int* _piRows, int* _piCols, unsigned short* _pusData16)
{
	return readCommonNamedMatrixOfInteger(_pstName, SCI_UINT16, _piRows, _piCols, _pusData16);
}

int readNamedMatrixOfUnsignedInteger32(char* _pstName, int* _piRows, int* _piCols, unsigned int* _puiData32)
{
	return readCommonNamedMatrixOfInteger(_pstName, SCI_UINT32, _piRows, _piCols, _puiData32);
}

int readNamedMatrixOfInteger8(char* _pstName, int* _piRows, int* _piCols, char* _pcData8)
{
	return readCommonNamedMatrixOfInteger(_pstName, SCI_INT8, _piRows, _piCols, _pcData8);
}

int readNamedMatrixOfInteger16(char* _pstName, int* _piRows, int* _piCols, short* _psData16)
{
	return readCommonNamedMatrixOfInteger(_pstName, SCI_INT16, _piRows, _piCols, _psData16);
}

int readNamedMatrixOfInteger32(char* _pstName, int* _piRows, int* _piCols, int* _piData32)
{
	return readCommonNamedMatrixOfInteger(_pstName, SCI_INT32, _piRows, _piCols, _piData32);
}

#ifdef __SCILAB_INT64__
int readNamedMatrixOfInteger64(char* _pstName, int* _piRows, int* _piCols, long long* _pllData64)
{
	return 1;
}
#endif

int readCommonNamedMatrixOfInteger(char* _pstName, int _iPrecision, int* _piRows, int* _piCols, void* _pvData)
{
	int iRet					= 0;
	int* piAddr				= NULL;
	int iSize					= 0;
	void* pvData			= NULL;

	iRet = getVarAddressFromName(_pstName, &piAddr);
	if(iRet)
	{
		return 1;
	}

	getCommonMatrixOfInteger(piAddr, _iPrecision, _piRows, _piCols, &pvData);

	iSize = *_piRows * *_piCols;

	if(pvData == NULL || _pvData == NULL)
	{
		return 0;
	}

	memcpy(_pvData, pvData, (_iPrecision % 10) * iSize);
	return 0;
}
/*--------------------------------------------------------------------------*/
