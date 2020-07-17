/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2020 - ESI Group - Clement DAVID
 *  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
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

#include <sstream>
#include <string>
#include <iostream>

extern "C"
{
#include "os_wtoi.h"
}

// similar API to std::stoi but does not throw
// Adapted from http://tinodidriksen.com/uploads/code/cpp/speed-string-to-int.cpp
int os_wtoi(const wchar_t* str, std::size_t* pos)
{
    const wchar_t* p = str;
    int x = 0;
    bool neg = false;
    if (*p == '-')
    {
        neg = true;
        ++p;
    }
    while (*p >= '0' && *p <= '9')
    {
        x = (x * 10) + (*p - '0');
        ++p;
    }
    if (neg)
    {
        x = -x;
    }

    if (pos)
    {
        *pos = p - str;
    }
    return x;
}


