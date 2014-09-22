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
    std::list<MatrixLineExp *>::const_iterator	row;
    for (row = e.getLines().begin() ; row != e.getLines().end() ; ++row )
    {
        (*row)->mute();
        MuteVisitor mute;
        (*row)->accept(mute);
    }
}

void MuteVisitor::visit (const MatrixLineExp &e)
{
    std::list<Exp *>::const_iterator	col;
    for (col = e.getColumns().begin() ; col != e.getColumns().end() ; ++col)
    {
        (*col)->mute();
        MuteVisitor mute;
        (*col)->accept(mute);
    }
}

void MuteVisitor::visit (const CellExp &e)
{
    std::list<MatrixLineExp *>::const_iterator	row;
    for (row = e.getLines().begin() ; row != e.getLines().end() ; ++row )
    {
        (*row)->mute();
        MuteVisitor mute;
        (*row)->accept(mute);
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
    std::list<Var *>::const_iterator var;
    for (var = e.getVars().begin() ; var != e.getVars().end() ; var++)
    {
        (*var)->mute();
        MuteVisitor mute;
        (*var)->accept(mute);
    }
}

void MuteVisitor::visit (const FieldExp &e)
{
    const_cast<Exp*>(e.getHead())->mute();
    const_cast<Exp*>(e.getTail())->mute();

    MuteVisitor mute;
    e.getHead()->accept(mute);
    e.getTail()->accept(mute);
}

void MuteVisitor::visit(const OpExp &e)
{
    const_cast<Exp&>(e.getLeft()).mute();
    const_cast<Exp&>(e.getRight()).mute();

    MuteVisitor mute;
    e.getLeft().accept(mute);
    e.getRight().accept(mute);
}

void MuteVisitor::visit(const LogicalOpExp &e)
{
    const_cast<Exp&>(e.getLeft()).mute();
    const_cast<Exp&>(e.getRight()).mute();

    MuteVisitor mute;
    e.getLeft().accept(mute);
    e.getRight().accept(mute);
}

void MuteVisitor::visit (const AssignExp  &e)
{
    const_cast<Exp&>(e.getLeftExp()).mute();
    const_cast<Exp&>(e.getRightExp()).mute();

    MuteVisitor mute;
    e.getLeftExp().accept(mute);
    e.getRightExp().accept(mute);
}

void MuteVisitor::visit(const CallExp &e)
{
    std::list<Exp *>::const_iterator arg;
    for (arg = e.getArgs().begin() ; arg != e.getArgs().end() ; arg++)
    {
        (*arg)->mute();
        MuteVisitor mute;
        (*arg)->accept(mute);
    }
}

void MuteVisitor::visit(const CellCallExp &e)
{
    std::list<Exp *>::const_iterator arg;
    for (arg = e.getArgs().begin() ; arg != e.getArgs().end() ; arg++)
    {
        (*arg)->mute();
        MuteVisitor mute;
        (*arg)->accept(mute);
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

    MuteVisitor mute;
    e.getTest().accept(mute);
    e.getThen().accept(mute);
    if (e.hasElse())
    {
        e.getElse().accept(mute);
    }
}

void MuteVisitor::visit (const TryCatchExp  &e)
{
    const_cast<SeqExp&>(e.getTry()).mute();
    const_cast<SeqExp&>(e.getCatch()).mute();

    MuteVisitor mute;
    e.getTry().accept(mute);
    e.getCatch().accept(mute);
}

void MuteVisitor::visit (const WhileExp  &e)
{
    const_cast<Exp&>(e.getTest()).mute();
    const_cast<Exp&>(e.getBody()).mute();

    MuteVisitor mute;
    e.getTest().accept(mute);
    e.getBody().accept(mute);
}

void MuteVisitor::visit (const ForExp  &e)
{
    const_cast<Exp&>(e.getBody()).mute();

    MuteVisitor mute;
    e.getBody().accept(mute);
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
    std::list<Exp *>::const_iterator seq;
    for (seq = e.getExps().begin() ; seq != e.getExps().end() ; seq++)
    {
        (*seq)->mute();
        MuteVisitor mute;
        (*seq)->accept(mute);
    }
}

void MuteVisitor::visit (const ArrayListExp  &e)
{
    std::list<Exp *>::const_iterator exp;
    for (exp = e.getExps().begin() ; exp != e.getExps().end() ; exp++)
    {
        (*exp)->mute();
        MuteVisitor mute;
        (*exp)->accept(mute);
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

    MuteVisitor mute;
    e.getBody().accept(mute);
}

void MuteVisitor::visit(const ListExp &/*e*/)
{
}
}
