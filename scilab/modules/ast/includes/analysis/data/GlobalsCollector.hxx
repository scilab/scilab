/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GLOBALS_COLLECTOR_HXX__
#define __GLOBALS_COLLECTOR_HXX__

#include <iostream>
#include <string>
#include <set>
#include <sstream>

#include "visitor.hxx"
#include "execvisitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "Chrono.hxx"
#include "tools.hxx"
#include "MacroDef.hxx"

namespace analysis
{

class GlobalsCollector : public ast::Visitor, public Chrono
{

    MacroDef & macrodef;
    std::set<symbol::Symbol> locals;
    std::set<symbol::Symbol> & globals;

    GlobalsCollector(MacroDef & _macrodef) : macrodef(_macrodef), globals(_macrodef.getGlobals()) { }

public:

    virtual ~GlobalsCollector()
    {
    }

    inline static void collect(MacroDef & macrodef)
    {
        GlobalsCollector gc(macrodef);
        gc.collect();
    }

    inline const std::set<symbol::Symbol> & getGlobals() const
    {
        return globals;
    }

    friend std::wostream & operator<<(std::wostream & out, const GlobalsCollector & gc)
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

    inline void print_info()
    {
        std::wcout << L"Globals collection: " << *static_cast<Chrono *>(this) << std::endl;
    }

private:

    inline void collect()
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

    void visit(ast::SimpleVar & e)
    {
        const symbol::Symbol & sym = e.getSymbol();
        if (locals.find(sym) == locals.end())
        {
            globals.emplace(sym);
        }
    }

    void visit(ast::DollarVar & e)
    {
        // nothing to do
    }

    void visit(ast::ColonVar & e)
    {
        // nothing to do
    }

    void visit(ast::ArrayListVar & e)
    {
        for (auto arg : e.getVars())
        {
            arg->accept(*this);
        }
    }

    void visit(ast::DoubleExp & e)
    {
    }

    void visit(ast::BoolExp & e)
    {
    }

    void visit(ast::StringExp & e)
    {
    }

    void visit(ast::CommentExp & e)
    {
        // ignored
    }

    void visit(ast::NilExp & e)
    {
        // nothing to do
    }

    void visit(ast::CallExp & e)
    {
        for (auto arg : e.getArgs())
        {
            arg->accept(*this);
        }
        e.getName().accept(*this);
    }

    void visit(ast::CellCallExp & e)
    {
        for (auto arg : e.getArgs())
        {
            arg->accept(*this);
        }
        e.getName().accept(*this);
    }

    void visit(ast::OpExp & e)
    {
        e.getLeft().accept(*this);
        e.getRight().accept(*this);
    }

    void visit(ast::LogicalOpExp & e)
    {
        e.getLeft().accept(*this);
        e.getRight().accept(*this);
    }

    void visit(ast::AssignExp & e)
    {
        if (e.getLeftExp().isSimpleVar())
        {
            const symbol::Symbol & Lsym = static_cast<ast::SimpleVar &>(e.getLeftExp()).getSymbol();
            locals.emplace(Lsym);
        }
        else if (e.getLeftExp().isCallExp())
        {
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

    void visit(ast::IfExp & e)
    {
        e.getTest().accept(*this);
        e.getThen().accept(*this);
        if (e.hasElse())
        {
            e.getElse().accept(*this);
        }
    }

    void visit(ast::WhileExp & e)
    {
        e.getTest().accept(*this);
        e.getBody().accept(*this);
    }

    void visit(ast::ForExp & e)
    {
        e.getVardec().accept(*this);
        e.getBody().accept(*this);
    }

    void visit(ast::BreakExp & e)
    {
        // nothing to do
    }

    void visit(ast::ContinueExp & e)
    {
        // nothing to do
    }

    void visit(ast::TryCatchExp & e)
    {
        e.getTry().accept(*this);
        e.getCatch().accept(*this);
    }

    void visit(ast::SelectExp & e)
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

    void visit(ast::CaseExp & e)
    {
        e.getTest()->accept(*this);
        e.getBody()->accept(*this);
    }

    void visit(ast::ReturnExp & e)
    {
        // Bug with return;
        //e.exp_get().accept(*this);
    }

    void visit(ast::FieldExp & e)
    {
        //e.head_get()->accept(*this);
        //e.tail_get()->accept(*this);
    }

    void visit(ast::NotExp & e)
    {
        e.getExp().accept(*this);
    }

    void visit(ast::TransposeExp & e)
    {
        e.getExp().accept(*this);
    }

    void visit(ast::MatrixExp & e)
    {
        for (auto mle : e.getLines())
        {
            mle->accept(*this);
        }
    }

    void visit(ast::MatrixLineExp & e)
    {
        for (auto _e : e.getColumns())
        {
            _e->accept(*this);
        }
    }

    void visit(ast::CellExp & e)
    {
        for (auto mle : e.getLines())
        {
            mle->accept(*this);
        }
    }

    void visit(ast::SeqExp & e)
    {
        for (auto _e : e.getExps())
        {
            _e->accept(*this);
        }
    }

    void visit(ast::ArrayListExp & e)
    {
        for (auto _e : e.getExps())
        {
            _e->accept(*this);
        }
    }

    void visit(ast::AssignListExp & e)
    {
    }

    void visit(ast::VarDec & e)
    {
        locals.emplace(e.getSymbol());
        e.getInit().accept(*this);
    }

    void visit(ast::FunctionDec & e)
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

    void visit(ast::ListExp & e)
    {
        e.getStart().accept(*this);
        e.getStep().accept(*this);
        e.getEnd().accept(*this);
    }

    void visit(ast::OptimizedExp & e)
    {
    }

    void visit(ast::DAXPYExp & e)
    {
    }
};

} // namespace analysis

#endif // __GLOBALS_COLLECTOR_HXX__
