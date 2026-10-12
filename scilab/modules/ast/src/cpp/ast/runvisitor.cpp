/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

//for Visual Leak Detector in debug compilation mode
//#define DEBUG_VLD
#if defined(DEBUG_VLD) && defined(_DEBUG)
#include <vld.h>
#endif

#include <string>
#include <cwctype>

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
#include "object.hxx"
#include "classdef.hxx"
#include "cell.hxx"
#include "listinsert.hxx"
#include "filemanager_interface.h"

#include "runner.hxx"
#include "threadmanagement.hxx"

#include "coverage_instance.hxx"

extern "C"
{
#include "sciprint.h"
#include "os_string.h"
#include "elem_common.h"
#include "storeCommand.h"
#include "prompt.h"
#include "scilabRead.h"
}

namespace ast
{
template <class T>
void RunVisitorT<T>::visitprivate(const StringExp & e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    if (e.getConstant() == nullptr)
    {
        types::String *psz = new types::String(e.getValue().c_str());
        (const_cast<StringExp *>(&e))->setConstant(psz);
    }
    setResult(e.getConstant());
    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const DoubleExp & e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    if (e.getConstant() == nullptr)
    {
        types::Double *pdbl;
        if (e.isComplex())
        {
            pdbl = new types::Double(0.0, e.getValue());
        }
        else
        {
            pdbl = new types::Double(e.getValue());
        }
        (const_cast<DoubleExp *>(&e))->setConstant(pdbl);
    }
    setResult(e.getConstant());
    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const BoolExp & e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    if (e.getConstant() == nullptr)
    {
        types::Bool *pB = new types::Bool(e.getValue());
        (const_cast<BoolExp *>(&e))->setConstant(pB);
    }
    setResult(e.getConstant());
    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const NilExp & e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    setResult(new types::Void());
    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const SimpleVar & e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    symbol::Context* ctx = symbol::Context::getInstance();
    symbol::Variable* var = ((SimpleVar&)e).getStack();
    types::InternalType *pI = ctx->get(var);
    setResult(pI);
    if (pI != nullptr)
    {
        if (e.isVerbose() && pI->isCallable() == false && ConfigVariable::isPrintOutput())
        {
            std::wstring wstrName = e.getSymbol().getName();
            scilabWriteW(printVarEqualTypeDimsInfo(pI, wstrName).c_str());
            VariableToString(pI, wstrName.c_str());
        }

        //check if var is recalled in current scope like
        //function f()
        //  a; //<=> a=a;
        //  a(2) = 18;
        //endfunction
        if (e.getParent()->isSeqExp())
        {
            if (ctx->getScopeLevel() > 1 && var->empty() == false && var->top()->m_iLevel != ctx->getScopeLevel())
            {
                //put var in current scope
                ctx->put(var, pI);
            }
        }
    }
    else
    {
        char pstError[bsiz];
        wchar_t* pwstError;

        char* strErr = wide_string_to_UTF8(e.getSymbol().getName().c_str());

        os_sprintf(pstError, _("Undefined variable: %s\n"), strErr);
        pwstError = to_wide_string(pstError);
        FREE(strErr);
        std::wstring wstError(pwstError);
        FREE(pwstError);
        CoverageInstance::stopChrono((void*)&e);
        throw InternalError(wstError, 999, e.getLocation());
        //Err, SimpleVar doesn't exist in Scilab scopes.
    }
    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const ColonVar & e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    types::Colon *pC = new types::Colon();
    setResult(pC);
    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const DollarVar & e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    setResult(types::Polynom::Dollar());
    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const BreakExp & e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    const_cast<BreakExp*>(&e)->setBreak();
    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const ContinueExp &e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    const_cast<ContinueExp*>(&e)->setContinue();
    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const ArrayListExp & e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    exps_t::const_iterator it;
    int iNbExpSize = this->getExpectedSize();
    this->setExpectedSize(1);

    types::typed_list lstIT;
    for (it = e.getExps().begin(); it != e.getExps().end(); it++)
    {
        (*it)->accept(*this);
        for (int j = 0; j < getResultSize(); j++)
        {
            lstIT.push_back(getResult(j));
        }
    }

    setResult(lstIT);

    this->setExpectedSize(iNbExpSize);
    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const VarDec & e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    try
    {
        /*getting what to assign*/
        e.getInit().accept(*this);
        if(getResultSize() != 1)
        {
            clearResult();
            setResult(NULL);
            return;
        }

        getResult()->IncreaseRef();
    }
    catch (const InternalError& error)
    {
        CoverageInstance::stopChrono((void*)&e);
        throw error;
    }
    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const CellExp & e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);

    exps_t::const_iterator row;
    exps_t::const_iterator col;
    int iColMax = 0;
    int iLineMax = 0;

    exps_t lines = e.getLines();
    iLineMax = static_cast<int>(lines.size());

    //check dimmension
    for (row = lines.begin(); row != lines.end(); ++row)
    {
        exps_t cols = (*row)->getAs<MatrixLineExp>()->getColumns();
        int iCurrentCols = static_cast<int>(cols.size());
        for (col = cols.begin(); col != cols.end(); ++col)
        {
            // remove comments in the columns count
            if((*col)->isCommentExp())
            {
                iCurrentCols--;
            }
        }

        // only comments in the line,
        // don't count them and go to the next one
        if(iCurrentCols == 0)
        {
            iLineMax--;
            continue;
        }

        // initialise the number of columns
        if (iColMax == 0)
        {
            iColMax = iCurrentCols;
        }

        if (iColMax != iCurrentCols)
        {
            std::wostringstream os;
            os << _W("inconsistent row/column dimensions\n");
            //os << ((Location)(*row)->getLocation()).getLocationString() << std::endl;
            CoverageInstance::stopChrono((void*)&e);
            throw InternalError(os.str(), 999, (*row)->getLocation());
        }
    }

    //alloc result cell
    types::Cell *pC = new types::Cell(iLineMax, iColMax);
    int i = 0;
    int j = 0;
    //insert items in cell
    for (i = 0, row = lines.begin(); row != lines.end(); ++row)
    {
        exps_t cols = (*row)->getAs<MatrixLineExp>()->getColumns();
        for (j = 0, col = cols.begin(); col != cols.end(); ++col)
        {
            try
            {
                (*col)->accept(*this);
            }
            catch (ScilabException &)
            {
                pC->killMe();
                CoverageInstance::stopChrono((void*)&e);
                throw;
            }

            types::InternalType *pIT = getResult();
            if (pIT == NULL)
            {
                continue;
            }

            if (pIT->isImplicitList())
            {
                types::InternalType * _pIT = pIT->getAs<types::ImplicitList>()->extractFullMatrix();
                if(_pIT)
                {
                    pIT = _pIT;
                }
            }

            pC->set(i, j++, pIT);
            clearResult();
        }

        // increment row iterator only
        // when the row is not empty
        if(j)
        {
            i++;
        }
    }

    //return new cell
    setResult(pC);

    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const FieldExp &e)
{
    /*
      a.b
      */

    CoverageInstance::invokeAndStartChrono((void*)&e);

    if (!e.getTail()->isSimpleVar())
    {
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("/!\\ Unmanaged FieldExp.\n").c_str());
        CoverageInstance::stopChrono((void*)&e);
        throw InternalError(szError, 999, e.getLocation());
    }

    try
    {
        e.getHead()->accept(*this);
    }
    catch (const InternalError& error)
    {
        CoverageInstance::stopChrono((void*)&e);
        throw error;
    }

    if (getResult() == NULL)
    {
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("Attempt to reference field of non-structure array.\n").c_str());
        CoverageInstance::stopChrono((void*)&e);
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
        CoverageInstance::stopChrono((void*)&e);
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
        else if (pValue->isClassdef())
        {
            ok = pValue->getAs<types::Classdef>()->extract(wstField, pReturn);
        }
    }
    catch (std::wstring & err)
    {
        clearResult();
        CoverageInstance::stopChrono((void*)&e);
        throw InternalError(err.c_str(), 999, e.getTail()->getLocation());
    }

    if (ok)
    {
        if (pReturn == NULL)
        {
            std::wostringstream os;
            os << _W("Invalid index.\n");
            CoverageInstance::stopChrono((void*)&e);
            throw InternalError(os.str(), 999, e.getLocation());
        }

        setResult(pReturn);
        if (pValue->isDeletable())
        {
            // prevent delete of pReturn in case where
            // extract not return a clone
            pReturn->IncreaseRef();
            pValue->killMe();
            pReturn->DecreaseRef();
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
        std::wstring wstrFuncName = L"%" + stType + L"_e";

        Ret = Overload::call(wstrFuncName.c_str(), in, 1, out, false, false, e.getLocation());
        if(Ret == types::Callable::OK_NoResult && wstrFuncName.length() > 8)
        {
            // overload not defined, try with the short name.
            // to compatibility with scilab 5 code.
            // tlist/mlist name are truncated to 8 first character
            wstrFuncName = L"%" + stType.substr(0, 8) + L"_e";
            Ret = Overload::call(wstrFuncName.c_str(), in, 1, out, false, false, e.getLocation());
        }

        if(pValue->isList() && Ret == types::Callable::OK_NoResult)
        {
            // last try that will throw an error if it not exists
            wstrFuncName = L"%l_e";
            Ret = Overload::call(wstrFuncName, in, 1, out, false, true, e.getLocation());
        }

        if (Ret != types::Callable::OK)
        {
            cleanInOut(in, out);
            setResult(NULL);
            CoverageInstance::stopChrono((void*)&e);
            throw InternalError(ConfigVariable::getLastErrorMessage(), ConfigVariable::getLastErrorNumber(), e.getLocation());
        }

        // An extraction have to return something
        if(out.empty())
        {
            setResult(NULL);
            cleanInOut(in, out);
            CoverageInstance::stopChrono((void*)&e);

            wchar_t wcstrError[512];
            os_swprintf(wcstrError, 512, _W("%ls: Extraction must have at least one output.\n").c_str(), wstrFuncName.c_str());

            throw InternalError(wcstrError, 999, e.getLocation());
        }

        setResult(out);
        cleanIn(in, out);
    }
    else
    {
        clearResult();
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("Attempt to reference field of non-structure array.\n").c_str());
        CoverageInstance::stopChrono((void*)&e);
        throw InternalError(szError, 999, e.getLocation());
    }

    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const ArgumentsExp  &e)
{
    /* FIXME: Implement visitor for ArgumentsExp */
}

template <class T>
void RunVisitorT<T>::visitprivate(const IfExp  &e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);

    //Create local exec visitor
    ShortCutVisitor SCTest;
    bool bTestStatus = false;

    //condition
    try
    {
        e.getTest().accept(SCTest);
        e.getTest().accept(*this);
    }
    catch (ScilabException &)
    {
        CoverageInstance::stopChrono((void*)&e);
        throw;
    }

    bTestStatus = getResult()->isTrue();
    clearResult();
    try
    {
        if (bTestStatus == true)
        {
            e.getThen().accept(*this);
        }
        else if (e.hasElse())
        {
            e.getElse().accept(*this);
        }
    }
    catch (ScilabException &)
    {
        CoverageInstance::stopChrono((void*)&e);
        throw;
    }

    bool elseIsBreak = e.hasElse() && (&e.getElse())->isBreak();
    if (e.isBreakable() && (elseIsBreak || (&e.getThen())->isBreak()))
    {
        const_cast<IfExp*>(&e)->setBreak();
        const_cast<Exp*>(&e.getThen())->resetBreak();
        if (e.hasElse())
        {
            const_cast<Exp*>(&e.getElse())->resetBreak();
        }
    }

    bool elseIsContinue = e.hasElse() && (&e.getElse())->isContinue();
    if (e.isContinuable() && (elseIsContinue || (&e.getThen())->isContinue()))
    {
        const_cast<IfExp*>(&e)->setContinue();
        const_cast<Exp*>(&e.getThen())->resetContinue();
        if (e.hasElse())
        {
            const_cast<Exp*>(&e.getElse())->resetContinue();
        }
    }

    bool elseIsReturn = e.hasElse() && (&e.getElse())->isReturn();
    if (e.isReturnable() && (elseIsReturn || (&e.getThen())->isReturn()))
    {
        const_cast<IfExp*>(&e)->setReturn();
        const_cast<Exp*>(&e.getThen())->resetReturn();
        if (e.hasElse())
        {
            const_cast<Exp*>(&e.getElse())->resetReturn();
        }
    }

    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const WhileExp  &e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);

    //Create local exec visitor
    ShortCutVisitor SCTest;

    try
    {
        //manage & and | like && and ||
        e.getTest().accept(SCTest);
        //condition
        e.getTest().accept(*this);
    }
    catch (ScilabException &)
    {
        CoverageInstance::stopChrono((void*)&e);
        throw;
    }

    types::InternalType* pIT = getResult();

    while (pIT->isTrue())
    {
        pIT->killMe();
        setResult(NULL);

        try
        {
            e.getBody().accept(*this);
        }
        catch (ScilabException &)
        {
            CoverageInstance::stopChrono((void*)&e);
            throw;
        }

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

        try
        {
            e.getTest().accept(*this);
        }
        catch (ScilabException &)
        {
            CoverageInstance::stopChrono((void*)&e);
            throw;
        }
        pIT = getResult();
    }

    //pIT->killMe();
    //clear result of condition or result of body
    clearResult();
    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const ForExp  &e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    symbol::Context* ctx = symbol::Context::getInstance();
    //vardec visit increase its result reference
    try
    {
        e.getVardec().accept(*this);
    }
    catch (ScilabException &)
    {
        CoverageInstance::stopChrono((void*)&e);
        throw;
    }
    types::InternalType* pIT = getResult();
    if(pIT == NULL)
    {
        char szError[bsiz];
        os_sprintf(szError, _("%s: Wrong number of output argument(s): %d expected.\n"), "for expression", 1);
        wchar_t* wError = to_wide_string(szError);
        std::wstring err(wError);
        FREE(wError);
        throw InternalError(err, 999, e.getLocation());
    }

    if (pIT->isImplicitList())
    {
        //get IL
        types::ImplicitList* pVar = pIT->getAs<types::ImplicitList>();
        if (pVar->isComputable() == false)
        {
            std::wostringstream os;
            os << _W("Invalid index.\n");
            throw ast::InternalError(os.str(), 999, e.getLocation());
        }
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
            CoverageInstance::stopChrono((void*)&e);
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
                            CoverageInstance::stopChrono((void*)&e);
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
                CoverageInstance::stopChrono((void*)&e);
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

        if (size == 0)
        {
            ctx->put(var, types::Double::Empty());
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
                CoverageInstance::stopChrono((void*)&e);
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
                CoverageInstance::stopChrono((void*)&e);
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
        /* if (pVar->getDims() > 2)
        {
            pIT->DecreaseRef();
            pIT->killMe();
            setResult(NULL);
            CoverageInstance::stopChrono((void*)&e);
            throw InternalError(_W("for expression can only manage 1 or 2 dimensions variables\n"), 999, e.getVardec().getLocation());
        }
        */

        symbol::Variable* var = e.getVardec().getAs<VarDec>()->getStack();
        int dim = pVar->getDims();
        int* dims = pVar->getDimsArray();
        int count = 1;
        for (int i = 1; i < dim; ++i)
        {
            count *= dims[i];
        }

        for (int i = 0; i < count; i++)
        {
            types::GenericType* pNew = pVar->getColumnValues(i);
            if (pNew == NULL)
            {
                pIT->DecreaseRef();
                pIT->killMe();
                setResult(NULL);
                CoverageInstance::stopChrono((void*)&e);
                throw InternalError(_W("for expression : Wrong type for loop iterator.\n"), 999, e.getVardec().getLocation());
            }

            if (ctx->isprotected(var))
            {
                std::wostringstream os;
                os << _W("Redefining permanent variable.\n");
                CoverageInstance::stopChrono((void*)&e);
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
                CoverageInstance::stopChrono((void*)&e);
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
        CoverageInstance::stopChrono((void*)&e);
        throw InternalError(_W("for expression : Wrong type for loop iterator.\n"), 999, e.getVardec().getLocation());
    }

    pIT->DecreaseRef();
    pIT->killMe();

    setResult(NULL);
    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const ReturnExp &e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    if (e.isGlobal())
    {
        if (ConfigVariable::getPauseLevel() != 0 && symbol::Context::getInstance()->getScopeLevel() == ConfigVariable::getActivePauseLevel())
        {
            //return or resume
            ConfigVariable::DecreasePauseLevel();
            CoverageInstance::stopChrono((void*)&e);
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
        if (isLambda() == false && (e.getParent() == nullptr || e.getParent()->isAssignExp() == false))
        {
            CoverageInstance::stopChrono((void*)&e);
            throw InternalError(_W("With input arguments, return / resume expects output arguments.\n"), 999, e.getLocation());
        }

        //in case of CallExp, we can return only one value
        int iSaveExpectedSize = getExpectedSize();
        setExpectedSize(1);
        try
        {
            e.getExp().accept(*this);
        }
        catch (ScilabException &)
        {
            CoverageInstance::stopChrono((void*)&e);
            throw;
        }
        setExpectedSize(iSaveExpectedSize);
        const_cast<ReturnExp*>(&e)->setReturn();
    }

    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const SelectExp &e)
{
    // FIXME : exec select ... case ... else ... end
    CoverageInstance::invokeAndStartChrono((void*)&e);
    try
    {
        e.getSelect()->accept(*this);
    }
    catch (ScilabException &)
    {
        CoverageInstance::stopChrono((void*)&e);
        throw;
    }

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
            CaseExp * pCase = exp->getAs<CaseExp>();
            try
            {
                pCase->getTest()->accept(*this);
            }
            catch (ScilabException &)
            {
                CoverageInstance::stopChrono((void*)&e);
                throw;
            }
            types::InternalType *pITCase = getResult();
            setResult(NULL);
            if (pITCase)
            {
                bool bEqual = false;
                if (pITCase->isCell()) //WARNING ONLY FOR CELL
                {
                    types::Cell* pC = pITCase->getAs<types::Cell>();
                    for (int i = 0; i < pC->getSize(); ++i)
                    {
                        if (*pC->get()[i] == *pIT)
                        {
                            bEqual = true;
                            break;
                        }
                    }
                }
                else if (*pITCase == *pIT)
                {
                    bEqual = true;
                }

                if (bEqual)
                {
                    try
                    {
                        // the good one
                        pCase->getBody()->accept(*this);
                    }
                    catch (const InternalError& ie)
                    {
                        pIT->DecreaseRef();
                        pIT->killMe();
                        CoverageInstance::stopChrono((void*)&e);
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
            CoverageInstance::stopChrono((void*)&e);
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
    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const NotExp &e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    /*
      @ or ~ !
      */
    try
    {
        e.getExp().accept(*this);
    }
    catch (ScilabException &)
    {
        CoverageInstance::stopChrono((void*)&e);
        throw;
    }

    types::InternalType * pValue = getResult();
    types::InternalType * pReturn = NULL;

    if (callObjectUnaryOperator(L"not", pValue, e))
    {
        CoverageInstance::stopChrono((void*)&e);
        return;
    }

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

        types::Callable::ReturnValue Ret = Overload::call(L"%" + pValue->getShortTypeStr() + L"_5", in, 1, out, true, true, e.getLocation());

        if (Ret != types::Callable::OK)
        {
            cleanInOut(in, out);
            CoverageInstance::stopChrono((void*)&e);
            throw InternalError(ConfigVariable::getLastErrorMessage(), ConfigVariable::getLastErrorNumber(), e.getLocation());
        }

        setResult(out);
        cleanIn(in, out);
    }
    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
bool RunVisitorT<T>::callObjectUnaryOperator(const std::wstring& name, types::InternalType* value, const Exp& e)
{
    if (value->isObject() == false)
    {
        return false;
    }

    types::typed_list in = {value};
    types::typed_list out;
    try
    {
        if (Overload::callMethodOperator(name, in, out, e.getLocation()) == false)
        {
            return false;
        }
    }
    catch (...)
    {
        CoverageInstance::stopChrono((void*)&e);
        throw;
    }

    if (out[0] != value)
    {
        value->killMe();
    }

    setResult(out[0]);
    return true;
}

template <class T>
void RunVisitorT<T>::visitprivate(const TransposeExp &e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    try
    {
        e.getExp().accept(*this);
    }
    catch (ScilabException &)
    {
        CoverageInstance::stopChrono((void*)&e);
        throw;
    }

    if (getResultSize() != 1)
    {
        clearResult();
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("%ls: Can not transpose multiple elements.\n").c_str(), L"Transpose");
        CoverageInstance::stopChrono((void*)&e);
        throw InternalError(szError, 999, e.getLocation());
    }

    types::InternalType * pValue = getResult();
    types::InternalType * pReturn = NULL;
    const bool bConjug = e.getConjugate() == TransposeExp::_Conjugate_;


    if (callObjectUnaryOperator(bConjug ? L"ctranspose" : L"transpose", pValue, e))
    {
        CoverageInstance::stopChrono((void*)&e);
        return;
    }

    if ((bConjug && pValue->adjoint(pReturn)) || (!bConjug && pValue->transpose(pReturn)))
    {
        if (pValue != pReturn)
        {
            pValue->killMe();
        }

        setResult(pReturn);
        CoverageInstance::stopChrono((void*)&e);

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
            Ret = Overload::call(L"%" + getResult()->getShortTypeStr() + L"_t", in, 1, out, true, true, e.getLocation());
        }
        else
        {
            Ret = Overload::call(L"%" + getResult()->getShortTypeStr() + L"_0", in, 1, out, true, true, e.getLocation());
        }

        if (Ret != types::Callable::OK)
        {
            cleanInOut(in, out);
            CoverageInstance::stopChrono((void*)&e);
            throw InternalError(ConfigVariable::getLastErrorMessage(), ConfigVariable::getLastErrorNumber(), e.getLocation());
        }

        setResult(out);
        cleanIn(in, out);
    }

    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const FunctionDec & e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    symbol::Context* ctx = symbol::Context::getInstance();
    /*
      function foo
      endfunction
      */

    // funcprot(0) : do nothing
    // funcprot(1) && warning(on) : warning
    types::Macro* pMacro = parseFunctionDec(e);
    if (ctx->isprotected(symbol::Symbol(pMacro->getName())))
    {
        pMacro->killMe();
        std::wostringstream os;
        os << _W("Redefining permanent variable.\n");
        CoverageInstance::stopChrono((void*)&e);
        throw InternalError(os.str(), 999, e.getLocation());
    }

    if (pMacro->isLambda())
    {
        setResult(pMacro);
    }
    else if (ctx->addMacro(pMacro) == false)
    {
        char pstError[1024];
        char* pstFuncName = wide_string_to_UTF8(e.getSymbol().getName().c_str());
        os_sprintf(pstError, _("It is not possible to redefine the %s primitive this way (see clearfun).\n"), pstFuncName);
        wchar_t* pwstError = to_wide_string(pstError);
        std::wstring wstError(pwstError);
        FREE(pstFuncName);
        FREE(pwstError);
        pMacro->killMe();
        CoverageInstance::stopChrono((void*)&e);
        throw InternalError(wstError, 999, e.getLocation());
    }

    // In case of exec file, set the file name in the Macro to store where it is defined.
    std::wstring strFile = ConfigVariable::getExecutedFile();
    const std::vector<ConfigVariable::WhereEntry>& lWhereAmI = ConfigVariable::getWhere();
    if (strFile != L"" &&  // check if we are executing a script or a macro
        lWhereAmI.empty() == false &&
        lWhereAmI.back().m_file_name != nullptr && // check the last function execution is a macro
        *(lWhereAmI.back().m_file_name) == strFile) // check the last execution is the same macro as the executed one
    {
        pMacro->setFileName(strFile);
    }

    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const ArgumentDec & e)
{
    /* FIXME: Implement Run Visitor for ArgumentDec */
}

template <class T>
void RunVisitorT<T>::visitprivate(const ClassDec & e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);

    std::map<std::wstring, types::OBJ_ATTR> props;
    for (auto&& p : e.getProperties())
    {
        PropertiesDec* prop = p->getAs<PropertiesDec>();

        bool staticFlag = false;
        types::AccessModifier accessFlag = types::AccessModifier::PUBLIC;

        for (auto&& a : prop->getAttributes())
        {
            if (a->isCommentExp()) continue;

            if (a->isSimpleVar())
            {
                std::wstring attr(a->getAs<SimpleVar>()->getSymbol().getName());
                std::transform(attr.begin(), attr.end(), attr.begin(), std::towlower);
                if (attr == L"static")
                {
                    staticFlag = true;
                    continue;
                }

                if (attr == L"private")
                {
                    accessFlag = types::AccessModifier::PRIVATE;
                    continue;
                }

                if (attr == L"protected")
                {
                    accessFlag = types::AccessModifier::PROTECTED;
                    continue;
                }

                if (attr == L"public")
                {
                    accessFlag = types::AccessModifier::PUBLIC;
                    continue;
                }

                wchar_t szError[bsiz];
                os_swprintf(szError, bsiz, _W("%ls: Unknown attribute \'%ls\' for properties.\n").c_str(), L"'classdef'", attr.data());
                throw InternalError(szError, 999, a->getLocation());
            }

            if (a->isAssignExp())
            {
                // later
            }
        }

        for (auto&& p : prop->getProperties())
        {
            if (p->isCommentExp()) continue;

            ArgumentDec* dec = p->getAs<ArgumentDec>();
            if (dec)
            {
                std::wstring name = dec->getArgumentName()->getAs<ast::SimpleVar>()->getSymbol().getName();
                types::OBJ_ATTR attr;
                attr.isStatic = staticFlag;
                attr.access = accessFlag;

                // default value
                if (dec->getArgumentDefaultValue()->isNilExp() == false)
                {
                    // the definition is deleted after its execution (exec, console)
                    attr.arg.default_value = dec->getArgumentDefaultValue()->clone();
                }

                types::Classdef::parsePropertyArgument(e.getSymbol().getName() + L"." + name, *dec, attr.arg);
                props[name] = attr;
            }
        }
    }

    std::map<std::wstring, types::OBJ_ATTR> methods;
    for (auto&& m : e.getMethods())
    {
        MethodsDec* method = m->getAs<MethodsDec>();

        bool staticFlag = false;
        types::AccessModifier accessFlag = types::AccessModifier::PUBLIC;

        for (auto&& a : method->getAttributes())
        {
            if (a->isCommentExp()) continue;

            if (a->isSimpleVar())
            {
                std::wstring attr(a->getAs<SimpleVar>()->getSymbol().getName());
                std::transform(attr.begin(), attr.end(), attr.begin(), std::towlower);
                if (attr == L"static")
                {
                    staticFlag = true;
                    continue;
                }

                if (attr == L"private")
                {
                    accessFlag = types::AccessModifier::PRIVATE;
                    continue;
                }

                if (attr == L"protected")
                {
                    accessFlag = types::AccessModifier::PROTECTED;
                    continue;
                }

                if (attr == L"public")
                {
                    accessFlag = types::AccessModifier::PUBLIC;
                    continue;
                }

                wchar_t szError[bsiz];
                os_swprintf(szError, bsiz, _W("%ls: Unknown attribute \'%ls\' for methods.\n").c_str(), L"'classdef'", attr.data());
                throw InternalError(szError, 999, a->getLocation());
            }

            if (a->isAssignExp())
            {
                // later
            }
        }

        for (auto&& me : method->getMethods())
        {
            if (me->isCommentExp()) continue;

            // the constructor initializes an instance (this), it can not be static
            std::wstring methodName;
            if (me->isFunctionDec())
            {
                methodName = me->getAs<ast::FunctionDec>()->getSymbol().getName();
            }
            else if (me->isAssignExp() && me->getAs<ast::AssignExp>()->getLeftExp().isSimpleVar())
            {
                methodName = me->getAs<ast::AssignExp>()->getLeftExp().getAs<ast::SimpleVar>()->getSymbol().getName();
            }

            if (staticFlag && methodName == e.getSymbol().getName())
            {
                wchar_t szError[bsiz];
                os_swprintf(szError, bsiz, _W("%ls: Constructor of \'%ls\' can not be static.\n").c_str(), L"'classdef'", methodName.data());
                throw InternalError(szError, 999, me->getLocation());
            }

            if (me->isFunctionDec())
            {
                types::Macro* method = parseFunctionDec(*me->getAs<ast::FunctionDec>());
                if (method)
                {
                    types::OBJ_ATTR attr;
                    attr.isStatic = staticFlag;
                    attr.access = accessFlag;
                    attr.callable = method;
                    methods[method->getName()] = attr;
                    // In case of exec file, set the file name in the Macro to store where it is defined.
                    std::wstring strFile = ConfigVariable::getExecutedFile();
                    const std::vector<ConfigVariable::WhereEntry>& lWhereAmI = ConfigVariable::getWhere();
                    if (strFile != L"" &&  // check if we are executing a script or a macro
                        lWhereAmI.empty() == false &&
                        lWhereAmI.back().m_file_name != nullptr && // check the last function execution is a macro
                        *(lWhereAmI.back().m_file_name) == strFile) // check the last execution is the same macro as the executed one
                    {
                        method->setFileName(strFile);
                    }
                    continue;
                }
            }

            if (me->isAssignExp())
            {
                AssignExp* ass = me->getAs<ast::AssignExp>();
                if (ass->getLeftExp().isSimpleVar() && ass->getRightExp().isSimpleVar())
                {
                    std::wstring name = ass->getLeftExp().getAs<ast::SimpleVar>()->getSymbol().getName();
                    symbol::Symbol fname = ass->getRightExp().getAs<ast::SimpleVar>()->getSymbol();
                    types::InternalType* f = symbol::Context::getInstance()->get(fname);
                    if (f != nullptr && f->isCallable())
                    {
                        types::OBJ_ATTR attr;
                        attr.isStatic = staticFlag;
                        attr.access = accessFlag;
                        attr.callable = f->getAs<types::Callable>();
                        f->IncreaseRef();
                        methods[name] = attr;
                        continue;
                    }
                    else
                    {
                        wchar_t szError[bsiz];
                        os_swprintf(szError, bsiz, _W("%ls: Right part of assignation must be a function.\n").c_str(), L"'classdef'");
                        throw InternalError(szError, 999, me->getLocation());
                    }
                }
            }

            wchar_t szError[bsiz];
            os_swprintf(szError, bsiz, _W("%ls: Unknown method declaration.\n").c_str(), L"'classdef'");
            throw InternalError(szError, 999, me->getLocation());
        }
    }

    std::map<std::wstring, std::vector<types::InternalType*>> enumerations;
    for (auto&& e : e.getEnumeration())
    {
        if (e->isCommentExp()) continue;

        EnumDec* enums = e->getAs<EnumDec>();
        for (auto&& e : enums->getEnumeration())
        {
            if (e->isSimpleVar())
            {
                enumerations[e->getAs<SimpleVar>()->getSymbol().getName()] = {};
            }
            else if (e->isCallExp())
            {
                ast::CallExp* c = e->getAs<CallExp>();
                std::vector<types::InternalType*> args;
                for (auto&& a : c->getArgs())
                {
                    a->accept(*this);
                    if (getResult() == nullptr)
                    {
                        // mama mia
                    }

                    types::InternalType* pIT = getResult();
                    pIT->IncreaseRef();
                    args.push_back(pIT);
                    setResult(nullptr);
                }

                if (c->getName().isSimpleVar())
                {
                    enumerations[c->getName().getAs<SimpleVar>()->getSymbol().getName()] = args;
                }
            }
        }
    }

    std::vector<std::wstring> superclass;
    for (auto&& e : e.getSuperClasses())
    {
        if (e->isSimpleVar())
        {
            superclass.push_back(e->getAs<SimpleVar>()->getSymbol().getName());
        }
    }

    symbol::Context::getInstance()->addClassdef(new types::Classdef(e.getSymbol().getName(), props, methods, enumerations, superclass));

    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const EnumDec & e)
{
    /* FIXME: Implement Run Visitor for EnumDec */
}

template <class T>
void RunVisitorT<T>::visitprivate(const PropertiesDec & e)
{
    /* FIXME: Implement Run Visitor for PropertiesDec */
}

template <class T>
void RunVisitorT<T>::visitprivate(const MethodsDec & e)
{
    /* FIXME: Implement Run Visitor for MethodsDec */
}

template <class T>
void RunVisitorT<T>::visitprivate(const ListExp &e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    try
    {
        e.getStart().accept(*this);
    }
    catch (ScilabException &)
    {
        CoverageInstance::stopChrono((void*)&e);
        throw;
    }

    types::InternalType* pITStart = getResult();
    types::GenericType* pStart = static_cast<types::GenericType*>(pITStart);
    if (pITStart == NULL ||
            ((pITStart->isGenericType() == false || pStart->getSize() != 1 || (pStart->isDouble() && pStart->getAs<types::Double>()->isComplex())) &&
             pStart->isList() == false)) // list case => call overload
    {
        setResult(NULL);
        wchar_t szError[bsiz];
        if (pITStart && pITStart->isImplicitList())
        {
            os_swprintf(szError, bsiz, _W("%ls: Too many %ls or wrong type for argument %d: Real scalar expected.\n").c_str(), L"':'", L"':'", 1);
        }
        else
        {
            os_swprintf(szError, bsiz, _W("%ls: Wrong type for argument %d: Real scalar expected.\n").c_str(), L"':'", 1);
        }

        if (pITStart)
        {
            pITStart->killMe();
        }

        CoverageInstance::stopChrono((void*)&e);
        throw InternalError(szError, 999, e.getLocation());
    }

    try
    {
        e.getStep().accept(*this);
    }
    catch (ScilabException &)
    {
        CoverageInstance::stopChrono((void*)&e);
        throw;
    }

    types::InternalType* pITStep = getResult();
    types::GenericType* pStep = static_cast<types::GenericType*>(pITStep);
    setResult(NULL);
    if (pITStep == NULL ||
            ((pITStep->isGenericType() == false || pStep->getSize() != 1 || (pStep->isDouble() && pStep->getAs<types::Double>()->isComplex())) &&
             pStep->isList() == false)) // list case => call overload
    {
        pITStart->killMe();
        if (pITStep)
        {
            pITStep->killMe();
        }

        setResult(NULL);
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("%ls: Wrong type for argument %d: Real scalar expected.\n").c_str(), L"':'", 2);
        CoverageInstance::stopChrono((void*)&e);
        throw InternalError(szError, 999, e.getLocation());
    }

    try
    {
        e.getEnd().accept(*this);
    }
    catch (ScilabException &)
    {
        CoverageInstance::stopChrono((void*)&e);
        throw;
    }

    types::InternalType* pITEnd = getResult();
    types::GenericType* pEnd = static_cast<types::GenericType*>(pITEnd);
    setResult(NULL);
    if (pITEnd == NULL ||
            ((pITEnd->isGenericType() == false || pEnd->getSize() != 1 || (pEnd->isDouble() && pEnd->getAs<types::Double>()->isComplex())) &&
             pEnd->isList() == false)) // list case => call overload
    {
        pITStart->killMe();
        pITStep->killMe();
        if (pITEnd)
        {
            pITEnd->killMe();
        }

        setResult(NULL);
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("%ls: Wrong type for argument %d: Real scalar expected.\n").c_str(), L"':'", 2 + e.hasExplicitStep());
        CoverageInstance::stopChrono((void*)&e);
        throw InternalError(szError, 999, e.getLocation());
    }

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
    if ((pStart->isPoly() || pStart->isDouble()) &&
            (pStep->isPoly() || pStep->isDouble()) &&
            (pEnd->isPoly() || pEnd->isDouble()))
    {
        // No need to kill piStart, ... because Implicit list ctor will incref them
        types::ImplicitList* pIL = new types::ImplicitList(pStart, pStep, pEnd);
        try
        {
            pIL->compute();
        }
        catch (const InternalError& ie)
        {
            // happends when compute() of ImplicitList cannot allocate memory
            pIL->killMe();
            throw ie;
        }

        setResult(pIL);
        CoverageInstance::stopChrono((void*)&e);
        return;
    }

    // int : double or int : int
    if (pStart->isInt() &&
            (pStep->isDouble() || pStep->isInt()) &&
            pEnd->isInt())
    {
        // check for same int type int8, int 16 ...
        if (pStart->getType() == pEnd->getType() &&
                (pStart->getType() == pStep->getType() ||
                 pStep->isDouble()))
        {
            // No need to kill piStart, ... because Implicit list ctor will incref them
            types::ImplicitList* pIL = new types::ImplicitList(pStart, pStep, pEnd);
            try
            {
                pIL->compute();
            }
            catch (const InternalError& ie)
            {
                // happends when compute() of ImplicitList cannot allocate memory
                pIL->killMe();
                throw ie;
            }

            setResult(pIL);
            CoverageInstance::stopChrono((void*)&e);
            return;
        }
    }

    // Call Overload
    types::Callable::ReturnValue Ret;
    types::typed_list in;
    types::typed_list out;

    pStart->IncreaseRef();
    in.push_back(pStart);

    try
    {
        if (e.hasExplicitStep())
        {
            // 1:2:4
            //call overload %typeStart_b_typeStep
            pStep->IncreaseRef();
            in.push_back(pStep);
            pEnd->IncreaseRef();
            in.push_back(pEnd);
            Ret = Overload::call(L"%" + pStart->getShortTypeStr() + L"_b_" + pStep->getShortTypeStr(), in, 1, out, true, true, e.getLocation());
        }
        else
        {
            // 1:2
            //call overload %typeStart_b_typeEnd
            pStep->killMe();
            pEnd->IncreaseRef();
            in.push_back(pEnd);
            Ret = Overload::call(L"%" + pStart->getShortTypeStr() + L"_b_" + pEnd->getShortTypeStr(), in, 1, out, true, true, e.getLocation());
        }
    }
    catch (const InternalError& error)
    {
        setResult(NULL);
        cleanInOut(in, out);
        CoverageInstance::stopChrono((void*)&e);
        throw error;
    }

    if (Ret != types::Callable::OK)
    {
        setResult(NULL);
        cleanInOut(in, out);
        CoverageInstance::stopChrono((void*)&e);
        throw InternalError(ConfigVariable::getLastErrorMessage(), ConfigVariable::getLastErrorNumber(), e.getLocation());
    }

    setResult(out);
    cleanIn(in, out);
    CoverageInstance::stopChrono((void*)&e);
}

template <class T>
void RunVisitorT<T>::visitprivate(const TryCatchExp  &e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    //save current prompt mode
    bool oldVal = ConfigVariable::isSilentError();
    int oldMode = ConfigVariable::getPromptMode();
    //set mode silent for errors
    ConfigVariable::setSilentError(true);

    symbol::Context* pCtx = symbol::Context::getInstance();
    try
    {
        int scope = pCtx->getScopeLevel();
        int level = ConfigVariable::getRecursionLevel();
        try
        {
            const_cast<Exp*>(&e.getTry())->setReturnable();
            e.getTry().accept(*this);
            //restore previous prompt mode
            ConfigVariable::setSilentError(oldVal);

            if (e.getTry().isReturn())
            {
                const_cast<Exp*>(&e.getTry())->resetReturn();
                const_cast<TryCatchExp*>(&e)->setReturn();
            }

            if (e.getTry().isContinue())
            {
                const_cast<Exp*>(&e.getTry())->resetContinue();
                const_cast<TryCatchExp*>(&e)->setContinue();
            }

            if (e.getTry().isBreak())
            {
                const_cast<Exp*>(&e.getTry())->resetBreak();
                const_cast<TryCatchExp*>(&e)->setBreak();
            }
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
            CoverageInstance::stopChrono((void*)&e);
            throw ast::InternalError(sz);
        }
        catch (const InternalAbort& ia)
        {
            //restore previous prompt mode
            ConfigVariable::setSilentError(oldVal);
            throw ia;
        }
    }
    catch (const InternalError& /* ie */)
    {
        //restore previous prompt mode
        ConfigVariable::setSilentError(oldVal);
        //to lock lasterror
        ConfigVariable::setLastErrorCall();
        // reset call stack filled when error occurred
        ConfigVariable::resetWhereError();
        // reset error flag
        ConfigVariable::resetError();
        try
        {
            const_cast<Exp*>(&e.getCatch())->setReturnable();
            e.getCatch().accept(*this);
            if (e.getCatch().isReturn())
            {
                const_cast<Exp*>(&e.getCatch())->resetReturn();
                const_cast<TryCatchExp*>(&e)->setReturn();
            }

            if (e.getCatch().isContinue())
            {
                const_cast<Exp*>(&e.getCatch())->resetContinue();
                const_cast<TryCatchExp*>(&e)->setContinue();
            }

            if (e.getCatch().isBreak())
            {
                const_cast<Exp*>(&e.getCatch())->resetBreak();
                const_cast<TryCatchExp*>(&e)->setBreak();
            }
        }
        catch (ScilabException &)
        {
            CoverageInstance::stopChrono((void*)&e);
            throw;
        }
    }
    CoverageInstance::stopChrono((void*)&e);
}


} /* namespace ast */

#include "run_SeqExp.hpp"
#include "run_CallExp.hpp"
#include "run_MatrixExp.hpp"
#include "run_OpExp.hpp"
#include "run_AssignExp.hpp"

template EXTERN_AST class ast::RunVisitorT<ast::ExecVisitor>;
template EXTERN_AST class ast::RunVisitorT<ast::StepVisitor>;
template EXTERN_AST class ast::RunVisitorT<ast::TimedVisitor>;
template EXTERN_AST class ast::RunVisitorT<ast::DebuggerVisitor>;
