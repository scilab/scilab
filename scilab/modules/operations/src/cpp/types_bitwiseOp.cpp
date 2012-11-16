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

#include "types_bitwiseOp.hxx"
#include "int.hxx"

int bitwiseAndToIntAndInt(types::InternalType* _pL, types::InternalType*  _pR, types::InternalType** _pOut)
{
    switch (_pL->getType())
    {
        case types::InternalType::RealInt8 :
        {
            types::Int8* pI1 = _pL->getAs<types::Int8>();
            types::Int8* pI2 = _pR->getAs<types::Int8>();
            return bitwiseAndToIntAndInt(pI1, pI2, _pOut);
        }
        case types::InternalType::RealUInt8 :
        {
            types::UInt8* pI1 = _pL->getAs<types::UInt8>();
            types::UInt8* pI2 = _pR->getAs<types::UInt8>();
            return bitwiseAndToIntAndInt(pI1, pI2, _pOut);
        }
        case types::InternalType::RealInt16 :
        {
            types::Int16* pI1 = _pL->getAs<types::Int16>();
            types::Int16* pI2 = _pR->getAs<types::Int16>();
            return bitwiseAndToIntAndInt(pI1, pI2, _pOut);
        }
        case types::InternalType::RealUInt16 :
        {
            types::UInt16* pI1 = _pL->getAs<types::UInt16>();
            types::UInt16* pI2 = _pR->getAs<types::UInt16>();
            return bitwiseAndToIntAndInt(pI1, pI2, _pOut);
        }
        case types::InternalType::RealInt32 :
        {
            types::Int32* pI1 = _pL->getAs<types::Int32>();
            types::Int32* pI2 = _pR->getAs<types::Int32>();
            return bitwiseAndToIntAndInt(pI1, pI2, _pOut);
        }
        case types::InternalType::RealUInt32 :
        {
            types::UInt32* pI1 = _pL->getAs<types::UInt32>();
            types::UInt32* pI2 = _pR->getAs<types::UInt32>();
            return bitwiseAndToIntAndInt(pI1, pI2, _pOut);
        }
        case types::InternalType::RealInt64 :
        {
            types::Int64* pI1 = _pL->getAs<types::Int64>();
            types::Int64* pI2 = _pR->getAs<types::Int64>();
            return bitwiseAndToIntAndInt(pI1, pI2, _pOut);
        }
        case types::InternalType::RealUInt64 :
        {
            types::UInt64* pI1 = _pL->getAs<types::UInt64>();
            types::UInt64* pI2 = _pR->getAs<types::UInt64>();
            return bitwiseAndToIntAndInt(pI1, pI2, _pOut);
        }
    }

    return 3;
}

template <class K>
int bitwiseAndToIntAndInt(K* _pL, K* _pR, types::InternalType** _pOut)
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


int bitwiseOrToIntAndInt(types::InternalType* _pL, types::InternalType*  _pR, types::InternalType** _pOut)
{
    switch (_pL->getType())
    {
        case types::InternalType::RealInt8 :
        {
            types::Int8* pI1 = _pL->getAs<types::Int8>();
            types::Int8* pI2 = _pR->getAs<types::Int8>();
            return bitwiseOrToIntAndInt(pI1, pI2, _pOut);
        }
        case types::InternalType::RealUInt8 :
        {
            types::UInt8* pI1 = _pL->getAs<types::UInt8>();
            types::UInt8* pI2 = _pR->getAs<types::UInt8>();
            return bitwiseOrToIntAndInt(pI1, pI2, _pOut);
        }
        case types::InternalType::RealInt16 :
        {
            types::Int16* pI1 = _pL->getAs<types::Int16>();
            types::Int16* pI2 = _pR->getAs<types::Int16>();
            return bitwiseOrToIntAndInt(pI1, pI2, _pOut);
        }
        case types::InternalType::RealUInt16 :
        {
            types::UInt16* pI1 = _pL->getAs<types::UInt16>();
            types::UInt16* pI2 = _pR->getAs<types::UInt16>();
            return bitwiseOrToIntAndInt(pI1, pI2, _pOut);
        }
        case types::InternalType::RealInt32 :
        {
            types::Int32* pI1 = _pL->getAs<types::Int32>();
            types::Int32* pI2 = _pR->getAs<types::Int32>();
            return bitwiseOrToIntAndInt(pI1, pI2, _pOut);
        }
        case types::InternalType::RealUInt32 :
        {
            types::UInt32* pI1 = _pL->getAs<types::UInt32>();
            types::UInt32* pI2 = _pR->getAs<types::UInt32>();
            return bitwiseOrToIntAndInt(pI1, pI2, _pOut);
        }
        case types::InternalType::RealInt64 :
        {
            types::Int64* pI1 = _pL->getAs<types::Int64>();
            types::Int64* pI2 = _pR->getAs<types::Int64>();
            return bitwiseOrToIntAndInt(pI1, pI2, _pOut);
        }
        case types::InternalType::RealUInt64 :
        {
            types::UInt64* pI1 = _pL->getAs<types::UInt64>();
            types::UInt64* pI2 = _pR->getAs<types::UInt64>();
            return bitwiseOrToIntAndInt(pI1, pI2, _pOut);
        }
    }

    return 3;
}

template <class K>
int bitwiseOrToIntAndInt(K* _pL, K* _pR, types::InternalType** _pOut)
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
