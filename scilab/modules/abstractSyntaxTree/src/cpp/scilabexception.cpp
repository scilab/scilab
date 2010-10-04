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
#include "lasterror.h"

namespace ast
{
    ScilabException::ScilabException(std::wstring _wstErrorMesssage)
    {
        m_wstErrorMessage = _wstErrorMesssage;
    }

    void ScilabException::SetErrorMessage(std::wstring _wstErrorMesssage)
    {
        m_wstErrorMessage = _wstErrorMesssage;
    }

    std::wstring ScilabException::GetErrorMessage(void)
    {
        return m_wstErrorMessage;
    }


    ScilabException::ScilabException(const Location& _ErrorLocation)
    {
        m_ErrorLocation     = _ErrorLocation;
    }

    ScilabException::ScilabException(std::wstring _wstErrorMesssage, int _iErrorNumber, const Location& _ErrorLocation)
    {
        m_wstErrorMessage   = _wstErrorMesssage;
        m_iErrorNumber      = _iErrorNumber;
        m_ErrorLocation     = _ErrorLocation;
    }

    ScilabException::ScilabException()
    {
        m_wstErrorMessage   = L"";
        m_iErrorNumber      = 0;
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

}