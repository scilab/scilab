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

#include "formatmode.h"
#include "configvariable.hxx"

void setFormatSize(int _iFormatSize)
{
    ConfigVariable::setFormatSize(_iFormatSize);
}

int getFormatSize(void)
{
    return ConfigVariable::getFormatSize();
}

void setFormatMode(int _iFormatMode)
{
    ConfigVariable::setFormatSize(_iFormatMode);
}

int getFormatMode(void)
{
    return ConfigVariable::getFormatMode();
}

void setConsoleWidth(int _iConsoleWidth)
{
    ConfigVariable::setConsoleWidth(_iConsoleWidth);
}

int getConsoleWidth(void)
{
    return ConfigVariable::getConsoleWidth();
}

void setConsoleLines(int _iConsoleLines)
{
    ConfigVariable::setConsoleLines(_iConsoleLines);
}

int getConsoleLines(void)
{
    return ConfigVariable::getConsoleLines();
}
