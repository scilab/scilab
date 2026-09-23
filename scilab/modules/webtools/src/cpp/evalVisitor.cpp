/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "evalVisitor.hxx"
#include "execvisitor.hxx"
#include "context.hxx"
#include "cell.hxx"
#include "user.hxx"
#include "UTF8.hxx"

namespace ast
{

void EvalVisitor::visit(const SeqExp& e)
{
    if (e.getExps().size() != 1)
    {
        // we dont want to manage multiple exp in one line
        return;
    }

    // get error state
    std::wstring wstError = ConfigVariable::getLastErrorMessage();
    std::wstring wstErrorFunction = ConfigVariable::getLastErrorFunction();
    int iErrorLine = ConfigVariable::getLastErrorLine();
    int iError = ConfigVariable::getLastErrorNumber();
    bool bError = ConfigVariable::isError();

    ConfigVariable::setLastErrorMessage(L"");
    try
    {
        e.getExps()[0]->accept(*this);
    }
    catch(...)
    {
        ConfigVariable::resetWhereError();
        clearResult();
    }

    // store eval error to be able to return error in read only mode
    _strLastError = scilab::UTF8::toUTF8(ConfigVariable::getLastErrorMessage());

    // set back the error state
    ConfigVariable::setLastErrorMessage(wstError);
    ConfigVariable::setLastErrorFunction(wstErrorFunction);
    ConfigVariable::setLastErrorLine(iErrorLine);
    ConfigVariable::setLastErrorNumber(iError);
    bError ? ConfigVariable::setError() : ConfigVariable::resetError();
}

void EvalVisitor::visit(const SimpleVar& e)
{
    symbol::Context* pCtx = symbol::Context::getInstance();
    if (_iScopeLvl < 0)
    {
        // get current scope lvl
        _iScopeLvl = pCtx->getScopeLevel();
    }

    types::InternalType* pIT = pCtx->getAtLevel(e.getSymbol(), _iScopeLvl);
    if (pIT == nullptr)
    {
        // A class name is not a variable. Resolve it here (a plain lookup, no
        // instantiation) so completion and navigation work on `ClassName.`.
        pIT = pCtx->getClassdef(e.getSymbol().getName());
    }

    setResult(pIT);
}

void EvalVisitor::visit(const FieldExp& e)
{
    types::InternalType * pReturn = nullptr;
    e.getHead()->accept(*this);
    types::InternalType* pIT = getResult();
    if (pIT && (pIT->isGenericType() || pIT->isUserType()))
    {
        SimpleVar * psvRightMember = static_cast<SimpleVar *>(const_cast<Exp *>(e.getTail()));
        std::wstring wstField = psvRightMember->getSymbol().getName();
        if (pIT->isGenericType())
        {
            pIT->getAs<types::GenericType>()->extract(wstField, pReturn);
        }
        else
        {
            pIT->getAs<types::UserType>()->extract(wstField, pReturn);
        }
    }

    setResult(pReturn);
}

void EvalVisitor::visit(const CallExp& e)
{
    e.getName().accept(*this);
    types::InternalType* pIT = getResult();

    if (pIT && (pIT->isGenericType() || pIT->isUserType()))
    {
        exps_t args = e.getArgs();
        types::typed_list tlArgs;
        for(auto& a : args)
        {
            a->accept(*this);
            types::InternalType* pITArg = getResult();
            if (pITArg == nullptr)
            {
                pIT->killMe();
                for(auto& arg : tlArgs)
                {
                    arg->killMe();
                }

                return;
            }

            tlArgs.push_back(pITArg);
        }

        ExecVisitor::visit(e);
        return;
    }

    setResult(nullptr);
}

void EvalVisitor::visit(const CellCallExp& e)
{
    e.getName().accept(*this);
    types::InternalType* pIT = getResult();
    types::List* pExtracted = nullptr;

    if (pIT && pIT->isCell())
    {
        exps_t args = e.getArgs();
        types::typed_list tlArgs;
        for(auto& a : args)
        {
            a->accept(*this);
            types::InternalType* pITArg = getResult();
            if (pITArg == nullptr)
            {
                pIT->killMe();
                for(auto& arg : tlArgs)
                {
                    arg->killMe();
                }

                return;
            }

            tlArgs.push_back(pITArg);
        }

        pExtracted = pIT->getAs<types::Cell>()->extractCell(&tlArgs);
    }

    setResult(pExtracted);
}

void EvalVisitor::visit(const NotExp& e)
{
    e.getExp().accept(*this);
    types::InternalType* pIT = getResult();
    types::InternalType* pReturn = nullptr;
    if (pIT)
    {
        pIT->neg(pReturn);
        if (pIT != pReturn)
        {
            clearResult();
        }
    }

    setResult(pReturn);
}

void EvalVisitor::visit(const ListExp& e)
{
    types::InternalType* pStart = nullptr;
    e.getStart().accept(*this);
    pStart = getResult();

    types::InternalType* pStep = nullptr;
    e.getStep().accept(*this);
    pStep = getResult();

    types::InternalType* pEnd = nullptr;
    e.getEnd().accept(*this);
    pEnd = getResult();

    if (pStart == nullptr || pStep == nullptr || pEnd == nullptr)
    {
        clearResult();
        return;
    }

    setResult(new types::ImplicitList(pStart, pStep, pEnd));
}

}
