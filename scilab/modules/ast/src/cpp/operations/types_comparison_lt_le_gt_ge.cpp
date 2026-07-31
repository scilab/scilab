/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#include "types_comparison_lt_le_gt_ge.hxx"
#include "double.hxx"
#include "sparse.hxx"
#include "int.hxx"
#include "operations.hxx"

extern "C"
{
#include "localization.h"
#include "charEncoding.h"
}

using namespace types;

InternalType *GenericCompLess(InternalType *_pLeftOperand, InternalType *_pRightOperand, const std::wstring& op)
{
    InternalType *pResult = NULL;

    /*
    ** [] < A TYPE
    */
    if (_pLeftOperand->isDouble() && _pLeftOperand->getAs<Double>()->isEmpty())
    {
        return Double::Empty();
    }

    /*
    ** A TYPE < []
    */
    if (_pRightOperand->isDouble() && _pRightOperand->getAs<Double>()->isEmpty())
    {
        return Double::Empty();
    }

    /*
    ** DOUBLE < DOUBLE
    */
    if (_pLeftOperand->isDouble() && _pRightOperand->isDouble())
    {
        Double *pL = _pLeftOperand->getAs<Double>();
        Double *pR = _pRightOperand->getAs<Double>();

        int iResult = DoubleLessDouble(pL, pR, (Bool**)&pResult);
        if (iResult)
        {
            throw ast::InternalError(errorSameSize(pL, pR, op));
        }

        return pResult;
    }

    /*
    ** SPARSE < SPARSE
    */
    if (_pLeftOperand->isSparse() && _pRightOperand->isSparse())
    {
        Sparse *pL = _pLeftOperand->getAs<Sparse>();
        Sparse *pR = _pRightOperand->getAs<Sparse>();

        int iResult = SparseLessSparse(pL, pR, (SparseBool**)&pResult);
        if (iResult)
        {
            throw ast::InternalError(errorSameSize(pL, pR, op));
        }

        return pResult;
    }

    /*
    ** DOUBLE < SPARSE
    */
    if (_pLeftOperand->isDouble() && _pRightOperand->isSparse())
    {
        Double *pL = _pLeftOperand->getAs<Double>();
        Sparse *pR = _pRightOperand->getAs<Sparse>();

        int iResult = DoubleLessSparse(pL, pR, (SparseBool**)&pResult);
        if (iResult)
        {
            throw ast::InternalError(errorSameSize(pL, pR, op));
        }

        return pResult;
    }

    /*
    ** SPARSE < DOUBLE
    */
    if (_pLeftOperand->isSparse() && _pRightOperand->isDouble())
    {
        Sparse *pL = _pLeftOperand->getAs<Sparse>();
        Double *pR = _pRightOperand->getAs<Double>();

        int iResult = SparseLessDouble(pL, pR, (SparseBool**)&pResult);
        if (iResult)
        {
            throw ast::InternalError(errorSameSize(pL, pR, op));
        }

        return pResult;
    }

    /*
    ** INT < INT
    */
    if (_pLeftOperand->isInt() && _pRightOperand->isInt())
    {
        if (_pLeftOperand->getType() != _pRightOperand->getType())
        {
            //call overload function to convert left or right or both to have comparable type
            return NULL;
        }

        int iResult = IntLessInt(_pLeftOperand, _pRightOperand, (GenericType**)&pResult);
        if (iResult)
        {
            throw ast::InternalError(errorSameSize(_pLeftOperand->getAs<types::GenericType>(), _pRightOperand->getAs<types::GenericType>(), op));
        }

        return pResult;
    }
    return NULL;
}

int DoubleLessDouble(Double* _pDouble1, Double* _pDouble2, Bool** _pOut)
{
    if (_pDouble1->isComplex() || _pDouble2->isComplex())
    {
        //call overload for complex cases
        return 0;
    }

    Bool* pB = NULL;
    if (_pDouble1->isScalar())
    {
        //d < D
        pB = new Bool(_pDouble2->getDims(), _pDouble2->getDimsArray());
        double dblRef	= _pDouble1->get(0);

        for (int i = 0 ; i < pB->getSize() ; i++)
        {
            pB->set(i, dblRef < _pDouble2->get(i));
        }

        *_pOut = pB;
        return 0;
    }
    else if (_pDouble2->isScalar())
    {
        //D < d
        pB = new Bool(_pDouble1->getDims(), _pDouble1->getDimsArray());
        double dblRef	= _pDouble2->get(0);

        for (int i = 0 ; i < pB->getSize() ; i++)
        {
            pB->set(i, _pDouble1->get(i) < dblRef);
        }

        *_pOut = pB;
        return 0;
    }

    //D < D
    if (checkSameSize(_pDouble1, _pDouble2) == false)
    {
        auto plan = makeExpandPlan(_pDouble1, _pDouble2, pB, L"<");

        auto l = _pDouble1->get();
        auto r = _pDouble2->get();
        auto o = pB->get();

        expandApply(plan, [&](int iL, int iR, int iO) {
            o[iO] = l[iL] < r[iR];
        });

        *_pOut = pB;
        return 0;
    }

    pB = new Bool(_pDouble1->getDims(), _pDouble1->getDimsArray());

    for (int i = 0 ; i < pB->getSize() ; i++)
    {
        pB->set(i, _pDouble1->get(i) < _pDouble2->get(i));
    }

    *_pOut = pB;
    return 0;
}

int SparseLessSparse(Sparse* _pSparse1, Sparse* _pSparse2, SparseBool** _pOut)
{
    if (_pSparse1->isComplex() || _pSparse2->isComplex())
    {
        //call overload for complex cases
        return 0;
    }

    if (_pSparse1->isScalar() || _pSparse2->isScalar() || //scalar cases
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


InternalType *GenericCompLessEqual(InternalType *_pLeftOperand, InternalType *_pRightOperand, const std::wstring& op)
{
    InternalType *pResult = NULL;

    /*
    ** [] <= A TYPE
    */
    if (_pLeftOperand->isDouble() && _pLeftOperand->getAs<Double>()->isEmpty())
    {
        return Double::Empty();
    }

    /*
    ** A TYPE <= []
    */
    if (_pRightOperand->isDouble() && _pRightOperand->getAs<Double>()->isEmpty())
    {
        return Double::Empty();
    }

    /*
    ** DOUBLE <= DOUBLE
    */
    if (_pLeftOperand->isDouble() && _pRightOperand->isDouble())
    {
        Double *pL = _pLeftOperand->getAs<Double>();
        Double *pR = _pRightOperand->getAs<Double>();

        int iResult = DoubleLessEqualDouble(pL, pR, (Bool**)&pResult);
        if (iResult)
        {
            throw ast::InternalError(errorSameSize(pL, pR, op));
        }

        return pResult;
    }

    /*
    ** SPARSE <= SPARSE
    */
    if (_pLeftOperand->isSparse() && _pRightOperand->isSparse())
    {
        Sparse *pL = _pLeftOperand->getAs<Sparse>();
        Sparse *pR = _pRightOperand->getAs<Sparse>();

        int iResult = SparseLessEqualSparse(pL, pR, (SparseBool**)&pResult);
        if (iResult)
        {
            throw ast::InternalError(errorSameSize(pL, pR, op));
        }

        return pResult;
    }

    /*
    ** DOUBLE <= SPARSE
    */
    if (_pLeftOperand->isDouble() && _pRightOperand->isSparse())
    {
        Double *pL = _pLeftOperand->getAs<Double>();
        Sparse *pR = _pRightOperand->getAs<Sparse>();

        int iResult = DoubleLessEqualSparse(pL, pR, (SparseBool**)&pResult);
        if (iResult)
        {
            throw ast::InternalError(errorSameSize(pL, pR, op));
        }

        return pResult;
    }

    /*
    ** SPARSE <= DOUBLE
    */
    if (_pLeftOperand->isSparse() && _pRightOperand->isDouble())
    {
        Sparse *pL = _pLeftOperand->getAs<Sparse>();
        Double *pR = _pRightOperand->getAs<Double>();

        int iResult = SparseLessEqualDouble(pL, pR, (SparseBool**)&pResult);
        if (iResult)
        {
            throw ast::InternalError(errorSameSize(pL, pR, op));
        }

        return pResult;
    }

    /*
    ** INT <= INT
    */
    if (_pLeftOperand->isInt() && _pRightOperand->isInt())
    {
        if (_pLeftOperand->getType() != _pRightOperand->getType())
        {
            //call overload function to convert left or right or both to have comparable type
            return NULL;
        }

        int iResult = IntLessEqualInt(_pLeftOperand, _pRightOperand, (GenericType**)&pResult);
        if (iResult)
        {
            throw ast::InternalError(errorSameSize(_pLeftOperand->getAs<types::GenericType>(), _pRightOperand->getAs<types::GenericType>(), op));
        }

        return pResult;
    }
    return NULL;
}

int DoubleLessEqualDouble(Double* _pDouble1, Double* _pDouble2, Bool** _pOut)
{
    if (_pDouble1->isComplex() || _pDouble2->isComplex())
    {
        //call overload for complex cases
        return 0;
    }

    Bool* pB = NULL;
    if (_pDouble1->isScalar())
    {
        //d <= D
        pB = new Bool(_pDouble2->getDims(), _pDouble2->getDimsArray());
        double dblRef	= _pDouble1->get(0);

        for (int i = 0 ; i < pB->getSize() ; i++)
        {
            pB->set(i, dblRef <= _pDouble2->get(i));
        }

        *_pOut = pB;
        return 0;
    }
    else if (_pDouble2->isScalar())
    {
        //D <= d
        pB = new Bool(_pDouble1->getDims(), _pDouble1->getDimsArray());
        double dblRef	= _pDouble2->get(0);

        for (int i = 0 ; i < pB->getSize() ; i++)
        {
            pB->set(i, _pDouble1->get(i) <= dblRef);
        }

        *_pOut = pB;
        return 0;
    }

    //D <= D
    if (checkSameSize(_pDouble1, _pDouble2) == false)
    {
        if (canExpandSize(_pDouble1, _pDouble2) == false)
        {
            return 1;
        }

        auto plan = makeExpandPlan(_pDouble1, _pDouble2, pB, L"<=");

        auto l = _pDouble1->get();
        auto r = _pDouble2->get();
        auto o = pB->get();

        expandApply(plan, [&](int iL, int iR, int iO) {
            o[iO] = l[iL] <= r[iR];
        });

        *_pOut = pB;
        return 0;
    }

    pB = new Bool(_pDouble1->getDims(), _pDouble1->getDimsArray());

    for (int i = 0 ; i < pB->getSize() ; i++)
    {
        pB->set(i, _pDouble1->get(i) <= _pDouble2->get(i));
    }

    *_pOut = pB;
    return 0;
}

int SparseLessEqualSparse(Sparse* _pSparse1, Sparse* _pSparse2, SparseBool** _pOut)
{
    if (_pSparse1->isComplex() || _pSparse2->isComplex())
    {
        //call overload for complex cases
        return 0;
    }

    if (_pSparse1->isScalar() || _pSparse2->isScalar() || //scalar cases
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
    return GenericCompLess(_pRightOperand, _pLeftOperand, L">");
}

InternalType *GenericGreaterEqual(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    return GenericCompLessEqual(_pRightOperand, _pLeftOperand, L">=");
}

InternalType *GenericLess(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    return GenericCompLess(_pLeftOperand, _pRightOperand, L"<");
}

InternalType *GenericLessEqual(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    return GenericCompLessEqual(_pLeftOperand, _pRightOperand, L"<=");
}

template <class T>
static int IntLessInt(T* _pL, T* _pR, types::GenericType** _pOut)
{
    if (_pL->isScalar())
    {
        Bool *pB = new Bool(_pR->getDims(), _pR->getDimsArray());

        int* pb = pB->get();
        const typename T::type x = _pL->get(0);

        for (int i = 0 ; i < pB->getSize() ; i++)
        {
            pb[i] = x < _pR->get(i);
        }

        *_pOut = pB;
        return 0;
    }

    if (_pR->isScalar())
    {
        Bool *pB = new Bool(_pL->getDims(), _pL->getDimsArray());

        int* pb = pB->get();
        const typename T::type x = _pR->get(0);

        for (int i = 0 ; i < pB->getSize() ; i++)
        {
            pb[i] = _pL->get(i) < x;
        }

        *_pOut = pB;
        return 0;
    }

    if (checkSameSize(_pL, _pR) == false)
    {
        if (canExpandSize(_pL, _pR) == false)
        {
            return 1;
        }

        Bool* pB = nullptr;
        auto plan = makeExpandPlan(_pL, _pR, pB, L"<");

        auto l = _pL->get();
        auto r = _pR->get();
        auto o = pB->get();

        expandApply(plan, [&](int iL, int iR, int iO) {
            o[iO] = l[iL] < r[iR];
        });

        *_pOut = pB;
        return 0;
    }

    Bool* pB = new Bool(_pR->getDims(), _pR->getDimsArray());
    for (int i = 0 ; i < _pL->getSize() ; i++)
    {
        pB->set(i, _pL->get(i) < _pR->get(i));
    }

    *_pOut = pB;
    return 0;
}

int IntLessInt(types::InternalType* _pL, types::InternalType*  _pR, types::GenericType** _pOut)
{
    switch (_pL->getType())
    {
        case InternalType::ScilabInt8 :
        {
            return IntLessInt(_pL->getAs<Int8>(), _pR->getAs<Int8>(), _pOut);
        }
        case InternalType::ScilabUInt8 :
        {
            return IntLessInt(_pL->getAs<UInt8>(), _pR->getAs<UInt8>(), _pOut);
        }
        case InternalType::ScilabInt16 :
        {
            return IntLessInt(_pL->getAs<Int16>(), _pR->getAs<Int16>(), _pOut);
        }
        case InternalType::ScilabUInt16 :
        {
            return IntLessInt(_pL->getAs<UInt16>(), _pR->getAs<UInt16>(), _pOut);
        }
        case InternalType::ScilabInt32 :
        {
            return IntLessInt(_pL->getAs<Int32>(), _pR->getAs<Int32>(), _pOut);
        }
        case InternalType::ScilabUInt32 :
        {
            return IntLessInt(_pL->getAs<UInt32>(), _pR->getAs<UInt32>(), _pOut);
        }
        case InternalType::ScilabInt64 :
        {
            return IntLessInt(_pL->getAs<Int64>(), _pR->getAs<Int64>(), _pOut);
        }
        case InternalType::ScilabUInt64 :
        {
            return IntLessInt(_pL->getAs<UInt64>(), _pR->getAs<UInt64>(), _pOut);
        }
        default:
        {
            return 0;
        }
    }
}

template <class T>
static int IntLessEqualInt(T* _pL, T* _pR, types::GenericType** _pOut)
{
    if (_pL->isScalar())
    {
        Bool *pB = new Bool(_pR->getDims(), _pR->getDimsArray());

        int* pb = pB->get();
        const typename T::type x = _pL->get(0);

        for (int i = 0 ; i < pB->getSize() ; i++)
        {
            pb[i] = x <= _pR->get(i);
        }

        *_pOut = pB;
        return 0;
    }

    if (_pR->isScalar())
    {
        Bool *pB = new Bool(_pL->getDims(), _pL->getDimsArray());

        int* pb = pB->get();
        const typename T::type x = _pR->get(0);

        for (int i = 0 ; i < pB->getSize() ; i++)
        {
            pb[i] = _pL->get(i) <= x;
        }

        *_pOut = pB;
        return 0;
    }

    if (checkSameSize(_pL, _pR) == false)
    {
        if (canExpandSize(_pL, _pR) == false)
        {
            return 1;
        }

        Bool* pB = nullptr;
        auto plan = makeExpandPlan(_pL, _pR, pB, L"<=");

        auto l = _pL->get();
        auto r = _pR->get();
        auto o = pB->get();

        expandApply(plan, [&](int iL, int iR, int iO) {
            o[iO] = l[iL] <= r[iR];
        });

        *_pOut = pB;
        return 0;
    }

    Bool* pB = new Bool(_pR->getDims(), _pR->getDimsArray());
    for (int i = 0 ; i < _pL->getSize() ; i++)
    {
        pB->set(i, _pL->get(i) <= _pR->get(i));
    }

    *_pOut = pB;
    return 0;
}

int IntLessEqualInt(types::InternalType* _pL, types::InternalType* _pR, types::GenericType** _pOut)
{
    switch (_pL->getType())
    {
        case InternalType::ScilabInt8 :
        {
            return IntLessEqualInt(_pL->getAs<Int8>(), _pR->getAs<Int8>(), _pOut);
        }
        case InternalType::ScilabUInt8 :
        {
            return IntLessEqualInt(_pL->getAs<UInt8>(), _pR->getAs<UInt8>(), _pOut);
        }
        case InternalType::ScilabInt16 :
        {
            return IntLessEqualInt(_pL->getAs<Int16>(), _pR->getAs<Int16>(), _pOut);
        }
        case InternalType::ScilabUInt16 :
        {
            return IntLessEqualInt(_pL->getAs<UInt16>(), _pR->getAs<UInt16>(), _pOut);
        }
        case InternalType::ScilabInt32 :
        {
            return IntLessEqualInt(_pL->getAs<Int32>(), _pR->getAs<Int32>(), _pOut);
        }
        case InternalType::ScilabUInt32 :
        {
            return IntLessEqualInt(_pL->getAs<UInt32>(), _pR->getAs<UInt32>(), _pOut);
        }
        case InternalType::ScilabInt64 :
        {
            return IntLessEqualInt(_pL->getAs<Int64>(), _pR->getAs<Int64>(), _pOut);
        }
        case InternalType::ScilabUInt64 :
        {
            return IntLessEqualInt(_pL->getAs<UInt64>(), _pR->getAs<UInt64>(), _pOut);
        }
        default:
        {
            return 0;
        }
    }
}
