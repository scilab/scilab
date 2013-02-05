/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef AST_RUNVISITOR_HXX
#define AST_RUNVISITOR_HXX

#include <time.h>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <iostream>

#include "visitor_common.hxx"
//#include "runvisitor.hxx"
//#include "execvisitor.hxx"
//#include "timedvisitor.hxx"
#include "shortcutvisitor.hxx"
#include "printvisitor.hxx"
#include "mutevisitor.hxx"

// Needed by visitprivate(const OpExp &)
// Needed by visitprivate(const LogicalOpExp &)
#include "generic_operations.hxx"
#include "types_or_and.hxx"
#include "configvariable.hxx"
#include "overload.hxx"
#include "scilabexception.hxx"

#include "matrix_transpose_int.hxx"

extern "C" {
#include "doublecomplex.h"
#include "matrix_transpose.h"
#include "os_swprintf.h"
#include "more.h"
#include "sciprint.h"
#include "MALLOC.h"
}

#include "timer.hxx"
#include "localization.h"

#include "scilabWrite.hxx"
#include "context.hxx"

#include "all.hxx"
#include "types.hxx"
#include "alltypes.hxx"

// FIXME : remove those using
using namespace types;

namespace ast
{
class RunVisitor : public ConstVisitor
{
public:
    RunVisitor()
    {
        _excepted_result = -1;
        _resultVect.push_back(NULL);
        _result = NULL;
        m_bSingleResult = true;
        m_pAns = new symbol::Symbol(L"ans");
    }

    ~RunVisitor()
    {
        result_clear();
    }

    void result_clear()
    {
        if (is_single_result())
        {
            if (_result != NULL && _result->isDeletable() == true)
            {
                //					std::cout << "before single delete : " << _result << std::endl;
                delete _result;
                //					std::cout << "after single delete" << std::endl;
            }
            _result = NULL;
        }
        else
        {
            for (unsigned int i = 0 ; i < _resultVect.size() ; i++)
            {
                if (_resultVect[i] != NULL && _resultVect[i]->isDeletable() == true)
                {
                    delete _resultVect[i];
                }
                _resultVect[i] = NULL;
            }
        }
        _resultVect.clear();
        m_bSingleResult = true;
        _result = NULL;
    }

public:
    int expected_getSize(void)
    {
        return _excepted_result;
    }

    int result_getSize(void)
    {
        if (is_single_result())
        {
            if (_result == NULL)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return static_cast<int>(_resultVect.size());
        }
    }

    void expected_setSize(int _iSize)
    {
        _excepted_result = _iSize;
    }

    types::InternalType* result_get(void)
    {
        if (is_single_result())
        {
            return _result;
        }
        else
        {
            return _resultVect[0];
        }
    }

    types::InternalType* result_get(int _iPos)
    {
        if (is_single_result() && _iPos == 0)
        {
            return _result;
        }

        if (_iPos >= static_cast<int>(_resultVect.size()))
        {
            return NULL;
        }
        return _resultVect[_iPos];
    }

    vector<types::InternalType*>* result_list_get()
    {
        if (result_getSize() == 1)
        {
            vector<types::InternalType*>* pList = new vector<types::InternalType*>;
            pList->push_back(_result);
            return pList;
        }
        else
        {
            return &_resultVect;
        }
    }

    void result_set(int _iPos, const types::InternalType *gtVal)
    {
        m_bSingleResult = false;
        if (_iPos <  static_cast<int>(_resultVect.size()))
        {
            if (_resultVect[_iPos] != NULL && _resultVect[_iPos]->isDeletable())
            {
                delete _resultVect[_iPos];
            }
        }

        if (_iPos >=  static_cast<int>(_resultVect.size()))
        {
            _resultVect.resize(_iPos + 1, NULL);
        }

        _resultVect[_iPos] = const_cast<types::InternalType *>(gtVal);
    }

    void result_set(const types::InternalType *gtVal)
    {
        m_bSingleResult = true;
        _result = const_cast<types::InternalType *>(gtVal);
    }

    bool is_single_result()
    {
        return m_bSingleResult;
    }

    /*-------------.
    | Attributes.  |
    `-------------*/
protected:
    vector<types::InternalType*>	_resultVect;
    types::InternalType*	_result;
    bool m_bSingleResult;
    int _excepted_result;
    symbol::Symbol* m_pAns;
};

template <class T>
class RunVisitorT : public RunVisitor
{
public :
    RunVisitorT() : RunVisitor()
    {
    }

    types::typed_list* GetArgumentList(std::list<ast::Exp *>const& _plstArg)
    {
        types::typed_list* pArgs = new types::typed_list();
        std::list<ast::Exp *>::const_iterator it;
        for (it = _plstArg.begin() ; it != _plstArg.end() ; it++)
        {
            (*it)->accept(*this);
            if (result_getSize() > 1)
            {
                for (int i = 0 ; i < result_getSize() ; i++)
                {
                    pArgs->push_back(result_get(i));
                }
            }
            else
            {
                pArgs->push_back(result_get());
            }
        }
        //to be sure, delete operation does not delete result
        result_set(NULL);
        return pArgs;
    }

public :
    void visitprivate(const MatrixLineExp &e)
    {
        /*
        All processes are done in MatrixExp
        */
    }


    void visitprivate(const CellExp &e)
    {
        std::list<MatrixLineExp *>::const_iterator row;
        std::list<Exp *>::const_iterator col;
        int iColMax = 0;

        //check dimmension
        for (row = e.lines_get().begin() ; row != e.lines_get().end() ; ++row )
        {
            if (iColMax == 0)
            {
                iColMax = static_cast<int>((*row)->columns_get().size());
            }

            if (iColMax != static_cast<int>((*row)->columns_get().size()))
            {
                std::wostringstream os;
                os << _W("inconsistent row/column dimensions\n");
                //os << ((Location)(*row)->location_get()).location_getString() << std::endl;
                throw ScilabError(os.str(), 999, (*row)->location_get());
            }
        }

        //alloc result cell
        types::Cell *pC = new types::Cell(static_cast<int>(e.lines_get().size()), iColMax);

        int i = 0;
        int j = 0;

        //insert items in cell
        for (i = 0, row = e.lines_get().begin() ; row != e.lines_get().end() ; row++, i++ )
        {
            for (j = 0, col = (*row)->columns_get().begin() ; col != (*row)->columns_get().end() ; col++, j++)
            {
                (*col)->accept(*this);
                InternalType *pIT = result_get();
                if (pIT->isImplicitList())
                {
                    pIT = pIT->getAs<ImplicitList>()->extractFullMatrix();
                }

                pC->set(i, j, pIT);
                result_set(NULL);
            }
        }

        //return new cell
        result_set(pC);
    }

    /** \name Visit Constant Expressions nodes.
    ** \{ */

    void visitprivate(const StringExp &e)
    {
        if (e.getBigString() == NULL)
        {
            types::String *psz = new types::String(e.value_get().c_str());
            (const_cast<StringExp *>(&e))->setBigString(psz);

        }
        result_set(e.getBigString());
    }


    void visitprivate(const CommentExp &e)
    {
        /*
        Nothing to do
        */
    }


    void visitprivate(const IntExp  &e)
    {
        /*
        Int does not exist, Int8 - 16 - 32 - 64 functions
        */
    }


    void visitprivate(const FloatExp  &e)
    {
        /*
        Float does not exist, float function
        */
    }


    void visitprivate(const DoubleExp  &e)
    {
        if (e.getBigDouble() == NULL)
        {
            Double *pdbl = new Double(e.value_get());
            (const_cast<DoubleExp *>(&e))->setBigDouble(pdbl);

        }
        result_set(e.getBigDouble());
    }


    void visitprivate(const BoolExp  &e)
    {
        if (e.getBigBool() == NULL)
        {
            Bool *pB = new Bool(e.value_get());
            (const_cast<BoolExp *>(&e))->setBigBool(pB);

        }
        result_set(e.getBigBool());
    }


    void visitprivate(const NilExp &e)
    {
        result_set(new types::Void());
    }


    void visitprivate(const SimpleVar &e)
    {
        InternalType *pI = symbol::Context::getInstance()->get(e.name_get());
        result_set(pI);
        if (pI != NULL)
        {
            if (e.is_verbose() && pI->isCallable() == false && ConfigVariable::isPromptShow())
            {
                std::wostringstream ostr;
                ostr << e.name_get().name_get() << L"  = " << L"(" << pI->getRef() << L")" << std::endl;
                ostr << std::endl;
                scilabWriteW(ostr.str().c_str());
                VariableToString(pI, e.name_get().name_get().c_str());
            }
        }
        else
        {
            char pstError[bsiz];
            wchar_t* pwstError;

            char* strErr =  wide_string_to_UTF8(e.name_get().name_get().c_str());

            sprintf(pstError, _("Undefined variable: %s\n"), strErr);
            pwstError = to_wide_string(pstError);
            FREE(strErr);
            std::wstring wstError(pwstError);
            FREE(pwstError);
            throw ScilabError(wstError, 999, e.location_get());
            //Err, SimpleVar doesn't exist in Scilab scopes.
        }
    }


    void visitprivate(const ColonVar &e)
    {
        //int pRank[1] = {2};
        //Double dblCoef(1,2);
        //dblCoef.set(0, 0, 0);
        //dblCoef.set(0, 1, 1);

        //Polynom *pVar = new Polynom(L"$", 1, 1, pRank);
        //SinglePoly *poPoly = pVar->get(0,0);
        //poPoly->setCoef(&dblCoef);

        //ImplicitList *pIL = new ImplicitList();
        //pIL->setStart(new Double(1));
        //pIL->setStep(new Double(1));
        //pIL->setEnd(pVar);

        Colon *pC = new Colon();
        result_set(pC);
        /*
        : = 1:$
        */
    }


    void visitprivate(const DollarVar &e)
    {
        //int pRank[1] = {2};
        //Double dblCoef(1,2);
        //dblCoef.set(0, 0, 0);
        //dblCoef.set(0, 1, 1);

        //Polynom *pVar = new Polynom(L"$", 1, 1, pRank);
        //SinglePoly *poPoly = pVar->get(0,0);
        //poPoly->setCoef(&dblCoef);

        Dollar* pVar = new Dollar();
        result_set(pVar);
    }


    void visitprivate(const ArrayListVar &e)
    {
        /*

        */
    }


    void visitprivate(const FieldExp &e)
    {
        /*
        a.b
        */
        try
        {
            e.head_get()->accept(*this);
        }
        catch (ScilabError error)
        {
            throw error;
        }

        SimpleVar *psvRightMember = dynamic_cast<SimpleVar *>(const_cast<Exp *>(e.tail_get()));
        std::wstring wstField = L"";
        if (psvRightMember != NULL)
        {
            wstField = psvRightMember->name_get().name_get();
        }
        else
        {
            wchar_t szError[bsiz];
            os_swprintf(szError, bsiz, _W("/!\\ Unmanaged FieldExp.\n"));
            throw ScilabError(szError, 999, e.location_get());
        }

        if (result_get() != NULL && result_get()->isStruct())
        {
            InternalType* pTemp = result_get();
            result_set(NULL);
            Struct* psValue = pTemp->getAs<Struct>();
            if (psValue->exists(wstField))
            {
                if (psValue->getSize() != 1)
                {
                    std::vector<std::wstring> wstFields;
                    wstFields.push_back(wstField);
                    std::vector<InternalType*> result;
                    result = psValue->extractFields(wstFields);
                    result_set(result[0]);
                }
                else
                {
                    InternalType* pIT = psValue->get(0)->get(wstField)->clone();
                    result_set(pIT);
                }
            }
            else
            {
                wchar_t szError[bsiz];
                os_swprintf(szError, bsiz, _W("Unknown field : %ls.\n"), wstField.c_str());
                throw ScilabError(szError, 999, e.tail_get()->location_get());
            }
        }
        else if (result_get() != NULL && (result_get()->isMList() || result_get()->isTList()))
        {
            TList* psValue = ((InternalType*)result_get())->getAs<MList>();

            if (psValue->exists(wstField))
            {
                //without overloading function, extract by name
                result_set(psValue->getField(wstField));
            }
            else
            {
                //call %mlisttype_e
                std::wostringstream ostr;
                types::typed_list in;
                types::optional_list opt;
                types::typed_list out;

                //firt argument: index
                String* pS = new String(wstField.c_str());
                pS->IncreaseRef();
                in.push_back(pS);

                //second argument: me ( mlist or tlist )
                psValue->IncreaseRef();
                in.push_back(psValue);

                Callable::ReturnValue Ret = Overload::call(L"%" + psValue->getShortTypeStr() + L"_e", in, 1, out, this);

                if (Ret != Callable::OK)
                {
                    throw ScilabError(L"", 999, e.location_get());
                }

                if (out.size() == 0)
                {
                    result_set(NULL);
                }
                else if (out.size() == 1)
                {
                    out[0]->DecreaseRef();
                    result_set(out[0]);
                }
                else
                {
                    for (int i = 0 ; i < static_cast<int>(out.size()) ; i++)
                    {
                        out[i]->DecreaseRef();
                        result_set(i, out[i]);
                    }
                }

                psValue->DecreaseRef();
                pS->DecreaseRef();
            }
        }
        else if (result_get() != NULL && result_get()->isHandle())
        {
            typed_list in;
            typed_list out;
            optional_list opt;

            String* pField = new String(psvRightMember->name_get().name_get().c_str());
            in.push_back(pField);
            in.push_back(result_get());

            Function* pCall = (Function*)symbol::Context::getInstance()->get(symbol::Symbol(L"%h_e"));
            Callable::ReturnValue ret =  pCall->call(in, opt, 1, out, this);
            if (ret == Callable::OK)
            {
                result_set(out[0]);
            }
        }
        else
        {
            wchar_t szError[bsiz];
            os_swprintf(szError, bsiz, _W("Attempt to reference field of non-structure array.\n"));
            throw ScilabError(szError, 999, e.location_get());
        }
    }

    void visitprivate(const IfExp  &e)
    {
        //Create local exec visitor
        ShortCutVisitor SCTest;
        bool bTestStatus = false;

        //condition
        e.test_get().accept(SCTest);
        e.test_get().accept(*this);

        bTestStatus = bConditionState(result_get());
        result_clear();
        if (bTestStatus == true)
        {
            //condition == true
            if (e.is_breakable())
            {
                const_cast<IfExp*>(&e)->break_reset();
                const_cast<Exp*>(&e.then_get())->breakable_set();
            }

            if (e.is_continuable())
            {
                const_cast<IfExp*>(&e)->continue_reset();
                const_cast<Exp*>(&e.then_get())->continuable_set();
            }

            if (e.is_returnable())
            {
                const_cast<IfExp*>(&e)->return_reset();
                const_cast<Exp*>(&e.then_get())->returnable_set();
            }

            e.then_get().accept(*this);
        }
        else
        {
            //condition == false

            if (e.has_else())
            {
                if (e.is_breakable())
                {
                    const_cast<Exp*>(&e.else_get())->breakable_set();
                }

                if (e.is_continuable())
                {
                    const_cast<IfExp*>(&e)->continue_reset();
                    const_cast<Exp*>(&e.else_get())->continuable_set();
                }

                if (e.is_returnable())
                {
                    const_cast<Exp*>(&e.else_get())->returnable_set();
                }

                e.else_get().accept(*this);
            }
        }

        if (e.is_breakable()
                && ( (&e.else_get())->is_break()
                     || (&e.then_get())->is_break() ))
        {
            const_cast<IfExp*>(&e)->break_set();
            const_cast<Exp*>(&e.else_get())->break_reset();
            const_cast<Exp*>(&e.then_get())->break_reset();
        }

        if (e.is_continuable()
                && ( (&e.else_get())->is_continue()
                     || (&e.then_get())->is_continue() ))
        {
            const_cast<IfExp*>(&e)->continue_set();
            const_cast<Exp*>(&e.else_get())->continue_reset();
            const_cast<Exp*>(&e.then_get())->continue_reset();
        }

        if (e.is_returnable()
                && ( (&e.else_get())->is_return()
                     || (&e.then_get())->is_return() ))
        {
            const_cast<IfExp*>(&e)->return_set();
            const_cast<Exp*>(&e.else_get())->return_reset();
            const_cast<Exp*>(&e.then_get())->return_reset();
        }
    }


    void visitprivate(const TryCatchExp  &e)
    {
        //save current prompt mode
        int oldVal = ConfigVariable::getSilentError();
        //set mode silent for errors
        ConfigVariable::setSilentError(1);
        try
        {
            e.try_get().accept(*this);
            //restore previous prompt mode
            ConfigVariable::setSilentError(oldVal);
        }
        catch (ScilabMessage sm)
        {
            //restore previous prompt mode
            ConfigVariable::setSilentError(oldVal);
            //to lock lasterror
            ConfigVariable::setLastErrorCall();
            e.catch_get().accept(*this);
        }
    }


    void visitprivate(const WhileExp  &e)
    {
        //allow break and continue operations
        const_cast<Exp*>(&e.body_get())->breakable_set();
        const_cast<Exp*>(&e.body_get())->continuable_set();
        //allow return operation
        if (e.is_returnable())
        {
            (&e.body_get())->is_returnable();
        }

        //condition
        e.test_get().accept(*this);
        while (bConditionState(result_get()))
        {
            e.body_get().accept(*this);
            if (e.body_get().is_break())
            {
                const_cast<Exp*>(&(e.body_get()))->break_reset();
                break;
            }

            if (e.body_get().is_return())
            {
                const_cast<WhileExp*>(&e)->return_set();
                const_cast<Exp*>(&(e.body_get()))->return_reset();
                break;
            }

            if (e.body_get().is_continue())
            {
                const_cast<WhileExp*>(&e)->continue_set();
                const_cast<Exp*>(&(e.body_get()))->continue_reset();
                e.test_get().accept(*this);
                continue;
            }

            //clear old result value before evaluate new one
            if (result_get() != NULL)
            {
                if (result_get()->isDeletable())
                {
                    delete result_get();
                }
            }

            e.test_get().accept(*this);
        }

        //clear result of condition or result of body
        result_clear();
    }


    void visitprivate(const ForExp  &e)
    {
        e.vardec_get().accept(*this);
        InternalType* pIT = result_get();
        //allow break and continue operations
        const_cast<Exp*>(&e.body_get())->breakable_set();
        const_cast<Exp*>(&e.body_get())->continuable_set();

        //allow return operation
        if (e.is_returnable())
        {
            (&e.body_get())->is_returnable();
        }

        if (result_get()->isImplicitList())
        {
            ImplicitList* pVar = pIT->getAs<ImplicitList>();

            InternalType *pIL = NULL;
            pIL = pVar->extractValue(0);
            symbol::Symbol varName = e.vardec_get().name_get();

            for (int i = 0 ; i < pVar->getSize() ; i++)
            {
                pIL = pVar->extractValue(i);
                symbol::Context::getInstance()->put(varName, *pIL);

                e.body_get().accept(*this);
                if (e.body_get().is_break())
                {
                    const_cast<Exp*>(&(e.body_get()))->break_reset();
                    break;
                }

                if (e.body_get().is_continue())
                {
                    const_cast<Exp*>(&(e.body_get()))->continue_reset();
                    continue;
                }

                if (e.body_get().is_return())
                {
                    const_cast<ForExp*>(&e)->return_set();
                    break;
                }
            }
        }
        else if (result_get()->isList())
        {
            List* pL = pIT->getAs<List>();
            for (int i = 0 ; i < pL->getSize() ; i++)
            {
                InternalType* pNew = pL->get(i);
                symbol::Context::getInstance()->put(e.vardec_get().name_get(), *pNew);
                e.body_get().accept(*this);
                if (e.body_get().is_break())
                {
                    const_cast<Exp*>(&(e.body_get()))->break_reset();
                    break;
                }

                if (e.body_get().is_continue())
                {
                    const_cast<Exp*>(&(e.body_get()))->continue_reset();
                    continue;
                }

                if (e.body_get().is_return())
                {
                    const_cast<ForExp*>(&e)->return_set();
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
                throw ScilabError(_W("for expression can only manage 1 or 2 dimensions variables\n"), 999, e.vardec_get().location_get());
            }

            for (int i = 0 ; i < pVar->getCols() ; i++)
            {
                GenericType* pNew = pVar->getColumnValues(i);
                symbol::Context::getInstance()->put(e.vardec_get().name_get(), *pNew);
                e.body_get().accept(*this);
                if (e.body_get().is_break())
                {
                    const_cast<Exp*>(&(e.body_get()))->break_reset();
                    break;
                }

                if (e.body_get().is_continue())
                {
                    const_cast<Exp*>(&(e.body_get()))->continue_reset();
                    continue;
                }

                if (e.body_get().is_return())
                {
                    const_cast<ForExp*>(&e)->return_set();
                    break;
                }
            }
        }

        pIT->DecreaseRef();
        if (pIT->isDeletable())
        {
            delete pIT;
        }

        result_set(NULL);
    }


    void visitprivate(const BreakExp &e)
    {
        const_cast<BreakExp*>(&e)->break_set();
    }

    void visitprivate(const ContinueExp &e)
    {
        const_cast<ContinueExp*>(&e)->continue_set();
    }

    void visitprivate(const ReturnExp &e)
    {
        //
        if (e.is_global())
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
                const_cast<ReturnExp*>(&e)->return_set();
            }
        }
        else
        {
            //return(x)

            //in case of CallExp, we can return only one values
            int iSaveExpectedSize = expected_getSize();
            expected_setSize(1);
            e.exp_get().accept(*this);
            expected_setSize(iSaveExpectedSize);

            if (result_getSize() == 1)
            {
                //protect variable
                result_get()->IncreaseRef();
            }
            else
            {
                for (int i = 0 ; i < result_getSize() ; i++)
                {
                    //protect variable
                    result_get(i)->IncreaseRef();
                }
            }

            if (result_getSize() == 1)
            {
                //unprotect variable
                result_get()->DecreaseRef();
            }
            else
            {
                for (int i = 0 ; i < result_getSize() ; i++)
                {
                    //unprotect variable
                    result_get(i)->DecreaseRef();
                }
            }

            const_cast<ReturnExp*>(&e)->return_set();
        }

    }


    void visitprivate(const SelectExp &e)
    {
        // FIXME : exec select ... case ... else ... end
        e.select_get()->accept(*this);
        bool bCase = false;


        InternalType* pIT = result_get();
        result_set(NULL);
        if (pIT)
        {
            //find good case
            cases_t::iterator it;
            for (it = e.cases_get()->begin(); it != e.cases_get()->end() ; it++)
            {
                CaseExp* pCase = *it;
                pCase->test_get()->accept(*this);
                InternalType *pITCase = result_get();
                result_set(NULL);
                if (pITCase)
                {
                    if (pITCase->isContainer()) //WARNING ONLY FOR CELL
                    {
                        //check each item
                    }
                    else if (*pITCase == *pIT)
                    {
                        if (e.is_breakable())
                        {
                            const_cast<SelectExp*>(&e)->break_reset();
                            pCase->body_get()->breakable_set();
                        }

                        if (e.is_continuable())
                        {
                            const_cast<SelectExp*>(&e)->continue_reset();
                            pCase->body_get()->continuable_set();
                        }

                        if (e.is_returnable())
                        {
                            const_cast<SelectExp*>(&e)->return_reset();
                            pCase->body_get()->returnable_set();
                        }

                        //the good one
                        pCase->body_get()->accept(*this);

                        if (e.is_breakable() && pCase->body_get()->is_break())
                        {
                            const_cast<SelectExp*>(&e)->break_set();
                            pCase->body_get()->break_reset();
                        }

                        if (e.is_continuable() && pCase->body_get()->is_continue())
                        {
                            const_cast<SelectExp*>(&e)->continue_set();
                            pCase->body_get()->continue_reset();
                        }

                        if (e.is_returnable() && pCase->body_get()->is_return())
                        {
                            const_cast<SelectExp*>(&e)->return_set();
                            pCase->body_get()->return_reset();
                        }

                        bCase = true;
                        break;
                    }
                }
            }
        }

        if (bCase == false && e.default_case_get() != NULL)
        {
            if (e.is_breakable())
            {
                const_cast<SelectExp*>(&e)->break_reset();
                e.default_case_get()->breakable_set();
            }

            if (e.is_continuable())
            {
                const_cast<SelectExp*>(&e)->continue_reset();
                e.default_case_get()->continuable_set();
            }

            if (e.is_returnable())
            {
                const_cast<SelectExp*>(&e)->return_reset();
                e.default_case_get()->returnable_set();
            }

            //default case
            e.default_case_get()->accept(*this);

            if (e.is_breakable() && e.default_case_get()->is_break())
            {
                const_cast<SelectExp*>(&e)->break_set();
                e.default_case_get()->break_reset();
            }

            if (e.is_continuable() && e.default_case_get()->is_continue())
            {
                const_cast<SelectExp*>(&e)->continue_set();
                e.default_case_get()->continue_reset();
            }

            if (e.is_returnable() && e.default_case_get()->is_return())
            {
                const_cast<SelectExp*>(&e)->return_set();
                e.default_case_get()->return_reset();
            }
        }

        result_clear();
    }


    void visitprivate(const CaseExp &e)
    {
    }


    void visitprivate(const SeqExp  &e)
    {
        //T execMe;
        std::list<Exp *>::const_iterator	itExp;

        for (itExp = e.exps_get().begin (); itExp != e.exps_get().end (); ++itExp)
        {
            if (e.is_breakable())
            {
                (*itExp)->break_reset();
                (*itExp)->breakable_set();
            }

            if (e.is_continuable())
            {
                (*itExp)->continue_reset();
                (*itExp)->continuable_set();
            }

            if (e.is_returnable())
            {
                (*itExp)->returnable_set();
            }

            try
            {
                //reset default values
                result_set(NULL);
                expected_setSize(-1);
                (*itExp)->accept(*this);

                if (result_get() != NULL)
                {
                    bool bImplicitCall = false;
                    if (result_get()->isCallable()) //to manage call without ()
                    {
                        Callable *pCall = ((InternalType*)result_get())->getAs<Callable>();
                        typed_list out;
                        typed_list in;
                        optional_list opt;

                        try
                        {
                            //in this case of calling, we can return only one values
                            int iSaveExpectedSize = expected_getSize();
                            expected_setSize(1);
                            Function::ReturnValue Ret = pCall->call(in, opt, expected_getSize(), out, this);
                            expected_setSize(iSaveExpectedSize);

                            if (Ret == Callable::OK)
                            {
                                if (out.size() == 0)
                                {
                                    result_set(NULL);
                                }
                                else
                                {
                                    result_set(out[0]);
                                }
                                bImplicitCall = true;
                            }
                            else if (Ret == Callable::Error)
                            {
                                if (ConfigVariable::getLastErrorFunction() == L"")
                                {
                                    ConfigVariable::setLastErrorFunction(pCall->getName());
                                    ConfigVariable::setLastErrorLine(e.location_get().first_line);
                                    throw ScilabError();
                                }

                                if (pCall->isMacro() || pCall->isMacroFile())
                                {
                                    wchar_t szError[bsiz];
                                    os_swprintf(szError, bsiz, _W("at line % 5d of function %ls called by :\n"), (*itExp)->location_get().first_line, pCall->getName().c_str());
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
                                os_swprintf(szError, bsiz, _W("at line % 5d of function %ls called by :\n"), sm.GetErrorLocation().first_line, pCall->getName().c_str());
                                throw ScilabMessage(szError + os.str());
                            }
                            else
                            {
                                sm.SetErrorMessage(sm.GetErrorMessage() + os.str());
                                throw sm;
                            }
                        }
                    }

                    SimpleVar* pVar = dynamic_cast<SimpleVar*>(*itExp);
                    //don't output Simplevar and empty result
                    if (result_get() != NULL && (pVar == NULL || bImplicitCall))
                    {
                        //symbol::Context::getInstance()->put(symbol::Symbol(L"ans"), *execMe.result_get());
                        InternalType* pITAns = result_get();
                        symbol::Context::getInstance()->put(*m_pAns, *pITAns);
                        if ((*itExp)->is_verbose() && ConfigVariable::isPromptShow())
                        {
                            //TODO manage multiple returns
                            scilabWriteW(L" ans  =\n\n");
                            VariableToString(pITAns, L"ans");
                        }
                    }
                }

                if ((&e)->is_breakable() && (*itExp)->is_break())
                {
                    const_cast<SeqExp *>(&e)->break_set();
                    break;
                }

                if ((&e)->is_continuable() && (*itExp)->is_continue())
                {
                    const_cast<SeqExp *>(&e)->continue_set();
                    break;
                }

                if ((&e)->is_returnable() && (*itExp)->is_return())
                {
                    const_cast<SeqExp *>(&e)->return_set();
                    (*itExp)->return_reset();
                    break;
                }
            }
            catch (ScilabMessage sm)
            {
                scilabErrorW(sm.GetErrorMessage().c_str());

                CallExp* pCall = dynamic_cast<CallExp*>(*itExp);
                if (pCall != NULL)
                {
                    //to print call expression only of it is a macro
                    pCall->name_get().accept(*this);

                    if (result_get() != NULL && (result_get()->isMacro() || result_get()->isMacroFile()))
                    {
                        wostringstream os;
                        PrintVisitor printMe(os);
                        pCall->accept(printMe);
                        //os << std::endl << std::endl;
                        if (ConfigVariable::getLastErrorFunction() == L"")
                        {
                            ConfigVariable::setLastErrorFunction(((InternalType*)result_get())->getAs<Callable>()->getName());
                        }
                        throw ScilabMessage(os.str(), 0, (*itExp)->location_get());
                    }
                }

                throw ScilabMessage((*itExp)->location_get());
            }
            catch (ScilabError se)
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
                        pCall->name_get().accept(*this);
                        if (result_get() != NULL && (result_get()->isMacro() || result_get()->isMacroFile()))
                        {
                            wostringstream os;
                            PrintVisitor printMe(os);
                            pCall->accept(printMe);
                            //os << std::endl << std::endl;
                            ConfigVariable::setLastErrorFunction(((InternalType*)result_get())->getAs<Callable>()->getName());
                            scilabErrorW(se.GetErrorMessage().c_str());
                            throw ScilabMessage(os.str(), 999, (*itExp)->location_get());
                        }
                    }
                    catch (ScilabError se2)
                    {
                        //just to catch exception, do nothing
                    }
                }

                scilabErrorW(se.GetErrorMessage().c_str());
                scilabErrorW(L"\n");
                throw ScilabMessage((*itExp)->location_get());
            }
            result_set(NULL);
        }
    }


    void visitprivate(const ArrayListExp  &e)
    {
        std::list<Exp *>::const_iterator it;
        int i = 0;

        std::list<InternalType*> lstIT;
        for (it = e.exps_get().begin() ; it != e.exps_get().end() ; it++)
        {
            (*it)->accept(*this);
            lstIT.push_back(result_get()->clone());
        }

        std::list<InternalType*>::iterator itIT = lstIT.begin();
        for (; itIT != lstIT.end(); itIT++)
        {
            result_set(i++, *itIT);
        }
    }


    void visitprivate(const AssignListExp  &e)
    {

    }
    /** \} */

    /** \name Visit Single Operation nodes.
    ** \{ */

    void visitprivate(const NotExp &e)
    {
        /*
        @ or ~ !
        */
        e.exp_get().accept(*this);

        if (result_get()->isDouble())
        {
            InternalType* pVar  = result_get();
            Double *pdbl        = pVar->getAs<Double>();
            Bool *pReturn       = new Bool(pdbl->getDims(), pdbl->getDimsArray());
            double *pR		    = pdbl->getReal();
            int *piB            = pReturn->get();
            for (int i = 0 ; i < pdbl->getSize() ; i++)
            {
                piB[i] = pR[i] == 0 ? 1 : 0;
            }

            if (result_get()->isDeletable())
            {
                delete result_get();
            }

            result_set(pReturn);
        }
        else if (result_get()->isBool())
        {
            InternalType* pIT   = result_get();
            Bool *pb            = pIT->getAs<types::Bool>();
            Bool *pReturn       = new Bool(pb->getDims(), pb->getDimsArray());
            int *piR            = pb->get();
            int *piB            = pReturn->get();

            for (int i = 0 ; i < pb->getSize() ; i++)
            {
                piB[i] = piR[i] == 1 ? 0 : 1;
            }

            if (result_get()->isDeletable())
            {
                delete result_get();
            }

            result_set(pReturn);
        }
        else if (result_get()->isSparseBool())
        {
            InternalType* pIT   = result_get();
            SparseBool *pb            = pIT->getAs<types::SparseBool>();
            SparseBool *pReturn       = new SparseBool(pb->getRows(), pb->getCols());

            for (int iRows = 0 ; iRows < pb->getRows() ; iRows++)
            {
                for (int iCols = 0 ; iCols < pb->getCols() ; iCols++)
                {
                    pReturn->set(iRows, iCols, !pb->get(iRows, iCols));
                }
            }


            result_set(pReturn);
        }
        else if (result_get()->isInt())
        {
            InternalType* pReturn = NULL;
            InternalType* pIT = result_get();
            switch (result_get()->getType())
            {
                case InternalType::RealInt8 :
                    pReturn = notInt< Int8, char >(pIT->getAs<Int8>());
                    break;
                case InternalType::RealUInt8 :
                    pReturn = notInt<UInt8, unsigned char>(pIT->getAs<UInt8>());
                    break;
                case InternalType::RealInt16 :
                    pReturn = notInt< Int16, short >(pIT->getAs<Int16>());
                    break;
                case InternalType::RealUInt16 :
                    pReturn = notInt<UInt16, unsigned short>(pIT->getAs<UInt16>());
                    break;
                case InternalType::RealInt32 :
                    pReturn = notInt<Int32, int>(pIT->getAs<Int32>());
                    break;
                case InternalType::RealUInt32 :
                    pReturn = notInt<UInt32, unsigned int>(pIT->getAs<UInt32>());
                    break;
                case InternalType::RealInt64 :
                    pReturn = notInt<Int64, long long>(pIT->getAs<Int64>());
                    break;
                case InternalType::RealUInt64 :
                    pReturn = notInt<UInt64, unsigned long long>(pIT->getAs<UInt64>());
                    break;
                default :
                    break;
            }

            if (result_get()->isDeletable())
            {
                delete result_get();
            }

            result_set(pReturn);
        }
    }

    template <class intT, typename Y>
    InternalType* notInt(intT* _pInt)
    {
        intT* pOut = new intT(_pInt->getRows(), _pInt->getCols());
        Y* pDataIn = _pInt->get();
        Y* pDataOut = pOut->get();
        for (int i = 0 ; i < _pInt->getSize() ; i++)
        {
            pDataOut[i] = ~pDataIn[i];
        }

        return pOut;
    }

    void visitprivate(const TransposeExp &e)
    {
        /*
        '
        */
        e.exp_get().accept(*this);

        bool bConjug = e.conjugate_get() == TransposeExp::_Conjugate_;

        if (result_get()->isImplicitList())
        {
            InternalType *pIT = ((InternalType*)result_get())->getAs<ImplicitList>()->extractFullMatrix();
            if (result_get()->isDeletable())
            {
                delete result_get();
            }

            result_set(pIT);
        }

        if (result_get()->isDouble())
        {
            InternalType* pVar  = result_get();
            Double *pdbl		= pVar->getAs<Double>();
            Double *pReturn	    = NULL;

            if (pdbl->isComplex())
            {
                pReturn         = new Double(pdbl->getCols(), pdbl->getRows(), true);
                double *pInR    = pdbl->getReal();
                double *pInI    = pdbl->getImg();
                double *pOutR   = pReturn->getReal();
                double *pOutI   = pReturn->getImg();

                vTransposeComplexMatrix(pInR, pInI, pdbl->getRows(), pdbl->getCols(), pOutR, pOutI, bConjug);
            }
            else
            {
                pReturn         = new Double(pdbl->getCols(), pdbl->getRows(), false);
                double *pInR    = pdbl->getReal();
                double *pOutR   = pReturn->getReal();

                vTransposeRealMatrix(pInR, pdbl->getRows(), pdbl->getCols(), pOutR);
            }

            if (result_get()->isDeletable())
            {
                delete result_get();
            }

            result_set(pReturn);
        }
        else if (result_get()->isPoly())
        {
            InternalType *pIT   = result_get();
            Polynom *pMP        = pIT->getAs<types::Polynom>();
            Polynom *pReturn    = NULL;

            //prepare rank array
            int* piRank = new int[pMP->getSize()];

            for (int i = 0 ; i < pMP->getRows() ; i++)
            {
                for (int j = 0 ; j < pMP->getCols() ; j++)
                {
                    piRank[i * pMP->getCols() + j] = pMP->get(i, j)->getRank();
                }
            }

            pReturn = new Polynom(pMP->getVariableName(), pMP->getCols(), pMP->getRows(), piRank);
            pReturn->setComplex(pMP->isComplex());

            if (pMP->isComplex() && bConjug)
            {
                for (int i = 0 ; i < pMP->getRows() ; i++)
                {
                    for (int j = 0 ; j < pMP->getCols() ; j++)
                    {
                        pReturn->setCoef(j, i, pMP->get(i, j)->getCoef());
                        double *pdblImg = pReturn->get(j, i)->getCoefImg();
                        for (int k = 0 ; k < pReturn->get(j, i)->getRank() ; k++)
                        {
                            pdblImg[k] *= -1;
                        }
                    }
                }
            }
            else
            {
                for (int i = 0 ; i < pMP->getRows() ; i++)
                {
                    for (int j = 0 ; j < pMP->getCols() ; j++)
                    {
                        pReturn->setCoef(j, i, pMP->get(i, j)->getCoef());
                    }
                }
            }

            if (result_get()->isDeletable())
            {
                delete result_get();
            }

            result_set(pReturn);
        }
        else if (result_get()->isString())
        {
            InternalType* pVar  = result_get();
            types::String *pS          = pVar->getAs<types::String>();
            types::String* pReturn     = new types::String(pS->getCols(), pS->getRows());

            for (int i = 0 ; i < pS->getRows() ; i++)
            {
                for (int j = 0 ; j < pS->getCols() ; j++)
                {
                    pReturn->set(j, i, pS->get(i, j));
                }
            }

            if (result_get()->isDeletable())
            {
                delete result_get();
            }

            result_set(pReturn);
        }
        else if (result_get()->isBool())
        {
            InternalType* pVar  = result_get();
            types::Bool *pB = pVar->getAs<types::Bool>();
            types::Bool* pReturn = new types::Bool(pB->getCols(), pB->getRows());

            for (int i = 0 ; i < pB->getRows() ; i++)
            {
                for (int j = 0 ; j < pB->getCols() ; j++)
                {
                    pReturn->set(j, i, pB->get(i, j));
                }
            }

            if (result_get()->isDeletable())
            {
                delete result_get();
            }

            result_set(pReturn);
        }
        else if (result_get()->isSparse())
        {
            types::InternalType* pIT = result_get();
            result_set(pIT->getAs<types::Sparse>()->newTransposed());
        }
        else if (result_get()->isSparseBool())
        {
            types::InternalType* pIT = result_get();
            result_set(pIT->getAs<types::SparseBool>()->newTransposed());
        }
        else if (result_get()->isInt())
        {
            InternalType* pVar      = result_get();
            InternalType* pReturn   = NULL;

            switch (pVar->getType())
            {
                case types::InternalType::RealInt8 :
                {
                    types::Int8* pIntIn  = pVar->getAs<types::Int8>();
                    types::Int8* pIntOut = new types::Int8(pIntIn->getCols(), pIntIn->getRows());
                    char* pIn  = pIntIn->get();
                    char* pOut = pIntOut->get();
                    vTransposeIntMatrix<char>(pIn, pIntIn->getRows(), pIntIn->getCols(), pOut);
                    pReturn = pIntOut;
                    break;
                }
                case types::InternalType::RealUInt8 :
                {
                    types::UInt8* pIntIn  = pVar->getAs<types::UInt8>();
                    types::UInt8* pIntOut = new types::UInt8(pIntIn->getCols(), pIntIn->getRows());
                    unsigned char* pIn  = pIntIn->get();
                    unsigned char* pOut = pIntOut->get();
                    vTransposeIntMatrix<unsigned char>(pIn, pIntIn->getRows(), pIntIn->getCols(), pOut);
                    pReturn = pIntOut;
                    break;
                }
                case types::InternalType::RealInt16 :
                {
                    types::Int16* pIntIn  = pVar->getAs<types::Int16>();
                    types::Int16* pIntOut = new types::Int16(pIntIn->getCols(), pIntIn->getRows());
                    short* pIn  = pIntIn->get();
                    short* pOut = pIntOut->get();
                    vTransposeIntMatrix<short>(pIn, pIntIn->getRows(), pIntIn->getCols(), pOut);
                    pReturn = pIntOut;
                    break;
                }
                case types::InternalType::RealUInt16 :
                {
                    types::UInt16* pIntIn  = pVar->getAs<types::UInt16>();
                    types::UInt16* pIntOut = new types::UInt16(pIntIn->getCols(), pIntIn->getRows());
                    unsigned short* pIn  = pIntIn->get();
                    unsigned short* pOut = pIntOut->get();
                    vTransposeIntMatrix<unsigned short>(pIn, pIntIn->getRows(), pIntIn->getCols(), pOut);
                    pReturn = pIntOut;
                    break;
                }
                case types::InternalType::RealInt32 :
                {
                    types::Int32* pIntIn  = pVar->getAs<types::Int32>();
                    types::Int32* pIntOut = new types::Int32(pIntIn->getCols(), pIntIn->getRows());
                    int* pIn  = pIntIn->get();
                    int* pOut = pIntOut->get();
                    vTransposeIntMatrix<int>(pIn, pIntIn->getRows(), pIntIn->getCols(), pOut);
                    pReturn = pIntOut;
                    break;
                }
                case types::InternalType::RealUInt32 :
                {
                    types::UInt32* pIntIn  = pVar->getAs<types::UInt32>();
                    types::UInt32* pIntOut = new types::UInt32(pIntIn->getCols(), pIntIn->getRows());
                    unsigned int* pIn  = pIntIn->get();
                    unsigned int* pOut = pIntOut->get();
                    vTransposeIntMatrix<unsigned int>(pIn, pIntIn->getRows(), pIntIn->getCols(), pOut);
                    pReturn = pIntOut;
                    break;
                }
                case types::InternalType::RealInt64 :
                {
                    types::Int64* pIntIn  = pVar->getAs<types::Int64>();
                    types::Int64* pIntOut = new types::Int64(pIntIn->getCols(), pIntIn->getRows());
                    long long* pIn  = pIntIn->get();
                    long long* pOut = pIntOut->get();
                    vTransposeIntMatrix<long long>(pIn, pIntIn->getRows(), pIntIn->getCols(), pOut);
                    pReturn = pIntOut;
                    break;
                }
                case types::InternalType::RealUInt64 :
                {
                    types::UInt64* pIntIn  = pVar->getAs<types::UInt64>();
                    types::UInt64* pIntOut = new types::UInt64(pIntIn->getCols(), pIntIn->getRows());
                    unsigned long long* pIn  = pIntIn->get();
                    unsigned long long* pOut = pIntOut->get();
                    vTransposeIntMatrix<unsigned long long>(pIn, pIntIn->getRows(), pIntIn->getCols(), pOut);
                    pReturn = pIntOut;
                    break;
                }
                default:
                    break;
            }

            if (pVar->isDeletable())
            {
                delete pVar;
            }

            result_set(pReturn);
        }
    }
    /** \} */

    /** \name Visit Declaration nodes.
    ** \{ */
    /** \brief Visit Var declarations. */

    void visitprivate(const VarDec  &e)
    {
        try
        {
            /*getting what to assign*/
            e.init_get().accept(*this);
            result_get()->IncreaseRef();
        }
        catch (ScilabError error)
        {
            throw error;
        }
    }


    void visitprivate(const FunctionDec  &e)
    {
        /*
        function foo
        endfunction
        */

        // funcprot(0) : do nothing
        // funcprot(1) && warning(on) : warning
        // funcprot(2) : error
        if (ConfigVariable::getFuncprot() == 1 && ConfigVariable::getWarningMode())
        {
            types::InternalType* pITFunc = symbol::Context::getInstance()->get(symbol::Symbol(e.name_get().name_get()));

            if (pITFunc && pITFunc->isCallable())
            {
                wchar_t pwstFuncName[1024];
                os_swprintf(pwstFuncName, 1024, L"%-24ls", e.name_get().name_get().c_str());
                char* pstFuncName = wide_string_to_UTF8(pwstFuncName);

                sciprint(_("Warning : redefining function: %s. Use funcprot(0) to avoid this message"), pstFuncName);
                sciprint("\n");
                FREE(pstFuncName);
            }
        }
        else if (ConfigVariable::getFuncprot() == 2)
        {
            types::InternalType* pITFunc = symbol::Context::getInstance()->get(symbol::Symbol(e.name_get().name_get()));

            if (pITFunc && pITFunc->isCallable())
            {
                char pstError[1024];
                char* pstFuncName = wide_string_to_UTF8(e.name_get().name_get().c_str());
                sprintf(pstError, _("It is not possible to redefine the %s primitive this way (see clearfun).\n"), pstFuncName);
                wchar_t* pwstError = to_wide_string(pstError);
                std::wstring wstError(pwstError);
                FREE(pstFuncName);
                FREE(pwstError);
                throw ScilabError(wstError, 999, e.location_get());
            }
        }

        std::list<ast::Var *>::const_iterator	i;

        //get input parameters list
        std::list<symbol::Symbol> *pVarList = new std::list<symbol::Symbol>();
        const ArrayListVar *pListVar = &e.args_get();
        for (i = pListVar->vars_get().begin() ; i != pListVar->vars_get().end() ; i++)
        {
            pVarList->push_back(static_cast<SimpleVar*>(*i)->name_get());
        }

        //get output parameters list
        std::list<symbol::Symbol> *pRetList = new std::list<symbol::Symbol>();
        const ArrayListVar *pListRet = &e.returns_get();
        for (i = pListRet->vars_get().begin() ; i != pListRet->vars_get().end() ; i++)
        {
            pRetList->push_back(static_cast<SimpleVar*>(*i)->name_get());
        }

        //            Location* newloc = const_cast<Location*>(&location_get())->clone();
        Exp* exp = const_cast<Exp*>(&e.body_get())->clone();

        //MuteVisitor mute;
        //exp->accept(mute);

        //types::Macro macro(VarList, RetList, (SeqExp&)e.body_get());
        types::Macro *pMacro = new types::Macro(e.name_get().name_get(), *pVarList, *pRetList,
                                                static_cast<SeqExp&>(*exp), L"script");
        symbol::Context::getInstance()->AddMacro(pMacro);
    }
    /** \} */

    /** \name Visit Type dedicated Expressions related node.
    ** \{ */

    void visitprivate(const ListExp &e)
    {
        try
        {
            e.start_get().accept(*this);
            GenericType* pITStart = static_cast<GenericType*>(result_get());
            if (pITStart->getRows() != 1 || pITStart->getCols() != 1)
            {
                throw 1;
            }
            InternalType* piStart = result_get();

            e.step_get().accept(*this);
            GenericType* pITStep = static_cast<GenericType*>(result_get());
            if (pITStep->getRows() != 1 || pITStep->getCols() != 1)
            {
                throw 2;
            }
            InternalType* piStep = result_get();

            e.end_get().accept(*this);
            GenericType* pITEnd = static_cast<GenericType*>(result_get());
            if (pITEnd->getRows() != 1 || pITEnd->getCols() != 1)
            {
                throw 3;
            }
            InternalType* piEnd = result_get();

            //check compatibility

            if (piStart->isInt())
            {
                //if Step or End are Int too, they must have the same precision
                if (piStep->isInt())
                {
                    if (piStep->getType() != piStart->getType())
                    {
                        throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.step_get().location_get());
                    }
                }
                else if (piStep->isPoly())
                {
                    throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.step_get().location_get());
                }


                if (piEnd->isInt())
                {
                    if (piEnd->getType() != piStart->getType())
                    {
                        throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.end_get().location_get());
                    }
                }
                else if (piEnd->isPoly())
                {
                    throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.end_get().location_get());
                }
            }
            else if (piStart->isPoly())
            {
                if (piStep->isInt())
                {
                    throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.step_get().location_get());
                }

                if (piEnd->isInt())
                {
                    throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.end_get().location_get());
                }
            }
            else if (piStep->isInt())
            {
                //if End is Int too, they must have the same precision
                if (piEnd->isInt())
                {
                    if (piEnd->getType() != piStep->getType())
                    {
                        throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.end_get().location_get());
                    }
                }
            }
            else if (piStep->isPoly())
            {
                if (piEnd->isInt())
                {
                    throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.step_get().location_get());
                }
            }

            if (piStart->isDouble() && piStart->getAs<Double>()->isComplex())
            {
                throw 1;
            }

            if (piStep->isDouble() && piStep->getAs<Double>()->isComplex())
            {
                throw 2;
            }

            if (piEnd->isDouble() && piEnd->getAs<Double>()->isComplex())
            {
                throw 3;
            }

            ImplicitList *pIL = new ImplicitList(piStart, piStep, piEnd);

            result_set(pIL);

            if (piStart && piStart->isDeletable())
            {
                delete piStart;
            }

            if (piStep && piStep->isDeletable())
            {
                delete piStep;
            }

            if (piEnd && piEnd->isDeletable())
            {
                delete piEnd;
            }
        }
        catch (int iPos)
        {
            wchar_t szError[bsiz];
            os_swprintf(szError, bsiz, _W("%ls: Wrong type for argument %d: Real scalar expected.\n"), L"':'", iPos);
            throw ScilabError(szError, 999, e.location_get());
        }
        catch (ScilabError error)
        {
            //TODO YaSp : Overloading
            throw error;
        }
    }

    void VariableToString(types::InternalType* pIT, const wchar_t* wcsVarName)
    {
        std::wostringstream ostr;

        if (pIT->isMList() || pIT->isTList() || pIT->hasToString() == false)
        {
            //call overload %type_p
            types::typed_list in;
            types::typed_list out;

            pIT->IncreaseRef();
            in.push_back(pIT);

            try
            {
                Overload::call(L"%" + pIT->getAs<TList>()->getShortTypeStr() + L"_p", in, 1, out, this);
            }
            catch (ScilabError /*&e*/)
            {
                ostr << wcsVarName;
                pIT->toString(ostr);
                scilabWriteW(ostr.str().c_str());
            }

            pIT->DecreaseRef();
        }
        else
        {
            //to manage lines information
            int iLines = ConfigVariable::getConsoleLines();

            bool bFinish = false;
            do
            {
                //block by block
                bFinish = pIT->toString(ostr);
                scilabWriteW(ostr.str().c_str());
                if (bFinish == false && iLines != 0)
                {
                    //show message on prompt
                    bFinish = linesmore() == 1;
                }
                ostr.str(L"");
            }
            while (bFinish == false);

            pIT->clearPrintState();
        }
    }

    /** \} */

#include "run_AssignExp.hxx"
    // This will define
    // void visitprivate(const AssignExp  &e)

#include "run_OpExp.hxx"
    // This will define
    // void visitprivate(const OpExp &e)
    // void visitprivate(const LogicalOpExp &e)

#include "run_MatrixExp.hxx"
    // This will define
    // void visitprivate(const MatrixExp &e)

#include "run_CallExp.hxx"
    // This will define
    // void visitprivate(const CallExp &e)
};
}

#endif // !AST_RUNVISITOR_HXX
