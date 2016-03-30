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

#include "CodePrinterVisitor.hxx"

namespace coverage
{

void CodePrinterVisitor::visit(const ast::MatrixExp & e)
{
    printer.handleExpStart(&e);
    printer.handleOpenClose(SCI_OPEN_MATRIX);

    const bool mustReturn = e.getLocation().first_line != e.getLocation().last_line;
    const int shift = mustReturn ? (printer.getLineCharCount() - printer.getIndentSize()) : 0;
    const ast::exps_t & lines = e.getLines();
    if (lines.size())
    {
        ast::exps_t::const_iterator last_line = std::prev(lines.end());
        for (ast::exps_t::const_iterator i = lines.begin(), er = lines.end(); i != er; ++i)
        {
            const ast::exps_t & columns = static_cast<ast::MatrixLineExp *>(*i)->getColumns();
            if (columns.size())
            {
                ast::exps_t::const_iterator last_column = std::prev(columns.end());
                for (ast::exps_t::const_iterator j = columns.begin(), ec = columns.end(); j != ec; ++j)
                {
                    (*j)->accept(*this);
                    if (j != last_column)
                    {
                        printer.handleDefault(SCI_COMMA);
                    }
                }

                if (i != last_line)
                {
                    printer.handleDefault(SCI_SEMICOLON);
                    if (mustReturn)
                    {
                        printer.handleNewLine();
                        printer.handleExpStart(&e);
                        if (shift > 0)
                        {
                            printer.handleNothing(std::wstring(shift, L' '));
                        }
                    }
                }
            }
        }
    }
    printer.handleOpenClose(SCI_CLOSE_MATRIX);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::MatrixLineExp & e)
{

}

void CodePrinterVisitor::visit(const ast::CellExp & e)
{
    printer.handleExpStart(&e);
    printer.handleOpenClose(SCI_OPEN_CELL);

    const bool mustReturn = e.getLocation().first_line != e.getLocation().last_line;
    const unsigned int shift = mustReturn ? (printer.getLineCharCount() - printer.getIndentSize()) : 0;
    const ast::exps_t & lines = e.getLines();
    if (lines.size())
    {
        ast::exps_t::const_iterator last_line = std::prev(lines.end());
        for (ast::exps_t::const_iterator i = lines.begin(), er = lines.end(); i != er; ++i)
        {
            const ast::exps_t & columns = static_cast<ast::MatrixLineExp *>(*i)->getColumns();
            if (columns.size())
            {
                ast::exps_t::const_iterator last_column = std::prev(columns.end());
                for (ast::exps_t::const_iterator j = columns.begin(), ec = columns.end(); j != ec; ++j)
                {
                    (*j)->accept(*this);
                    if (j != last_column)
                    {
                        printer.handleDefault(SCI_COMMA);
                    }
                }

                if (i != last_line)
                {
                    printer.handleDefault(SCI_SEMICOLON);
                    if (mustReturn)
                    {
                        printer.handleNewLine();
                        printer.handleNothing(std::wstring(shift, L' '));
                    }
                }
            }
        }
    }
    printer.handleOpenClose(SCI_CLOSE_CELL);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::StringExp & e)
{
    printer.handleExpStart(&e);
    printer.handleOpenClose(SCI_OPEN_STRING);
    printer.handleString(e.getValue());
    printer.handleOpenClose(SCI_CLOSE_STRING);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::CommentExp & e)
{
    printer.handleExpStart(&e);
    printer.handleComment(L"// " + e.getComment());
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::DoubleExp & e)
{
    printer.handleExpStart(&e);
    const double x = e.getValue();
    if (analysis::tools::isAnInt(x))
    {
        printer.handleNumber(x >= 0 ? std::to_wstring((uint64_t)x) : std::to_wstring((int64_t)x));
    }
    else
    {
        printer.handleNumber(std::to_wstring(e.getValue()));
    }
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::BoolExp & e)
{
    printer.handleExpStart(&e);
    printer.handleConstants(e.getValue() ? SCI_TRUE : SCI_FALSE);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::NilExp &/*e*/)
{
    /* Do Nothing */
}

void CodePrinterVisitor::visit(const ast::SimpleVar & e)
{
    printer.handleExpStart(&e);
    const symbol::Symbol & sym = e.getSymbol();
    types::InternalType * pIT = symbol::Context::getInstance()->get(sym);
    if (pIT)
    {
        switch (pIT->getType())
        {
            case types::InternalType::ScilabFunction:
                printer.handleCommands(sym.getName());
                break;
            case types::InternalType::ScilabMacro:
                printer.handleMacros(sym.getName());
                break;
            case types::InternalType::ScilabMacroFile:
                printer.handleMacros(sym.getName());
                break;
            default:
            {
                const std::wstring & name = sym.getName();
                if (name == L"%t" || name == L"%T" || name == L"%f" || name == L"%F" || name == L"%e" || name == L"%pi" || name == L"%inf" || name == L"%i" || name == L"%z" || name == L"%s" || name == L"%nan" || name == L"%eps" || name == L"SCI" || name == L"WSCI" || name == L"SCIHOME" || name == L"TMPDIR")
                {
                    printer.handleConstants(name);
                }
                else
                {
                    printer.handleName(name);
                }
                break;
            }
        }
    }
    else
    {
        printer.handleName(sym.getName());
    }
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::ColonVar & e)
{
    printer.handleExpStart(&e);
    printer.handleSpecial(SCI_COLON);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::DollarVar & e)
{
    printer.handleExpStart(&e);
    printer.handleSpecial(SCI_DOLLAR);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::ArrayListVar & e)
{
    printer.handleExpStart(&e);
    const ast::exps_t & vars = e.getVars();
    if (vars.size())
    {
        ast::exps_t::const_iterator last = std::prev(vars.end());
        for (ast::exps_t::const_iterator i = vars.begin(), e = vars.end(); i != e; ++i)
        {
            const symbol::Symbol & sym = static_cast<ast::SimpleVar *>(*i)->getSymbol();
            printer.handleInOutArgsDec(sym.getName());
            if (i != last)
            {
                printer.handleDefault(SCI_COMMA);
                printer.handleNothing(L" ");
            }
        }
    }
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::FieldExp & e)
{
    printer.handleExpStart(&e);
    e.getHead()->accept(*this);
    printer.handleOperator(SCI_FVAR_SEPARATOR);
    e.getTail()->accept(*this);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::OpExp & e)
{
    printer.handleExpStart(&e);
    if (e.getOper() != ast::OpExp::unaryMinus)
    {
        if (e.getLeft().isOpExp() || e.getLeft().isLogicalOpExp())
        {
            printer.handleOpenClose(SCI_LPAREN);
            e.getLeft().accept(*this);
            printer.handleOpenClose(SCI_RPAREN);
        }
        else
        {
            e.getLeft().accept(*this);
        }
        printer.handleNothing(L" ");
    }

    switch (e.getOper())
    {
        case ast::OpExp::plus:
            printer.handleOperator(SCI_PLUS);
            break;
        case ast::OpExp::unaryMinus:
        case ast::OpExp::minus:
            printer.handleOperator(SCI_MINUS);
            break;
        case ast::OpExp::times:
            printer.handleOperator(SCI_TIMES);
            break;
        case ast::OpExp::rdivide:
            printer.handleOperator(SCI_RDIVIDE);
            break;
        case ast::OpExp::ldivide:
            printer.handleOperator(SCI_LDIVIDE);
            break;
        case ast::OpExp::power:
            printer.handleOperator(SCI_POWER);
            break;
        case ast::OpExp::dottimes:
            printer.handleOperator(SCI_DOTTIMES);
            break;
        case ast::OpExp::dotrdivide:
            printer.handleOperator(SCI_DOTRDIVIDE);
            break;
        case ast::OpExp::dotldivide:
            printer.handleOperator(SCI_DOTLDIVIDE);
            break;
        case ast::OpExp::dotpower:
            printer.handleOperator(SCI_DOTPOWER);
            break;
        case ast::OpExp::krontimes:
            printer.handleOperator(SCI_KRONTIMES);
            break;
        case ast::OpExp::kronrdivide:
            printer.handleOperator(SCI_KRONRDIVIDE);
            break;
        case ast::OpExp::kronldivide:
            printer.handleOperator(SCI_KRONLDIVIDE);
            break;
        case ast::OpExp::controltimes:
            printer.handleOperator(SCI_CONTROLTIMES);
            break;
        case ast::OpExp::controlrdivide:
            printer.handleOperator(SCI_CONTROLRDIVIDE);
            break;
        case ast::OpExp::controlldivide:
            printer.handleOperator(SCI_CONTROLLDIVIDE);
            break;
        case ast::OpExp::eq:
            printer.handleOperator(SCI_EQ);
            break;
        case ast::OpExp::ne:
            printer.handleOperator(SCI_NE);
            break;
        case ast::OpExp::lt:
            printer.handleOperator(SCI_LT);
            break;
        case ast::OpExp::le:
            printer.handleOperator(SCI_LE);
            break;
        case ast::OpExp::gt:
            printer.handleOperator(SCI_GT);
            break;
        case ast::OpExp::ge:
            printer.handleOperator(SCI_GE);
            break;
        default :
            break;
    }

    if (e.getOper() != ast::OpExp::unaryMinus)
    {
        printer.handleNothing(L" ");
    }

    if (e.getRight().isOpExp() || e.getRight().isLogicalOpExp())
    {
        printer.handleOpenClose(SCI_LPAREN);
        e.getRight().accept(*this);
        printer.handleOpenClose(SCI_RPAREN);
    }
    else
    {
        e.getRight().accept(*this);
    }
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::LogicalOpExp & e)
{
    printer.handleExpStart(&e);
    if (e.getLeft().isOpExp() || e.getLeft().isLogicalOpExp())
    {
        printer.handleOpenClose(SCI_LPAREN);
        e.getLeft().accept(*this);
        printer.handleOpenClose(SCI_RPAREN);
    }
    else
    {
        e.getLeft().accept(*this);
    }
    printer.handleNothing(L" ");

    switch (e.getOper())
    {
        case ast::LogicalOpExp::logicalAnd:
            printer.handleOperator(SCI_AND);
            break;
        case ast::LogicalOpExp::logicalOr:
            printer.handleOperator(SCI_OR);
            break;
        case ast::LogicalOpExp::logicalShortCutAnd:
            printer.handleOperator(SCI_ANDAND);
            break;
        case ast::LogicalOpExp::logicalShortCutOr:
            printer.handleOperator(SCI_OROR);
            break;
        default :
            break;
    }

    printer.handleNothing(L" ");
    if (e.getRight().isOpExp() || e.getRight().isLogicalOpExp())
    {
        printer.handleOpenClose(SCI_LPAREN);
        e.getRight().accept(*this);
        printer.handleOpenClose(SCI_RPAREN);
    }
    else
    {
        e.getRight().accept(*this);
    }
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::AssignExp & e)
{
    printer.handleExpStart(&e);
    e.getLeftExp().accept(*this);
    printer.handleNothing(L" ");
    printer.handleOperator(SCI_ASSIGN);
    printer.handleNothing(L" ");
    e.getRightExp().accept(*this);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::CellCallExp & e)
{
    printer.handleExpStart(&e);
    e.getName().accept(*this);
    printer.handleOpenClose(SCI_OPEN_CELL);
    const ast::exps_t & args = e.getArgs();
    if (args.size())
    {
        ast::exps_t::const_iterator last = std::prev(args.end());
        for (ast::exps_t::const_iterator i = args.begin(), e = args.end(); i != e; ++i)
        {
            (*i)->accept(*this);
            if (i != last)
            {
                printer.handleDefault(SCI_COMMA);
                printer.handleNothing(L" ");
            }
        }
    }
    printer.handleOpenClose(SCI_CLOSE_CELL);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::CallExp & e)
{
    printer.handleExpStart(&e);
    e.getName().accept(*this);
    printer.handleOpenClose(SCI_OPEN_CALL);
    const ast::exps_t & args = e.getArgs();
    if (args.size())
    {
        ast::exps_t::const_iterator last = std::prev(args.end());
        for (ast::exps_t::const_iterator i = args.begin(), e = args.end(); i != e; ++i)
        {
            (*i)->accept(*this);
            if (i != last)
            {
                printer.handleDefault(SCI_COMMA);
                printer.handleNothing(L" ");
            }
        }
    }
    printer.handleOpenClose(SCI_CLOSE_CALL);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::IfExp & e)
{
    printer.handleExpStart(&e);
    printer.handleStructureKwds(SCI_IF);
    printer.handleNothing(L" ");
    //printer.handleOpenClose(SCI_OPEN_TEST);
    e.getTest().accept(*this);
    //printer.handleOpenClose(SCI_CLOSE_TEST);
    printer.handleNothing(L" ");
    printer.handleStructureKwds(SCI_THEN);

    printer.incIndent();
    printer.handleNewLine();
    e.getThen().accept(*this);
    printer.decIndent();
    printer.handleNewLine();

    if (e.hasElse() && !e.getElse().isCommentExp())
    {
        printer.handleStructureKwds(SCI_ELSE);
        printer.incIndent();
        printer.handleNewLine();
        e.getElse().accept(*this);
        printer.decIndent();
        printer.handleNewLine();
    }

    printer.handleStructureKwds(SCI_ENDIF);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::TryCatchExp & e)
{
    printer.handleExpStart(&e);
    printer.handleStructureKwds(SCI_TRY);

    printer.incIndent();
    printer.handleNewLine();
    e.getTry().accept(*this);
    printer.decIndent();
    printer.handleNewLine();

    printer.handleStructureKwds(SCI_CATCH);

    printer.incIndent();
    printer.handleNewLine();
    e.getCatch().accept(*this);
    printer.decIndent();
    printer.handleNewLine();

    printer.handleStructureKwds(SCI_ENDTRY);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::WhileExp & e)
{
    printer.handleExpStart(&e);
    printer.handleStructureKwds(SCI_WHILE);
    printer.handleNothing(L" ");
    //printer.handleOpenClose(SCI_OPEN_TEST);
    e.getTest().accept(*this);
    //printer.handleOpenClose(SCI_CLOSE_TEST);

    printer.incIndent();
    printer.handleNewLine();
    e.getBody().accept(*this);
    printer.decIndent();
    printer.handleNewLine();

    printer.handleStructureKwds(SCI_ENDWHILE);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::ForExp & e)
{
    printer.handleExpStart(&e);
    printer.handleStructureKwds(SCI_FOR);
    printer.handleNothing(L" ");
    e.getVardec().accept(*this);
    printer.handleNothing(L" ");
    printer.handleStructureKwds(SCI_DO);

    printer.incIndent();
    printer.handleNewLine();
    e.getBody().accept(*this);
    printer.decIndent();
    printer.handleNewLine();

    printer.handleStructureKwds(SCI_ENDFOR);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::BreakExp & e)
{
    printer.handleExpStart(&e);
    printer.handleControlKwds(SCI_BREAK);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::ContinueExp & e)
{
    printer.handleExpStart(&e);
    printer.handleControlKwds(SCI_CONTINUE);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::ReturnExp & e)
{
    printer.handleExpStart(&e);
    printer.handleControlKwds(SCI_RETURN);

    if (!e.isGlobal())
    {
        printer.handleNothing(L" ");
        e.getExp().accept(*this);
    }
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::SelectExp & e)
{
    printer.handleExpStart(&e);
    printer.handleStructureKwds(SCI_SELECT);
    printer.handleNothing(L" ");
    printer.handleOpenClose(SCI_OPEN_TEST);
    e.getSelect()->accept(*this);
    printer.handleOpenClose(SCI_CLOSE_TEST);

    printer.incIndent();
    printer.handleNewLine();

    ast::exps_t cases = e.getCases();
    for (auto exp : cases)
    {
        exp->accept(*this);
    }

    if (e.hasDefault())
    {
        printer.handleStructureKwds(SCI_DEFAULT_CASE);
        printer.incIndent();
        printer.handleNewLine();
        e.getDefaultCase()->accept(*this);
        printer.decIndent();
        printer.handleNewLine();
    }

    printer.decIndent();
    printer.handleNewLine();
    printer.handleStructureKwds(SCI_ENDSELECT);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::CaseExp & e)
{
    printer.handleExpStart(&e);
    printer.handleStructureKwds(SCI_CASE);
    printer.handleNothing(L" ");
    e.getTest()->accept(*this);

    printer.incIndent();
    printer.handleNewLine();
    e.getBody()->accept(*this);
    printer.decIndent();
    printer.handleNewLine();
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::SeqExp & e)
{
    printer.handleExpStart(&e);
    const ast::exps_t & exps = e.getExps();
    if (exps.size())
    {
        ast::exps_t::const_iterator last = std::prev(exps.end());
        for (ast::exps_t::const_iterator i = exps.begin(), e = exps.end(); i != e; ++i)
        {
            (*i)->accept(*this);
            if (!(*i)->isCommentExp() && !(*i)->isIfExp() && !(*i)->isForExp() && !(*i)->isWhileExp() && !(*i)->isTryCatchExp() && !(*i)->isFunctionDec())
            {
                printer.handleNothing(L";");
            }

            if (i != last)
            {
                printer.handleNewLine();
            }
        }
    }

    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::ArrayListExp & e)
{
    printer.handleExpStart(&e);
    printer.handleOpenClose(SCI_LPAREN);
    const ast::exps_t & exps = e.getExps();
    if (exps.size())
    {
        ast::exps_t::const_iterator last = std::prev(exps.end());
        for (ast::exps_t::const_iterator i = exps.begin(), e = exps.end(); i != e; ++i)
        {
            (*i)->accept(*this);
            if (i != last)
            {
                printer.handleDefault(SCI_COMMA);
                printer.handleNothing(L" ");
            }
        }
    }

    printer.handleOpenClose(SCI_RPAREN);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::AssignListExp & e)
{
    printer.handleExpStart(&e);
    printer.handleOpenClose(SCI_LBRACK);
    const ast::exps_t & exps = e.getExps();
    if (exps.size())
    {
        ast::exps_t::const_iterator last = std::prev(exps.end());
        for (ast::exps_t::const_iterator i = exps.begin(), e = exps.end(); i != e; ++i)
        {
            (*i)->getOriginal()->accept(*this);
            if (i != last)
            {
                printer.handleDefault(SCI_COMMA);
                printer.handleNothing(L" ");
            }
        }
    }

    printer.handleOpenClose(SCI_RBRACK);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::NotExp & e)
{
    printer.handleExpStart(&e);
    printer.handleOperator(SCI_NOT);
    if (e.getExp().isOpExp() || e.getExp().isLogicalOpExp())
    {
        printer.handleOpenClose(SCI_LPAREN);
        e.getExp().accept(*this);
        printer.handleOpenClose(SCI_RPAREN);
    }
    else
    {
        e.getExp().accept(*this);
    }
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::TransposeExp & e)
{
    printer.handleExpStart(&e);
    if (e.getExp().isOpExp() || e.getExp().isLogicalOpExp())
    {
        printer.handleOpenClose(SCI_LPAREN);
        e.getExp().accept(*this);
        printer.handleOpenClose(SCI_RPAREN);
    }
    else
    {
        e.getExp().accept(*this);
    }

    if (e.getConjugate() == ast::TransposeExp::_Conjugate_)
    {
        printer.handleOperator(SCI_CONJUGATE_TRANSPOSE);
    }
    else if (e.getConjugate() == ast::TransposeExp::_NonConjugate_)
    {
        printer.handleOperator(SCI_TRANSPOSE);
    }
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::VarDec & e)
{
    printer.handleExpStart(&e);
    printer.handleName(e.getSymbol().getName());
    printer.handleNothing(L" ");
    printer.handleOperator(SCI_ASSIGN);
    printer.handleNothing(L" ");
    e.getInit().accept(*this);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::FunctionDec & e)
{
    printer.handleExpStart(&e);
    printer.handleFunctionKwds(SCI_FUNCTION);
    printer.handleNothing(L" ");
    const ast::ArrayListVar & ret = e.getReturns();
    const ast::ArrayListVar & args = e.getArgs();

    if (ret.getVars().size() > 1)
    {
        printer.handleOpenClose(SCI_OPEN_RETURNS);
        ret.accept(*this);
        printer.handleOpenClose(SCI_CLOSE_RETURNS);
    }
    else
    {
        ret.accept(*this);
    }

    if (ret.getVars().size() != 0)
    {
        printer.handleNothing(L" ");
        printer.handleOperator(SCI_ASSIGN);
        printer.handleNothing(L" ");
    }

    printer.handleFunctionNameDec(e.getSymbol().getName());

    printer.handleOpenClose(SCI_OPEN_ARGS);
    args.accept(*this);
    printer.handleOpenClose(SCI_CLOSE_ARGS);

    printer.incIndent();
    printer.handleNewLine();
    e.getBody().accept(*this);
    printer.decIndent();
    printer.handleNewLine();

    printer.handleFunctionKwds(SCI_ENDFUNCTION);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::ListExp & e)
{
    printer.handleExpStart(&e);
    if (e.getStart().isOpExp() || e.getStart().isLogicalOpExp())
    {
        printer.handleOpenClose(SCI_LPAREN);
        e.getStart().accept(*this);
        printer.handleOpenClose(SCI_RPAREN);
    }
    else
    {
        e.getStart().accept(*this);
    }

    if (e.hasExplicitStep())
    {
        printer.handleOperator(SCI_IMPLICIT_LIST);
        if (e.getStep().isOpExp() || e.getStep().isLogicalOpExp())
        {
            printer.handleOpenClose(SCI_LPAREN);
            e.getStep().accept(*this);
            printer.handleOpenClose(SCI_RPAREN);
        }
        else
        {
            e.getStep().accept(*this);
        }
    }

    printer.handleOperator(SCI_IMPLICIT_LIST);
    if (e.getEnd().isOpExp() || e.getEnd().isLogicalOpExp())
    {
        printer.handleOpenClose(SCI_LPAREN);
        e.getEnd().accept(*this);
        printer.handleOpenClose(SCI_RPAREN);
    }
    else
    {
        e.getEnd().accept(*this);
    }
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::OptimizedExp & e)
{
    printer.handleExpStart(&e);
    e.getOriginal()->accept(*this);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::DAXPYExp & e)
{
    printer.handleExpStart(&e);
    e.getOriginal()->accept(*this);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::MemfillExp & e)
{
    printer.handleExpStart(&e);
    e.getOriginal()->accept(*this);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::IntSelectExp & e)
{
    printer.handleExpStart(&e);
    e.getOriginal()->accept(*this);
    printer.handleExpEnd(&e);
}

void CodePrinterVisitor::visit(const ast::StringSelectExp & e)
{
    printer.handleExpStart(&e);
    e.getOriginal()->accept(*this);
    printer.handleExpEnd(&e);
}
}
