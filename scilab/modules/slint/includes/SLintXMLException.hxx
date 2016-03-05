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

#ifndef __SLINT_XML_EXCEPTION_HXX__
#define __SLINT_XML_EXCEPTION_HXX__

#include <exception>
#include <string>

namespace slint
{

class SLintXMLException : public std::exception
{
    std::string msg;

public:

    SLintXMLException(const std::string & path, const std::string & error);

#ifdef _MSC_VER
    virtual const char * what() const;
#else
    virtual const char * what() const noexcept;
#endif

};

} // namespace slint

#endif // __SLINT_XML_EXCEPTION_HXX__
