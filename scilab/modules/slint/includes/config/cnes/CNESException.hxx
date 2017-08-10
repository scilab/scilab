/*
 *  Scilab ( http://www.scilab.org/ ) - This cnes is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This cnes must be used under the terms of the CeCILL.
 *  This source cnes is licensed as described in the cnes COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __CNES_EXCEPTION_HXX__
#define __CNES_EXCEPTION_HXX__

#include <exception>
#include <string>

namespace slint
{

namespace CNES
{
class CNESException : public std::exception
{

    std::string msg;

public:

    CNESException(const std::wstring & error);

#ifdef _MSC_VER
    virtual const char * what() const;
#else
    virtual const char * what() const noexcept;
#endif

};
} // namespace CNES

} // namespace slint

#endif // __CNES_EXCEPTION_HXX__
