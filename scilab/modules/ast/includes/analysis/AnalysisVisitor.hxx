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
#include <memory>
#include <limits>
#include <map>

#include "visitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "Checkers.hxx"
#include "ForList.hxx"
#include "Result.hxx"
#include "SymInfo.hxx"

namespace analysis
{

class AnalysisVisitor : public ast::Visitor
{

public:

    typedef std::map<symbol::Symbol, SymInfo> MapSymInfo;

private:

    MapSymInfo symsinfo;
    Result _result;
    unsigned int scalars_tmp[TIType::COUNT][2];
    unsigned int arrays_tmp[TIType::COUNT][2];

public:

    AnalysisVisitor()
    {
        std::fill(&scalars_tmp[0][0], &scalars_tmp[0][0] + TIType::COUNT * 2, 0);
        std::fill(&arrays_tmp[0][0], &arrays_tmp[0][0] + TIType::COUNT * 2, 0);
    }

    const MapSymInfo & get_infos() const
    {
        return symsinfo;
    }

    // Only for debug use
    inline void print_info() const
    {
        std::wcout << L"Scalars:" << std::endl;
        for (unsigned int i = 0; i < TIType::COUNT; ++i)
        {
            if (scalars_tmp[i][0] || scalars_tmp[i][1])
            {
                std::wcout << TIType((TIType::Type)i) << ": " << scalars_tmp[i][0] << L" and " << scalars_tmp[i][1] << std::endl;
            }
        }

        std::wcout << std::endl;

        std::wcout << L"Arrays:" << std::endl;
        for (unsigned int i = 0; i < TIType::COUNT; ++i)
        {
            if (arrays_tmp[i][0] || arrays_tmp[i][1])
            {
                std::wcout << TIType((TIType::Type)i) << ": " << arrays_tmp[i][0] << L" and " << arrays_tmp[i][1] << std::endl;
            }
        }

        std::wcout << std::endl;

        for (MapSymInfo::const_iterator i = symsinfo.begin(), end = symsinfo.end(); i != end; ++i)
        {
            std::wcout << i->first.name_get() << L" -> " << i->second << std::endl;
        }

        std::wcout << std::endl;
    }

private:

    inline void add_tmp(const TIType & t, const int n = 1, const bool scalar = false)
    {
        if (scalar)
        {
            scalars_tmp[t.type][0] += n;
            if (n > 0 && scalars_tmp[t.type][0] > scalars_tmp[t.type][1])
            {
                scalars_tmp[t.type][1] = scalars_tmp[t.type][0];
            }
        }
        else
        {
            arrays_tmp[t.type][0] += n;
            if (n > 0 && arrays_tmp[t.type][0] > arrays_tmp[t.type][1])
            {
                arrays_tmp[t.type][1] = arrays_tmp[t.type][0];
            }
        }
    }

    inline TIType get_ti(const symbol::Symbol & sym)
    {
        MapSymInfo::const_iterator i = symsinfo.find(sym);
        if (i != symsinfo.end())
        {
            return i->second.current_type;
        }

        types::InternalType * pIT = symbol::Context::getInstance()->get(sym);
        TIType t;
        if (pIT && pIT->isGenericType())
        {
            TIType::Type type;
            types::GenericType * pGT = static_cast<types::GenericType *>(pIT);
            switch (pIT->getType())
            {
                case types::InternalType::ScilabInt8 :
                    type = TIType::Type::INT8;
                    break;
                case types::InternalType::ScilabUInt8 :
                    type = TIType::Type::UINT8;
                    break;
                case types::InternalType::ScilabInt16 :
                    type = TIType::Type::INT16;
                    break;
                case types::InternalType::ScilabUInt16 :
                    type = TIType::Type::UINT16;
                    break;
                case types::InternalType::ScilabInt32 :
                    type = TIType::Type::INT32;
                    break;
                case types::InternalType::ScilabUInt32 :
                    type = TIType::Type::UINT32;
                    break;
                case types::InternalType::ScilabInt64 :
                    type = TIType::Type::INT64;
                    break;
                case types::InternalType::ScilabUInt64 :
                    type = TIType::Type::UINT64;
                    break;
                case types::InternalType::ScilabString :
                    type = TIType::Type::STRING;
                    break;
                case types::InternalType::ScilabDouble :
                {
                    types::Double * pDbl = static_cast<types::Double *>(pGT);
                    if (pDbl->isEmpty())
                    {
                        type = TIType::Type::EMPTY;
                    }
                    else if (pDbl->isComplex())
                    {
                        type = TIType::Type::COMPLEX;
                    }
                    else
                    {
                        type = TIType::Type::DOUBLE;
                    }
                    break;
                }
                case types::InternalType::ScilabBool :
                    type = TIType::Type::BOOLEAN;
                    break;
                case types::InternalType::ScilabPolynom :
                    type = TIType::Type::POLYNOMIAL;
                    break;
                case types::InternalType::ScilabSparse :
                    type = TIType::Type::SPARSE;
                    break;
                default :
                    type = TIType::Type::UNKNOWN;
            }

            t = TIType(type, pGT->getRows(), pGT->getCols());
        }
        else
        {
            t = TIType();
        }

        SymInfo si;
        si.current_type = t;
        symsinfo.emplace(sym, si);

        return t;
    }

    inline void set_sym_use(const symbol::Symbol & sym, SymInfo::Kind k)
    {
        MapSymInfo::iterator i = symsinfo.find(sym);
        if (i != symsinfo.end())
        {
            i->second.set(k);
        }
        else
        {
            symsinfo.emplace(sym, k);
        }
    }

    inline void set_sym_use(const symbol::Symbol & sym, SymInfo::Kind k1, SymInfo::Kind k2)
    {
        MapSymInfo::iterator i = symsinfo.find(sym);
        if (i != symsinfo.end())
        {
            i->second.set(k1, k2);
        }
        else
        {
            symsinfo.emplace(sym, SymInfo(k1, k2));
        }
    }

    inline void set_sym_type(const symbol::Symbol & sym, const TIType & t)
    {
        MapSymInfo::iterator i = symsinfo.find(sym);
        if (i != symsinfo.end())
        {
            i->second.current_type = t;
        }
        else
        {
            SymInfo si;
            si.current_type = t;
            symsinfo.emplace(sym, si);
        }
    }

    inline void result_set(Result & val)
    {
        _result = val;
    }

    inline void result_set(Result && val)
    {
        _result = val;
    }

    inline Result & result_get()
    {
        return _result;
    }

    void visit(ast::SimpleVar & e)
    {
        symbol::Symbol & sym = e.name_get();
        TIType typ = get_ti(sym);
        e.decorator_get().res = Result(typ, false);
        result_set(e.decorator_get().res);
        set_sym_use(e.name_get(), SymInfo::READ);
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
        const std::list<ast::Var *> & vars = e.vars_get();
        for (std::list<ast::Var *>::const_iterator i = vars.begin(), end = vars.end(); i != end ; ++i)
        {
            (*i)->accept(*this);
        }
    }

    void visit(ast::IntExp & e)
    {
        // nothing to do
    }

    void visit(ast::FloatExp & e)
    {
        // nothing to do
    }

    void visit(ast::DoubleExp & e)
    {
        e.decorator_get().res = Result(TIType(TIType::DOUBLE, 1, 1), false);
        result_set(e.decorator_get().res);
    }

    void visit(ast::BoolExp & e)
    {
        e.decorator_get().res = Result(TIType(TIType::BOOLEAN, 1, 1), false);
        result_set(e.decorator_get().res);
    }

    void visit(ast::StringExp & e)
    {
        e.decorator_get().res = Result(TIType(TIType::STRING, 1, 1), false);
        result_set(e.decorator_get().res);
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
        e.name_get().accept(*this);
        const std::list<ast::Exp *> & args = e.args_get();
        for (std::list<ast::Exp *>::const_iterator i = args.begin(), end = args.end(); i != end; ++i)
        {
            (*i)->accept(*this);
        }
    }

    void visit(ast::CellCallExp & e)
    {
        visit(static_cast<ast::CallExp &>(e));
    }

    void visit(ast::OpExp & e)
    {
        e.left_get().accept(*this);
        Result LR = result_get();
        e.right_get().accept(*this);
        Result & RR = result_get();
        const TIType & LT = LR.get_type();
        const TIType & RT = RR.get_type();
        TIType resT;
        bool allocTmp = false;

        // We can released the temp vars
        if (LR.istemp())
        {
            add_tmp(LT, -1);
        }
        if (RR.istemp())
        {
            add_tmp(RT, -1);
        }

        switch (e.oper_get())
        {
            case ast::OpExp::plus :
            case ast::OpExp::minus :
            case ast::OpExp::dottimes :
            {
                // TODO: check if the rules for addition and subtraction are the same
                resT = check_add(LT, RT);
                break;
            }
            case ast::OpExp::times :
            {
                // multiplication is not commutative for matrice pxq
                resT = check_times(LT, RT);
                break;
            }
        }

        if (resT.isknown() && !resT.isscalar())
        {
            // result is a matrix so we need a tmp
            add_tmp(resT.type, 1);
            allocTmp = true;
        }

        e.decorator_get().res = Result(resT, allocTmp);
        result_set(e.decorator_get().res);
    }

    void visit(ast::LogicalOpExp & e)
    {
        e.left_get().accept(*this);
        e.right_get().accept(*this);
    }

    void visit(ast::AssignExp & e)
    {
        if (e.left_exp_get().is_simple_var())
        {
            ast::SimpleVar & var = static_cast<ast::SimpleVar &>(e.left_exp_get());
            symbol::Symbol & sym = var.name_get();

            e.right_exp_get().accept(*this);
            var.decorator_get().res = result_get();

            set_sym_use(sym, SymInfo::REPLACE);
            set_sym_type(sym, result_get().get_type());
        }
        else
        {
            // TODO: handle this case
        }
    }

    void visit(ast::IfExp & e)
    {
        e.test_get().accept(*this);
        e.then_get().accept(*this);
        if (e.has_else())
        {
            e.else_get().accept(*this);
        }
    }

    void visit(ast::WhileExp & e)
    {
        e.test_get().accept(*this);
        e.body_get().accept(*this);
    }

    void visit(ast::ForExp & e)
    {
        e.vardec_get().accept(*this);
        e.body_get().accept(*this);
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
        e.try_get().accept(*this);
        e.catch_get().accept(*this);
    }

    void visit(ast::SelectExp & e)
    {
        e.select_get()->accept(*this);
        ast::cases_t * cases = e.cases_get();
        for (ast::cases_t::const_iterator i = cases->begin(), end = cases->end(); i != end; ++i)
        {
            (*i)->accept(*this);
        }
        e.default_case_get()->accept(*this);
    }

    void visit(ast::CaseExp & e)
    {
        e.test_get()->accept(*this);
        e.body_get()->accept(*this);
    }

    void visit(ast::ReturnExp & e)
    {
        e.exp_get().accept(*this);
    }

    void visit(ast::FieldExp & e)
    {
        // a.b.c <=> (a.b).c where a.b is the head and c is the tail

        //e.head_get()->accept(*this);
        //e.tail_get()->accept(*this);
    }

    void visit(ast::NotExp & e)
    {
        e.exp_get().accept(*this);
    }

    void visit(ast::TransposeExp & e)
    {
        e.exp_get().accept(*this);
    }

    void visit(ast::MatrixExp & e)
    {
        const std::list<ast::MatrixLineExp *> & lines = e.lines_get();
        for (std::list<ast::MatrixLineExp *>::const_iterator i = lines.begin(), end = lines.end(); i != end; ++i)
        {
            (*i)->accept(*this);
        }
    }

    void visit(ast::MatrixLineExp & e)
    {
        const std::list<ast::Exp *> & columns = e.columns_get();
        for (std::list<ast::Exp *>::const_iterator i = columns.begin(), end = columns.end(); i != end; ++i)
        {
            (*i)->accept(*this);
        }
    }

    void visit(ast::CellExp & e)
    {
        visit(static_cast<ast::MatrixExp &>(e));
    }

    void visit(ast::SeqExp & e)
    {
        for (std::list<ast::Exp *>::const_iterator i = e.exps_get().begin(), end = e.exps_get().end(); i != end; ++i)
        {
            (*i)->accept(*this);
        }
    }

    void visit(ast::ArrayListExp & e)
    {
        const std::list<ast::Exp *> & exps = e.exps_get();
        for (std::list<ast::Exp *>::const_iterator i = exps.begin(), end = exps.end(); i != end; ++i)
        {
            (*i)->accept(*this);
        }
    }

    void visit(ast::AssignListExp & e)
    {
        visit(static_cast<ast::ArrayListExp &>(e));
    }

    void visit(ast::VarDec & e)
    {
        // VarDec is only used in For loop for iterator declaration
        symbol::Symbol & sym = e.name_get();
        if (e.init_get().is_list_exp())
        {
            ast::ListExp & le = static_cast<ast::ListExp &>(e.init_get());
            if (le.start_get().is_double_exp() && le.step_get().is_double_exp() && le.end_get().is_double_exp())
            {
                ForList64 fl(static_cast<const ast::DoubleExp &>(le.start_get()).value_get(),
                             static_cast<const ast::DoubleExp &>(le.step_get()).value_get(),
                             static_cast<const ast::DoubleExp &>(le.end_get()).value_get());
                e.list_info_set(fl);
                set_sym_use(sym, SymInfo::REPLACE, SymInfo::FOR_IT);
                set_sym_type(sym, fl.get_type());
                // No need to visit the list (it has been visited just before)
            }
            else
            {
                e.list_info_set(ForList64());
                le.accept(*this);
            }
        }
    }

    void visit(ast::FunctionDec & e)
    {
        e.args_get().accept(*this);
        e.returns_get().accept(*this);
        e.body_get().accept(*this);
    }

    void visit(ast::ListExp & e)
    {
        double start = std::numeric_limits<double>::quiet_NaN();
        double step = std::numeric_limits<double>::quiet_NaN();
        double end = std::numeric_limits<double>::quiet_NaN();

        if (e.start_get().is_double_exp())
        {
            start = static_cast<const ast::DoubleExp &>(e.start_get()).value_get();
        }

        if (e.step_get().is_double_exp())
        {
            step = static_cast<ast::DoubleExp &>(e.step_get()).value_get();
        }

        if (e.end_get().is_double_exp())
        {
            end = static_cast<ast::DoubleExp &>(e.end_get()).value_get();
        }

        const_cast<ast::ListExp &>(e).set_values(start, step, end);
    }
};

} // namespace analysis

#endif // __ANALYSIS_VISITOR_HXX__
