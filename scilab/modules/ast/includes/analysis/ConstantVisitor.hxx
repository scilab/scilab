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

    ConstantVisitor(AnalysisVisitor & _parent) : parent(&_parent), lhs(1), evalSymbols(true)
    {
        start_chrono();
    }

    ConstantVisitor(const bool _evalSymbols = true) : parent(nullptr), evalSymbols(_evalSymbols)
    {
        start_chrono();
    }

    virtual ~ConstantVisitor()
    {
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

    inline static std::unordered_set<std::wstring> init()
    {
        std::unordered_set<std::wstring> _constants;
        _constants.emplace(L"%pi");
        _constants.emplace(L"%eps");
        _constants.emplace(L"%e");
        _constants.emplace(L"%i");
        _constants.emplace(L"%nan");
        _constants.emplace(L"%inf");
        _constants.emplace(L"%t");
        _constants.emplace(L"%f");
        _constants.emplace(L"%T");
        _constants.emplace(L"%F");
        _constants.emplace(L"SCI");
        _constants.emplace(L"WSCI");
        _constants.emplace(L"SCIHOME");
        _constants.emplace(L"TMPDIR");

        return _constants;
    }

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

    void visit(ast::IfExp & e)
    {
    }

    void visit(ast::DollarVar & e)
    {
        setResult(false);
    }

    void visit(ast::ColonVar & e)
    {
        setResult(false);
    }

    void visit(ast::ArrayListVar & e)
    {
    }

    void visit(ast::DoubleExp & e)
    {
        setResult(true);
    }

    void visit(ast::BoolExp & e)
    {
        setResult(true);
    }

    void visit(ast::StringExp & e)
    {
        setResult(true);
    }

    void visit(ast::CommentExp & e)
    {
        // ignored
    }

    void visit(ast::NilExp & e)
    {
        // nothing to do
    }

    void visit(ast::CellCallExp & e)
    {
        setResult(false);
    }

    void visit(ast::AssignExp & e)
    {
    }

    void visit(ast::WhileExp & e)
    {
    }

    void visit(ast::ForExp & e)
    {
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
    }

    void visit(ast::SelectExp & e)
    {
    }

    void visit(ast::CaseExp & e)
    {
    }

    void visit(ast::ReturnExp & e)
    {
    }

    void visit(ast::FieldExp & e)
    {
    }

    void visit(ast::MatrixLineExp & e)
    {
    }

    void visit(ast::SeqExp & e)
    {
    }

    void visit(ast::ArrayListExp & e)
    {
    }

    void visit(ast::AssignListExp & e)
    {
    }

    void visit(ast::VarDec & e)
    {
    }

    void visit(ast::FunctionDec & e)
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
    }

    void visit(ast::StringSelectExp & e)
    {
    }

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
