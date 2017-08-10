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

#ifndef __SLINT_VISITOR_HXX__
#define __SLINT_VISITOR_HXX__

#include <stack>

#include "visitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "alldec.hxx"

#include "SLintContext.hxx"
#include "SLintOptions.hxx"

namespace slint
{

class SLintVisitor : public ast::ConstVisitor
{

    SLintContext & context;
    SLintResult & result;
    SLintOptions options;
    bool rhs;
    std::stack<std::pair<ast::exps_t::const_iterator, ast::exps_t::const_iterator>> seqExpIts;

public:

    SLintVisitor(SLintContext & _context, SLintResult & _result) : context(_context), result(_result), rhs(true) { }
    SLintVisitor(const SLintVisitor & slv) : context(slv.context), result(slv.result), options(slv.options), rhs(slv.rhs) { }
    ~SLintVisitor() { }

    void setOptions(SLintOptions & _options);
    const SLintOptions & getOptions() const;
    SLintResult & getResult();
    SLintVisitor * clone();
    void preCheckFile();
    void postCheckFile();
    const ast::Exp * getNextRelevantExp() const;

private:

    inline SLintOptions::range preCheck(const ast::Exp & e)
    {
        SLintOptions::MapCheckers & checkers = options.getCheckers();
        SLintOptions::range range = checkers.equal_range(e.getType());

        std::for_each(range.first, range.second, [&](SLintOptions::value_type & checker)
        {
            checker.second->preCheckNode(e, context, result);
        });

        return range;
    }

    inline void postCheck(const ast::Exp & e, SLintOptions::range & range)
    {
        SLintOptions::MapCheckers & checkers = options.getCheckers();
        std::for_each(range.first, range.second, [&](SLintOptions::value_type & checker)
        {
            checker.second->postCheckNode(e, context, result);
        });
    }

    void visit(const ast::SeqExp & e);
    void visit(const ast::IfExp & e);
    void visit(const ast::WhileExp & e);
    void visit(const ast::ForExp & e);
    void visit(const ast::TryCatchExp & e);
    void visit(const ast::SelectExp & e);
    void visit(const ast::CaseExp & e);
    void visit(const ast::FunctionDec & e);
    void visit(const ast::CallExp & e);
    void visit(const ast::SimpleVar & e);
    void visit(const ast::NotExp & e);
    void visit(const ast::TransposeExp & e);
    void visit(const ast::MatrixExp & e);
    void visit(const ast::CellExp & e);
    void visit(const ast::OpExp & e);
    void visit(const ast::LogicalOpExp & e);
    void visit(const ast::ListExp & e);
    void visit(const ast::DollarVar & e);
    void visit(const ast::ColonVar & e);
    void visit(const ast::ArrayListVar & e);
    void visit(const ast::DoubleExp & e);
    void visit(const ast::BoolExp & e);
    void visit(const ast::StringExp & e);
    void visit(const ast::CommentExp & e);
    void visit(const ast::NilExp & e);
    void visit(const ast::CellCallExp & e);
    void visit(const ast::AssignExp & e);
    void visit(const ast::BreakExp & e);
    void visit(const ast::ContinueExp & e);
    void visit(const ast::ReturnExp & e);
    void visit(const ast::FieldExp & e);
    void visit(const ast::MatrixLineExp & e);
    void visit(const ast::ArrayListExp & e);
    void visit(const ast::AssignListExp & e);
    void visit(const ast::VarDec & e);
    void visit(const ast::OptimizedExp & e);
    void visit(const ast::DAXPYExp & e);
    void visit(const ast::IntSelectExp &e);
    void visit(const ast::StringSelectExp &e);
    void visit(const ast::MemfillExp &e);
};

} // namespace slint

#endif // __SLINT_VISITOR_HXX__
