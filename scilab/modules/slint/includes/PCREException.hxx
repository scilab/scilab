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
