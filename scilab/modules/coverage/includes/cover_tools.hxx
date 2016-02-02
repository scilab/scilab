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

#ifndef __COVER_TOOLS_HXX__
#define __COVER_TOOLS_HXX__

#include <string>
#include "allexp.hxx"

namespace coverage
{
namespace tools
{
template<typename T>
inline static std::wstring getPlural(const T x)
{
    return x > 1 ? L"s" : L"";
}

inline static std::wstring getUnderline(const std::wstring & s)
{
    return std::wstring(s.length(), L'-');
}

inline static bool isBlockExp(const ast::Exp * e)
{
    return e && (e->isIfExp() || e->isWhileExp() || e->isForExp() || e->isTryCatchExp());
}

inline static std::wstring underline(const std::wstring & str)
{
    return str + L"\n" + getUnderline(str) + L"\n";
}

} // namespace tools

} // namespace coverage

#endif // __COVER_TOOLS_HXX__
