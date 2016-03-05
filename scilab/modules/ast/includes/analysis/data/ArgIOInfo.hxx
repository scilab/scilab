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
