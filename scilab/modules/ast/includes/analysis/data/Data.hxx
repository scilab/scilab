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
