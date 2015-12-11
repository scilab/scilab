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
