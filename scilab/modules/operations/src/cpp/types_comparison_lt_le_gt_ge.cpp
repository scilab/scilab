/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_comparison_lt_le_gt_ge.hxx"
#include "double.hxx"
#include "sparse.hxx"

#include "scilabexception.hxx"

extern "C"
{
#include "localization.h"
#include "charEncoding.h"
}

using namespace types;

InternalType *GenericLess(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;

    /*
    ** DOUBLE < DOUBLE
    */
    if(_pLeftOperand->isDouble() && _pRightOperand->isDouble())
    {
        Double *pL = _pLeftOperand->getAs<Double>();
        Double *pR = _pRightOperand->getAs<Double>();

        int iResult = DoubleLessDouble(pL, pR, (Bool**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** SPARSE < SPARSE
    */
    if(_pLeftOperand->isSparse() && _pRightOperand->isSparse())
    {
        Sparse *pL = _pLeftOperand->getAs<Sparse>();
        Sparse *pR = _pRightOperand->getAs<Sparse>();

        int iResult = SparseLessSparse(pL, pR, (SparseBool**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** DOUBLE < SPARSE
    */
    if(_pLeftOperand->isDouble() && _pRightOperand->isSparse())
    {
        Double *pL = _pLeftOperand->getAs<Double>();
        Sparse *pR = _pRightOperand->getAs<Sparse>();

        int iResult = DoubleLessSparse(pL, pR, (SparseBool**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** SPARSE < DOUBLE
    */
    if(_pLeftOperand->isSparse() && _pRightOperand->isDouble())
    {
        Sparse *pL = _pLeftOperand->getAs<Sparse>();
        Double *pR = _pRightOperand->getAs<Double>();

        int iResult = SparseLessDouble(pL, pR, (SparseBool**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }
    return NULL;
}

int DoubleLessDouble(Double* _pDouble1, Double* _pDouble2, Bool** _pOut)
{
    if(_pDouble1->isComplex() || _pDouble2->isComplex())
    {//call overload for complex cases
        return 0;
    }

    Bool* pB = NULL;
    if(_pDouble1->isScalar())
    {//d < D
        pB = new Bool(_pDouble2->getDims(), _pDouble2->getDimsArray());
        double dblRef	= _pDouble1->get(0);

        for(int i = 0 ; i < pB->getSize() ; i++)
        {
            pB->set(i, dblRef < _pDouble2->get(i));
        }

        *_pOut = pB;
        return 0;
    }
    else if(_pDouble2->isScalar())
    {//D < d
        pB = new Bool(_pDouble1->getDims(), _pDouble1->getDimsArray());
        double dblRef	= _pDouble2->get(0);

        for(int i = 0 ; i < pB->getSize() ; i++)
        {
            pB->set(i, _pDouble1->get(i) < dblRef);
        }

        *_pOut = pB;
        return 0;
    }

    if(_pDouble1->getDims() != _pDouble2->getDims())
    {
        return 1;
    }

    //D < D
    int* piDims1 = _pDouble1->getDimsArray();
    int* piDims2 = _pDouble2->getDimsArray();

    for(int i = 0 ; i < _pDouble1->getDims() ; i++)
    {
        if(piDims1[i] != piDims2[i])
        {
            return 1;
        }
    }

    pB = new Bool(_pDouble1->getDims(), _pDouble1->getDimsArray());

    for(int i = 0 ; i < pB->getSize() ; i++)
    {
        pB->set(i, _pDouble1->get(i) < _pDouble2->get(i));
    }

    *_pOut = pB;
    return 0;
}

int SparseLessSparse(Sparse* _pSparse1, Sparse* _pSparse2, SparseBool** _pOut)
{
    if(_pSparse1->isComplex() || _pSparse2->isComplex())
    {//call overload for complex cases
        return 0;
    }

    if(_pSparse1->isScalar() || _pSparse2->isScalar() || //scalar cases
        (_pSparse1->getRows() == _pSparse2->getRows() && _pSparse1->getCols() == _pSparse2->getCols())) //matrix case
    {
        *_pOut = _pSparse1->newLessThan(*_pSparse2);
        return 0;
    }

    *_pOut = NULL;
    return 1;
}

int DoubleLessSparse(Double* _pDouble, Sparse* _pSparse, SparseBool** _pOut)
{
    Sparse* pSparse = new Sparse(*_pDouble);
    int iRet = SparseLessSparse(pSparse, _pSparse, _pOut);
    delete pSparse;
    return iRet;
}

int SparseLessDouble(Sparse* _pSparse, Double* _pDouble, SparseBool** _pOut)
{
    Sparse* pSparse = new Sparse(*_pDouble);
    int iRet = SparseLessSparse(_pSparse, pSparse, _pOut);
    delete pSparse;
    return iRet;
}


InternalType *GenericLessEqual(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;

    /*
    ** DOUBLE < DOUBLE
    */
    if(_pLeftOperand->isDouble() && _pRightOperand->isDouble())
    {
        Double *pL = _pLeftOperand->getAs<Double>();
        Double *pR = _pRightOperand->getAs<Double>();

        int iResult = DoubleLessEqualDouble(pL, pR, (Bool**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** SPARSE < SPARSE
    */
    if(_pLeftOperand->isSparse() && _pRightOperand->isSparse())
    {
        Sparse *pL = _pLeftOperand->getAs<Sparse>();
        Sparse *pR = _pRightOperand->getAs<Sparse>();

        int iResult = SparseLessEqualSparse(pL, pR, (SparseBool**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** DOUBLE < SPARSE
    */
    if(_pLeftOperand->isDouble() && _pRightOperand->isSparse())
    {
        Double *pL = _pLeftOperand->getAs<Double>();
        Sparse *pR = _pRightOperand->getAs<Sparse>();

        int iResult = DoubleLessEqualSparse(pL, pR, (SparseBool**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** SPARSE < DOUBLE
    */
    if(_pLeftOperand->isSparse() && _pRightOperand->isDouble())
    {
        Sparse *pL = _pLeftOperand->getAs<Sparse>();
        Double *pR = _pRightOperand->getAs<Double>();

        int iResult = SparseLessEqualDouble(pL, pR, (SparseBool**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }
    return NULL;
}

int DoubleLessEqualDouble(Double* _pDouble1, Double* _pDouble2, Bool** _pOut)
{
    if(_pDouble1->isComplex() || _pDouble2->isComplex())
    {//call overload for complex cases
        return 0;
    }

    Bool* pB = NULL;
    if(_pDouble1->isScalar())
    {//d <= D
        pB = new Bool(_pDouble2->getDims(), _pDouble2->getDimsArray());
        double dblRef	= _pDouble1->get(0);

        for(int i = 0 ; i < pB->getSize() ; i++)
        {
            pB->set(i, dblRef <= _pDouble2->get(i));
        }

        *_pOut = pB;
        return 0;
    }
    else if(_pDouble2->isScalar())
    {//D <= d
        pB = new Bool(_pDouble1->getDims(), _pDouble1->getDimsArray());
        double dblRef	= _pDouble2->get(0);

        for(int i = 0 ; i < pB->getSize() ; i++)
        {
            pB->set(i, _pDouble1->get(i) <= dblRef);
        }

        *_pOut = pB;
        return 0;
    }

    if(_pDouble1->getDims() != _pDouble2->getDims())
    {
        return 1;
    }

    //D <= D
    int* piDims1 = _pDouble1->getDimsArray();
    int* piDims2 = _pDouble2->getDimsArray();

    for(int i = 0 ; i < _pDouble1->getDims() ; i++)
    {
        if(piDims1[i] != piDims2[i])
        {
            return 1;
        }
    }

    pB = new Bool(_pDouble1->getDims(), _pDouble1->getDimsArray());

    for(int i = 0 ; i < pB->getSize() ; i++)
    {
        pB->set(i, _pDouble1->get(i) <= _pDouble2->get(i));
    }

    *_pOut = pB;
    return 0;
}

int SparseLessEqualSparse(Sparse* _pSparse1, Sparse* _pSparse2, SparseBool** _pOut)
{
    if(_pSparse1->isComplex() || _pSparse2->isComplex())
    {//call overload for complex cases
        return 0;
    }

    if(_pSparse1->isScalar() || _pSparse2->isScalar() || //scalar cases
        (_pSparse1->getRows() == _pSparse2->getRows() && _pSparse1->getCols() == _pSparse2->getCols())) //matrix case
    {
        *_pOut = _pSparse1->newLessOrEqual(*_pSparse2);
        return 0;
    }

    *_pOut = NULL;
    return 1;
}

int DoubleLessEqualSparse(Double* _pDouble, Sparse* _pSparse, SparseBool** _pOut)
{
    Sparse* pSparse = new Sparse(*_pDouble);
    int iRet = SparseLessEqualSparse(pSparse, _pSparse, _pOut);
    delete pSparse;
    return iRet;
}

int SparseLessEqualDouble(Sparse* _pSparse, Double* _pDouble, SparseBool** _pOut)
{
    Sparse* pSparse = new Sparse(*_pDouble);
    int iRet = SparseLessEqualSparse(_pSparse, pSparse, _pOut);
    delete pSparse;
    return iRet;
}

InternalType *GenericGreater(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    return GenericLess(_pRightOperand, _pLeftOperand);
}

InternalType *GenericGreaterEqual(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    return GenericLessEqual(_pRightOperand, _pLeftOperand);
}
