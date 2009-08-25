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

#include "MALLOC.h"
#include "stack-c.h"

int getPointer(int* _piAddress, void** _pvPtr)
{
	int *piTmp = NULL;

	if(	_piAddress == NULL || getVarType(_piAddress) != sci_lufact_pointer)
	{
		return 1;
	}

	piTmp = (_piAddress + 4);

	*_pvPtr = (void*)*(long long*)piTmp;
	return 0;
}

int fillPointer(int *_piAddress, void** _pvPtr)
{
	if(_piAddress == NULL)
	{
		return 1;
	}

	_piAddress[0] = sci_lufact_pointer;
	_piAddress[1] = 1;
	_piAddress[2] = 1;
	_piAddress[3] = 0;

	*_pvPtr = _piAddress + 4;

	return 0;
}

int allocPointer(int _iVar, void** _pvPtr)
{
	int iRet				= 0;
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);
	int* piAddr			= NULL;
	void* pvPtr			= NULL;

	iRet = getNewVarAddressFromPosition(iNewPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = fillPointer(piAddr, &pvPtr);
	if(iRet)
	{
		return 1;
	}

	*_pvPtr = pvPtr;
	iRet = updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + 4));
	if(iRet)
	{
		return 1;
	}

	iRet = updateLstk(iNewPos, sadr(iadr(iAddr) + 4), 2);
	if(iRet)
	{
		return 1;
	}

	return 0;
}

int createPointer(int _iVar, void* _pvPtr)
{
	int iRet			= 0;
	void* pvPtr		= NULL;

	iRet = allocPointer(_iVar, &pvPtr);
	if(iRet)
	{
		return 1;
	}

	((long long*)pvPtr)[0] = (long long) ((unsigned long int) _pvPtr);

	return 0;
}
