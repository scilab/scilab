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

#include "SLintVisitor.hxx"

namespace slint
{

SLintVisitor * SLintVisitor::clone()
{
    return new SLintVisitor(*this);
}

void SLintVisitor::setOptions(SLintOptions & _options)
{
    options = _options;
}

const SLintOptions & SLintVisitor::getOptions() const
{
    return options;
}

SLintResult & SLintVisitor::getResult()
{
    return result;
}

void SLintVisitor::preCheckFile()
{
    SLintOptions::FileCheckers & fc = options.getFileCheckers();
    for (auto c : fc)
    {
        c->preCheckFile(context, result);
    }
}

void SLintVisitor::postCheckFile()
{
    SLintOptions::FileCheckers & fc = options.getFileCheckers();
    for (auto c : fc)
    {
        c->postCheckFile(context, result);
    }
}

const ast::Exp * SLintVisitor::getNextRelevantExp() const
{
    const ast::exps_t::const_iterator & seqExpEnd = seqExpIts.top().second;
    ast::exps_t::const_iterator it = std::next(seqExpIts.top().first);
    for (; it != seqExpEnd; ++it)
    {
        const ast::Exp * e = *it;
        if (!e->isCommentExp())
        {
            return e;
        }
    }
    return nullptr;
}

void SLintVisitor::visit(const ast::SeqExp & e)
{
    const ast::exps_t & exps = e.getExps();
    seqExpIts.emplace(exps.begin(), exps.end());
    auto range = preCheck(e);
    ast::exps_t::const_iterator & seqExpIt = seqExpIts.top().first;
    const ast::exps_t::const_iterator & seqExpEnd = seqExpIts.top().second;
    for (; seqExpIt != seqExpEnd; ++seqExpIt)
    {
        (*seqExpIt)->accept(*this);
    }
    postCheck(e, range);
    seqExpIts.pop();
}

void SLintVisitor::visit(const ast::IfExp & e)
{
    auto range = preCheck(e);
    e.getTest().accept(*this);
    e.getThen().accept(*this);
    if (e.hasElse())
    {
        e.getElse().accept(*this);
    }
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::WhileExp & e)
{
    context.pushLoop(&e);
    auto range = preCheck(e);
    e.getTest().accept(*this);
    e.getBody().accept(*this);
    postCheck(e, range);
    context.popLoop();
}

void SLintVisitor::visit(const ast::ForExp & e)
{
    context.pushLoop(&e);
    auto range = preCheck(e);
    e.getVardec().accept(*this);
    e.getBody().accept(*this);
    postCheck(e, range);
    context.popLoop();
}

void SLintVisitor::visit(const ast::TryCatchExp & e)
{
    auto range = preCheck(e);
    e.getTry().accept(*this);
    e.getCatch().accept(*this);
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::SelectExp & e)
{
    auto range = preCheck(e);
    e.getSelect()->accept(*this);

    ast::exps_t cases = e.getCases();
    for (auto exp : cases)
    {
        exp->accept(*this);
    }

    if (e.getDefaultCase() != NULL)
    {
        e.getDefaultCase()->accept(*this);
    }

    postCheck(e, range);
}

void SLintVisitor::visit(const ast::CaseExp & e)
{
    auto range = preCheck(e);
    e.getTest()->accept(*this);
    e.getBody()->accept(*this);
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::FunctionDec & e)
{
    context.pushFn(&e);
    auto range = preCheck(e);
    e.getBody().accept(*this);
    postCheck(e, range);

    context.popFn();
}

void SLintVisitor::visit(const ast::CallExp & e)
{
    auto range = preCheck(e);
    e.getName().accept(*this);

    ast::exps_t args = e.getArgs();
    for (auto arg : args)
    {
        arg->accept(*this);
    }
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::SimpleVar & e)
{
    auto range = preCheck(e);

    postCheck(e, range);
}

void SLintVisitor::visit(const ast::NotExp & e)
{
    auto range = preCheck(e);
    e.getExp().accept(*this);
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::TransposeExp & e)
{
    auto range = preCheck(e);
    e.getExp().accept(*this);
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::MatrixExp & e)
{
    auto range = preCheck(e);
    const ast::exps_t & lines = e.getLines();
    for (ast::exps_t::const_iterator it = lines.begin(), itEnd = lines.end(); it != itEnd ; ++it)
    {
        (*it)->accept(*this);
    }
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::CellExp & e)
{
    auto range = preCheck(e);
    ast::exps_t lines = e.getLines();
    for (ast::exps_t::const_iterator it = lines.begin(), itEnd = lines.end(); it != itEnd ; ++it)
    {
        (*it)->accept(*this);
    }
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::OpExp & e)
{
    auto range = preCheck(e);
    if (e.getOper() != ast::OpExp::unaryMinus)
    {
	e.getLeft().accept(*this);
    }
    e.getRight().accept(*this);
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::LogicalOpExp & e)
{
    auto range = preCheck(e);
    e.getLeft().accept(*this);
    e.getRight().accept(*this);
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::ListExp & e)
{
    auto range = preCheck(e);
    e.getStart().accept(*this);
    if (e.hasExplicitStep())
    {
        e.getStep().accept(*this);
    }
    e.getEnd().accept(*this);
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::DollarVar & e)
{
    auto range = preCheck(e);

    postCheck(e, range);
}

void SLintVisitor::visit(const ast::ColonVar & e)
{
    auto range = preCheck(e);

    postCheck(e, range);
}

void SLintVisitor::visit(const ast::ArrayListVar & e)
{
    auto range = preCheck(e);
    ast::exps_t vars = e.getVars();
    for (ast::exps_t::const_iterator it = vars.begin (), itEnd = vars.end(); it != itEnd; ++it)
    {
        (*it)->accept(*this);
    }
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::DoubleExp & e)
{
    auto range = preCheck(e);

    postCheck(e, range);
}

void SLintVisitor::visit(const ast::BoolExp & e)
{
    auto range = preCheck(e);

    postCheck(e, range);
}

void SLintVisitor::visit(const ast::StringExp & e)
{
    auto range = preCheck(e);

    postCheck(e, range);
}

void SLintVisitor::visit(const ast::CommentExp & e)
{
    auto range = preCheck(e);

    postCheck(e, range);
}

void SLintVisitor::visit(const ast::NilExp & e)
{
    auto range = preCheck(e);

    postCheck(e, range);
}

void SLintVisitor::visit(const ast::CellCallExp & e)
{
    auto range = preCheck(e);
    e.getName().accept(*this);

    ast::exps_t args = e.getArgs();
    for (auto arg : args)
    {
        arg->accept(*this);
    }
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::AssignExp & e)
{
    context.setLHSExp(&e.getLeftExp());
    auto range = preCheck(e);
    e.getRightExp().accept(*this);
    context.setLHSExp(&e.getLeftExp());
    e.getLeftExp().accept(*this);
    postCheck(e, range);
    context.setLHSExp(nullptr);
}

void SLintVisitor::visit(const ast::BreakExp & e)
{
    auto range = preCheck(e);

    postCheck(e, range);
}

void SLintVisitor::visit(const ast::ContinueExp & e)
{
    auto range = preCheck(e);

    postCheck(e, range);
}

void SLintVisitor::visit(const ast::ReturnExp & e)
{
    auto range = preCheck(e);
    if (!e.isGlobal())
    {
        e.getExp().accept(*this);
    }
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::FieldExp & e)
{
    auto range = preCheck(e);
    e.getHead()->accept(*this);
    e.getTail()->accept(*this);
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::MatrixLineExp & e)
{
    auto range = preCheck(e);
    const ast::exps_t & columns = e.getColumns();
    for (const auto column : columns)
    {
        column->accept(*this);
    }
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::ArrayListExp & e)
{
    auto range = preCheck(e);
    const ast::exps_t & exps = e.getExps();
    for (ast::exps_t::const_iterator it = exps.begin(), itEnd = exps.end(); it != itEnd; ++it)
    {
        (*it)->accept(*this);
    }
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::AssignListExp & e)
{
    auto range = preCheck(e);
    const ast::exps_t & exps = e.getExps();
    for (ast::exps_t::const_iterator it = exps.begin(), itEnd = exps.end(); it != itEnd; ++it)
    {
        (*it)->accept(*this);
    }
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::VarDec & e)
{
    auto range = preCheck(e);
    e.getInit().accept(*this);
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::OptimizedExp & e)
{
    auto range = preCheck(e);
    e.getOriginal()->accept(*this);
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::DAXPYExp & e)
{
    auto range = preCheck(e);
    e.getOriginal()->accept(*this);
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::IntSelectExp & e)
{
    auto range = preCheck(e);
    e.getOriginal()->accept(*this);
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::StringSelectExp & e)
{
    auto range = preCheck(e);
    e.getOriginal()->accept(*this);
    postCheck(e, range);
}

void SLintVisitor::visit(const ast::MemfillExp & e)
{
    auto range = preCheck(e);
    e.getOriginal()->accept(*this);
    postCheck(e, range);
}
}
