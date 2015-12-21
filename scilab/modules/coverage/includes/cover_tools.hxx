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
