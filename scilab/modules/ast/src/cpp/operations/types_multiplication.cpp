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

#include "types_multiplication.hxx"
#include "types_addition.hxx"
#include "double.hxx"
#include "int.hxx"
#include "sparse.hxx"
#include "polynom.hxx"
#include "singlepoly.hxx"
#include "operations.hxx"

#include <Eigen/Sparse>
#include <Eigen/Dense>

extern "C"
{
#include "matrix_multiplication.h"
#include "matrix_addition.h"
#include "localization.h"
#include "charEncoding.h"
#include "elem_common.h"
}


using namespace types;

// define arrays on operation functions
static intmul_function pIntMulfunction[types::InternalType::IdLast][types::InternalType::IdLast] = {NULL};
static std::wstring op = L"*";

void fillIntMulFunction(){
#define scilab_fill_intmul(id1, id2, func, typeIn1, typeIn2, typeOut) \
    pIntMulfunction[types::InternalType::Id##id1][types::InternalType::Id##id2] = (intmul_function) & intmul_##func<typeIn1, typeIn2, typeOut>

    //Matrix * Matrix
    scilab_fill_intmul(Int8, Int8,   M_M, Int8, Int8, Int8);
    scilab_fill_intmul(UInt8, UInt8,  M_M, UInt8, UInt8, UInt8);
    scilab_fill_intmul(Int16, Int16,  M_M, Int16, Int16, Int16);
    scilab_fill_intmul(UInt16, UInt16, M_M, UInt16, UInt16, UInt16);
    scilab_fill_intmul(Int32, Int32,  M_M, Int32, Int32, Int32);
    scilab_fill_intmul(UInt32, UInt32, M_M, UInt32, UInt32, UInt32);
    scilab_fill_intmul(Int64, Int64,  M_M, Int64, Int64, Int64);
    scilab_fill_intmul(UInt64, UInt64, M_M, UInt64, UInt64, UInt64);


    // Matrix * Scalar
    scilab_fill_intmul(Int8, ScalarInt8,   M_S, Int8, Int8, Int8);
    scilab_fill_intmul(UInt8, ScalarUInt8,  M_S, UInt8, UInt8, UInt8);
    scilab_fill_intmul(Int16, ScalarInt16,  M_S, Int16, Int16, Int16);
    scilab_fill_intmul(UInt16, ScalarUInt16, M_S, UInt16, UInt16, UInt16);
    scilab_fill_intmul(Int32, ScalarInt32,  M_S, Int32, Int32, Int32);
    scilab_fill_intmul(UInt32, ScalarUInt32, M_S, UInt32, UInt32, UInt32);
    scilab_fill_intmul(Int64, ScalarInt64,  M_S, Int64, Int64, Int64);
    scilab_fill_intmul(UInt64, ScalarUInt64, M_S, UInt64, UInt64, UInt64);


    // Scalar * Matrix
    scilab_fill_intmul(ScalarInt8, Int8,   S_M, Int8, Int8, Int8);
    scilab_fill_intmul(ScalarUInt8, UInt8,  S_M, UInt8, UInt8, UInt8);
    scilab_fill_intmul(ScalarInt16, Int16,  S_M, Int16, Int16, Int16);
    scilab_fill_intmul(ScalarUInt16, UInt16, S_M, UInt16, UInt16, UInt16);
    scilab_fill_intmul(ScalarInt32, Int32,  S_M, Int32, Int32, Int32);
    scilab_fill_intmul(ScalarUInt32, UInt32, S_M, UInt32, UInt32, UInt32);
    scilab_fill_intmul(ScalarInt64, Int64,  S_M, Int64, Int64, Int64);
    scilab_fill_intmul(ScalarUInt64, UInt64, S_M, UInt64, UInt64, UInt64);


    // Scalar * Scalar
    scilab_fill_intmul(ScalarInt8, ScalarInt8,   S_S, Int8, Int8, Int8);
    scilab_fill_intmul(ScalarUInt8, ScalarUInt8,  S_S, UInt8, UInt8, UInt8);
    scilab_fill_intmul(ScalarInt16, ScalarInt16,  S_S, Int16, Int16, Int16);
    scilab_fill_intmul(ScalarUInt16, ScalarUInt16, S_S, UInt16, UInt16, UInt16);
    scilab_fill_intmul(ScalarInt32, ScalarInt32,  S_S, Int32, Int32, Int32);
    scilab_fill_intmul(ScalarUInt32, ScalarUInt32, S_S, UInt32, UInt32, UInt32);
    scilab_fill_intmul(ScalarInt64, ScalarInt64,  S_S, Int64, Int64, Int64);
    scilab_fill_intmul(ScalarUInt64, ScalarUInt64, S_S, UInt64, UInt64, UInt64);
}


InternalType *GenericTimes(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;
    GenericType::ScilabType TypeL = _pLeftOperand->getType();
    GenericType::ScilabType TypeR = _pRightOperand->getType();

    if (TypeL == GenericType::ScilabDouble && _pLeftOperand->getAs<Double>()->isEmpty())
    {
        return Double::Empty();
    }

    if (TypeR == GenericType::ScilabDouble && _pRightOperand->getAs<Double>()->isEmpty())
    {
        return Double::Empty();
    }

    /*
    ** DOUBLE * DOUBLE
    */
    if (TypeL == GenericType::ScilabDouble && TypeR == GenericType::ScilabDouble)
    {
        Double *pL   = _pLeftOperand->getAs<Double>();
        Double *pR   = _pRightOperand->getAs<Double>();

        int iResult = MultiplyDoubleByDouble(pL, pR, (Double**)&pResult);
        if (iResult)
        {
            throw ast::InternalError(errorMultiplySize(pL, pR));
        }

        return pResult;
    }

    /*
    ** INT * INT
    */
    if (_pLeftOperand->isInt() && _pRightOperand->isInt())
    {
        intmul_function intmul = pIntMulfunction[_pLeftOperand->getId()][_pRightOperand->getId()];
        if (intmul)
        {
            pResult = intmul(_pLeftOperand, _pRightOperand);
            if (pResult)
            {
                return pResult;
            }
            else
            {
                throw ast::InternalError(errorMultiplySize(_pLeftOperand->getAs<GenericType>(), _pRightOperand->getAs<GenericType>()));
            }
        }
    }

    /*
    ** DOUBLE * POLY
    */
    else if (TypeL == InternalType::ScilabDouble && TypeR == InternalType::ScilabPolynom)
    {
        Double *pL   = _pLeftOperand->getAs<Double>();
        Polynom *pR     = _pRightOperand->getAs<types::Polynom>();

        int iResult = MultiplyDoubleByPoly(pL, pR, (Polynom**)&pResult);
        if (iResult)
        {
             throw ast::InternalError(errorMultiplySize(pL, pR));
        }

        return pResult;
    }

    /*
    ** POLY * DOUBLE
    */
    else if (TypeL == InternalType::ScilabPolynom && TypeR == InternalType::ScilabDouble)
    {
        Polynom *pL          = _pLeftOperand->getAs<types::Polynom>();
        Double *pR              = _pRightOperand->getAs<Double>();

        int iResult = MultiplyPolyByDouble(pL, pR, (Polynom**)&pResult);
        if (iResult)
        {
             throw ast::InternalError(errorMultiplySize(pL, pR));
        }

        return pResult;
    }

    /*
    ** POLY * POLY
    */
    else if (TypeL == InternalType::ScilabPolynom && TypeR == InternalType::ScilabPolynom)
    {
        Polynom *pL          = _pLeftOperand->getAs<types::Polynom>();
        Polynom *pR          = _pRightOperand->getAs<types::Polynom>();


        //check varname
        if (pL->getVariableName() != pR->getVariableName())
        {
            //call overload
            return NULL;
        }

        int iResult = MultiplyPolyByPoly(pL, pR, (Polynom**)&pResult);
        if (iResult)
        {
             throw ast::InternalError(errorMultiplySize(pL, pR));
        }

        return pResult;
    }

    /*
    ** SPARSE * SPARSE
    */
    if (TypeL == GenericType::ScilabSparse && TypeR == GenericType::ScilabSparse)
    {
        Sparse *pL   = _pLeftOperand->getAs<Sparse>();
        Sparse *pR   = _pRightOperand->getAs<Sparse>();

        int iResult = MultiplySparseBySparse(pL, pR, (Sparse**)&pResult);
        if (iResult)
        {
             throw ast::InternalError(errorMultiplySize(pL, pR));
        }

        return pResult;
    }

    /*
    ** DOUBLE * SPARSE
    */
    if (TypeL == GenericType::ScilabDouble && TypeR == GenericType::ScilabSparse)
    {
        Double *pL   = _pLeftOperand->getAs<Double>();
        Sparse *pR   = _pRightOperand->getAs<Sparse>();

        int iResult = MultiplyDoubleBySparse(pL, pR, (GenericType**)&pResult);
        if (iResult)
        {
             throw ast::InternalError(errorMultiplySize(pL, pR));
        }

        return pResult;
    }

    /*
    ** SPARSE * DOUBLE
    */
    if (TypeL == GenericType::ScilabSparse && TypeR == GenericType::ScilabDouble)
    {
        Sparse *pL   = _pLeftOperand->getAs<Sparse>();
        Double *pR   = _pRightOperand->getAs<Double>();

        int iResult = MultiplySparseByDouble(pL, pR, (GenericType**)&pResult);
        if (iResult)
        {
             throw ast::InternalError(errorMultiplySize(pL, pR));
        }

        return pResult;
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return NULL;

}

int MultiplyDoubleByDouble(Double* _pDouble1, Double* _pDouble2, Double** _pDoubleOut)
{
    if (_pDouble1->isScalar())
    {
        bool bComplex1  = _pDouble1->isComplex();
        bool bComplex2  = _pDouble2->isComplex();

        (*_pDoubleOut) = new Double(_pDouble2->getDims(), _pDouble2->getDimsArray(), bComplex1 | bComplex2);

        if (bComplex1 == false && bComplex2 == false)
        {
            iMultiRealScalarByRealMatrix(_pDouble1->get(0), _pDouble2->get(), _pDouble2->getSize(), 1, (*_pDoubleOut)->get());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            iMultiRealScalarByComplexMatrix(_pDouble1->get(0), _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            iMultiComplexScalarByRealMatrix(_pDouble1->get(0), _pDouble1->getImg(0), _pDouble2->get(), _pDouble2->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else //if(bComplex1 == true && bComplex2 == true)
        {
            iMultiComplexScalarByComplexMatrix(_pDouble1->get(0), _pDouble1->getImg(0), _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }

        return 0;
    }

    if (_pDouble2->isScalar())
    {
        bool bComplex1  = _pDouble1->isComplex();
        bool bComplex2  = _pDouble2->isComplex();

        (*_pDoubleOut) = new Double(_pDouble1->getDims(), _pDouble1->getDimsArray(), bComplex1 | bComplex2);

        if (bComplex1 == false && bComplex2 == false)
        {
            //Real Matrix by Real Scalar
            iMultiRealScalarByRealMatrix(_pDouble2->get(0), _pDouble1->get(), _pDouble1->getSize(), 1, (*_pDoubleOut)->get());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            //Real Matrix by Scalar Complex
            iMultiComplexScalarByRealMatrix(_pDouble2->get(0), _pDouble2->getImg(0), _pDouble1->get(), _pDouble1->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            iMultiRealScalarByComplexMatrix(_pDouble2->get(0, 0), _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else //if(bComplex1 == true && bComplex2 == true)
        {
            iMultiComplexScalarByComplexMatrix(_pDouble2->get(0, 0), _pDouble2->getImg(0, 0), _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }

        return 0;
    }

    if (_pDouble1->getDims() > 2 || _pDouble2->getDims() > 2)
    {
        //call overload
        return 0;
    }

    if (_pDouble1->getCols() != _pDouble2->getRows())
    {
        // Both matrices but with wrong dimensions: error out
        return 1;
    }

    bool bComplex1  = _pDouble1->isComplex();
    bool bComplex2  = _pDouble2->isComplex();
    (*_pDoubleOut) = new Double(_pDouble1->getRows(), _pDouble2->getCols(), bComplex1 | bComplex2);

    if (bComplex1 == false && bComplex2 == false)
    {
        //Real Matrix by Real Matrix
        iMultiRealMatrixByRealMatrix(
            _pDouble1->get(), _pDouble1->getRows(), _pDouble1->getCols(),
            _pDouble2->get(), _pDouble2->getRows(), _pDouble2->getCols(),
            (*_pDoubleOut)->get());
    }
    else if (bComplex1 == false && bComplex2 == true)
    {
        //Real Matrix by Matrix Complex
        iMultiRealMatrixByComplexMatrix(
            _pDouble1->get(), _pDouble1->getRows(), _pDouble1->getCols(),
            _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
            (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
    }
    else if (bComplex1 == true && bComplex2 == false)
    {
        //Complex Matrix by Real Matrix
        iMultiComplexMatrixByRealMatrix(
            _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
            _pDouble2->get(), _pDouble2->getRows(), _pDouble2->getCols(),
            (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
    }
    else //if(bComplex1 == true && bComplex2 == true)
    {
        //Complex Matrix by Complex Matrix
        iMultiComplexMatrixByComplexMatrix(
            _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
            _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
            (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
    }
    return 0;
}

int MultiplyDoubleByPoly(Double* _pDouble, Polynom* _pPoly, Polynom** _pPolyOut)
{
    bool bComplex1  = _pDouble->isComplex();
    bool bComplex2  = _pPoly->isComplex();

    if (_pDouble->isScalar())
    {
        int* piRank = new int[_pPoly->getSize()];
        for (int i = 0 ; i < _pPoly->getSize() ; i++)
        {
            piRank[i] = _pPoly->get(i)->getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pPoly->getDims(), _pPoly->getDimsArray(), piRank);
        delete[] piRank;
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        for (int i = 0 ; i < _pPoly->getSize() ; i++)
        {
            SinglePoly *pPolyIn     = _pPoly->get(i);
            double* pRealIn         = pPolyIn->get();
            double* pImgIn          = pPolyIn->getImg();

            SinglePoly *pPolyOut    = (*_pPolyOut)->get(i);
            double* pRealOut        = pPolyOut->get();
            double* pImgOut         = pPolyOut->getImg();

            if (bComplex1 == false && bComplex2 == false)
            {
                iMultiRealScalarByRealMatrix(_pDouble->get(0), pRealIn, 1, pPolyIn->getSize(), pRealOut);
            }
            else if (bComplex1 == false && bComplex2 == true)
            {
                iMultiRealScalarByComplexMatrix(_pDouble->get(0), pRealIn, pImgIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == false)
            {
                iMultiComplexScalarByRealMatrix(_pDouble->get(0), _pDouble->getImg(0), pRealIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == true)
            {
                iMultiComplexScalarByComplexMatrix(_pDouble->get(0), _pDouble->getImg(0), pRealIn, pImgIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
        }
        (*_pPolyOut)->updateRank();
        return 0;
    }

    if (_pPoly->isScalar())
    {
        int* piRank = new int[_pDouble->getSize()];
        for (int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            piRank[i] = _pPoly->get(0)->getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pDouble->getDims(), _pDouble->getDimsArray(), piRank);
        delete[] piRank;
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        double *pDoubleR    = _pDouble->get();
        double *pDoubleI    = _pDouble->getImg();

        SinglePoly *pPolyIn = _pPoly->get(0);
        double* pRealIn     = pPolyIn->get();
        double* pImgIn      = pPolyIn->getImg();

        for (int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            SinglePoly *pPolyOut    = (*_pPolyOut)->get(i);
            double* pRealOut        = pPolyOut->get();
            double* pImgOut         = pPolyOut->getImg();

            if (bComplex1 == false && bComplex2 == false)
            {
                iMultiRealScalarByRealMatrix(pDoubleR[i], pRealIn, 1, pPolyIn->getSize(), pRealOut);
            }
            else if (bComplex1 == false && bComplex2 == true)
            {
                iMultiRealScalarByComplexMatrix(pDoubleR[i], pRealIn, pImgIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == false)
            {
                iMultiComplexScalarByRealMatrix(pDoubleR[i], pDoubleI[i], pRealIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == true)
            {
                iMultiComplexScalarByComplexMatrix(pDoubleR[i], pDoubleI[i], pRealIn, pImgIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
        }

        (*_pPolyOut)->updateRank();
        return 0;
    }

    if (_pPoly->getDims() > 2 || _pDouble->getDims() > 2)
    {
        //call overload
        return 0;
    }

    if (_pDouble->getCols() != _pPoly->getRows())
    {
        // wrong dimensions
        return 1;
    }

    int* piRank = new int[_pDouble->getRows() * _pPoly->getCols()];
    int iMaxRank = _pPoly->getMaxRank();
    for (int i = 0 ; i < _pDouble->getRows() * _pPoly->getCols() ; i++)
    {
        piRank[i] = iMaxRank;
    }

    (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pDouble->getRows(), _pPoly->getCols(), piRank);
    delete[] piRank;
    if (bComplex1 || bComplex2)
    {
        (*_pPolyOut)->setComplex(true);
    }

    Double *pCoef = _pPoly->getCoef();
    Double *pTemp = new Double(_pDouble->getRows(), pCoef->getCols(), bComplex1 || bComplex2);

    if (bComplex1 == false && bComplex2 == false)
    {
        iMultiRealMatrixByRealMatrix(_pDouble->get(), _pDouble->getRows(), _pDouble->getCols(),
                                     pCoef->get(), pCoef->getRows(), pCoef->getCols(),
                                     pTemp->get());
    }
    else if (bComplex1 == false && bComplex2 == true)
    {
        iMultiRealMatrixByComplexMatrix(_pDouble->get(), _pDouble->getRows(), _pDouble->getCols(),
                                        pCoef->get(), pCoef->getImg(), pCoef->getRows(), pCoef->getCols(),
                                        pTemp->get(), pTemp->getImg());

    }
    else if (bComplex1 == true && bComplex2 == false)
    {
        iMultiComplexMatrixByRealMatrix(_pDouble->get(), _pDouble->getImg(), _pDouble->getRows(), _pDouble->getCols(),
                                        pCoef->get(), pCoef->getRows(), pCoef->getCols(),
                                        pTemp->get(), pTemp->getImg());
    }
    else //if(bComplex1 == true && bComplex2 == true)
    {
        iMultiComplexMatrixByComplexMatrix(_pDouble->get(), _pDouble->getImg(), _pDouble->getRows(), _pDouble->getCols(),
                                           pCoef->get(), pCoef->getImg(), pCoef->getRows(), pCoef->getCols(),
                                           pTemp->get(), pTemp->getImg());
    }

    pCoef->killMe();
    (*_pPolyOut)->setCoef(pTemp);
    (*_pPolyOut)->updateRank();
    delete pTemp;
    return 0;
}

int MultiplyPolyByDouble(Polynom* _pPoly, Double* _pDouble, Polynom **_pPolyOut)
{
    bool bComplex1  = _pPoly->isComplex();
    bool bComplex2  = _pDouble->isComplex();
    bool bScalar1   = _pPoly->isScalar();
    bool bScalar2   = _pDouble->isScalar();

    if (bScalar1)
    {
        int* piRank = new int[_pDouble->getSize()];
        for (int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            piRank[i] = _pPoly->get(0)->getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pDouble->getDims(), _pDouble->getDimsArray(), piRank);
        delete[] piRank;
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        double *pDoubleR    = _pDouble->get();
        double *pDoubleI    = _pDouble->getImg();

        SinglePoly *pPolyIn = _pPoly->get(0);
        double* pRealIn     = pPolyIn->get();
        double* pImgIn      = pPolyIn->getImg();

        for (int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            SinglePoly *pPolyOut    = (*_pPolyOut)->get(i);
            double* pRealOut        = pPolyOut->get();
            double* pImgOut         = pPolyOut->getImg();

            if (bComplex1 == false && bComplex2 == false)
            {
                iMultiRealScalarByRealMatrix(pDoubleR[i], pRealIn, 1, pPolyIn->getSize(), pRealOut);
            }
            else if (bComplex1 == false && bComplex2 == true)
            {
                iMultiComplexScalarByRealMatrix(pDoubleR[i], pDoubleI[i], pRealIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == false)
            {
                iMultiRealScalarByComplexMatrix(pDoubleR[i], pRealIn, pImgIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == true)
            {
                iMultiComplexScalarByComplexMatrix(pDoubleR[i], pDoubleI[i], pRealIn, pImgIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
        }

        (*_pPolyOut)->updateRank();
        return 0;
    }
    else if (bScalar2)
    {
        int* piRank = new int[_pPoly->getSize()];
        for (int i = 0 ; i < _pPoly->getSize() ; i++)
        {
            piRank[i] = _pPoly->get(i)->getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pPoly->getDims(), _pPoly->getDimsArray(), piRank);
        delete[] piRank;
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        for (int i = 0 ; i < _pPoly->getSize() ; i++)
        {
            SinglePoly *pPolyIn = _pPoly->get(i);
            double* pRealIn     = pPolyIn->get();
            double* pImgIn      = pPolyIn->getImg();

            SinglePoly *pPolyOut    = (*_pPolyOut)->get(i);
            double* pRealOut        = pPolyOut->get();
            double* pImgOut         = pPolyOut->getImg();

            if (bComplex1 == false && bComplex2 == false)
            {
                iMultiRealScalarByRealMatrix(_pDouble->get(0), pRealIn, 1, pPolyIn->getSize(), pRealOut);
            }
            else if (bComplex1 == false && bComplex2 == true)
            {
                iMultiComplexScalarByRealMatrix(_pDouble->get(0), _pDouble->getImg(0), pRealIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == false)
            {
                iMultiRealScalarByComplexMatrix(_pDouble->get(0), pRealIn, pImgIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == true)
            {
                iMultiComplexScalarByComplexMatrix(_pDouble->get(0), _pDouble->getImg(0), pRealIn, pImgIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
        }

        (*_pPolyOut)->updateRank();
        return 0;
    }

    if (_pDouble->getDims() > 2 || _pPoly->getDims() > 2)
    {
        //call overload
        return 0;
    }

    if (_pPoly->getCols() != _pDouble->getRows())
    {
        //call overload
        return 1;
    }

    int* piRank = new int[_pPoly->getRows() * _pDouble->getCols()];
    int iMaxRank = _pPoly->getMaxRank();
    for (int i = 0 ; i < _pPoly->getRows() * _pDouble->getCols() ; i++)
    {
        piRank[i] = iMaxRank;
    }

    (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pPoly->getRows(), _pDouble->getCols(), piRank);
    delete[] piRank;
    if (bComplex1 || bComplex2)
    {
        (*_pPolyOut)->setComplex(true);
    }

    //Distribution a la mano par appels a des sous-fonctions ( iMulti...ScalarBy...Scalar ) plus iAdd...To... )

    //for each line of _pPoly
    for (int iRow1 = 0 ; iRow1 < _pPoly->getRows() ; iRow1++)
    {
        //for each col of _pDouble
        for (int iCol2 = 0 ; iCol2 < _pDouble->getCols() ; iCol2++)
        {
            SinglePoly* pSPOut = (*_pPolyOut)->get(iRow1, iCol2);
            pSPOut->setZeros();

            //for each rows of _pDouble / cols of _pPoly
            for (int iRow2 = 0 ; iRow2 < _pDouble->getRows() ; iRow2++)
            {
                // SinglePoly(iRow1, iRow2) * Double(iRow2, iCol2)
                SinglePoly* pSPIn = _pPoly->get(iRow1, iRow2);
                int iSize = pSPIn->getSize();
                double* pdblMult = new double[iSize];

                if (bComplex1 == false && bComplex2 == false)
                {
                    //Real Matrix by Real Scalar
                    iMultiRealScalarByRealMatrix(_pDouble->get(iRow2, iCol2), pSPIn->get(), iSize, 1, pdblMult);
                    add(pSPOut->get(), (long long)iSize, pdblMult, pSPOut->get());
                }
                else if (bComplex1 == false && bComplex2 == true)
                {
                    //Real Matrix by Scalar Complex
                    double* pdblMultImg = new double[iSize];
                    iMultiComplexScalarByRealMatrix(_pDouble->get(iRow2, iCol2), _pDouble->getImg(iRow2, iCol2), pSPIn->get(), pSPIn->getSize(), 1, pdblMult, pdblMultImg);
                    add(pSPOut->get(), pSPOut->getImg(), (long long)iSize, pdblMult, pdblMultImg, pSPOut->get(), pSPOut->getImg());
                    delete[] pdblMultImg;
                }
                else if (bComplex1 == true && bComplex2 == false)
                {
                    double* pdblMultImg = new double[iSize];
                    iMultiRealScalarByComplexMatrix(_pDouble->get(iRow2, iCol2), pSPIn->get(), pSPIn->getImg(), pSPIn->getSize(), 1, pdblMult, pdblMultImg);
                    add(pSPOut->get(), pSPOut->getImg(), (long long)iSize, pdblMult, pdblMultImg, pSPOut->get(), pSPOut->getImg());
                    delete[] pdblMultImg;
                }
                else //if(bComplex1 == true && bComplex2 == true)
                {
                    double* pdblMultImg = new double[iSize];
                    iMultiComplexScalarByComplexMatrix(_pDouble->get(iRow2, iCol2), _pDouble->getImg(iRow2, iCol2), pSPIn->get(), pSPIn->getImg(), pSPIn->getSize(), 1, pdblMult, pdblMultImg);
                    add(pSPOut->get(), pSPOut->getImg(), (long long)iSize, pdblMult, pdblMultImg, pSPOut->get(), pSPOut->getImg());
                    delete[] pdblMultImg;
                }

                delete[] pdblMult;
            }//for(int iRow2 = 0 ; iRow2 < _pDouble->getRows() ; iRow2++)
        }//for(int iCol2 = 0 ; iCol2 < _pDouble->getCols() ; iCol2++)
    }//for(int iRow1 = 0 ; iRow1 < _pPoly->getRows() ; iRow1++)

    (*_pPolyOut)->updateRank();
    return 0;
}

int MultiplyPolyByPoly(Polynom* _pPoly1, Polynom* _pPoly2, Polynom** _pPolyOut)
{
    bool bComplex1  = _pPoly1->isComplex();
    bool bComplex2  = _pPoly2->isComplex();

    if (_pPoly1->isScalar() && _pPoly2->isScalar())
    {
        //poly1(0) * poly2(0)
        int iRank = _pPoly1->get(0)->getRank() + _pPoly2->get(0)->getRank();
        (*_pPolyOut) = new Polynom(_pPoly1->getVariableName(), 1, 1, &iRank);
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        if (bComplex1 == false && bComplex2 == false)
        {
            SinglePoly *pPoly1  = _pPoly1->get(0);
            SinglePoly *pPoly2  = _pPoly2->get(0);
            SinglePoly *pPolyOut = (*_pPolyOut)->get(0);

            pPolyOut->setZeros();

            iMultiScilabPolynomByScilabPolynom(
                pPoly1->get(), pPoly1->getSize(),
                pPoly2->get(), pPoly2->getSize(),
                pPolyOut->get(), pPolyOut->getSize());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            SinglePoly *pPoly1  = _pPoly1->get(0);
            SinglePoly *pPoly2  = _pPoly2->get(0);
            SinglePoly *pPolyOut = (*_pPolyOut)->get(0);

            pPolyOut->setZeros();

            iMultiScilabPolynomByComplexPoly(
                pPoly1->get(), pPoly1->getSize(),
                pPoly2->get(), pPoly2->getImg(), pPoly2->getSize(),
                pPolyOut->get(), pPolyOut->getImg(), pPolyOut->getSize());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            SinglePoly *pPoly1  = _pPoly1->get(0);
            SinglePoly *pPoly2  = _pPoly2->get(0);
            SinglePoly *pPolyOut = (*_pPolyOut)->get(0);

            pPolyOut->setZeros();

            iMultiComplexPolyByScilabPolynom(
                pPoly1->get(), pPoly1->getImg(), pPoly1->getSize(),
                pPoly2->get(), pPoly2->getSize(),
                pPolyOut->get(), pPolyOut->getImg(), pPolyOut->getSize());
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            SinglePoly *pPoly1   = _pPoly1->get(0);
            SinglePoly *pPoly2   = _pPoly2->get(0);
            SinglePoly *pPolyOut  = (*_pPolyOut)->get(0);

            pPolyOut->setZeros();

            iMultiComplexPolyByComplexPoly(
                pPoly1->get(), pPoly1->getImg(), pPoly1->getSize(),
                pPoly2->get(), pPoly2->getImg(), pPoly2->getSize(),
                pPolyOut->get(), pPolyOut->getImg(), pPolyOut->getSize());
        }

        (*_pPolyOut)->updateRank();
        return 0;
    }

    if (_pPoly1->isScalar())
    {
        //poly1(0) * poly2(n)
        int* piRank = new int[_pPoly2->getSize()];
        for (int i = 0 ; i < _pPoly2->getSize() ; i++)
        {
            piRank[i] = _pPoly1->get(0)->getRank() + _pPoly2->get(i)->getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly1->getVariableName(), _pPoly2->getDims(), _pPoly2->getDimsArray(), piRank);
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }
        delete[] piRank;


        SinglePoly *pPoly1  = _pPoly1->get(0);
        if (bComplex1 == false && bComplex2 == false)
        {
            for (int iPoly = 0 ; iPoly < _pPoly2->getSize() ; iPoly++)
            {
                SinglePoly *pPoly2  = _pPoly2->get(iPoly);
                SinglePoly *pPolyOut = (*_pPolyOut)->get(iPoly);

                pPolyOut->setZeros();

                iMultiScilabPolynomByScilabPolynom(
                    pPoly1->get(), pPoly1->getSize(),
                    pPoly2->get(), pPoly2->getSize(),
                    pPolyOut->get(), pPolyOut->getSize());
            }
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            for (int iPoly = 0 ; iPoly < _pPoly2->getSize() ; iPoly++)
            {
                SinglePoly *pPoly2  = _pPoly2->get(iPoly);
                SinglePoly *pPolyOut = (*_pPolyOut)->get(iPoly);

                pPolyOut->setZeros();

                iMultiScilabPolynomByComplexPoly(
                    pPoly1->get(), pPoly1->getSize(),
                    pPoly2->get(), pPoly2->getImg(), pPoly2->getSize(),
                    pPolyOut->get(), pPolyOut->getImg(), pPolyOut->getSize());
            }
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            for (int iPoly = 0 ; iPoly < _pPoly2->getSize() ; iPoly++)
            {
                SinglePoly *pPoly2  = _pPoly2->get(iPoly);
                SinglePoly *pPolyOut = (*_pPolyOut)->get(iPoly);

                pPolyOut->setZeros();

                iMultiComplexPolyByScilabPolynom(
                    pPoly1->get(), pPoly1->getImg(), pPoly1->getSize(),
                    pPoly2->get(), pPoly2->getSize(),
                    pPolyOut->get(), pPolyOut->getImg(), pPolyOut->getSize());
            }
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            for (int iPoly = 0 ; iPoly < _pPoly2->getSize() ; iPoly++)
            {
                SinglePoly *pPoly2   = _pPoly2->get(iPoly);
                SinglePoly *pPolyOut  = (*_pPolyOut)->get(iPoly);

                pPolyOut->setZeros();

                iMultiComplexPolyByComplexPoly(
                    pPoly1->get(), pPoly1->getImg(), pPoly1->getSize(),
                    pPoly2->get(), pPoly2->getImg(), pPoly2->getSize(),
                    pPolyOut->get(), pPolyOut->getImg(), pPolyOut->getSize());
            }
        }

        (*_pPolyOut)->updateRank();
        return 0;
    }

    if (_pPoly2->isScalar())
    {
        //poly1(n) * poly2(0)
        int* piRank = new int[_pPoly1->getSize()];
        for (int i = 0 ; i < _pPoly1->getSize() ; i++)
        {
            piRank[i] = _pPoly2->get(0)->getRank() + _pPoly1->get(i)->getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly1->getVariableName(), _pPoly1->getDims(), _pPoly1->getDimsArray(), piRank);
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }
        delete[] piRank;

        SinglePoly *pPoly2  = _pPoly2->get(0);
        if (bComplex1 == false && bComplex2 == false)
        {
            for (int iPoly = 0 ; iPoly < _pPoly1->getSize() ; iPoly++)
            {
                SinglePoly *pPoly1  = _pPoly1->get(iPoly);
                SinglePoly *pPolyOut = (*_pPolyOut)->get(iPoly);

                pPolyOut->setZeros();

                iMultiScilabPolynomByScilabPolynom(
                    pPoly1->get(), pPoly1->getSize(),
                    pPoly2->get(), pPoly2->getSize(),
                    pPolyOut->get(), pPolyOut->getSize());
            }
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            for (int iPoly = 0 ; iPoly < _pPoly1->getSize() ; iPoly++)
            {
                SinglePoly *pPoly1  = _pPoly1->get(iPoly);
                SinglePoly *pPolyOut = (*_pPolyOut)->get(iPoly);

                pPolyOut->setZeros();

                iMultiScilabPolynomByComplexPoly(
                    pPoly1->get(), pPoly1->getSize(),
                    pPoly2->get(), pPoly2->getImg(), pPoly2->getSize(),
                    pPolyOut->get(), pPolyOut->getImg(), pPolyOut->getSize());
            }
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            for (int iPoly = 0 ; iPoly < _pPoly1->getSize() ; iPoly++)
            {
                SinglePoly *pPoly1  = _pPoly1->get(iPoly);
                SinglePoly *pPolyOut = (*_pPolyOut)->get(iPoly);

                pPolyOut->setZeros();

                iMultiComplexPolyByScilabPolynom(
                    pPoly1->get(), pPoly1->getImg(), pPoly1->getSize(),
                    pPoly2->get(), pPoly2->getSize(),
                    pPolyOut->get(), pPolyOut->getImg(), pPolyOut->getSize());
            }
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            for (int iPoly = 0 ; iPoly < _pPoly1->getSize() ; iPoly++)
            {
                SinglePoly *pPoly1   = _pPoly1->get(iPoly);
                SinglePoly *pPolyOut  = (*_pPolyOut)->get(iPoly);

                pPolyOut->setZeros();

                iMultiComplexPolyByComplexPoly(
                    pPoly1->get(), pPoly1->getImg(), pPoly1->getSize(),
                    pPoly2->get(), pPoly2->getImg(), pPoly2->getSize(),
                    pPolyOut->get(), pPolyOut->getImg(), pPolyOut->getSize());
            }
        }

        (*_pPolyOut)->updateRank();
        return 0;
    }

    if (_pPoly1->getDims() > 2 || _pPoly2->getDims() > 2)
    {
        //call overload
        return 0;
    }

    if (_pPoly1->getCols() != _pPoly2->getRows())
    {
        // wrong dimensions
        return 1;
    }

    // matrix by matrix
    int* piRank = new int[_pPoly1->getRows() * _pPoly2->getCols()];
    int iMaxRank = _pPoly1->getMaxRank() + _pPoly2->getMaxRank();
    for (int i = 0 ; i < _pPoly1->getRows() * _pPoly2->getCols() ; i++)
    {
        piRank[i] = iMaxRank;
    }

    (*_pPolyOut) = new Polynom(_pPoly1->getVariableName(), _pPoly1->getRows(), _pPoly2->getCols(), piRank);
    if (bComplex1 || bComplex2)
    {
        (*_pPolyOut)->setComplex(true);
    }

    delete[] piRank;


    if (bComplex1 == false && bComplex2 == false)
    {
        double *pReal = NULL;
        SinglePoly *pTemp  = new SinglePoly(&pReal, (*_pPolyOut)->getMaxRank());

        for (int iRow = 0 ; iRow < _pPoly1->getRows() ; iRow++)
        {
            for (int iCol = 0 ; iCol < _pPoly2->getCols() ; iCol++)
            {
                SinglePoly *pResult = (*_pPolyOut)->get(iRow, iCol);
                pResult->setZeros();

                for (int iCommon = 0 ; iCommon < _pPoly1->getCols() ; iCommon++)
                {
                    SinglePoly *pL   = _pPoly1->get(iRow, iCommon);
                    SinglePoly *pR   = _pPoly2->get(iCommon, iCol);

                    pTemp->setZeros();

                    iMultiScilabPolynomByScilabPolynom(
                        pL->get(), pL->getSize(),
                        pR->get(), pR->getSize(),
                        pTemp->get(), pL->getRank() + pR->getRank() + 1);

                    iAddScilabPolynomToScilabPolynom(
                        pResult->get(), pResult->getSize(),
                        pTemp->get(), pResult->getSize(),
                        pResult->get(), pResult->getSize());
                }
            }
        }

        pTemp->killMe();
    }
    else if (bComplex1 == false && bComplex2 == true)
    {
        double *pReal = NULL;
        double *pImg = NULL;
        SinglePoly *pTemp  = new SinglePoly(&pReal, &pImg, (*_pPolyOut)->getMaxRank());

        for (int iRow = 0 ; iRow < _pPoly1->getRows() ; iRow++)
        {
            for (int iCol = 0 ; iCol < _pPoly2->getCols() ; iCol++)
            {
                SinglePoly *pResult = (*_pPolyOut)->get(iRow, iCol);
                pResult->setZeros();

                for (int iCommon = 0 ; iCommon < _pPoly1->getCols() ; iCommon++)
                {
                    SinglePoly *pL   = _pPoly1->get(iRow, iCommon);
                    SinglePoly *pR   = _pPoly2->get(iCommon, iCol);

                    pTemp->setZeros();

                    iMultiScilabPolynomByComplexPoly(
                        pL->get(), pL->getSize(),
                        pR->get(), pR->getImg(), pR->getSize(),
                        pTemp->get(), pTemp->getImg(), pL->getRank() + pR->getRank() + 1);

                    iAddComplexPolyToComplexPoly(
                        pResult->get(), pResult->getImg(), pResult->getSize(),
                        pTemp->get(), pTemp->getImg(), pResult->getSize(),
                        pResult->get(), pResult->getImg(), pResult->getSize());
                }
            }
        }

        pTemp->killMe();
    }
    else if (bComplex1 == true && bComplex2 == false)
    {
        double *pReal = NULL;
        double *pImg = NULL;
        SinglePoly *pTemp  = new SinglePoly(&pReal, &pImg, (*_pPolyOut)->getMaxRank());

        for (int iRow = 0 ; iRow < _pPoly1->getRows() ; iRow++)
        {
            for (int iCol = 0 ; iCol < _pPoly2->getCols() ; iCol++)
            {
                SinglePoly *pResult = (*_pPolyOut)->get(iRow, iCol);
                pResult->setZeros();

                for (int iCommon = 0 ; iCommon < _pPoly1->getCols() ; iCommon++)
                {
                    SinglePoly *pL   = _pPoly1->get(iRow, iCommon);
                    SinglePoly *pR   = _pPoly2->get(iCommon, iCol);

                    pTemp->setZeros();

                    iMultiScilabPolynomByComplexPoly(
                        pR->get(), pR->getSize(),
                        pL->get(), pL->getImg(), pL->getSize(),
                        pTemp->get(), pTemp->getImg(), pL->getRank() + pR->getRank() + 1);

                    iAddComplexPolyToComplexPoly(
                        pResult->get(), pResult->getImg(), pResult->getSize(),
                        pTemp->get(), pTemp->getImg(), pResult->getSize(),
                        pResult->get(), pResult->getImg(), pResult->getSize());
                }
            }
        }

        pTemp->killMe();
    }
    else if (bComplex1 == true && bComplex2 == true)
    {
        double *pReal = NULL;
        double *pImg = NULL;
        SinglePoly *pTemp  = new SinglePoly(&pReal, &pImg, (*_pPolyOut)->getMaxRank());

        for (int iRow = 0 ; iRow < _pPoly1->getRows() ; iRow++)
        {
            for (int iCol = 0 ; iCol < _pPoly2->getCols() ; iCol++)
            {
                SinglePoly *pResult = (*_pPolyOut)->get(iRow, iCol);
                pResult->setZeros();

                for (int iCommon = 0 ; iCommon < _pPoly1->getCols() ; iCommon++)
                {
                    SinglePoly *pL   = _pPoly1->get(iRow, iCommon);
                    SinglePoly *pR   = _pPoly2->get(iCommon, iCol);

                    pTemp->setZeros();

                    iMultiComplexPolyByComplexPoly(
                        pL->get(), pL->getImg(), pL->getSize(),
                        pR->get(), pR->getImg(), pR->getSize(),
                        pTemp->get(), pTemp->getImg(), pL->getRank() + pR->getRank() + 1);

                    iAddComplexPolyToComplexPoly(
                        pResult->get(), pResult->getImg(), pResult->getSize(),
                        pTemp->get(), pTemp->getImg(), pResult->getSize(),
                        pResult->get(), pResult->getImg(), pResult->getSize());
                }
            }
        }

        pTemp->killMe();
    }
    (*_pPolyOut)->updateRank();

    return 0;
}

int MultiplySparseBySparse(Sparse* _pSparse1, Sparse* _pSparse2, Sparse** _pSparseOut)
{
    if (_pSparse1->isScalar())
    {
        //scalar * sp
        Double* pDbl = NULL;
        if (_pSparse1->isComplex())
        {
            std::complex<double> dbl = _pSparse1->getImg(0, 0);
            pDbl = new Double(dbl.real(), dbl.imag());
        }
        else
        {
            pDbl = new Double(_pSparse1->get(0, 0));
        }

        MultiplyDoubleBySparse(pDbl, _pSparse2, (GenericType**)_pSparseOut);
        delete pDbl;
        return 0;
    }

    if (_pSparse2->isScalar())
    {
        //sp * scalar
        Double* pDbl = NULL;
        if (_pSparse2->isComplex())
        {
            std::complex<double> dbl = _pSparse2->getImg(0, 0);
            pDbl = new Double(dbl.real(), dbl.imag());
        }
        else
        {
            pDbl = new Double(_pSparse2->get(0, 0));
        }

        MultiplySparseByDouble(_pSparse1, pDbl, (GenericType**)_pSparseOut);
        delete pDbl;
        return 0;
    }

    if (_pSparse1->getCols() != _pSparse2->getRows())
    {
        return 1;
    }

    *_pSparseOut = _pSparse1->multiply(*_pSparse2);
    return 0;
}

int MultiplyDoubleBySparse(Double* _pDouble, Sparse *_pSparse, GenericType** _pOut)
{
    //D * SP
    if (_pDouble->isScalar())
    {
        //d * SP -> SP
        Sparse* pOut = NULL;
        if (_pDouble->isComplex())
        {
            std::complex<double> dbl(_pDouble->get(0), _pDouble->getImg(0));
            pOut = _pSparse->multiply(dbl);
        }
        else
        {
            pOut = _pSparse->multiply(_pDouble->get(0));
        }
        *_pOut = pOut;
        return 0;
    }

    if (_pSparse->isScalar())
    {
        //D * sp -> D .* d
        Double* pD = NULL;

        if (_pSparse->isComplex())
        {
            std::complex<double> dbl(_pSparse->getImg(0, 0));
            pD = new Double(dbl.real(), dbl.imag());
        }
        else
        {
            pD = new Double(_pSparse->get(0, 0));
        }

        InternalType* pIT = GenericDotTimes(_pDouble, pD);
        *_pOut = pIT->getAs<GenericType>();
        delete pD;
        return 0;
    }

    if (_pDouble->getDims() > 2)
    {
        //call overload
        return 0;
    }

    if (_pDouble->getCols() != _pSparse->getRows())
    {
        return 1;
    }

    //try to be smart and use Eigen3 library Map and View features

    Double* pOut = new Double(_pDouble->getRows(), _pSparse->getCols(), _pDouble->isComplex() | _pSparse->isComplex());
    pOut->setZeros();
    
    Eigen::Map<Eigen::MatrixXd> mdblR(_pDouble->get(),_pDouble->getRows(),_pDouble->getCols());
    Eigen::Map<Eigen::MatrixXd> mdblI(_pDouble->getImg(),_pDouble->getRows(),_pDouble->getCols());
    Eigen::Map<Eigen::MatrixXd> moutR(pOut->get(),_pDouble->getRows(), _pSparse->getCols());
    Eigen::Map<Eigen::MatrixXd> moutI(pOut->getImg(),_pDouble->getRows(), _pSparse->getCols());

    if (_pDouble->isComplex() == false && _pSparse->isComplex() == false)
    {
        moutR = mdblR * *(_pSparse->matrixReal);
    }
    else if (_pDouble->isComplex() == false && _pSparse->isComplex() == true)
    {
        // a*sparsecomplex(b,c) -> complex(a*b,a*c)
        moutR =  mdblR * _pSparse->matrixCplx->real();
        moutI =  mdblR * _pSparse->matrixCplx->imag();      
    }
    else if (_pDouble->isComplex() == true && _pSparse->isComplex() == false)
    {
        // complex(a,b)*sparse(c) -> complex(a*c,b*c)
        moutR = mdblR * *(_pSparse->matrixReal);
        moutI = mdblI * *(_pSparse->matrixReal);
    }
    else if (_pDouble->isComplex() == true && _pSparse->isComplex() == true)
    {
        // complex(a,b)*sparsecomplex(c,d) -> complex(a*c-b*d,b*c+a*d)
        moutR = mdblR * _pSparse->matrixCplx->real() - mdblI * _pSparse->matrixCplx->imag();
        moutI = mdblI * _pSparse->matrixCplx->real() + mdblR * _pSparse->matrixCplx->imag();
    }

    *_pOut = pOut;

    return 0;
}

int MultiplySparseByDouble(Sparse *_pSparse, Double*_pDouble, GenericType** _pOut)
{
    if (_pDouble->isScalar())
    {
        //SP * d -> SP
        Sparse* pOut = NULL;
        if (_pDouble->isComplex())
        {
            std::complex<double> dbl(_pDouble->get(0), _pDouble->getImg(0));
            pOut = _pSparse->multiply(dbl);
        }
        else
        {
            pOut = _pSparse->multiply(_pDouble->get(0));
        }
        *_pOut = pOut;
        return 0;
    }

    if (_pSparse->isScalar())
    {
        //D * sp -> D .* d
        Double* pD = NULL;

        if (_pSparse->isComplex())
        {
            std::complex<double> dbl(_pSparse->getImg(0, 0));
            pD = new Double(dbl.real(), dbl.imag());
        }
        else
        {
            pD = new Double(_pSparse->get(0, 0));
        }

        InternalType* pIT = GenericDotTimes(_pDouble, pD);
        *_pOut = pIT->getAs<GenericType>();
        delete pD;
        return 0;
    }

    if (_pDouble->getDims() > 2)
    {
        //call overload
        return 0;
    }

    if (_pSparse->getCols() != _pDouble->getRows())
    {
        return 1;
    }

    //try to be smart and use Eigen3 library Map and View features

    Double* pOut = new Double(_pSparse->getRows(), _pDouble->getCols(), _pDouble->isComplex() | _pSparse->isComplex());
    pOut->setZeros();
    
    Eigen::Map<Eigen::MatrixXd> mdblR(_pDouble->get(),_pDouble->getRows(),_pDouble->getCols());
    Eigen::Map<Eigen::MatrixXd> mdblI(_pDouble->getImg(),_pDouble->getRows(),_pDouble->getCols());
    Eigen::Map<Eigen::MatrixXd> moutR(pOut->get(),_pSparse->getRows(), _pDouble->getCols());
    Eigen::Map<Eigen::MatrixXd> moutI(pOut->getImg(),_pSparse->getRows(), _pDouble->getCols());

    if (_pDouble->isComplex() == false && _pSparse->isComplex() == false)
    {
        moutR = *(_pSparse->matrixReal) * mdblR;
    }
    else if (_pDouble->isComplex() == false && _pSparse->isComplex() == true)
    {
        // sparsecomplex(a,b) * c -> complex(a*c,b*c)
        moutR = _pSparse->matrixCplx->real() * mdblR;
        moutI = _pSparse->matrixCplx->imag() * mdblR;        
    }
    else if (_pDouble->isComplex() == true && _pSparse->isComplex() == false)
    {
        // sparse(a)*complex(b,c) -> complex(a*b,a*c)
        moutR = *(_pSparse->matrixReal) * mdblR;
        moutI = *(_pSparse->matrixReal) * mdblI;
    }
    else if (_pDouble->isComplex() == true && _pSparse->isComplex() == true)
    {
        // sparsecomplex(a,b)*complex(c,d) -> complex(a*c-b*d,b*c+a*d)
        moutR = _pSparse->matrixCplx->real() * mdblR - _pSparse->matrixCplx->imag()*mdblI;
        moutI = _pSparse->matrixCplx->real() * mdblI + _pSparse->matrixCplx->imag()*mdblR;
    }

    *_pOut = pOut;

    return 0;
}


template<class T, class U, class O>
InternalType* intmul_M_M2(T* _pL, U* _pR)
{
    if (_pR->getRows() != _pL->getCols())
    {
        return nullptr;
    }

    O* pOut = new O(_pL->getRows(), _pR->getCols());

    typename T::type* l = _pL->get();
    typename U::type* r = _pR->get();
    typename O::type* o = pOut->get();

    for (int i = 0; i < _pL->getRows(); ++i)
    {
        for (int j = 0; j < _pR->getCols(); ++j)
        {
            o[i + j * _pL->getRows()] = 0;
            for (int k = 0; k < _pL->getCols(); ++k)
            {
                o[i + j * _pL->getRows()] += l[i + k * _pL->getRows()] * r[k + j * _pR->getRows()];
            }
        }
    }

    return pOut;
}

template<class T, class U, class O>
InternalType* intmul_M_M(T* _pL, U* _pR)
{
    if (_pL->getDims() > 2 || _pR->getDims() > 2)
    {
        return nullptr;
    }

    if (_pR->getRows() != _pL->getCols())
    {
        return nullptr;
    }

    O* pOut = new O(_pL->getRows(), _pR->getCols());
    Eigen::Map<const Eigen::Matrix<typename T::type, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>> int1Map(_pL->get(), _pL->getRows(), _pL->getCols());
    Eigen::Map<const Eigen::Matrix<typename U::type, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>> int2Map(_pR->get(), _pR->getRows(), _pR->getCols());
    Eigen::Map<Eigen::Matrix<typename O::type, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>> intOutMap(pOut->get(), _pL->getRows(), _pR->getCols());
    intOutMap = int1Map * int2Map;
    return pOut;
}

template<class T, class U, class O>
InternalType* intmul_M_S(T* _pL, U* _pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    typename T::type* l = _pL->get();
    typename O::type* o = pOut->get();
    typename O::type r = _pR->get(0);
    for (int i = 0; i < _pL->getSize(); ++i)
    {
        o[i] = l[i] * r;
    }

    return pOut;
}

template<class T, class U, class O>
InternalType* intmul_S_M(T* _pL, U* _pR)
{
    return intmul_M_S<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* intmul_S_S(T* _pL, U* _pR)
{
    return new O(_pL->get(0) * _pR->get(0));
}
