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

#include "function.hxx"
#include "overload.hxx"

extern "C"
{
#include <stdlib.h>
#include "api_oldstack.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

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
		std::cout << "pStr == NULL" << std::endl;
		return 0;
	}

	if(pStr->m_pIn == NULL)
	{
		std::cout << "pStr->m_pin == NULL" << std::endl;
		return 0;
	}

	return (int)pStr->m_pIn->size();
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
    GatewayStruct *pStr = (GatewayStruct*)_piKey;
    int iRhs            = api_Rhs(_piKey);

    if(iRhs > _iMax || iRhs < _iMin)
    {
        if (_iMin == _iMax)
        {/* No optional argument */
            ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), pStr->m_pstName, _iMax);
        }
        else
        {
            ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), pStr->m_pstName, _iMin, _iMax);
        }
        return 0;
    }
    return 1;
}

int api_CheckLhs(int _iMin, int _iMax, int* _piKey)
{
    GatewayStruct *pStr = (GatewayStruct*)_piKey;
	int iLhs = api_Lhs(_piKey);

	if(iLhs > _iMax || iLhs < _iMin)
	{
        if (_iMin == _iMax)
        {/* No optional argument */
            ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d expected.\n"), pStr->m_pstName, _iMax);
        }
        else
        {
            ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d to %d expected.\n"), pStr->m_pstName, _iMin, _iMax);
        }
        return 0;
	}
	return 1;
}

int* api_LhsVar(int _iVal, void* _pvCtx)
{
	//do nothing but don't crash
	if(_pvCtx == NULL)
	{
		return &api_fake_int;
	}

	GatewayStruct* pStr = (GatewayStruct*)_pvCtx;

	//do nothing but don't crash
	if(_iVal > *pStr->m_piRetCount)
	{
		return &api_fake_int;
	}

	int* pVal = &(pStr->m_pOutOrder[_iVal - 1]);
	return pVal;
}

void api_OverLoad(int _iVal, int* _piKey)
{
    GatewayStruct* pStr = (GatewayStruct*)_piKey;
    Function::ReturnValue callResult;
    typed_list tlReturnedValues;

    std::wstring wsFunName;

    if(_iVal == 0)
    {
        wsFunName = std::wstring(L"%_") + std::wstring(pStr->m_pstName);
    }
    else
    {
        wsFunName = std::wstring(L"%") + (*pStr->m_pIn)[_iVal - 1]->getShortTypeStr() + L"_" + std::wstring(pStr->m_pstName);
    }

    callResult = Overload::call(wsFunName, *(pStr->m_pIn), *(pStr->m_piRetCount), tlReturnedValues, pStr->m_pVisitor);
    if (callResult == Function::OK)
    {
        int i = 0;
        typed_list::iterator it;
        for (it = tlReturnedValues.begin() ; it != tlReturnedValues.end() ; ++it, ++i)
        {
            (pStr->m_pOut)[i] = *it;
            pStr->m_pOutOrder[i] = pStr->m_pIn->size() + i + 1;
        }
    }
}
