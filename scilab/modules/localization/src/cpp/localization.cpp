/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include "localization.hxx"

std::wstring gettextW(const char* s)
{
    wchar_t* pwst = to_wide_string(s);
    if (pwst)
    {
        std::wstring ret(pwst);
        FREE(pwst);
        return ret;
    }

    return std::wstring(L"");
}
