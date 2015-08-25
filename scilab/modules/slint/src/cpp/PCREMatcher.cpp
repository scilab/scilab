/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
        extra = nullptr;
    }
    else
    {
        const char * error = nullptr;
        int errorOffset = -1;
        re = pcre_compile(scilab::UTF8::toUTF8(pattern).c_str(), PCRE_UTF8, &error, &errorOffset, nullptr);
        if (re)
        {
            extra = pcre_study(re, 0, &error);
        }
        else
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
    if (extra)
    {
        pcre_free_study(extra);
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
        int result = pcre_exec(re, extra, _str, len, 0, 0, resVect, sizeof(resVect) / sizeof(int));
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
