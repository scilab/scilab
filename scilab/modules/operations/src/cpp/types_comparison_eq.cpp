/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_comparison_eq.hxx"
#include "bool.hxx"
#include "double.hxx"
#include "string.hxx"
#include "list.hxx"
#include "cell.hxx"
#include "sparse.hxx"

using namespace types;

InternalType *GenericComparisonEqual(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    GenericType::RealType TypeL = _pLeftOperand->getType();
    GenericType::RealType TypeR = _pRightOperand->getType();
    InternalType *pResult = NULL;

    /*
    ** [] == ??
    */
    if(TypeL == GenericType::RealDouble && _pLeftOperand->getAs<Double>()->getSize() == 0)
    {
        if(TypeR != InternalType::RealDouble)
        {
            return (new Bool(false));
        }
    }

    /*
    ** ?? == []
    */
    if(TypeR == GenericType::RealDouble && _pRightOperand->getAs<Double>()->getSize() == 0)
    {
        if(TypeL != InternalType::RealDouble)
        {
            return(new Bool(false));
        }
    }

    /*
    ** DOUBLE == DOUBLE
    */
    if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
    {
        Double *pL			= _pLeftOperand->getAs<Double>();
        Double *pR			= _pRightOperand->getAs<Double>();

        if(pR->getSize() == 0 && pL->getSize() == 0)
        {
            return new Bool(true);
        }
        else if(pL->getSize() == 0  || pR->getSize() == 0)
        {
            return new Bool(false);
        }
        else if(pR->getSize() == 1)
        {
            Bool* pB = new Bool(pL->getDims(), pL->getDimsArray());
            double dblRef	= pR->get(0);
            for(int i = 0 ; i < pL->getSize() ; i++)
            {
                pB->getAs<Bool>()->set(i, pL->get(i) == dblRef);
            }
            pResult = pB;
        }
        else if(pL->getSize() == 1)
        {
            Bool* pB = new Bool(pR->getDims(), pR->getDimsArray());
            double dblRef	= pL->get(0);
            for(int i = 0 ; i < pR->getSize() ; i++)
            {
                pB->getAs<Bool>()->set(i, dblRef == pR->get(i));
            }

            pResult = pB;
        }
        else
        {
            if(pL->getDims() != pR->getDims())
            {
                return new Bool(false);
            }
            else
            {
                int* piDimsL = pL->getDimsArray();
                int* piDimsR = pR->getDimsArray();

                for(int i = 0 ; i < pL->getDims() ; i++)
                {
                    if(piDimsL[i] != piDimsR[i])
                    {
                        return new Bool(false);
                    }
                }

                Bool* pB = new Bool(pR->getRows(), pR->getCols());
                for(int i = 0 ; i < pL->getSize() ; i++)
                {
                    pB->set(i, pL->get(i) == pR->get(i));
                }

                pResult = pB;
            }
        }
        return pResult;
    }

    /*
    ** STRING == STRING
    */
    if(TypeL == GenericType::RealString && TypeR == GenericType::RealString)
    {
        String *pL			= _pLeftOperand->getAs<String>();
        String *pR			= _pRightOperand->getAs<String>();

        if(pL->getSize() == 1)
        {
            pResult = new Bool(pR->getRows(), pR->getCols());

            wchar_t* pstL = pL->get()[0];
            for(int i = 0 ; i < pR->getRows() ; i++)
            {
                for(int j = 0 ; j < pR->getCols() ; j++)
                {
                    wchar_t* pstR = pR->get(i,j);
                    if(wcscmp(pstL, pstR) == 0)
                    {
                        pResult->getAs<Bool>()->set(i,j,true);
                    }
                    else
                    {
                        pResult->getAs<Bool>()->set(i,j,false);
                    }
                }
            }
        }
        else if(pR->getSize() == 1)
        {
            pResult = new Bool(pL->getRows(), pL->getCols());

            wchar_t* pstR = pR->get()[0];
            for(int i = 0 ; i < pL->getRows() ; i++)
            {
                for(int j = 0 ; j < pL->getCols() ; j++)
                {
                    wchar_t* pstL = pL->get(i,j);
                    if(wcscmp(pstL, pstR) == 0)
                    {
                        pResult->getAs<Bool>()->set(i,j,true);
                    }
                    else
                    {
                        pResult->getAs<Bool>()->set(i,j,false);
                    }
                }
            }
        }
        else if(pL->getRows() == pR->getRows() && pL->getCols() == pR->getCols())
        {
            pResult = new Bool(pL->getRows(), pL->getCols());

            for(int i = 0 ; i < pL->getRows() ; i++)
            {
                for(int j = 0 ; j < pL->getCols() ; j++)
                {
                    wchar_t* pstR = pR->get(i,j);
                    wchar_t* pstL = pL->get(i,j);
                    if(wcscmp(pstL, pstR) == 0)
                    {
                        pResult->getAs<Bool>()->set(i,j,true);
                    }
                    else
                    {
                        pResult->getAs<Bool>()->set(i,j,false);
                    }
                }
            }
        }
        else
        {
            pResult = new Bool(false);
        }
        return pResult;
    }

    /*
    ** BOOL == BOOL
    */
    if(TypeL == GenericType::RealBool && TypeR == GenericType::RealBool)
    {
        Bool *pL			= _pLeftOperand->getAs<Bool>();
        Bool *pR			= _pRightOperand->getAs<Bool>();

        if(pL->getSize() == 1)
        {
            pResult = new Bool(pR->getRows(), pR->getCols());

            int iL = pL->get()[0];
            for(int i = 0 ; i < pR->getRows() ; i++)
            {
                for(int j = 0 ; j < pR->getCols() ; j++)
                {
                    if(iL == pR->get(i,j))
                    {
                        pResult->getAs<Bool>()->set(i,j,true);
                    }
                    else
                    {
                        pResult->getAs<Bool>()->set(i,j,false);
                    }
                }
            }
        }
        else if(pR->getSize() == 1)
        {
            pResult = new Bool(pL->getRows(), pL->getCols());

            int iR = pR->get()[0];
            for(int i = 0 ; i < pL->getRows() ; i++)
            {
                for(int j = 0 ; j < pL->getCols() ; j++)
                {
                    if(iR == pL->get(i,j))
                    {
                        pResult->getAs<Bool>()->set(i,j,true);
                    }
                    else
                    {
                        pResult->getAs<Bool>()->set(i,j,false);
                    }
                }
            }
        }
        else if(pL->getRows() == pR->getRows() && pL->getCols() == pR->getCols())
        {
            pResult = new Bool(pL->getRows(), pL->getCols());

            for(int i = 0 ; i < pL->getRows() ; i++)
            {
                for(int j = 0 ; j < pL->getCols() ; j++)
                {
                    if(pL->get(i,j) == pR->get(i,j))
                    {
                        pResult->getAs<Bool>()->set(i,j,true);
                    }
                    else
                    {
                        pResult->getAs<Bool>()->set(i,j,false);
                    }
                }
            }
        }
        else
        {
            pResult = new Bool(false);
        }
        return pResult;
    }

    /*
    ** LIST == LIST
    */
    if(TypeL == GenericType::RealList && TypeR == GenericType::RealList)
    {
        List* pLL = _pLeftOperand->getAs<List>();
        List* pLR = _pRightOperand->getAs<List>();

        if(pLL->getSize() != pLR->getSize())
        {
            return new Bool(false);
        }

        if(pLL->getSize() == 0)
        {//list() == list() -> return true
            return new Bool(true);
        }

        Bool* pB = new Bool(1, pLL->getSize());
        for(int i = 0 ; i < pLL->getSize() ; i++)
        {
            if(*pLL->get(i) == *pLR->get(i))
            {
                pB->set(i, true);
            }
            else
            {
                pB->set(i, false);
            }
        }
        return pB;
    }

    /*
    ** CELL == CELL
    */
    if(TypeL == GenericType::RealCell && TypeR == GenericType::RealCell)
    {
        Cell* pCL = _pLeftOperand->getAs<Cell>();
        Cell* pCR = _pRightOperand->getAs<Cell>();

        /* check dimension*/
        if(pCL->getDims() != pCR->getDims())
        {
            return new Bool(false);
        }

        int* piDimsL = pCL->getDimsArray();
        int* piDimsR = pCR->getDimsArray();

        for(int i = 0 ; i < pCL->getDims() ; i++)
        {
            if(piDimsL[i] != piDimsR[i])
            {
                return new Bool(false);
            }
        }

        if(pCL->getSize() == 0)
        {//{} == {} -> return true
            return new Bool(true);
        }

        Bool *pB = new Bool(pCL->getDims(), piDimsL);
        for(int i = 0 ; i < pCL->getSize() ; i++)
        {
            pB->set(i, *pCL->get(i) == *pCR->get(i));
        }
        return pB;
    }


    /*
    ** DOUBLE == STRING
    */
    if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealString)
    {
        return new Bool(0);
    }

    /*
    ** STRING == DOUBLE
    */
    if(TypeL == GenericType::RealString && TypeR == GenericType::RealDouble)
    {
        return new Bool(0);;
    }

    /*
    ** SPARSE == SPARSE
    */
    if(TypeL == GenericType::RealSparse && TypeR == GenericType::RealSparse)
    {
        Sparse* pL = _pLeftOperand->getAs<Sparse>();
        Sparse* pR = _pRightOperand->getAs<Sparse>();

        int iResult = EqualToSparseAndSparse(pL, pR, (GenericType**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }
        
    /*
    ** SPARSE == DOUBLE
    */
    if(TypeL == GenericType::RealSparse && TypeR == GenericType::RealDouble)
    {
        Sparse* pL = _pLeftOperand->getAs<Sparse>();
        Double* pR = _pRightOperand->getAs<Double>();

        int iResult = EqualToSparseAndDouble(pL, pR, (GenericType**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** DOUBLE == SPARSE
    */
    if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealSparse)
    {
        Double* pL = _pLeftOperand->getAs<Double>();
        Sparse* pR = _pRightOperand->getAs<Sparse>();

        int iResult = EqualToDoubleAndSparse(pL, pR, (GenericType**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** SPARSE BOOL == SPARSE BOOL
    */
    if(TypeL == GenericType::RealSparseBool && TypeR == GenericType::RealSparseBool)
    {
        SparseBool* pL = _pLeftOperand->getAs<SparseBool>();
        SparseBool* pR = _pRightOperand->getAs<SparseBool>();

        int iResult = EqualToSparseBoolAndSparseBool(pL, pR, (GenericType**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** SPARSE BOOL == BOOL
    */
    if(TypeL == GenericType::RealSparseBool && TypeR == GenericType::RealBool)
    {
        SparseBool* pL = _pLeftOperand->getAs<SparseBool>();
        Bool* pR = _pRightOperand->getAs<Bool>();

        int iResult = EqualToSparseBoolAndBool(pL, pR, (GenericType**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** BOOL == SPARSE BOOL
    */
    if(TypeL == GenericType::RealBool && TypeR == GenericType::RealSparseBool)
    {
        Bool* pL = _pLeftOperand->getAs<Bool>();
        SparseBool* pR = _pRightOperand->getAs<SparseBool>();

        int iResult = EqualToBoolAndSparseBool(pL, pR, (GenericType**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return NULL;
}

int EqualToSparseBoolAndSparseBool(SparseBool* _pSB1, SparseBool* _pSB2, GenericType** _pOut)
{
    SparseBool* pOut = NULL;
    if(_pSB1->isScalar())
    {
        pOut = new SparseBool(_pSB2->getRows(), _pSB2->getCols());
        bool bVal = _pSB1->get(0, 0);
        
        for(int i = 0 ; i < pOut->getRows() ; i++)
        {
            for(int j = 0 ; j < pOut->getCols() ; j++)
            {
                pOut->set(i, j, bVal == _pSB2->get(i,j));
            }
        }

        *_pOut = pOut;
        return 0;
    }

    if(_pSB2->isScalar())
    {
        pOut = new SparseBool(_pSB1->getRows(), _pSB1->getCols());
        bool bVal = _pSB2->get(0, 0);
        
        for(int i = 0 ; i < pOut->getRows() ; i++)
        {
            for(int j = 0 ; j < pOut->getCols() ; j++)
            {
                pOut->set(i, j, _pSB1->get(i,j) == bVal);
            }
        }

        *_pOut = pOut;
        return 0;
    }

    if(_pSB1->getRows() != _pSB2->getRows() || _pSB1->getCols() != _pSB2->getCols())
    {
        return 1;
    }

    pOut = new SparseBool(_pSB1->getRows(), _pSB1->getCols());

    for(int i = 0 ; i < pOut->getRows() ; i++)
    {
        for(int j = 0 ; j < pOut->getCols() ; j++)
        {
            pOut->set(i, j, _pSB1->get(i,j) == _pSB2->get(i,j));
        }
    }

    *_pOut = pOut;
    return 0;
}

int EqualToSparseAndSparse(Sparse* _pSparse1, Sparse* _pSparse2, GenericType** _pOut)
{
    if(_pSparse1->isScalar())
    {//sp == SP
        SparseBool* pSB = new SparseBool(_pSparse2->getRows() , _pSparse2->getCols());

        std::complex<double> dbl = _pSparse1->getImg(0, 0);
        for(int i = 0 ; i < pSB->getRows() ; i++)
        {
            for(int j = 0 ; j < pSB->getCols() ; j++)
            {
                if(_pSparse2->getImg(i, j) == dbl)
                {
                    pSB->set(i, j, true);
                }
            }
        }
        
        *_pOut = pSB;
        return 0;
    }

    if(_pSparse2->isScalar())
    {//SP == sp
        SparseBool* pSB = new SparseBool(_pSparse1->getRows() , _pSparse1->getCols());
        std::complex<double> dbl = _pSparse2->getImg(0, 0);

        for(int i = 0 ; i < pSB->getRows() ; i++)
        {
            for(int j = 0 ; j < pSB->getCols() ; j++)
            {
                if(_pSparse1->getImg(i, j) == dbl)
                {
                    pSB->set(i, j, true);
                }
            }
        }
        
        *_pOut = pSB;
        return 0;
    }

    if(_pSparse1->getRows() != _pSparse2->getRows() || _pSparse1->getCols() != _pSparse2->getCols())
    {
        return 1;
    }

    *_pOut = _pSparse1->newEqualTo(*_pSparse2);
    return 0;
}

int EqualToDoubleAndSparse(Double* _pDouble, Sparse* _pSparse, GenericType** _pOut)
{
    Sparse* pSparse = NULL;
    if(_pDouble->isScalar())
    {
        pSparse = new Sparse(_pSparse->getRows(), _pSparse->getCols(), _pDouble->isComplex());
        if(pSparse->isComplex())
        {
            std::complex<double> dbl(_pDouble->get(0), _pDouble->getImg(0));
            for(int i = 0 ; i < pSparse->getRows() ; i++)
            {
                for(int j = 0 ; j < pSparse->getCols() ; j++)
                {
                    pSparse->set(i, j, dbl);
                }
            }
        }
        else
        {
            double dbl = _pDouble->get(0);
            for(int i = 0 ; i < pSparse->getRows() ; i++)
            {
                for(int j = 0 ; j < pSparse->getCols() ; j++)
                {
                    pSparse->set(i, j, dbl);
                }
            }
        }
    }
    else
    {
        pSparse = new Sparse(*_pDouble);
    }

    int iRet = EqualToSparseAndSparse(pSparse, _pSparse, _pOut);
    delete pSparse;
    return iRet;
}

int EqualToSparseAndDouble(Sparse* _pSparse, Double* _pDouble, GenericType** _pOut)
{
    return EqualToDoubleAndSparse(_pDouble, _pSparse, _pOut);
}

int EqualToSparseBoolAndBool(SparseBool* _pSB1, Bool* _pB2, GenericType** _pOut)
{
    SparseBool* pSB = new SparseBool(*_pB2);
    int iRet = EqualToSparseBoolAndSparseBool(_pSB1, pSB, _pOut);
    delete pSB;
    return iRet;
}

int EqualToBoolAndSparseBool(Bool* _pB1, SparseBool* _pSB2, GenericType** _pOut)
{
    SparseBool* pSB = new SparseBool(*_pB1);
    int iRet = EqualToSparseBoolAndSparseBool(pSB, _pSB2, _pOut);
    delete pSB;
    return iRet;
}
