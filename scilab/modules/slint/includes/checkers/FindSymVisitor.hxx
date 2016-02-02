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

#ifndef __SLINT_FIND_SYM_VISITOR_HXX__
#define __SLINT_FIND_SYM_VISITOR_HXX__

#include <set>

#include "dummyvisitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "alldec.hxx"

namespace slint
{

class FindSymVisitor : public ast::DummyVisitor
{

    std::set<symbol::Symbol> & syms;

public:

    FindSymVisitor(std::set<symbol::Symbol> & _syms, const ast::Exp & e) : syms(_syms)
    {
        e.accept(*this);
    }

    ~FindSymVisitor() { }

private:

    void visit(const ast::SimpleVar & e)
    {
        const symbol::Symbol & sym = e.getSymbol();
        auto i = syms.find(sym);
        if (i != syms.end())
        {
            syms.erase(i);
        }
    }
};

} // namespace FindSym

#endif // __SLINT_FIND_SYM_VISITOR_HXX__
