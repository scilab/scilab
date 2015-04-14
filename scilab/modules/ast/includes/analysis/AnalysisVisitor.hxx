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

#ifndef __ANALYSIS_VISITOR_HXX__
#define __ANALYSIS_VISITOR_HXX__

#include <algorithm>
#include <limits>
#include <map>
#include <memory>
#include <vector>

#include "visitor.hxx"
#include "execvisitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "calls/CallAnalyzer.hxx"
#include "checkers/Checkers.hxx"
#include "Chrono.hxx"
#include "ForList.hxx"
#include "Result.hxx"
#include "SymInfo.hxx"
#include "Temporary.hxx"
#include "TIType.hxx"

#include "data/DataManager.hxx"
#include "data/PolymorphicMacroCache.hxx"
#include "gvn/ConstraintManager.hxx"
#include "dynlib_ast.h"

namespace analysis
{

class EXTERN_AST AnalysisVisitor : public ast::Visitor, public Chrono
{

public:

    typedef std::map<symbol::Symbol, SymInfo> MapSymInfo;
    typedef unordered_map<std::wstring, std::shared_ptr<CallAnalyzer>> MapSymCall;
    typedef std::vector<Call *> Calls;

private:

    MapSymInfo symsinfo;
    Result _result;
    Temporary temp;
    Calls allCalls;
    DataManager dm;
    PolymorphicMacroCache pmc;
    ConstraintManager cm;

    std::vector<Result> multipleLHS;

    static MapSymCall symscall;
    static MapSymCall initCalls();

public:

    static bool asDouble(ast::Exp & e, double & out);
    static bool isDoubleConstant(const ast::Exp & e);
    static bool asDoubleMatrix(ast::Exp & e, types::Double *& data);

    AnalysisVisitor()
    {
        start_chrono();
    }

    virtual ~AnalysisVisitor()
    {
        //std::cerr << "delete AnalysisVisitor" << std::endl;
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

    // Only for debug use
    inline void print_info()
    {
        stop_chrono();

        //std::wcout << getGVN() << std::endl << std::endl; function z=foo(x,y);z=argn(2);endfunction;jit("x=123;y=456;t=foo(x,y)")
        // function z=foo(x,y);[z,u]=argn(0);endfunction;jit("x=123;y=456;t=foo(x,y)")

        std::wcerr << L"Analysis: " << *static_cast<Chrono *>(this) << std::endl;
        //std::wcout << temp << std::endl;

        std::wcerr << dm << std::endl;

        std::wcerr << std::endl;
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

    inline const Temporary & getTemp() const
    {
        return temp;
    }

    inline Temporary & getTemp()
    {
        return temp;
    }

    inline const Calls & getCalls() const
    {
        return allCalls;
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

    template<typename T>
    inline void visitArguments(const std::wstring & name, const unsigned int lhs, const TIType & calltype, ast::CallExp & e, T && args)
    {
        std::vector<Result> resargs;
        std::vector<TIType> vargs;
        vargs.reserve(args.size());
        resargs.reserve(args.size());

        for (typename T::const_iterator i = args.begin(), end = args.end(); i != end; ++i)
        {
            if ((*i)->getDecorator().res.hasBeenVisited())
            {
                resargs.push_back((*i)->getDecorator().res);
                vargs.push_back((*i)->getDecorator().res.getType());
            }
            else
            {
                (*i)->accept(*this);
                resargs.push_back(getResult());
                vargs.push_back(getResult().getType());
            }
        }

        const symbol::Symbol & sym = static_cast<ast::SimpleVar &>(e.getName()).getSymbol();
        int tempId = -1;
        if (lhs > 1)
        {
            std::vector<TIType> types = dm.call(*this, lhs, sym, vargs, &e);
            multipleLHS.clear();
            multipleLHS.reserve(types.size());
            for (const auto & type : types)
            {
                multipleLHS.emplace_back(type);
            }
        }
        else
        {
            std::vector<TIType> out = dm.call(*this, lhs, sym, vargs, &e);
            if (lhs == 1)
            {
                e.getDecorator().res = Result(out[0], tempId);
                e.getDecorator().setCall(Call(calltype, name, vargs));
                setResult(e.getDecorator().res);
            }
        }


        /*TIType out = Checkers::check(name, vargs);
          int tempId = -1;

          if (true || (!out.isscalar() && args.size() == 1 && Checkers::isElementWise(name)))
          {
          Result & LR = resargs[0];
          TIType & LT = vargs[0];
          if (false && LR.istemp() && LT == out)
          {
          tempId = LR.getTempId();
          }
          else
          {
          tempId = temp.add(out);
          }
          }

          e.getDecorator().res = Result(out, tempId);
          e.getDecorator().setCall(Call(calltype, name, vargs));
          setResult(e.getDecorator().res);*/
    }

    inline Info & getSymInfo(const symbol::Symbol & sym)
    {
        return dm.getInfo(sym);
    }

private:

    inline void pushCall(Call * c)
    {
        if (c)
        {
            allCalls.push_back(c);
        }
    }

    void visit(ast::SimpleVar & e)
    {
        symbol::Symbol & sym = e.getSymbol();
        Info & info = dm.read(sym, &e);
        Result & res = e.getDecorator().setResult(info.type);
        res.setConstant(info.getConstant());
        setResult(res);
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
        const ast::exps_t & vars = e.getVars();
        for (auto var : vars)
        {
            var->accept(*this);
        }
    }

    void visit(ast::DoubleExp & e)
    {
        Result & res = e.getDecorator().setResult(TIType(dm.getGVN(), TIType::DOUBLE));
        res.getConstant().set(e.getValue());
        setResult(res);
    }

    void visit(ast::BoolExp & e)
    {
        Result & res = e.getDecorator().setResult(TIType(dm.getGVN(), TIType::BOOLEAN));
        res.getConstant().set(e.getValue());
        setResult(res);
    }

    void visit(ast::StringExp & e)
    {
        Result & res = e.getDecorator().setResult(TIType(dm.getGVN(), TIType::STRING));
        res.getConstant().set(&e.getValue());
        setResult(res);
    }

    void visit(ast::CommentExp & e)
    {
        // ignored
    }

    void visit(ast::NilExp & e)
    {
        // nothing to do
    }

    void visit(ast::CallExp & e, const unsigned int lhs)
    {
        if (e.getName().isSimpleVar())
        {
            ast::SimpleVar & var = static_cast<ast::SimpleVar &>(e.getName());
            symbol::Symbol & sym = var.getSymbol();
            const std::wstring & name = sym.getName();
            Info & info = getSymInfo(sym); // that put the sym in the current block !

            // Special analysis cases: size, zeros, ones, ...
            MapSymCall::iterator it = symscall.find(sym.getName());
            if (it != symscall.end() && it->second.get()->analyze(*this, lhs, e))
            {
                pushCall(e.getDecorator().getCall());
                return;
            }

            visitArguments(name, lhs, info.type, e, e.getArgs());
            pushCall(e.getDecorator().getCall());
        }
    }

    void visit(ast::CallExp & e)
    {
        visit(e, 1);
    }

    void visit(ast::CellCallExp & e)
    {
        visit(static_cast<ast::CallExp &>(e));
    }

    void visit(ast::OpExp & e)
    {
        e.getLeft().accept(*this);
        Result LR = getResult();
        e.getRight().accept(*this);
        Result & RR = getResult();
        TIType & LT = LR.getType();
        TIType & RT = RR.getType();
        TIType resT;
        int tempId = -1;

        switch (e.getOper())
        {
            case ast::OpExp::plus :
            case ast::OpExp::minus :
            case ast::OpExp::dottimes :
            {
                // TODO: check if the rules for addition and subtraction are the same
                // If a temp is LHS or RHS we could use it again to avoid a malloc
                // TODO: It should be ok for element-wise operations (check this assumption)
                resT = check_add(dm.getGVN(), LT, RT);
                if (resT.isUnknownDims())
                {
                    const bool ret = getCM().check(ConstraintManager::SAMEDIMS, LT.rows.getValue(), LT.cols.getValue(), RT.rows.getValue(), RT.cols.getValue());
                    if (ret)
                    {
                        resT = check_add(dm.getGVN(), LT, RT);
                    }
                    else
                    {
                        resT = check_add(dm.getGVN(), LT.asUnknownMatrix(), RT.asUnknownMatrix());
                    }
                }
                if (!resT.isscalar())
                {
                    if (LR.istemp() && LT == resT)
                    {
                        tempId = LR.getTempId();
                        temp.remove(RT, RR.getTempId());
                    }
                    else if (RR.istemp() && RT == resT)
                    {
                        tempId = RR.getTempId();
                        temp.remove(LT, LR.getTempId());
                    }
                    else
                    {
                        tempId = temp.add(resT);
                    }
                }
                break;
            }
            case ast::OpExp::times :
            {
                resT = check_times(dm.getGVN(), LT, RT);
                if (resT.isUnknownDims())
                {
                    const bool ret = getCM().check(ConstraintManager::EQUAL, LT.cols.getValue(), RT.rows.getValue());
                    if (ret)
                    {
                        resT = check_times(dm.getGVN(), LT, RT);
                    }
                    else
                    {
                        resT = check_times(dm.getGVN(), LT.asUnknownMatrix(), RT.asUnknownMatrix());
                    }
                }
                temp.remove(LT, LR.getTempId());
                temp.remove(RT, RR.getTempId());
                if (resT.isknown() && !resT.isscalar())
                {
                    tempId = temp.add(resT);
                }
                break;
            }
            case ast::OpExp::rdivide :
            {
                // multiplication is not commutative for matrice pxq
                resT = check_times(dm.getGVN(), LT, RT);
                temp.remove(LT, LR.getTempId());
                temp.remove(RT, RR.getTempId());
                if (resT.isknown() && !resT.isscalar())
                {
                    tempId = temp.add(resT);
                }
                break;
            }
            case ast::OpExp::krontimes :
            {
                resT = check_krontimes(dm.getGVN(), LT, RT);
                temp.remove(LT, LR.getTempId());
                temp.remove(RT, RR.getTempId());
                if (resT.isknown() && !resT.isscalar())
                {
                    tempId = temp.add(resT);
                }
                break;
            }
        }

        e.getDecorator().res = Result(resT, tempId);
        setResult(e.getDecorator().res);
    }

    void visit(ast::LogicalOpExp & e)
    {
        e.getLeft().accept(*this);
        e.getRight().accept(*this);
    }

    void visit(ast::AssignExp & e)
    {
        /*if (e.left_exp_get().is_simple_var())
          {
          ast::SimpleVar & var = static_cast<ast::SimpleVar &>(e.left_exp_get());
          symbol::Symbol & sym = var.name_get();

          e.right_exp_get().accept(*this);
          Result & RR = getResult();
          // Don't remove tmp... temp.remove(RR);
          var.getDecorator().res = RR;

          set_sym_use(sym, SymInfo::REPLACE);
          set_sym_type(sym, getResult().get_type());
          }
          elseg
          {
          // TODO: handle this case
          }*/

        if (e.getLeftExp().isSimpleVar())
        {
            ast::SimpleVar & var = static_cast<ast::SimpleVar &>(e.getLeftExp());
            symbol::Symbol & sym = var.getSymbol();

            if (e.getRightExp().isSimpleVar())
            {
                // We have a=b (so the data associated to b is shared with a)
                symbol::Symbol & symR = static_cast<ast::SimpleVar &>(e.getRightExp()).getSymbol();
                dm.share(sym, symR, getSymInfo(symR).getType(), &e);
            }
            else
            {
                // We have something like a=expression
                if (e.getRightExp().isCallExp())
                {
                    visit(static_cast<ast::CallExp &>(e.getRightExp()), /* LHS */ 1);
                }
                else
                {
                    e.getRightExp().accept(*this);
                }
                Result & RR = getResult();
                // Don't remove tmp... temp.remove(RR);
                var.getDecorator().res = RR;
                Info & info = dm.define(sym, RR.getType(), &e);
                double value;
                if (asDouble(e.getRightExp(), value) || RR.getConstant().getDblValue(value))
                {
                    info.getConstant().set(value);
                }
                if (GVN::Value * gvnValue = RR.getConstant().getGVNValue())
                {
                    info.getConstant().set(gvnValue);
                }
            }
        }
        else if (e.getLeftExp().isCallExp())
        {
            // We have something like a(12)=...
            ast::CallExp & ce = static_cast<ast::CallExp &>(e.getLeftExp());
            if (ce.getName().isSimpleVar())
            {
                symbol::Symbol & symL = static_cast<ast::SimpleVar &>(ce.getName()).getSymbol();
                e.getRightExp().accept(*this);
                Result & RR = getResult();
                ce.getDecorator().res = RR;
                dm.write(symL, RR.getType(), &e);
            }
        }
        else if (e.getLeftExp().isAssignListExp())
        {
            ast::AssignListExp & ale = static_cast<ast::AssignListExp &>(e.getLeftExp());
            if (e.getRightExp().isCallExp())
            {
                const ast::exps_t & exps = ale.getExps();
                visit(static_cast<ast::CallExp &>(e.getRightExp()), /* LHS */ static_cast<unsigned int>(exps.size()));
                std::vector<Result>::iterator j = multipleLHS.begin();
                for (const auto exp : exps)
                {
                    // TODO: handle fields...
                    if (exp->isSimpleVar() && j != multipleLHS.end())
                    {
                        ast::SimpleVar & var = *static_cast<ast::SimpleVar *>(exp);
                        symbol::Symbol & sym = var.getSymbol();
                        Info & info = dm.define(sym, j->getType(), exp);
                        info.setConstant(j->getConstant());
                        ++j;
                    }
                }
            }
        }
    }

    void visit(ast::IfExp & e)
    {
        dm.addBlock(Block::EXCLUSIVE, &e);
        // TODO: analyze the test, e.g. a=argn(2); if a==1....
        // When we analyze a macro call, argn(2) is known so we are able to take the good branch and skip the analysis
        // the others.
        // There is a lot of code with: rhs=argn(2) or if argn(2)==1 ... then...
        e.getTest().accept(*this);
        dm.addBlock(Block::NORMAL, &e.getThen());
        e.getThen().accept(*this);
        dm.finalizeBlock();
        dm.addBlock(Block::NORMAL, e.hasElse() ? &e.getElse() : nullptr);
        if (e.hasElse())
        {
            e.getElse().accept(*this);
        }
        dm.finalizeBlock();
        dm.finalizeBlock();
    }

    void visit(ast::WhileExp & e)
    {
        e.getTest().accept(*this);
        e.getBody().accept(*this);
    }

    void visit(ast::ForExp & e)
    {
        dm.addBlock(Block::LOOP, &e);
        e.getVardec().accept(*this);
        dm.addBlock(Block::NORMAL, &e.getBody());
        e.getBody().accept(*this);

        if (dm.requiresAnotherTrip())
        {
            std::cerr << "Invalid forexp: types or refcount are not the same before and after the loop" << std::endl;

            dm.finalizeBlock();
            dm.addBlock(Block::NORMAL, &e.getBody());
            e.getBody().accept(*this);
        }

        dm.finalizeBlock();
        dm.finalizeBlock();
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
        dm.addBlock(Block::EXCLUSIVE, &e);
        e.getSelect()->accept(*this);
        ast::exps_t cases = e.getCases();
        for (auto exp : cases)
        {
            dm.addBlock(Block::NORMAL, exp);
            exp->accept(*this);
            dm.finalizeBlock();
        }

        if (e.getDefaultCase())
        {
            dm.addBlock(Block::NORMAL, e.getDefaultCase());
            e.getDefaultCase()->accept(*this);
            dm.finalizeBlock();
        }
        dm.finalizeBlock();
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
        // a.b.c <=> (a.b).c where a.b is the head and c is the tail

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
        Result & res = getResult();
        const TIType & type = res.getType();
        e.getDecorator().res = Result(TIType(dm.getGVN(), type.type, type.cols, type.rows));
        setResult(e.getDecorator().res);
    }

    void visit(ast::MatrixExp & e);
    void visit(ast::MatrixLineExp & e) { }

    void visit(ast::CellExp & e)
    {
        visit(static_cast<ast::MatrixExp &>(e));
    }

    void visit(ast::SeqExp & e)
    {
        for (const auto exp : e.getExps())
        {
            if (exp->isCallExp())
            {
                visit(*static_cast<ast::CallExp *>(exp), /* LHS */ 0);
            }
            else
            {
                exp->accept(*this);
            }
        }
    }

    void visit(ast::ArrayListExp & e)
    {
        const ast::exps_t & exps = e.getExps();
        for (const auto exp : e.getExps())
        {
            exp->accept(*this);
        }
    }

    void visit(ast::AssignListExp & e)
    {
        visit(static_cast<ast::ArrayListExp &>(e));
    }

    void visit(ast::VarDec & e)
    {
        // VarDec is only used in For loop for iterator declaration
        const symbol::Symbol & sym = e.getSymbol();
        if (e.getInit().isListExp())
        {
            ast::ListExp & le = static_cast<ast::ListExp &>(e.getInit());
            double start, step, end;
            if (asDouble(le.getStart(), start) && asDouble(le.getStep(), step) && asDouble(le.getEnd(), end))
            {
                ForList64 fl(start, step, end);
                e.setListInfo(fl);
                dm.define(sym, fl.getType(), &e).isint = true;
                // No need to visit the list (it has been visited just before)
            }
            else
            {
                e.setListInfo(ForList64());
                le.accept(*this);
            }
        }
    }

    void visit(ast::FunctionDec & e)
    {
        /*e.args_get().accept(*this);
          e.returns_get().accept(*this);
          e.body_get().accept(*this);*/
        dm.macrodef(&e);
    }

    void visit(ast::ListExp & e);

    void visit(ast::OptimizedExp & e)
    {
    }

    void visit(ast::DAXPYExp & e)
    {
    }
};

} // namespace analysis

#endif // __ANALYSIS_VISITOR_HXX__
