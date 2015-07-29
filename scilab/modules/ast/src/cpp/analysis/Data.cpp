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

#include "tools.hxx"
#include "data/Data.hxx"

namespace analysis
{

    Data::Data(const bool _known, const symbol::Symbol & sym) : known(_known), valid(true)
    {
        sharedSyms.emplace(sym);
    }

    void Data::clear()
    {
        sharedSyms.clear();
    }

    void Data::add(const symbol::Symbol & sym)
    {
        if (valid)
        {
            sharedSyms.emplace(sym);
        }
    }

    void Data::rem(const symbol::Symbol & sym)
    {
        if (valid)
        {
            sharedSyms.erase(sym);
        }
    }

    bool Data::hasOneOwner() const
    {
        return valid && known && (sharedSyms.size() == 1);
    }

    bool Data::isDeletable() const
    {
        return valid && known && sharedSyms.empty();
    }

    bool Data::same(Data * data)
    {
        return (valid && data->valid) && (data == this || (known == data->known && sharedSyms == data->sharedSyms));
    }

    std::wostream & operator<<(std::wostream & out, const Data & data)
    {
        out << L"known:" << (data.known ? L"T" : L"F")
            << L", valid:" << (data.valid ? L"T" : L"F")
            << L", ";

        tools::printSet(data.sharedSyms, out);

        return out;
    }

} // namespace analysis
