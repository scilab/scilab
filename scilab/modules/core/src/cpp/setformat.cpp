/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 * Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "configvariable.hxx"

extern "C"
{
#include "setformat.h"
}

/*--------------------------------------------------------------------------*/
void setformat(const char * format, int width)
{
    ConfigVariable::setFormatMode(*format == 'e' ? 0 : 1);
    ConfigVariable::setFormatSize(width);
}
/*--------------------------------------------------------------------------*/
const char* getformat()
{
    return ConfigVariable::getFormatMode() ? "v" : "e";
}
/*--------------------------------------------------------------------------*/
int getformatwidth()
{
    return ConfigVariable::getFormatSize();
}
