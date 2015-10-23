/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

//for Visual Leak Detector in debug compilation mode
//#define DEBUG_VLD
#if defined(DEBUG_VLD) && defined(_DEBUG)
#include <vld.h>
#endif

#include <string>

#include "execvisitor.hxx"
#include "stepvisitor.hxx"
#include "timedvisitor.hxx"
#include "shortcutvisitor.hxx"
#include "printvisitor.hxx"
//#include "AnalysisVisitor.hxx"
#include "debuggervisitor.hxx"
#include "debugmanager.hxx"

#include "visitor_common.hxx"

#include "context.hxx"
#include "generic_operations.hxx"
#include "types_or.hxx"
#include "types_and.hxx"
#include "localization.hxx"

#include "macrofile.hxx"
#include "macro.hxx"
#include "cell.hxx"
#include "filemanager_interface.h"

#include "runner.hxx"
#include "threadmanagement.hxx"

#include "Jitter.hxx"

extern "C"
{
#include "sciprint.h"
#include "os_string.h"
#include "elem_common.h"
#include "storeCommand.h"
}

namespace ast
{
template <class T>
void RunVisitorT<T>::visitprivate(const CellExp &e)
{
    exps_t::const_iterator row;
    exps_t::const_iterator col;
    int iColMax = 0;

    exps_t lines = e.getLines();
    //check dimmension
    for (row = lines.begin() ; row != lines.end() ; ++row )
    {
        exps_t cols = (*row)->getAs<MatrixLineExp>()->getColumns();
        if (iColMax == 0)
        {
            iColMax = static_cast<int>(cols.size());
        }

        if (iColMax != static_cast<int>(cols.size()))
        {
            std::wostringstream os;
            os << _W("inconsistent row/column dimensions\n");
            //os << ((Location)(*row)->getLocation()).getLocationString() << std::endl;
            throw InternalError(os.str(), 999, (*row)->getLocation());
        }
    }

    //alloc result cell
    types::Cell *pC = new types::Cell(static_cast<int>(lines.size()), iColMax);

    int i = 0;
    int j = 0;

    //insert items in cell
    for (i = 0, row = lines.begin() ; row != lines.end() ; ++row, ++i)
    {
        exps_t cols = (*row)->getAs<MatrixLineExp>()->getColumns();
        for (j = 0, col = cols.begin() ; col != cols.end() ; ++col, ++j)
        {
            (*col)->accept(*this);
            types::InternalType *pIT = getResult();
            if (pIT->isImplicitList())
            {
                types::InternalType * _pIT = pIT->getAs<types::ImplicitList>()->extractFullMatrix();
                pC->set(i, j, _pIT);
                _pIT->killMe();
            }
            else
            {
                pC->set(i, j, pIT);
            }
            clearResult();
        }
    }

    //return new cell
    setResult(pC);
}

template <class T>
void RunVisitorT<T>::visitprivate(const FieldExp &e)
{
    /*
      a.b
    */

    if (!e.getTail()->isSimpleVar())
    {
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("/!\\ Unmanaged FieldExp.\n").c_str());
        throw InternalError(szError, 999, e.getLocation());
    }

    try
    {
        e.getHead()->accept(*this);
    }
    catch (const InternalError& error)
    {
        throw error;
    }

    if (getResult() == NULL)
    {
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("Attempt to reference field of non-structure array.\n").c_str());
        throw InternalError(szError, 999, e.getLocation());
    }

    // TODO: handle case where getSize() > 1
    // l=list(struct("toto","coucou"),struct("toto","hello"),1,2);[a,b]=l(1:2).toto
    //
    if (getResultSize() > 1)
    {
        clearResult();
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("Not yet implemented in Scilab.\n").c_str());
        throw InternalError(szError, 999, e.getLocation());
    }

    SimpleVar * psvRightMember = static_cast<SimpleVar *>(const_cast<Exp *>(e.getTail()));
    std::wstring wstField = psvRightMember->getSymbol().getName();
    types::InternalType * pValue = getResult();
    types::InternalType * pReturn = NULL;
    bool ok = false;

    try
    {
        if (pValue->isGenericType() || pValue->isUserType())
        {
            ok = pValue->getAs<types::GenericType>()->extract(wstField, pReturn);
        }
    }
    catch (std::wstring & err)
    {
        pValue->killMe();
        throw InternalError(err.c_str(), 999, e.getTail()->getLocation());
    }

    if (ok)
    {
        if (pReturn == NULL)
        {
            std::wostringstream os;
            os << _W("Invalid index.\n");
            throw InternalError(os.str(), 999, e.getLocation());
        }

        setResult(pReturn);
        if (pValue->isDeletable())
        {
            if (pValue->isContainer())
            {
                // prevent delete of pReturn in case where
                // extract not return a clone
                pReturn->IncreaseRef();
                pValue->killMe();
                pReturn->DecreaseRef();
            }
            else
            {
                pValue->killMe();
            }
        }
    }
    else if (pValue->isFieldExtractionOverloadable())
    {
        types::typed_list in;
        types::typed_list out;

        types::String* pS = new types::String(wstField.c_str());

        //TODO: in the case where overload is a macro there is no need to incref in
        // because args will be put in context, removed and killed if required.
        // But if the overload is a function... it is another story...

        pS->IncreaseRef();
        pValue->IncreaseRef();

        in.push_back(pS);
        in.push_back(pValue);
        types::Callable::ReturnValue Ret = types::Callable::Error;
        std::wstring stType = pValue->getShortTypeStr();

        try
        {
            Ret = Overload::call(L"%" + stType + L"_e", in, 1, out, this);
        }
        catch (const InternalError& ie)
        {
            try
            {
                //to compatibility with scilab 5 code.
                //tlist/mlist name are truncated to 8 first character
                if (stType.size() > 8)
                {
                    Ret = Overload::call(L"%" + stType.substr(0, 8) + L"_e", in, 1, out, this);
                }
                else
                {
                    throw ie;
                }
            }
            catch (const InternalError& ie)
            {
                // TList or Mlist
                if (pValue->isList())
                {
                    Ret = Overload::call(L"%l_e", in, 1, out, this);
                }
                else
                {
                    throw ie;
                }
            }
        }

        if (Ret != types::Callable::OK)
        {
            cleanInOut(in, out);
            setResult(NULL);
            throw InternalError(ConfigVariable::getLastErrorMessage(), ConfigVariable::getLastErrorNumber(), e.getLocation());
        }

        setResult(out);
        cleanIn(in, out);
    }
    else
    {
        pValue->killMe();
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("Attempt to reference field of non-structure array.\n").c_str());
        throw InternalError(szError, 999, e.getLocation());
    }
}

template <class T>
void RunVisitorT<T>::visitprivate(const IfExp  &e)
{
    //Create local exec visitor
    ShortCutVisitor SCTest;
    bool bTestStatus = false;

    //condition
    e.getTest().accept(SCTest);
    e.getTest().accept(*this);

    bTestStatus = getResult()->isTrue();
    clearResult();
    if (bTestStatus == true)
    {
        e.getThen().accept(*this);
    }
    else if (e.hasElse())
    {
        e.getElse().accept(*this);
    }

    if (e.isBreakable()
            && ( (&e.getElse())->isBreak()
                 || (&e.getThen())->isBreak() ))
    {
        const_cast<IfExp*>(&e)->setBreak();
        const_cast<Exp*>(&e.getElse())->resetBreak();
        const_cast<Exp*>(&e.getThen())->resetBreak();
    }

    if (e.isContinuable()
            && ( (&e.getElse())->isContinue()
                 || (&e.getThen())->isContinue() ))
    {
        const_cast<IfExp*>(&e)->setContinue();
        const_cast<Exp*>(&e.getElse())->resetContinue();
        const_cast<Exp*>(&e.getThen())->resetContinue();
    }

    if (e.isReturnable()
            && ( (&e.getElse())->isReturn()
                 || (&e.getThen())->isReturn() ))
    {
        const_cast<IfExp*>(&e)->setReturn();
        const_cast<Exp*>(&e.getElse())->resetReturn();
        const_cast<Exp*>(&e.getThen())->resetReturn();
    }
}

template <class T>
void RunVisitorT<T>::visitprivate(const WhileExp  &e)
{

    //Create local exec visitor
    ShortCutVisitor SCTest;

    //manage & and | like && and ||
    e.getTest().accept(SCTest);
    //condition
    e.getTest().accept(*this);

    types::InternalType* pIT = getResult();

    while (pIT->isTrue())
    {
        pIT->killMe();
        e.getBody().accept(*this);

        //clear old result value before evaluate new one
        if (getResult() != NULL)
        {
            getResult()->killMe();
        }

        if (e.getBody().isBreak())
        {
            const_cast<Exp*>(&(e.getBody()))->resetBreak();
            break;
        }

        if (e.getBody().isReturn())
        {
            const_cast<WhileExp*>(&e)->setReturn();
            const_cast<Exp*>(&(e.getBody()))->resetReturn();
            break;
        }

        if (e.getBody().isContinue())
        {
            const_cast<Exp*>(&(e.getBody()))->resetContinue();
        }

        e.getTest().accept(*this);
        pIT = getResult();
    }

    //pIT->killMe();
    //clear result of condition or result of body
    clearResult();
}

template <class T>
void RunVisitorT<T>::visitprivate(const ForExp  &e)
{
    symbol::Context* ctx = symbol::Context::getInstance();
    //vardec visit increase its result reference
    e.getVardec().accept(*this);
    types::InternalType* pIT = getResult();

    if (pIT->isImplicitList())
    {
        //get IL
        types::ImplicitList* pVar = pIT->getAs<types::ImplicitList>();
        //get IL initial Type
        types::InternalType * pIL = pVar->getInitalType();
        //std::cout << "for IL: " << pIL << std::endl;
        //std::cout << "  for IV: " << pIT << std::endl;
        //get index stack
        symbol::Variable* var = e.getVardec().getAs<VarDec>()->getStack();

        if (ctx->isprotected(var))
        {
            std::wostringstream os;
            os << _W("Redefining permanent variable.\n");
            throw ast::InternalError(os.str(), 999, e.getVardec().getLocation());
        }

        ctx->put(var, pIL);
        //use ref count to lock var against clear and detect any changes
        pIL->IncreaseRef();

        int size = static_cast<int>(pVar->getSize());
        for (int i = 0; i < size; ++i)
        {
            //check if loop index has changed, deleted, copy ...
            if (pIL->getRef() != 2)
            {
                switch (pIL->getRef())
                {
                    case 1:
                        //someone clear me
                        ctx->put(var, pIL);
                        break;
                    default:
                        //someone assign me to another var
                        //a = i;
                        //unlock me
                        pIL->DecreaseRef();

                        //no need to destroy, it already assign to another var
                        //pIL->killMe();

                        //create a new me
                        pIL = pVar->getInitalType();
                        //lock loop index
                        pIL->IncreaseRef();
                        //update me ( must decrease ref of a )
                        if (ctx->isprotected(var))
                        {
                            std::wostringstream os;
                            os << _W("Redefining permanent variable.\n");
                            throw ast::InternalError(os.str(), 999, e.getVardec().getLocation());
                        }

                        ctx->put(var, pIL);
                        break;
                }
            }

            pVar->extractValue(i, pIL);

            try
            {
                e.getBody().accept(*this);
            }
            catch (const InternalError& ie)
            {
                //unlock loop index and implicit list
                pIL->DecreaseRef();
                pIL->killMe();
                pIT->DecreaseRef();
                pIT->killMe();

                setResult(NULL);
                throw ie;
            }

            if (e.getBody().isBreak())
            {
                const_cast<Exp&>(e.getBody()).resetBreak();
                break;
            }

            if (e.getBody().isContinue())
            {
                const_cast<Exp&>(e.getBody()).resetContinue();
                continue;
            }

            if (e.getBody().isReturn())
            {
                const_cast<ForExp&>(e).setReturn();
                const_cast<Exp&>(e.getBody()).resetReturn();
                break;
            }
        }

        //unlock loop index
        pIL->DecreaseRef();
        pIL->killMe();
    }
    else if (pIT->isList())
    {
        types::List* pL = pIT->getAs<types::List>();
        const int size = pL->getSize();
        symbol::Variable* var = e.getVardec().getAs<VarDec>()->getStack();
        for (int i = 0; i < size; ++i)
        {
            types::InternalType* pNew = pL->get(i);

            if (ctx->isprotected(var))
            {
                std::wostringstream os;
                os << _W("Redefining permanent variable.\n");
                throw ast::InternalError(os.str(), 999, e.getVardec().getLocation());
            }
            ctx->put(var, pNew);

            try
            {
                e.getBody().accept(*this);
            }
            catch (const InternalError& ie)
            {
                //implicit list
                pIT->DecreaseRef();
                pIT->killMe();
                setResult(NULL);
                throw ie;
            }

            if (e.getBody().isBreak())
            {
                const_cast<Exp*>(&(e.getBody()))->resetBreak();
                break;
            }

            if (e.getBody().isContinue())
            {
                const_cast<Exp*>(&(e.getBody()))->resetContinue();
                continue;
            }

            if (e.getBody().isReturn())
            {
                const_cast<ForExp*>(&e)->setReturn();
                const_cast<Exp&>(e.getBody()).resetReturn();
                break;
            }
        }
    }
    else if (pIT->isGenericType())
    {
        //Matrix i = [1,3,2,6] or other type
        types::GenericType* pVar = pIT->getAs<types::GenericType>();
        if (pVar->getDims() > 2)
        {
            pIT->DecreaseRef();
            pIT->killMe();
            throw InternalError(_W("for expression can only manage 1 or 2 dimensions variables\n"), 999, e.getVardec().getLocation());
        }

        symbol::Variable* var = e.getVardec().getAs<VarDec>()->getStack();
        for (int i = 0; i < pVar->getCols(); i++)
        {
            types::GenericType* pNew = pVar->getColumnValues(i);
            if (pNew == NULL)
            {
                pIT->DecreaseRef();
                pIT->killMe();
                throw InternalError(_W("for expression : Wrong type for loop iterator.\n"), 999, e.getVardec().getLocation());
            }

            if (ctx->isprotected(var))
            {
                std::wostringstream os;
                os << _W("Redefining permanent variable.\n");
                throw InternalError(os.str(), 999, e.getVardec().getLocation());
            }
            ctx->put(var, pNew);

            try
            {
                e.getBody().accept(*this);
            }
            catch (const InternalError& ie)
            {
                //implicit list
                pIT->DecreaseRef();
                pIT->killMe();
                setResult(NULL);
                throw ie;
            }

            if (e.getBody().isBreak())
            {
                const_cast<Exp*>(&(e.getBody()))->resetBreak();
                break;
            }

            if (e.getBody().isContinue())
            {
                const_cast<Exp*>(&(e.getBody()))->resetContinue();
                continue;
            }

            if (e.getBody().isReturn())
            {
                const_cast<ForExp*>(&e)->setReturn();
                const_cast<Exp&>(e.getBody()).resetReturn();
                break;
            }
        }
    }
    else
    {
        pIT->DecreaseRef();
        pIT->killMe();
        throw InternalError(_W("for expression : Wrong type for loop iterator.\n"), 999, e.getVardec().getLocation());
    }

    pIT->DecreaseRef();
    pIT->killMe();

    setResult(NULL);
}

template <class T>
void RunVisitorT<T>::visitprivate(const ReturnExp &e)
{
    if (e.isGlobal())
    {
        if (ConfigVariable::getPauseLevel() != 0 && symbol::Context::getInstance()->getScopeLevel() == ConfigVariable::getActivePauseLevel())
        {
            if (ConfigVariable::getEnableDebug() == true)
            {
                sciprint(_("%s: function is disabled in debug mode.\n"), "resume");
                return;
            }

            //return or resume
            ConfigVariable::DecreasePauseLevel();
            return;
        }
        else
        {
            const_cast<ReturnExp*>(&e)->setReturn();
        }
    }
    else
    {
        //return(x)

        //in case of CallExp, we can return only one values
        int iSaveExpectedSize = getExpectedSize();
        setExpectedSize(1);
        e.getExp().accept(*this);
        setExpectedSize(iSaveExpectedSize);
        const_cast<ReturnExp*>(&e)->setReturn();
    }
}

template <class T>
void RunVisitorT<T>::visitprivate(const IntSelectExp &e)
{
    bool found = false;
    //e.getSelect()->accept(*this);
    //InternalType* pIT = getResult();
    //setResult(nullptr);
    //if (pIT && pIT->isDouble())
    //{
    //    Double * pDbl = static_cast<Double *>(pIT);
    //    if (!pDbl->isComplex() && pDbl->getSize() == 1)
    //    {
    //        int64_t val;
    //        if (analysis::tools::asInteger<int64_t>(pDbl->get(0), val))
    //        {
    //            Exp * exp = e.getExp(val);
    //            found = true;
    //            if (exp)
    //            {
    //                Exp * body = exp->isCaseExp() ? exp->getAs<CaseExp>()->getBody() : exp;
    //                if (e.isBreakable())
    //                {
    //                    const_cast<IntSelectExp*>(&e)->resetBreak();
    //                    body->setBreakable();
    //                }

    //                if (e.isContinuable())
    //                {
    //                    const_cast<IntSelectExp*>(&e)->resetContinue();
    //                    body->setContinuable();
    //                }

    //                if (e.isReturnable())
    //                {
    //                    const_cast<IntSelectExp*>(&e)->resetReturn();
    //                    body->setReturnable();
    //                }

    //                try
    //                {
    //                    //the good one
    //                    body->accept(*this);
    //                }
    //                catch (const InternalError& ie)
    //                {
    //                    pIT->killMe();
    //                    throw ie;
    //                }

    //                if (e.isBreakable() && body->isBreak())
    //                {
    //                    const_cast<IntSelectExp*>(&e)->setBreak();
    //                    body->resetBreak();
    //                }

    //                if (e.isContinuable() && body->isContinue())
    //                {
    //                    const_cast<IntSelectExp*>(&e)->setContinue();
    //                    body->resetContinue();
    //                }

    //                if (e.isReturnable() && body->isReturn())
    //                {
    //                    const_cast<IntSelectExp*>(&e)->setReturn();
    //                    body->resetReturn();
    //                }
    //            }
    //        }
    //    }
    //}

    if (!found)
    {
        e.getOriginal()->accept(*this);
    }
}

template <class T>
void RunVisitorT<T>::visitprivate(const StringSelectExp &e)
{
    e.getSelect()->accept(*this);
    types::InternalType* pIT = getResult();
    setResult(nullptr);
    bool found = false;
    if (pIT && pIT->isString())
    {
        types::String * pStr = static_cast<types::String *>(pIT);
        if (pStr->getSize() == 1)
        {
            if (wchar_t * s = pStr->get(0))
            {
                const std::wstring ws(s);
                Exp * exp = e.getExp(ws);
                found = true;
                if (exp)
                {
                    Exp * body = exp->isCaseExp() ? exp->getAs<CaseExp>()->getBody() : exp;
                    if (e.isBreakable())
                    {
                        const_cast<StringSelectExp*>(&e)->resetBreak();
                        body->setBreakable();
                    }

                    if (e.isContinuable())
                    {
                        const_cast<StringSelectExp*>(&e)->resetContinue();
                        body->setContinuable();
                    }

                    if (e.isReturnable())
                    {
                        const_cast<StringSelectExp*>(&e)->resetReturn();
                        body->setReturnable();
                    }

                    try
                    {
                        //the good one
                        body->accept(*this);
                    }
                    catch (const InternalError& ie)
                    {
                        pIT->killMe();
                        throw ie;
                    }

                    if (e.isBreakable() && body->isBreak())
                    {
                        const_cast<StringSelectExp*>(&e)->setBreak();
                        body->resetBreak();
                    }

                    if (e.isContinuable() && body->isContinue())
                    {
                        const_cast<StringSelectExp*>(&e)->setContinue();
                        body->resetContinue();
                    }

                    if (e.isReturnable() && body->isReturn())
                    {
                        const_cast<StringSelectExp*>(&e)->setReturn();
                        body->resetReturn();
                    }
                }
            }
        }
    }

    if (!found)
    {
        e.getOriginal()->accept(*this);
    }
}

template <class T>
void RunVisitorT<T>::visitprivate(const SelectExp &e)
{
    // FIXME : exec select ... case ... else ... end
    e.getSelect()->accept(*this);
    bool bCase = false;

    types::InternalType* pIT = getResult();
    setResult(NULL);
    if (pIT)
    {
        // protect pIT to avoid double free when
        // the variable in select is override in the case
        pIT->IncreaseRef();

        //find good case
        exps_t cases = e.getCases();
        for (auto exp : cases)
        {
            CaseExp* pCase = exp->getAs<CaseExp>();
            pCase->getTest()->accept(*this);
            types::InternalType *pITCase = getResult();
            setResult(NULL);
            if (pITCase)
            {
                if (pITCase->isContainer()) //WARNING ONLY FOR CELL
                {
                    //check each item
                }
                else if (*pITCase == *pIT)
                {
                    try
                    {
                        //the good one
                        pCase->getBody()->accept(*this);
                    }
                    catch (const InternalError& ie)
                    {
                        pIT->DecreaseRef();
                        pIT->killMe();
                        throw ie;
                    }

                    if (e.isBreakable() && pCase->getBody()->isBreak())
                    {
                        const_cast<SelectExp*>(&e)->setBreak();
                        pCase->getBody()->resetBreak();
                    }

                    if (e.isContinuable() && pCase->getBody()->isContinue())
                    {
                        const_cast<SelectExp*>(&e)->setContinue();
                        pCase->getBody()->resetContinue();
                    }

                    if (e.isReturnable() && pCase->getBody()->isReturn())
                    {
                        const_cast<SelectExp*>(&e)->setReturn();
                        pCase->getBody()->resetReturn();
                    }

                    pITCase->killMe();
                    bCase = true;
                    break;
                }

                pITCase->killMe();
            }
        }
    }

    if (bCase == false && e.getDefaultCase() != NULL)
    {
        try
        {
            //default case
            e.getDefaultCase()->accept(*this);
        }
        catch (const InternalError& ie)
        {
            if (pIT)
            {
                pIT->DecreaseRef();
                pIT->killMe();
            }
            throw ie;
        }

        if (e.isBreakable() && e.getDefaultCase()->isBreak())
        {
            const_cast<SelectExp*>(&e)->setBreak();
            e.getDefaultCase()->resetBreak();
        }

        if (e.isContinuable() && e.getDefaultCase()->isContinue())
        {
            const_cast<SelectExp*>(&e)->setContinue();
            e.getDefaultCase()->resetContinue();
        }

        if (e.isReturnable() && e.getDefaultCase()->isReturn())
        {
            const_cast<SelectExp*>(&e)->setReturn();
            e.getDefaultCase()->resetReturn();
        }
    }

    clearResult();

    if (pIT)
    {
        pIT->DecreaseRef();
        pIT->killMe();
    }
}

template <class T>
void RunVisitorT<T>::visitprivate(const SeqExp  &e)
{
    for (auto exp : e.getExps())
    {
        if (exp->isCommentExp())
        {
            continue;
        }

        if (ConfigVariable::isExecutionBreak())
        {
            ConfigVariable::resetExecutionBreak();
            StorePrioritaryCommand("pause");
        }

        // interrupt me to execute a prioritary command
        while (StaticRunner_isInterruptibleCommand() == 1 && StaticRunner_isRunnerAvailable() == 1)
        {
            StaticRunner_launch();
            StaticRunner_setInterruptibleCommand(1);
        }

        try
        {
            //reset default values
            setResult(NULL);
            int iExpectedSize = getExpectedSize();
            setExpectedSize(-1);
            exp->accept(*this);
            setExpectedSize(iExpectedSize);
            types::InternalType * pIT = getResult();

            // In case of exec file, set the file name in the Macro to store where it is defined.
            int iFileID = ConfigVariable::getExecutedFileID();
            if (iFileID && exp->isFunctionDec())
            {
                types::InternalType* pITMacro = symbol::Context::getInstance()->get(exp->getAs<FunctionDec>()->getSymbol());
                if (pITMacro)
                {
                    types::Macro* pMacro = pITMacro->getAs<types::Macro>();
                    const wchar_t* filename = getfile_filename(iFileID);
                    // scilab.quit is not open with mopen
                    // in this case filename is NULL because FileManager have not been filled.
                    if (filename)
                    {
                        pMacro->setFileName(filename);
                    }
                }
            }

            if (pIT != NULL)
            {
                bool bImplicitCall = false;
                if (pIT->isCallable()) //to manage call without ()
                {
                    types::Callable *pCall = pIT->getAs<types::Callable>();
                    types::typed_list out;
                    types::typed_list in;
                    types::optional_list opt;

                    try
                    {
                        //in this case of calling, we can return only one values
                        int iSaveExpectedSize = getExpectedSize();
                        setExpectedSize(1);

                        pCall->invoke(in, opt, getExpectedSize(), out, e);
                        setExpectedSize(iSaveExpectedSize);

                        if (out.size() == 0)
                        {
                            setResult(NULL);
                        }
                        else
                        {
                            setResult(out[0]);
                        }

                        bImplicitCall = true;
                    }
                    catch (const InternalError& ie)
                    {
                        if (ConfigVariable::getLastErrorFunction() == L"")
                        {
                            ConfigVariable::setLastErrorFunction(pCall->getName());
                            ConfigVariable::setLastErrorLine(e.getLocation().first_line);
                        }

                        throw ie;
                    }
                }
                else if (pIT->isImplicitList())
                {
                    //expand implicit when possible
                    types::ImplicitList* pIL = pIT->getAs<types::ImplicitList>();
                    if (pIL->isComputable())
                    {
                        types::InternalType* p = pIL->extractFullMatrix();
                        if (p)
                        {
                            setResult(p);
                            pIL->killMe();
                        }
                    }
                }

                //don't output Simplevar and empty result
                if (getResult() != NULL && (!exp->isSimpleVar() || bImplicitCall))
                {
                    //symbol::Context::getInstance()->put(symbol::Symbol(L"ans"), *execMe.getResult());
                    types::InternalType* pITAns = getResult();
                    symbol::Context::getInstance()->put(m_pAns, pITAns);
                    if (exp->isVerbose() && ConfigVariable::isPromptShow())
                    {
                        //TODO manage multiple returns
                        scilabWriteW(L" ans  =\n\n");
                        std::wostringstream ostrName;
                        ostrName << L"ans";
                        VariableToString(pITAns, ostrName.str().c_str());
                    }
                }

                pIT->killMe();
            }

            if ((&e)->isBreakable() && exp->isBreak())
            {
                const_cast<SeqExp *>(&e)->setBreak();
                exp->resetBreak();
                break;
            }

            if ((&e)->isContinuable() && exp->isContinue())
            {
                const_cast<SeqExp *>(&e)->setContinue();
                exp->resetContinue();
                break;
            }

            if ((&e)->isReturnable() && exp->isReturn())
            {
                const_cast<SeqExp *>(&e)->setReturn();
                exp->resetReturn();
                break;
            }
        }
        catch (const InternalError& ie)
        {
            ConfigVariable::fillWhereError(ie.GetErrorLocation().first_line);
            throw ie;
        }

        // If something other than NULL is given to setResult, then that would imply
        // to make a cleanup in visit(ForExp) for example (e.getBody().accept(*this);)
        setResult(NULL);
    }
}

template <class T>
void RunVisitorT<T>::visitprivate(const NotExp &e)
{
    /*
      @ or ~ !
    */
    e.getExp().accept(*this);

    types::InternalType * pValue = getResult();
    types::InternalType * pReturn = NULL;
    if (pValue->neg(pReturn))
    {
        if (pValue != pReturn)
        {
            pValue->killMe();
        }

        setResult(pReturn);
    }
    else
    {
        // neg returned false so the negation is not possible so we call the overload (%foo_5)
        types::typed_list in;
        types::typed_list out;

        pValue->IncreaseRef();
        in.push_back(pValue);

        types::Callable::ReturnValue Ret = Overload::call(L"%" + pValue->getShortTypeStr() + L"_5", in, 1, out, this);

        if (Ret != types::Callable::OK)
        {
            cleanInOut(in, out);
            throw InternalError(ConfigVariable::getLastErrorMessage(), ConfigVariable::getLastErrorNumber(), e.getLocation());
        }

        setResult(out);
        cleanIn(in, out);
    }
}

template <class T>
void RunVisitorT<T>::visitprivate(const TransposeExp &e)
{
    e.getExp().accept(*this);

    if (getResultSize() != 1)
    {
        clearResult();
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("%ls: Can not transpose multiple elements.\n").c_str(), L"Transpose");
        throw InternalError(szError, 999, e.getLocation());
    }

    types::InternalType * pValue = getResult();
    types::InternalType * pReturn = NULL;
    const bool bConjug = e.getConjugate() == TransposeExp::_Conjugate_;

    if ((bConjug && pValue->adjoint(pReturn)) || (!bConjug && pValue->transpose(pReturn)))
    {
        if (pValue != pReturn)
        {
            pValue->killMe();
        }

        setResult(pReturn);

        return;
    }
    else
    {
        // transpose returned false so the negation is not possible so we call the overload (%foo_t or %foo_0)
        types::typed_list in;
        types::typed_list out;

        pValue->IncreaseRef();
        in.push_back(pValue);

        types::Callable::ReturnValue Ret;
        if (bConjug)
        {
            Ret = Overload::call(L"%" + getResult()->getShortTypeStr() + L"_t", in, 1, out, this);
        }
        else
        {
            Ret = Overload::call(L"%" + getResult()->getShortTypeStr() + L"_0", in, 1, out, this);
        }

        if (Ret != types::Callable::OK)
        {
            cleanInOut(in, out);
            throw InternalError(ConfigVariable::getLastErrorMessage(), ConfigVariable::getLastErrorNumber(), e.getLocation());
        }

        setResult(out);
        cleanIn(in, out);
    }
}

template <class T>
void RunVisitorT<T>::visitprivate(const FunctionDec & e)
{
    symbol::Context* ctx = symbol::Context::getInstance();
    /*
      function foo
      endfunction
    */

    // funcprot(0) : do nothing
    // funcprot(1) && warning(on) : warning
    //get input parameters list
    std::list<symbol::Variable*> *pVarList = new std::list<symbol::Variable*>();
    const exps_t & vars = e.getArgs().getVars();
    for (const auto var : vars)
    {
        pVarList->push_back(var->getAs<SimpleVar>()->getStack());
    }

    //get output parameters list
    std::list<symbol::Variable*> *pRetList = new std::list<symbol::Variable*>();
    const exps_t & rets = e.getReturns().getVars();
    for (const auto ret : rets)
    {
        pRetList->push_back(ret->getAs<SimpleVar>()->getStack());
    }

    types::Macro *pMacro = new types::Macro(e.getSymbol().getName(), *pVarList, *pRetList,
                                            const_cast<SeqExp&>(static_cast<const SeqExp&>(e.getBody())), L"script");
    pMacro->setLines(e.getLocation().first_line, e.getLocation().last_line);

    bool bEquals = false;
    int iFuncProt = ConfigVariable::getFuncprot();
    if (iFuncProt != 0)
    {
        types::InternalType* pITFunc = ctx->get(((FunctionDec&)e).getStack());
        if (pITFunc && pITFunc->isCallable())
        {
            if (pITFunc->isMacroFile())
            {
                types::MacroFile* pMF = pITFunc->getAs<types::MacroFile>();
                bEquals = *pMF->getMacro() == *pMacro;
            }
            else if (pITFunc->isMacro())
            {
                types::Macro* pM = pITFunc->getAs<types::Macro>();
                bEquals = *pM == *pMacro;
            }
        }
        else
        {
            bEquals = true; //avoid msg but keep assignation
        }
    }

    if (bEquals == false && iFuncProt == 1 && ConfigVariable::getWarningMode())
    {
        wchar_t pwstFuncName[1024];
        os_swprintf(pwstFuncName, 1024, L"%-24ls", e.getSymbol().getName().c_str());
        char* pstFuncName = wide_string_to_UTF8(pwstFuncName);


        sciprint(_("Warning : redefining function: %s. Use funcprot(0) to avoid this message"), pstFuncName);
        sciprint("\n");
        FREE(pstFuncName);
    }
    else if (bEquals == false && iFuncProt == 2)
    {
        char pstError[1024];
        char* pstFuncName = wide_string_to_UTF8(e.getSymbol().getName().c_str());
        os_sprintf(pstError, _("It is not possible to redefine the %s primitive this way (see clearfun).\n"), pstFuncName);
        wchar_t* pwstError = to_wide_string(pstError);
        std::wstring wstError(pwstError);
        FREE(pstFuncName);
        FREE(pwstError);
        delete pMacro;
        throw InternalError(wstError, 999, e.getLocation());
    }


    if (ctx->isprotected(symbol::Symbol(pMacro->getName())))
    {
        delete pMacro;
        std::wostringstream os;
        os << _W("Redefining permanent variable.\n");
        throw InternalError(os.str(), 999, e.getLocation());
    }

    ctx->addMacro(pMacro);

}

template <class T>
void RunVisitorT<T>::visitprivate(const ListExp &e)
{
    e.getStart().accept(*this);
    types::GenericType* pITStart = static_cast<types::GenericType*>(getResult());
    if ((pITStart->getSize() != 1 || (pITStart->isDouble() && pITStart->getAs<types::Double>()->isComplex())) &&
            pITStart->isList() == false) // list case => call overload
    {
        pITStart->killMe();
        setResult(NULL);
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("%ls: Wrong type for argument %d: Real scalar expected.\n").c_str(), L"':'", 1);
        throw InternalError(szError, 999, e.getLocation());
    }
    types::InternalType * piStart = pITStart;

    e.getStep().accept(*this);
    types::GenericType* pITStep = static_cast<types::GenericType*>(getResult());
    setResult(NULL);
    if ((pITStep->getSize() != 1 || (pITStep->isDouble() && pITStep->getAs<types::Double>()->isComplex())) &&
            pITStep->isList() == false) // list case => call overload
    {
        pITStart->killMe();
        pITStep->killMe();
        setResult(NULL);
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("%ls: Wrong type for argument %d: Real scalar expected.\n").c_str(), L"':'", 2);
        throw InternalError(szError, 999, e.getLocation());
    }
    types::InternalType* piStep = pITStep;

    e.getEnd().accept(*this);
    types::GenericType* pITEnd = static_cast<types::GenericType*>(getResult());
    setResult(NULL);
    if ((pITEnd->getSize() != 1 || (pITEnd->isDouble() && pITEnd->getAs<types::Double>()->isComplex())) &&
            pITEnd->isList() == false) // list case => call overload
    {
        pITStart->killMe();
        pITStep->killMe();
        pITEnd->killMe();
        setResult(NULL);
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("%ls: Wrong type for argument %d: Real scalar expected.\n").c_str(), L"':'", 3);
        throw InternalError(szError, 999, e.getLocation());
    }
    types::InternalType* piEnd = pITEnd;

    ////check if implicitlist is 1:$ to replace by ':'
    //if (piStart->isDouble() && piStep->isDouble() && piEnd->isPoly())
    //{
    //    if (piStart->getAs<Double>()->get()[0] == 1 && piStep->getAs<Double>()->get()[0] == 1)
    //    {
    //        SinglePoly* end = piEnd->getAs<Polynom>()->get()[0];
    //        if (end->getRank() == 1 && end->get()[0] == 0 && end->get()[1] == 1)
    //        {
    //            setResult(new Colon());
    //            return;
    //        }
    //    }
    //}

    //check compatibility
    // double : double : double or poly : poly : poly and mix like double : double : poly
    if ((piStart->isPoly() || piStart->isDouble()) &&
            (piStep->isPoly() || piStep->isDouble()) &&
            (piEnd->isPoly() || piEnd->isDouble()))
    {
        // No need to kill piStart, ... because Implicit list ctor will incref them
        setResult(new types::ImplicitList(piStart, piStep, piEnd));
        return;
    }

    // int : double or int : int
    if (piStart->isInt() &&
            (piStep->isDouble() || piStep->isInt()) &&
            piEnd->isInt())
    {
        // check for same int type int8, int 16 ...
        if (piStart->getType() == piEnd->getType() &&
                (piStart->getType() == piStep->getType() ||
                 piStep->isDouble()))
        {
            // No need to kill piStart, ... because Implicit list ctor will incref them
            setResult(new types::ImplicitList(piStart, piStep, piEnd));
            return;
        }
    }

    // Call Overload
    types::Callable::ReturnValue Ret;
    types::typed_list in;
    types::typed_list out;

    piStart->IncreaseRef();
    in.push_back(piStart);

    try
    {
        if (e.hasExplicitStep())
        {
            // 1:2:4
            //call overload %typeStart_b_typeStep
            piStep->IncreaseRef();
            in.push_back(piStep);
            piEnd->IncreaseRef();
            in.push_back(piEnd);
            Ret = Overload::call(L"%" + piStart->getShortTypeStr() + L"_b_" + piStep->getShortTypeStr(), in, 1, out, true);
        }
        else
        {
            // 1:2
            //call overload %typeStart_b_typeEnd
            piStep->killMe();
            piEnd->IncreaseRef();
            in.push_back(piEnd);
            Ret = Overload::call(L"%" + piStart->getShortTypeStr() + L"_b_" + piEnd->getShortTypeStr(), in, 1, out, true);
        }
    }
    catch (const InternalError& error)
    {
        setResult(NULL);
        cleanInOut(in, out);
        throw error;
    }

    if (Ret != types::Callable::OK)
    {
        setResult(NULL);
        cleanInOut(in, out);
        throw InternalError(ConfigVariable::getLastErrorMessage(), ConfigVariable::getLastErrorNumber(), e.getLocation());
    }

    setResult(out);
    cleanIn(in, out);
}

template <class T>
void RunVisitorT<T>::visitprivate(const OptimizedExp &e)
{
}

template <class T>
void RunVisitorT<T>::visitprivate(const MemfillExp &e)
{
    e.getOriginal()->accept(*this);
}

template <class T>
void RunVisitorT<T>::visitprivate(const DAXPYExp &e)
{
    types::InternalType* pIT = NULL;
    types::Double* ad = NULL;
    int ar = 0;
    int ac = 0;

    types::Double* xd = NULL;
    int xr = 0;
    int xc = 0;

    types::Double* yd = NULL;
    int yr = 0;
    int yc = 0;

    //check types and dimensions

    //y must be double
    const Exp &ye = e.getY();
    ye.accept(*this);
    pIT = getResult();
    if (pIT->isDouble())
    {
        yd = pIT->getAs<types::Double>();
        if (yd->getDims() == 2 && yd->isComplex() == false)
        {
            yr = yd->getRows();
            yc = yd->getCols();
        }
        else
        {
            yd->killMe();
            e.getOriginal()->accept(*this);
            return;
        }
    }
    else
    {
        pIT->killMe();
        e.getOriginal()->accept(*this);
        return;
    }

    //x
    const Exp &xe = e.getX();
    xe.accept(*this);
    pIT = getResult();

    if (pIT->isDouble())
    {
        xd = pIT->getAs<types::Double>();
        if (xd->isScalar() && xd->isComplex() == false)
        {
            // x become a
            ad = xd;
            ar = 1;
            ac = 1;
        }
        else if (xd->getDims() == 2 && xd->isComplex() == false)
        {
            xr = xd->getRows();
            xc = xd->getCols();
        }
        else
        {
            yd->killMe();
            xd->killMe();
            e.getOriginal()->accept(*this);
            return;
        }
    }
    else
    {
        pIT->killMe();
        yd->killMe();
        e.getOriginal()->accept(*this);
        return;
    }

    const Exp &ae = e.getA();
    ae.accept(*this);
    pIT = getResult();

    if (pIT->isDouble())
    {
        if (ad)
        {
            xd = pIT->getAs<types::Double>();
            //X is scalar it become A
            //now use A as X
            if (xd->getDims() == 2 && xd->isComplex() == false)
            {
                xr = xd->getRows();
                xc = xd->getCols();
            }
            else
            {
                yd->killMe();
                xd->killMe();
                ad->killMe();
                e.getOriginal()->accept(*this);
                return;
            }
        }
        else
        {
            //a is a and it must be scalar
            ad = pIT->getAs<types::Double>();
            if (/*ad->isScalar() && */ad->isComplex() == false)
            {
                ar = ad->getRows(); //1;
                ac = ad->getCols();//1;
            }
            else
            {
                yd->killMe();
                xd->killMe();
                ad->killMe();
                e.getOriginal()->accept(*this);
                return;
            }
        }
    }
    else
    {
        pIT->killMe();
        yd->killMe();
        xd->killMe();
        e.getOriginal()->accept(*this);
        return;
    }

    if (ad && xd && yd)
    {
        if ( ac == 1 &&
                ar == 1 &&
                xr == yr &&
                xc == yc)
        {
            //go !
            int one = 1;
            int size = xc * xr;
            //Double* od = (Double*)yd->clone();
            C2F(daxpy)(&size, ad->get(), xd->get(), &one, yd->get(), &one);
            //setResult(od);
            //yd->killMe();
            xd->killMe();
            ad->killMe();
            return;
        }
        else if (ac == xr && ar == yr && xc == yc)
        {
            char n = 'n';
            double one = 1;
            C2F(dgemm)(&n, &n, &ar, &xc, &ac, &one, ad->get(), &ar, xd->get(), &ac, &one, yd->get(), &ar);
            xd->killMe();
            ad->killMe();
            return;
        }
    }

    if (yd)
    {
        yd->killMe();
    }

    if (xd)
    {
        xd->killMe();
    }

    if (ad)
    {
        ad->killMe();
    }

    e.getOriginal()->accept(*this);
    return;
}

template <class T>
void RunVisitorT<T>::visitprivate(const TryCatchExp  &e)
{
    //save current prompt mode
    int oldVal = ConfigVariable::getSilentError();
    int oldMode = ConfigVariable::getPromptMode();
    //set mode silent for errors
    ConfigVariable::setSilentError(1);

    symbol::Context* pCtx = symbol::Context::getInstance();
    try
    {
        int scope = pCtx->getScopeLevel();
        int level = ConfigVariable::getRecursionLevel();
        try
        {
            e.getTry().accept(*this);
            //restore previous prompt mode
            ConfigVariable::setSilentError(oldVal);
        }
        catch (const RecursionException& /* re */)
        {
            ConfigVariable::setPromptMode(oldMode);

            //close opened scope during try
            while (pCtx->getScopeLevel() > scope)
            {
                pCtx->scope_end();
            }

            //decrease recursion to init value and close where
            while (ConfigVariable::getRecursionLevel() > level)
            {
                ConfigVariable::where_end();
                ConfigVariable::decreaseRecursion();
            }

            //print msg about recursion limit and trigger an error
            wchar_t sz[1024];
            os_swprintf(sz, 1024, _W("Recursion limit reached (%d).\n").data(), ConfigVariable::getRecursionLimit());
            throw ast::InternalError(sz);
        }

    }
    catch (const InternalError& /* ie */)
    {
        //restore previous prompt mode
        ConfigVariable::setSilentError(oldVal);
        //to lock lasterror
        ConfigVariable::setLastErrorCall();
        // reset call stack filled when error occured
        ConfigVariable::resetWhereError();
        e.getCatch().accept(*this);
    }
}


} /* namespace ast */

#include "run_CallExp.hpp"
#include "run_MatrixExp.hpp"
#include "run_OpExp.hpp"
#include "run_AssignExp.hpp"

template EXTERN_AST class ast::RunVisitorT<ast::ExecVisitor>;
template EXTERN_AST class ast::RunVisitorT<ast::StepVisitor>;
template EXTERN_AST class ast::RunVisitorT<ast::TimedVisitor>;
template EXTERN_AST class ast::RunVisitorT<ast::DebuggerVisitor>;
