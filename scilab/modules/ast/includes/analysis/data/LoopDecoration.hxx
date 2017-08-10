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

#ifndef __LOOPDECORATION_HXX__
#define __LOOPDECORATION_HXX__

#include <iostream>

#include "tools.hxx"
#include "Clone.hxx"
#include "VarPromotion.hxx"

namespace analysis
{

class LoopDecoration
{

    Clone clone;
    VarPromotion promotion;

public:

    LoopDecoration() { }

    inline void addClone(const symbol::Symbol & sym)
    {
        clone.add(sym);
    }

    inline const tools::SymbolSet & getClone() const
    {
        return clone.get();
    }

    inline void addPromotion(const symbol::Symbol & sym, const TIType & first, const TIType & second)
    {
        promotion.add(sym, first, second);
    }

    inline const tools::SymbolMap<VarPromotion::Promotion> & getPromotion() const
    {
        return promotion.get();
    }

    inline void clear()
    {
        clone.clear();
        promotion.clear();
    }

    friend std::wostream & operator<<(std::wostream & out, const LoopDecoration & ld)
    {
        const bool cl = ld.getClone().empty();
        const bool pr = ld.getPromotion().empty();

        if (!cl || !pr)
        {
            out << L"LoopDecoration: ";
            if (!cl)
            {
                out << ld.clone;
                if (pr)
                {
                    out << L"; ";
                }
            }
            if (!pr)
            {
                out << ld.promotion;
            }
        }

        return out;
    }
};

} // namespace analysis


#endif // __LOOPDECORATION_HXX__
