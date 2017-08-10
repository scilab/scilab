/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

#include "types_opposite.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "int.hxx"
#include "sparse.hxx"
#include "bool.hxx"
#include "generic_operations.hxx"

using namespace types;
static opposite_function pOppositefunction[types::InternalType::IdLast] = {NULL};

void fillOppositeFunction()
{
#define scilab_fill_opposite(id1, func, typeIn1, typeOut) \
    pOppositefunction[types::InternalType::Id ## id1] = (opposite_function)&opposite_##func<typeIn1, typeOut>

    //Empty
    scilab_fill_opposite(Empty, E, Double, Double);

    //Identity
    scilab_fill_opposite(Identity, I, Double, Double);
    scilab_fill_opposite(IdentityComplex, IC, Double, Double);

    //Scalar
    scilab_fill_opposite(ScalarDouble, S, Double, Double);
    scilab_fill_opposite(ScalarDoubleComplex, SC, Double, Double);
    scilab_fill_opposite(ScalarBool, S, Bool, Bool);
    scilab_fill_opposite(ScalarInt8, S, Int8, Int8);
    scilab_fill_opposite(ScalarUInt8, S, UInt8, UInt8);
    scilab_fill_opposite(ScalarInt16, S, Int16, Int16);
    scilab_fill_opposite(ScalarUInt16, S, UInt16, UInt16);
    scilab_fill_opposite(ScalarInt32, S, Int32, Int32);
    scilab_fill_opposite(ScalarUInt32, S, UInt32, UInt32);
    scilab_fill_opposite(ScalarInt64, S, Int64, Int64);
    scilab_fill_opposite(ScalarUInt64, S, UInt64, UInt64);
    scilab_fill_opposite(ScalarPolynom, S, Polynom, Polynom);
    scilab_fill_opposite(ScalarPolynomComplex, SC, Polynom, Polynom);

    //Matrix
    scilab_fill_opposite(Double, M, Double, Double);
    scilab_fill_opposite(DoubleComplex, MC, Double, Double);
    scilab_fill_opposite(Bool, M, Bool, Bool);
    scilab_fill_opposite(Int8, M, Int8, Int8);
    scilab_fill_opposite(UInt8, M, UInt8, UInt8);
    scilab_fill_opposite(Int16, M, Int16, Int16);
    scilab_fill_opposite(UInt16, M, UInt16, UInt16);
    scilab_fill_opposite(Int32, M, Int32, Int32);
    scilab_fill_opposite(UInt32, M, UInt32, UInt32);
    scilab_fill_opposite(Int64, M, Int64, Int64);
    scilab_fill_opposite(UInt64, M, UInt64, UInt64);

    scilab_fill_opposite(Sparse, M, Sparse, Sparse);
    scilab_fill_opposite(SparseComplex, MC, Sparse, Sparse);

    scilab_fill_opposite(Polynom, M, Polynom, Polynom);
    scilab_fill_opposite(PolynomComplex, MC, Polynom, Polynom);


#undef scilab_fill_subtract
}

types::InternalType* GenericUnaryMinus(types::InternalType* _pL)
{
    InternalType *pResult = NULL;

    opposite_function opp = pOppositefunction[_pL->getId()];
    if (opp)
    {
        pResult = opp(_pL);
        if (pResult)
        {
            return pResult;
        }
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return NULL;
}


template<class T, class O>
types::InternalType* opposite_E(T *_pL)
{
    return _pL;
}

template<class T, class O>
types::InternalType* opposite_I(T *_pL)
{
    double* pR = NULL;
    O* pOut = new types::Double(-1, -1, &pR);
    opposite(_pL->get(0), pR);
    return pOut;
}

template<class T, class O>
types::InternalType* opposite_IC(T *_pL)
{
    double* pR = NULL;
    double* pI = NULL;
    O* pOut = new types::Double(-1, -1, &pR, &pI);
    opposite(_pL->get(0), _pL->getImg(0), pR, pI);
    return pOut;
}

template<class T, class O>
types::InternalType* opposite_S(T *_pL)
{
    O* pOut = new O(0);
    opposite(_pL->get(0), pOut->get());
    return pOut;
}

template<class T, class O>
types::InternalType* opposite_SC(T *_pL)
{
    O* pOut = new O(0.0, 0.0);
    opposite(_pL->get(0), _pL->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class O>
types::InternalType* opposite_M(T *_pL)
{
    int iDimsL = _pL->getDims();
    int* piDimsL = _pL->getDimsArray();

    O* pOut = new O(iDimsL, piDimsL);

    int iSize = pOut->getSize();

    opposite(_pL->get(), iSize, pOut->get());
    return pOut;
}

template<class T, class O>
types::InternalType* opposite_MC(T *_pL)
{
    int iDimsL = _pL->getDims();
    int* piDimsL = _pL->getDimsArray();

    O* pOut = new O(iDimsL, piDimsL, true);

    int iSize = pOut->getSize();

    opposite(_pL->get(), _pL->getImg(), iSize, pOut->get(), pOut->getImg());
    return pOut;
}

//Boolean
template<>
types::InternalType* opposite_M<types::Bool, types::Double>(types::Bool* _pL)
{
    Double* pOut = new Double(_pL->getDims(), _pL->getDimsArray());
    int iSize = _pL->getSize();

    int* pI = _pL->get();
    double* pD = pOut->get();
    for (int i = 0 ; i < iSize ; ++i)
    {
        pD[i] = pI[i] == 0 ? 1 : 0;
    }

    return pOut;
}

//Sparse
template<>
types::InternalType* opposite_M<types::Sparse, types::Sparse>(types::Sparse *_pL)
{
    types::Sparse* pOut = (types::Sparse*)(_pL->clone());
    pOut->opposite();
    return pOut;
}

template<>
types::InternalType* opposite_MC<types::Sparse, types::Sparse>(types::Sparse *_pL)
{
    types::Sparse* pOut = (types::Sparse*)(_pL->clone());
    pOut->opposite();
    return pOut;
}

//Polynom
template<>
types::InternalType* opposite_S<types::Polynom, types::Polynom>(types::Polynom *_pL)
{
    types::Polynom* pOut = (types::Polynom*)_pL->clone();
    types::SinglePoly* pSPL = _pL->get(0);
    types::SinglePoly* pSPO = pOut->get(0);
    opposite(pSPL->get(), pSPL->getSize(), pSPO->get());
    return pOut;
}

template<>
types::InternalType* opposite_SC<types::Polynom, types::Polynom>(types::Polynom *_pL)
{
    types::Polynom* pOut = (types::Polynom*)_pL->clone();
    types::SinglePoly* pSPL = _pL->get(0);
    types::SinglePoly* pSPO = pOut->get(0);
    opposite(pSPL->get(), pSPL->getImg(), pSPL->getSize(), pSPO->get(), pSPO->getImg());
    return pOut;
}

template<>
types::InternalType* opposite_M<types::Polynom, types::Polynom>(types::Polynom *_pL)
{
    types::Polynom* pOut = (types::Polynom*)_pL->clone();

    int iSize = _pL->getSize();

    for (int i = 0 ; i < iSize ; ++i)
    {
        types::SinglePoly* pSPL = _pL->get(i);
        types::SinglePoly* pSPO = pOut->get(i);
        opposite(pSPL->get(), pSPL->getSize(), pSPO->get());
    }
    return pOut;
}

template<>
types::InternalType* opposite_MC<types::Polynom, types::Polynom>(types::Polynom *_pL)
{
    types::Polynom* pOut = (types::Polynom*)_pL->clone();

    int iSize = _pL->getSize();

    for (int i = 0 ; i < iSize ; ++i)
    {
        types::SinglePoly* pSPL = _pL->get(i);
        types::SinglePoly* pSPO = pOut->get(i);
        opposite(pSPL->get(), pSPL->getImg(), pSPL->getSize(), pSPO->get(), pSPO->getImg());
    }
    return pOut;
}
