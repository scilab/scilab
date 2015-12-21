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

#ifndef __LOCAL_INFO_HXX__
#define __LOCAL_INFO_HXX__

#include <set>

#include "TypeLocal.hxx"

namespace analysis
{

struct LocalInfo
{
    bool refcount;
    std::set<TypeLocal> set;

    LocalInfo() : refcount(false), set() { }

};

} // namespace analysis

#endif // __LOCAL_INFO_HXX__
