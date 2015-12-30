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

#ifndef AST_SCILABEXCEPTION_HXX
#define AST_SCILABEXCEPTION_HXX

#include <string>
#include <stdexcept>
#include "location.hxx"

extern "C"
{
#include "lasterror.h"
#include "sci_malloc.h"
#include "localization.h"
}

//disable warnings about exports STL items
#ifdef _MSC_VER
#pragma warning (disable : 4251)
#endif

namespace ast
{

enum ExceptionType
{
    TYPE_ERROR,
    TYPE_EXCEPTION
};

class ScilabException : public std::exception
{
public :
    ScilabException() {}

    ScilabException(const std::string& _stErrorMesssage)
    {
        m_type = TYPE_EXCEPTION;
        createScilabException(_stErrorMesssage, 999, Location());
    }

    ScilabException(const std::string& _stErrorMesssage, int _iErrorNumber, const Location& _ErrorLocation)
    {
        m_type = TYPE_EXCEPTION;
        createScilabException(_stErrorMesssage, _iErrorNumber, _ErrorLocation);
    }

    virtual ~ScilabException() throw() {};

    void SetErrorMessage(const std::string& _stErrorMesssage)
    {
        m_stErrorMessage = _stErrorMesssage;
    }

    std::string GetErrorMessage(void) const
    {
        return m_stErrorMessage;
    }

    void SetErrorNumber(int _iErrorNumber)
    {
        m_iErrorNumber = _iErrorNumber;
    }

    int GetErrorNumber(void) const
    {
        return m_iErrorNumber;
    }

    void SetErrorLocation(const Location& _ErrorLocation)
    {
        m_ErrorLocation = _ErrorLocation;
    }

    const Location& GetErrorLocation(void) const
    {
        return m_ErrorLocation;
    }

    void SetErrorType(ExceptionType _type)
    {
        m_type = _type;
    }

    ExceptionType GetErrorType(void)
    {
        return m_type;
    }

protected :
    std::string m_stErrorMessage;
    int m_iErrorNumber;
    Location m_ErrorLocation;
    ExceptionType m_type;

protected :
    void createScilabException(const std::string& _stErrorMessage, int _iErrorNumber, const Location& _ErrorLocation)
    {
        m_stErrorMessage = _stErrorMessage;
        m_iErrorNumber = _iErrorNumber;
        m_ErrorLocation = _ErrorLocation;
    }
};

class InternalError : public ScilabException
{
public :
    InternalError(const std::string& _stErrorMesssage) : ScilabException(_stErrorMesssage)
    {
        m_type = TYPE_ERROR;
        setLastError(999, _stErrorMesssage.c_str(), 0, NULL);
    }

    InternalError(const std::string& _stErrorMesssage, int _iErrorNumber, const Location& _ErrorLocation) : ScilabException(_stErrorMesssage, _iErrorNumber, _ErrorLocation)
    {
        m_type = TYPE_ERROR;
        setLastError(_iErrorNumber, _stErrorMesssage.c_str(), _ErrorLocation.first_line, NULL);
    }
};


class InternalAbort : public ScilabException
{
public:
    InternalAbort() {}
};

class RecursionException : public ScilabException
{
public:
    RecursionException() {}
};
}
#endif // !AST_SCILABEXCEPTION_HXX
