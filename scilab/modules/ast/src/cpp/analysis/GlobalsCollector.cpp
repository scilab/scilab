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

#include "data/GlobalsCollector.hxx"
#include "tools.hxx"

namespace analysis
{

void GlobalsCollector::collect(MacroDef & macrodef)
{
    GlobalsCollector(macrodef).collect();
}

const tools::SymbolOrdSet & GlobalsCollector::getGlobals() const
{
    return globals;
}

std::wostream & operator<<(std::wostream & out, const GlobalsCollector & gc)
{
    out << L"name: " << gc.macrodef.getName() << std::endl
        << L" -out: ";
    tools::printSet(gc.macrodef.getOut(), out);
    out << std::endl
        << L" -in: ";
    tools::printSet(gc.macrodef.getIn(), out);
    out << std::endl
        << L" -locals: ";
    tools::printSet(gc.locals, out);
    out << std::endl
        << L" -globals: ";
    tools::printSet(gc.globals, out);

    return out;
}

void GlobalsCollector::print_info()
{
    std::wcout << L"Globals collection: " << *static_cast<Chrono *>(this) << std::endl
               << *this << std::endl;
}

void GlobalsCollector::collect()
{
    start_chrono();

    for (const auto arg : macrodef.getIn())
    {
        locals.emplace(arg);
    }

    macrodef.getOriginalBody().accept(*this);

    for (const auto & out : macrodef.getOut())
    {
        if (locals.find(out) == locals.end())
        {
            globals.emplace(out);
        }
    }

    stop_chrono();
}

void GlobalsCollector::visit(const ast::SimpleVar & e)
{
    if (!e.getParent()->isFieldExp() || static_cast<ast::FieldExp *>(e.getParent())->getTail() != &e)
    {
        const symbol::Symbol & sym = e.getSymbol();
        if (locals.find(sym) == locals.end())
        {
            globals.emplace(sym);
        }
    }
}

void GlobalsCollector::visit(const ast::DollarVar & e)
{
    // nothing to do
}

void GlobalsCollector::visit(const ast::ColonVar & e)
{
    // nothing to do
}

void GlobalsCollector::visit(const ast::ArrayListVar & e)
{
    for (auto arg : e.getVars())
    {
        arg->accept(*this);
    }
}

void GlobalsCollector::visit(const ast::DoubleExp & e)
{
}

void GlobalsCollector::visit(const ast::BoolExp & e)
{
}

void GlobalsCollector::visit(const ast::StringExp & e)
{
}

void GlobalsCollector::visit(const ast::CommentExp & e)
{
    // ignored
}

void GlobalsCollector::visit(const ast::NilExp & e)
{
    // nothing to do
}

void GlobalsCollector::visit(const ast::CallExp & e)
{
    for (auto arg : e.getArgs())
    {
        arg->accept(*this);
    }
    e.getName().accept(*this);
}

void GlobalsCollector::visit(const ast::CellCallExp & e)
{
    for (auto arg : e.getArgs())
    {
        arg->accept(*this);
    }
    e.getName().accept(*this);
}

void GlobalsCollector::visit(const ast::OpExp & e)
{
    e.getLeft().accept(*this);
    e.getRight().accept(*this);
}

void GlobalsCollector::visit(const ast::LogicalOpExp & e)
{
    e.getLeft().accept(*this);
    e.getRight().accept(*this);
}

void GlobalsCollector::visit(const ast::AssignExp & e)
{
    if (e.getLeftExp().isSimpleVar())
    {
        // A = ...
        const symbol::Symbol & Lsym = static_cast<ast::SimpleVar &>(e.getLeftExp()).getSymbol();
        locals.emplace(Lsym);
    }
    else if (e.getLeftExp().isCallExp())
    {
        // A(...) = ...
        ast::CallExp & ce = static_cast<ast::CallExp &>(e.getLeftExp());
        if (ce.getName().isSimpleVar())
        {
            const symbol::Symbol & Lsym = static_cast<const ast::SimpleVar &>(ce.getName()).getSymbol();
            locals.emplace(Lsym);
        }
        for (auto arg : ce.getArgs())
        {
            arg->accept(*this);
        }
    }
    else if (e.getLeftExp().isAssignListExp())
    {
        // [A, ...] =
        ast::AssignListExp & ale = static_cast<ast::AssignListExp &>(e.getLeftExp());
        for (const auto re : ale.getExps())
        {
            if (re->isSimpleVar())
            {
                const symbol::Symbol & Lsym = static_cast<const ast::SimpleVar *>(re)->getSymbol();
                locals.emplace(Lsym);
            }
        }
    }
    else
    {
        e.getLeftExp().accept(*this);
    }
    e.getRightExp().accept(*this);
}

void GlobalsCollector::visit(const ast::IfExp & e)
{
    e.getTest().accept(*this);
    e.getThen().accept(*this);
    if (e.hasElse())
    {
        e.getElse().accept(*this);
    }
}

void GlobalsCollector::visit(const ast::WhileExp & e)
{
    e.getTest().accept(*this);
    e.getBody().accept(*this);
}

void GlobalsCollector::visit(const ast::ForExp & e)
{
    e.getVardec().accept(*this);
    e.getBody().accept(*this);
}

void GlobalsCollector::visit(const ast::BreakExp & e)
{
    // nothing to do
}

void GlobalsCollector::visit(const ast::ContinueExp & e)
{
    // nothing to do
}

void GlobalsCollector::visit(const ast::TryCatchExp & e)
{
    e.getTry().accept(*this);
    e.getCatch().accept(*this);
}

void GlobalsCollector::visit(const ast::SelectExp & e)
{
    e.getSelect()->accept(*this);
    for (auto _e : e.getCases())
    {
        _e->accept(*this);
    }
    if (ast::Exp * def = e.getDefaultCase())
    {
        def->accept(*this);
    }
}

void GlobalsCollector::visit(const ast::CaseExp & e)
{
    e.getTest()->accept(*this);
    e.getBody()->accept(*this);
}

void GlobalsCollector::visit(const ast::ReturnExp & e)
{
    // Bug with return;
    //e.exp_get().accept(*this);
}

void GlobalsCollector::visit(const ast::FieldExp & e)
{
    e.getHead()->accept(*this);
    e.getTail()->accept(*this);
}

void GlobalsCollector::visit(const ast::NotExp & e)
{
    e.getExp().accept(*this);
}

void GlobalsCollector::visit(const ast::TransposeExp & e)
{
    e.getExp().accept(*this);
}

void GlobalsCollector::visit(const ast::MatrixExp & e)
{
    for (auto mle : e.getLines())
    {
        mle->accept(*this);
    }
}

void GlobalsCollector::visit(const ast::MatrixLineExp & e)
{
    for (auto _e : e.getColumns())
    {
        _e->accept(*this);
    }
}

void GlobalsCollector::visit(const ast::CellExp & e)
{
    for (auto mle : e.getLines())
    {
        mle->accept(*this);
    }
}

void GlobalsCollector::visit(const ast::SeqExp & e)
{
    for (auto _e : e.getExps())
    {
        _e->accept(*this);
    }
}

void GlobalsCollector::visit(const ast::ArrayListExp & e)
{
    for (auto _e : e.getExps())
    {
        _e->accept(*this);
    }
}

void GlobalsCollector::visit(const ast::AssignListExp & e)
{
}

void GlobalsCollector::visit(const ast::VarDec & e)
{
    locals.emplace(e.getSymbol());
    e.getInit().accept(*this);
}

void GlobalsCollector::visit(const ast::FunctionDec & e)
{
    locals.emplace(e.getSymbol());
    DeclaredMacroDef dmd(const_cast<ast::FunctionDec *>(&e));
    GlobalsCollector gc(dmd);

    for (const auto global : gc.globals)
    {
        if (locals.find(global) == locals.end())
        {
            globals.emplace(global);
        }
    }
}

void GlobalsCollector::visit(const ast::ListExp & e)
{
    e.getStart().accept(*this);
    e.getStep().accept(*this);
    e.getEnd().accept(*this);
}

void GlobalsCollector::visit(const ast::OptimizedExp & e)
{
}

void GlobalsCollector::visit(const ast::MemfillExp & e)
{
}

void GlobalsCollector::visit(const ast::DAXPYExp & e)
{
}

void GlobalsCollector::visit(const ast::IntSelectExp & e)
{
    visit(static_cast<const ast::SelectExp &>(e));
}

void GlobalsCollector::visit(const ast::StringSelectExp & e)
{
    visit(static_cast<const ast::SelectExp &>(e));
}

} // namespace analysis
