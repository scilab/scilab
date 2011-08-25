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

#include "alltypes.hxx"

// Needed by visitprivate(const OpExp &)
// Needed by visitprivate(const LogicalOpExp &)
#include "types_multiplication.hxx"
#include "types_addition.hxx"
#include "types_substraction.hxx"
#include "types_divide.hxx"
#include "types_power.hxx"
#include "types_comparison_equal.hxx"
#include "types_comparison_non_equal.hxx"
#include "configvariable.hxx"
#include "overload.hxx"
#include "scilabexception.hxx"

extern "C" {
#include "doublecomplex.h"
#include "matrix_transpose.h"
#include "os_swprintf.h"
}

#include "timer.hxx"
#include "localization.h"

#include "yaspio.hxx"
#include "context.hxx"

#include "all.hxx"
#include "types.hxx"


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
        }

        ~RunVisitor()
        {
            result_clear();
        }

        void result_clear()
        {
            if(is_single_result())
            {
                if(_result != NULL && _result->isDeletable() == true)
                {
                    //					std::cout << "before single delete : " << _result << std::endl;
                    delete _result;
                    _result = NULL;
                    //					std::cout << "after single delete" << std::endl;
                }
            }
            else
            {
                for(unsigned int i = 0 ; i < _resultVect.size() ; i++)
                {
                    if(_resultVect[i] != NULL && _resultVect[i]->isDeletable() == true)
                    {
                        delete _resultVect[i];
                        _resultVect[i] = NULL;
                    }
                }
            }
        }

    public:
        int expected_getSize(void)
        {
            return _excepted_result;
        }

        int result_getSize(void)
        {
            if(is_single_result())
            {
                if(_result == NULL)
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

        void expected_size_set(int _iSize)
        {
            _excepted_result = _iSize;
        }

        types::InternalType* result_get(void)
        {
            if(is_single_result())
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
            if(is_single_result() && _iPos == 0)
            {
                return _result;
            }

            if(_iPos >= static_cast<int>(_resultVect.size()))
            {
                return NULL;
            }
            return _resultVect[_iPos];
        }

        vector<types::InternalType*>* result_list_get()
        {
            if(result_getSize() == 1)
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
            if(_iPos <  static_cast<int>(_resultVect.size()))
            {
                if(_resultVect[_iPos] != NULL && _resultVect[_iPos]->isDeletable())
                {
                    delete _resultVect[_iPos];
                }
            }

            if(_iPos >=  static_cast<int>(_resultVect.size()))
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
    };

    template <class T>
    class RunVisitorT : public RunVisitor
    {
    public :

        typed_list* GetArgumentList(std::list<ast::Exp *>const& _plstArg)
        {
            typed_list* pArgs = new typed_list();
            std::list<ast::Exp *>::const_iterator it;
            T *execMeArg  = new T();
            for(it = _plstArg.begin() ; it != _plstArg.end() ; it++)
            {
                (*it)->accept(*execMeArg);
                pArgs->push_back(execMeArg->result_get());
            }
            //to be sure, delete operation does not delete result
            execMeArg->result_set(NULL);
            delete execMeArg;
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
                if(iColMax == 0)
                {
                    iColMax = static_cast<int>((*row)->columns_get().size());
                }

                if(iColMax != static_cast<int>((*row)->columns_get().size()))
                {
                    std::wostringstream os;
                    os << L"inconsistent row/column dimensions\n";
                    //os << ((Location)(*row)->location_get()).location_getString() << std::endl;
                    throw ScilabError(os.str(), 999, (*row)->location_get());
                }
            }

            //alloc result cell
            Cell *pC = new Cell(static_cast<int>(e.lines_get().size()), iColMax);

            int i = 0;
            int j = 0;

            //insert items in cell
            for (i = 0, row = e.lines_get().begin() ; row != e.lines_get().end() ; row++, i++ )
            {
                for (j = 0, col = (*row)->columns_get().begin() ; col != (*row)->columns_get().end() ; col++, j++)
                {
                    T execMe;
                    (*col)->accept(execMe);
                    pC->set(i,j, execMe.result_get());
                }
            }

            //return new cell
            result_set(pC);
        }

        /** \name Visit Constant Expressions nodes.
        ** \{ */

        void visitprivate(const StringExp &e)
        {
            String *psz = new String(e.value_get().c_str());
            result_set(psz);
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
            if(e.getBigDouble() == NULL)
            {
                Double *pdbl = new Double(e.value_get());
                pdbl->IncreaseRef();
                (const_cast<DoubleExp *>(&e))->setBigDouble(pdbl);

            }
            result_set(e.getBigDouble());
        }


        void visitprivate(const BoolExp  &e)
        {
            Bool *pb = new Bool(e.value_get());
            result_set(pb);
        }


        void visitprivate(const NilExp &e)
        {
            /*
            FIXME :
            */
        }


        void visitprivate(const SimpleVar &e)
        {
            InternalType *pI = symbol::Context::getInstance()->get(e.name_get());
            if(pI != NULL)
            {
                result_set(pI);
                if(pI != NULL && pI->getAsCallable() == false && e.is_verbose())
                {
                    std::wostringstream ostr;
                    ostr << e.name_get().name_get() << L"  = " << L"(" << pI->getRef() << L")"<< std::endl;
                    ostr << std::endl;
                    ostr << pI->toString(ConfigVariable::getFormat(), ConfigVariable::getConsoleWidth());
                    YaspWriteW(ostr.str().c_str());
                }
            }
            else
            {
                wchar_t szError[bsiz];
                os_swprintf(szError, bsiz, _W("Undefined variable: %ls\n"), e.name_get().name_get().c_str());
                throw ScilabError(szError, 999, e.location_get());
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
            T execHead;
            try
            {
                e.head_get()->accept(execHead);
            }
            catch(ScilabError error)
            {
                throw error;
            }

            if(execHead.result_get() != NULL && execHead.result_get()->isStruct())
            {
                SimpleVar *psvRightMember = dynamic_cast<SimpleVar *>(const_cast<Exp *>(e.tail_get()));
                if(psvRightMember != NULL)
                {
                    InternalType* pTemp = execHead.result_get();
                    Struct* psValue = pTemp->getAs<Struct>();
                    if(psValue->exists(psvRightMember->name_get().name_get()))
                    {
                        if(psValue->getSize() != 1)
                        {
                            std::list<std::wstring> wstFields;
                            wstFields.push_back(psvRightMember->name_get().name_get());

                            std::vector<InternalType*> result;
                            result = psValue->extractFields(wstFields);

                            result_set(result[0]);
                        }
                        else
                        {
                            InternalType* pIT = psValue->get(0)->get(psvRightMember->name_get().name_get());
                            result_set(pIT);
                        }
                    }
                    else
                    {
                        wchar_t szError[bsiz];
                        os_swprintf(szError, bsiz, _W("Unknown field : %ls.\n"), psvRightMember->name_get().name_get().c_str());
                        throw ScilabError(szError, 999, psvRightMember->location_get());
                    }
                }
                else
                {
                    wchar_t szError[bsiz];
                    os_swprintf(szError, bsiz, _W("/!\\ Unmanaged FieldExp.\n"));
                    throw ScilabError(szError, 999, e.location_get());
                }
            }
            else if(execHead.result_get() != NULL && execHead.result_get()->isTList())
            {
                SimpleVar *psvRightMember = dynamic_cast<SimpleVar *>(const_cast<Exp *>(e.tail_get()));
                if(psvRightMember != NULL)
                {
                    TList* psValue = execHead.result_get()->getAsTList();
                    if(psValue->exists(psvRightMember->name_get().name_get()))
                    {
                        InternalType* pIT = psValue->get(psvRightMember->name_get().name_get());
                        result_set(pIT);
                    }
                    else
                    {
                        wchar_t szError[bsiz];
                        os_swprintf(szError, bsiz, _W("Unknown field : %ls.\n"), psvRightMember->name_get().name_get().c_str());
                        throw ScilabError(szError, 999, psvRightMember->location_get());
                    }
                }
                else
                {
                    wchar_t szError[bsiz];
                    os_swprintf(szError, bsiz, _W("/!\\ Unmanaged FieldExp.\n"));
                    throw ScilabError(szError, 999, e.location_get());
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
            T execMeTest;
            ShortCutVisitor SCTest;
            T execMeAction;
            bool bTestStatus							= false;

            //condition
            e.test_get().accept(SCTest);
            e.test_get().accept(execMeTest);

            bTestStatus = bConditionState(execMeTest.result_get());
            if(bTestStatus == true)
            {//condition == true
                if(e.is_breakable())
                {
                    const_cast<Exp*>(&e.then_get())->breakable_set();
                }

                if(e.is_continuable())
                {
                    const_cast<IfExp*>(&e)->continue_reset();
                    const_cast<Exp*>(&e.then_get())->continuable_set();
                }

                if(e.is_returnable())
                {
                    const_cast<Exp*>(&e.then_get())->returnable_set();
                }

                e.then_get().accept(execMeAction);
            }
            else
            {//condition == false

                if(e.has_else())
                {
                    if(e.is_breakable())
                    {
                        const_cast<Exp*>(&e.else_get())->breakable_set();
                    }

                    if(e.is_continuable())
                    {
                        const_cast<IfExp*>(&e)->continue_reset();
                        const_cast<Exp*>(&e.else_get())->continuable_set();
                    }

                    if(e.is_returnable())
                    {
                        const_cast<Exp*>(&e.else_get())->returnable_set();
                    }

                    e.else_get().accept(execMeAction);
                }
            }

            if(e.is_breakable()
                && ( (&e.else_get())->is_break()
                || (&e.then_get())->is_break() ))
            {
                const_cast<IfExp*>(&e)->break_set();
                const_cast<Exp*>(&e.else_get())->break_reset();
                const_cast<Exp*>(&e.then_get())->break_reset();
            }

            if(e.is_continuable()
                && ( (&e.else_get())->is_continue()
                || (&e.then_get())->is_continue() ))
            {
                const_cast<IfExp*>(&e)->continue_set();
                const_cast<Exp*>(&e.else_get())->continue_reset();
                const_cast<Exp*>(&e.then_get())->continue_reset();
            }

            if(e.is_returnable()
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
                T execMe;
                e.try_get().accept(execMe);
                //restore previous prompt mode
                ConfigVariable::setSilentError(oldVal);
            }
            catch(ScilabMessage sm)
            {
                T execMe;
                //restore previous prompt mode
                ConfigVariable::setSilentError(oldVal);
                //to lock lasterror
                ConfigVariable::setLastErrorCall();
                e.catch_get().accept(execMe);
            }
        }


        void visitprivate(const WhileExp  &e)
        {
            T execMeTest;
            T execMeAction;

            //allow break and continue operations
            const_cast<Exp*>(&e.body_get())->breakable_set();
            const_cast<Exp*>(&e.body_get())->continuable_set();
            //allow return operation
            if(e.is_returnable())
            {
                (&e.body_get())->is_returnable();
            }

            //condition
            e.test_get().accept(execMeTest);
            while(bConditionState(execMeTest.result_get()))
            {
                e.body_get().accept(execMeAction);
                if(e.body_get().is_break())
                {
                    break;
                }

                if(e.body_get().is_return())
                {
                    const_cast<WhileExp*>(&e)->return_set();
                    break;
                }

                if(e.body_get().is_continue())
                {
                    const_cast<WhileExp*>(&e)->continue_set();
                    const_cast<Exp*>(&(e.body_get()))->continue_reset();
                    e.test_get().accept(execMeTest);
                    continue;
                }

                //clear old result value before evaluate new one
                if(execMeTest.result_get() != NULL)
                {
                    if(execMeTest.result_get()->isDeletable())
                    {
                        delete execMeTest.result_get();
                    }
                }

                e.test_get().accept(execMeTest);
            }
        }


        void visitprivate(const ForExp  &e)
        {
            T execVar;
            e.vardec_get().accept(execVar);

            //allow break and continue operations
            const_cast<Exp*>(&e.body_get())->breakable_set();
            const_cast<Exp*>(&e.body_get())->continuable_set();
            //allow return operation
            if(e.is_returnable())
            {
                (&e.body_get())->is_returnable();
            }

            if(execVar.result_get()->isImplicitList())
            {
                T execBody;
                ImplicitList* pVar = execVar.result_get()->getAsImplicitList();
                //			std::cout << "ImplicitList references : " << pVar->getRef() << std::endl;

                InternalType *pIT = NULL;
                pIT = pVar->extractValue(0);
                pIT->IncreaseRef();
                symbol::Symbol varName = e.vardec_get().name_get();
                //symbol::Context::getInstance()->put(varName, *pIT);

                Double *pDouble = pIT->getAs<Double>();
                for(int i = 0 ; i < pVar->getSize() ; i++)
                {
                    bool bNew = false;
                    if(pIT->isRef(1))
                    {
                        //decrease reference to pIT
                        //this variable does not push in context
                        //so increase and decrease ref are not "automaticly" manage
                        //std::wcout << varName.name_get() << " : " << pIT->getRef() << std::endl;
                        //pIT->DecreaseRef();
                        pIT = pIT->clone();
                        pDouble = pIT->getAs<Double>();
                        bNew = true;
                    }

                    if(pIT->isDouble())
                    {
                        pDouble->get()[0] = pVar->extractValueInDouble(i);
                    }
                    else if(pIT->isInt())
                    {
                        switch(pIT->getType())
                        {
                        case InternalType::RealInt8 :
                        {
                            Int8* pI = pIT->getAs<Int8>();
                            pI->set(0, (char)pVar->extractValueInInteger(i));
                            break;
                        }
                        case InternalType::RealUInt8 :
                        {
                            UInt8* pI = pIT->getAs<UInt8>();
                            pI->set(0, (unsigned char)pVar->extractValueInInteger(i));
                            break;
                        }
                        case InternalType::RealInt16 :
                        {
                            Int16* pI = pIT->getAs<Int16>();
                            pI->set(0, (short)pVar->extractValueInInteger(i));
                            break;
                        }
                        case InternalType::RealUInt16 :
                        {
                            UInt16* pI = pIT->getAs<UInt16>();
                            pI->set(0, (unsigned short)pVar->extractValueInInteger(i));
                            break;
                        }
                        case InternalType::RealInt32 :
                        {
                            Int32* pI = pIT->getAs<Int32>();
                            pI->set(0, (int)pVar->extractValueInInteger(i));
                            break;
                        }
                        case InternalType::RealUInt32 :
                        {
                            UInt32* pI = pIT->getAs<UInt32>();
                            pI->set(0, (unsigned int)pVar->extractValueInInteger(i));
                            break;
                        }
                        case InternalType::RealInt64 :
                        {
                            Int64* pI = pIT->getAs<Int64>();
                            pI->set(0, (long long)pVar->extractValueInInteger(i));
                            break;
                        }
                        case InternalType::RealUInt64 :
                        {
                            UInt64* pI = pIT->getAs<UInt64>();
                            pI->set(0, (unsigned long long)pVar->extractValueInInteger(i));
                            break;
                        }
                        }
                    }
                    else
                    {
                        pIT = pVar->extractValue(i);
                    }


                    symbol::Context::getInstance()->put(varName, *pIT);
                    e.body_get().accept(execBody);
                    if(e.body_get().is_break())
                    {
                        break;
                    }

                    if(e.body_get().is_continue())
                    {
                        const_cast<Exp*>(&(e.body_get()))->continue_reset();
                        continue;
                    }

                    if(e.body_get().is_return())
                    {
                        const_cast<ForExp*>(&e)->return_set();
                        break;
                    }
                }
                pIT->DecreaseRef();
                pVar->DecreaseRef();
                //delete pVar;
            }
            else
            {//Matrix i = [1,3,2,6] or other type
                T execBody;
                InternalType* pIT = execVar.result_get();
                GenericType* pVar = pIT->getAs<GenericType>();
                if(pVar->getDims() > 2)
                {
                    throw ScilabError(_W("for expression can only manage 1 or 2 dimensions variables\n"), 999, e.vardec_get().location_get());
                }

                for(int i = 0 ; i < pVar->getCols() ; i++)
                {
                    GenericType* pNew = pVar->getColumnValues(i);
                    symbol::Context::getInstance()->put(e.vardec_get().name_get(), *pNew);
                    e.body_get().accept(execBody);
                    if(e.body_get().is_break())
                    {
                        break;
                    }

                    if(e.body_get().is_continue())
                    {
                        continue;
                    }

                    if(e.body_get().is_return())
                    {
                        const_cast<ForExp*>(&e)->return_set();
                        break;
                    }
                }
            }
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
            if(e.is_global() == false)
            {//return(x)
                T execVar;
                e.exp_get().accept(execVar);

                if(execVar.result_getSize() == 1)
                {
                    //protect variable
                    InternalType *pIT = execVar.result_get();
                    pIT->IncreaseRef();
                    result_set(pIT);
                }
                else
                {
                    for(int i = 0 ; i < execVar.result_getSize() ; i++)
                    {
                        //protect variable
                        InternalType *pIT = execVar.result_get(i);
                        pIT->IncreaseRef();
                        result_set(i, pIT);
                    }
                }
            }

            if(result_getSize() == 1)
            {
                //unprotect variable
                result_get()->DecreaseRef();
            }
            else
            {
                for(int i = 0 ; i < result_getSize() ; i++)
                {
                    //unprotect variable
                    result_get(i)->DecreaseRef();
                }
            }
            const_cast<ReturnExp*>(&e)->return_set();
        }


        void visitprivate(const SelectExp &e)
        {
            // FIXME : exec select ... case ... else ... end
            T execMe;
            e.select_get()->accept(execMe);
            bool bCase = false;


            if(execMe.result_get() != NULL)
            {//find good case
                cases_t::iterator it;
                for(it = e.cases_get()->begin(); it != e.cases_get()->end() ; it++)
                {
                    T execCase;
                    CaseExp* pCase = *it;
                    pCase->test_get()->accept(execCase);
                    if(execCase.result_get() != NULL)
                    {
                        if(execCase.result_get()->isContainer()) //WARNING ONLY FOR CELL
                        {//check each item
                        }
                        else if(*execCase.result_get() == *execMe.result_get())
                        {//the good one
                            T execBody;
                            pCase->body_get()->accept(execBody);
                            bCase = true;
                            break;
                        }
                    }
                }
            }

            if(bCase == false)
            {//default case
                T execDefault;
                e.default_case_get()->accept(execDefault);
            }
        }


        void visitprivate(const CaseExp &e)
        {
        }


        void visitprivate(const SeqExp  &e)
        {
            std::list<Exp *>::const_iterator	itExp;

            for (itExp = e.exps_get().begin (); itExp != e.exps_get().end (); ++itExp)
            {
                T execMe;
                if(e.is_breakable())
                {
                    (*itExp)->breakable_set();
                }

                if(e.is_continuable())
                {
                    (*itExp)->continue_reset();
                    (*itExp)->continuable_set();
                }

                if(e.is_returnable())
                {
                    (*itExp)->returnable_set();
                }

                try
                {
                    (*itExp)->accept(execMe);

                    if(execMe.result_get() != NULL)
                    {
                        bool bImplicitCall = false;
                        if(execMe.result_get()->getAsCallable())//to manage call without ()
                        {
                            Callable *pCall = execMe.result_get()->getAsCallable();
                            types::typed_list out;
                            types::typed_list in;

                            try
                            {
                                T execCall;
                                Function::ReturnValue Ret = pCall->call(in, expected_getSize(), out, &execCall);

                                if(Ret == Callable::OK)
                                {
                                    if(out.size() == 0)
                                    {
                                        execMe.result_set(NULL);
                                    }
                                    else if(out.size() == 1)
                                    {
                                        out[0]->DecreaseRef();
                                        execMe.result_set(out[0]);
                                    }
                                    else
                                    {
                                        for(int i = 0 ; i < static_cast<int>(out.size()) ; i++)
                                        {
                                            out[i]->DecreaseRef();
                                            execMe.result_set(i, out[i]);
                                        }
                                    }

                                    bImplicitCall = true;
                                }
                                else if(Ret == Callable::Error)
                                {
                                    if(ConfigVariable::getLastErrorFunction() == L"")
                                    {
                                        ConfigVariable::setLastErrorFunction(pCall->getName());
                                        ConfigVariable::setLastErrorLine(e.location_get().first_line);
                                    }

                                    if(pCall->isMacro() || pCall->isMacroFile())
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
                            catch(ScilabMessage sm)
                            {
                                wostringstream os;
                                PrintVisitor printMe(os);
                                (*itExp)->accept(printMe);
                                os << std::endl << std::endl;
                                if(ConfigVariable::getLastErrorFunction() == L"")
                                {
                                    ConfigVariable::setLastErrorFunction(pCall->getName());
                                }

                                if(pCall->isMacro() || pCall->isMacroFile())
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
                        if(execMe.result_get() != NULL && (pVar == NULL || bImplicitCall))
                        {
                            symbol::Context::getInstance()->put(symbol::Symbol(L"ans"), *execMe.result_get());
                            if((*itExp)->is_verbose())
                            {
                                //TODO manage multiple returns
                                std::wostringstream ostr;
                                ostr << L"ans = " << std::endl << std::endl;
                                ostr << execMe.result_get()->toString(ConfigVariable::getFormat(), ConfigVariable::getConsoleWidth());
                                YaspWriteW(ostr.str().c_str());
                            }
                        }

                    }

                    if((&e)->is_breakable() && (*itExp)->is_break())
                    {
                        const_cast<SeqExp *>(&e)->break_set();
                        break;
                    }

                    if((&e)->is_continuable() && (*itExp)->is_continue())
                    {
                        const_cast<SeqExp *>(&e)->continue_set();
                        break;
                    }

                    if((&e)->is_returnable() && (*itExp)->is_return())
                    {
                        const_cast<SeqExp *>(&e)->return_set();
                        (*itExp)->return_reset();
                        break;
                    }
                }
                catch(ScilabMessage sm)
                {
                    YaspErrorW(sm.GetErrorMessage().c_str());

                    CallExp* pCall = dynamic_cast<CallExp*>(*itExp);
                    if(pCall != NULL)
                    {//to print call expression only of it is a macro
                        T execFunc;
                        pCall->name_get().accept(execFunc);

                        if(execFunc.result_get() != NULL && execFunc.result_get()->isCallable())
                        {
                            wostringstream os;
                            PrintVisitor printMe(os);
                            pCall->accept(printMe);
                            os << std::endl << std::endl;
                            if(ConfigVariable::getLastErrorFunction() == L"")
                            {
                                ConfigVariable::setLastErrorFunction(execFunc.result_get()->getAsCallable()->getName());
                            }
                            throw ScilabMessage(os.str(), 0, (*itExp)->location_get());
                        }
                    }

                    throw ScilabMessage((*itExp)->location_get());
                }
                catch(ScilabError se)
                {
                    if(ConfigVariable::getLastErrorMessage() == L"")
                    {
                        ConfigVariable::setLastErrorMessage(se.GetErrorMessage());
                        ConfigVariable::setLastErrorNumber(se.GetErrorNumber());
                        ConfigVariable::setLastErrorLine(se.GetErrorLocation().first_line);
                        ConfigVariable::setLastErrorFunction(wstring(L""));
                    }

                    CallExp* pCall = dynamic_cast<CallExp*>(*itExp);
                    if(pCall != NULL)
                    {//to print call expression only of it is a macro
                        T execFunc;

                        try
                        {
                            pCall->name_get().accept(execFunc);
                            if(execFunc.result_get() != NULL &&
                                (execFunc.result_get()->isMacro() || execFunc.result_get()->isMacroFile()))
                            {
                                wostringstream os;
                                PrintVisitor printMe(os);
                                pCall->accept(printMe);
                                os << std::endl << std::endl;
                                ConfigVariable::setLastErrorFunction(execFunc.result_get()->getAsCallable()->getName());
                                YaspErrorW(se.GetErrorMessage().c_str());
                                throw ScilabMessage(os.str(), 0, (*itExp)->location_get());
                            }
                        }
                        catch(ScilabError se2)
                        {//just to catch exception, do nothing
                        }
                    }

                    YaspErrorW(se.GetErrorMessage().c_str());
                    throw ScilabMessage((*itExp)->location_get());
                }
            }
        }


        void visitprivate(const ArrayListExp  &e)
        {
            std::list<Exp *>::const_iterator it;
            int i = 0;
            for(it = e.exps_get().begin() ; it != e.exps_get().end() ; it++)
            {
                T execArg;
                (*it)->accept(execArg);
                result_set(i, execArg.result_get()->clone());
                i++;
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
            @ or ~= !
            */
            T execMe;
            e.exp_get().accept(execMe);

            if(execMe.result_get()->isDouble())
            {
                InternalType* pVar  = execMe.result_get();
                Double *pdbl        = pVar->getAs<Double>();
                Bool *pReturn       = new Bool(pdbl->getRows(), pdbl->getCols());
                double *pR		    = pdbl->getReal();
                int *piB            = pReturn->get();
                for(int i = 0 ; i < pdbl->getSize() ; i++)
                {
                    piB[i] = pR[i] == 0 ? 1 : 0;
                }
                result_set(pReturn);
            }
            else if(execMe.result_get()->isBool())
            {
                InternalType* pIT	= execMe.result_get();
                Bool *pb			= pIT->getAs<types::Bool>();
                Bool *pReturn	= new Bool(pb->getRows(), pb->getCols());
                int *piR			= pb->get();
                int *piB			= pReturn->get();

                for(int i = 0 ; i < pb->getSize() ; i++)
                {
                    piB[i] = piR[i] == 1 ? 0 : 1;
                }
                result_set(pReturn);
            }
        }


        void visitprivate(const TransposeExp &e)
        {
            /*
            '
            */
            T execMe;
            e.exp_get().accept(execMe);

            bool bConjug = e.conjugate_get() == TransposeExp::_Conjugate_;

            if(execMe.result_get()->isImplicitList())
            {
                InternalType *pIT = execMe.result_get()->getAsImplicitList()->extractFullMatrix();
                execMe.result_set(pIT);
            }

            if(execMe.result_get()->isDouble())
            {
                InternalType* pVar  = execMe.result_get();
                Double *pdbl		= pVar->getAs<Double>();
                Double *pReturn	    = NULL;

                if(pdbl->isComplex())
                {
                    pReturn				= new Double(pdbl->getCols(), pdbl->getRows(), true);
                    double *pInR	=	pdbl->getReal();
                    double *pInI	=	pdbl->getImg();
                    double *pOutR	=	pReturn->getReal();
                    double *pOutI	=	pReturn->getImg();

                    vTransposeComplexMatrix(pInR, pInI, pdbl->getRows(), pdbl->getCols(), pOutR, pOutI, bConjug);
                }
                else
                {
                    pReturn				= new Double(pdbl->getCols(), pdbl->getRows(), false);
                    double *pInR	=	pdbl->getReal();
                    double *pOutR	=	pReturn->getReal();

                    vTransposeRealMatrix(pInR, pdbl->getRows(), pdbl->getCols(), pOutR);
                }
                result_set(pReturn);
            }
            else if(execMe.result_get()->isPoly())
            {
                InternalType *pIT	    = execMe.result_get();
                Polynom *pMP			= pIT->getAs<types::Polynom>();
                Polynom *pReturn	    = NULL;

                //prepare rank array
                int* piRank = new int[pMP->getSize()];

                for(int i = 0 ; i < pMP->getRows() ; i++)
                {
                    for(int j = 0 ; j < pMP->getCols() ; j++)
                    {
                        piRank[i * pMP->getCols() + j] = pMP->get(i,j)->getRank();
                    }
                }

                pReturn = new Polynom(pMP->getVariableName(), pMP->getCols(), pMP->getRows(), piRank);
                pReturn->setComplex(pMP->isComplex());

                if(pMP->isComplex() && bConjug)
                {
                    for(int i = 0 ; i < pMP->getRows() ; i++)
                    {
                        for(int j = 0 ; j < pMP->getCols() ; j++)
                        {
                            pReturn->setCoef(j, i, pMP->get(i,j)->getCoef());
                            double *pdblImg = pReturn->get(j, i)->getCoefImg();
                            for(int k = 0 ; k < pReturn->get(j, i)->getRank() ; k++)
                            {
                                pdblImg[k] *= -1;
                            }
                        }
                    }
                }
                else
                {
                    for(int i = 0 ; i < pMP->getRows() ; i++)
                    {
                        for(int j = 0 ; j < pMP->getCols() ; j++)
                        {
                            pReturn->setCoef(j, i, pMP->get(i,j)->getCoef());
                        }
                    }
                }

                result_set(pReturn);
            }
            else if(execMe.result_get()->isString())
            {
                InternalType* pVar  = execMe.result_get();
                String *pS          = pVar->getAs<types::String>();
                String* pReturn     = new String(pS->getCols(), pS->getRows());

                for(int i = 0 ; i < pS->getRows() ; i++)
                {
                    for(int j = 0 ; j < pS->getCols() ; j++)
                    {
                        pReturn->set(j,i, pS->get(i,j));
                    }
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
            /*Create local exec visitor*/
            T execMe;
            try
            {
                /*getting what to assign*/
                e.init_get().accept(execMe);
                result_set(execMe.result_get());
                result_get()->IncreaseRef();
            }
            catch(ScilabError error)
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
            std::list<ast::Var *>::const_iterator	i;

            //get input parameters list
            std::list<symbol::Symbol> *pVarList = new std::list<symbol::Symbol>();
            const ArrayListVar *pListVar = &e.args_get();
            for(i = pListVar->vars_get().begin() ; i != pListVar->vars_get().end() ; i++)
            {
                pVarList->push_back(static_cast<SimpleVar*>(*i)->name_get());
            }

            //get output parameters list
            std::list<symbol::Symbol> *pRetList = new std::list<symbol::Symbol>();
            const ArrayListVar *pListRet = &e.returns_get();
            for(i = pListRet->vars_get().begin() ; i != pListRet->vars_get().end() ; i++)
            {
                pRetList->push_back(static_cast<SimpleVar*>(*i)->name_get());
            }

//            Location* newloc = const_cast<Location*>(&location_get())->clone();
            Exp* exp = const_cast<Exp*>(&e.body_get())->clone();
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
            T	execMeStart;
            T	execMeStep;
            T	execMeEnd;

            try
            {
                e.start_get().accept(execMeStart);
                GenericType* pITStart = static_cast<GenericType*>(execMeStart.result_get());
                if(pITStart->getRows() != 1 || pITStart->getCols() != 1)
                {
                    throw 1;
                }


                e.step_get().accept(execMeStep);
                GenericType* pITStep = static_cast<GenericType*>(execMeStep.result_get());
                if(pITStep->getRows() != 1 || pITStep->getCols() != 1)
                {
                    throw 2;
                }

                e.end_get().accept(execMeEnd);
                GenericType* pITEnd = static_cast<GenericType*>(execMeEnd.result_get());
                if(pITEnd->getRows() != 1 || pITEnd->getCols() != 1)
                {
                    throw 3;
                }

                //check compatibility

                if(execMeStart.result_get()->isInt())
                {//if Step or End are Int too, they must have the same precision
                    if(execMeStep.result_get()->isInt())
                    {
                        if(execMeStep.result_get()->getType() != execMeStart.result_get()->getType())
                        {
                            throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.step_get().location_get());
                        }
                    }
                    else if(execMeStep.result_get()->isPoly())
                    {
                        throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.step_get().location_get());
                    }


                    if(execMeEnd.result_get()->isInt())
                    {
                        if(execMeEnd.result_get()->getType() != execMeStart.result_get()->getType())
                        {
                            throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.end_get().location_get());
                        }
                    }
                    else if(execMeEnd.result_get()->isPoly())
                    {
                            throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.end_get().location_get());
                    }
                }
                else if(execMeStart.result_get()->isPoly())
                {
                    if(execMeStep.result_get()->isInt())
                    {
                        throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.step_get().location_get());
                    }

                    if(execMeEnd.result_get()->isInt())
                    {
                        throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.end_get().location_get());
                    }
                }
                else if(execMeStep.result_get()->isInt())
                {//if End is Int too, they must have the same precision
                    if(execMeEnd.result_get()->isInt())
                    {
                        if(execMeEnd.result_get()->getType() != execMeStep.result_get()->getType())
                        {
                            throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.end_get().location_get());
                        }
                    }
                }
                else if(execMeStep.result_get()->isPoly())
                {
                    if(execMeEnd.result_get()->isInt())
                    {
                        throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.step_get().location_get());
                    }
                }

                ImplicitList *pIL	= new ImplicitList(
                    execMeStart.result_get(),
                    execMeStep.result_get(),
                    execMeEnd.result_get());

                result_set(pIL);
            }
            catch(int iPos)
            {
                wchar_t szError[bsiz];
                os_swprintf(szError, bsiz, _W("%ls: Wrong type for argument %d: Scalar expected.\n"), L":", iPos);
                throw ScilabError(szError, 999, e.location_get());
            }
            catch(ScilabError error)
            {
                //TODO YaSp : Overloading
                throw error;
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
