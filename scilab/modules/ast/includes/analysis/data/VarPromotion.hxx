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
