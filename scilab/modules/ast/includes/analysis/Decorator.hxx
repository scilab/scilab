/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __DECORATOR_HXX__
#define __DECORATOR_HXX__

#include "Result.hxx"

#include <iostream>

namespace analysis
{
struct Decorator
{
    Result res;

    Decorator() : res() { }

    friend std::wostream & operator<<(std::wostream & out, const Decorator & deco)
    {
        out << deco.res;
        return out;
    }
};

} // namespace analysis

#endif // __DECORATOR_HXX__
