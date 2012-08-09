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
#include "polynom.hxx"
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
        Double *pL   = _pLeftOperand->getAs<Double>();
        Double *pR   = _pRightOperand->getAs<Double>();

        if(pR->isEmpty() && pL->isEmpty())
        {
            return new Bool(true);
        }
        
        if(pL->isEmpty() || pR->isEmpty())
        {
            return new Bool(false);
        }
        
        if(pR->isScalar())
        {
            Bool* pB = new Bool(pL->getDims(), pL->getDimsArray());
            double dblRef = pR->get(0);
            for(int i = 0 ; i < pL->getSize() ; i++)
            {
                pB->getAs<Bool>()->set(i, pL->get(i) == dblRef);
            }
            return pB;
        }
        
        if(pL->isScalar())
        {
            Bool* pB = new Bool(pR->getDims(), pR->getDimsArray());
            double dblRef = pL->get(0);
            for(int i = 0 ; i < pR->getSize() ; i++)
            {
                pB->getAs<Bool>()->set(i, dblRef == pR->get(i));
            }

            return pB;
        }
        

        if(pL->getDims() != pR->getDims())
        {
            return new Bool(false);
        }

        int* piDimsL = pL->getDimsArray();
        int* piDimsR = pR->getDimsArray();

        for(int i = 0 ; i < pL->getDims() ; i++)
        {
            if(piDimsL[i] != piDimsR[i])
            {
                return new Bool(false);
            }
        }

        Bool* pB = new Bool(pR->getDims(), pR->getDimsArray());
        for(int i = 0 ; i < pL->getSize() ; i++)
        {
            pB->set(i, pL->get(i) == pR->get(i));
        }

        return pB;
    }

    /*
    ** STRING == STRING
    */
    if(TypeL == GenericType::RealString && TypeR == GenericType::RealString)
    {
        String *pL   = _pLeftOperand->getAs<String>();
        String *pR   = _pRightOperand->getAs<String>();

        if(pL->isScalar())
        {
            pResult = new Bool(pR->getDims(), pR->getDimsArray());

            wchar_t* pstL = pL->get(0);
            for(int i = 0 ; i < pR->getSize() ; i++)
            {
                pResult->getAs<Bool>()->set(i, wcscmp(pstL, pR->get(i)) == 0);
            }

            return pResult;
        }

        if(pR->isScalar())
        {
            pResult = new Bool(pL->getDims(), pL->getDimsArray());

            wchar_t* pstR = pR->get(0);
            for(int i = 0 ; i < pL->getSize() ; i++)
            {
                pResult->getAs<Bool>()->set(i, wcscmp(pL->get(i), pstR) == 0);
            }

            return pResult;
        }

        int iDims1 = pL->getDims();
        int iDims2 = pR->getDims();

        if(iDims1 != iDims2)
        {
            return new Bool(false);
        }

        int* piDims1 = pL->getDimsArray();
        int* piDims2 = pR->getDimsArray();

        for(int i = 0 ; i < iDims1 ; i++)
        {
            if(piDims1[i] != piDims2[i])
            {
                return new Bool(false);
            }
        }

        pResult = new Bool(pL->getDims(), pL->getDimsArray());

        for(int i = 0 ; i < pL->getSize() ; i++)
        {
            pResult->getAs<Bool>()->set(i, wcscmp(pL->get(i), pR->get(i)) == 0);
        }
        return pResult;
    }

    /*
    ** BOOL == BOOL
    */
    if(TypeL == GenericType::RealBool && TypeR == GenericType::RealBool)
    {
        Bool *pL   = _pLeftOperand->getAs<Bool>();
        Bool *pR   = _pRightOperand->getAs<Bool>();

        if(pL->isScalar())
        {
            pResult = new Bool(pR->getDims(), pR->getDimsArray());

            int iL = pL->get(0);
            for(int i = 0 ; i < pR->getSize() ; i++)
            {
                pResult->getAs<Bool>()->set(i, iL == pR->get(i));
            }
            return pResult;
        }
        
        if(pR->isScalar())
        {
            pResult = new Bool(pL->getDims(), pL->getDimsArray());

            int iR = pR->get(0);
            for(int i = 0 ; i < pL->getSize() ; i++)
            {
                pResult->getAs<Bool>()->set(i, iR == pL->get(i));
            }
            return pResult;
        }
        
        
        int iDims1 = pL->getDims();
        int iDims2 = pR->getDims();

        if(iDims1 != iDims2)
        {
            return new Bool(false);
        }

        int* piDims1 = pL->getDimsArray();
        int* piDims2 = pR->getDimsArray();

        for(int i = 0 ; i < iDims1 ; i++)
        {
            if(piDims1[i] != piDims2[i])
            {
                return new Bool(false);
            }
        }

        pResult = new Bool(pL->getDims(), pL->getDimsArray());

        for(int i = 0 ; i < pL->getSize() ; i++)
        {
            pResult->getAs<Bool>()->set(i, pL->get(i) == pR->get(i));
        }
        return pResult;
    }

    /*
    ** POLY == POLY
    */
    if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealPoly)
    {
        Polynom *pL   = _pLeftOperand->getAs<Polynom>();
        Polynom *pR   = _pRightOperand->getAs<Polynom>();

        if(pL->isScalar())
        {
            pResult = new Bool(pR->getDims(), pR->getDimsArray());

            SinglePoly* pSL = pL->get(0);
            for(int i = 0 ; i < pR->getSize() ; i++)
            {
                pResult->getAs<Bool>()->set(i, *pSL == *pR->get(i));
            }
            return pResult;
        }

        if(pR->isScalar())
        {
            pResult = new Bool(pL->getDims(), pL->getDimsArray());

            SinglePoly* pSR = pR->get(0);
            for(int i = 0 ; i < pL->getSize() ; i++)
            {
                pResult->getAs<Bool>()->set(i, *pSR == *pL->get(i));
            }
            return pResult;
        }

        if(pL->getDims() != pR->getDims())
        {
            return new Bool(false);
        }

        int* piDims1 = pL->getDimsArray();
        int* piDims2 = pR->getDimsArray();

        for(int i = 0 ; i < pL->getDims() ; i++)
        {
            if(piDims1[i] != piDims2[i])
            {
                return new Bool(false);
            }
        }

        pResult = new Bool(pL->getDims(), pL->getDimsArray());

        for(int i = 0 ; i < pL->getSize() ; i++)
        {
            pResult->getAs<Bool>()->set(i, *pL->get(i) == *pR->get(i));
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

        if(pLL->getSize() == 0 && pLR->getSize() == 0)
        {//list() == list() -> return true
            return new Bool(true);
        }

        Bool* pB = new Bool(1, pLL->getSize());
        for(int i = 0 ; i < pLL->getSize() ; i++)
        {
            pB->set(i, *pLL->get(i) == *pLR->get(i));
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
    if((_pSparse1->getRows() == _pSparse2->getRows() && _pSparse1->getCols() == _pSparse2->getCols()) //matrix case
        || _pSparse1->isScalar() || _pSparse2->isScalar()) //scalar cases
    {
        *_pOut = _pSparse1->newEqualTo(*_pSparse2);
        return 0;
    }

    return 1;
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
