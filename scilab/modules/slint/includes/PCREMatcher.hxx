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

#ifndef __PCRE_MATCHER_HXX__
#define __PCRE_MATCHER_HXX__

#include <string>
#include <pcre.h>

#include "PCREException.hxx"

namespace slint
{

class PCREMatcher
{

    const std::wstring pattern;
    pcre * re;
    pcre_extra * extra;

public:

    PCREMatcher(const std::wstring & _pattern);
    ~PCREMatcher();

    bool match(const std::wstring & str, const bool full = true) const;
    bool match(const wchar_t * str, const bool full = true) const;
    bool match(const wchar_t * str, const unsigned int len, const bool full = true) const;
    const std::wstring & getPattern() const;
};

} // namespace slint

#endif // __PCRE_MATCHER_HXX__
