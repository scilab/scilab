/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __DATA_HXX__
#define __DATA_HXX__

#include <iostream>

#include "symbol.hxx"
#include "tools.hxx"

namespace analysis
{

struct Data
{
    bool known;
    bool valid;
    tools::SymbolSet sharedSyms;

    Data(const bool _known, const symbol::Symbol & sym);
    Data(const Data & d) : known(d.known), valid(true), sharedSyms(d.sharedSyms) {  }

    void clear();
    void add(const symbol::Symbol & sym);
    void rem(const symbol::Symbol & sym);
    bool hasOneOwner() const;
    bool isDeletable() const;
    bool same(Data * data);
    void setValidity(const bool v);
    friend std::wostream & operator<<(std::wostream & out, const Data & data);
};

} // namespace analysis

#endif // __DATA_HXX__
