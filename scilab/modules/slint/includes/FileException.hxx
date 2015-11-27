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

#ifndef __FILE_EXCEPTION_HXX__
#define __FILE_EXCEPTION_HXX__

#include <exception>
#include <string>

namespace slint
{

class FileException : public std::exception
{
    std::string msg;

public:

    FileException(const std::wstring & filename, const std::wstring & error);
    FileException(const std::wstring & filename, const std::string & error);

#ifdef _MSC_VER
    virtual const char * what() const;
#else
    virtual const char * what() const noexcept;
#endif

};

} // namespace slint

#endif // __FILE_EXCEPTION_HXX__
