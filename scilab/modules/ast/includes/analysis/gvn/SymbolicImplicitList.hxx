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

#ifndef __SYMBOLIC_IMPLICITLIST_HXX__
#define __SYMBOLIC_IMPLICITLIST_HXX__

#include <iostream>

#include "GVN.hxx"
#include "tools.hxx"

namespace analysis
{

/**
 * \class SymbolicImplicitlist
 * \brief Manage a symbolic implicitlist
 */
class SymbolicImplicitList
{
    GVN * gvn;
    GVN::Value * start;
    GVN::Value * step;
    GVN::Value * end;

};

} // namespace analysis

#endif // __SYMBOLIC_IMPLICITLIST_HXX__
