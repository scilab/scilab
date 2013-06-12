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

#include "scilabexception.hxx"

extern "C"
{
#include "lasterror.h"
#include "charEncoding.h"
#include "MALLOC.h"
}

namespace ast
{

void ScilabException::createScilabException(std::wstring _wstErrorMesssage, int _iErrorNumber, const Location& _ErrorLocation)
{
    m_wstErrorMessage   = _wstErrorMesssage;
    m_iErrorNumber      = _iErrorNumber;
    m_ErrorLocation     = _ErrorLocation;
}

ScilabException::ScilabException(std::wstring _wstErrorMesssage)
{
    setLastError(999, _wstErrorMesssage.c_str(), 0, NULL);
    createScilabException(_wstErrorMesssage);
}

ScilabException::ScilabException(std::string _stErrorMesssage)
{

    wchar_t* pwst = to_wide_string(_stErrorMesssage.c_str());
    setLastError(999, pwst, 0, NULL);
    createScilabException(pwst);
    FREE(pwst);
}

ScilabException::ScilabException(const Location& _ErrorLocation)
{
    createScilabException(L"", 0, _ErrorLocation);
}

ScilabException::ScilabException(std::wstring _wstErrorMesssage, int _iErrorNumber, const Location& _ErrorLocation)
{
    setLastError(_iErrorNumber, _wstErrorMesssage.c_str(), _ErrorLocation.first_line, NULL);
    createScilabException(_wstErrorMesssage, _iErrorNumber, _ErrorLocation);
}

ScilabException::ScilabException()
{
    createScilabException();
}

void ScilabException::SetErrorMessage(std::wstring _wstErrorMesssage)
{
    m_wstErrorMessage = _wstErrorMesssage;
}

std::wstring ScilabException::GetErrorMessage(void)
{
    return m_wstErrorMessage;
}

void ScilabException::SetErrorNumber(int _iErrorNumber)
{
    m_iErrorNumber = _iErrorNumber;
}

int ScilabException::GetErrorNumber(void)
{
    return m_iErrorNumber;
}

void ScilabException::SetErrorLocation(const Location& _ErrorLocation)
{
    m_ErrorLocation = _ErrorLocation;
}

Location& ScilabException::GetErrorLocation(void)
{
    return m_ErrorLocation;
}

ScilabMessage::ScilabMessage(std::wstring _wstErrorMesssage)
{
    //setLastError(999, _wstErrorMesssage.c_str(), 0, NULL);
    createScilabException(_wstErrorMesssage);
}

ScilabMessage::ScilabMessage(std::string _stErrorMesssage)
{

    wchar_t* pwst = to_wide_string(_stErrorMesssage.c_str());
    //setLastError(999, pwst, 0, NULL);
    createScilabException(pwst);
    FREE(pwst);
}

ScilabMessage::ScilabMessage(std::wstring _wstErrorMesssage, int _iErrorNumber, const Location& _ErrorLocation)
{
    //setLastError(_iErrorNumber, _wstErrorMesssage.c_str(), 0, NULL);
    createScilabException(_wstErrorMesssage, _iErrorNumber, _ErrorLocation);
}

}
