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

#include <set>
#include <memory>
#include <limits>

#include "visitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
namespace analysis
{

class AnalysisVisitor : public ast::ConstVisitor
{

    std::set<symbol::Symbol> SymRead;
    std::set<symbol::Symbol> SymWrite;

public:

    AnalysisVisitor() { }

    const std::set<symbol::Symbol> & get_read() const
    {
        return SymRead;
    }
    const std::set<symbol::Symbol> & get_write() const
    {
        return SymWrite;
    }

    bool is_read(const symbol::Symbol & s) const
    {
        std::set<symbol::Symbol>::const_iterator it = SymRead.find(s);

        return it != SymRead.end();
    }

    bool is_write(const symbol::Symbol & s) const
    {
        std::set<symbol::Symbol>::const_iterator it = SymWrite.find(s);

        return it != SymWrite.end();
    }

private:

    /*            inline void result_set( & val)
                  {
                  _result = val;
                  }

                  inline void result_set( && val)
                  {
                  _result = val;
                  }

                  inline  & result_get()
                  {
                  return  _result;
                  }
    */
    void visit(const ast::SimpleVar &e)
    {
        std::set<symbol::Symbol>::const_iterator it = SymWrite.find(e.name_get());
        if (it == SymWrite.end())
        {
            SymRead.insert(e.name_get());
        }
    }

    void visit(const ast::DollarVar &e)
    {

    }

    void visit(const ast::ColonVar &e)
    {

    }

    void visit(const ast::ArrayListVar &e)
    {

    }

    void visit(const ast::IntExp &e)
    {

    }

    void visit(const ast::FloatExp &e)
    {

    }

    void visit(const ast::DoubleExp &e)
    {

    }

    void visit(const ast::BoolExp &e)
    {

    }

    void visit(const ast::StringExp &e)
    {

    }

    void visit(const ast::CommentExp &e)
    {
        // ignored
    }

    void visit(const ast::NilExp &e)
    {

    }

    void visit(const ast::CallExp &e)
    {

    }

    void visit(const ast::CellCallExp &e)
    {

    }

    void visit(const ast::OpExp &e)
    {
        e.left_get().accept(*this);
        e.right_get().accept(*this);
    }

    void visit(const ast::LogicalOpExp &e)
    {
        e.left_get().accept(*this);
        e.right_get().accept(*this);
    }

    void visit(const ast::AssignExp &e)
    {
        if (e.left_exp_get().is_simple_var())
        {
            ast::SimpleVar & var = static_cast<ast::SimpleVar &>(e.left_exp_get());
            std::set<symbol::Symbol>::const_iterator it = SymRead.find(var.name_get());
            if (it != SymRead.end())
            {
                SymRead.erase(it);
            }

            SymWrite.insert(var.name_get());
        }
        else
        {
            // TODO: handle this case
        }

        e.right_exp_get().accept(*this);
    }

    void visit(const ast::IfExp &e)
    {

    }

    void visit(const ast::WhileExp &e)
    {

    }

    void visit(const ast::ForExp &e)
    {
        e.vardec_get().accept(*this);
        e.body_get().accept(*this);
    }

    void visit(const ast::BreakExp &e)
    {

    }

    void visit(const ast::ContinueExp &e)
    {

    }

    void visit(const ast::TryCatchExp &e)
    {

    }

    void visit(const ast::SelectExp &e)
    {

    }

    void visit(const ast::CaseExp &e)
    {

    }

    void visit(const ast::ReturnExp &e)
    {

    }

    void visit(const ast::FieldExp &e)
    {

    }

    void visit(const ast::NotExp &e)
    {

    }

    void visit(const ast::TransposeExp &e)
    {

    }

    void visit(const ast::MatrixExp &e)
    {

    }

    void visit(const ast::MatrixLineExp &e)
    {

    }

    void visit(const ast::CellExp &e)
    {

    }

    void visit(const ast::SeqExp &e)
    {
        for (std::list<ast::Exp *>::const_iterator i = e.exps_get().begin(), end = e.exps_get().end(); i != end; ++i)
        {
            (*i)->accept(*this);
        }
    }

    void visit(const ast::ArrayListExp &e)
    {

    }

    void visit(const ast::AssignListExp &e)
    {

    }

    void visit(const ast::VarDec &e)
    {
        std::set<symbol::Symbol>::const_iterator it = SymRead.find(e.name_get());
        if (it != SymRead.end())
        {
            SymRead.erase(it);
        }

        SymWrite.insert(e.name_get());
        e.init_get().accept(*this);
    }

    void visit(const ast::FunctionDec &e)
    {

    }

    void visit(const ast::ListExp &e)
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
            step = static_cast<const ast::DoubleExp &>(e.step_get()).value_get();
        }

        if (e.end_get().is_double_exp())
        {
            end = static_cast<const ast::DoubleExp &>(e.end_get()).value_get();
        }

        const_cast<ast::ListExp &>(e).set_values(start, step, end);
    }
};

} // namespace analysis

#endif // __ANALYSIS_VISITOR_HXX__
