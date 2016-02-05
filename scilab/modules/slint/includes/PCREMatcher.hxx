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
