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
#include "api_pointer.h"
#include "localization.h"

#include "MALLOC.h"
#include "stack-c.h"

StrErr getPointer(void* _pvCtx, int* _piAddress, void** _pvPtr)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iType = 0;
	int *piTmp = NULL;

	if(	_piAddress == NULL)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getPointer");
		return strErr;
	}

	strErr = getVarType(_pvCtx, _piAddress, &iType);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_POINTER, _("%s: Unable to get argument #%d"), "getPointer", getRhsFromAddress(_pvCtx, _piAddress));
		return strErr;
	}
	
	if(iType != sci_pointer)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), "getPointer", _("pointer"));
		return strErr;
	}
	
	piTmp = (_piAddress + 4);

	*_pvPtr = (void*)*(long long*)piTmp;
	return strErr;
}

StrErr fillPointer(void* _pvCtx, int *_piAddress, void** _pvPtr)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	if(_piAddress == NULL)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "fillPointer");
		return strErr;
	}

	_piAddress[0] = sci_pointer;
	_piAddress[1] = 1;
	_piAddress[2] = 1;
	_piAddress[3] = 0;

	*_pvPtr = _piAddress + 4;

	return strErr;
}

StrErr allocPointer(void* _pvCtx, int _iVar, void** _pvPtr)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);
	int* piAddr			= NULL;
	void* pvPtr			= NULL;

	int iMemSize = 2;
	int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(iAddr));
	if (iMemSize > iFreeSpace)
	{
		addStackSizeError(&strErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
		return strErr;
	}

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	strErr = fillPointer(_pvCtx, piAddr, &pvPtr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_ALLOC_POINTER, _("%s: Unable to create variable in Scilab memory"), "allocPointer");;
		return strErr;
	}

	*_pvPtr = pvPtr;
	updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + 4));
	updateLstk(iNewPos, sadr(iadr(iAddr) + 4), 2);

	return strErr;
}

StrErr createPointer(void* _pvCtx, int _iVar, void* _pvPtr)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	void* pvPtr		= NULL;

	strErr = allocPointer(_pvCtx, _iVar, &pvPtr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_POINTER, _("%s: Unable to create variable in Scilab memory"), "createPointer");
		return strErr;
	}

	((long long*)pvPtr)[0] = (long long) ((unsigned long int) _pvPtr);

	return strErr;
}
