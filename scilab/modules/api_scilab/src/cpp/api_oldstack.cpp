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

#include <stdlib.h>
#include "api_oldstack.h"
#include "function.hxx"
#include "sciprint.h"

using namespace types;

int api_Top(int* _piKey)
{
	return api_Rhs(_piKey);
}

int api_Rhs(int* _piKey)
{
	GatewayStruct *pStr =  (GatewayStruct*)_piKey;

	if(pStr == NULL)
	{
		return 0;
	}

	if(pStr->m_pin == NULL)
	{
		return 0;
	}

	return (int)pStr->m_pin->size();
}

int api_Lhs(int* _piKey)
{
	GatewayStruct *pStr =  (GatewayStruct*)_piKey;

	if(pStr == NULL)
	{
		return 0;
	}

	if(pStr->m_piRetCount == NULL)
	{
		return 0;
	}

	return abs(*pStr->m_piRetCount);
}

int api_CheckRhs(int _iMin, int _iMax, int* _piKey)
{
	int iRhs = api_Rhs(_piKey);

	if(iRhs > _iMax || iRhs < _iMin)
	{
		return 0;
	}

	return 1;
}

int api_CheckLhs(int _iMin, int _iMax, int* _piKey)
{
	int iLhs = api_Lhs(_piKey);

	if(iLhs > _iMax || iLhs < _iMin)
	{
		return 0;
	}

	return 1;
}

int* api_LhsVar(int _iVal)
{
	return &api_fake_int;
}

void api_OverLoad(int _iVal)
{
	sciprint((char*)"call overload %d\n", _iVal);
}
