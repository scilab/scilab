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

#include "printvisitor.hxx"

namespace ast
{

/** \name Visit Matrix Expressions nodes.
 ** \{ */
void PrintVisitor::visit (const MatrixExp &e)
{
    std::list<MatrixLineExp *>::const_iterator	i, j;
    *ostr << SCI_OPEN_MATRIX;
    ++indent;
    this->is_last_matrix_line = false;
    for (i = e.getLines().begin() ; i != e.getLines().end() ; )
    {
        j = i;
        if (++j == e.getLines().end())
        {
            this->is_last_matrix_line = true;
        }
        (*i)->accept (*this);
        ++i;

        if (e.getLines().size() > 1)
        {
            *ostr << std::endl;
            this->apply_indent();
        }
    }
    *ostr << SCI_CLOSE_MATRIX;
    --indent;
}

void PrintVisitor::visit (const MatrixLineExp &e)
{
    std::list<Exp *>::const_iterator        i;
    bool                                    last_column_is_comment = false;

    for (i = e.getColumns().begin() ; i != e.getColumns().end() ; )
    {
        (*i)->accept (*this);
        if (dynamic_cast<ast::CommentExp*>(*i) != NULL)
        {
            last_column_is_comment = true;
        }
        if (++i != e.getColumns().end())
        {
            if (dynamic_cast<ast::CommentExp*>(*i) == NULL)
            {
                *ostr << SCI_COLUMN_SEPARATOR;
            }
            *ostr << " ";
        }
    }
    if (!last_column_is_comment && this->is_last_matrix_line == false)
    {
        *ostr << SCI_LINE_SEPARATOR;
    }
}
/** \} */

/** \name Visit Cell Expressions nodes.
 ** \{ */
void PrintVisitor::visit (const CellExp &e)
{
    std::list<MatrixLineExp *>::const_iterator	i;
    *ostr << SCI_OPEN_CELL;
    ++indent;
    for (i = e.getLines().begin() ; i != e.getLines().end() ; )
    {
        (*i)->accept (*this);
        if (++i != e.getLines().end())
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
    std::wstring::const_iterator it;

    *ostr << SCI_OPEN_STRING;
    std::wstring value = e.getValue();

    for (it = value.begin() ; it != value.end() ; ++it)
    {
        if ( *it == '\'' || *it == '"')
        {
            // ' -> '' and " -> "" in scilab strings
            *ostr << *it;
        }
        *ostr << *it;
    }
    *ostr << SCI_CLOSE_STRING;
}

void PrintVisitor::visit (const CommentExp &e)
{
    *ostr << SCI_OPEN_COMMENT;
    *ostr << e.getComment();
}

void PrintVisitor::visit (const DoubleExp  &e)
{
    *ostr << e.getValue();
}

void PrintVisitor::visit (const BoolExp  &e)
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
    std::list<Var *>::const_iterator	i;
    for (i = e.getVars().begin() ; i != e.getVars().end() ; )
    {
        (*i)->accept (*this);
        if (++i != e.getVars().end())
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
    e.getHead()->accept(*this);
    *ostr << SCI_FVAR_SEPARATOR;
    e.getTail()->accept(*this);
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
        e.getLeft().accept(*this);
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
    e.getRight().accept(*this);
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
    e.getLeft().accept(*this);
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
    e.getRight().accept(*this);
    this->set_force_parenthesis(old_force_parenthesis);

    if (force_parenthesis)
    {
        *ostr << SCI_RPAREN;
    }
}

void PrintVisitor::visit (const AssignExp  &e)
{
    e.getLeftExp().accept (*this);
    *ostr << " " << SCI_ASSIGN << " ";
    e.getRightExp().accept (*this);
}

void PrintVisitor::visit(const CellCallExp &e)
{
    e.getName().accept (*this);
    *ostr << SCI_OPEN_CELL;
    std::list<Exp *>::const_iterator	i;

    for (i = e.getArgs().begin (); i != e.getArgs().end ();)
    {
        (*i)->accept (*this);
        if (++i != e.getArgs().end ())
        {
            *ostr << SCI_COMMA << " ";
        }
    }
    *ostr << SCI_CLOSE_CELL;
}

void PrintVisitor::visit(const CallExp &e)
{
    e.getName().accept (*this);
    *ostr << SCI_OPEN_CALL;
    std::list<Exp *>::const_iterator	i;

    for (i = e.getArgs().begin(); i != e.getArgs().end();)
    {
        (*i)->accept (*this);
        if (++i != e.getArgs().end())
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
    e.getTest().accept(*this);
    *ostr << SCI_CLOSE_TEST << " ";
    *ostr << SCI_THEN << std::endl;
    ++indent;
    e.getThen().accept(*this);
    --indent;
    if (e.hasElse())
    {
        this->apply_indent();
        *ostr << SCI_ELSE << std::endl;
        ++indent;
        e.getElse().accept(*this);
        --indent;
    }
    this->apply_indent();
    *ostr << SCI_ENDIF;
}

void PrintVisitor::visit (const TryCatchExp  &e)
{
    *ostr << SCI_TRY << std::endl;
    ++indent;
    e.getTry().accept(*this);
    --indent;
    this->apply_indent();
    *ostr << SCI_CATCH << std::endl;
    ++indent;
    e.getCatch().accept(*this);
    --indent;
    this->apply_indent();
    *ostr << SCI_ENDTRY;
}

void PrintVisitor::visit (const WhileExp  &e)
{
    *ostr << SCI_WHILE;
    *ostr << " " << SCI_OPEN_TEST;
    e.getTest().accept (*this);
    *ostr << SCI_CLOSE_TEST << " " << SCI_DO << std::endl;
    ++indent;
    e.getBody().accept (*this);
    --indent;
    this->apply_indent();
    *ostr << SCI_ENDWHILE;
}

void PrintVisitor::visit (const ForExp  &e)
{
    *ostr << SCI_FOR;
    *ostr << " " << SCI_OPEN_TEST;
    e.getVardec().accept(*this);
    *ostr << SCI_CLOSE_TEST << " ";
    *ostr << SCI_DO << std::endl;
    ++indent;
    e.getBody().accept (*this);
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
        e.getExp().accept(*this);
    }
}

void PrintVisitor::visit (const SelectExp &e)
{
    *ostr << SCI_SELECT;
    *ostr << " " << SCI_OPEN_TEST;
    e.getSelect()->accept(*this);
    *ostr << SCI_CLOSE_TEST << std::endl;
    ++indent;
    cases_t::const_iterator it;
    for (it = e.getCases()->begin() ; it != e.getCases()->end() ; ++it)
    {
        (*it)->accept (*this);
    }
    if (e.getDefaultCase() != NULL)
    {
        this->apply_indent();
        *ostr << SCI_DEFAULT_CASE << std::endl;
        ++indent;
        e.getDefaultCase()->accept(*this);
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
    e.getTest()->accept(*this);
    *ostr << SCI_CLOSE_TEST << std::endl;
    indent++;
    e.getBody()->accept(*this);
    indent--;
}

void PrintVisitor::visit (const SeqExp  &e)
{
    std::list<Exp *>::const_iterator	i;
    for (i = e.getExps().begin (); i != e.getExps().end (); ++i)
    {
        this->apply_indent();
        (*i)->accept (*this);
        if (!(*i)->isVerbose())
        {
            *ostr << ";";
        }
        *ostr << std::endl;
    }
}

void PrintVisitor::visit (const ArrayListExp  &e)
{
    std::list<Exp *>::const_iterator	i;
    *ostr << SCI_LPAREN;
    for (i = e.getExps().begin (); i != e.getExps().end ();)
    {
        (*i)->accept (*this);
        if (++i != e.getExps().end ())
        {
            *ostr << SCI_COMMA << " ";
        }
    }
    *ostr << SCI_RPAREN;
}

void PrintVisitor::visit (const AssignListExp  &e)
{
    std::list<Exp *>::const_iterator	i;
    *ostr << SCI_LBRACK;
    for (i = e.getExps().begin (); i != e.getExps().end ();)
    {
        (*i)->accept (*this);
        if (++i != e.getExps().end ())
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
    e.getExp().accept (*this);
    *ostr << SCI_RPAREN;
}

void PrintVisitor::visit (const TransposeExp &e)
{
    *ostr << SCI_LPAREN;
    e.getExp().accept (*this);
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
    e.getInit().accept (*this);
}

void PrintVisitor::visit (const FunctionDec  &e)
{
    *ostr << SCI_FUNCTION << " ";

    // First ask if there are some return values.
    if (e.getReturns().getVars().size() > 1)
    {
        *ostr << SCI_OPEN_RETURNS;
    }

    visit(e.getReturns());

    if (e.getReturns().getVars().size() > 1)
    {
        *ostr << SCI_CLOSE_RETURNS;
    }

    *ostr << " ";
    if (e.getReturns().getVars().size() > 0)
    {
        *ostr << SCI_ASSIGN << " ";
    }

    // Then get the function name
    *ostr << e.getSymbol().getName();

    // Then get function args
    *ostr << SCI_OPEN_ARGS;
    visit(e.getArgs());
    *ostr << SCI_CLOSE_ARGS << std::endl;

    // Now print function body
    ++indent;
    e.getBody().accept(*this);
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
    e.getStart().accept(*this);
    if (e.hasExplicitStep())
    {
        *ostr << SCI_IMPLICIT_LIST;
        e.getStep().accept(*this);
    }
    *ostr << SCI_IMPLICIT_LIST;
    e.getEnd().accept(*this);
    *ostr << SCI_RPAREN;
}
/** \} */

void       	PrintVisitor::apply_indent()
{
    int i;
    for (i = 0; i < indent; ++i)
    {
        *ostr << "    ";
    }
}

void	PrintVisitor::enable_force_parenthesis()
{
    force_parenthesis = true;
}

void	PrintVisitor::set_force_parenthesis(bool new_state)
{
    force_parenthesis = new_state;
}

}
