/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "config/cnes/CNESException.hxx"
#include "UTF8.hxx"

namespace slint
{

namespace CNES
{
CNESException::CNESException(const std::wstring & error)
{
    msg = scilab::UTF8::toUTF8(error);
}

#ifdef _MSC_VER
const char * CNESException::what() const
#else
const char * CNESException::what() const noexcept
#endif
{
    return msg.c_str();
}

} // namespace CNES

} // namespace slint
