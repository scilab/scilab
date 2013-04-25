/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
 *  Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C"
{
#include "os_swprintf.h"
}

#include "types_or_and.hxx"
#include "int.hxx"
#include "bool.hxx"
#include "sparse.hxx"


using namespace types;

// &&
InternalType* GenericShortcutAnd(InternalType* _pL)
{
    InternalType* pResult = NULL;
    if (_pL->isBool())
    {
        BoolAndBool(_pL->getAs<Bool>(), (Bool**)&pResult);
    }

    if (_pL->isInt())
    {
        IntAndInt(_pL, (Bool**)&pResult);
    }

    if (_pL->isSparseBool())
    {
        SparseBoolAndSparseBool(_pL, (Bool**)&pResult);
    }

    return pResult;
}

// &
InternalType* GenericLogicalAnd(InternalType* _pL, InternalType* _pR)
{
    InternalType* pResult = NULL;

    if (_pL->isBool() && _pR->isBool())
    {
        Bool *pL = (Bool*)_pL;
        Bool *pR = (Bool*)_pR;

        int iResult = BoolLogicalAndBool(pL, pR, (Bool**)&pResult);
        if (iResult != 0)
        {
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"), L"&", pL->DimToString().c_str(), pR->DimToString().c_str());
            throw ast::ScilabError(pMsg);
        }
        return pResult;
    }

    if (_pL->isInt() && _pR->isInt())
    {
        int iResult = IntLogicalAndInt(_pL, _pR, &pResult);
        if (iResult != 0)
        {
            GenericType* pL = _pL->getAs<GenericType>();
            GenericType* pR = _pR->getAs<GenericType>();
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"), L"&", pL->DimToString().c_str(), pR->DimToString().c_str());
            throw ast::ScilabError(pMsg);
        }
        return pResult;
    }

    if (_pL->isSparseBool() && _pR->isSparseBool())
    {
        int iResult = SparseBoolLogicalAndSparseBool(_pL, _pR, &pResult);
        if (iResult != 0)
        {
            GenericType* pL = _pL->getAs<GenericType>();
            GenericType* pR = _pR->getAs<GenericType>();
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"), L"&", pL->DimToString().c_str(), pR->DimToString().c_str());
            throw ast::ScilabError(pMsg);
        }
        return pResult;
    }

    return NULL;
}

// ||
InternalType* GenericShortcutOr(InternalType* _pL)
{
    InternalType* pResult = NULL;

    if (_pL->isBool())
    {
        BoolOrBool(_pL->getAs<Bool>(), (Bool**)&pResult);
    }

    if (_pL->isInt())
    {
        IntOrInt(_pL, (Bool**)&pResult);
    }

    if (_pL->isSparseBool())
    {
        SparseBoolOrSparseBool(_pL, (Bool**)&pResult);
    }

    return pResult;
}

// |
InternalType* GenericLogicalOr(InternalType* _pL, InternalType* _pR)
{
    InternalType* pResult = NULL;

    if (_pL->isBool() && _pR->isBool())
    {
        Bool *pL = (Bool*)_pL;
        Bool *pR = (Bool*)_pR;

        int iResult = BoolLogicalOrBool(pL, pR, (Bool**)&pResult);
        if (iResult != 0)
        {
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"), L"|", pL->DimToString().c_str(), pR->DimToString().c_str());
            throw ast::ScilabError(pMsg);
        }
        return pResult;
    }

    if (_pL->isInt() && _pR->isInt())
    {
        int iResult = IntLogicalOrInt(_pL, _pR, &pResult);
        if (iResult != 0)
        {
            GenericType* pL = _pL->getAs<GenericType>();
            GenericType* pR = _pR->getAs<GenericType>();
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"), L"|", pL->DimToString().c_str(), pR->DimToString().c_str());
            throw ast::ScilabError(pMsg);
        }
        return pResult;
    }

    if (_pL->isSparseBool() && _pR->isSparseBool())
    {
        int iResult = SparseBoolLogicalOrSparseBool(_pL, _pR, &pResult);
        if (iResult != 0)
        {
            GenericType* pL = _pL->getAs<GenericType>();
            GenericType* pR = _pR->getAs<GenericType>();
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"), L"|", pL->DimToString().c_str(), pR->DimToString().c_str());
            throw ast::ScilabError(pMsg);
        }
        return pResult;
    }

    return NULL;
}

int BoolAndBool(Bool* _pL, Bool** _pOut)
{
    for (int i = 0 ; i < _pL->getSize() ; i++)
    {
        if (_pL->get(i) == 0)
        {
            //false && something -> false
            *_pOut = new Bool(0);
            return 0;
        }
    }

    //call non shorcut opearion
    *_pOut = NULL;
    return 0;
}

int BoolOrBool(Bool* _pL, Bool** _pOut)
{
    for (int i = 0 ; i < _pL->getSize() ; i++)
    {
        if (_pL->get(i) == 0)
        {
            //call non shorcut opearion
            *_pOut = NULL;
            return 0;
        }
    }

    *_pOut = new Bool(1); //true && something -> true
    return 0;
}

template <class K>
static int IntOrInt(K* _pL, Bool** _pOut)
{
    for (int i = 0 ; i < _pL->getSize() ; i++)
    {
        if (_pL->get(i) == 0)
        {
            //call non shorcut opearion
            *_pOut = NULL;
            return 0;
        }
    }

    *_pOut = new Bool(1); //true && something -> true
    return 0;
}

int IntOrInt(InternalType* _pL, Bool** _pOut)
{
    switch (_pL->getType())
    {
        case InternalType::RealInt8 :
        {
            return IntOrInt(_pL->getAs<Int8>(), _pOut);
        }
        case InternalType::RealUInt8 :
        {
            return IntOrInt(_pL->getAs<UInt8>(), _pOut);
        }
        case InternalType::RealInt16 :
        {
            return IntOrInt(_pL->getAs<Int16>(), _pOut);
        }
        case InternalType::RealUInt16 :
        {
            return IntOrInt(_pL->getAs<UInt16>(), _pOut);
        }
        case InternalType::RealInt32 :
        {
            return IntOrInt(_pL->getAs<Int32>(), _pOut);
        }
        case InternalType::RealUInt32 :
        {
            return IntOrInt(_pL->getAs<UInt32>(), _pOut);
        }
        case InternalType::RealInt64 :
        {
            return IntOrInt(_pL->getAs<Int64>(), _pOut);
        }
        case InternalType::RealUInt64 :
        {
            return IntOrInt(_pL->getAs<UInt64>(), _pOut);
        }
    }

    return 3;
}

template <class K>
static int IntAndInt(K* _pL, Bool** _pOut)
{
    for (int i = 0 ; i < _pL->getSize() ; i++)
    {
        if (_pL->get(i) == 0)
        {
            //false && something -> false
            *_pOut = new Bool(0);
            return 0;
        }
    }

    //call non shorcut opearion
    *_pOut = NULL;
    return 0;
}

int IntAndInt(InternalType* _pL, Bool** _pOut)
{
    switch (_pL->getType())
    {
        case InternalType::RealInt8 :
        {
            return IntAndInt(_pL->getAs<Int8>(), _pOut);
        }
        case InternalType::RealUInt8 :
        {
            return IntAndInt(_pL->getAs<UInt8>(), _pOut);
        }
        case InternalType::RealInt16 :
        {
            return IntAndInt(_pL->getAs<Int16>(), _pOut);
        }
        case InternalType::RealUInt16 :
        {
            return IntAndInt(_pL->getAs<UInt16>(), _pOut);
        }
        case InternalType::RealInt32 :
        {
            return IntAndInt(_pL->getAs<Int32>(), _pOut);
        }
        case InternalType::RealUInt32 :
        {
            return IntAndInt(_pL->getAs<UInt32>(), _pOut);
        }
        case InternalType::RealInt64 :
        {
            return IntAndInt(_pL->getAs<Int64>(), _pOut);
        }
        case InternalType::RealUInt64 :
        {
            return IntAndInt(_pL->getAs<UInt64>(), _pOut);
        }
    }

    return 3;
}

template <class K>
static int IntLogicalAndInt(K* _pL, K* _pR, InternalType** _pOut)
{
    // left scalar bitwise and right matrix
    if (_pL->isScalar())
    {
        K *pI = new K(_pR->getDims(), _pR->getDimsArray());
        for (int i = 0 ; i < pI->getSize() ; i++)
        {
            pI->set(i, _pL->get(0) & _pR->get(i));
        }

        *_pOut = pI;
        return 0;
    }

    // right scalar bitwise and left matrix
    if (_pR->isScalar())
    {
        K *pI = new K(_pL->getDims(), _pL->getDimsArray());
        for (int i = 0 ; i < pI->getSize() ; i++)
        {
            pI->set(i, _pR->get(0) & _pL->get(i));
        }

        *_pOut = pI;
        return 0;
    }

    // check dims
    if (_pL->getDims() != _pR->getDims())
    {
        return 1;
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0 ; i < _pL->getDims() ; i++)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            return 1;
        }
    }

    // left matrix bitwise and right matrix
    K* pI = new K(_pR->getDims(), _pR->getDimsArray());
    for (int i = 0 ; i < _pL->getSize() ; i++)
    {
        pI->set(i, _pL->get(i) & _pR->get(i));
    }

    *_pOut = pI;
    return 0;
}

int IntLogicalAndInt(InternalType* _pL, InternalType*  _pR, InternalType** _pOut)
{
    switch (_pL->getType())
    {
        case InternalType::RealInt8 :
        {
            return IntLogicalAndInt(_pL->getAs<Int8>(), _pR->getAs<Int8>(), _pOut);
        }
        case InternalType::RealUInt8 :
        {
            return IntLogicalAndInt(_pL->getAs<UInt8>(), _pR->getAs<UInt8>(), _pOut);
        }
        case InternalType::RealInt16 :
        {
            return IntLogicalAndInt(_pL->getAs<Int16>(), _pR->getAs<Int16>(), _pOut);
        }
        case InternalType::RealUInt16 :
        {
            return IntLogicalAndInt(_pL->getAs<UInt16>(), _pR->getAs<UInt16>(), _pOut);
        }
        case InternalType::RealInt32 :
        {
            return IntLogicalAndInt(_pL->getAs<Int32>(), _pR->getAs<Int32>(), _pOut);
        }
        case InternalType::RealUInt32 :
        {
            return IntLogicalAndInt(_pL->getAs<UInt32>(), _pR->getAs<UInt32>(), _pOut);
        }
        case InternalType::RealInt64 :
        {
            return IntLogicalAndInt(_pL->getAs<Int64>(), _pR->getAs<Int64>(), _pOut);
        }
        case InternalType::RealUInt64 :
        {
            return IntLogicalAndInt(_pL->getAs<UInt64>(), _pR->getAs<UInt64>(), _pOut);
        }
    }

    return 3;
}

template <class K>
static int IntLogicalOrInt(K* _pL, K* _pR, InternalType** _pOut)
{
    // left scalar bitwise or right matrix
    if (_pL->isScalar())
    {
        K *pI = new K(_pR->getDims(), _pR->getDimsArray());
        for (int i = 0 ; i < pI->getSize() ; i++)
        {
            pI->set(i, _pL->get(0) | _pR->get(i));
        }

        *_pOut = pI;
        return 0;
    }

    // right scalar bitwise or left matrix
    if (_pR->isScalar())
    {
        K *pI = new K(_pL->getDims(), _pL->getDimsArray());
        for (int i = 0 ; i < pI->getSize() ; i++)
        {
            pI->set(i, _pR->get(0) | _pL->get(i));
        }

        *_pOut = pI;
        return 0;
    }

    // check dims
    if (_pL->getDims() != _pR->getDims())
    {
        return 1;
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0 ; i < _pL->getDims() ; i++)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            return 1;
        }
    }

    // left matrix bitwise or right matrix
    K* pI = new K(_pR->getDims(), _pR->getDimsArray());
    for (int i = 0 ; i < _pL->getSize() ; i++)
    {
        pI->set(i, _pL->get(i) | _pR->get(i));
    }

    *_pOut = pI;
    return 0;
}

int IntLogicalOrInt(InternalType* _pL, InternalType*  _pR, InternalType** _pOut)
{
    switch (_pL->getType())
    {
        case InternalType::RealInt8 :
        {
            Int8* pI1 = _pL->getAs<Int8>();
            Int8* pI2 = _pR->getAs<Int8>();
            return IntLogicalOrInt(pI1, pI2, _pOut);
        }
        case InternalType::RealUInt8 :
        {
            UInt8* pI1 = _pL->getAs<UInt8>();
            UInt8* pI2 = _pR->getAs<UInt8>();
            return IntLogicalOrInt(pI1, pI2, _pOut);
        }
        case InternalType::RealInt16 :
        {
            Int16* pI1 = _pL->getAs<Int16>();
            Int16* pI2 = _pR->getAs<Int16>();
            return IntLogicalOrInt(pI1, pI2, _pOut);
        }
        case InternalType::RealUInt16 :
        {
            UInt16* pI1 = _pL->getAs<UInt16>();
            UInt16* pI2 = _pR->getAs<UInt16>();
            return IntLogicalOrInt(pI1, pI2, _pOut);
        }
        case InternalType::RealInt32 :
        {
            Int32* pI1 = _pL->getAs<Int32>();
            Int32* pI2 = _pR->getAs<Int32>();
            return IntLogicalOrInt(pI1, pI2, _pOut);
        }
        case InternalType::RealUInt32 :
        {
            UInt32* pI1 = _pL->getAs<UInt32>();
            UInt32* pI2 = _pR->getAs<UInt32>();
            return IntLogicalOrInt(pI1, pI2, _pOut);
        }
        case InternalType::RealInt64 :
        {
            Int64* pI1 = _pL->getAs<Int64>();
            Int64* pI2 = _pR->getAs<Int64>();
            return IntLogicalOrInt(pI1, pI2, _pOut);
        }
        case InternalType::RealUInt64 :
        {
            UInt64* pI1 = _pL->getAs<UInt64>();
            UInt64* pI2 = _pR->getAs<UInt64>();
            return IntLogicalOrInt(pI1, pI2, _pOut);
        }
    }

    return 3;
}

int BoolLogicalOrBool(Bool* _pL, Bool*  _pR, Bool** _pOut)
{
    int* piR = _pR->get();
    int* piL = _pL->get();
    int* piB = NULL;

    // M | scalar
    if (_pR->getSize() == 1)
    {
        *_pOut = new Bool(_pL->getRows(), _pL->getCols(), &piB);
        for (int i = 0 ; i < _pL->getSize(); i++)
        {
            piB[i] = (piR[0] == 1) || (piL[i] == 1);
        }

        return 0;
    }

    if (_pL->getSize() == 1)
    {
        // scalar | M
        *_pOut = new Bool(_pR->getRows(), _pR->getCols(), &piB);
        for (int i = 0 ; i < _pR->getSize(); i++)
        {
            piB[i] = (piR[i] == 1) || (piL[0] == 1);
        }

        return 0;
    }

    if (_pR->getRows() == _pL->getRows() && _pR->getCols() == _pL->getCols())
    {
        // M | N (generic case)
        *_pOut = new Bool(_pR->getRows(), _pR->getCols(), &piB);
        for (int i = 0 ; i < _pL->getSize(); i++)
        {
            piB[i] = (piR[i] == 1) || (piL[i] == 1);
        }

        return 0;
    }

    return 1;
}

int BoolLogicalAndBool(Bool* _pL, Bool*  _pR, Bool** _pOut)
{
    int* piR = _pR->get();
    int* piL = _pL->get();
    int* piB = NULL;

    // M & scalar
    if (_pR->getSize() == 1)
    {
        *_pOut = new Bool(_pL->getRows(), _pL->getCols(), &piB);
        for (int i = 0 ; i < _pL->getSize(); i++)
        {
            piB[i] = (piR[0] == 1) && (piL[i] == 1);
        }

        return 0;
    }

    if (_pL->getSize() == 1)
    {
        // scalar & M
        *_pOut = new Bool(_pR->getRows(), _pR->getCols(), &piB);
        for (int i = 0 ; i < _pR->getSize(); i++)
        {
            piB[i] = (piR[i] == 1) && (piL[0] == 1);
        }

        return 0;
    }

    if (_pR->getRows() == _pL->getRows() && _pR->getCols() == _pL->getCols())
    {
        // M & N (generic case)
        *_pOut = new Bool(_pR->getRows(), _pR->getCols(), &piB);
        for (int i = 0 ; i < _pL->getSize(); i++)
        {
            piB[i] = (piR[i] == 1) && (piL[i] == 1);
        }

        return 0;
    }

    return 1;
}

int SparseBoolAndSparseBool(InternalType* _pL, Bool** _pOut)
{
    SparseBool* pL = _pL->getAs<SparseBool>();
    if (pL->nbTrue() != pL->getSize())
    {
        *_pOut = new Bool(0);
        return 0;
    }

    *_pOut = NULL;
    return 0;
}

int SparseBoolLogicalAndSparseBool(InternalType* _pL, InternalType*  _pR, InternalType** _pOut)
{
    SparseBool *pL = (SparseBool*)_pL;
    SparseBool *pR = (SparseBool*)_pR;

    if (pL->isScalar())
    {
        if (pL->get(0, 0))
        {
            *_pOut = pR->clone();
        }
        else
        {
            *_pOut = new SparseBool(pR->getRows(), pR->getCols());
        }

        return 0;
    }

    if (pR->isScalar())
    {
        if (pR->get(0, 0))
        {
            *_pOut = _pL->clone();
        }
        else
        {
            *_pOut = new SparseBool(pL->getRows(), pL->getCols());
        }

        return 0;
    }

    if (pL->getRows() == pR->getRows() && pL->getCols() == pR->getCols())
    {
        *_pOut = pL->newLogicalAnd(*pR);
        return 0;
        /*
        int nbTrueL = static_cast<int>(_pL->nbTrue());
        double* pLRows = new double[nbTrueL * 2];
        _pL->outputRowCol(pLRows);
        double* pLCols = pLRows + nbTrueL;

        int nbTrueR = static_cast<int>(_pR->nbTrue());
        double* pRRows = new double[nbTrueR * 2];
        _pR->outputRowCol(pRRows);
        double* pRCols = pRRows + nbTrueR;

        *_pOut = new SparseBool(_pR->getRows(), _pL->getCols());
        (*_pOut)->no
        for(int i = 0 ; i < nbTrueL ; i++)
        {
            if(_pL->get(pLRows[i], pLRows[i]) == _pR->get(pLRows[i], pLRows[i]))
            {//set 1
                (*_pOut)->set(pLRows[i], pLRows[i], true);
            }

        }
        */
    }

    return 1;
}

int SparseBoolOrSparseBool(InternalType* _pL, Bool** _pOut)
{
    SparseBool* pL = _pL->getAs<SparseBool>();
    if (pL->nbTrue() == pL->getSize())
    {
        *_pOut = new Bool(1);
        return 0;
    }

    *_pOut = NULL;
    return 0;
}

int SparseBoolLogicalOrSparseBool(InternalType* _pL, InternalType*  _pR, InternalType** _pOut)
{
    SparseBool *pL = (SparseBool*)_pL;
    SparseBool *pR = (SparseBool*)_pR;

    if (pL->isScalar())
    {
        if (pL->get(0, 0))
        {
            *_pOut = _pR->clone();
        }
        else
        {
            *_pOut = new SparseBool(pR->getRows(), pR->getCols());
        }

        return 0;
    }

    if (pR->isScalar())
    {
        if (pR->get(0, 0))
        {
            *_pOut = _pL->clone();
        }
        else
        {
            *_pOut = new SparseBool(pL->getRows(), pL->getCols());
        }

        return 0;
    }

    if (pL->getRows() == pR->getRows() && pL->getCols() == pR->getCols())
    {
        *_pOut = pL->newLogicalOr(*pR);
        return 0;
    }
    return 1;
}
