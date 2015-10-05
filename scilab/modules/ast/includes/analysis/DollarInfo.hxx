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

#ifndef __DOLLAR_INFO_HXX__
#define __DOLLAR_INFO_HXX__

#include <iostream>
#include "symbol.hxx"
#include <cstdint>

namespace analysis
{

class DollarInfo
{

    const symbol::Symbol sym;
    uint32_t index;

public:

    DollarInfo(const symbol::Symbol _sym, const uint32_t _index) : sym(_sym), index(_index) { }

    inline symbol::Symbol getSymbol() const
    {
        return sym;
    }

    inline uint32_t getIndex() const
    {
        return index;
    }

    inline uint32_t & getIndex()
    {
        return index;
    }

    friend std::wostream & operator<<(std::wostream & out, const DollarInfo & di)
    {
        out << L"$ in " << di.sym << L" at index " << di.index;
        return out;
    }

};

} // namespace analysis

#endif // __DOLLAR_INFO_HXX__
