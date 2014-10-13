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

#include "debugvisitor.hxx"


namespace ast
{
static int level = -1;

static void DEBUG_START_NODE(const ast::Ast& e)
{
    wcerr << L"(" << e.getNodeNumber() << L") ";
    ++level;
}

static void DEBUG_END_NODE(void)
{
    --level;
}

static void DEBUG(wstring str)
{
    for (int i = 0 ; i < level; ++i)
    {
        wcerr << L"  ";
    }
    if (level > 0)
    {
        wcerr << L"     ";
    }
    wcerr << str << endl;
}

static void DEBUG(wstring str, const Exp &e)
{
    for (int i = 0 ; i < level; ++i)
    {
        wcerr << L"  ";
    }
    if (level > 0)
    {
        wcerr << L"|_./ ";
    }
    wcerr << str;

    Location loc = e.getLocation();
    wcerr << L" @(" << loc.first_line << L"." << loc.first_column << L" -> ";
    wcerr << loc.last_line << L"." << loc.last_column << L")";
    wcerr << L" Deco(" << e.getDecorator() << L")" << endl;
}


void DebugVisitor::visit (const MatrixExp &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec MatrixExp", e);
    exps_t lines = e.getLines();
    for (exps_t::const_iterator it = lines.begin(), itEnd = lines.end(); it != itEnd ; ++it)
    {
        (*it)->accept (*this);
    }
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const MatrixLineExp &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec MatrixLineExp", e);
    exps_t columns = e.getColumns();
    for (exps_t::const_iterator it = columns.begin(), itEnd = columns.end(); it != itEnd ; ++it)
    {
        (*it)->accept (*this);
    }
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const CellExp &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec CellExp", e);
    exps_t lines = e.getLines();
    for (exps_t::const_iterator it = lines.begin(), itEnd = lines.end(); it != itEnd ; ++it)
    {
        (*it)->accept (*this);
    }
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const StringExp &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec StringExp : " + e.getValue(), e);
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const CommentExp &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec CommentExp : " + e.getComment(), e);
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const DoubleExp  &e)
{
    DEBUG_START_NODE(e);
    wostringstream stream;
    stream << e.getValue();
    DEBUG(L"Exec DoubleExp : " + stream.str(), e);
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const BoolExp  &e)
{
    DEBUG_START_NODE(e);
    wostringstream stream;
    stream << e.getValue();
    DEBUG(L"Exec getv    : " + stream.str(), e);
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const NilExp &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec NilExp", e);
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const SimpleVar &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec SimpleVar : " + e.getSymbol().getName(), e);
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const ColonVar &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec ColonVar", e);
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const DollarVar &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec DollarVar", e);
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const ArrayListVar &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec ArrayListVar", e);
    exps_t vars = e.getVars();
    for (exps_t::const_iterator it = vars.begin (), itEnd = vars.end(); it != itEnd; ++it)
    {
        (*it)->accept(*this);
    }
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const FieldExp &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec FieldExp", e);
    e.getHead()->accept(*this);
    e.getTail()->accept(*this);
    DEBUG_END_NODE();
}

void DebugVisitor::visit(const OpExp &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec OpExp", e);
    e.getLeft().accept(*this);
    e.getRight().accept(*this);
    DEBUG_END_NODE();
}

void DebugVisitor::visit(const LogicalOpExp &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec LogicalOpExp", e);
    e.getLeft().accept(*this);
    e.getRight().accept(*this);
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const AssignExp  &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec AssignExp", e);
    e.getLeftExp().accept (*this);
    e.getRightExp().accept (*this);
    DEBUG_END_NODE();
}

void DebugVisitor::visit(const CellCallExp &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec CellCallExp", e);
    e.getName().accept (*this);

    exps_t args = e.getArgs();
    for (exps_t::const_iterator it = args.begin (), itEnd = args.end(); it != itEnd; ++it)
    {
        (*it)->accept(*this);
    }
    DEBUG_END_NODE();
}

void DebugVisitor::visit(const CallExp &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec CallExp", e);
    e.getName().accept (*this);

    exps_t args = e.getArgs();
    for (exps_t::const_iterator it = args.begin (), itEnd = args.end(); it != itEnd; ++it)
    {
        (*it)->accept (*this);
    }
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const IfExp  &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec IfExp", e);
    e.getTest ().accept(*this);
    e.getThen ().accept(*this);
    if (e.hasElse())
    {
        e.getElse ().accept(*this);
    }
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const TryCatchExp  &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec TryCatchExp", e);
    e.getTry ().accept(*this);
    e.getCatch ().accept(*this);
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const WhileExp  &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec WhileExp", e);
    e.getTest().accept (*this);
    e.getBody().accept (*this);
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const ForExp  &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec ForExp", e);
    e.getVardec().accept(*this);
    e.getBody().accept (*this);
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const ContinueExp &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec ContinueExp", e);
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const BreakExp &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec BreakExp", e);
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const ReturnExp &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec ReturnExp", e);
    if (!e.isGlobal())
    {
        e.getExp().accept(*this);
    }
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const SelectExp &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec SelectExp", e);
    e.getSelect()->accept(*this);

    exps_t* cases = e.getCases();
    for (exps_t::iterator it = cases->begin(), itEnd = cases->end(); it !=  itEnd ; ++it)
    {
        (*it)->accept(*this);
    }

    if (e.getDefaultCase() != NULL)
    {
        e.getDefaultCase()->accept(*this);
    }
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const CaseExp &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec CaseExp", e);
    e.getTest()->accept(*this);
    e.getBody()->accept(*this);
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const SeqExp  &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec SeqExp", e);
    for (exps_t::const_iterator it = e.getExps().begin (), itEnd = e.getExps().end(); it != itEnd; ++it)
    {
        (*it)->accept(*this);
    }
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const ArrayListExp  &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec ArrayListExp", e);
    for (exps_t::const_iterator it = e.getExps().begin (), itEnd = e.getExps().end(); it != itEnd; ++it)
    {
        (*it)->accept (*this);
    }
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const AssignListExp  &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec AssignListExp", e);
    for (exps_t::const_iterator it = e.getExps().begin (), itEnd = e.getExps().end(); it != itEnd; ++it)
    {
        (*it)->accept (*this);
    }
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const NotExp &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec NotExp", e);
    e.getExp().accept (*this);
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const TransposeExp &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec TransposeExp", e);
    e.getExp().accept (*this);
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const VarDec  &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec VarDec", e);
    DEBUG_START_NODE(e);
    DEBUG(L"Exec Symbol : " + e.getSymbol().getName(), e);
    DEBUG_END_NODE();
    e.getInit().accept(*this);
    DEBUG_END_NODE();
}

void DebugVisitor::visit (const FunctionDec  &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec FunctionDec", e);
    // FIXME

    // First ask if there are some return values.
    //visit(e.returns_get());

    // Then get the function name
    //visit(e.getName());

    // Then get function args
    //visit(e.args_get());

    // Now debug function body
    e.getBody().accept(*this);

    DEBUG_END_NODE();
}

void DebugVisitor::visit(const ListExp &e)
{
    DEBUG_START_NODE(e);
    DEBUG(L"Exec ListExp", e);
    e.getStart().accept(*this);
    e.getStep().accept(*this);
    e.getEnd().accept(*this);
    DEBUG_END_NODE();
}

void DebugVisitor::visit(const OptimizedExp &e)
{
    e.getOriginal()->accept(*this);
}

void DebugVisitor::visit(const DAXPYExp &e)
{
    e.getOriginal()->accept(*this);
}

}
