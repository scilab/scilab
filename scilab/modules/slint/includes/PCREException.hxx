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

#ifndef __PCRE_EXCEPTION_HXX__
#define __PCRE_EXCEPTION_HXX__

#include <exception>
#include <string>

namespace slint
{

class PCREException : public std::exception
{
    std::string msg;

public:

    PCREException(const std::wstring & pattern, const std::string & error, const int offset);

#ifdef _MSC_VER
    virtual const char * what() const;
#else
    virtual const char * what() const noexcept;
#endif

};

} // namespace slint

#endif // __PCRE_EXCEPTION_HXX__
