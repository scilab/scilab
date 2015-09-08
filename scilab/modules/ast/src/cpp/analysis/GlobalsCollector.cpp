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

    macrodef.getBody().accept(*this);

    for (const auto & out : macrodef.getOut())
    {
        if (locals.find(out) == locals.end())
        {
            globals.emplace(out);
        }
    }

    stop_chrono();
}

void GlobalsCollector::visit(ast::SimpleVar & e)
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

void GlobalsCollector::visit(ast::DollarVar & e)
{
    // nothing to do
}

void GlobalsCollector::visit(ast::ColonVar & e)
{
    // nothing to do
}

void GlobalsCollector::visit(ast::ArrayListVar & e)
{
    for (auto arg : e.getVars())
    {
        arg->accept(*this);
    }
}

void GlobalsCollector::visit(ast::DoubleExp & e)
{
}

void GlobalsCollector::visit(ast::BoolExp & e)
{
}

void GlobalsCollector::visit(ast::StringExp & e)
{
}

void GlobalsCollector::visit(ast::CommentExp & e)
{
    // ignored
}

void GlobalsCollector::visit(ast::NilExp & e)
{
    // nothing to do
}

void GlobalsCollector::visit(ast::CallExp & e)
{
    for (auto arg : e.getArgs())
    {
        arg->accept(*this);
    }
    e.getName().accept(*this);
}

void GlobalsCollector::visit(ast::CellCallExp & e)
{
    for (auto arg : e.getArgs())
    {
        arg->accept(*this);
    }
    e.getName().accept(*this);
}

void GlobalsCollector::visit(ast::OpExp & e)
{
    e.getLeft().accept(*this);
    e.getRight().accept(*this);
}

void GlobalsCollector::visit(ast::LogicalOpExp & e)
{
    e.getLeft().accept(*this);
    e.getRight().accept(*this);
}

void GlobalsCollector::visit(ast::AssignExp & e)
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
            const symbol::Symbol & Lsym = static_cast<ast::SimpleVar &>(ce.getName()).getSymbol();
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

void GlobalsCollector::visit(ast::IfExp & e)
{
    e.getTest().accept(*this);
    e.getThen().accept(*this);
    if (e.hasElse())
    {
        e.getElse().accept(*this);
    }
}

void GlobalsCollector::visit(ast::WhileExp & e)
{
    e.getTest().accept(*this);
    e.getBody().accept(*this);
}

void GlobalsCollector::visit(ast::ForExp & e)
{
    e.getVardec().accept(*this);
    e.getBody().accept(*this);
}

void GlobalsCollector::visit(ast::BreakExp & e)
{
    // nothing to do
}

void GlobalsCollector::visit(ast::ContinueExp & e)
{
    // nothing to do
}

void GlobalsCollector::visit(ast::TryCatchExp & e)
{
    e.getTry().accept(*this);
    e.getCatch().accept(*this);
}

void GlobalsCollector::visit(ast::SelectExp & e)
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

void GlobalsCollector::visit(ast::CaseExp & e)
{
    e.getTest()->accept(*this);
    e.getBody()->accept(*this);
}

void GlobalsCollector::visit(ast::ReturnExp & e)
{
    // Bug with return;
    //e.exp_get().accept(*this);
}

void GlobalsCollector::visit(ast::FieldExp & e)
{
    e.getHead()->accept(*this);
    e.getTail()->accept(*this);
}

void GlobalsCollector::visit(ast::NotExp & e)
{
    e.getExp().accept(*this);
}

void GlobalsCollector::visit(ast::TransposeExp & e)
{
    e.getExp().accept(*this);
}

void GlobalsCollector::visit(ast::MatrixExp & e)
{
    for (auto mle : e.getLines())
    {
        mle->accept(*this);
    }
}

void GlobalsCollector::visit(ast::MatrixLineExp & e)
{
    for (auto _e : e.getColumns())
    {
        _e->accept(*this);
    }
}

void GlobalsCollector::visit(ast::CellExp & e)
{
    for (auto mle : e.getLines())
    {
        mle->accept(*this);
    }
}

void GlobalsCollector::visit(ast::SeqExp & e)
{
    for (auto _e : e.getExps())
    {
        _e->accept(*this);
    }
}

void GlobalsCollector::visit(ast::ArrayListExp & e)
{
    for (auto _e : e.getExps())
    {
        _e->accept(*this);
    }
}

void GlobalsCollector::visit(ast::AssignListExp & e)
{
}

void GlobalsCollector::visit(ast::VarDec & e)
{
    locals.emplace(e.getSymbol());
    e.getInit().accept(*this);
}

void GlobalsCollector::visit(ast::FunctionDec & e)
{
    locals.emplace(e.getSymbol());
    DeclaredMacroDef dmd(&e);
    GlobalsCollector gc(dmd);

    for (const auto global : gc.globals)
    {
        if (locals.find(global) == locals.end())
        {
            globals.emplace(global);
        }
    }
}

void GlobalsCollector::visit(ast::ListExp & e)
{
    e.getStart().accept(*this);
    e.getStep().accept(*this);
    e.getEnd().accept(*this);
}

void GlobalsCollector::visit(ast::OptimizedExp & e)
{
}

void GlobalsCollector::visit(ast::MemfillExp & e)
{
}

void GlobalsCollector::visit(ast::DAXPYExp & e)
{
}

void GlobalsCollector::visit(ast::IntSelectExp & e)
{
    visit(static_cast<ast::SelectExp &>(e));
}

void GlobalsCollector::visit(ast::StringSelectExp & e)
{
    visit(static_cast<ast::SelectExp &>(e));
}

} // namespace analysis
