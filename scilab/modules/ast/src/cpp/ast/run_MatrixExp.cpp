/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

//file included in runvisitor.cpp

/*
    [1,2;3,4] with/without special character $ and :
*/
template<class T>
void RunVisitorT<T>::visitprivate(const MatrixExp &e)
{
    try
    {
        std::list<MatrixLineExp *>::const_iterator row;
        std::list<Exp *>::const_iterator col;
        InternalType *poResult = NULL;
        list<InternalType*> rowList;

        if (e.lines_get().size() == 0)
        {
            result_set(Double::Empty());
            return;
        }

        //do all [x,x]
        for (row = e.lines_get().begin() ; row != e.lines_get().end() ; row++)
        {
            InternalType* poRow = NULL;
            for (col = (*row)->columns_get().begin() ; col != (*row)->columns_get().end() ; col++)
            {
                (*col)->accept(*this);

                InternalType *pIT = result_get();
                if (pIT == NULL)
                {
                    continue;
                }

                //reset result but whitout delete the value
                result_clear_except_first();

                if (pIT->isGenericType() == false)
                {
                    pIT->killMe();
                    std::wostringstream os;
                    os << _W("unable to concatenate\n");
                    throw ast::ScilabError(os.str(), 999, (*col)->location_get());
                }

                GenericType* pGT = pIT->getAs<GenericType>();

                if (pGT->isImplicitList() && pGT->getAs<ImplicitList>()->isComputable())
                {
                    ImplicitList *pIL = pGT->getAs<ImplicitList>();
                    InternalType* pIT2 = pIL->extractFullMatrix();
                    pGT->killMe();
                    pGT = pIT2->getAs<GenericType>();
                }

                if (pGT->isDouble() && pGT->getAs<Double>()->isEmpty())
                {
                    pGT->killMe();
                    continue;
                }

                if (poRow == NULL)
                {
                    //first loop
                    poRow = pGT;
                    continue;
                }

                GenericType* pGTResult = poRow->getAs<GenericType>();

                if (pGT->isList() || pGTResult->isList())
                {
                    poRow = callOverloadMatrixExp(L"c", pGTResult, pGT);
                    continue;
                }

                // hypermatrix case, will call %hm_c_hm
                if (pGT->getDims() > 2)
                {
                    poRow = callOverloadMatrixExp(L"c", pGTResult, pGT);
                    continue;
                }

                //check dimension
                if (pGT->getDims() != 2 || pGT->getRows() != pGTResult->getRows())
                {
                    poRow->killMe();
                    if (poRow != pGT)
                    {
                        pGT->killMe();
                    }
                    std::wostringstream os;
                    os << _W("inconsistent row/column dimensions\n");
                    throw ast::ScilabError(os.str(), 999, (*row)->location_get());
                }

                // if we concatenate [Double Sparse], transform the Double to Sparse and perform [Sparse Sparse]
                // this avoids to allocate a Double result of size of Double+Sparse and initialize all elements.
                if (pGT->isSparse() && pGTResult->isDouble())
                {
                    poRow = new types::Sparse(*pGTResult->getAs<types::Double>());
                }
                else if (pGT->isSparseBool() && pGTResult->isBool()) // [Bool SparseBool] => [SparseBool SparseBool]
                {
                    poRow = new types::SparseBool(*pGTResult->getAs<types::Bool>());
                }

                InternalType *p = AddElementToVariable(NULL, poRow, pGTResult->getRows(), pGTResult->getCols() + pGT->getCols());
                p = AddElementToVariable(p, pGT, 0, pGTResult->getCols());
                if (poRow != pGT)
                {
                    pGT->killMe();
                }
                if (p != poRow)
                {
                    poRow->killMe();
                    poRow = p;
                }
            }

            if (poRow == NULL)
            {
                continue;
            }

            GenericType* pGT = poRow->getAs<GenericType>();
            if (poResult == NULL)
            {
                poResult = pGT;
                continue;
            }

            //check dimension
            GenericType* pGTResult = poResult->getAs<GenericType>();

            if (pGT->isList() || pGTResult->isList())
            {
                poResult = callOverloadMatrixExp(L"f", pGTResult, pGT);
                continue;
            }

            // hypermatrix case, will call %hm_f_hm
            if (pGT->getDims() > 2)
            {
                poResult = callOverloadMatrixExp(L"f", pGTResult, pGT);
                continue;
            }

            //check dimension
            if (pGT->getCols() != pGTResult->getCols())
            {
                poRow->killMe();
                if (poResult)
                {
                    poResult->killMe();
                }
                std::wostringstream os;
                os << _W("inconsistent row/column dimensions\n");
                throw ast::ScilabError(os.str(), 999, (*e.lines_get().begin())->location_get());
            }

            // if we concatenate [Double Sparse], transform the Double to Sparse and perform [Sparse Sparse]
            // this avoids to allocate a Double result of size of Double+Sparse and initialize all elements.
            if (pGT->isSparse() && pGTResult->isDouble())
            {
                poResult = new types::Sparse(*pGTResult->getAs<types::Double>());
            }
            else if (pGT->isSparseBool() && pGTResult->isBool()) // [Bool SparseBool] => [SparseBool SparseBool]
            {
                poResult = new types::SparseBool(*pGTResult->getAs<types::Bool>());
            }

            InternalType *p = AddElementToVariable(NULL, poResult, pGTResult->getRows() + pGT->getRows(), pGT->getCols());
            p = AddElementToVariable(p, pGT, pGTResult->getRows(), 0);
            if (poResult != poRow)
            {
                poRow->killMe();
            }
            if (p != poResult)
            {
                poResult->killMe();
                poResult = p;
            }
        }

        if (poResult)
        {
            result_set(poResult);
        }
        else
        {
            result_set(Double::Empty());
        }
    }
    catch (ast::ScilabError error)
    {
        throw error;
    }
}

template<class T>
types::InternalType* RunVisitorT<T>::callOverloadMatrixExp(std::wstring strType, types::InternalType* _paramL, types::InternalType* _paramR)
{
    types::typed_list in;
    types::typed_list out;
    Callable::ReturnValue Ret;

    _paramL->IncreaseRef();
    _paramR->IncreaseRef();

    in.push_back(_paramL);
    in.push_back(_paramR);

    if (_paramR->isGenericType() && _paramR->getAs<types::GenericType>()->getDims() > 2)
    {
        Ret = Overload::call(L"%hm_" + strType + L"_hm", in, 1, out, this);
    }
    else
    {
        Ret = Overload::call(L"%" + _paramL->getAs<List>()->getShortTypeStr() + L"_" + strType + L"_" + _paramR->getAs<List>()->getShortTypeStr(), in, 1, out, this);
    }

    if (Ret != Callable::OK)
    {
        clean_in_out(in, out);
        throw ScilabError();
    }

    clean_in(in, out);

    if (out.empty())
    {
        // TODO: avoid crash if out is empty but must return an error...
        return NULL;
    }

    return out[0];
}
