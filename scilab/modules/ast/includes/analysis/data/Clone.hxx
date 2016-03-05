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

    friend std::wostream & operator<<(std::wostream & out, const Clone & c)
    {
        out << L"Clone";
        tools::printSet(c.set, out);

        return out;
    }
};

} // namespace analysis


#endif // __CLONE_HXX__
