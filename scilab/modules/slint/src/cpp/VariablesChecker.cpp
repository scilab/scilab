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

#include "checkers/VariablesChecker.hxx"

namespace slint
{
void VariablesChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.isFunctionDec())
    {
        const ast::FunctionDec & fd = static_cast<const ast::FunctionDec &>(e);
        if (!assigned.empty())
        {
            // we declare the function in the current scope
            std::pair<Location, ast::AssignListExp *> p = { e.getLocation(), nullptr };
            assigned.top().emplace(fd.getSymbol().getName(), p);
        }

        assigned.emplace(std::unordered_map<std::wstring, std::pair<Location, ast::AssignListExp *>>());
        used.emplace(std::unordered_map<std::wstring, const ast::Exp *>());

        // a function cans refer to itself
        std::pair<Location, ast::AssignListExp *> p = { e.getLocation(), nullptr };
        assigned.top().emplace(fd.getSymbol().getName(), p);
    }
    else
    {
        if (!used.empty())
        {
            if (e.isSimpleVar())
            {
                const ast::SimpleVar & var = static_cast<const ast::SimpleVar &>(e);
                if (context.isAssignedVar(var))
                {
                    // if we are not in the context on a nested assignment in a function call (foo(a,b=2))
                    if (!var.getParent()->getParent() || !var.getParent()->getParent()->isCallExp())
                    {
                        const std::wstring & name = var.getSymbol().getName();
                        auto i = used.top().find(name);
                        if (!context.topLoop() || (i == used.top().end()) || !isParentOf(context.topLoop(), i->second))
                        {
                            // the variable has never been used or the last time it wasn't in a loop
                            if (i != used.top().end())
                            {
                                used.top().erase(i);
                            }
                            if (var.getParent() == context.getLHSExp() && var.getParent()->isAssignListExp())
                            {
                                // we have something like [lhs, rhs] = argn(0)
                                // if rhs is used and lhs is not used then this not an "error":
                                // we are obliged to define lhs just to get rhs.
                                // So when rhs is used lhs is "used" too.
                                std::pair<Location, ast::AssignListExp *> p = { var.getLocation(), static_cast<ast::AssignListExp *>(var.getParent()) };
                                assigned.top().emplace(name, p);
                            }
                            else
                            {
                                std::pair<Location, ast::AssignListExp *> p = { var.getLocation(), nullptr };
                                assigned.top().emplace(name, p);
                            }
                        }
                        else /*if (context.topLoop() && i != used.top.end() && isParentOf(context.topLoop(), i->second))*/
                        {
                            // Just to remember...
                            // Here the variable has already been used in the current loop and we reassign it
                            // something like:
                            // while (...)
                            //  ... use i
                            //  ...
                            //  i = ...
                            // end
                            // In general the loop will looping so the assignment is implicitly destroyed by the use in the next iteration
                            // so we don't add this assignment !
                        }
                    }
                }
                else if (!e.getParent()->isFieldExp() || static_cast<const ast::FieldExp *>(e.getParent())->getTail() != &e)
                {
                    const symbol::Symbol & sym = var.getSymbol();
                    const std::wstring & name = sym.getName();
                    if (used.top().find(name) == used.top().end())
                    {
                        used.top().emplace(name, context.topLoop());
                        auto i = assigned.top().find(name);
                        if (i == assigned.top().end())
                        {
                            if (!context.isFunIn(name) && !SLintChecker::isScilabConstant(name))
                            {
                                types::InternalType * pIT = symbol::Context::getInstance()->get(sym);
                                if (pIT)
                                {
                                    if (!pIT->isFunction() && !pIT->isMacroFile() && !pIT->isMacro())
                                    {
                                        result.report(context, e.getLocation(), *this, _("Use of non-initialized variable \'%s\' may have any side-effects."), name);
                                    }
                                }
                                else if (!context.isPrivateFunction(sym))
                                {
                                    /* The symbol doesn't correspond to a private function:
                                       function tata()
                                       titi()
                                       end

                                       function titi()
                                       ...
                                       end

                                       titi is private but usable in tata.
                                    */
                                    std::wstring fname;
                                    if (context.isExternPrivateFunction(sym, fname))
                                    {
                                        result.report(context, e.getLocation(), *this, _("Use of a private macro \'%s\' defined in an other file %s."), name, fname);
                                    }
                                    else if (!context.getPublicFunction(fname))
                                    {
                                        // The macro has not been declared somewhere in the project
                                        result.report(context, e.getLocation(), *this, _("Use of non-initialized variable \'%s\' may have any side-effects."), name);
                                    }
                                }
                            }
                        }
                        else
                        {
                            if (ast::AssignListExp * ale = i->second.second)
                            {
                                // the variable is in an AssignListExp
                                // so we must "use" the variables which preceed it too
                                for (auto e : ale->getExps())
                                {
                                    if (e->isSimpleVar())
                                    {
                                        const std::wstring & prevName = static_cast<ast::SimpleVar *>(e)->getSymbol().getName();
                                        assigned.top().erase(prevName);
                                        if (prevName == name)
                                        {
                                            break;
                                        }
                                        else
                                        {
                                            used.top().emplace(prevName, context.topLoop());
                                        }
                                    }
                                }
                            }
                            else
                            {
                                assigned.top().erase(i);
                            }
                        }
                    }
                }
            }
            // for i=1:10... end even if i is not used, i is useful to make the loop
            /*else if (e.isVarDec())
            {
                const ast::VarDec & vd = static_cast<const ast::VarDec &>(e);
                const std::wstring & name = vd.getSymbol().getName();
                std::pair<Location, ast::AssignListExp *> p = { vd.getLocation(), nullptr };
                assigned.top().emplace(name, p);
                used.top().erase(name);
            }*/
        }
    }
}

void VariablesChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.isFunctionDec())
    {
        const ast::FunctionDec & fd = static_cast<const ast::FunctionDec &>(e);
        auto & map = assigned.top();
        const auto & out = context.getFunOut();
        for (const auto & n : out)
        {
            map.erase(n);
        }
        map.erase(fd.getSymbol().getName());
        for (const auto & p : map)
        {
            result.report(context, p.second.first, *this, _("Declared variable and might be unused: %s."), p.first);
        }
        assigned.pop();
        used.pop();
    }
}

bool VariablesChecker::isParentOf(const ast::Exp * potentialParent, const ast::Exp * potentialChild)
{
    const ast::Exp * c = potentialChild;
    while (c && (potentialParent != c))
    {
        c = c->getParent();
    }

    return c != nullptr;
}

const std::string VariablesChecker::getName() const
{
    return "VariablesChecker";
}
}
