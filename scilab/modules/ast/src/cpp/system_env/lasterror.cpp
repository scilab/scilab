/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
