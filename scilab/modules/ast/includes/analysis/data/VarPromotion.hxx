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

#ifndef __VARPROMOTION_HXX__
#define __VARPROMOTION_HXX__

#include <iostream>

#include "tools.hxx"
#include "TIType.hxx"

namespace analysis
{

class VarPromotion
{

public:

    struct Promotion
    {
        TIType first;
        TIType second;

        Promotion(const TIType & _first, const TIType & _second) : first(_first), second(_second) { }

        friend std::wostream & operator<<(std::wostream & out, const Promotion & p)
        {
            out << L"(" << p.first << L" => " << p.second << L")";
            return out;
        }
    };

private:

    tools::SymbolMap<Promotion> map;

public:

    VarPromotion() { }

    inline void add(const symbol::Symbol & sym, const TIType & first, const TIType & second)
    {
        map.emplace(sym, Promotion(first, second));
    }

    inline const tools::SymbolMap<Promotion> & get() const
    {
        return map;
    }

    inline void clear()
    {
        map.clear();
    }

    friend std::wostream & operator<<(std::wostream & out, const VarPromotion & c)
    {
        out << L"VarPromotion";
        tools::printMap(c.map, out);

        return out;
    }
};

} // namespace analysis


#endif // __VARPROMOTION_HXX__
