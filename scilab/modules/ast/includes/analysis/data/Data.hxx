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

#ifndef __DATA_HXX__
#define __DATA_HXX__

#include <iostream>
#include <set>

#include "tools.hxx"
#include "symbol.hxx"

namespace analysis
{

struct Data
{
    bool known;
    bool valid;
    std::set<symbol::Symbol> sharedSyms;

    Data(const bool _known, const symbol::Symbol & sym) : known(_known), valid(true)
    {
        sharedSyms.emplace(sym);
    }
    Data(const Data & d) : known(d.known), valid(true), sharedSyms(d.sharedSyms) {  }

    inline void clear()
    {
        sharedSyms.clear();
    }
    inline void add(const symbol::Symbol & sym)
    {
        if (valid)
        {
            sharedSyms.emplace(sym);
        }
    }

    inline void rem(const symbol::Symbol & sym)
    {
        if (valid)
        {
            sharedSyms.erase(sym);
        }
    }

    inline bool hasOneOwner()
    {
        return valid && known && (sharedSyms.size() == 1);
    }
    inline bool isDeletable()
    {
        return valid && known && sharedSyms.empty();
    }

    inline bool same(Data * data)
    {
        return (valid && data->valid) && (data == this || (known == data->known && sharedSyms == data->sharedSyms));
    }

    friend std::wostream & operator<<(std::wostream & out, const Data & data)
    {
        out << L"known:" << (data.known ? L"T" : L"F")
            << L", valid:" << (data.valid ? L"T" : L"F")
            << L", ";

        tools::printSet(data.sharedSyms, out);

        return out;
    }
};

} // namespace analysis

#endif // __DATA_HXX__
