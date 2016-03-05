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
