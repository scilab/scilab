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

    struct __Info
    {
        tools::SymbolSet assigned;
        tools::SymbolSet inserted;
        tools::SymbolSet shared;
        tools::SymbolSet used;

        __Info() { }
    };

    std::unordered_map<ast::Exp *, __Info> info;

    std::stack<std::pair<ast::Exp *, __Info *>> loops;
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

    virtual LoopAnalyzer* clone()
    {
        return new LoopAnalyzer(seq->clone());
    }


    inline bool isAssigned(ast::Exp * e, const symbol::Symbol & sym) const
    {
        if (e)
        {
            const auto i = info.find(e);
            if (i != info.end())
            {
                return i->second.assigned.find(sym) != i->second.assigned.end();
            }
        }
        return false;
    }

    inline const tools::SymbolSet * getInserted(ast::Exp * e) const
    {
        if (e)
        {
            const auto i = info.find(e);
            if (i != info.end())
            {
                return &i->second.inserted;
            }
        }
        return nullptr;
    }

    inline const tools::SymbolSet * getShared(ast::Exp * e) const
    {
        if (e)
        {
            const auto i = info.find(e);
            if (i != info.end())
            {
                return &i->second.shared;
            }
        }
        return nullptr;
    }

    friend std::wostream & operator<<(std::wostream & out, const LoopAnalyzer & la)
    {
        if (!la.info.empty())
        {
            std::wostringstream wos_ass, wos_ins, wos_sh, wos_used;
            for (const auto & p : la.info)
            {
                wos_ass << L"  -Loop at " << p.first->getLocation().getLocationString() << L": ";
                tools::printSet(p.second.assigned, wos_ass);
                wos_ass << L"\n";

                wos_ins << L"  -Loop at " << p.first->getLocation().getLocationString() << L": ";
                tools::printSet(p.second.inserted, wos_ins);
                wos_ins << L"\n";

                wos_sh << L"  -Loop at " << p.first->getLocation().getLocationString() << L": ";
                tools::printSet(p.second.inserted, wos_sh);
                wos_sh << L"\n";

                wos_used << L"  -Loop at " << p.first->getLocation().getLocationString() << L": ";
                tools::printSet(p.second.inserted, wos_used);
                wos_used << L"\n";
            }

            std::wstring str = wos_ass.str();
            if (!str.empty())
            {
                out << L" Assigned:\n";
                out << str;
            }
            str = wos_ins.str();
            if (!str.empty())
            {
                out << L" Inserted:\n";
                out << str;
            }
            str = wos_sh.str();
            if (!str.empty())
            {
                out << L" Shared:\n";
                out << str;
            }
            str = wos_used.str();
            if (!str.empty())
            {
                out << L" Used:\n";
                out << str;
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

    inline void emplaceAssigned(const symbol::Symbol & sym)
    {
        loops.top().second->assigned.emplace(sym);
    }

    inline void emplaceInserted(const symbol::Symbol & sym)
    {
        loops.top().second->inserted.emplace(sym);
    }

    inline void emplaceShared(const symbol::Symbol & sym)
    {
        loops.top().second->shared.emplace(sym);
    }

    inline void emplaceUsed(const symbol::Symbol & sym)
    {
        loops.top().second->used.emplace(sym);
    }

    inline void push(ast::Exp & e)
    {
        __Info * i = &(info.emplace(&e, __Info()).first->second);
        loops.push({ &e, i });
    }

    inline void pushAssigned()
    {
        std::pair<ast::Exp *, __Info *> last = loops.top();
        loops.pop();

        if (!loops.empty() && !last.second->assigned.empty())
        {
            std::pair<ast::Exp *, __Info *> & penult = loops.top();
            penult.second->assigned.insert(last.second->assigned.begin(), last.second->assigned.end());
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
            emplaceAssigned(Lsym);
            if (e.getRightExp().isSimpleVar())
            {
                const symbol::Symbol & Rsym = static_cast<ast::SimpleVar &>(e.getRightExp()).getSymbol();
                emplaceShared(Lsym);
                emplaceShared(Rsym);
                emplaceUsed(Rsym);
            }
        }
        else if (e.getLeftExp().isCallExp())
        {
            // A(...) = ...
            ast::CallExp & ce = static_cast<ast::CallExp &>(e.getLeftExp());
            if (ce.getName().isSimpleVar())
            {
                const symbol::Symbol & Lsym = static_cast<ast::SimpleVar &>(ce.getName()).getSymbol();
                emplaceInserted(Lsym);
                emplaceAssigned(Lsym);
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
                    emplaceAssigned(Lsym);
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
        push(e);
        e.getBody().accept(*this);

        // pushAssigned pops loops
        pushAssigned();
    }

    void visit(ast::ForExp & e)
    {
        push(e);
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
        emplaceAssigned(e.getSymbol());
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

