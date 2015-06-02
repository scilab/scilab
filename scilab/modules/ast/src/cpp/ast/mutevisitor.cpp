/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "mutevisitor.hxx"

namespace ast
{

void MuteVisitor::visit (const MatrixExp &e)
{
    exps_t lines = e.getLines();
    for (exps_t::const_iterator it = lines.begin(), itEnd = lines.end(); it != itEnd ; ++it)
    {
        (*it)->mute();
        (*it)->accept(*this);
    }
}

void MuteVisitor::visit (const MatrixLineExp &e)
{
    exps_t columns = e.getColumns();
    for (exps_t::const_iterator it = columns.begin(), itEnd = columns.end(); it != itEnd ; ++it)
    {
        (*it)->mute();
        (*it)->accept(*this);
    }
}

void MuteVisitor::visit (const CellExp &e)
{
    exps_t lines = e.getLines();
    for (exps_t::const_iterator it = lines.begin(), itEnd = lines.end(); it != itEnd ; ++it)
    {
        (*it)->mute();
        (*it)->accept(*this);
    }
}

void MuteVisitor::visit (const StringExp &/*e*/)
{
}

void MuteVisitor::visit (const CommentExp &/*e*/)
{
}

void MuteVisitor::visit (const DoubleExp  &/*e*/)
{
}

void MuteVisitor::visit (const BoolExp  &/*e*/)
{
}

void MuteVisitor::visit (const NilExp &/*e*/)
{
}

void MuteVisitor::visit (const SimpleVar &/*e*/)
{
}

void MuteVisitor::visit (const ColonVar &/*e*/)
{
}

void MuteVisitor::visit (const DollarVar &/*e*/)
{
}

void MuteVisitor::visit (const ArrayListVar &e)
{
    exps_t vars = e.getVars();
    for (exps_t::const_iterator it = vars.begin (), itEnd = vars.end(); it != itEnd; ++it)
    {
        (*it)->mute();
        (*it)->accept(*this);
    }
}

void MuteVisitor::visit (const FieldExp &e)
{
    const_cast<Exp*>(e.getHead())->mute();
    const_cast<Exp*>(e.getTail())->mute();

    e.getHead()->accept(*this);
    e.getTail()->accept(*this);
}

void MuteVisitor::visit(const OpExp &e)
{
    const_cast<Exp&>(e.getLeft()).mute();
    const_cast<Exp&>(e.getRight()).mute();

    e.getLeft().accept(*this);
    e.getRight().accept(*this);
}

void MuteVisitor::visit(const LogicalOpExp &e)
{
    const_cast<Exp&>(e.getLeft()).mute();
    const_cast<Exp&>(e.getRight()).mute();

    e.getLeft().accept(*this);
    e.getRight().accept(*this);
}

void MuteVisitor::visit (const AssignExp  &e)
{
    const_cast<Exp&>(e.getLeftExp()).mute();
    const_cast<Exp&>(e.getRightExp()).mute();

    e.getLeftExp().accept(*this);
    e.getRightExp().accept(*this);
}

void MuteVisitor::visit(const CallExp &e)
{
    exps_t args = e.getArgs();
    for (auto arg : args)
    {
        arg->mute();
        arg->accept(*this);
    }
}

void MuteVisitor::visit(const CellCallExp &e)
{
    exps_t args = e.getArgs();
    for (auto arg : args)
    {
        arg->mute();
        arg->accept(*this);
    }
}

void MuteVisitor::visit (const IfExp  &e)
{
    const_cast<Exp&>(e.getTest()).mute();
    const_cast<Exp&>(e.getThen()).mute();
    if (e.hasElse())
    {
        const_cast<Exp&>(e.getElse()).mute();
    }

    e.getTest().accept(*this);
    e.getThen().accept(*this);
    if (e.hasElse())
    {
        e.getElse().accept(*this);
    }
}

void MuteVisitor::visit (const TryCatchExp  &e)
{
    const_cast<Exp&>(e.getTry()).mute();
    const_cast<Exp&>(e.getCatch()).mute();

    e.getTry().accept(*this);
    e.getCatch().accept(*this);
}

void MuteVisitor::visit (const WhileExp  &e)
{
    const_cast<Exp&>(e.getTest()).mute();
    const_cast<Exp&>(e.getBody()).mute();

    e.getTest().accept(*this);
    e.getBody().accept(*this);
}

void MuteVisitor::visit (const ForExp  &e)
{
    const_cast<Exp&>(e.getBody()).mute();
    e.getBody().accept(*this);
}

void MuteVisitor::visit (const BreakExp &/*e*/)
{
}

void MuteVisitor::visit (const ContinueExp &/*e*/)
{
}

void MuteVisitor::visit (const ReturnExp &/*e*/)
{
}

void MuteVisitor::visit (const SelectExp &/*e*/)
{
}

void MuteVisitor::visit (const CaseExp &/*e*/)
{
}

void MuteVisitor::visit (const SeqExp  &e)
{
    for (exps_t::const_iterator it = e.getExps().begin (), itEnd = e.getExps().end(); it != itEnd; ++it)
    {
        (*it)->mute();
        (*it)->accept(*this);
    }
}

void MuteVisitor::visit (const ArrayListExp  &e)
{
    for (exps_t::const_iterator it = e.getExps().begin (), itEnd = e.getExps().end(); it != itEnd; ++it)
    {
        (*it)->mute();
        (*it)->accept(*this);
    }
}

void MuteVisitor::visit (const AssignListExp  &/*e*/)
{
}

void MuteVisitor::visit (const NotExp &/*e*/)
{
}

void MuteVisitor::visit (const TransposeExp &/*e*/)
{
}

void MuteVisitor::visit (const VarDec  &/*e*/)
{
}

void MuteVisitor::visit (const FunctionDec  &e)
{
    const_cast<Exp&>(e.getBody()).mute();
    e.getBody().accept(*this);
}

void MuteVisitor::visit(const ListExp &/*e*/)
{
}
}
