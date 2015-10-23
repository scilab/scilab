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

#include "FileException.hxx"
#include "UTF8.hxx"

extern "C"
{
#include "charEncoding.h"
}

namespace slint
{

FileException::FileException(const std::wstring & filename, const std::wstring & error)
{
    msg = std::string("Error with file ") + scilab::UTF8::toUTF8(filename) + ":\n" + scilab::UTF8::toUTF8(error);
}

#ifdef _MSC_VER
const char * FileException::what() const
#else
const char * FileException::what() const noexcept
#endif
{
    return msg.c_str();
}

} // namespace slint
