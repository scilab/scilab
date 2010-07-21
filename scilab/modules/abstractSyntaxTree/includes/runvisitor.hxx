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

// Needed by visitprivate(const OpExp &)
// Needed by visitprivate(const LogicalOpExp &)
#include "types_multiplication.hxx"
#include "types_addition.hxx"
#include "types_substraction.hxx"
#include "types_divide.hxx"
#include "types_power.hxx"
#include "configvariable.hxx"
#include "overload.hxx"

extern "C" {
#include "doublecomplex.h"
#include "matrix_transpose.h"
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
                    }
                }
            }
        }

    public:
        int expected_size_get(void)
        {
            return _excepted_result;
        }

        int result_size_get(void)
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
            if(_iPos >= static_cast<int>(_resultVect.size()))
            {
                return NULL;
            }
            return _resultVect[_iPos];
        }

        vector<types::InternalType*>* result_list_get()
        {
            if(result_size_get() == 1)
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
    protected :
        int  GetIndexList(InternalType* _pRef, std::list<ast::Exp *>const& _plstArg, int** _piIndexSeq, int** _piMaxDim, InternalType *_pRefVar, int *_iDimSize)
        {
            //Create list of indexes
            //std::vector<std::vector<int>> IndexList;
            int iProductElem				= static_cast<int>(_plstArg.size());
            int **piIndexList				= NULL;
            int *piTabsize					= NULL;
            int iTotalCombi					= 1;
            int k										= 0;

            piTabsize			= new int[iProductElem];
            piIndexList		= new int*[iProductElem];

            (*_piMaxDim)	= new int[iProductElem];

            T execMeArg;
            std::list<Exp *>::const_iterator	i;
            for(i = _plstArg.begin() ; i != _plstArg.end() ; i++,k++)
            {
                (*i)->accept(execMeArg);
                InternalType *pIn = NULL;
                Double *pDbl = NULL;
                bool bDeleteDbl = false;

                if(execMeArg.result_get()->getType() == InternalType::RealImplicitList)
                {//a:b:c
                    int iMaxDim = 0;
                    if(_pRefVar != NULL)
                    {
                        iMaxDim = GetVarMaxDim(_pRefVar, k, iProductElem);
                    }

                    Double dbl(iMaxDim); // $
                    ImplicitList *pIL = execMeArg.result_get()->getAsImplicitList();
                    if(pIL->computable() == false)
                    {
                        if(pIL->start_type_get() == InternalType::RealPoly)
                        {
                            MatrixPoly *poPoly	= pIL->start_get()->getAsPoly();
                            pIL->start_set(poPoly->evaluate(&dbl));
                        }
                        if(pIL->step_type_get() == InternalType::RealPoly)
                        {
                            MatrixPoly *poPoly	= pIL->step_get()->getAsPoly();
                            pIL->step_set(poPoly->evaluate(&dbl));
                        }
                        if(pIL->end_type_get() == InternalType::RealPoly)
                        {
                            MatrixPoly *poPoly	= pIL->end_get()->getAsPoly();
                            pIL->end_set(poPoly->evaluate(&dbl));
                        }
                    }

                    pDbl = pIL->extract_matrix()->getAsDouble();
                    bDeleteDbl = true;
                }
                else if(execMeArg.result_get()->getType() == InternalType::RealBool)
                {
                    Bool *pB    = execMeArg.result_get()->getAsBool();
                    int *piB    = pB->bool_get();

                    //find true item count
                    int iItemCount = 0;
                    for(int j = 0 ; j < pB->size_get() ; j++)
                    {
                        if(piB[j])
                        {
                            iItemCount++;
                        }
                    }

                    //allow new Double variable
                    pDbl            = new Double(iItemCount, 1);
                    double* pdbl    = pDbl->real_get();

                    int j = 0;
                    for(int l = 0 ; l < pB->size_get() ; l++)
                    {
                        if(piB[l])
                        {
                            pdbl[j++] = l + 1;
                        }
                    }

                    bDeleteDbl		= true;
                }
                else if(execMeArg.result_get()->getType() == InternalType::RealPoly)
                {
                    MatrixPoly *pPoly = execMeArg.result_get()->getAsPoly();

                    if(_pRefVar != NULL)
                    {
                        int iMaxDim = GetVarMaxDim(_pRefVar, k, iProductElem);
                        Double dbl(iMaxDim); // $
                        pDbl = pPoly->evaluate(&dbl);
                        bDeleteDbl = true;
                    }
                    else
                    {//houston we have a problem ...
                        Double dbl(0);
                        pDbl = pPoly->evaluate(&dbl);

                    }
                }
                else if(execMeArg.result_get()->getType() == InternalType::RealString)
                {
                    String* pS	= execMeArg.result_get()->getAsString();
                    if(_pRef != NULL && _pRef->isTList())
                    {
                        TList* pTL      = _pRef->getAsTList();
                        pDbl            = new Double(pS->size_get(), 1);
                        double* pdbl    = pDbl->real_get();

                        for(int i = 0 ; i < pS->size_get() ; i++)
                        {
                            pdbl[i] =  pTL->getIndexFromString(pS->string_get(i));

                            if(pdbl[i] == -1)
                            {//field not found
                                return 0;
                            }
                            else
                            {
                                pdbl[i]++; //indexed at 1
                            }
                        }

                        bDeleteDbl		= true;
                    }
                    else if(_pRef != NULL && _pRef->isStruct())
                    {

                    }
                    else
                    {//Heu ... ?
                    }
                }
                else if(execMeArg.result_get()->getType() == InternalType::RealDouble)
                {
                    pDbl	= execMeArg.result_get()->getAsDouble();
                }
                else
                {//Heu ... ?
                }

                double *pData = pDbl->real_get();

                piTabsize[k] = pDbl->size_get();
                piIndexList[k] = new int[piTabsize[k]];

                (*_piMaxDim)[k] = static_cast<int>(pData[0] + 0.5);
                int iSize = pDbl->size_get();
                if(_iDimSize != NULL)
                {
                    _iDimSize[k] = iSize;
                }

                for(int j = 0 ; j < iSize ; j++)
                {
                    piIndexList[k][j] = static_cast<int>(pData[j] + 0.5);
                    if(piIndexList[k][j] > (*_piMaxDim)[k])
                    {
                        (*_piMaxDim)[k] = piIndexList[k][j];
                    }
                }
                iTotalCombi *= iSize;

                if(bDeleteDbl == true)
                {
                    delete pDbl;
                }
            }

            int iTabsize	= iTotalCombi * iProductElem;
            *_piIndexSeq	= new int[iTabsize];

            if(iTabsize > 1)
            {
                ExpandList(piIndexList, piTabsize, iProductElem, *_piIndexSeq);
            }
            else
            {
                _piIndexSeq[0][0] = piIndexList[0][0];
            }

            delete [] piTabsize;

            for(int l = 0 ; l < iProductElem ; l++)
            {
                delete[] piIndexList[l];
            }
            delete[] piIndexList;
            return iTotalCombi;
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
            int iColMax = -1;

            //check dimmension
            for (row = e.lines_get().begin() ; row != e.lines_get().end() ; ++row )
            {
                if(iColMax == -1)
                {
                    iColMax = static_cast<int>((*row)->columns_get().size());
                }

                if(iColMax != static_cast<int>((*row)->columns_get().size()))
                {
                    std::wostringstream os;
                    os << L"inconsistent row/column dimensions";
                    os << ((Location)(*row)->location_get()).location_string_get() << std::endl;
                    throw os.str();
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
            Double *pdbl = new Double(e.value_get());
            result_set(pdbl);
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
                    ostr << e.name_get() << L" = " << L"(" << pI->getRef() << L")"<< std::endl;
                    ostr << std::endl;
                    ostr << pI->toString(ConfigVariable::getFormat(), ConfigVariable::getConsoleWidth()) << std::endl;
                    YaspWriteW(ostr.str().c_str());
                }
            }
            else
            {
                wchar_t szError[bsiz];
#ifdef _MSC_VER
                swprintf_s(szError, bsiz, _W("Undefined variable: %s\n"), e.name_get().c_str());
#else
                swprintf(szError, bsiz, _W("Undefined variable: %ls\n"), e.name_get().c_str());
#endif
                throw wstring(szError);
                //Err, SimpleVar doesn't exist in Scilab scopes.
            }
        }


        void visitprivate(const ColonVar &e)
        {
            int pRank[1] = {2};
            Double dblCoef(1,2);
            dblCoef.val_set(0, 0, 0);
            dblCoef.val_set(0, 1, 1);

            MatrixPoly *pVar = new MatrixPoly(L"$", 1, 1, pRank);
            Poly *poPoly = pVar->poly_get(0,0);
            poPoly->coef_set(&dblCoef);

            ImplicitList *pIL = new ImplicitList();
            pIL->start_set(new Double(1));
            pIL->step_set(new Double(1));
            pIL->end_set(pVar);
            result_set(pIL);
            /*
            : = 1:$
            */
        }


        void visitprivate(const DollarVar &e)
        {
            int pRank[1] = {2};
            Double dblCoef(1,2);
            dblCoef.val_set(0, 0, 0);
            dblCoef.val_set(0, 1, 1);

            MatrixPoly *pVar = new MatrixPoly(L"$", 1, 1, pRank);
            Poly *poPoly = pVar->poly_get(0,0);
            poPoly->coef_set(&dblCoef);
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
            catch(wstring sz)
            {
                throw sz;
            }

            if(execHead.result_get() != NULL && execHead.result_get()->isStruct())
            {
                SimpleVar *psvRightMember = dynamic_cast<SimpleVar *>(const_cast<Exp *>(e.tail_get()));
                if(psvRightMember != NULL)
                {
                    Struct* psValue = execHead.result_get()->getAsStruct();
                    if(psValue->exists(psvRightMember->name_get()))
                    {
                        InternalType* pIT = psValue->get(psvRightMember->name_get());
                        result_set(pIT);
                    }
                    else
                    {
                        wchar_t szError[bsiz];
#ifdef _MSC_VER
                        swprintf_s(szError, bsiz, _W("Unknown field : %s.\n"), psvRightMember->name_get().c_str());
#else
                        swprintf(szError, bsiz, _W("Unknown field : %ls.\n"), psvRightMember->name_get().c_str());
#endif
                        throw wstring(szError);
                    }
                }
                else
                {
                    wchar_t szError[bsiz];
#ifdef _MSC_VER
                    swprintf_s(szError, bsiz, _W("/!\\ Unmanaged FieldExp.\n"));
#else
                    swprintf(szError, bsiz, _W("/!\\ Unmanaged FieldExp.\n"));
#endif
                    throw wstring(szError);
                }
            }
            else if(execHead.result_get() != NULL && execHead.result_get()->isTList())
            {
                SimpleVar *psvRightMember = dynamic_cast<SimpleVar *>(const_cast<Exp *>(e.tail_get()));
                if(psvRightMember != NULL)
                {
                    TList* psValue = execHead.result_get()->getAsTList();
                    if(psValue->exists(psvRightMember->name_get()))
                    {
                        InternalType* pIT = psValue->get(psvRightMember->name_get());
                        result_set(pIT);
                    }
                    else
                    {
                        wchar_t szError[bsiz];
#ifdef _MSC_VER
                        swprintf_s(szError, bsiz, _W("Unknown field : %s.\n"), psvRightMember->name_get().c_str());
#else
                        swprintf(szError, bsiz, _W("Unknown field : %ls.\n"), psvRightMember->name_get().c_str());
#endif
                        throw wstring(szError);
                    }
                }
                else
                {
                    wchar_t szError[bsiz];
#ifdef _MSC_VER
                    swprintf_s(szError, bsiz, _W("/!\\ Unmanaged FieldExp.\n"));
#else
                    swprintf(szError, bsiz, _W("/!\\ Unmanaged FieldExp.\n"));
#endif
                    throw wstring(szError);
                }
            }
            else
            {
                wchar_t szError[bsiz];
#ifdef _MSC_VER
                swprintf_s(szError, bsiz, _W("Attempt to reference field of non-structure array.\n"));
#else
                swprintf(szError, bsiz, _W("Attempt to reference field of non-structure array.\n"));
#endif
                throw wstring(szError);
            }
        }


        void visitprivate(const CellCallExp &e)
        {
            //get head
            T execMeCell;
            e.name_get().accept(execMeCell);

            if(execMeCell.result_get() != NULL)
            {//a{xxx} with a variable, extraction
                InternalType *pIT = NULL;

                pIT = execMeCell.result_get();

                if(pIT)
                {
                    int iArgDim = static_cast<int>(e.args_get().size());
                    std::vector<InternalType*> ResultList;

                    //Create list of indexes
                    bool bSeeAsVector   = iArgDim == 1;
                    int *piIndexSeq		= NULL;
                    int *piMaxDim       = NULL;
                    int *piDimSize		= new int[iArgDim];
                    int iTotalCombi		= GetIndexList(pIT, e.args_get(), &piIndexSeq, &piMaxDim, pIT, piDimSize);

                    //check we don't have bad indexes like "< 1"
                    for(int i = 0 ; i < iTotalCombi * iArgDim; i++)
                    {
                        if(piIndexSeq[i] < 1)
                        {
                            //manage error
                            std::wostringstream os;
                            os << _W("Indexes must be positive .\n");
                            os << ((Location)e.name_get().location_get()).location_string_get() << std::endl;
                            throw os.str();
                        }
                    }

                    ResultList = pIT->getAsCell()->extract_cell(iTotalCombi, piIndexSeq, piMaxDim, piDimSize, bSeeAsVector);

                    delete[] piDimSize;
                    delete[] piIndexSeq;
                    delete[] piMaxDim;

                    if(ResultList.size() == 0)
                    {
                        std::wostringstream os;
                        os << L"inconsistent row/column dimensions";
                        os << ((*e.args_get().begin())->location_get()).location_string_get() << std::endl;
                        throw os.str();
                    }

                    for(int i = 0 ; i < static_cast<int>(ResultList.size()) ; i++)
                    {
                        result_set(i, ResultList[i]);
                    }
                }
            }
            else
            {
                //result == NULL ,variable doesn't exist :(
                // Sould never be in this case
                // In worst case variable pointing to function does not exists
                // visitprivate(SimpleVar) will throw the right exception.
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
        }


        void visitprivate(const WhileExp  &e)
        {
            T execMeTest;
            T execMeAction;

            //allow break operation
            const_cast<Exp*>(&e.body_get())->breakable_set();
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
                e.test_get().accept(execMeTest);
            }
        }


        void visitprivate(const ForExp  &e)
        {
            T execVar;
            e.vardec_get().accept(execVar);

            //allow break operation
            const_cast<Exp*>(&e.body_get())->breakable_set();
            //allow return operation
            if(e.is_returnable())
            {
                (&e.body_get())->is_returnable();
            }

            if(execVar.result_get()->getType() == InternalType::RealImplicitList)
            {
                T execBody;
                ImplicitList* pVar = execVar.result_get()->getAsImplicitList();
                //			std::cout << "ImplicitList references : " << pVar->getRef() << std::endl;

                InternalType *pIT = NULL;
                pIT = pVar->extract_value(0);
                wstring varName = e.vardec_get().name_get();
                symbol::Context::getInstance()->put(varName, *pIT);

                for(int i = 0 ; i < pVar->size_get() ; i++)
                {
                    if(pIT->getAsDouble())
                    {
                        Double *pDouble = pIT->getAsDouble();
                        pDouble->real_get()[0] = pVar->extract_value_double(i);
                    }
                    else if(pIT->getAsInt())
                    {
                        Int *pInt = pIT->getAsInt();
                        pInt->data_set(0, 0,  pVar->extract_value_int(i));
                    }
                    else
                    {
                        pIT = pVar->extract_value(i);
                        symbol::Context::getInstance()->put(varName, *pIT);
                    }

                    e.body_get().accept(execBody);
                    if(e.body_get().is_break())
                    {
                        break;
                    }

                    if(e.body_get().is_return())
                    {
                        const_cast<ForExp*>(&e)->return_set();
                        break;
                    }
                }

                pVar->DecreaseRef();
            }
            else
            {//Matrix i = [1,3,2,6] or other type
                T execBody;
                GenericType* pVar = static_cast<GenericType*>(execVar.result_get());
                for(int i = 0 ; i < pVar->cols_get() ; i++)
                {
                    GenericType* pNew = pVar->get_col_value(i);
                    symbol::Context::getInstance()->put(e.vardec_get().name_get(), *pNew);
                    e.body_get().accept(execBody);
                    if(e.body_get().is_break())
                    {
                        break;
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


        void visitprivate(const ReturnExp &e)
        {
            if(e.is_global() == false)
            {//return(x)
                T execVar;
                e.exp_get().accept(execVar);

                if(execVar.result_size_get() == 1)
                {
                    //protect variable
                    InternalType *pIT = execVar.result_get();
                    pIT->IncreaseRef();
                    result_set(pIT);
                }
                else
                {
                    for(int i = 0 ; i < execVar.result_size_get() ; i++)
                    {
                        //protect variable
                        InternalType *pIT = execVar.result_get(i);
                        pIT->IncreaseRef();
                        result_set(i, pIT);
                    }
                }
            }

            if(result_size_get() == 1)
            {
                //unprotect variable
                result_get()->DecreaseRef();
            }
            else
            {
                for(int i = 0 ; i < result_size_get() ; i++)
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

                if(e.is_returnable())
                {
                    (*itExp)->returnable_set();
                }

                (*itExp)->accept(execMe);

                if(execMe.result_get() != NULL)
                {
                    bool bImplicitCall = false;
                    if(execMe.result_get()->getAsCallable())//to manage call without ()
                    {
                        Callable *pCall = execMe.result_get()->getAsCallable();
                        types::typed_list out;
                        types::typed_list in;

                        T execCall;
                        Function::ReturnValue Ret = pCall->call(in, expected_size_get(), out, &execCall);

                        if(Ret == Callable::OK)
                        {
                            if(expected_size_get() == 1 && out.size() == 0) //to manage ans
                            {
                                if(static_cast<int>(out.size()) < expected_size_get())
                                {
                                    std::wostringstream os;
                                    os << L"bad lhs, expected : " << expected_size_get() << L" returned : " << out.size() << std::endl;
                                    throw os.str();
                                }
                            }

                            if(out.size() == 1)
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
                            std::wostringstream os;
                            wchar_t szError[bsiz];
#ifdef _MSC_VER
                            swprintf_s(szError, bsiz, _W("Function \"%s\" failed\n"), pCall->getName().c_str());
#else
                            swprintf(szError, bsiz, _W("Function \"%ls\" failed\n"), pCall->getName().c_str());
#endif
                            throw wstring(szError);
                        }
                    }


                    SimpleVar* pVar = dynamic_cast<SimpleVar*>(*itExp);
                    //don't output Silplevar and empty result
                    if(execMe.result_get() != NULL && (pVar == NULL || bImplicitCall))
                    {
                        symbol::Context::getInstance()->put(L"ans", *execMe.result_get());
                        if((*itExp)->is_verbose())
                        {
                            //TODO manage multiple returns
                            std::wostringstream ostr;
                            ostr << L"ans = " << std::endl << std::endl;
                            ostr << execMe.result_get()->toString(ConfigVariable::getFormat(), ConfigVariable::getConsoleWidth()) << std::endl;
                            YaspWriteW(ostr.str().c_str());
                        }
                    }

                }

                if((&e)->is_breakable() && (*itExp)->is_break())
                {
                    const_cast<SeqExp *>(&e)->break_set();
                    break;
                }

                if((&e)->is_returnable() && (*itExp)->is_return())
                {
                    const_cast<SeqExp *>(&e)->return_set();
                    (*itExp)->return_reset();
                    break;
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
                Double *pdbl	= execMe.result_get()->getAsDouble();
                Bool *pReturn	= new Bool(pdbl->rows_get(), pdbl->cols_get());
                double *pR		= pdbl->real_get();
                int *piB			= pReturn->bool_get();
                for(int i = 0 ; i < pdbl->size_get() ; i++)
                {
                    piB[i] = pR[i] == 0 ? 1 : 0;
                }
                result_set(pReturn);
            }
            else if(execMe.result_get()->isBool())
            {
                Bool *pb			= execMe.result_get()->getAsBool();
                Bool *pReturn	= new Bool(pb->rows_get(), pb->cols_get());
                int *piR			= pb->bool_get();
                int *piB			= pReturn->bool_get();

                for(int i = 0 ; i < pb->size_get() ; i++)
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
                InternalType *pIT = execMe.result_get()->getAsImplicitList()->extract_matrix();
                execMe.result_set(pIT);
            }

            if(execMe.result_get()->isDouble())
            {
                Double *pdbl		= execMe.result_get()->getAsDouble();
                Double *pReturn	= NULL;

                if(pdbl->isComplex())
                {
                    pReturn				= new Double(pdbl->cols_get(), pdbl->rows_get(), true);
                    double *pInR	=	pdbl->real_get();
                    double *pInI	=	pdbl->img_get();
                    double *pOutR	=	pReturn->real_get();
                    double *pOutI	=	pReturn->img_get();

                    vTransposeComplexMatrix(pInR, pInI, pdbl->rows_get(), pdbl->cols_get(), pOutR, pOutI, bConjug);
                }
                else
                {
                    pReturn				= new Double(pdbl->cols_get(), pdbl->rows_get(), false);
                    double *pInR	=	pdbl->real_get();
                    double *pOutR	=	pReturn->real_get();

                    vTransposeRealMatrix(pInR, pdbl->rows_get(), pdbl->cols_get(), pOutR);
                }
                result_set(pReturn);
            }
            else if(execMe.result_get()->isPoly())
            {
                MatrixPoly *pMP			= execMe.result_get()->getAsPoly();
                MatrixPoly *pReturn	= NULL;

                //prepare rank array
                int* piRank = new int[pMP->size_get()];

                for(int i = 0 ; i < pMP->rows_get() ; i++)
                {
                    for(int j = 0 ; j < pMP->cols_get() ; j++)
                    {
                        piRank[i * pMP->cols_get() + j] = pMP->poly_get(i,j)->rank_get();
                    }
                }

                pReturn = new MatrixPoly(pMP->var_get(), pMP->cols_get(), pMP->rows_get(), piRank);
                pReturn->complex_set(pMP->isComplex());

                if(pMP->isComplex() && bConjug)
                {
                    for(int i = 0 ; i < pMP->rows_get() ; i++)
                    {
                        for(int j = 0 ; j < pMP->cols_get() ; j++)
                        {
                            pReturn->poly_set(j, i, pMP->poly_get(i,j)->coef_get());
                            double *pdblImg = pReturn->poly_get(j, i)->coef_img_get();
                            for(int k = 0 ; k < pReturn->poly_get(j, i)->rank_get() ; k++)
                            {
                                pdblImg[k] *= -1;
                            }
                        }
                    }
                }
                else
                {
                    for(int i = 0 ; i < pMP->rows_get() ; i++)
                    {
                        for(int j = 0 ; j < pMP->cols_get() ; j++)
                        {
                            pReturn->poly_set(j, i, pMP->poly_get(i,j)->coef_get());
                        }
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
            catch(wstring sz)
            {
                throw sz;
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
            std::list<wstring> *pVarList = new std::list<wstring>();
            const ArrayListVar *pListVar = &e.args_get();
            for(i = pListVar->vars_get().begin() ; i != pListVar->vars_get().end() ; i++)
            {
                pVarList->push_back(static_cast<SimpleVar*>(*i)->name_get());
            }

            //get output parameters list
            std::list<wstring> *pRetList = new std::list<wstring>();
            const ArrayListVar *pListRet = &e.returns_get();
            for(i = pListRet->vars_get().begin() ; i != pListRet->vars_get().end() ; i++)
            {
                pRetList->push_back(static_cast<SimpleVar*>(*i)->name_get());
            }

            //types::Macro macro(VarList, RetList, (SeqExp&)e.body_get());
            types::Macro *pMacro = new types::Macro(e.name_get(), *pVarList, *pRetList,
                static_cast<SeqExp&>(const_cast<Exp&>(e.body_get())), L"script");
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
                if(pITStart->rows_get() != 1 || pITStart->cols_get() != 1)
                {
                    throw 1;
                }


                e.step_get().accept(execMeStep);
                GenericType* pITStep = static_cast<GenericType*>(execMeStep.result_get());
                if(pITStep->rows_get() != 1 || pITStep->cols_get() != 1)
                {
                    throw 2;
                }

                e.end_get().accept(execMeEnd);
                GenericType* pITEnd = static_cast<GenericType*>(execMeEnd.result_get());
                if(pITEnd->rows_get() != 1 || pITEnd->cols_get() != 1)
                {
                    throw 3;
                }

                //check compatibility

                if(execMeStart.result_get()->getType() == InternalType::RealInt)
                {//if Step or End are Int too, they must have the same precision
                    Int::IntType IT = execMeStart.result_get()->getAsInt()->getIntType();

                    if(execMeStep.result_get()->getType() == InternalType::RealInt)
                    {
                        if(execMeStep.result_get()->getAsInt()->getIntType() != IT)
                        {
                            throw string(_("Undefined operation for the given operands.\n"));
                        }
                    }
                    else if(execMeStep.result_get()->getType() == InternalType::RealPoly)
                    {
                        throw string(_("Undefined operation for the given operands.\n"));
                    }


                    if(execMeEnd.result_get()->getType() == InternalType::RealInt)
                    {
                        if(execMeEnd.result_get()->getAsInt()->getIntType() != IT)
                        {
                            throw string(_("Undefined operation for the given operands.\n"));
                        }
                    }
                    else if(execMeEnd.result_get()->getType() == InternalType::RealPoly)
                    {
                        throw string(_("Undefined operation for the given operands.\n"));
                    }
                }
                else if(execMeStart.result_get()->getType() == InternalType::RealPoly)
                {
                    if(execMeStep.result_get()->getType() == InternalType::RealInt)
                    {
                        throw string(_("Undefined operation for the given operands.\n"));
                    }

                    if(execMeEnd.result_get()->getType() == InternalType::RealInt)
                    {
                        throw string(_("Undefined operation for the given operands.\n"));
                    }
                }
                else if(execMeStep.result_get()->getType() == InternalType::RealInt)
                {//if End are Int too, they must have the same precision
                    Int::IntType IT = execMeStep.result_get()->getAsInt()->getIntType();

                    if(execMeEnd.result_get()->getType() == InternalType::RealInt)
                    {
                        if(execMeEnd.result_get()->getAsInt()->getIntType() != IT)
                        {
                            throw string(_("Undefined operation for the given operands.\n"));
                        }
                    }
                }
                else if(execMeStep.result_get()->getType() == InternalType::RealPoly)
                {
                    if(execMeEnd.result_get()->getType() == InternalType::RealInt)
                    {
                        throw string(_("Undefined operation for the given operands.\n"));
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
                char st[bsiz];
#ifdef _MSC_VER
                sprintf_s(st, bsiz, _("%s: Wrong type for argument %d: Scalar expected.\n"), ":", iPos);
#else
                sprintf(st, _("%s: Wrong type for argument %d: Scalar expected.\n"), "::", 1);
#endif
                throw string(st);
            }
            catch(wstring sz)
            {
                //TODO YaSp : Overloading
                throw sz;
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
