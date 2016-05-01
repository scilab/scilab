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

#ifndef __URLENCODER_HXX__
#define __URLENCODER_HXX__

#include <sstream>
#include <set>
#include <string>

namespace coverage
{

class URLEncoder
{
    struct __Pair
    {
        const wchar_t c;
        const std::wstring rep;

        __Pair(const wchar_t _c, const std::wstring _rep) : c(_c), rep(_rep) { }
        __Pair(const wchar_t _c) : c(_c) { }

        inline bool operator<(const __Pair & R) const
        {
            return c < R.c;
        }
    };
    static std::set<__Pair> pairs;

public:

    inline static void replace(std::wostringstream & out, const wchar_t c)
    {
        auto i = pairs.find(__Pair(c));
        if (i == pairs.end())
        {
            out << c;
        }
        else
        {
            out << i->rep;
        }
    }

private:

    inline static std::set<__Pair> init()
    {
        std::set<__Pair> pairs;
        pairs.emplace(L'%', L"%25");
        pairs.emplace(L'!', L"%21");
        pairs.emplace(L'\"', L"%22");
        pairs.emplace(L'#', L"%23");
        pairs.emplace(L'$', L"%24");
        pairs.emplace(L'&', L"%26");
        pairs.emplace(L'\'', L"%27");
        pairs.emplace(L'(', L"%28");
        pairs.emplace(L')', L"%29");
        pairs.emplace(L'*', L"%2A");
        pairs.emplace(L'+', L"%2B");
        pairs.emplace(L',', L"%2C");
        pairs.emplace(L'/', L"%2F");
        pairs.emplace(L':', L"%3A");
        pairs.emplace(L';', L"%3B");
        pairs.emplace(L'=', L"%3D");
        pairs.emplace(L'?', L"%3F");
        pairs.emplace(L'@', L"%40");
        pairs.emplace(L'[', L"%5B");
        pairs.emplace(L']', L"%5D");
        pairs.emplace(L' ', L"%20");
        pairs.emplace(L'-', L"%2D");
        pairs.emplace(L'<', L"%3C");
        pairs.emplace(L'>', L"%3E");
        pairs.emplace(L'{', L"%7B");
        pairs.emplace(L'}', L"%7D");
        pairs.emplace(L'_', L"%5F");
        pairs.emplace(L'~', L"%7E");

        return pairs;
    }
};

} // namespace coverage

#endif // __URLENCODER_HXX__
