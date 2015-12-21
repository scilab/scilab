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

#include "PCREException.hxx"
#include "UTF8.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "charEncoding.h"
}

namespace slint
{

PCREException::PCREException(const std::wstring & pattern, const std::string & error, const int offset)
{
    msg = std::string("Error in regex pattern: ") + error + "\n" + scilab::UTF8::toUTF8(pattern) + "\n" + std::string(offset, ' ') + "^";
}


#ifdef _MSC_VER
const char * PCREException::what() const
#else
const char * PCREException::what() const noexcept
#endif
{
    return msg.c_str();
}

} // namespace slint
