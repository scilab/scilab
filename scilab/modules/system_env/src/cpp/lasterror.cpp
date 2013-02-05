/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdlib.h>

#include "lasterror.h"

#include "configvariable.hxx"

int getLastErrorNumber(void)
{
    return ConfigVariable::getLastErrorNumber();
}
/*--------------------------------------------------------------------------*/
wchar_t *getLastErrorMessage(void)
{
    return const_cast<wchar_t*>(ConfigVariable::getLastErrorMessage().c_str());
}
/*--------------------------------------------------------------------------*/
int getLastErrorLine(void)
{
    return ConfigVariable::getLastErrorLine();
}
/*--------------------------------------------------------------------------*/
wchar_t *getLastErrorFunction(void)
{
    return const_cast<wchar_t*>(ConfigVariable::getLastErrorFunction().c_str());
}
/*--------------------------------------------------------------------------*/
void setLastErrorNumber(int _iError)
{
    ConfigVariable::setLastErrorNumber(_iError);
    ConfigVariable::setError();
}
/*--------------------------------------------------------------------------*/
void setLastErrorMessage(const wchar_t* _pwstError)
{
    if (_pwstError == NULL)
    {
        std::wstring wstError(L"");
        ConfigVariable::setLastErrorFunction(wstError);
    }
    else
    {
        std::wstring wstError(_pwstError);
        ConfigVariable::setLastErrorMessage(wstError);
    }
}
/*--------------------------------------------------------------------------*/
void setLastErrorLine(int _iLine)
{
    ConfigVariable::setLastErrorLine(_iLine);
}
/*--------------------------------------------------------------------------*/
void setLastErrorFunction(const wchar_t* _pwstFunction)
{
    if (_pwstFunction == NULL)
    {
        std::wstring wstError(L"");
        ConfigVariable::setLastErrorFunction(wstError);
    }
    else
    {
        std::wstring wstError(_pwstFunction);
        ConfigVariable::setLastErrorFunction(wstError);
    }
}
/*--------------------------------------------------------------------------*/
void setLastError(int _iError, const wchar_t* _pwstError, int _iLine, const wchar_t* _pwstFunction)
{
    setLastErrorNumber(_iError);
    setLastErrorMessage(_pwstError);
    setLastErrorLine(_iLine);
    setLastErrorFunction(_pwstFunction);
}

/*--------------------------------------------------------------------------*/
void SetError()
{
    ConfigVariable::setError();
}
/*--------------------------------------------------------------------------*/
int isError()
{
    return ConfigVariable::isError() ? 1 : 0;
}
/*--------------------------------------------------------------------------*/
void resetError()
{
    ConfigVariable::resetError();
}
