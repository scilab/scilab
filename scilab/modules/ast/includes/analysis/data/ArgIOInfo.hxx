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

#ifndef __ARGIO_INFO_HXX__
#define __ARGIO_INFO_HXX__

#include "TypeLocal.hxx"

namespace analysis
{

struct ArgIOInfo
{
    const symbol::Symbol sym;
    bool refcount;
    TypeLocal tl;

    ArgIOInfo(const symbol::Symbol & _sym, const bool _refcount, const TypeLocal & _tl) : sym(_sym), refcount(_refcount), tl(_tl) { }

};

} // namespace analysis

#endif // __ARGIO_INFO_HXX__
