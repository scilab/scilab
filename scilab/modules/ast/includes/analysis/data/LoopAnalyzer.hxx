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

#ifndef __LOOP_ANALYZER_HXX__
#define __LOOP_ANALYZER_HXX__

#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <unordered_map>

#include "visitor.hxx"
#include "execvisitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "Chrono.hxx"
#include "tools.hxx"

namespace analysis
{

class LoopAnalyzer : public ast::Visitor, public Chrono
{
    std::unordered_map<ast::Exp *, tools::SymbolSet> assigned;
    std::unordered_map<ast::Exp *, tools::SymbolSet> inserted;
    std::unordered_map<ast::Exp *, tools::SymbolSet> shared;
    std::stack<ast::Exp *> loops;
    ast::Exp * seq;

public:

    LoopAnalyzer(ast::Exp * _seq) : seq(_seq)
    {
        //start_chrono();
        static_cast<ast::SeqExp *>(seq)->accept(*this);
        //stop_chrono();
    }

    virtual ~LoopAnalyzer()
    {
    }

    inline bool isAssigned(ast::Exp * e, const symbol::Symbol & sym) const
    {
        if (e)
        {
            const auto i = assigned.find(e);
            if (i != assigned.end())
            {
                return i->second.find(sym) != i->second.end();
            }
        }
        return false;
    }

    inline const tools::SymbolSet * getInserted(ast::Exp * e) const
    {
        if (e)
        {
            const auto i = inserted.find(e);
            if (i != inserted.end())
            {
                return &i->second;
            }
        }
        return nullptr;
    }

    inline const tools::SymbolSet * getShared(ast::Exp * e) const
    {
        if (e)
        {
            const auto i = shared.find(e);
            if (i != shared.end())
            {
                return &i->second;
            }
        }
        return nullptr;
    }

    friend std::wostream & operator<<(std::wostream & out, const LoopAnalyzer & la)
    {
        if (!la.assigned.empty())
        {
            out << L" Assigned:\n";
            for (const auto & p : la.assigned)
            {
                out << L"  -Loop at " << p.first->getLocation().getLocationString() << L": ";
                tools::printSet(p.second, out);
                out << L"\n";
            }
        }

        if (!la.inserted.empty())
        {
            out << L" Inserted:\n";
            for (const auto & p : la.inserted)
            {
                out << L"  -Loop at " << p.first->getLocation().getLocationString() << L": ";
                tools::printSet(p.second, out);
                out << L"\n";
            }
        }

        if (!la.shared.empty())
        {
            out << L" Shared:\n";
            for (const auto & p : la.shared)
            {
                out << L"  -Loop at " << p.first->getLocation().getLocationString() << L": ";
                tools::printSet(p.second, out);
                out << L"\n";
            }
        }

        return out;
    }

    inline void print_info()
    {
        std::wcerr << L"Loop analyze: " << *static_cast<Chrono *>(this) << std::endl
                   << *this << std::endl;
    }

private:

    inline void emplace(std::unordered_map<ast::Exp *, tools::SymbolSet> & map, const symbol::Symbol & sym)
    {
        ast::Exp * loop = loops.top();
        auto i = map.find(loop);
        if (i == map.end())
        {
            i = map.emplace(loop, tools::SymbolSet()).first;
        }
        i->second.emplace(sym);
    }

    inline void pushAssigned()
    {
        ast::Exp * last = loops.top();
        loops.pop();

        if (!loops.empty())
        {
            ast::Exp * penult = loops.top();
            const auto i = assigned.find(last);
            if (i != assigned.end())
            {
                const auto j = assigned.find(penult);
                if (j == assigned.end())
                {
                    assigned.emplace(penult, i->second);
                }
                else
                {
                    j->second.insert(i->second.begin(), i->second.end());
                }
            }
        }
    }

    void visit(ast::SimpleVar & e)
    {
    }

    void visit(ast::DollarVar & e)
    {
    }

    void visit(ast::ColonVar & e)
    {
    }

    void visit(ast::ArrayListVar & e)
    {
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
    }

    void visit(ast::NilExp & e)
    {
    }

    void visit(ast::CallExp & e)
    {
    }

    void visit(ast::CellCallExp & e)
    {
    }

    void visit(ast::OpExp & e)
    {
    }

    void visit(ast::LogicalOpExp & e)
    {
    }

    void visit(ast::AssignExp & e)
    {
        if (e.getLeftExp().isSimpleVar())
        {
            // A = ....
            const symbol::Symbol & Lsym = static_cast<ast::SimpleVar &>(e.getLeftExp()).getSymbol();
            emplace(assigned, Lsym);
            if (e.getRightExp().isSimpleVar())
            {
                const symbol::Symbol & Rsym = static_cast<ast::SimpleVar &>(e.getRightExp()).getSymbol();
                emplace(shared, Lsym);
                emplace(shared, Rsym);
            }
        }
        else if (e.getLeftExp().isCallExp())
        {
            // A(...) = ...
            ast::CallExp & ce = static_cast<ast::CallExp &>(e.getLeftExp());
            if (ce.getName().isSimpleVar())
            {
                const symbol::Symbol & Lsym = static_cast<ast::SimpleVar &>(ce.getName()).getSymbol();
                emplace(inserted, Lsym);
                emplace(assigned, Lsym);
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
                    emplace(assigned, Lsym);
                }
            }
        }
    }

    void visit(ast::IfExp & e)
    {
        e.getThen().accept(*this);
        if (e.hasElse())
        {
            e.getElse().accept(*this);
        }
    }

    void visit(ast::WhileExp & e)
    {
        loops.push(&e);
        e.getBody().accept(*this);

        // pushAssigned pops loops
        pushAssigned();
    }

    void visit(ast::ForExp & e)
    {
        loops.push(&e);
        e.getVardec().accept(*this);
        e.getBody().accept(*this);
        pushAssigned();
    }

    void visit(ast::BreakExp & e)
    {
    }

    void visit(ast::ContinueExp & e)
    {
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
        e.getBody()->accept(*this);
    }

    void visit(ast::ReturnExp & e)
    {
    }

    void visit(ast::FieldExp & e)
    {
    }

    void visit(ast::NotExp & e)
    {
    }

    void visit(ast::TransposeExp & e)
    {
    }

    void visit(ast::MatrixExp & e)
    {
    }

    void visit(ast::MatrixLineExp & e)
    {
    }

    void visit(ast::CellExp & e)
    {
    }

    void visit(ast::SeqExp & e)
    {
        if (loops.empty())
        {
            for (auto _e : e.getExps())
            {
                if (_e->isForExp() || _e->isWhileExp())
                {
                    _e->accept(*this);
                }
            }
        }
        else
        {
            for (auto _e : e.getExps())
            {
                _e->accept(*this);
            }
        }
    }

    void visit(ast::ArrayListExp & e)
    {
    }

    void visit(ast::AssignListExp & e)
    {
    }

    void visit(ast::VarDec & e)
    {
        emplace(assigned, e.getSymbol());
    }

    void visit(ast::FunctionDec & e)
    {
    }

    void visit(ast::ListExp & e)
    {
    }

    void visit(ast::OptimizedExp & e)
    {
    }

    void visit(ast::MemfillExp & e)
    {
    }

    void visit(ast::DAXPYExp & e)
    {
    }

    void visit(ast::IntSelectExp & e)
    {
        visit(static_cast<ast::SelectExp &>(e));
    }

    void visit(ast::StringSelectExp & e)
    {
        visit(static_cast<ast::SelectExp &>(e));
    }
};

} // namespace analysis

#endif // __LOOP_ANALYZER_HXX__

