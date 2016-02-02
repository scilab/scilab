/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
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

#include <cmath>

#include "string.hxx"
#include "printvisitor.hxx"

namespace ast
{

/** \name Visit Matrix Expressions nodes.
 ** \{ */
void PrintVisitor::visit (const MatrixExp &e)
{
    ast::exps_t::const_iterator	i, j;
    *ostr << SCI_OPEN_MATRIX;
    ++indent;
    this->is_last_matrix_line = false;
    ast::exps_t lines = e.getLines();
    for (i = lines.begin() ; i != lines.end() ; )
    {
        bool addIndent = false;
        j = i;
        if (++j == lines.end())
        {
            this->is_last_matrix_line = true;
        }
        else
        {
            if ((*i)->getLocation().last_line != (*j)->getLocation().first_line)
            {
                addIndent = true;
            }
        }

        if (displayOriginal)
        {
            (*i)->getOriginal()->accept(*this);
        }
        else
        {
            (*i)->accept(*this);
        }

        //if (lines.size() > 1 || this->is_last_column_comment)
        if (addIndent)
        {
            *ostr << std::endl;
            this->apply_indent();
        }

        ++i;
    }
    *ostr << SCI_CLOSE_MATRIX;
    --indent;
}

void PrintVisitor::visit (const MatrixLineExp &e)
{
    ast::exps_t::const_iterator	i, j;
    this->is_last_column_comment = false;

    ast::exps_t cols = e.getColumns();
    for (i = cols.begin() ; i != cols.end() ; )
    {
        if (displayOriginal)
        {
            (*i)->getOriginal()->accept(*this);
        }
        else
        {
            (*i)->accept(*this);
        }

        if ((*i)->isCommentExp())
        {
            this->is_last_column_comment = true;
        }
        if (++i != cols.end())
        {
            if ((*i)->isCommentExp() == false)
            {
                *ostr << SCI_COLUMN_SEPARATOR;
            }
            *ostr << " ";
        }
    }

    if (!this->is_last_column_comment && this->is_last_matrix_line == false)
    {
        *ostr << SCI_LINE_SEPARATOR;
    }
}
/** \} */

/** \name Visit Cell Expressions nodes.
 ** \{ */
void PrintVisitor::visit (const CellExp &e)
{
    ast::exps_t::const_iterator	i, j;

    *ostr << SCI_OPEN_CELL;
    ++indent;
    ast::exps_t lines = e.getLines();
    for (i = lines.begin() ; i != lines.end() ; )
    {
        if (displayOriginal)
        {
            (*i)->getOriginal()->accept(*this);
        }
        else
        {
            (*i)->accept(*this);
        }
        if (++i != lines.end())
        {
            *ostr << SCI_LINE_SEPARATOR << std::endl;
            this->apply_indent();
        }
    }
    *ostr << SCI_CLOSE_CELL;
    --indent;
}
/** \} */

/** \name Visit Constant Expressions nodes.
 ** \{ */
void PrintVisitor::visit (const StringExp &e)
{
    if (types::InternalType * pIT = e.getConstant())
    {
        types::String * pStr = static_cast<types::String *>(pIT);
        if (pStr->getSize() == 0)
        {
            *ostr << L"[]";
        }
        if (pStr->getSize() == 1)
        {
            std::wstring wstr(pStr->get(0, 0));
            printString(wstr);
        }
        else
        {
            *ostr << L"[";
            const int r = pStr->getRows();
            const int c = pStr->getCols();
            for (int i = 0; i < r; ++i)
            {
                for (int j = 0; j < c - 1; ++j)
                {
                    std::wstring wstr(pStr->get(i, j));
                    printString(wstr);
                    *ostr << L" ";
                }
                std::wstring wstr(pStr->get(i, c - 1));
                printString(wstr);
                *ostr << L";";
            }
            *ostr << L"]";
        }
    }
    else
    {
        printString(e.getValue());
    }
}

void PrintVisitor::visit (const CommentExp &e)
{
    *ostr << SCI_OPEN_COMMENT;
    *ostr << e.getComment();
}

void PrintVisitor::visit (const DoubleExp  &e)
{
    if (types::InternalType * pIT = e.getConstant())
    {
        if (pIT->isDouble())
        {
            types::Double * pDbl = static_cast<types::Double *>(pIT);
            if (pDbl->getSize() == 0)
            {
                *ostr << L"[]";
            }
            else if (pDbl->getSize() == 1)
            {
                if (pDbl->isComplex())
                {
                    const double imag = pDbl->getImg()[0];
                    if (imag != 0)
                    {
                        *ostr << pDbl->getReal()[0] << (imag > 0 ? L"+%i*" : L"-%i*") << std::abs(imag);
                    }
                    else
                    {
                        *ostr << pDbl->getReal()[0];
                    }
                }
                else
                {
                    *ostr << pDbl->getReal()[0];
                }
            }
            else
            {
                *ostr << L"[";
                const int r = pDbl->getRows();
                const int c = pDbl->getCols();
                if (pDbl->isComplex())
                {
                    for (int i = 0; i < r; ++i)
                    {
                        for (int j = 0; j < c - 1; ++j)
                        {
                            const double imag = pDbl->getImg(i, j);
                            if (imag != 0)
                            {
                                *ostr << pDbl->getReal(i, j) << (imag > 0 ? L"+%i*" : L"-%i*") << std::abs(imag) << L" ";
                            }
                            else
                            {
                                *ostr << pDbl->get(i, j) << L" ";
                            }
                        }
                        *ostr << pDbl->get(i, c - 1) << L";";
                    }
                }
                else
                {
                    for (int i = 0; i < r; ++i)
                    {
                        for (int j = 0; j < c - 1; ++j)
                        {
                            *ostr << pDbl->get(i, j) << L" ";
                        }
                        *ostr << pDbl->get(i, c - 1) << L";";
                    }
                }
                *ostr << L"]";
            }
        }
    }
    else
    {
        *ostr << e.getValue();
    }
}

void PrintVisitor::visit (const BoolExp  &e)
{
    if (types::InternalType * pIT = e.getConstant())
    {
        if (pIT->isBool())
        {
            types::Bool * pBool = static_cast<types::Bool *>(pIT);
            if (pBool->getSize() == 0)
            {
                *ostr << L"[]";
            }
            if (pBool->getSize() == 1)
            {
                *ostr << (pBool->get(0, 0) ? SCI_TRUE : SCI_FALSE);
            }
            else
            {
                *ostr << L"[";
                const int r = pBool->getRows();
                const int c = pBool->getCols();
                for (int i = 0; i < r; ++i)
                {
                    for (int j = 0; j < c - 1; ++j)
                    {
                        *ostr << (pBool->get(i, j) ? SCI_TRUE : SCI_FALSE) << L" ";
                    }
                    *ostr << (pBool->get(i, c - 1) ? SCI_TRUE : SCI_FALSE) << L";";
                }
                *ostr << L"]";
            }
        }
    }
    else
    {
        if (e.getValue() == true)
        {
            *ostr << SCI_TRUE;
        }
        else
        {
            *ostr << SCI_FALSE;
        }
    }
}

void PrintVisitor::visit (const NilExp &/*e*/)
{
    /* Do Nothing */
}
/** \} */

/** \name Visit Variable related nodes.
 ** \{ */
void PrintVisitor::visit (const SimpleVar &e)
{
    *ostr << e.getSymbol().getName();
}

void PrintVisitor::visit (const ColonVar &/*e*/)
{
    *ostr << SCI_COLON;
}

void PrintVisitor::visit (const DollarVar &/*e*/)
{
    *ostr << SCI_DOLLAR;
}

void PrintVisitor::visit (const ArrayListVar &e)
{
    exps_t vars = e.getVars();
    for (exps_t::const_iterator it = vars.begin (), itEnd = vars.end(); it != itEnd; /**/)
    {
        if (displayOriginal)
        {
            (*it)->getOriginal()->accept(*this);
        }
        else
        {
            (*it)->accept(*this);
        }
        if (++it != itEnd)
        {
            *ostr << ", ";
        }
    }
}
/** \} */

/** \name Visit Control Expressions or Instructions nodes.
 ** \{ */

void PrintVisitor::visit (const FieldExp &e)
{
    if (displayOriginal)
    {
        e.getHead()->getOriginal()->accept(*this);
    }
    else
    {
        e.getHead()->accept(*this);
    }
    *ostr << SCI_FVAR_SEPARATOR;
    if (displayOriginal)
    {
        e.getTail()->getOriginal()->accept(*this);
    }
    else
    {
        e.getTail()->accept(*this);
    }
}

void PrintVisitor::visit(const OpExp &e)
{
    bool old_force_parenthesis = force_parenthesis;

    if (force_parenthesis)
    {
        *ostr << SCI_LPAREN;
    }

    if (e.getOper() != OpExp::unaryMinus)
    {
        // Getting Left Operand
        this->enable_force_parenthesis();
        if (displayOriginal)
        {
            e.getLeft().getOriginal()->accept(*this);
        }
        else
        {
            e.getLeft().accept(*this);
        }
        this->set_force_parenthesis(old_force_parenthesis);
        *ostr << " ";
    }

    switch (e.getOper())
    {
        // Arithmetics.
        case OpExp::plus:
            *ostr << SCI_PLUS;
            break;
        case OpExp::unaryMinus:
        case OpExp::minus:
            *ostr << SCI_MINUS;
            break;
        case OpExp::times:
            *ostr << SCI_TIMES;
            break;
        case OpExp::rdivide:
            *ostr << SCI_RDIVIDE;
            break;
        case OpExp::ldivide:
            *ostr << SCI_LDIVIDE;
            break;
        case OpExp::power:
            *ostr << SCI_POWER;
            break;
        // Element Ways.
        case OpExp::dottimes:
            *ostr << SCI_DOTTIMES;
            break;
        case OpExp::dotrdivide:
            *ostr << SCI_DOTRDIVIDE;
            break;
        case OpExp::dotldivide:
            *ostr << SCI_DOTLDIVIDE;
            break;
        case OpExp::dotpower:
            *ostr << SCI_DOTPOWER;
            break;
        // Kroneckers
        case OpExp::krontimes:
            *ostr << SCI_KRONTIMES;
            break;
        case OpExp::kronrdivide:
            *ostr << SCI_KRONRDIVIDE;
            break;
        case OpExp::kronldivide:
            *ostr << SCI_KRONLDIVIDE;
            break;
        // Control
        case OpExp::controltimes:
            *ostr << SCI_CONTROLTIMES;
            break;
        case OpExp::controlrdivide:
            *ostr << SCI_CONTROLRDIVIDE;
            break;
        case OpExp::controlldivide:
            *ostr << SCI_CONTROLLDIVIDE;
            break;
        // Comparisons
        case OpExp::eq:
            *ostr << SCI_EQ;
            break;
        case OpExp::ne:
            *ostr << SCI_NE;
            break;
        case OpExp::lt:
            *ostr << SCI_LT;
            break;
        case OpExp::le:
            *ostr << SCI_LE;
            break;
        case OpExp::gt:
            *ostr << SCI_GT;
            break;
        case OpExp::ge:
            *ostr << SCI_GE;
            break;
        default :
            // FIXME : This case should never happend.
            break;
    }
    *ostr << " ";

    // Now getting right operand
    this->enable_force_parenthesis();
    if (displayOriginal)
    {
        e.getRight().getOriginal()->accept(*this);
    }
    else
    {
        e.getRight().accept(*this);
    }
    this->set_force_parenthesis(old_force_parenthesis);

    if (force_parenthesis)
    {
        *ostr << SCI_RPAREN;
    }
}

void PrintVisitor::visit(const LogicalOpExp &e)
{
    bool old_force_parenthesis = force_parenthesis;

    if (force_parenthesis)
    {
        *ostr << SCI_LPAREN;
    }

    // Getting Left Operand
    this->enable_force_parenthesis();
    if (displayOriginal)
    {
        e.getLeft().getOriginal()->accept(*this);
    }
    else
    {
        e.getLeft().accept(*this);
    }
    this->set_force_parenthesis(old_force_parenthesis);

    *ostr << " ";
    switch (e.getOper())
    {
        // Binary Operators
        case LogicalOpExp::logicalAnd:
            *ostr << SCI_AND;
            break;
        case LogicalOpExp::logicalOr:
            *ostr << SCI_OR;
            break;
        case LogicalOpExp::logicalShortCutAnd:
            *ostr << SCI_ANDAND;
            break;
        case LogicalOpExp::logicalShortCutOr:
            *ostr << SCI_OROR;
            break;
        default :
            // FIXME : This case should never happend.
            break;
    }
    *ostr << " ";

    // Now getting right operand
    this->enable_force_parenthesis();
    if (displayOriginal)
    {
        e.getRight().getOriginal()->accept(*this);
    }
    else
    {
        e.getRight().accept(*this);
    }
    this->set_force_parenthesis(old_force_parenthesis);

    if (force_parenthesis)
    {
        *ostr << SCI_RPAREN;
    }
}

void PrintVisitor::visit (const AssignExp  &e)
{
    if (displayOriginal)
    {
        e.getLeftExp().getOriginal()->accept(*this);
        *ostr << " " << SCI_ASSIGN << " ";
        e.getRightExp().getOriginal()->accept(*this);
    }
    else
    {
        e.getLeftExp().accept(*this);
        *ostr << " " << SCI_ASSIGN << " ";
        e.getRightExp().accept(*this);
    }
}

void PrintVisitor::visit(const CellCallExp &e)
{
    if (displayOriginal)
    {
        e.getName().getOriginal()->accept(*this);
    }
    else
    {
        e.getName().accept(*this);
    }
    *ostr << SCI_OPEN_CELL;
    exps_t args = e.getArgs();
    for (exps_t::const_iterator it = args.begin(), itEnd = args.end(); it != itEnd; /**/)
    {
        if (displayOriginal)
        {
            (*it)->getOriginal()->accept(*this);
        }
        else
        {
            (*it)->accept(*this);
        }
        if (++it != itEnd)
        {
            *ostr << SCI_COMMA << " ";
        }
    }
    *ostr << SCI_CLOSE_CELL;
}

void PrintVisitor::visit(const CallExp &e)
{
    if (displayOriginal)
    {
        e.getName().getOriginal()->accept(*this);
    }
    else
    {
        e.getName().accept(*this);
    }
    *ostr << SCI_OPEN_CALL;

    exps_t args = e.getArgs();
    for (exps_t::const_iterator it = args.begin(), itEnd = args.end(); it != itEnd; /**/)
    {
        if (displayOriginal)
        {
            (*it)->getOriginal()->accept(*this);
        }
        else
        {
            (*it)->accept(*this);
        }
        if (++it != itEnd)
        {
            *ostr << SCI_COMMA << " ";
        }
    }
    *ostr << SCI_CLOSE_CALL;
}

void PrintVisitor::visit (const IfExp  &e)
{
    *ostr << SCI_IF;
    *ostr << " " << SCI_OPEN_TEST;
    if (displayOriginal)
    {
        e.getTest().getOriginal()->accept(*this);
    }
    else
    {
        e.getTest().accept(*this);
    }
    *ostr << SCI_CLOSE_TEST << " ";
    *ostr << SCI_THEN << std::endl;

    if (headerOnly)
    {
        return;
    }

    ++indent;
    if (displayOriginal)
    {
        e.getThen().getOriginal()->accept(*this);
    }
    else
    {
        e.getThen().accept(*this);
    }
    --indent;
    if (e.hasElse())
    {
        this->apply_indent();
        *ostr << SCI_ELSE << std::endl;
        ++indent;
        if (displayOriginal)
        {
            e.getElse().getOriginal()->accept(*this);
        }
        else
        {
            e.getElse().accept(*this);
        }
        --indent;
    }
    this->apply_indent();
    *ostr << SCI_ENDIF;
}

void PrintVisitor::visit (const TryCatchExp  &e)
{
    *ostr << SCI_TRY << std::endl;

    if (headerOnly)
    {
        return;
    }

    ++indent;
    if (displayOriginal)
    {
        e.getTry().getOriginal()->accept(*this);
    }
    else
    {
        e.getTry().accept(*this);
    }
    --indent;
    this->apply_indent();
    *ostr << SCI_CATCH << std::endl;
    ++indent;
    if (displayOriginal)
    {
        e.getCatch().getOriginal()->accept(*this);
    }
    else
    {
        e.getCatch().accept(*this);
    }
    --indent;
    this->apply_indent();
    *ostr << SCI_ENDTRY;
}

void PrintVisitor::visit (const WhileExp  &e)
{
    *ostr << SCI_WHILE;
    *ostr << " " << SCI_OPEN_TEST;
    if (displayOriginal)
    {
        e.getTest().getOriginal()->accept(*this);
    }
    else
    {
        e.getTest().accept(*this);
    }
    *ostr << SCI_CLOSE_TEST << " " << SCI_DO << std::endl;

    if (headerOnly)
    {
        return;
    }

    ++indent;
    if (displayOriginal)
    {
        e.getBody().getOriginal()->accept(*this);
    }
    else
    {
        e.getBody().accept(*this);
    }
    --indent;
    this->apply_indent();
    *ostr << SCI_ENDWHILE;
}

void PrintVisitor::visit (const ForExp  &e)
{
    *ostr << SCI_FOR;
    *ostr << " " << SCI_OPEN_TEST;
    if (displayOriginal)
    {
        e.getVardec().getOriginal()->accept(*this);
    }
    else
    {
        e.getVardec().accept(*this);
    }
    *ostr << SCI_CLOSE_TEST << " ";
    *ostr << SCI_DO << std::endl;

    if (headerOnly)
    {
        return;
    }

    ++indent;
    if (displayOriginal)
    {
        e.getBody().getOriginal()->accept(*this);
    }
    else
    {
        e.getBody().accept(*this);
    }
    --indent;
    this->apply_indent();
    *ostr << SCI_ENDFOR;
}

void PrintVisitor::visit (const BreakExp &/*e*/)
{
    *ostr << SCI_BREAK;
}

void PrintVisitor::visit (const ContinueExp &/*e*/)
{
    *ostr << SCI_CONTINUE;
}

void PrintVisitor::visit (const ReturnExp &e)
{
    *ostr << SCI_RETURN;
    if (!e.isGlobal())
    {
        *ostr << " " ;
        if (displayOriginal)
        {
            e.getExp().getOriginal()->accept(*this);
        }
        else
        {
            e.getExp().accept(*this);
        }
    }
}

void PrintVisitor::visit (const SelectExp &e)
{
    *ostr << SCI_SELECT;
    *ostr << " " << SCI_OPEN_TEST;
    if (displayOriginal)
    {
        e.getSelect()->getOriginal()->accept(*this);
    }
    else
    {
        e.getSelect()->accept(*this);
    }
    *ostr << SCI_CLOSE_TEST << std::endl;
    ++indent;
    exps_t cases = e.getCases();
    for (auto exp : cases)
    {
        if (displayOriginal)
        {
            exp->getOriginal()->accept(*this);
        }
        else
        {
            exp->accept(*this);
        }
    }

    if (e.hasDefault())
    {
        this->apply_indent();
        *ostr << SCI_DEFAULT_CASE << std::endl;
        ++indent;
        if (displayOriginal)
        {
            e.getDefaultCase()->getOriginal()->accept(*this);
        }
        else
        {
            e.getDefaultCase()->accept(*this);
        }
        --indent;
    }
    --indent;
    this->apply_indent();
    *ostr << SCI_ENDSELECT;
}

void PrintVisitor::visit (const CaseExp &e)
{
    this->apply_indent();
    *ostr << SCI_CASE;
    *ostr << " " << SCI_OPEN_TEST;
    if (displayOriginal)
    {
        e.getTest()->getOriginal()->accept(*this);
    }
    else
    {
        e.getTest()->accept(*this);
    }
    *ostr << SCI_CLOSE_TEST << std::endl;
    indent++;
    if (displayOriginal)
    {
        e.getBody()->getOriginal()->accept(*this);
    }
    else
    {
        e.getBody()->accept(*this);
    }
    indent--;
}

void PrintVisitor::visit (const SeqExp  &e)
{
    int previousLine = -1;
    bool bPreviousVerbose = false;
    for (exps_t::const_iterator it = e.getExps().begin (), itEnd = e.getExps().end(); it != itEnd; ++it)
    {
        if (previousLine == -1)
        {
            this->apply_indent();
        }

        if (previousLine != -1 && (*it)->getLocation().first_line != previousLine)
        {
            *ostr << std::endl;
            this->apply_indent();
        }

        if ((*it)->getLocation().first_line == previousLine && bPreviousVerbose)
        {
            *ostr << ",";
        }

        if (displayOriginal)
        {
            (*it)->getOriginal()->accept(*this);
        }
        else
        {
            (*it)->accept(*this);
        }
        bPreviousVerbose = (*it)->isVerbose();
        if (!(*it)->isVerbose())
        {
            *ostr << ";";
        }

        previousLine = (*it)->getLocation().last_line;
    }

    *ostr << std::endl;
}

void PrintVisitor::visit (const ArrayListExp  &e)
{
    *ostr << SCI_LPAREN;
    for (exps_t::const_iterator it = e.getExps().begin (), itEnd = e.getExps().end(); it != itEnd; /**/)
    {
        if (displayOriginal)
        {
            (*it)->getOriginal()->accept(*this);
        }
        else
        {
            (*it)->accept(*this);
        }
        if (++it != itEnd)
        {
            *ostr << SCI_COMMA << " ";
        }
    }
    *ostr << SCI_RPAREN;
}

void PrintVisitor::visit (const AssignListExp  &e)
{
    *ostr << SCI_LBRACK;
    ast::exps_t exps = e.getExps();
    for (exps_t::const_iterator it = exps.begin (), itEnd = exps.end(); it != itEnd; /**/)
    {
        if (displayOriginal)
        {
            (*it)->getOriginal()->accept(*this);
        }
        else
        {
            (*it)->accept(*this);
        }
        if (++it != itEnd)
        {
            *ostr << SCI_COMMA << " ";
        }
    }
    *ostr << SCI_RBRACK;
}
/** \} */

/** \name Visit Single Operation nodes.
 ** \{ */
void PrintVisitor::visit (const NotExp &e)
{
    *ostr << SCI_NOT;
    *ostr << SCI_LPAREN;
    if (displayOriginal)
    {
        e.getExp().getOriginal()->accept(*this);
    }
    else
    {
        e.getExp().accept(*this);
    }
    *ostr << SCI_RPAREN;
}

void PrintVisitor::visit (const TransposeExp &e)
{
    *ostr << SCI_LPAREN;
    if (displayOriginal)
    {
        e.getExp().getOriginal()->accept(*this);
    }
    else
    {
        e.getExp().accept(*this);
    }
    *ostr << SCI_RPAREN;
    if (e.getConjugate() == TransposeExp::_Conjugate_)
    {
        *ostr << SCI_CONJUGATE_TRANSPOSE;
    }
    if (e.getConjugate() == TransposeExp::_NonConjugate_)
    {
        *ostr << SCI_TRANSPOSE;
    }
}
/** \} */

/** \name Visit Declaration nodes.
 ** \{ */
/** \brief Visit Var declarations. */
void PrintVisitor::visit (const VarDec  &e)
{
    *ostr << e.getSymbol().getName();
    *ostr << SCI_ASSIGN;
    if (displayOriginal)
    {
        e.getInit().getOriginal()->accept(*this);
    }
    else
    {
        e.getInit().accept(*this);
    }
}

void PrintVisitor::visit (const FunctionDec  &e)
{
    *ostr << SCI_FUNCTION << " ";

    // First ask if there are some return values.
    if (e.getReturns().getAs<ArrayListVar>()->getVars().size() > 1)
    {
        *ostr << SCI_OPEN_RETURNS;
    }

    if (displayOriginal)
    {
        e.getReturns().getOriginal()->accept(*this);
    }
    else
    {
        e.getReturns().accept(*this);
    }

    if (e.getReturns().getAs<ArrayListVar>()->getVars().size() > 1)
    {
        *ostr << SCI_CLOSE_RETURNS;
    }

    *ostr << " ";
    if (e.getReturns().getAs<ArrayListVar>()->getVars().size() > 0)
    {
        *ostr << SCI_ASSIGN << " ";
    }

    // Then get the function name
    *ostr << e.getSymbol().getName();

    // Then get function args
    *ostr << SCI_OPEN_ARGS;
    if (displayOriginal)
    {
        e.getArgs().getOriginal()->accept(*this);
    }
    else
    {
        e.getArgs().accept(*this);
    }
    *ostr << SCI_CLOSE_ARGS << std::endl;

    // Now print function body
    ++indent;
    if (displayOriginal)
    {
        e.getBody().getOriginal()->accept(*this);
    }
    else
    {
        e.getBody().accept(*this);
    }
    --indent;
    this->apply_indent();

    // Close function declaration
    *ostr << SCI_ENDFUNCTION;
}
/** \} */

/** \name Visit Type dedicated Expressions related node.
 ** \{ */
void PrintVisitor::visit(const ListExp &e)
{
    *ostr << SCI_LPAREN;
    if (displayOriginal)
    {
        e.getStart().getOriginal()->accept(*this);
    }
    else
    {
        e.getStart().accept(*this);
    }
    if (e.hasExplicitStep())
    {
        *ostr << SCI_IMPLICIT_LIST;
        if (displayOriginal)
        {
            e.getStep().getOriginal()->accept(*this);
        }
        else
        {
            e.getStep().accept(*this);
        }
    }
    *ostr << SCI_IMPLICIT_LIST;
    if (displayOriginal)
    {
        e.getEnd().getOriginal()->accept(*this);
    }
    else
    {
        e.getEnd().accept(*this);
    }
    *ostr << SCI_RPAREN;
}
/** \} */


void PrintVisitor::visit(const OptimizedExp &e)
{
    e.getOriginal()->accept(*this);
}

void PrintVisitor::visit(const MemfillExp &e)
{
    e.getOriginal()->accept(*this);
}

void PrintVisitor::visit(const DAXPYExp &e)
{
    e.getOriginal()->accept(*this);
}

void PrintVisitor::visit(const IntSelectExp &e)
{
    e.getOriginal()->accept(*this);
}

void PrintVisitor::visit(const StringSelectExp &e)
{
    e.getOriginal()->accept(*this);
}

void PrintVisitor::apply_indent()
{
    int i;
    for (i = 0; i < indent; ++i)
    {
        *ostr << "    ";
    }
}

void PrintVisitor::enable_force_parenthesis()
{
    force_parenthesis = true;
}

void PrintVisitor::set_force_parenthesis(bool new_state)
{
    force_parenthesis = new_state;
}

}
