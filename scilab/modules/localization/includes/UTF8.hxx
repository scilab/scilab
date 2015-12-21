/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string>

#include "dynlib_localization.h"

namespace scilab
{

class UTF8
{

public:

    LOCALIZATION_IMPEXP static std::string toUTF8(const std::wstring & wstr);
    LOCALIZATION_IMPEXP static std::wstring toWide(const std::string & str);

private:

    static int ReadUTF8Character(const char * str, int * nBytes);
    static bool IsValidUTF8(const char*  pStText);

};

}
