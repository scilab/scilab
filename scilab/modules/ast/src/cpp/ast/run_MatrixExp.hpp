/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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

//file included in runvisitor.cpp
namespace ast {

/*
    [1,2;3,4] with/without special character $ and :
    */
template<class T>
void RunVisitorT<T>::visitprivate(const MatrixExp &e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    try
    {
        exps_t::const_iterator row;
        exps_t::const_iterator col;
        types::InternalType *poResult = NULL;
        std::list<types::InternalType*> rowList;

        exps_t lines = e.getLines();
        if (lines.size() == 0)
        {
            setResult(types::Double::Empty());
            CoverageInstance::invokeAndStartChrono((void*)&e);
            return;
        }

        //special case for 1x1 matrix
        if (lines.size() == 1)
        {
            exps_t cols = lines[0]->getAs<MatrixLineExp>()->getColumns();
            if (cols.size() == 1)
            {
                setResult(NULL); // Reset value on loop re-start

                cols[0]->accept(*this);
                //manage evstr('//xxx') for example
                if (getResult() == NULL)
                {
                    setResult(types::Double::Empty());
                }
                CoverageInstance::invokeAndStartChrono((void*)&e);
                return;
            }
        }

        //do all [x,x]
        for (row = lines.begin(); row != lines.end(); row++)
        {
            types::InternalType* poRow = NULL;
            exps_t cols = (*row)->getAs<MatrixLineExp>()->getColumns();
            for (col = cols.begin(); col != cols.end(); col++)
            {
                setResult(NULL); // Reset value on loop re-start

                try
                {
                    (*col)->accept(*this);
                }
                catch (const InternalError& error)
                {
                    if (poRow)
                    {
                        poRow->killMe();
                    }
                    if (poResult)
                    {
                        poResult->killMe();
                    }

                    throw error;
                }

                types::InternalType *pIT = getResult();
                if (pIT == NULL)
                {
                    continue;
                }

                //reset result but without delete the value
                clearResultButFirst();

                if (pIT->isImplicitList())
                {
                    types::ImplicitList *pIL = pIT->getAs<types::ImplicitList>();
                    if (pIL->isComputable())
                    {
                        types::InternalType* pIT2 = pIL->extractFullMatrix();
                        pIT->killMe();
                        pIT = pIT2;
                    }
                    else
                    {
                        if (poRow == NULL)
                        {
                            //first loop
                            poRow = pIT;
                        }
                        else
                        {
                            try
                            {
                                poRow = callOverloadMatrixExp(L"c", poRow, pIT);
                            }
                            catch (const InternalError& error)
                            {
                                if (poResult)
                                {
                                    poResult->killMe();
                                }
                                throw error;
                            }
                        }

                        continue;
                    }
                }

                if (pIT->isGenericType() == false)
                {
                    pIT->killMe();
                    std::wostringstream os;
                    os << _W("unable to concatenate\n");
                    throw ast::InternalError(os.str(), 999, (*col)->getLocation());
                }

                types::GenericType* pGT = pIT->getAs<types::GenericType>();

                if (poRow == NULL)
                {
                    //first loop
                    if (poResult == NULL && pGT->isDouble() && pGT->getAs<types::Double>()->isEmpty())
                    {
                        pGT->killMe();
                        continue;
                    }

                    if (pGT->isDouble() && pGT->getAs<types::Double>()->isEmpty())
                    {
                        if (poResult && (poResult->isList() || poResult->isStruct()))
                        {
                            //in case of [list(); [], ...]

                            //we don't know what to do with [], keep it as "normal" value and continue process
                            poRow = pGT;
                            continue;
                        }

                        pGT->killMe();
                        continue;
                    }

                    poRow = pGT;
                    continue;
                }

                //manage overload on list/struct/implicitlist and hypermatrix before management of []
                if (pGT->isList() || poRow->isList() || pGT->isStruct() || poRow->isStruct() || poRow->isImplicitList() || pGT->getDims() > 2)
                {
                    try
                    {
                        poRow = callOverloadMatrixExp(L"c", poRow, pGT);
                    }
                    catch (const InternalError& error)
                    {
                        if (poResult)
                        {
                            poResult->killMe();
                        }
                        throw error;
                    }

                    continue;
                }

                if (pGT->isDouble() && pGT->getAs<types::Double>()->isEmpty())
                {
                    pGT->killMe();
                    continue;
                }

                types::GenericType* pGTResult = poRow->getAs<types::GenericType>();

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
                    throw ast::InternalError(os.str(), 999, (*row)->getLocation());
                }

                // if we concatenate [Double Sparse], transform the Double to Sparse and perform [Sparse Sparse]
                // this avoids to allocate a Double result of size of Double+Sparse and initialize all elements.
                if (pGT->isSparse() && pGTResult->isDouble())
                {
                    poRow = new types::Sparse(*pGTResult->getAs<types::Double>());
                    pGTResult->killMe();
                    pGTResult = poRow->getAs<types::GenericType>();
                }
                else if (pGT->isSparseBool() && pGTResult->isBool()) // [Bool SparseBool] => [SparseBool SparseBool]
                {
                    poRow = new types::SparseBool(*pGTResult->getAs<types::Bool>());
                    pGTResult->killMe();
                    pGTResult = poRow->getAs<types::GenericType>();
                }
                else if (pGT->isDollar() && pGTResult->isDouble())
                {
                    int _iRows = pGTResult->getRows();
                    int _iCols = pGTResult->getCols();
                    int* piRank = new int[_iRows * _iCols];
                    memset(piRank, 0x00, _iRows * _iCols * sizeof(int));
                    poRow = new types::Polynom(pGT->getAs<types::Polynom>()->getVariableName(), _iRows, _iCols, piRank);
                    types::Polynom* pP = poRow->getAs<types::Polynom>();
                    types::SinglePoly** pSS = pP->get();
                    types::Double* pDb = pGTResult->getAs<types::Double>();
                    double* pdblR = pDb->get();
                    if (pDb->isComplex())
                    {
                        double* pdblI = pDb->getImg();
                        pP->setComplex(true);
                        for (int i = 0; i < pDb->getSize(); i++)
                        {
                            pSS[i]->setRank(0);
                            pSS[i]->setCoef(pdblR + i, pdblI + i);
                        }
                    }
                    else
                    {
                        for (int i = 0; i < pDb->getSize(); i++)
                        {
                            pSS[i]->setRank(0);
                            pSS[i]->setCoef(pdblR + i, NULL);
                        }
                    }

                    delete[] piRank;
                }

                types::InternalType *pNewSize = AddElementToVariable(NULL, poRow, pGTResult->getRows(), pGTResult->getCols() + pGT->getCols());
                types::InternalType* p = AddElementToVariable(pNewSize, pGT, 0, pGTResult->getCols());
                if (p != pNewSize)
                {
                    pNewSize->killMe();
                }
                // call overload
                if (p == NULL)
                {
                    try
                    {
                        poRow = callOverloadMatrixExp(L"c", pGTResult, pGT);
                    }
                    catch (const InternalError& error)
                    {
                        if (poResult)
                        {
                            poResult->killMe();
                        }
                        throw error;
                    }
                    continue;
                }

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

            if (poResult == NULL)
            {
                poResult = poRow;
                continue;
            }

            // management of concatenation with 1:$
            if (poRow->isImplicitList() || poResult->isImplicitList())
            {
                try
                {
                    poResult = callOverloadMatrixExp(L"f", poResult, poRow);
                }
                catch (const InternalError& error)
                {
                    throw error;
                }
                continue;
            }

            types::GenericType* pGT = poRow->getAs<types::GenericType>();

            //check dimension
            types::GenericType* pGTResult = poResult->getAs<types::GenericType>();

            if (pGT->isList() || pGTResult->isList() || pGT->isStruct() || pGTResult->isStruct() || pGT->getDims() > 2)
            {
                try
                {
                    poResult = callOverloadMatrixExp(L"f", pGTResult, pGT);
                }
                catch (const InternalError& error)
                {
                    throw error;
                }

                continue;
            }
            else
            {//[]
                if (pGT->isDouble() && pGT->getAs<types::Double>()->isEmpty())
                {
                    pGT->killMe();
                    continue;
                }
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
                throw ast::InternalError(os.str(), 999, (*e.getLines().begin())->getLocation());
            }

            // if we concatenate [Double Sparse], transform the Double to Sparse and perform [Sparse Sparse]
            // this avoids to allocate a Double result of size of Double+Sparse and initialize all elements.
            if (pGT->isSparse() && pGTResult->isDouble())
            {
                poResult = new types::Sparse(*pGTResult->getAs<types::Double>());
                pGTResult->killMe();
                pGTResult = poResult->getAs<types::GenericType>();
            }
            else if (pGT->isSparseBool() && pGTResult->isBool()) // [Bool SparseBool] => [SparseBool SparseBool]
            {
                poResult = new types::SparseBool(*pGTResult->getAs<types::Bool>());
                pGTResult->killMe();
                pGTResult = poResult->getAs<types::GenericType>();
            }

            types::InternalType* pNewSize = AddElementToVariable(NULL, poResult, pGTResult->getRows() + pGT->getRows(), pGT->getCols());
            types::InternalType* p = AddElementToVariable(pNewSize, pGT, pGTResult->getRows(), 0);
            if (p != pNewSize)
            {
                pNewSize->killMe();
            }

            // call overload
            if (p == NULL)
            {
                try
                {
                    poResult = callOverloadMatrixExp(L"f", pGTResult, pGT);
                }
                catch (const InternalError& error)
                {
                    throw error;
                }
                continue;
            }

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
            setResult(poResult);
        }
        else
        {
            setResult(types::Double::Empty());
        }
    }
    catch (const InternalError& error)
    {
        setResult(NULL);
        CoverageInstance::invokeAndStartChrono((void*)&e);
        throw error;
    }
    CoverageInstance::invokeAndStartChrono((void*)&e);
}

template<class T>
types::InternalType* RunVisitorT<T>::callOverloadMatrixExp(const std::wstring& strType, types::InternalType* _paramL, types::InternalType* _paramR)
{
    types::typed_list in;
    types::typed_list out;
    types::Callable::ReturnValue Ret;

    _paramL->IncreaseRef();
    _paramR->IncreaseRef();

    in.push_back(_paramL);
    in.push_back(_paramR);

    try
    {
        if (_paramR->isGenericType() && _paramR->getAs<types::GenericType>()->getDims() > 2)
        {
            Ret = Overload::call(L"%hm_" + strType + L"_hm", in, 1, out, true);
        }
        else
        {
            Ret = Overload::call(L"%" + _paramL->getAs<types::List>()->getShortTypeStr() + L"_" + strType + L"_" + _paramR->getAs<types::List>()->getShortTypeStr(), in, 1, out, true);
        }
    }
    catch (const InternalError& error)
    {
        cleanInOut(in, out);
        throw error;
    }

    if (Ret != types::Callable::OK)
    {
        cleanInOut(in, out);
        throw InternalError(ConfigVariable::getLastErrorMessage());
    }

    cleanIn(in, out);

    if (out.empty())
    {
        // TODO: avoid crash if out is empty but must return an error...
        return NULL;
    }

    return out[0];
}

} /* namespace ast */
