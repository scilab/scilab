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

#include <algorithm>
#include "configvariable.hxx"
extern "C"
{
#include "lasterror.h"
}


int getLastErrorNumber(void)
{
    return ConfigVariable::getLastErrorNumber();
}
/*--------------------------------------------------------------------------*/
char *getLastErrorMessage(void)
{
    return const_cast<char*>(ConfigVariable::getLastErrorMessage().c_str());
}
/*--------------------------------------------------------------------------*/
int getLastErrorLine(void)
{
    return ConfigVariable::getLastErrorLine();
}
/*--------------------------------------------------------------------------*/
char *getLastErrorFunction(void)
{
    return const_cast<char*>(ConfigVariable::getLastErrorFunction().c_str());
}
/*--------------------------------------------------------------------------*/
void setLastErrorNumber(int _iError)
{
    ConfigVariable::setLastErrorNumber(_iError);
    ConfigVariable::setError();
}
/*--------------------------------------------------------------------------*/
void setLastErrorMessage(const char* _pstError)
{
    if (_pstError == NULL)
    {
        std::string stError("");
        ConfigVariable::setLastErrorFunction(stError);
    }
    else
    {
        std::string stError(_pstError);
        ConfigVariable::setLastErrorMessage(stError);
    }
}
/*--------------------------------------------------------------------------*/
void setLastErrorLine(int _iLine)
{
    ConfigVariable::setLastErrorLine(_iLine);
}
/*--------------------------------------------------------------------------*/
void setLastErrorFunction(const char* _pstFunction)
{
    if (_pstFunction == NULL)
    {
        std::string stError("");
        ConfigVariable::setLastErrorFunction(stError);
    }
    else
    {
        std::string stError(_pstFunction);
        ConfigVariable::setLastErrorFunction(stError);
    }
}
/*--------------------------------------------------------------------------*/
void setLastError(int _iError, const char* _pstError, int _iLine, const char* _pstFunction)
{
    setLastErrorNumber(_iError);
    setLastErrorMessage(_pstError);
    setLastErrorLine(_iLine);
    setLastErrorFunction(_pstFunction);
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
