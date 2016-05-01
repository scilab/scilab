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

FileException::FileException(const std::wstring & filename, const std::string & error)
{
    msg = std::string("Error with file ") + scilab::UTF8::toUTF8(filename) + ":\n" + error;
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
