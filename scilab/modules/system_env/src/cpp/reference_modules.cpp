/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/


#include "configvariable.hxx"

extern "C"
{
#include "reference_modules.h"
}

int checkReferenceModule(const wchar_t* _module)
{
    return (ConfigVariable::checkReferenceModule(_module) ? 1 : 0);
}

void addReferenceModule(const wchar_t* _module)
{
    ConfigVariable::addReferenceModule(_module);
}

void removeReferenceModule(const wchar_t* _module)
{
    ConfigVariable::removeReferenceModule(_module);
}
