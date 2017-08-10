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

#include "checkers/McCabeVisitor.hxx"

namespace slint
{

unsigned int McCabeVisitor::getComplexity(const ast::FunctionDec & fd)
{
    complexity = 1;
    fd.getBody().accept(*this);

    return complexity;
}

void McCabeVisitor::visit(const ast::IfExp & e)
{
    ++complexity;
    ast::DummyVisitor::visit(e);
}

void McCabeVisitor::visit(const ast::WhileExp & e)
{
    ++complexity;
    ast::DummyVisitor::visit(e);
}

void McCabeVisitor::visit(const ast::ForExp & e)
{
    ++complexity;
    ast::DummyVisitor::visit(e);
}

void McCabeVisitor::visit(const ast::TryCatchExp & e)
{
    ++complexity;
    ast::DummyVisitor::visit(e);
}

void McCabeVisitor::visit(const ast::SelectExp & e)
{
    complexity += static_cast<unsigned int>(e.getCases().size()) - 1;
    if (e.hasDefault())
    {
        ++complexity;
    }
    ast::DummyVisitor::visit(e);
}

void McCabeVisitor::visit(const ast::ReturnExp & e)
{
    ++complexity;
}

void McCabeVisitor::visit(const ast::SeqExp & e)
{
    ast::DummyVisitor::visit(e);
}

void McCabeVisitor::visit(const ast::CaseExp & e)
{
    ast::DummyVisitor::visit(e);
}

void McCabeVisitor::visit(const ast::FunctionDec & e)
{
}

void McCabeVisitor::visit(const ast::CallExp & e)
{
}

void McCabeVisitor::visit(const ast::SimpleVar & e)
{
}

void McCabeVisitor::visit(const ast::NotExp & e)
{
}

void McCabeVisitor::visit(const ast::TransposeExp & e)
{
}

void McCabeVisitor::visit(const ast::MatrixExp & e)
{
}

void McCabeVisitor::visit(const ast::CellExp & e)
{
}

void McCabeVisitor::visit(const ast::OpExp & e)
{
}

void McCabeVisitor::visit(const ast::LogicalOpExp & e)
{
}

void McCabeVisitor::visit(const ast::ListExp & e)
{
}

void McCabeVisitor::visit(const ast::DollarVar & e)
{
}

void McCabeVisitor::visit(const ast::ColonVar & e)
{
}

void McCabeVisitor::visit(const ast::ArrayListVar & e)
{
}

void McCabeVisitor::visit(const ast::DoubleExp & e)
{
}

void McCabeVisitor::visit(const ast::BoolExp & e)
{
}

void McCabeVisitor::visit(const ast::StringExp & e)
{
}

void McCabeVisitor::visit(const ast::CommentExp & e)
{
}

void McCabeVisitor::visit(const ast::NilExp & e)
{
}

void McCabeVisitor::visit(const ast::CellCallExp & e)
{
}

void McCabeVisitor::visit(const ast::AssignExp & e)
{
}

void McCabeVisitor::visit(const ast::BreakExp & e)
{
}

void McCabeVisitor::visit(const ast::ContinueExp & e)
{
}

void McCabeVisitor::visit(const ast::FieldExp & e)
{
}

void McCabeVisitor::visit(const ast::MatrixLineExp & e)
{
}

void McCabeVisitor::visit(const ast::ArrayListExp & e)
{
}

void McCabeVisitor::visit(const ast::AssignListExp & e)
{
}

void McCabeVisitor::visit(const ast::VarDec & e)
{
}

void McCabeVisitor::visit(const ast::OptimizedExp & e)
{
}

void McCabeVisitor::visit(const ast::DAXPYExp & e)
{
}

void McCabeVisitor::visit(const ast::IntSelectExp & e)
{
}

void McCabeVisitor::visit(const ast::StringSelectExp & e)
{
}

void McCabeVisitor::visit(const ast::MemfillExp & e)
{
}

} // namespace mcCabe
