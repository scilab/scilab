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

#ifndef __CONSTANT_VISITOR_HXX__
#define __CONSTANT_VISITOR_HXX__

#include <iostream>
#include <string>
#include <unordered_set>

#include "visitor.hxx"
#include "execvisitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "Chrono.hxx"

namespace analysis
{

class AnalysisVisitor;

class EXTERN_AST ConstantVisitor : public ast::Visitor, public Chrono
{

    bool _result;

    AnalysisVisitor * parent;
    ast::ExecVisitor exec;
    unsigned int lhs;
    const bool evalSymbols;

    static std::unordered_set<std::wstring> constants;

public:

    ConstantVisitor(AnalysisVisitor & _parent) : _result(false), parent(&_parent), exec(), lhs(1), evalSymbols(true)
    {
        start_chrono();
    }

    ConstantVisitor(const bool _evalSymbols = true) : _result(false), parent(nullptr), exec(), lhs(1), evalSymbols(_evalSymbols)
    {
        start_chrono();
    }

    virtual ~ConstantVisitor()
    {
    }


    virtual ConstantVisitor* clone()
    {
        if (parent)
        {
            return new ConstantVisitor(parent);
        }
        else
        {
            return new ConstantVisitor(evalSymbols);
        }
    }

    inline void print_info()
    {
        stop_chrono();
    }

    inline ast::ExecVisitor & getExec()
    {
        return exec;
    }

    inline void setLHS(const unsigned int _lhs)
    {
        lhs = _lhs;
    }

private:

    static std::unordered_set<std::wstring> init();

    inline void setResult(const bool val)
    {
        _result = val;
    }

public:

    inline bool getResult()
    {
        return _result;
    }

    void visit(ast::SimpleVar & e);
    void visit(ast::NotExp & e);
    void visit(ast::TransposeExp & e);
    void visit(ast::MatrixExp & e);
    void visit(ast::CellExp & e);
    void visit(ast::CallExp & e);
    void visit(ast::OpExp & e);
    void visit(ast::LogicalOpExp & e);
    void visit(ast::ListExp & e);
    void visit(ast::IfExp & e);
    void visit(ast::DollarVar & e);
    void visit(ast::ColonVar & e);
    void visit(ast::ArrayListVar & e);
    void visit(ast::DoubleExp & e);
    void visit(ast::BoolExp & e);
    void visit(ast::StringExp & e);
    void visit(ast::CommentExp & e);
    void visit(ast::NilExp & e);
    void visit(ast::CellCallExp & e);
    void visit(ast::AssignExp & e);
    void visit(ast::WhileExp & e);
    void visit(ast::ForExp & e);
    void visit(ast::BreakExp & e);
    void visit(ast::ContinueExp & e);
    void visit(ast::TryCatchExp & e);
    void visit(ast::SelectExp & e);
    void visit(ast::CaseExp & e);
    void visit(ast::ReturnExp & e);
    void visit(ast::FieldExp & e);
    void visit(ast::MatrixLineExp & e);
    void visit(ast::SeqExp & e);
    void visit(ast::ArrayListExp & e);
    void visit(ast::AssignListExp & e);
    void visit(ast::VarDec & e);
    void visit(ast::FunctionDec & e);
    void visit(ast::OptimizedExp & e);
    void visit(ast::MemfillExp & e);
    void visit(ast::DAXPYExp & e);
    void visit(ast::IntSelectExp & e);
    void visit(ast::StringSelectExp & e);

private:

    inline bool execAndReplace(ast::Exp & e)
    {
        try
        {
            e.accept(exec);
            types::InternalType * pIT = exec.getResult();
            exec.setResult(nullptr);
            ast::Exp * exp = pIT->getExp(e.getLocation());
            if (exp)
            {
                exp->setVerbose(e.isVerbose());
                e.replace(exp);
                return true;
            }
        }
        catch (const ast::ScilabException & /*se*/) { }

        return false;
    }
};

} // namespace analysis

#endif // __CONSTANT_VISITOR_HXX__
