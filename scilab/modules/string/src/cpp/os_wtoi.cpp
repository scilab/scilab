/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <sstream>
#include <string>
#include <iostream>

extern "C"
{
#include "os_wtoi.h"
}

int os_wtoi(const wchar_t *_pwcsSource)
{
    std::wstring wstr(_pwcsSource);
    std::wistringstream wstrm(wstr);
    int num;
    wstrm >> num;
    return num;
}


