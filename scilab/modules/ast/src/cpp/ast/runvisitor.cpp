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

#include <string>

#include "runvisitor.hxx"
#include "execvisitor.hxx"
#include "stepvisitor.hxx"
#include "timedvisitor.hxx"
#include "shortcutvisitor.hxx"
#include "printvisitor.hxx"
#include "mutevisitor.hxx"

#include "visitor_common.hxx"

#include "context.hxx"
#include "generic_operations.hxx"
#include "types_or.hxx"
#include "types_and.hxx"
#include "localization.hxx"

#include "macrofile.hxx"
#include "macro.hxx"

extern "C"
{
#include "sciprint.h"
#include "os_swprintf.h"
}

namespace ast
{
template <class T>
void RunVisitorT<T>::visitprivate(const CellExp &e)
{
    std::list<MatrixLineExp *>::const_iterator row;
    std::list<Exp *>::const_iterator col;
    int iColMax = 0;

    //check dimmension
    for (row = e.getLines().begin() ; row != e.getLines().end() ; ++row )
    {
        if (iColMax == 0)
        {
            iColMax = static_cast<int>((*row)->getColumns().size());
        }

        if (iColMax != static_cast<int>((*row)->getColumns().size()))
        {
            std::wostringstream os;
            os << _W("inconsistent row/column dimensions\n");
            //os << ((Location)(*row)->getLocation()).getLocationString() << std::endl;
            throw ScilabError(os.str(), 999, (*row)->getLocation());
        }
    }

    //alloc result cell
    types::Cell *pC = new types::Cell(static_cast<int>(e.getLines().size()), iColMax);

    int i = 0;
    int j = 0;

    //insert items in cell
    for (i = 0, row = e.getLines().begin() ; row != e.getLines().end() ; ++row, ++i)
    {
        for (j = 0, col = (*row)->getColumns().begin() ; col != (*row)->getColumns().end() ; ++col, ++j)
        {
            (*col)->accept(*this);
            InternalType *pIT = getResult();
            if (pIT->isImplicitList())
            {
                InternalType * _pIT = pIT->getAs<ImplicitList>()->extractFullMatrix();
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
        throw ScilabError(szError, 999, e.getLocation());
    }

    try
    {
        e.getHead()->accept(*this);
    }
    catch (const ScilabError& error)
    {
        throw error;
    }

    if (getResult() == NULL)
    {
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("Attempt to reference field of non-structure array.\n").c_str());
        throw ScilabError(szError, 999, e.getLocation());
    }

    // TODO: handle case where getSize() > 1
    // l=list(struct("toto","coucou"),struct("toto","hello"),1,2);[a,b]=l(1:2).toto
    //
    if (getResultSize() > 1)
    {
        clearResult();
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("Not yet implemented in Scilab.\n").c_str());
        throw ScilabError(szError, 999, e.getLocation());
    }

    SimpleVar * psvRightMember = static_cast<SimpleVar *>(const_cast<Exp *>(e.getTail()));
    std::wstring wstField = psvRightMember->getSymbol().getName();
    InternalType * pValue = getResult();
    InternalType * pReturn = NULL;
    bool ok;

    try
    {
        ok = pValue->extract(wstField, pReturn);
    }
    catch (std::wstring & err)
    {
        pValue->killMe();
        throw ScilabError(err.c_str(), 999, e.getTail()->getLocation());
    }

    if (ok)
    {
        setResult(pReturn);
    }
    else if (pValue->isFieldExtractionOverloadable())
    {
        types::typed_list in;
        types::typed_list out;

        String* pS = new String(wstField.c_str());

        //TODO: in the case where overload is a macro there is no need to incref in
        // because args will be put in context, removed and killed if required.
        // But if the overload is a function... it is another story...

        pS->IncreaseRef();
        pValue->IncreaseRef();

        in.push_back(pS);
        in.push_back(pValue);

        Callable::ReturnValue Ret = Overload::call(L"%" + pValue->getShortTypeStr() + L"_e", in, 1, out, this);

        if (Ret != Callable::OK)
        {
            cleanInOut(in, out);
            throw ScilabError();
        }

        setResult(out);
        cleanIn(in, out);
    }
    else
    {
        pValue->killMe();
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("Attempt to reference field of non-structure array.\n").c_str());
        throw ScilabError(szError, 999, e.getLocation());
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
        //condition == true
        if (e.isBreakable())
        {
            const_cast<IfExp*>(&e)->resetBreak();
            const_cast<Exp*>(&e.getThen())->setBreakable();
        }

        if (e.isContinuable())
        {
            const_cast<IfExp*>(&e)->resetContinue();
            const_cast<Exp*>(&e.getThen())->setContinuable();
        }

        if (e.isReturnable())
        {
            const_cast<IfExp*>(&e)->resetReturn();
            const_cast<Exp*>(&e.getThen())->setReturnable();
        }

        e.getThen().accept(*this);
    }
    else
    {
        //condition == false

        if (e.hasElse())
        {
            if (e.isBreakable())
            {
                const_cast<Exp*>(&e.getElse())->setBreakable();
            }

            if (e.isContinuable())
            {
                const_cast<IfExp*>(&e)->resetContinue();
                const_cast<Exp*>(&e.getElse())->setContinuable();
            }

            if (e.isReturnable())
            {
                const_cast<Exp*>(&e.getElse())->setReturnable();
            }

            e.getElse().accept(*this);
        }
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
    //allow break and continue operations
    const_cast<Exp*>(&e.getBody())->setBreakable();
    const_cast<Exp*>(&e.getBody())->setContinuable();
    //allow return operation
    if (e.isReturnable())
    {
        (&e.getBody())->isReturnable();
    }

    //condition
    e.getTest().accept(*this);
    while (getResult()->isTrue())
    {
        e.getBody().accept(*this);
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
            const_cast<WhileExp*>(&e)->setContinue();
            const_cast<Exp*>(&(e.getBody()))->resetContinue();
            e.getTest().accept(*this);
            continue;
        }

        //clear old result value before evaluate new one
        if (getResult() != NULL)
        {
            getResult()->killMe();
        }

        e.getTest().accept(*this);
    }

    //clear result of condition or result of body
    clearResult();
}

template <class T>
void RunVisitorT<T>::visitprivate(const ForExp  &e)
{
    e.getVardec().accept(*this);
    InternalType* pIT = getResult();
    //allow break and continue operations
    const_cast<Exp&>(e.getBody()).setBreakable();
    const_cast<Exp&>(e.getBody()).setContinuable();

    //allow return operation
    if (e.isReturnable())
    {
        e.getBody().isReturnable();
    }

    if (getResult()->isImplicitList())
    {
        ImplicitList* pVar = pIT->getAs<ImplicitList>();
        for (int i = 0; i < pVar->getSize(); ++i)
        {
            //TODO : maybe it would be interesting here to reuse the same InternalType (to avoid delete/new)
            InternalType * pIL = pVar->extractValue(i);
            symbol::Context::getInstance()->put(e.getVardec().getStack(), pIL);

            e.getBody().accept(*this);
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
                break;
            }
        }
    }
    else if (getResult()->isList())
    {
        List* pL = pIT->getAs<List>();
        const int size = pL->getSize();
        for (int i = 0; i < size; ++i)
        {
            InternalType* pNew = pL->get(i);
            symbol::Context::getInstance()->put(e.getVardec().getStack(), pNew);

            e.getBody().accept(*this);
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
                break;
            }
        }
    }
    else
    {
        //Matrix i = [1,3,2,6] or other type
        GenericType* pVar = pIT->getAs<GenericType>();
        if (pVar->getDims() > 2)
        {
            pIT->DecreaseRef();
            pIT->killMe();

            throw ScilabError(_W("for expression can only manage 1 or 2 dimensions variables\n"), 999, e.getVardec().getLocation());
        }

        for (int i = 0; i < pVar->getCols(); i++)
        {
            GenericType* pNew = pVar->getColumnValues(i);
            symbol::Context::getInstance()->put(e.getVardec().getStack(), pNew);

            e.getBody().accept(*this);
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
                break;
            }
        }
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
        //return or resume
        if (ConfigVariable::getPauseLevel() != 0)
        {
            ThreadId* pThreadId = ConfigVariable::getLastPausedThread();
            if (pThreadId == NULL)
            {
                //no paused thread, so just go leave
                return;
            }

            //force exit without prompt of current thread ( via Aborted status )
            ThreadId* pMe = ConfigVariable::getThread(__GetCurrentThreadKey());
            pMe->setStatus(ThreadId::Aborted);

            //resume previous execution thread
            pThreadId->resume();

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

        if (getResultSize() == 1)
        {
            //protect variable
            getResult()->IncreaseRef();
        }
        else
        {
            for (int i = 0 ; i < getResultSize() ; i++)
            {
                //protect variable
                getResult(i)->IncreaseRef();
            }
        }

        if (getResultSize() == 1)
        {
            //unprotect variable
            getResult()->DecreaseRef();
        }
        else
        {
            for (int i = 0 ; i < getResultSize() ; i++)
            {
                //unprotect variable
                getResult(i)->DecreaseRef();
            }
        }

        const_cast<ReturnExp*>(&e)->setReturn();
    }
}

template <class T>
void RunVisitorT<T>::visitprivate(const SelectExp &e)
{
    // FIXME : exec select ... case ... else ... end
    e.getSelect()->accept(*this);
    bool bCase = false;


    InternalType* pIT = getResult();
    setResult(NULL);
    if (pIT)
    {
        //find good case
        cases_t::iterator it;
        for (it = e.getCases()->begin(); it != e.getCases()->end() ; it++)
        {
            CaseExp* pCase = *it;
            pCase->getTest()->accept(*this);
            InternalType *pITCase = getResult();
            setResult(NULL);
            if (pITCase)
            {
                if (pITCase->isContainer()) //WARNING ONLY FOR CELL
                {
                    //check each item
                }
                else if (*pITCase == *pIT)
                {
                    if (e.isBreakable())
                    {
                        const_cast<SelectExp*>(&e)->resetBreak();
                        pCase->getBody()->setBreakable();
                    }

                    if (e.isContinuable())
                    {
                        const_cast<SelectExp*>(&e)->resetContinue();
                        pCase->getBody()->setContinuable();
                    }

                    if (e.isReturnable())
                    {
                        const_cast<SelectExp*>(&e)->resetReturn();
                        pCase->getBody()->setReturnable();
                    }

                    //the good one
                    pCase->getBody()->accept(*this);

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

                    bCase = true;
                    break;
                }
            }
        }
    }

    if (bCase == false && e.getDefaultCase() != NULL)
    {
        if (e.isBreakable())
        {
            const_cast<SelectExp*>(&e)->resetBreak();
            e.getDefaultCase()->setBreakable();
        }

        if (e.isContinuable())
        {
            const_cast<SelectExp*>(&e)->resetContinue();
            e.getDefaultCase()->setContinuable();
        }

        if (e.isReturnable())
        {
            const_cast<SelectExp*>(&e)->resetReturn();
            e.getDefaultCase()->setReturnable();
        }

        //default case
        e.getDefaultCase()->accept(*this);

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
}

template <class T>
void RunVisitorT<T>::visitprivate(const SeqExp  &e)
{
    //T execMe;
    std::list<Exp *>::const_iterator        itExp;

    for (itExp = e.getExps().begin (); itExp != e.getExps().end (); ++itExp)
    {
        if (e.isBreakable())
        {
            (*itExp)->resetBreak();
            (*itExp)->setBreakable();
        }

        if (e.isContinuable())
        {
            (*itExp)->resetContinue();
            (*itExp)->setContinuable();
        }

        if (e.isReturnable())
        {
            (*itExp)->setReturnable();
        }

        try
        {
            //reset default values
            setResult(NULL);
            setExpectedSize(-1);
            (*itExp)->accept(*this);
            InternalType * pIT = getResult();

            if (pIT != NULL)
            {
                bool bImplicitCall = false;
                if (pIT->isCallable()) //to manage call without ()
                {
                    Callable *pCall = pIT->getAs<Callable>();
                    typed_list out;
                    typed_list in;
                    optional_list opt;

                    try
                    {
                        //in this case of calling, we can return only one values
                        int iSaveExpectedSize = getExpectedSize();
                        setExpectedSize(1);
                        Function::ReturnValue Ret = pCall->call(in, opt, getExpectedSize(), out, this);
                        setExpectedSize(iSaveExpectedSize);

                        if (Ret == Callable::OK)
                        {
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
                        else if (Ret == Callable::Error)
                        {
                            if (ConfigVariable::getLastErrorFunction() == L"")
                            {
                                ConfigVariable::setLastErrorFunction(pCall->getName());
                                ConfigVariable::setLastErrorLine(e.getLocation().first_line);
                                throw ScilabError();
                            }

                            if (pCall->isMacro() || pCall->isMacroFile())
                            {
                                wchar_t szError[bsiz];
                                os_swprintf(szError, bsiz, _W("at line % 5d of function %ls called by :\n").c_str(), (*itExp)->getLocation().first_line, pCall->getName().c_str());
                                throw ScilabMessage(szError);
                            }
                            else
                            {
                                throw ScilabMessage();
                            }
                        }
                    }
                    catch (ScilabMessage sm)
                    {
                        wostringstream os;
                        PrintVisitor printMe(os);
                        (*itExp)->accept(printMe);
                        //os << std::endl << std::endl;
                        if (ConfigVariable::getLastErrorFunction() == L"")
                        {
                            ConfigVariable::setLastErrorFunction(pCall->getName());
                        }

                        if (pCall->isMacro() || pCall->isMacroFile())
                        {
                            wchar_t szError[bsiz];
                            os_swprintf(szError, bsiz, _W("at line % 5d of function %ls called by :\n").c_str(), sm.GetErrorLocation().first_line, pCall->getName().c_str());
                            throw ScilabMessage(szError + os.str());
                        }
                        else
                        {
                            sm.SetErrorMessage(sm.GetErrorMessage() + os.str());
                            throw sm;
                        }
                    }
                }

                //don't output Simplevar and empty result
                if (getResult() != NULL && (!(*itExp)->isSimpleVar() || bImplicitCall))
                {
                    //symbol::Context::getInstance()->put(symbol::Symbol(L"ans"), *execMe.getResult());
                    InternalType* pITAns = getResult();
                    symbol::Context::getInstance()->put(m_pAns, pITAns);
                    if ((*itExp)->isVerbose() && ConfigVariable::isPromptShow())
                    {
                        //TODO manage multiple returns
                        scilabWriteW(L" ans  =\n\n");
                        VariableToString(pITAns, L"ans");
                    }
                }

                pIT->killMe();
            }

            if ((&e)->isBreakable() && (*itExp)->isBreak())
            {
                const_cast<SeqExp *>(&e)->setBreak();
                break;
            }

            if ((&e)->isContinuable() && (*itExp)->isContinue())
            {
                const_cast<SeqExp *>(&e)->setContinue();
                break;
            }

            if ((&e)->isReturnable() && (*itExp)->isReturn())
            {
                const_cast<SeqExp *>(&e)->setReturn();
                (*itExp)->resetReturn();
                break;
            }
        }
        catch (const ScilabMessage& sm)
        {
            scilabErrorW(sm.GetErrorMessage().c_str());

            CallExp* pCall = dynamic_cast<CallExp*>(*itExp);
            if (pCall != NULL)
            {
                //to print call expression only of it is a macro
                pCall->getName().accept(*this);

                if (getResult() != NULL && (getResult()->isMacro() || getResult()->isMacroFile()))
                {
                    wostringstream os;
                    PrintVisitor printMe(os);
                    pCall->accept(printMe);
                    //os << std::endl << std::endl;
                    if (ConfigVariable::getLastErrorFunction() == L"")
                    {
                        ConfigVariable::setLastErrorFunction(((InternalType*)getResult())->getAs<Callable>()->getName());
                    }
                    throw ScilabMessage(os.str(), 0, (*itExp)->getLocation());
                }
            }

            throw ScilabMessage((*itExp)->getLocation());
        }
        catch (const ScilabError& se)
        {
            // check on error number because error message can be empty.
            if (ConfigVariable::getLastErrorNumber() == 0)
            {
                ConfigVariable::setLastErrorMessage(se.GetErrorMessage());
                ConfigVariable::setLastErrorNumber(se.GetErrorNumber());
                ConfigVariable::setLastErrorLine(se.GetErrorLocation().first_line);
                ConfigVariable::setLastErrorFunction(wstring(L""));
            }

            CallExp* pCall = dynamic_cast<CallExp*>(*itExp);
            if (pCall != NULL)
            {
                //to print call expression only of it is a macro
                try
                {
                    pCall->getName().accept(*this);
                    if (getResult() != NULL && (getResult()->isMacro() || getResult()->isMacroFile()))
                    {
                        wostringstream os;
                        PrintVisitor printMe(os);
                        pCall->accept(printMe);
                        //os << std::endl << std::endl;
                        ConfigVariable::setLastErrorFunction(((InternalType*)getResult())->getAs<Callable>()->getName());
                        scilabErrorW(se.GetErrorMessage().c_str());
                        throw ScilabMessage(os.str(), 999, (*itExp)->getLocation());
                    }
                }
                catch (ScilabError se2)
                {
                    //just to catch exception, do nothing
                }
            }

            scilabErrorW(se.GetErrorMessage().c_str());
            scilabErrorW(L"\n");
            throw ScilabMessage((*itExp)->getLocation());
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

    InternalType * pValue = getResult();
    InternalType * pReturn = NULL;
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

        Callable::ReturnValue Ret = Overload::call(L"%" + pValue->getShortTypeStr() + L"_5", in, 1, out, this);

        if (Ret != Callable::OK)
        {
            cleanInOut(in, out);
            throw ScilabError();
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
        throw ScilabError(szError, 999, e.getLocation());
    }

    InternalType * pValue = getResult();
    InternalType * pReturn = NULL;
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

        Callable::ReturnValue Ret;
        if (bConjug)
        {
            Ret = Overload::call(L"%" + getResult()->getShortTypeStr() + L"_t", in, 1, out, this);
        }
        else
        {
            Ret = Overload::call(L"%" + getResult()->getShortTypeStr() + L"_0", in, 1, out, this);
        }

        if (Ret != Callable::OK)
        {
            cleanInOut(in, out);
            throw ScilabError();
        }

        setResult(out);
        cleanIn(in, out);
    }
}

template <class T>
void RunVisitorT<T>::visitprivate(const FunctionDec & e)
{
    /*
      function foo
      endfunction
    */

    // funcprot(0) : do nothing
    // funcprot(1) && warning(on) : warning
    //get input parameters list
    std::list<symbol::Variable*> *pVarList = new std::list<symbol::Variable*>();
    const ArrayListVar *pListVar = &e.getArgs();
    for (std::list<Var *>::const_iterator i = pListVar->getVars().begin(), end = pListVar->getVars().end(); i != end; ++i)
    {
        pVarList->push_back(static_cast<SimpleVar*>(*i)->getStack());
    }

    //get output parameters list
    std::list<symbol::Variable*> *pRetList = new std::list<symbol::Variable*>();
    const ArrayListVar *pListRet = &e.getReturns();
    for (std::list<Var *>::const_iterator i = pListRet->getVars().begin(), end = pListRet->getVars().end(); i != end; ++i)
    {
        pRetList->push_back(static_cast<SimpleVar*>(*i)->getStack());
    }

    types::Macro *pMacro = new types::Macro(e.getSymbol().getName(), *pVarList, *pRetList,
                                            const_cast<SeqExp&>(static_cast<const SeqExp&>(e.getBody())), L"script");
    pMacro->setFirstLine(e.getLocation().first_line);

    bool bEquals = false;
    int iFuncProt = ConfigVariable::getFuncprot();
    if (iFuncProt != 0)
    {
        types::InternalType* pITFunc = symbol::Context::getInstance()->get(((FunctionDec&)e).getStack());
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
        sprintf(pstError, _("It is not possible to redefine the %s primitive this way (see clearfun).\n"), pstFuncName);
        wchar_t* pwstError = to_wide_string(pstError);
        std::wstring wstError(pwstError);
        FREE(pstFuncName);
        FREE(pwstError);
        throw ScilabError(wstError, 999, e.getLocation());
    }

    symbol::Context::getInstance()->addMacro(pMacro);

}

template <class T>
void RunVisitorT<T>::visitprivate(const ListExp &e)
{
    e.getStart().accept(*this);
    GenericType* pITStart = static_cast<GenericType*>(getResult());
    if ((pITStart->getSize() != 1 || (pITStart->isDouble() && pITStart->getAs<Double>()->isComplex())) &&
            pITStart->isList() == false) // list case => call overload
    {
        pITStart->killMe();
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("%ls: Wrong type for argument %d: Real scalar expected.\n").c_str(), L"':'", 1);
        throw ScilabError(szError, 999, e.getLocation());
    }
    InternalType * piStart = pITStart;

    e.getStep().accept(*this);
    GenericType* pITStep = static_cast<GenericType*>(getResult());
    if ((pITStep->getSize() != 1 || (pITStep->isDouble() && pITStep->getAs<Double>()->isComplex())) &&
            pITStep->isList() == false) // list case => call overload
    {
        pITStart->killMe();
        pITStep->killMe();
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("%ls: Wrong type for argument %d: Real scalar expected.\n").c_str(), L"':'", 2);
        throw ScilabError(szError, 999, e.getLocation());
    }
    InternalType* piStep = pITStep;

    e.getEnd().accept(*this);
    GenericType* pITEnd = static_cast<GenericType*>(getResult());
    if ((pITEnd->getSize() != 1 || (pITEnd->isDouble() && pITEnd->getAs<Double>()->isComplex())) &&
            pITEnd->isList() == false) // list case => call overload
    {
        pITStart->killMe();
        pITStep->killMe();
        pITEnd->killMe();
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("%ls: Wrong type for argument %d: Real scalar expected.\n").c_str(), L"':'", 3);
        throw ScilabError(szError, 999, e.getLocation());
    }
    InternalType* piEnd = pITEnd;

    //check compatibility
    // double : double : double or poly : poly : poly and mix like double : double : poly
    if ((piStart->isPoly() || piStart->isDouble()) &&
            (piStep->isPoly()  || piStep->isDouble())  &&
            (piEnd->isPoly()   || piEnd->isDouble()))
    {
        // No need to kill piStart, ... because Implicit list ctor will incref them
        setResult(new ImplicitList(piStart, piStep, piEnd));
        return;
    }

    // int : double or int : int
    if ( piStart->isInt()   &&
            (piStep->isDouble() || piStep->isInt()) &&
            piEnd->isInt())
    {
        // check for same int type int8, int 16 ...
        if (piStart->getType() == piEnd->getType()  &&
                (piStart->getType() == piStep->getType() ||
                 piStep->isDouble()))
        {
            // No need to kill piStart, ... because Implicit list ctor will incref them
            setResult(new ImplicitList(piStart, piStep, piEnd));
            return;
        }
    }

    // Call Overload
    Callable::ReturnValue Ret;
    types::typed_list in;
    types::typed_list out;

    piStart->IncreaseRef();
    piStep->IncreaseRef();
    piEnd->IncreaseRef();

    in.push_back(piStart);
    if (e.hasExplicitStep())
    {
        // 1:2:4
        //call overload %typeStart_b_typeEnd
        in.push_back(piStep);
        in.push_back(piEnd);
        Ret = Overload::call(L"%" + piStart->getShortTypeStr() + L"_b_" + piStep->getShortTypeStr(), in, 1, out, this, true);
    }
    else
    {
        // 1:2
        //call overload %typeStart_b_typeStep
        in.push_back(piEnd);
        Ret = Overload::call(L"%" + piStart->getShortTypeStr() + L"_b_" + piEnd->getShortTypeStr(), in, 1, out, this, true);
    }

    if (Ret != Callable::OK)
    {
        cleanInOut(in, out);
        throw ScilabError();
    }

    setResult(out);
    cleanIn(in, out);
}

#include "run_CallExp.cpp"
#include "run_MatrixExp.cpp"
#include "run_OpExp.cpp"
#include "run_AssignExp.cpp"
}

template EXTERN_AST class ast::RunVisitorT<ast::ExecVisitor>;
template EXTERN_AST class ast::RunVisitorT<ast::StepVisitor>;
template EXTERN_AST class ast::RunVisitorT<ast::TimedVisitor>;
