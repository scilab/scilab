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

#include <iostream>
#include <set>

#include "checkers/FunctionArgsChecker.hxx"

namespace slint
{
void FunctionArgsChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::exps_t & args = static_cast<const ast::FunctionDec &>(e).getArgs().getVars();
    std::set<symbol::Symbol> in;
    std::set<symbol::Symbol> in_dup;
    for (const auto arg : args)
    {
        const symbol::Symbol & sym = static_cast<const ast::SimpleVar *>(arg)->getSymbol();
        if (in.find(sym) != in.end())
        {
            in_dup.emplace(sym);
        }
        else
        {
            in.emplace(sym);
        }
    }

    const ast::exps_t & rets = static_cast<const ast::FunctionDec &>(e).getReturns().getVars();
    std::set<symbol::Symbol> out;
    std::set<symbol::Symbol> out_dup;
    for (const auto ret : rets)
    {
        const symbol::Symbol & sym = static_cast<const ast::SimpleVar *>(ret)->getSymbol();
        if (out.find(sym) != out.end())
        {
            out_dup.emplace(sym);
        }
        else
        {
            out.emplace(sym);
        }
    }

    std::set<symbol::Symbol> inter;
    for (const auto & sym : in)
    {
        if (out.find(sym) != out.end())
        {
            inter.emplace(sym);
        }
    }

    if (!in_dup.empty())
    {
        std::wostringstream wos;
        for (std::set<symbol::Symbol>::const_iterator i = in_dup.begin(), end = std::prev(in_dup.end()); i != end; ++i)
        {
            wos << i->getName() << L", ";
        }
        wos << std::prev(in_dup.end())->getName();
        result.report(context, e.getLocation(), *this, _("Duplicated function arguments: %s."), wos.str());
    }

    if (!out_dup.empty())
    {
        std::wostringstream wos;
        for (std::set<symbol::Symbol>::const_iterator i = out_dup.begin(), end = std::prev(out_dup.end()); i != end; ++i)
        {
            wos << i->getName() << L", ";
        }
        wos << std::prev(out_dup.end())->getName();
        result.report(context, e.getLocation(), *this, _("Duplicated function output values: %s."), wos.str());
    }

    if (!inter.empty())
    {
        std::wostringstream wos;
        for (std::set<symbol::Symbol>::const_iterator i = inter.begin(), end = std::prev(inter.end()); i != end; ++i)
        {
            wos << i->getName() << L", ";
        }
        wos << std::prev(inter.end())->getName();
        result.report(context, e.getLocation(), *this, _("Function arguments used as output values: %s."), wos.str());
    }
}

void FunctionArgsChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string FunctionArgsChecker::getName() const
{
    return "FunctionArgsChecker";
}
}
