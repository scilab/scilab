/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __ANALYSIS_VISITOR_HXX__
#define __ANALYSIS_VISITOR_HXX__

#include <algorithm>
#include <limits>
#include <map>
#include <memory>
#include <stack>
#include <vector>

#include "visitor.hxx"
#include "prettyprintvisitor.hxx"
#include "execvisitor.hxx"
#include "printvisitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "analyzers/CallAnalyzer.hxx"
#include "checkers/Checkers.hxx"
#include "Chrono.hxx"
#include "DollarInfo.hxx"
#include "ForList.hxx"
#include "Result.hxx"
#include "TIType.hxx"
#include "ConstantVisitor.hxx"
#include "gvn/SymbolicList.hxx"
#include "FBlockEmittedListener.hxx"
#include "data/DataManager.hxx"
#include "data/PolymorphicMacroCache.hxx"
#include "gvn/ConstraintManager.hxx"
#include "logging/Logger.hxx"
#include "dynlib_ast.h"

namespace analysis
{

class EXTERN_AST AnalysisVisitor : public ast::Visitor, public Chrono
{

public:

    typedef std::unordered_map<std::wstring, std::shared_ptr<CallAnalyzer>> MapSymCall;

private:

    Result _result;
    DataManager dm;
    PolymorphicMacroCache pmc;
    ConstraintManager cm;
    ConstantVisitor cv;
    ast::PrettyPrintVisitor dv;
    ast::PrintVisitor pv;
    std::vector<Result> multipleLHS;
    logging::Logger logger;
    std::vector<FBlockEmittedListener *> fblockListeners;
    std::stack<ast::Exp *> loops;
    std::stack<DollarInfo> argIndices;

    static MapSymCall symscall;
    static MapSymCall initCalls();

public:

    static bool asDouble(ast::Exp & e, double & out);
    static bool asDouble(types::InternalType * pIT, double & out);
    static bool isDoubleConstant(const ast::Exp & e);
    static bool asDoubleMatrix(ast::Exp & e, types::Double *& data);
    static void analyze(ast::SelectExp & e);

    AnalysisVisitor();
    virtual ~AnalysisVisitor();

    virtual AnalysisVisitor* clone()
    {
        return new AnalysisVisitor();
    }

    inline CallAnalyzer * getAnalyzer(const symbol::Symbol & sym)
    {
        MapSymCall::iterator it = symscall.find(sym.getName());
        if (it == symscall.end())
        {
            return nullptr;
        }
        else
        {
            return it->second.get();
        }
    }

    inline DataManager & getDM()
    {
        return dm;
    }

    inline GVN & getGVN()
    {
        return dm.getGVN();
    }

    inline PolymorphicMacroCache & getPMC()
    {
        return pmc;
    }

    inline ConstantVisitor & getCV()
    {
        return cv;
    }

    inline ast::PrintVisitor & getPV()
    {
        return pv;
    }

    inline ast::PrettyPrintVisitor & getDV()
    {
        return dv;
    }

    inline ast::ExecVisitor & getExec()
    {
        return cv.getExec();
    }

    inline void finalize()
    {
        //dm.finalize(nullptr);
    }

    inline void setResult(Result & val)
    {
        _result = val;
    }

    inline void setResult(Result && val)
    {
        _result = val;
    }

    inline Result & getResult()
    {
        return _result;
    }

    inline std::vector<Result> & getLHSContainer()
    {
        return multipleLHS;
    }

    inline ConstraintManager & getCM()
    {
        if (FunctionBlock * fblock = getDM().topFunction())
        {
            return fblock->getConstraintManager();
        }
        else
        {
            return cm;
        }
    }

    inline ast::Exp * getCurrentLoop() const
    {
        if (!loops.empty())
        {
            return loops.top();
        }

        return nullptr;
    }

    inline void registerFBlockEmittedListener(FBlockEmittedListener * listener)
    {
        if (listener)
        {
            fblockListeners.push_back(listener);
        }
    }

    inline void emitFunctionBlock(FunctionBlock & fblock)
    {
        for (auto listener : fblockListeners)
        {
            listener->action(fblock);
        }
    }

    inline Info & getSymInfo(const symbol::Symbol & sym)
    {
        return dm.getInfo(sym);
    }

    void reset();
    bool analyzeIndices(TIType & type, ast::CallExp & ce);

    // Only for debug use
    void print_info();
    logging::Logger & getLogger();

private:

    bool getDimension(SymbolicDimension & dim, ast::Exp & arg, bool & safe, SymbolicDimension & out);

    /*
       Workaround for a C++11 bug with Intel compiler
       https://software.intel.com/fr-fr/forums/topic/514793
    */
    inline static TIType _check_plus(GVN & gvn, const TIType & Ltype, const TIType & Rtype)
    {
        return Checkers::check_____add____(gvn, Ltype, Rtype);
    }

    inline static TIType _check_minus(GVN & gvn, const TIType & Ltype, const TIType & Rtype)
    {
        return Checkers::check_____sub____(gvn, Ltype, Rtype);
    }

    inline static TIType _check_dottimes(GVN & gvn, const TIType & Ltype, const TIType & Rtype)
    {
        return Checkers::check_____dottimes____(gvn, Ltype, Rtype);
    }

    inline static TIType _check_dotrdiv(GVN & gvn, const TIType & Ltype, const TIType & Rtype)
    {
        return Checkers::check_____dotrdiv____(gvn, Ltype, Rtype);
    }

    inline static TIType _check_dotpower(GVN & gvn, const TIType & Ltype, const TIType & Rtype)
    {
        return Checkers::check_____dotpower____(gvn, Ltype, Rtype);
    }

    inline static TIType _check_eq(GVN & gvn, const TIType & Ltype, const TIType & Rtype)
    {
        return Checkers::check_____eq____(gvn, Ltype, Rtype);
    }

    inline static TIType _check_neq(GVN & gvn, const TIType & Ltype, const TIType & Rtype)
    {
        return Checkers::check_____neq____(gvn, Ltype, Rtype);
    }

    inline static TIType _check_lt(GVN & gvn, const TIType & Ltype, const TIType & Rtype)
    {
        return Checkers::check_____lt____(gvn, Ltype, Rtype);
    }

    inline static TIType _check_le(GVN & gvn, const TIType & Ltype, const TIType & Rtype)
    {
        return Checkers::check_____le____(gvn, Ltype, Rtype);
    }

    inline static TIType _check_gt(GVN & gvn, const TIType & Ltype, const TIType & Rtype)
    {
        return Checkers::check_____gt____(gvn, Ltype, Rtype);
    }

    inline static TIType _check_ge(GVN & gvn, const TIType & Ltype, const TIType & Rtype)
    {
        return Checkers::check_____ge____(gvn, Ltype, Rtype);
    }

    inline static TIType _check_and(GVN & gvn, const TIType & Ltype, const TIType & Rtype)
    {
        return Checkers::check_____and____(gvn, Ltype, Rtype);
    }

    inline static TIType _check_or(GVN & gvn, const TIType & Ltype, const TIType & Rtype)
    {
        return Checkers::check_____or____(gvn, Ltype, Rtype);
    }

    inline static TIType _check_andand(GVN & gvn, const TIType & Ltype, const TIType & Rtype)
    {
        return Checkers::check_____and____(gvn, Ltype, Rtype);
    }

    inline static TIType _check_oror(GVN & gvn, const TIType & Ltype, const TIType & Rtype)
    {
        return Checkers::check_____or____(gvn, Ltype, Rtype);
    }

    template<TIType (F)(GVN &, const TIType &, const TIType &)>
    inline TIType checkEWBinOp(TIType & LT, TIType & RT, const Result & LR, const Result & RR, bool & safe, int & tempId, ast::Exp * Lexp, ast::Exp * Rexp)
    {
        TIType resT = F(getGVN(), LT, RT);
        if (resT.hasInvalidDims())
        {
            const bool ret = getCM().check(ConstraintManager::SAMEDIMS, LT.rows.getValue(), LT.cols.getValue(), RT.rows.getValue(), RT.cols.getValue());

            if (ret)
            {
                resT = F(getGVN(), LT, RT);
                safe = true;
            }
            else
            {
                resT = resT.asUnknownMatrix();
            }
        }
        else
        {
            safe = true;
        }

        tempId = getTmpIdForEWOp(resT, LR, RR, Lexp, Rexp);

        if (resT.isscalar())
        {
            // TODO
        }

        return resT;
    }

    bool operGVNValues(ast::OpExp & oe);
    bool operSymbolicRange(ast::OpExp & oe);

    // get temp id for an element-wise operation
    // A + (B + 1) => B+1 is a temp, A is not and we can reuse the temp to put the result of A + (B+1)
    int getTmpIdForEWOp(const TIType & resT, const Result & LR, const Result & RR, ast::Exp * Lexp, ast::Exp * Rexp);
    void visitArguments(const std::wstring & name, const unsigned int lhs, const TIType & calltype, ast::CallExp & e, const ast::exps_t & args);

    void visit(ast::SelectExp & e);
    void visit(ast::ListExp & e);
    void visitInVarDecCtxt(ast::ListExp & e);
    void visit(ast::MatrixExp & e);
    void visit(ast::OpExp & e);
    void visit(ast::NotExp & e);
    void visit(ast::TransposeExp & e);
    void visit(ast::AssignExp & e);
    void visit(ast::IfExp & e);
    void visit(ast::ForExp & e);
    void visit(ast::CallExp & e);
    void visit(ast::CallExp & e, const unsigned int lhs);
    void visit(ast::SeqExp & e);
    void visit(ast::DoubleExp & e);
    void visit(ast::BoolExp & e);
    void visit(ast::StringExp & e);
    void visit(ast::SimpleVar & e);
    void visit(ast::DollarVar & e);
    void visit(ast::VarDec & e);

    void visit(ast::MatrixLineExp & e)
    {
        /* treated in MatrixExp */
    }
    void visit(ast::OptimizedExp & e) { }
    void visit(ast::MemfillExp & e) { }
    void visit(ast::DAXPYExp & e) { }
    void visit(ast::IntSelectExp & e) { }
    void visit(ast::StringSelectExp & e) { }
    void visit(ast::CommentExp & e) { }
    void visit(ast::NilExp & e) { }
    void visit(ast::ColonVar & e) { }
    void visit(ast::WhileExp & e);

    void visit(ast::ArrayListVar & e)
    {
        logger.log(L"ArrayListVar", e.getLocation());
        const ast::exps_t & vars = e.getVars();
        for (auto var : vars)
        {
            var->accept(*this);
        }
    }

    void visit(ast::CellCallExp & e)
    {
        logger.log(L"CellCallExp", e.getLocation());
        visit(static_cast<ast::CallExp &>(e));
    }

    void visit(ast::LogicalOpExp & e)
    {
        logger.log(L"LogicalOpExp", e.getLocation());
        visit(static_cast<ast::OpExp &>(e));
    }

    void visit(ast::BreakExp & e)
    {
        logger.log(L"BreakExp", e.getLocation());
        // nothing to do
    }

    void visit(ast::ContinueExp & e)
    {
        logger.log(L"ContinueExp", e.getLocation());
        // nothing to do
    }

    void visit(ast::TryCatchExp & e)
    {
        logger.log(L"TryCatchExp", e.getLocation());
        e.getTry().accept(*this);
        e.getCatch().accept(*this);
    }

    void visit(ast::CaseExp & e)
    {
        logger.log(L"CaseExp", e.getLocation());
        e.getTest()->accept(*this);
        e.getBody()->accept(*this);
    }

    void visit(ast::ReturnExp & e)
    {
        logger.log(L"ReturnExp", e.getLocation());
        getDM().getCurrent()->setReturn(true);
        // Bug with return;
        //e.exp_get().accept(*this);

    }

    void visit(ast::FieldExp & e)
    {
        logger.log(L"FieldExp", e.getLocation());
        // a.b.c <=> (a.b).c where a.b is the head and c is the tail

        //e.head_get()->accept(*this);
        //e.tail_get()->accept(*this);
    }

    void visit(ast::CellExp & e)
    {
        logger.log(L"CellExp", e.getLocation());
        visit(static_cast<ast::MatrixExp &>(e));
    }

    void visit(ast::ArrayListExp & e)
    {
        logger.log(L"ArrayListExp", e.getLocation());
        const ast::exps_t & exps = e.getExps();
        for (const auto exp : e.getExps())
        {
            exp->accept(*this);
        }
    }

    void visit(ast::AssignListExp & e)
    {
        logger.log(L"AssignListExp", e.getLocation());
        visit(static_cast<ast::ArrayListExp &>(e));
    }

    void visit(ast::FunctionDec & e)
    {
        /*e.args_get().accept(*this);
          e.returns_get().accept(*this);
          e.body_get().accept(*this);*/
        logger.log(L"FunctionDec", e.getLocation());
        dm.macrodef(&e);
    }
};

} // namespace analysis

#endif // __ANALYSIS_VISITOR_HXX__
