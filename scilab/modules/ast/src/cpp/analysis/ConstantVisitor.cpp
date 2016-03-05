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

#include "AnalysisVisitor.hxx"
#include "ConstantVisitor.hxx"

namespace analysis
{

std::unordered_set<std::wstring> ConstantVisitor::constants = init();

void ConstantVisitor::visit(ast::SimpleVar & e)
{
    bool isConstant = false;
    if (evalSymbols)
    {
        const symbol::Symbol & sym = e.getSymbol();
        const std::wstring & name = sym.getName();
        if (constants.find(name) != constants.end() && symbol::Context::getInstance()->isOriginalSymbol(sym))
        {
            if (types::InternalType * pIT = symbol::Context::getInstance()->get(sym))
            {
                e.replace(pIT->getExp(e.getLocation()));
                isConstant = true;
            }
        }
        else if (parent)
        {
            if (FunctionBlock * fblock = parent->getDM().topFunction())
            {
                ast::Exp * loop = parent->getCurrentLoop();
                if (!fblock->getLoopAnalyzer().isAssigned(loop, sym))
                {
                    // We propagate the constant only if we aren't in a loop which modifies the sym
                    Info & info = parent->getDM().read(sym, &e);
                    ConstantValue & constant = info.getConstant();
                    const ConstantValue::Kind kind = constant.getKind();
                    if (kind == ConstantValue::GVNVAL)
                    {
                        double val;
                        if (constant.getDblValue(val))
                        {
                            e.replace(new ast::DoubleExp(e.getLocation(), val));
                            isConstant = true;
                        }
                    }
                    else if (kind == ConstantValue::ITVAL)
                    {
                        types::InternalType * pIT = constant.getIT();
                        if (ast::Exp * exp = pIT->getExp(e.getLocation()))
                        {
                            e.replace(exp);
                            isConstant = true;
                        }
                    }
                }
            }
        }
    }

    setResult(isConstant);
}

void ConstantVisitor::visit(ast::OpExp & e)
{
    e.getLeft().accept(*this);
    const bool constL = getResult();
    e.getRight().accept(*this);
    const bool constR = getResult();

    if (constL && constR)
    {
        setResult(execAndReplace(e));
        return;
    }
    else if (constL || constR)
    {
        const ast::Exp::ExpType ty = e.getParent()->getType();
        if (ty == ast::Exp::IFEXP || ty == ast::Exp::WHILEEXP)
        {
            ast::OpExp::Oper oper = e.getOper();
            if (oper == ast::OpExp::Oper::logicalShortCutAnd || oper == ast::OpExp::Oper::logicalAnd)
            {
                if (constL)
                {
                    e.getLeft().accept(exec);
                    if (exec.getResult()->isTrue())
                    {
                        e.replace(e.getRight().clone());
                    }
                    else
                    {
                        types::InternalType * b = new types::Bool(0);
                        e.replace(b->getExp(e.getLocation()));
                        setResult(true);
                        return;
                    }
                }
                else
                {
                    e.getRight().accept(exec);
                    if (exec.getResult()->isTrue())
                    {
                        e.replace(e.getLeft().clone());
                    }
                    else
                    {
                        types::InternalType * b = new types::Bool(0);
                        e.replace(b->getExp(e.getLocation()));
                        setResult(true);
                        return;
                    }
                }
            }
            else if (oper == ast::OpExp::Oper::logicalShortCutOr || oper == ast::OpExp::Oper::logicalOr)
            {
                if (constL)
                {
                    e.getLeft().accept(exec);
                    if (exec.getResult()->isTrue())
                    {
                        types::InternalType * b = new types::Bool(1);
                        e.replace(b->getExp(e.getLocation()));
                        setResult(true);
                        return;
                    }
                    else
                    {
                        e.replace(e.getRight().clone());
                    }
                }
                else
                {
                    e.getRight().accept(exec);
                    if (exec.getResult()->isTrue())
                    {
                        types::InternalType * b = new types::Bool(1);
                        e.replace(b->getExp(e.getLocation()));
                        setResult(true);
                        return;
                    }
                    else
                    {
                        e.replace(e.getLeft().clone());
                    }
                }
            }
        }
    }

    setResult(false);
}

void ConstantVisitor::visit(ast::LogicalOpExp & e)
{
    visit(static_cast<ast::OpExp &>(e));
}

void ConstantVisitor::visit(ast::CallExp & e)
{
    bool isConstant = false;

    if (evalSymbols)
    {
        ast::SimpleVar & var = static_cast<ast::SimpleVar &>(e.getName());
        const symbol::Symbol & sym = var.getSymbol();
        if (symbol::Context::getInstance()->isOriginalSymbol(sym))
        {
            const std::wstring & name = sym.getName();
            ast::exps_t args = e.getArgs();

            bool allConstant = true;
            for (auto arg : args)
            {
                arg->accept(*this);
                if (allConstant && !getResult())
                {
                    allConstant = false;
                }
            }

            if (allConstant && Checkers::isConst(name, args.size()))
            {
                if (name == L"argn")
                {
                    if (parent && parent->getAnalyzer(sym)->analyze(*parent, lhs, e))
                    {
                        if (lhs == 1)
                        {
                            double val;
                            parent->getResult().getConstant().getDblValue(val);
                            e.replace(new ast::DoubleExp(e.getLocation(), val));
                            isConstant = true;
                        }
                        else
                        {
                            double val;
                            ast::exps_t * exps = new ast::exps_t();
                            exps->reserve(2);
                            std::vector<Result> & res = parent->getLHSContainer();
                            res.front().getConstant().getDblValue(val);
                            exps->push_back(new ast::DoubleExp(e.getLocation(), val));
                            res.back().getConstant().getDblValue(val);
                            exps->push_back(new ast::DoubleExp(e.getLocation(), val));
                            e.replace(new ast::ArrayListExp(e.getLocation(), *exps));
                            isConstant = true;
                        }
                    }
                }
                else
                {
                    isConstant = execAndReplace(e);
                }
            }

            if (parent && args.size() == 1)
            {
                if (name == L"type" || name == L"inttype")
                {
                    if (parent->getAnalyzer(sym)->analyze(*parent, 1, e))
                    {
                        double val;
                        parent->getResult().getConstant().getDblValue(val);
                        e.replace(new ast::DoubleExp(e.getLocation(), val));
                        isConstant = true;
                    }
                }
                else if (name == L"typeof")
                {
                    if (parent->getAnalyzer(sym)->analyze(*parent, 1, e))
                    {
                        std::wstring wstr;
                        if (parent->getResult().getConstant().getStrValue(wstr))
                        {
                            e.replace(new ast::StringExp(e.getLocation(), wstr));
                            isConstant = true;
                        }
                    }
                }
                else if (name == L"isreal" || name == L"isscalar")
                {
                    if (parent->getAnalyzer(sym)->analyze(*parent, 1, e))
                    {
                        bool val;
                        parent->getResult().getConstant().getBoolValue(val);
                        e.replace(new ast::BoolExp(e.getLocation(), val));
                        isConstant = true;
                    }
                }
            }
        }
    }

    setResult(isConstant);
}

void ConstantVisitor::visit(ast::MatrixExp & e)
{
    const ast::exps_t & lines = e.getLines();
    if (lines.empty())
    {
        setResult(execAndReplace(e));
    }
    else
    {
        for (auto line : lines)
        {
            const ast::exps_t & columns = static_cast<ast::MatrixLineExp *>(line)->getColumns();
            for (auto column : columns)
            {
                column->accept(*this);
                if (!getResult())
                {
                    return;
                }
            }
        }
        setResult(execAndReplace(e));
    }
}

void ConstantVisitor::visit(ast::NotExp & e)
{
    e.getExp().accept(*this);
    if (getResult())
    {
        setResult(execAndReplace(e));
    }
    else
    {
        setResult(false);
    }
}

void ConstantVisitor::visit(ast::TransposeExp & e)
{
    e.getExp().accept(*this);
    if (getResult())
    {
        setResult(execAndReplace(e));
    }
    else
    {
        setResult(false);
    }
}

void ConstantVisitor::visit(ast::CellExp & e)
{
    visit(static_cast<ast::MatrixExp &>(e));
}

void ConstantVisitor::visit(ast::ListExp & e)
{
    e.getStart().accept(*this);
    const bool startConst = getResult();
    e.getStep().accept(*this);
    const bool stepConst = getResult();
    e.getEnd().accept(*this);
    const bool endConst = getResult();

    if (startConst && stepConst && endConst)
    {
        setResult(execAndReplace(e));
    }
    else
    {
        setResult(false);
    }
}

void ConstantVisitor::visit(ast::IfExp & e)
{
}

void ConstantVisitor::visit(ast::DollarVar & e)
{
    setResult(false);
}

void ConstantVisitor::visit(ast::ColonVar & e)
{
    setResult(false);
}

void ConstantVisitor::visit(ast::ArrayListVar & e)
{
}

void ConstantVisitor::visit(ast::DoubleExp & e)
{
    setResult(true);
}

void ConstantVisitor::visit(ast::BoolExp & e)
{
    setResult(true);
}

void ConstantVisitor::visit(ast::StringExp & e)
{
    setResult(true);
}

void ConstantVisitor::visit(ast::CommentExp & e)
{
    // ignored
}

void ConstantVisitor::visit(ast::NilExp & e)
{
    // nothing to do
}

void ConstantVisitor::visit(ast::CellCallExp & e)
{
    setResult(false);
}

void ConstantVisitor::visit(ast::AssignExp & e)
{
}

void ConstantVisitor::visit(ast::WhileExp & e)
{
}

void ConstantVisitor::visit(ast::ForExp & e)
{
}

void ConstantVisitor::visit(ast::BreakExp & e)
{
    // nothing to do
}

void ConstantVisitor::visit(ast::ContinueExp & e)
{
    // nothing to do
}

void ConstantVisitor::visit(ast::TryCatchExp & e)
{
}

void ConstantVisitor::visit(ast::SelectExp & e)
{
}

void ConstantVisitor::visit(ast::CaseExp & e)
{
}

void ConstantVisitor::visit(ast::ReturnExp & e)
{
}

void ConstantVisitor::visit(ast::FieldExp & e)
{
}

void ConstantVisitor::visit(ast::MatrixLineExp & e)
{
}

void ConstantVisitor::visit(ast::SeqExp & e)
{
}

void ConstantVisitor::visit(ast::ArrayListExp & e)
{
}

void ConstantVisitor::visit(ast::AssignListExp & e)
{
}

void ConstantVisitor::visit(ast::VarDec & e)
{
}

void ConstantVisitor::visit(ast::FunctionDec & e)
{
}

void ConstantVisitor::visit(ast::OptimizedExp & e)
{
}

void ConstantVisitor::visit(ast::MemfillExp & e)
{
}

void ConstantVisitor::visit(ast::DAXPYExp & e)
{
}

void ConstantVisitor::visit(ast::IntSelectExp & e)
{
}

void ConstantVisitor::visit(ast::StringSelectExp & e)
{
}

std::unordered_set<std::wstring> ConstantVisitor::init()
{
    std::unordered_set<std::wstring> _constants;
    _constants.emplace(L"%pi");
    _constants.emplace(L"%eps");
    _constants.emplace(L"%e");
    _constants.emplace(L"%i");
    _constants.emplace(L"%nan");
    _constants.emplace(L"%inf");
    _constants.emplace(L"%t");
    _constants.emplace(L"%f");
    _constants.emplace(L"%T");
    _constants.emplace(L"%F");
    _constants.emplace(L"SCI");
    _constants.emplace(L"WSCI");
    _constants.emplace(L"SCIHOME");
    _constants.emplace(L"TMPDIR");

    return _constants;
}
}
