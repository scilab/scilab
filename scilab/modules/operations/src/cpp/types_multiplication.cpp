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

#include "types_multiplication.hxx"
#include "types_addition.hxx"
#include "arrayof.hxx"

#include "scilabexception.hxx"

extern "C"
{
#include "matrix_multiplication.h"
#include "matrix_addition.h"
#include "operation_f.h"
#include "localization.h"
#include "charEncoding.h"
}

using namespace types;

InternalType *GenericDotTimes(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;

    /*
    ** DOUBLE .* DOUBLE
    */
    if(_pLeftOperand->isDouble() && _pRightOperand->isDouble())
    {
        Double *pL			= _pLeftOperand->getAs<Double>();
        Double *pR			= _pRightOperand->getAs<Double>();

        int iResult = DotMultiplyDoubleByDouble(pL, pR, (Double**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** SPARSE .* SPARSE
    */
    if(_pLeftOperand->isSparse() && _pRightOperand->isSparse())
    {
        Sparse *pL			= _pLeftOperand->getAs<Sparse>();
        Sparse *pR			= _pRightOperand->getAs<Sparse>();

        int iResult = DotMultiplySparseBySparse(pL, pR, (Sparse**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** SPARSE .* DOUBLE
    */
    if(_pLeftOperand->isSparse() && _pRightOperand->isDouble())
    {
        Sparse *pL			= _pLeftOperand->getAs<Sparse>();
        Double *pR			= _pRightOperand->getAs<Double>();

        int iResult = DotMultiplySparseByDouble(pL, pR, (GenericType**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** DOUBLE .* SPARSE
    */
    if(_pLeftOperand->isDouble() && _pRightOperand->isSparse())
    {
        Double *pL			= _pLeftOperand->getAs<Double>();
        Sparse *pR			= _pRightOperand->getAs<Sparse>();

        int iResult = DotMultiplyDoubleBySparse(pL, pR, (GenericType**)&pResult);
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

InternalType *GenericTimes(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;
    GenericType::RealType TypeL = _pLeftOperand->getType();
    GenericType::RealType TypeR = _pRightOperand->getType();

    /*
    ** DOUBLE * DOUBLE
    */
    if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
    {
        Double *pL			= _pLeftOperand->getAs<Double>();
        Double *pR			= _pRightOperand->getAs<Double>();

        int iResult = MultiplyDoubleByDouble(pL, pR, (Double**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** DOUBLE * POLY
    */
    else if(TypeL == InternalType::RealDouble && TypeR == InternalType::RealPoly)
    {
        Double *pL			= _pLeftOperand->getAs<Double>();
        Polynom *pR	    = _pRightOperand->getAs<types::Polynom>();

        int iResult = MultiplyDoubleByPoly(pL, pR, (Polynom**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** POLY * DOUBLE
    */
    else if(TypeL == InternalType::RealPoly && TypeR == InternalType::RealDouble)
    {
        Polynom *pL          = _pLeftOperand->getAs<types::Polynom>();
        Double *pR              = _pRightOperand->getAs<Double>();

        int iResult = MultiplyPolyByDouble(pL, pR, (Polynom**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** POLY * POLY
    */
    else if(TypeL == InternalType::RealPoly && TypeR == InternalType::RealPoly)
    {
        Polynom *pL          = _pLeftOperand->getAs<types::Polynom>();
        Polynom *pR          = _pRightOperand->getAs<types::Polynom>();

        int iResult = MultiplyPolyByPoly(pL, pR, (Polynom**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** SPARSE * SPARSE
    */
    if(TypeL == GenericType::RealSparse && TypeR == GenericType::RealSparse)
    {
        Sparse *pL			= _pLeftOperand->getAs<Sparse>();
        Sparse *pR			= _pRightOperand->getAs<Sparse>();

        int iResult = MultiplySparseBySparse(pL, pR, (Sparse**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** DOUBLE * SPARSE
    */
    if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealSparse)
    {
        Double *pL			= _pLeftOperand->getAs<Double>();
        Sparse *pR			= _pRightOperand->getAs<Sparse>();

        int iResult = MultiplyDoubleBySparse(pL, pR, (GenericType**)&pResult);
        if(iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** SPARSE * DOUBLE
    */
    if(TypeL == GenericType::RealSparse && TypeR == GenericType::RealDouble)
    {
        Sparse *pL			= _pLeftOperand->getAs<Sparse>();
        Double *pR			= _pRightOperand->getAs<Double>();

        int iResult = MultiplySparseByDouble(pL, pR, (GenericType**)&pResult);
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

int MultiplyDoubleByDouble(Double* _pDouble1, Double* _pDouble2, Double** _pDoubleOut)
{
    bool bComplex1  = _pDouble1->isComplex();
    bool bComplex2  = _pDouble2->isComplex();
    bool bScalar1   = _pDouble1->getRows() == 1 && _pDouble1->getCols() == 1;
    bool bScalar2   = _pDouble2->getRows() == 1 && _pDouble2->getCols() == 1;

    int iRowResult  = 0;
    int iColResult  = 0;

    if(bScalar1)
    {
        iRowResult = _pDouble2->getRows();
        iColResult = _pDouble2->getCols();
    }
    else if(bScalar2)
    {
        iRowResult = _pDouble1->getRows();
        iColResult = _pDouble1->getCols();
    }
    else if(_pDouble1->getCols() == _pDouble2->getRows())
    {
        iRowResult = _pDouble1->getRows();
        iColResult = _pDouble2->getCols();
    }
    else
    {
        return 1;
    }

    //Output variables
    bool bComplexOut    = bComplex1 || bComplex2;
    (*_pDoubleOut)      = new Double(iRowResult, iColResult, bComplexOut);

    double *pReal       = (*_pDoubleOut)->getReal();
    double *pImg        = (*_pDoubleOut)->getImg();

    if(bScalar1)
    {//cst*b
        if(bComplex1 == false && bComplex2 == false)
        {
            iMultiRealScalarByRealMatrix(_pDouble1->getReal(0,0), _pDouble2->getReal(), iRowResult, iColResult, pReal);
        }
        else if(bComplex1 == false && bComplex2 == true)
        {
            iMultiRealScalarByComplexMatrix(_pDouble1->getReal(0,0), _pDouble2->getReal(), _pDouble2->getImg(), iRowResult, iColResult, pReal, pImg);
        }
        else if(bComplex1 == true && bComplex2 == false)
        {
            iMultiComplexScalarByRealMatrix(_pDouble1->getReal(0,0), _pDouble1->getImg(0,0), _pDouble2->getReal(), iRowResult, iColResult, pReal, pImg);
        }
        else //if(bComplex1 == true && bComplex2 == true)
        {
            iMultiComplexScalarByComplexMatrix(_pDouble1->getReal(0,0), _pDouble1->getImg(0,0), _pDouble2->getReal(), _pDouble2->getImg(), iRowResult, iColResult, pReal, pImg);
        }
    }
    else if(bScalar2)
    {//a * cst
        if(bComplex1 == false && bComplex2 == false)
        {//Real Matrix by Real Scalar
            iMultiRealScalarByRealMatrix(_pDouble2->getReal(0,0), _pDouble1->getReal(), iRowResult, iColResult, pReal);
        }
        else if(bComplex1 == false && bComplex2 == true)
        {//Real Matrix by Scalar Complex
            iMultiComplexScalarByRealMatrix(_pDouble2->getReal(0,0), _pDouble2->getImg(0,0), _pDouble1->getReal(), iRowResult, iColResult, pReal, pImg);
        }
        else if(bComplex1 == true && bComplex2 == false)
        {
            iMultiRealScalarByComplexMatrix(_pDouble2->getReal(0,0), _pDouble1->getReal(), _pDouble1->getImg(), iRowResult, iColResult, pReal, pImg);
        }
        else //if(bComplex1 == true && bComplex2 == true)
        {
            iMultiComplexScalarByComplexMatrix(_pDouble2->getReal(0,0), _pDouble2->getImg(0,0), _pDouble1->getReal(), _pDouble1->getImg(), iRowResult, iColResult, pReal, pImg);
        }
    }
    else if(_pDouble1->getCols() == _pDouble2->getRows())
    {//a * b
        if(bComplex1 == false && bComplex2 == false)
        {//Real Matrix by Real Matrix
            iMultiRealMatrixByRealMatrix(
                _pDouble1->getReal(), _pDouble1->getRows(), _pDouble1->getCols(),
                _pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getCols(),
                pReal);
        }
        else if(bComplex1 == false && bComplex2 == true)
        {//Real Matrix by Matrix Complex
            iMultiRealMatrixByComplexMatrix(
                _pDouble1->getReal(), _pDouble1->getRows(), _pDouble1->getCols(),
                _pDouble2->getReal(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
                pReal, pImg);
        }
        else if(bComplex1 == true && bComplex2 == false)
        {//Complex Matrix by Real Matrix
            iMultiComplexMatrixByRealMatrix(
                _pDouble1->getReal(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
                _pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getCols(),
                pReal, pImg);
        }
        else //if(bComplex1 == true && bComplex2 == true)
        {//Complex Matrix by Complex Matrix
            iMultiComplexMatrixByComplexMatrix(
                _pDouble1->getReal(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
                _pDouble2->getReal(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
                pReal, pImg);
        }
    }
    else
    {
        return 1;
    }
    return 0;
}

int DotMultiplyDoubleByDouble(Double* _pDouble1, Double* _pDouble2, Double**	 _pDoubleOut)
{
    bool bComplex1  = _pDouble1->isComplex();
    bool bComplex2  = _pDouble2->isComplex();
    bool bScalar1   = _pDouble1->getRows() == 1 && _pDouble1->getCols() == 1;
    bool bScalar2   = _pDouble2->getRows() == 1 && _pDouble2->getCols() == 1;

    int iRowResult  = 0;
    int iColResult  = 0;

    if(bScalar1)
    {
        iRowResult = _pDouble2->getRows();
        iColResult = _pDouble2->getCols();
    }
    else if(bScalar2)
    {
        iRowResult = _pDouble1->getRows();
        iColResult = _pDouble1->getCols();
    }
    else if(_pDouble1->getRows() == _pDouble2->getRows() && _pDouble1->getCols() == _pDouble2->getCols())
    {
        iRowResult = _pDouble2->getRows();
        iColResult = _pDouble2->getCols();
    }
    else
    {//Error
        return 1;
    }

    bool bComplexOut    = bComplex1 || bComplex2;
    (*_pDoubleOut)      = new Double(iRowResult, iColResult, bComplexOut);

    double *pReal       = (*_pDoubleOut)->getReal();
    double *pImg        = (*_pDoubleOut)->getImg();

    if(bScalar1)
    {
        if(bComplex1 == false && bComplex2 == false)
        {
            iMultiRealScalarByRealMatrix(_pDouble1->getReal(0,0), _pDouble2->getReal(), iRowResult, iColResult, pReal);
        }
        else if(bComplex1 == false && bComplex2 == true)
        {
            iMultiRealScalarByComplexMatrix(_pDouble1->getReal(0,0), _pDouble2->getReal(), _pDouble2->getImg(), iRowResult, iColResult, pReal, pImg);
        }
        else if(bComplex1 == true && bComplex2 == false)
        {
            iMultiComplexScalarByRealMatrix(_pDouble1->getReal(0,0), _pDouble1->getImg(0,0), _pDouble2->getReal(), iRowResult, iColResult, pReal, pImg);
        }
        else //if(bComplex1 == true && bComplex2 == true)
        {
            iMultiComplexScalarByComplexMatrix(_pDouble1->getReal(0,0), _pDouble1->getImg(0,0), _pDouble2->getReal(), _pDouble2->getImg(), iRowResult, iColResult, pReal, pImg);
        }
    }
    else if(bScalar2)
    {
        if(bComplex1 == false && bComplex2 == false)
        {//Real Matrix by Real Scalar
            iMultiRealScalarByRealMatrix(_pDouble2->getReal(0,0), _pDouble1->getReal(), iRowResult, iColResult, pReal);
        }
        else if(bComplex1 == false && bComplex2 == true)
        {//Real Matrix by Scalar Complex
            iMultiComplexScalarByRealMatrix(_pDouble2->getReal(0,0), _pDouble2->getImg(0,0), _pDouble1->getReal(), iRowResult, iColResult, pReal, pImg);
        }
        else if(bComplex1 == true && bComplex2 == false)
        {
            iMultiRealScalarByComplexMatrix(_pDouble2->getReal(0,0), _pDouble1->getReal(), _pDouble1->getImg(), iRowResult, iColResult, pReal, pImg);
        }
        else //if(bComplex1 == true && bComplex2 == true)
        {
            iMultiComplexScalarByComplexMatrix(_pDouble2->getReal(0,0), _pDouble2->getImg(0,0), _pDouble1->getReal(), _pDouble1->getImg(), iRowResult, iColResult, pReal, pImg);
        }
    }
    else
    {//multiply element wise element
        if(bComplex1 == false && bComplex2 == false)
        {
            iDotMultiplyRealMatrixByRealMatrix(_pDouble1->getReal(), _pDouble2->getReal(), pReal, iRowResult, iColResult);
        }
        else if(bComplex1 == false && bComplex2 == true)
        {
            iDotMultiplyRealMatrixByComplexMatrix(_pDouble1->getReal(), _pDouble2->getReal(), _pDouble2->getImg(), pReal, pImg, iRowResult, iColResult);
        }
        else if(bComplex1 == true && bComplex2 == false)
        {
            iDotMultiplyComplexMatrixByRealMatrix(_pDouble1->getReal(), _pDouble1->getImg(), _pDouble2->getReal(), pReal, pImg, iRowResult, iColResult);
        }
        else //if(bComplex1 == true && bComplex2 == true)
        {
            iDotMultiplyComplexMatrixByComplexMatrix(_pDouble1->getReal(), _pDouble1->getImg(), _pDouble2->getReal(), _pDouble2->getImg(), pReal, pImg, iRowResult, iColResult);
        }
    }

    return 0;
}
int MultiplyDoubleByPoly(Double* _pDouble, Polynom* _pPoly, Polynom** _pPolyOut)
{
    bool bComplex1  = _pDouble->isComplex();
    bool bComplex2  = _pPoly->isComplex();
    bool bScalar1   = _pDouble->getRows() == 1 && _pDouble->getCols() == 1;
    bool bScalar2   = _pPoly->getRows() == 1 && _pPoly->getCols() == 1;

    int iRowResult  = 0;
    int iColResult  = 0;
    int *piRank     = NULL;

    if(bScalar1)
    {
        iRowResult = _pPoly->getRows();
        iColResult = _pPoly->getCols();

        piRank = new int[iRowResult * iColResult];
        for(int i = 0 ; i < iRowResult * iColResult ; i++)
        {
            piRank[i] = _pPoly->get(i)->getRank();
        }
    }
    else if (bScalar2)
    {
        iRowResult = _pDouble->getRows();
        iColResult = _pDouble->getCols();

        piRank = new int[iRowResult * iColResult];
        for(int i = 0 ; i < iRowResult * iColResult ; i++)
        {
            piRank[i] = _pPoly->get(0)->getRank();
        }
    }
    else if(_pDouble->getCols() == _pPoly->getRows())
    {
        iRowResult = _pDouble->getRows();
        iColResult = _pPoly->getCols();
        piRank = new int[iRowResult * iColResult];
        for(int i = 0 ; i < iRowResult * iColResult ; i++)
        {
            piRank[i] = _pPoly->getMaxRank();
        }
    }
    else
    {
        return 1;
    }

    (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), iRowResult, iColResult, piRank);
    delete[] piRank;
    if(bComplex1 || bComplex2)
    {
        (*_pPolyOut)->setComplex(true);
    }

    if(bScalar1)
    {//cst * p
        for(int i = 0 ; i < _pPoly->getSize() ; i++)
        {
            SinglePoly *pPolyIn     = _pPoly->get(i);
            double* pRealIn         = pPolyIn->getCoef()->getReal();
            double* pImgIn          = pPolyIn->getCoef()->getImg();

            SinglePoly *pPolyOut    = (*_pPolyOut)->get(i);
            double* pRealOut        = pPolyOut->getCoef()->getReal();
            double* pImgOut         = pPolyOut->getCoef()->getImg();

            if(bComplex1 == false && bComplex2 == false)
            {
                iMultiRealScalarByRealMatrix(
                    _pDouble->getReal(0,0),
                    pRealIn, 1, pPolyIn->getRank(),
                    pRealOut);
            }
            else if(bComplex1 == false && bComplex2 == true)
            {
                iMultiRealScalarByComplexMatrix(
                    _pDouble->getReal(0,0),
                    pRealIn, pImgIn, 1, pPolyIn->getRank(),
                    pRealOut, pImgOut);
            }
            else if(bComplex1 == true && bComplex2 == false)
            {
                iMultiComplexScalarByRealMatrix(
                    _pDouble->getReal(0,0), _pDouble->getImg(0,0),
                    pRealIn, 1, pPolyIn->getRank(),
                    pRealOut, pImgOut);
            }
            else if(bComplex1 == true && bComplex2 == true)
            {
                iMultiComplexScalarByComplexMatrix(
                    _pDouble->getReal(0,0), _pDouble->getImg(0,0),
                    pRealIn, pImgIn, 1, pPolyIn->getRank(),
                    pRealOut, pImgOut);
            }
        }
    }
    else if(bScalar2)
    {
        double *pDoubleR    = _pDouble->getReal();
        double *pDoubleI    = _pDouble->getImg();

        SinglePoly *pPolyIn = _pPoly->get(0);
        double* pRealIn     = pPolyIn->getCoef()->getReal();
        double* pImgIn      = pPolyIn->getCoef()->getImg();

        for(int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            SinglePoly *pPolyOut    = (*_pPolyOut)->get(i);
            double* pRealOut        = pPolyOut->getCoef()->getReal();
            double* pImgOut         = pPolyOut->getCoef()->getImg();

            if(bComplex1 == false && bComplex2 == false)
            {
                iMultiRealScalarByRealMatrix(
                    pDoubleR[i],
                    pRealIn, 1, pPolyIn->getRank(),
                    pRealOut);
            }
            else if(bComplex1 == false && bComplex2 == true)
            {
                iMultiRealScalarByComplexMatrix(
                    pDoubleR[i],
                    pRealIn, pImgIn, 1, pPolyIn->getRank(),
                    pRealOut, pImgOut);
            }
            else if(bComplex1 == true && bComplex2 == false)
            {
                iMultiComplexScalarByRealMatrix(
                    pDoubleR[i], pDoubleI[i],
                    pRealIn, 1, pPolyIn->getRank(),
                    pRealOut, pImgOut);
            }
            else if(bComplex1 == true && bComplex2 == true)
            {
                iMultiComplexScalarByComplexMatrix(
                    pDoubleR[i], pDoubleI[i],
                    pRealIn, pImgIn, 1, pPolyIn->getRank(),
                    pRealOut, pImgOut);
            }
        }
    }
    else if(_pDouble->getCols() == _pPoly->getRows())
    {
        Double *pCoef = _pPoly->getCoef();
        Double *pTemp = new Double(_pDouble->getRows(), pCoef->getCols(), bComplex1 || bComplex2);

        if(bComplex1 == false && bComplex2 == false)
        {
            iMultiRealMatrixByRealMatrix(
                _pDouble->getReal(), _pDouble->getRows(), _pDouble->getCols(),
                pCoef->getReal(), pCoef->getRows(), pCoef->getCols(),
                pTemp->getReal());
        }
        else if(bComplex1 == false && bComplex2 == true)
        {
            iMultiRealMatrixByComplexMatrix(
                _pDouble->getReal(), _pDouble->getRows(), _pDouble->getCols(),
                pCoef->getReal(), pCoef->getImg(), pCoef->getRows(), pCoef->getCols(),
                pTemp->getReal(), pTemp->getImg());

        }
        else if(bComplex1 == true && bComplex2 == false)
        {
            iMultiComplexMatrixByRealMatrix(
                _pDouble->getReal(), _pDouble->getImg(), _pDouble->getRows(), _pDouble->getCols(),
                pCoef->getReal(), pCoef->getRows(), pCoef->getCols(),
                pTemp->getReal(), pTemp->getImg());
        }
        else //if(bComplex1 == true && bComplex2 == true)
        {
            iMultiComplexMatrixByComplexMatrix(
                _pDouble->getReal(), _pDouble->getImg(), _pDouble->getRows(), _pDouble->getCols(),
                pCoef->getReal(), pCoef->getImg(), pCoef->getRows(), pCoef->getCols(),
                pTemp->getReal(), pTemp->getImg());
        }

        (*_pPolyOut)->setCoef(pTemp);
        delete pTemp;
    }
    else
    {
        return 1;
    }

    return 0;
}

int MultiplyPolyByDouble(Polynom* _pPoly, Double* _pDouble, Polynom **_pPolyOut)
{
    bool bComplex1  = _pPoly->isComplex();
    bool bComplex2  = _pDouble->isComplex();
    bool bScalar1   = _pPoly->getRows() == 1 && _pPoly->getCols() == 1;
    bool bScalar2   = _pDouble->getRows() == 1 && _pDouble->getCols() == 1;

    int iRowResult  = 0;
    int iColResult  = 0;
    int *piRank     = NULL;

    if(bScalar1)
    {
        iRowResult = _pDouble->getRows();
        iColResult = _pDouble->getCols();

        piRank = new int[iRowResult * iColResult];
        for(int i = 0 ; i < iRowResult * iColResult ; i++)
        {
            piRank[i] = _pPoly->get(0)->getRank();
        }
    }
    else if (bScalar2)
    {
        iRowResult = _pPoly->getRows();
        iColResult = _pPoly->getCols();

        piRank = new int[iRowResult * iColResult];
        for(int i = 0 ; i < iRowResult * iColResult ; i++)
        {
            piRank[i] = _pPoly->get(i)->getRank();
        }
    }
    else if(_pPoly->getCols() == _pDouble->getRows())
    {
        iRowResult = _pPoly->getRows();
        iColResult = _pDouble->getCols();
        piRank = new int[iRowResult * iColResult];
        for(int i = 0 ; i < iRowResult * iColResult ; i++)
        {
            piRank[i] = _pPoly->getMaxRank();
        }
    }
    else
    {
        return 1;
    }

    (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), iRowResult, iColResult, piRank);
    delete[] piRank;
    if(bComplex1 || bComplex2)
    {
        (*_pPolyOut)->setComplex(true);
    }
    //Output variables
    //Polynom *pResult	= NULL;
    bool bComplexOut = bComplex1 || bComplex2;

    if(bScalar1)
    {//p[1] * A
        double *pDoubleR    = _pDouble->getReal();
        double *pDoubleI    = _pDouble->getImg();

        SinglePoly *pPolyIn = _pPoly->get(0);
        double* pRealIn     = pPolyIn->getCoef()->getReal();
        double* pImgIn      = pPolyIn->getCoef()->getImg();

        for(int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            SinglePoly *pPolyOut    = (*_pPolyOut)->get(i);
            double* pRealOut        = pPolyOut->getCoef()->getReal();
            double* pImgOut         = pPolyOut->getCoef()->getImg();

            if(bComplex1 == false && bComplex2 == false)
            {
                iMultiRealScalarByRealMatrix(
                    pDoubleR[i],
                    pRealIn, 1, pPolyIn->getRank(),
                    pRealOut);
            }
            else if(bComplex1 == false && bComplex2 == true)
            {
                iMultiComplexScalarByRealMatrix(
                    pDoubleR[i], pDoubleI[i],
                    pRealIn, 1, pPolyIn->getRank(),
                    pRealOut, pImgOut);
            }
            else if(bComplex1 == true && bComplex2 == false)
            {
                iMultiRealScalarByComplexMatrix(
                    pDoubleR[i],
                    pRealIn, pImgIn, 1, pPolyIn->getRank(),
                    pRealOut, pImgOut);
            }
            else if(bComplex1 == true && bComplex2 == true)
            {
                iMultiComplexScalarByComplexMatrix(
                    pDoubleR[i], pDoubleI[i],
                    pRealIn, pImgIn, 1, pPolyIn->getRank(),
                    pRealOut, pImgOut);
            }
        }
    }
    else if(bScalar2)
    {//p * cst
        for(int i = 0 ; i < _pPoly->getSize() ; i++)
        {
            SinglePoly *pPolyIn = _pPoly->get(i);
            double* pRealIn     = pPolyIn->getCoef()->getReal();
            double* pImgIn      = pPolyIn->getCoef()->getImg();

            SinglePoly *pPolyOut    = (*_pPolyOut)->get(i);
            double* pRealOut        = pPolyOut->getCoef()->getReal();
            double* pImgOut         = pPolyOut->getCoef()->getImg();

            if(bComplex1 == false && bComplex2 == false)
            {
                iMultiRealScalarByRealMatrix(
                    _pDouble->getReal(0,0),
                    pRealIn, 1, pPolyIn->getRank(),
                    pRealOut);
            }
            else if(bComplex1 == false && bComplex2 == true)
            {
                iMultiComplexScalarByRealMatrix(
                    _pDouble->getReal(0,0), _pDouble->getImg(0,0),
                    pRealIn, 1, pPolyIn->getRank(),
                    pRealOut, pImgOut);
            }
            else if(bComplex1 == true && bComplex2 == false)
            {
                iMultiRealScalarByComplexMatrix(
                    _pDouble->getReal(0,0),
                    pRealIn, pImgIn, 1, pPolyIn->getRank(),
                    pRealOut, pImgOut);
            }
            else if(bComplex1 == true && bComplex2 == true)
            {
                iMultiComplexScalarByComplexMatrix(
                    _pDouble->getReal(0,0), _pDouble->getImg(0,0),
                    pRealIn, pImgIn, 1, pPolyIn->getRank(),
                    pRealOut, pImgOut);
            }
        }
    }
    else if(_pPoly->getCols() == _pDouble->getRows())
    {
        //Distribution a la mano par appels a des sous-fonctions ( iMulti...ScalarBy...Scalar ) plus iAdd...To... )

        //for each line of _pPoly
        for(int iRow1 = 0 ; iRow1 < _pPoly->getRows() ; iRow1++)
        {//for each col of _pDouble
            for(int iCol2 = 0 ; iCol2 < _pDouble->getCols() ; iCol2++)
            {//for each rows of _pDouble / cols of _pPoly
                for(int iRow2 = 0 ; iRow2 < _pDouble->getRows() ; iRow2++)
                {
                    Double *pPolyCoef = _pPoly->get(iRow1, iRow2)->getCoef();

                    Double *pDouble = NULL;
                    if(bComplex2 == false)
                    {
                        pDouble	= new Double(_pDouble->getReal(iRow2, iCol2));
                    }
                    else
                    {
                        pDouble	= new Double(_pDouble->getReal(iRow2, iCol2), _pDouble->getImg(iRow2, iCol2));
                    }

                    Double *TimeDouble = NULL; //(pPolyCoef->getRows(), pPolyCoef->getCols(), bComplexOut);
                    MultiplyDoubleByDouble(pPolyCoef, pDouble, &TimeDouble);
                    Double *pAddDouble = NULL;

                    //Adjust size to allow vector multiplication
                    Double* pCoef = (*_pPolyOut)->get(iRow1, iCol2)->getCoef();

                    if(TimeDouble->getRows() > pCoef->getRows())
                    {
                        pCoef->resize(TimeDouble->getRows(), pCoef->getCols());
                    }
                    else if(TimeDouble->getRows() < pCoef->getRows())
                    {
                        TimeDouble->resize(pCoef->getRows(), TimeDouble->getCols());
                    }

                    if(TimeDouble->getCols() > pCoef->getCols())
                    {
                        pCoef->resize(pCoef->getRows(), TimeDouble->getCols());
                    }
                    else if(TimeDouble->getCols() < pCoef->getCols())
                    {
                        TimeDouble->resize(TimeDouble->getRows(), pCoef->getCols());
                    }

                    AddDoubleToDouble(TimeDouble, pCoef, &pAddDouble);
                    (*_pPolyOut)->setCoef(iRow1, iCol2, pAddDouble);

                    delete pAddDouble;
                    delete pDouble;
                }//for(int iRow2 = 0 ; iRow2 < _pDouble->getRows() ; iRow2++)
            }//for(int iCol2 = 0 ; iCol2 < _pDouble->getCols() ; iCol2++)
        }//for(int iRow1 = 0 ; iRow1 < _pPoly->getRows() ; iRow1++)
    }
    else //Wrong dimensions.
    {
        return 1;
    }
    return 0;
}

int MultiplyPolyByPoly(Polynom* _pPoly1, Polynom* _pPoly2, Polynom** _pPolyOut)
{
    bool bComplex1 	= _pPoly1->isComplex();
    bool bComplex2 	= _pPoly2->isComplex();
    bool bScalar1		= _pPoly1->getRows() == 1 && _pPoly1->getCols() == 1;
    bool bScalar2		= _pPoly2->getRows() == 1 && _pPoly2->getCols() == 1;

    int iRowResult 	= 0;
    int iColResult	= 0;
    int *piRank			= NULL;

    if(_pPoly1->getSize() == 1 && _pPoly2->getSize() == 1)
    {
        iRowResult = 1;
        iColResult = 1;

        piRank = new int[1];
        piRank[0] = _pPoly1->get(0)->getRank() + _pPoly2->get(0)->getRank() - 1;
    }
    else if(_pPoly1->getSize() == 1)
    {
        iRowResult = _pPoly2->getRows();
        iColResult = _pPoly2->getCols();

        piRank = new int[iRowResult * iColResult];
        for(int i = 0 ; i < iRowResult * iColResult ; i++)
        {
            piRank[i] = _pPoly1->get(0)->getRank() + _pPoly2->get(i)->getRank() - 1;
        }
    }
    else if (_pPoly2->getSize() == 1)
    {
        iRowResult = _pPoly1->getRows();
        iColResult = _pPoly1->getCols();

        piRank = new int[iRowResult * iColResult];
        for(int i = 0 ; i < iRowResult * iColResult ; i++)
        {
            piRank[i] = _pPoly2->get(0)->getRank() * _pPoly1->get(i)->getRank();
        }
    }
    else if(_pPoly1->getCols() == _pPoly2->getRows())
    {
        iRowResult = _pPoly1->getRows();
        iColResult = _pPoly2->getCols();
        piRank = new int[iRowResult * iColResult];
        for(int i = 0 ; i < iRowResult * iColResult ; i++)
        {
            piRank[i] = _pPoly1->getMaxRank() * _pPoly2->getMaxRank();
        }
    }
    else
    {
        return 1;
    }

    (*_pPolyOut) = new Polynom(_pPoly1->getVariableName(), iRowResult, iColResult, piRank);

    delete[] piRank;

    if(bComplex1 || bComplex2)
    {
        (*_pPolyOut)->setComplex(true);
    }

    if(bScalar1 && bScalar2)
    {//poly1(0) * poly2(0)
        if(bComplex1 == false && bComplex2 == false)
        {
            SinglePoly *pPoly1		= _pPoly1->get(0);
            SinglePoly *pPoly2		= _pPoly2->get(0);
            SinglePoly *pPolyOut	= (*_pPolyOut)->get(0);

            pPolyOut->getCoef()->setZeros();

            iMultiRealPolyByRealPoly(
                pPoly1->getCoef()->getReal(), pPoly1->getRank(),
                pPoly2->getCoef()->getReal(), pPoly2->getRank(),
                pPolyOut->getCoef()->getReal(), pPolyOut->getRank());
        }
        else if(bComplex1 == false && bComplex2 == true)
        {
            SinglePoly *pPoly1		= _pPoly1->get(0);
            SinglePoly *pPoly2		= _pPoly2->get(0);
            SinglePoly *pPolyOut	= (*_pPolyOut)->get(0);

            pPolyOut->getCoef()->setZeros();

            iMultiRealPolyByComplexPoly(
                pPoly1->getCoef()->getReal(), pPoly1->getRank(),
                pPoly2->getCoef()->getReal(), pPoly2->getCoef()->getImg(), pPoly2->getRank(),
                pPolyOut->getCoef()->getReal(), pPolyOut->getCoef()->getImg(), pPolyOut->getRank());
        }
        else if(bComplex1 == true && bComplex2 == false)
        {
            SinglePoly *pPoly1		= _pPoly1->get(0);
            SinglePoly *pPoly2		= _pPoly2->get(0);
            SinglePoly *pPolyOut	= (*_pPolyOut)->get(0);

            pPolyOut->getCoef()->setZeros();

            iMultiComplexPolyByRealPoly(
                pPoly1->getCoef()->getReal(), pPoly1->getCoef()->getImg(), pPoly1->getRank(),
                pPoly2->getCoef()->getReal(), pPoly2->getRank(),
                pPolyOut->getCoef()->getReal(), pPolyOut->getCoef()->getImg(), pPolyOut->getRank());
        }
        else if(bComplex1 == true && bComplex2 == true)
        {
            SinglePoly *pPoly1			= _pPoly1->get(0);
            SinglePoly *pPoly2			= _pPoly2->get(0);
            SinglePoly *pPolyOut		= (*_pPolyOut)->get(0);
            Double *pCoef1		= pPoly1->getCoef();
            Double *pCoef2		= pPoly2->getCoef();
            Double *pCoefOut	= pPolyOut->getCoef();

            pCoefOut->setZeros();
            iMultiComplexPolyByComplexPoly(
                pCoef1->getReal(), pCoef1->getImg(), pPoly1->getRank(),
                pCoef2->getReal(), pCoef2->getImg(), pPoly2->getRank(),
                pCoefOut->getReal(), pCoefOut->getImg(), pPolyOut->getRank());
        }
    }
    else if(bScalar1)
    {//poly1(0) * poly2(n)
        SinglePoly *pPoly1		= _pPoly1->get(0);
        if(bComplex1 == false && bComplex2 == false)
        {
            for(int iPoly = 0 ; iPoly < _pPoly2->getSize() ; iPoly++)
            {
                SinglePoly *pPoly2		= _pPoly2->get(iPoly);
                SinglePoly *pPolyOut	= (*_pPolyOut)->get(iPoly);

                pPolyOut->getCoef()->setZeros();

                iMultiRealPolyByRealPoly(
                    pPoly1->getCoef()->getReal(), pPoly1->getRank(),
                    pPoly2->getCoef()->getReal(), pPoly2->getRank(),
                    pPolyOut->getCoef()->getReal(), pPolyOut->getRank());
            }
        }
        else if(bComplex1 == false && bComplex2 == true)
        {
            for(int iPoly = 0 ; iPoly < _pPoly2->getSize() ; iPoly++)
            {
                SinglePoly *pPoly2		= _pPoly2->get(iPoly);
                SinglePoly *pPolyOut	= (*_pPolyOut)->get(iPoly);

                pPolyOut->getCoef()->setZeros();

                iMultiRealPolyByComplexPoly(
                    pPoly1->getCoef()->getReal(), pPoly1->getRank(),
                    pPoly2->getCoef()->getReal(), pPoly2->getCoef()->getImg(), pPoly2->getRank(),
                    pPolyOut->getCoef()->getReal(), pPolyOut->getCoef()->getImg(), pPolyOut->getRank());
            }
        }
        else if(bComplex1 == true && bComplex2 == false)
        {
            for(int iPoly = 0 ; iPoly < _pPoly2->getSize() ; iPoly++)
            {
                SinglePoly *pPoly2		= _pPoly2->get(iPoly);
                SinglePoly *pPolyOut	= (*_pPolyOut)->get(iPoly);

                pPolyOut->getCoef()->setZeros();

                iMultiComplexPolyByRealPoly(
                    pPoly1->getCoef()->getReal(), pPoly1->getCoef()->getImg(), pPoly1->getRank(),
                    pPoly2->getCoef()->getReal(), pPoly2->getRank(),
                    pPolyOut->getCoef()->getReal(), pPolyOut->getCoef()->getImg(), pPolyOut->getRank());
            }
        }
        else if(bComplex1 == true && bComplex2 == true)
        {
            Double *pCoef1		= pPoly1->getCoef();
            for(int iPoly = 0 ; iPoly < _pPoly2->getSize() ; iPoly++)
            {
                SinglePoly *pPoly2			= _pPoly2->get(iPoly);
                SinglePoly *pPolyOut		= (*_pPolyOut)->get(iPoly);
                Double *pCoef2		= pPoly2->getCoef();
                Double *pCoefOut	= pPolyOut->getCoef();


                pCoefOut->setZeros();

                iMultiComplexPolyByComplexPoly(
                    pCoef1->getReal(), pCoef1->getImg(), pPoly1->getRank(),
                    pCoef2->getReal(), pCoef2->getImg(), pPoly2->getRank(),
                    pCoefOut->getReal(), pCoefOut->getImg(), pPolyOut->getRank());
            }
        }
    }
    else if(bScalar2)
    {//poly1(n) * poly2(0)
        SinglePoly *pPoly2		= _pPoly2->get(0);
        if(bComplex1 == false && bComplex2 == false)
        {
            for(int iPoly = 0 ; iPoly < _pPoly1->getSize() ; iPoly++)
            {
                SinglePoly *pPoly1		= _pPoly1->get(iPoly);
                SinglePoly *pPolyOut	= (*_pPolyOut)->get(iPoly);

                pPolyOut->getCoef()->setZeros();

                iMultiRealPolyByRealPoly(
                    pPoly1->getCoef()->getReal(), pPoly1->getRank(),
                    pPoly2->getCoef()->getReal(), pPoly2->getRank(),
                    pPolyOut->getCoef()->getReal(), pPolyOut->getRank());
            }
        }
        else if(bComplex1 == false && bComplex2 == true)
        {
            for(int iPoly = 0 ; iPoly < _pPoly1->getSize() ; iPoly++)
            {
                SinglePoly *pPoly1		= _pPoly1->get(iPoly);
                SinglePoly *pPolyOut	= (*_pPolyOut)->get(iPoly);

                pPolyOut->getCoef()->setZeros();

                iMultiRealPolyByComplexPoly(
                    pPoly1->getCoef()->getReal(), pPoly1->getRank(),
                    pPoly2->getCoef()->getReal(), pPoly2->getCoef()->getImg(), pPoly2->getRank(),
                    pPolyOut->getCoef()->getReal(), pPolyOut->getCoef()->getImg(), pPolyOut->getRank());
            }
        }
        else if(bComplex1 == true && bComplex2 == false)
        {
            for(int iPoly = 0 ; iPoly < _pPoly1->getSize() ; iPoly++)
            {
                SinglePoly *pPoly1		= _pPoly1->get(iPoly);
                SinglePoly *pPolyOut	= (*_pPolyOut)->get(iPoly);

                pPolyOut->getCoef()->setZeros();

                iMultiComplexPolyByRealPoly(
                    pPoly1->getCoef()->getReal(), pPoly1->getCoef()->getImg(), pPoly1->getRank(),
                    pPoly2->getCoef()->getReal(), pPoly2->getRank(),
                    pPolyOut->getCoef()->getReal(), pPolyOut->getCoef()->getImg(), pPolyOut->getRank());
            }
        }
        else if(bComplex1 == true && bComplex2 == true)
        {
            Double *pCoef2		= pPoly2->getCoef();
            for(int iPoly = 0 ; iPoly < _pPoly1->getSize() ; iPoly++)
            {
                SinglePoly *pPoly1			= _pPoly1->get(iPoly);
                SinglePoly *pPolyOut		= (*_pPolyOut)->get(iPoly);
                Double *pCoef1		= pPoly1->getCoef();
                Double *pCoefOut	= pPolyOut->getCoef();


                pCoefOut->setZeros();

                iMultiComplexPolyByComplexPoly(
                    pCoef1->getReal(), pCoef1->getImg(), pPoly1->getRank(),
                    pCoef2->getReal(), pCoef2->getImg(), pPoly2->getRank(),
                    pCoefOut->getReal(), pCoefOut->getImg(), pPolyOut->getRank());
            }
        }
    }
    else
    {// matrix by matrix
        if(bComplex1 == false && bComplex2 == false)
        {
            double *pReal	= NULL;
            SinglePoly *pTemp		= new SinglePoly(&pReal, (*_pPolyOut)->getMaxRank());

            for(int iRow = 0 ; iRow < _pPoly1->getRows() ; iRow++)
            {
                for(int iCol = 0 ; iCol < _pPoly2->getCols() ; iCol++)
                {
                    SinglePoly *pResult = (*_pPolyOut)->get(iRow, iCol);
                    pResult->getCoef()->setZeros();

                    for(int iCommon = 0 ; iCommon < _pPoly1->getCols() ; iCommon++)
                    {
                        SinglePoly *pL			= _pPoly1->get(iRow, iCommon);
                        SinglePoly *pR			= _pPoly2->get(iCommon, iCol);

                        pTemp->getCoef()->setZeros();

                        iMultiRealPolyByRealPoly(
                            pL->getCoef()->getReal(), pL->getRank(),
                            pR->getCoef()->getReal(), pR->getRank(),
                            pTemp->getCoef()->getReal(), pL->getRank() + pR->getRank() - 1);

                        iAddRealPolyToRealPoly(
                            pResult->getCoef()->getReal(), pResult->getRank(),
                            pTemp->getCoef()->getReal(), pResult->getRank(),
                            pResult->getCoef()->getReal(), pResult->getRank());
                    }
                }
            }
        }
        else if(bComplex1 == false && bComplex2 == true)
        {
            double *pReal	= NULL;
            double *pImg	= NULL;
            SinglePoly *pTemp		= new SinglePoly(&pReal, &pImg, (*_pPolyOut)->getMaxRank());

            for(int iRow = 0 ; iRow < _pPoly1->getRows() ; iRow++)
            {
                for(int iCol = 0 ; iCol < _pPoly2->getCols() ; iCol++)
                {
                    SinglePoly *pResult = (*_pPolyOut)->get(iRow, iCol);
                    pResult->getCoef()->setZeros();

                    for(int iCommon = 0 ; iCommon < _pPoly1->getCols() ; iCommon++)
                    {
                        SinglePoly *pL			= _pPoly1->get(iRow, iCommon);
                        SinglePoly *pR			= _pPoly2->get(iCommon, iCol);

                        pTemp->getCoef()->setZeros();

                        iMultiRealPolyByComplexPoly(
                            pL->getCoef()->getReal(), pL->getRank(),
                            pR->getCoef()->getReal(), pR->getCoef()->getImg(), pR->getRank(),
                            pTemp->getCoef()->getReal(), pTemp->getCoef()->getImg(), pL->getRank() + pR->getRank() - 1);

                        iAddComplexPolyToComplexPoly(
                            pResult->getCoef()->getReal(), pResult->getCoef()->getImg(), pResult->getRank(),
                            pTemp->getCoef()->getReal(), pTemp->getCoef()->getImg(), pResult->getRank(),
                            pResult->getCoef()->getReal(), pResult->getCoef()->getImg(), pResult->getRank());
                    }
                }
            }
        }
        else if(bComplex1 == true && bComplex2 == false)
        {
            double *pReal	= NULL;
            double *pImg	= NULL;
            SinglePoly *pTemp		= new SinglePoly(&pReal, &pImg, (*_pPolyOut)->getMaxRank());

            for(int iRow = 0 ; iRow < _pPoly1->getRows() ; iRow++)
            {
                for(int iCol = 0 ; iCol < _pPoly2->getCols() ; iCol++)
                {
                    SinglePoly *pResult = (*_pPolyOut)->get(iRow, iCol);
                    pResult->getCoef()->setZeros();

                    for(int iCommon = 0 ; iCommon < _pPoly1->getCols() ; iCommon++)
                    {
                        SinglePoly *pL			= _pPoly1->get(iRow, iCommon);
                        SinglePoly *pR			= _pPoly2->get(iCommon, iCol);

                        pTemp->getCoef()->setZeros();

                        iMultiRealPolyByComplexPoly(
                            pR->getCoef()->getReal(), pR->getRank(),
                            pL->getCoef()->getReal(), pL->getCoef()->getImg(), pL->getRank(),
                            pTemp->getCoef()->getReal(), pTemp->getCoef()->getImg(), pL->getRank() + pR->getRank() - 1);

                        iAddComplexPolyToComplexPoly(
                            pResult->getCoef()->getReal(), pResult->getCoef()->getImg(), pResult->getRank(),
                            pTemp->getCoef()->getReal(), pTemp->getCoef()->getImg(), pResult->getRank(),
                            pResult->getCoef()->getReal(), pResult->getCoef()->getImg(), pResult->getRank());
                    }
                }
            }
        }
        else if(bComplex1 == true && bComplex2 == true)
        {
            double *pReal	= NULL;
            double *pImg	= NULL;
            SinglePoly *pTemp		= new SinglePoly(&pReal, &pImg, (*_pPolyOut)->getMaxRank());

            for(int iRow = 0 ; iRow < _pPoly1->getRows() ; iRow++)
            {
                for(int iCol = 0 ; iCol < _pPoly2->getCols() ; iCol++)
                {
                    SinglePoly *pResult = (*_pPolyOut)->get(iRow, iCol);
                    pResult->getCoef()->setZeros();

                    for(int iCommon = 0 ; iCommon < _pPoly1->getCols() ; iCommon++)
                    {
                        SinglePoly *pL			= _pPoly1->get(iRow, iCommon);
                        SinglePoly *pR			= _pPoly2->get(iCommon, iCol);

                        pTemp->getCoef()->setZeros();

                        iMultiComplexPolyByComplexPoly(
                            pL->getCoef()->getReal(), pL->getCoef()->getImg(), pL->getRank(),
                            pR->getCoef()->getReal(), pR->getCoef()->getImg(), pR->getRank(),
                            pTemp->getCoef()->getReal(), pTemp->getCoef()->getImg(), pL->getRank() + pR->getRank() - 1);

                        iAddComplexPolyToComplexPoly(
                            pResult->getCoef()->getReal(), pResult->getCoef()->getImg(), pResult->getRank(),
                            pTemp->getCoef()->getReal(), pTemp->getCoef()->getImg(), pResult->getRank(),
                            pResult->getCoef()->getReal(), pResult->getCoef()->getImg(), pResult->getRank());
                    }
                }
            }
        }
    }
    return 0; //No Error;
}

int MultiplySparseBySparse(Sparse* _pSparse1, Sparse* _pSparse2, Sparse** _pSparseOut)
{
    if(_pSparse1->getRows() == 1 && _pSparse1->getCols() == 1)
    {//scalar * sp
        Double* pDbl = NULL;
        if(_pSparse1->isComplex())
        {
            std::complex<double> dbl = _pSparse1->getImg(0,0);
            pDbl = new Double(dbl.real(), dbl.imag());
        }
        else
        {
            pDbl = new Double(_pSparse1->get(0,0));
        }
        
        MultiplyDoubleBySparse(pDbl, _pSparse2, (GenericType**)_pSparseOut);
        delete pDbl;
        return 0;
    }

    if(_pSparse2->getRows() == 1 && _pSparse2->getCols() == 1)
    {//sp * scalar
        Double* pDbl = NULL;
        if(_pSparse2->isComplex())
        {
            std::complex<double> dbl = _pSparse2->getImg(0,0);
            pDbl = new Double(dbl.real(), dbl.imag());
        }
        else
        {
            pDbl = new Double(_pSparse2->get(0,0));
        }
        
        MultiplySparseByDouble(_pSparse1, pDbl, (GenericType**)_pSparseOut);
        delete pDbl;
        return 0;
    }

    if(_pSparse1->getCols() != _pSparse2->getRows())
    {
        return 1;
    }

    *_pSparseOut = _pSparse1->multiply(*_pSparse2);
    return 0;
}

int MultiplyDoubleBySparse(Double* _pDouble, Sparse *_pSparse, GenericType** _pOut)
{//D * SP
    if(_pDouble->isEmpty())
    {
        *_pOut = Double::Empty();
        return 0;
    }

    if(_pDouble->isScalar())
    {//d * SP -> SP
        Sparse* pOut = NULL;
        if(_pDouble->isComplex())
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

    if(_pSparse->isScalar())
    {//D * sp -> D .* d
        Double* pD = NULL;
        
        if(_pSparse->isComplex())
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

    if(_pDouble->getCols() != _pSparse->getRows())
    {
        return 1;
    }

    //try to be smart and only compute for non zero values

    //get some information
    int iNonZeros = static_cast<int>(_pSparse->nonZeros());
    double* pRows = new double[iNonZeros * 2];
    _pSparse->outputRowCol(pRows);
    double* pCols = pRows + iNonZeros;
    double* pValR = new double[iNonZeros];
    double* pValI = new double[iNonZeros];
    _pSparse->outputValues(pValR, pValI);

    Double* pOut = new Double(_pDouble->getRows(), _pSparse->getCols(), _pDouble->isComplex() | _pSparse->isComplex());
    pOut->setZeros();

    if(_pDouble->isComplex() == false && _pSparse->isComplex() == false)
    {
        for(int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dbl = pValR[i];

            for(int j = 0 ; j < _pDouble->getRows() ; j++)
            {
                double dblVal = _pDouble->get(j, iRow) * dbl;
                pOut->set(j, iCol, pOut->get(j, iCol) + dblVal);
            }
        }
    }
    else if(_pDouble->isComplex() == false && _pSparse->isComplex() == true)
    {//a * (b ci) -> ab ac
        for(int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dblR = pValR[i];
            double dblI = pValI[i];

            for(int j = 0 ; j < _pDouble->getRows() ; j++)
            {
                double dblValR = _pDouble->get(j, iRow) * dblR;
                double dblValI = _pDouble->get(j, iRow) * dblI;
                pOut->set(j, iCol, pOut->get(j, iCol) + dblValR);
                pOut->setImg(j, iCol, pOut->getImg(j, iCol) + dblValI);
            }
        }
    }
    else if(_pDouble->isComplex() == true && _pSparse->isComplex() == false)
    {//(a bi) * c -> ac + bc
        for(int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dblR = pValR[i];

            for(int j = 0 ; j < _pDouble->getRows() ; j++)
            {
                double dblValR = _pDouble->get(j, iRow) * dblR;
                double dblValI = _pDouble->getImg(j, iRow) * dblR;
                pOut->set(j, iCol, pOut->get(j, iCol) + dblValR);
                pOut->setImg(j, iCol, pOut->getImg(j, iCol) + dblValI);
            }
        }
    }
    else if(_pDouble->isComplex() == true && _pSparse->isComplex() == true)
    {
        for(int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dblR = pValR[i];
            double dblI = pValI[i];

            for(int j = 0 ; j < _pDouble->getRows() ; j++)
            {
                double dblValR = _pDouble->get(j, iRow) * dblR - _pDouble->getImg(j, iRow) * dblI;
                double dblValI = _pDouble->get(j, iRow) * dblI + _pDouble->getImg(j, iRow) * dblR;
                pOut->set(j, iCol, pOut->get(j, iCol) + dblValR);
                pOut->setImg(j, iCol, pOut->getImg(j, iCol) + dblValI);
            }
        }
    }
    
    *_pOut = pOut;
    delete[] pRows;
    delete[] pValR;
    delete[] pValI;

    return 0;
}

int MultiplySparseByDouble(Sparse *_pSparse, Double*_pDouble, GenericType** _pOut)
{
    if(_pDouble->isEmpty())
    {//SP * [] -> []
        *_pOut = Double::Empty();
        return 0;
    }

    if(_pDouble->isScalar())
    {//SP * d -> SP
        Sparse* pOut = NULL;
        if(_pDouble->isComplex())
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

    if(_pSparse->isScalar())
    {//D * sp -> D .* d
        Double* pD = NULL;
        
        if(_pSparse->isComplex())
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

    if(_pSparse->getCols() != _pDouble->getRows())
    {
        return 1;
    }

    //try to be smart and only compute for non zero values

    //get some information
    int iNonZeros = static_cast<int>(_pSparse->nonZeros());
    double* pRows = new double[iNonZeros * 2];
    _pSparse->outputRowCol(pRows);
    double* pCols = pRows + iNonZeros;
    double* pValR = new double[iNonZeros];
    double* pValI = new double[iNonZeros];
    _pSparse->outputValues(pValR, pValI);

    Double* pOut = new Double(_pSparse->getRows(), _pDouble->getCols(), _pDouble->isComplex() | _pSparse->isComplex());
    pOut->setZeros();

    if(_pDouble->isComplex() == false && _pSparse->isComplex() == false)
    {
        for(int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow    = static_cast<int>(pRows[i]) - 1;
            int iCol    = static_cast<int>(pCols[i]) - 1;
            double dbl  = pValR[i];

            for(int j = 0 ; j < _pDouble->getCols() ; j++)
            {
                double dblVal = _pDouble->get(iCol, j) * dbl;
                pOut->set(iRow, j, pOut->get(iRow, j) + dblVal);
            }
        }
    }
    else if(_pDouble->isComplex() == false && _pSparse->isComplex() == true)
    {//a * (b ci) -> ab ac
        for(int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dblR = pValR[i];
            double dblI = pValI[i];

            for(int j = 0 ; j < _pDouble->getCols() ; j++)
            {
                double dblValR = _pDouble->get(iCol, j) * dblR;
                double dblValI = _pDouble->get(iCol, j) * dblI;
                pOut->set(iRow, j, pOut->get(iRow, j) + dblValR);
                pOut->setImg(iRow, j, pOut->getImg(iRow, j) + dblValI);
            }
        }
    }
    else if(_pDouble->isComplex() == true && _pSparse->isComplex() == false)
    {//(a bi) * c -> ac + bc
        for(int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dblR = pValR[i];

            for(int j = 0 ; j < _pDouble->getCols() ; j++)
            {
                double dblValR = _pDouble->get(iCol, j) * dblR;
                double dblValI = _pDouble->getImg(iCol, j) * dblR;
                pOut->set(iRow, j, pOut->get(iRow, j) + dblValR);
                pOut->setImg(iRow, j, pOut->getImg(iRow, j) + dblValI);
            }
        }
    }
    else if(_pDouble->isComplex() == true && _pSparse->isComplex() == true)
    {
        for(int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dblR = pValR[i];
            double dblI = pValI[i];

            for(int j = 0 ; j < _pDouble->getCols() ; j++)
            {
                double dblValR = _pDouble->get(iCol, j) * dblR - _pDouble->getImg(iCol, j) * dblI;
                double dblValI = _pDouble->get(iCol, j) * dblI + _pDouble->getImg(iCol, j) * dblR;
                pOut->set(iRow, j, pOut->get(iRow, j) + dblValR);
                pOut->setImg(iRow, j, pOut->getImg(iRow, j) + dblValI);
            }
        }
    }
    
    *_pOut = pOut;
    delete[] pRows;
    delete[] pValR;
    delete[] pValI;

    return 0;
}

int DotMultiplySparseBySparse(Sparse* _pSparse1, Sparse* _pSparse2, Sparse** _pOut)
{
    if(_pSparse1->isScalar() || _pSparse2->isScalar())
    {//SP .* sp or sp .* SP
        return MultiplySparseBySparse(_pSparse1, _pSparse2, _pOut);
    }

    if(_pSparse1->getRows() != _pSparse2->getRows() || _pSparse1->getCols() != _pSparse2->getCols())
    {
        return 1;
    }

    *_pOut = _pSparse1->dotMultiply(*_pSparse2);

    return 0;
}

int DotMultiplyDoubleBySparse(Double* _pDouble, Sparse* _pSparse, GenericType**  _pOut)
{
    if(_pDouble->isEmpty())
    {
        *_pOut = Double::Empty();
        return 0;
    }

    if(_pDouble->isScalar())
    {
        return MultiplyDoubleBySparse(_pDouble, _pSparse, _pOut);
    }

    if(_pSparse->isScalar())
    {
        return MultiplyDoubleBySparse(_pDouble, _pSparse, _pOut);
    }

    if(_pSparse->getRows() != _pDouble->getRows() || _pSparse->getCols() != _pDouble->getCols())
    {
        return 1;
    }

    Sparse* pOut = new Sparse(_pDouble->getRows(), _pDouble->getCols(), _pSparse->isComplex() || _pDouble->isComplex());
    //get some information
    int iNonZeros = static_cast<int>(_pSparse->nonZeros());
    double* pRows = new double[iNonZeros * 2];
    _pSparse->outputRowCol(pRows);
    double* pCols = pRows + iNonZeros;

    if(_pDouble->isComplex() == false && _pSparse->isComplex() == false)
    {
        for(int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            pOut->set(iRow, iCol, _pSparse->get(iRow, iCol) * _pDouble->get(iRow, iCol));
        }
    }
    else if(_pDouble->isComplex() == false && _pSparse->isComplex() == true)
    {
        for(int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            std::complex<double> dbl = _pSparse->getImg(iRow, iCol);
            std::complex<double> newVal(dbl.real() * _pDouble->get(iRow, iCol), dbl.imag() * _pDouble->get(iRow, iCol));
            pOut->set(iRow, iCol, newVal);
        }
    }
    else if(_pDouble->isComplex() == true && _pSparse->isComplex() == false)
    {
        for(int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            std::complex<double> dbl = _pSparse->getImg(iRow, iCol);
            std::complex<double> newVal(dbl.real() * _pDouble->get(iRow, iCol), dbl.real() * _pDouble->getImg(iRow, iCol));
            pOut->set(iRow, iCol, newVal);
        }
    }
    else if(_pDouble->isComplex() == true && _pSparse->isComplex() == true)
    {
        for(int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            std::complex<double> dbl = _pSparse->getImg(iRow, iCol);
            double dblR = _pDouble->get(iRow, iCol) * dbl.real() - _pDouble->getImg(iRow, iCol) * dbl.imag();
            double dblI = _pDouble->getImg(iRow, iCol) * dbl.real() + _pDouble->get(iRow, iCol) * dbl.imag();

            std::complex<double> newVal(dblR, dblI);
            pOut->set(iRow, iCol, newVal);
        }
    }
    
    *_pOut = pOut;
    delete[] pRows;

    return 0;
}

int DotMultiplySparseByDouble(Sparse* _pSparse, Double* _pDouble, GenericType** _pOut)
{
    return DotMultiplyDoubleBySparse(_pDouble, _pSparse, _pOut);
}
