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

#ifndef __CLONE_HXX__
#define __CLONE_HXX__

#include <iostream>

#include "tools.hxx"

namespace analysis
{

class Clone
{

    tools::SymbolSet set;

public:

    Clone() { }
    Clone(const symbol::Symbol & sym)
    {
        add(sym);
    }

    inline void add(const symbol::Symbol & sym)
    {
        set.emplace(sym);
    }

    inline const tools::SymbolSet & get() const
    {
        return set;
    }

    inline void clear()
    {
        set.clear();
    }

    friend std::ostream & operator<<(std::ostream & out, const Clone & c)
    {
        out << "Clone";
        tools::printSet(c.set, out);

        return out;
    }
};

} // namespace analysis


#endif // __CLONE_HXX__
