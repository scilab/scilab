/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
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
