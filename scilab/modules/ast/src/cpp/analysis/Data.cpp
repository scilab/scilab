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

#include "tools.hxx"
#include "data/Data.hxx"

namespace analysis
{

Data::Data(const bool _known, const symbol::Symbol & sym) : known(_known), valid(true), sharedSyms()
{
    sharedSyms.emplace(sym);
}

void Data::clear()
{
    valid = true;
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

void Data::setValidity(const bool v)
{
    if (valid && !v)
    {
        valid = false;
        sharedSyms.clear();
    }
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
