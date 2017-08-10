/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
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

#include "PCREMatcher.hxx"
#include "UTF8.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "charEncoding.h"
}

namespace slint
{

PCREMatcher::PCREMatcher(const std::wstring & _pattern) : pattern(_pattern)
{
    if (_pattern.empty())
    {
        re = nullptr;
    }
    else
    {
        const char * error = nullptr;
        int errorOffset = -1;
        re = pcre_compile(scilab::UTF8::toUTF8(pattern).c_str(), PCRE_UTF8, &error, &errorOffset, nullptr);
        if (!re)
        {
            if (error)
            {
                throw PCREException(pattern, error, errorOffset);
            }
            else
            {
                throw PCREException(pattern, "No error message from PCRE", 0);
            }
        }
    }
}

PCREMatcher::~PCREMatcher()
{
    if (re)
    {
        pcre_free(re);
    }
}

bool PCREMatcher::match(const std::wstring & str, const bool full) const
{
    if (!pattern.empty())
    {
        return match(str.c_str(), str.size(), full);
    }
    return true;
}

bool PCREMatcher::match(const wchar_t * str, const bool full) const
{
    return match(str, wcslen(str), full);
}

bool PCREMatcher::match(const wchar_t * str, const unsigned int len, const bool full) const
{
    if (!pattern.empty())
    {
        int resVect[3];
        char * _str = wide_string_to_UTF8(str);
        int result = pcre_exec(re, nullptr, _str, len, 0, 0, resVect, sizeof(resVect) / sizeof(int));
        FREE(_str);
        if (full)
        {
            if (result == 1 && resVect[0] == 0 && resVect[1] == len)
            {
                return true;
            }
        }
        else
        {
            return result == 1;
        }

        return false;
    }
    return true;
}

const std::wstring & PCREMatcher::getPattern() const
{
    return pattern;
}

} // namespace slint
