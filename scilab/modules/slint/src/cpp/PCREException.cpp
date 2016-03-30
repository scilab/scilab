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
