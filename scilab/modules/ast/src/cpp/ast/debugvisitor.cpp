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

void DebugVisitor::START_NODE(const ast::Ast& e)
{
    *ostr << L"(" << e.getNodeNumber() << L") ";
    ++level;
}

void DebugVisitor::END_NODE(void)
{
    --level;
}

void DebugVisitor::print(const std::wstring& str)
{
    for (int i = 0 ; i < level; ++i)
    {
        *ostr << L"  ";
    }
    if (level > 0)
    {
        *ostr << L"     ";
    }
    *ostr << str << std::endl;
}

void DebugVisitor::print(const std::wstring& str, const Exp &e)
{
    for (int i = 0 ; i < level; ++i)
    {
        *ostr << L"  ";
    }
    if (level > 0)
    {
        *ostr << L"|_./ ";
    }
    *ostr << str;

    Location loc = e.getLocation();
    *ostr << L" @(" << loc.first_line << L"." << loc.first_column << L" -> ";
    *ostr << loc.last_line << L"." << loc.last_column << L")";
    if (printDecoration)
    {
        *ostr << L" Deco(" << e.getDecorator() << L")";
    }
    *ostr << std::endl;
}


void DebugVisitor::visit (const MatrixExp &e)
{
    START_NODE(e);
    print(L"Exec MatrixExp", e);
    exps_t lines = e.getLines();
    for (exps_t::const_iterator it = lines.begin(), itEnd = lines.end(); it != itEnd ; ++it)
    {
        (*it)->accept(*this);
    }
    END_NODE();
}

void DebugVisitor::visit (const MatrixLineExp &e)
{
    START_NODE(e);
    print(L"Exec MatrixLineExp", e);
    exps_t columns = e.getColumns();
    for (exps_t::const_iterator it = columns.begin(), itEnd = columns.end(); it != itEnd ; ++it)
    {
        (*it)->accept(*this);
    }
    END_NODE();
}

void DebugVisitor::visit (const CellExp &e)
{
    START_NODE(e);
    print(L"Exec CellExp", e);
    exps_t lines = e.getLines();
    for (exps_t::const_iterator it = lines.begin(), itEnd = lines.end(); it != itEnd ; ++it)
    {
        (*it)->accept(*this);
    }
    END_NODE();
}

void DebugVisitor::visit (const StringExp &e)
{
    START_NODE(e);
    std::wostringstream stream;
    if (e.getConstant())
    {
        printInternalType<types::String>(stream, e.getConstant());
    }
    else
    {
        stream << e.getValue();
    }
    print(L"Exec StringExp : " + stream.str(), e);
    END_NODE();
}

void DebugVisitor::visit (const CommentExp &e)
{
    START_NODE(e);
    print(L"Exec CommentExp : " + e.getComment(), e);
    END_NODE();
}

void DebugVisitor::visit (const DoubleExp  &e)
{
    START_NODE(e);
    std::wostringstream stream;
    types::InternalType * pIT = e.getConstant();
    if (pIT)
    {
        if (pIT->isImplicitList())
        {
            types::ImplicitList * pIL = static_cast<types::ImplicitList *>(pIT);
            stream << static_cast<types::Double *>(pIL->getStart())->get(0) << L":"
                   << static_cast<types::Double *>(pIL->getStep())->get(0) << L":"
                   << static_cast<types::Double *>(pIL->getEnd())->get(0);
        }
        else
        {
            printInternalType<types::Double>(stream, pIT);
        }
    }
    else
    {
        stream << e.getValue();
    }
    print(L"Exec DoubleExp : " + stream.str(), e);
    END_NODE();
}

void DebugVisitor::visit (const BoolExp  &e)
{
    START_NODE(e);
    std::wostringstream stream;
    if (e.getConstant())
    {
        printInternalType<types::Bool>(stream, e.getConstant());
    }
    else
    {
        stream << e.getValue();
    }
    print(L"Exec BoolExp : " + stream.str(), e);
    END_NODE();
}

void DebugVisitor::visit (const NilExp &e)
{
    START_NODE(e);
    print(L"Exec NilExp", e);
    END_NODE();
}

void DebugVisitor::visit (const SimpleVar &e)
{
    std::wstring ty;
    analysis::TIType type = e.getDecorator().getResult().getType();
    if (type.type != analysis::TIType::UNKNOWN)
    {
        if (type.isscalar())
        {
            ty = L" (" + analysis::TIType::toString(type.type) + L")";
        }
        else
        {
            ty = L" (" + analysis::TIType::toString(type.type) + L"*)";
        }
    }

    START_NODE(e);
    print(L"Exec SimpleVar : " + e.getSymbol().getName() + ty, e);
    END_NODE();
}

void DebugVisitor::visit (const ColonVar &e)
{
    START_NODE(e);
    print(L"Exec ColonVar", e);
    END_NODE();
}

void DebugVisitor::visit (const DollarVar &e)
{
    START_NODE(e);
    print(L"Exec DollarVar", e);
    END_NODE();
}

void DebugVisitor::visit (const ArrayListVar &e)
{
    START_NODE(e);
    print(L"Exec ArrayListVar", e);
    exps_t vars = e.getVars();
    for (exps_t::const_iterator it = vars.begin (), itEnd = vars.end(); it != itEnd; ++it)
    {
        (*it)->accept(*this);
    }
    END_NODE();
}

void DebugVisitor::visit (const FieldExp &e)
{
    START_NODE(e);
    print(L"Exec FieldExp", e);
    e.getHead()->accept(*this);
    e.getTail()->accept(*this);
    END_NODE();
}

void DebugVisitor::visit(const OpExp &e)
{
    START_NODE(e);
    print(L"Exec OpExp", e);
    e.getLeft().accept(*this);
    e.getRight().accept(*this);
    END_NODE();
}

void DebugVisitor::visit(const LogicalOpExp &e)
{
    START_NODE(e);
    print(L"Exec LogicalOpExp", e);
    e.getLeft().accept(*this);
    e.getRight().accept(*this);
    END_NODE();
}

void DebugVisitor::visit (const AssignExp  &e)
{
    START_NODE(e);
    print(L"Exec AssignExp", e);
    e.getLeftExp().accept(*this);
    e.getRightExp().accept(*this);
    END_NODE();
}

void DebugVisitor::visit(const CellCallExp &e)
{
    START_NODE(e);
    print(L"Exec CellCallExp", e);
    e.getName().accept(*this);

    exps_t args = e.getArgs();
    for (auto arg : args)
    {
        arg->accept(*this);
    }

    END_NODE();
}

void DebugVisitor::visit(const CallExp &e)
{
    std::wstring str;

    START_NODE(e);
    print(L"Exec CallExp" + str, e);
    e.getName().accept(*this);

    exps_t args = e.getArgs();
    for (auto arg : args)
    {
        arg->accept(*this);
    }

    END_NODE();
}

void DebugVisitor::visit (const IfExp  &e)
{
    START_NODE(e);
    print(L"Exec IfExp", e);
    e.getTest ().accept(*this);
    e.getThen ().accept(*this);
    if (e.hasElse())
    {
        e.getElse ().accept(*this);
    }
    END_NODE();
}

void DebugVisitor::visit (const TryCatchExp  &e)
{
    START_NODE(e);
    print(L"Exec TryCatchExp", e);
    e.getTry ().accept(*this);
    e.getCatch ().accept(*this);
    END_NODE();
}

void DebugVisitor::visit (const WhileExp  &e)
{
    START_NODE(e);
    print(L"Exec WhileExp", e);
    e.getTest().accept(*this);
    e.getBody().accept(*this);
    END_NODE();
}

void DebugVisitor::visit (const ForExp  &e)
{
    START_NODE(e);
    print(L"Exec ForExp", e);
    e.getVardec().accept(*this);
    e.getBody().accept(*this);
    END_NODE();
}

void DebugVisitor::visit (const ContinueExp &e)
{
    START_NODE(e);
    print(L"Exec ContinueExp", e);
    END_NODE();
}

void DebugVisitor::visit (const BreakExp &e)
{
    START_NODE(e);
    print(L"Exec BreakExp", e);
    END_NODE();
}

void DebugVisitor::visit (const ReturnExp &e)
{
    START_NODE(e);
    print(L"Exec ReturnExp", e);
    if (!e.isGlobal())
    {
        e.getExp().accept(*this);
    }
    END_NODE();
}

void DebugVisitor::visit (const SelectExp &e)
{
    START_NODE(e);
    print(L"Exec SelectExp", e);
    e.getSelect()->accept(*this);

    exps_t cases = e.getCases();
    for (auto exp : cases)
    {
        exp->accept(*this);
    }

    if (e.getDefaultCase() != NULL)
    {
        e.getDefaultCase()->accept(*this);
    }
    END_NODE();
}

void DebugVisitor::visit (const CaseExp &e)
{
    START_NODE(e);
    print(L"Exec CaseExp", e);
    e.getTest()->accept(*this);
    e.getBody()->accept(*this);
    END_NODE();
}

void DebugVisitor::visit (const SeqExp  &e)
{
    START_NODE(e);
    print(L"Exec SeqExp", e);
    for (exps_t::const_iterator it = e.getExps().begin (), itEnd = e.getExps().end(); it != itEnd; ++it)
    {
        (*it)->accept(*this);
    }
    END_NODE();
}

void DebugVisitor::visit (const ArrayListExp  &e)
{
    START_NODE(e);
    print(L"Exec ArrayListExp", e);
    for (exps_t::const_iterator it = e.getExps().begin (), itEnd = e.getExps().end(); it != itEnd; ++it)
    {
        (*it)->accept(*this);
    }
    END_NODE();
}

void DebugVisitor::visit (const AssignListExp  &e)
{
    START_NODE(e);
    print(L"Exec AssignListExp", e);
    for (exps_t::const_iterator it = e.getExps().begin (), itEnd = e.getExps().end(); it != itEnd; ++it)
    {
        (*it)->accept(*this);
    }
    END_NODE();
}

void DebugVisitor::visit (const NotExp &e)
{
    START_NODE(e);
    print(L"Exec NotExp", e);
    e.getExp().accept(*this);
    END_NODE();
}

void DebugVisitor::visit (const TransposeExp &e)
{
    START_NODE(e);
    print(L"Exec TransposeExp", e);
    e.getExp().accept(*this);
    END_NODE();
}

void DebugVisitor::visit (const VarDec  &e)
{
    START_NODE(e);
    print(L"Exec VarDec", e);
    START_NODE(e);
    print(L"Exec Symbol : " + e.getSymbol().getName(), e);
    END_NODE();
    e.getInit().accept(*this);
    END_NODE();
}

void DebugVisitor::visit (const FunctionDec  &e)
{
    START_NODE(e);
    print(L"Exec FunctionDec", e);
    // FIXME

    // First ask if there are some return values.
    //visit(e.returns_get());

    // Then get the function name
    //visit(e.getName());

    // Then get function args
    //visit(e.args_get());

    // Now debug function body
    e.getBody().accept(*this);

    END_NODE();
}

void DebugVisitor::visit(const ListExp &e)
{
    START_NODE(e);
    print(L"Exec ListExp", e);
    e.getStart().accept(*this);
    e.getStep().accept(*this);
    e.getEnd().accept(*this);
    END_NODE();
}

void DebugVisitor::visit(const OptimizedExp &e)
{
    e.getOriginal()->accept(*this);
}

void DebugVisitor::visit(const MemfillExp &e)
{
    START_NODE(e);
    print(L"Exec MemfillExp", e);
    e.getValue().accept(*this);

    exps_t args = e.getArgs();
    for (auto arg : args)
    {
        arg->accept(*this);
    }

    END_NODE();
}

void DebugVisitor::visit(const DAXPYExp &e)
{
    START_NODE(e);
    print(L"Exec DAXPYExp", e);
    e.getA().accept(*this);
    e.getX().accept(*this);
    e.getY().accept(*this);
    END_NODE();

    //e.getOriginal()->accept(*this);
}

void DebugVisitor::visit(const IntSelectExp & e)
{
    e.getOriginal()->accept(*this);
}

void DebugVisitor::visit(const StringSelectExp & e)
{
    e.getOriginal()->accept(*this);
}

}
