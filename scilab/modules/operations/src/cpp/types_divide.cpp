/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_divide.hxx"
#include "types_finite.hxx"

#include "scilabexception.hxx"

extern "C"
{
#include "matrix_right_division.h"
#include "sciprint.h"
#include "localization.h"
#include "charEncoding.h"
#include "warningmode.h"
#include "elem_common.h"
}

using namespace types;

InternalType *GenericRDivide(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult       = NULL;
    GenericType::RealType TypeL = _pLeftOperand->getType();
    GenericType::RealType TypeR = _pRightOperand->getType();

    int iResult = 0;

    if (_pLeftOperand->isDouble() && _pLeftOperand->getAs<Double>()->isEmpty())
    {
        return Double::Empty();
    }

    if (_pRightOperand->isDouble() && _pRightOperand->getAs<Double>()->isEmpty())
    {
        return Double::Empty();
    }

    /*
    ** DOUBLE / DOUBLE
    */
    if (TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
    {
        Double *pL  = _pLeftOperand->getAs<Double>();
        Double *pR  = _pRightOperand->getAs<Double>();

        iResult = RDivideDoubleByDouble(pL, pR, (Double**)&pResult);
    }

    /*
    ** POLY / DOUBLE
    */
    else if (TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
    {
        Polynom *pL = _pLeftOperand->getAs<types::Polynom>();
        Double *pR  = _pRightOperand->getAs<Double>();

        iResult = RDividePolyByDouble(pL, pR, (Polynom**)&pResult);
    }

    /*
    ** DOUBLE / POLY
    */
    else if (TypeL == GenericType::RealDouble && TypeR == GenericType::RealPoly)
    {
        Double *pL  = _pLeftOperand->getAs<Double>();
        Polynom *pR = _pRightOperand->getAs<types::Polynom>();

        iResult = RDivideDoubleByPoly(pL, pR, (Polynom**)&pResult);
    }

    //manage errors
    if (iResult)
    {
        switch (iResult)
        {
            case 1 :
                throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
            case 2 :
                throw ast::ScilabError(_W("With NaN or Inf a division by scalar expected.\n"));
            case 3 :
                throw ast::ScilabError(_W("Division by zero...\n"));
            case 4 :
                if (getWarningMode())
                {
                    sciprint(_("Warning : Division by zero...\n"));
                }
                break;
                //            default : throw ast::ScilabError(_W("Operator / : Error %d not yet managed.\n"), iResult);
            default :
                sciprint(_("Operator / : Error %d not yet managed.\n"), iResult);
        }
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return pResult;
}

InternalType *GenericDotRDivide(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult       = NULL;
    GenericType::RealType TypeL = _pLeftOperand->getType();
    GenericType::RealType TypeR = _pRightOperand->getType();

    int iResult = 0;

    if (_pLeftOperand->isDouble() && _pLeftOperand->getAs<Double>()->isEmpty())
    {
        return Double::Empty();
    }

    if (_pRightOperand->isDouble() && _pRightOperand->getAs<Double>()->isEmpty())
    {
        return Double::Empty();
    }

    /*
    ** DOUBLE ./ DOUBLE
    */
    if (TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
    {
        Double *pL  = _pLeftOperand->getAs<Double>();
        Double *pR  = _pRightOperand->getAs<Double>();

        iResult = DotRDivideDoubleByDouble(pL, pR, (Double**)&pResult);
    }

    if (TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
    {
        Polynom *pL = _pLeftOperand->getAs<Polynom>();
        Double *pR  = _pRightOperand->getAs<Double>();

        // ou exclusif
        if(pL->isScalar() ^ pR->isScalar())
        {
            // call overload
            return NULL;
        }

        iResult = DotRDividePolyByDouble(pL, pR, (Polynom**)&pResult);
    }

    //manage errors
    if (iResult)
    {
        switch (iResult)
        {
            case 1 :
                throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
            case 2 :
                throw ast::ScilabError(_W("With NaN or Inf a division by scalar expected.\n"));
            case 3 :
                throw ast::ScilabError(_W("Division by zero...\n"));
            case 4 :
                if (getWarningMode())
                {
                    sciprint(_("Warning : Division by zero...\n"));
                }
                break;
                //            default : throw ast::ScilabError(_W("Operator / : Error %d not yet managed.\n"), iResult);
            default :
                sciprint(_("Operator ./ : Error %d not yet managed.\n"), iResult);
        }
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return pResult;
}

int RDivideDoubleByDouble(Double *_pDouble1, Double *_pDouble2, Double **_pDoubleOut)
{
    int iErr = 0;

    //check finite values of _pDouble1 and _pDouble2
    if (isDoubleFinite(_pDouble1) == false || isDoubleFinite(_pDouble2) == false)
    {
        if (_pDouble2->isScalar() == false)
        {
            return 2;
        }
    }

    if (_pDouble2->isScalar())
    {
        //Y / x
        int iInc1       = 1;
        int iInc2       = 0;
        bool bComplex1  = _pDouble1->isComplex();
        bool bComplex2  = _pDouble2->isComplex();

        *_pDoubleOut    = new Double(_pDouble1->getDims(), _pDouble1->getDimsArray(), bComplex1 || bComplex2);

        if (bComplex1 == false && bComplex2 == false)
        {
            // Real1 \ Real2 -> Real2 / Real1
            iErr = iRightDivisionRealMatrixByRealMatrix(
                       _pDouble1->get(), iInc1,
                       _pDouble2->get(), iInc2,
                       (*_pDoubleOut)->get(), 1, _pDouble1->getSize());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            // Real \ Complex -> Complex / Real
            iErr = iRightDivisionRealMatrixByComplexMatrix(
                       _pDouble1->get(), iInc1,
                       _pDouble2->get(), _pDouble2->getImg(), iInc2,
                       (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg(), 1, _pDouble1->getSize());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            // Complex \ Real -> Real / Complex
            iErr = iRightDivisionComplexMatrixByRealMatrix(
                       _pDouble1->get(), _pDouble1->getImg(), iInc1,
                       _pDouble2->get(), iInc2,
                       (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg(), 1, _pDouble1->getSize());
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            // Complex \ Complex
            iErr = iRightDivisionComplexMatrixByComplexMatrix(
                       _pDouble1->get(), _pDouble1->getImg(), iInc1,
                       _pDouble2->get(), _pDouble2->getImg(), iInc2,
                       (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg(), 1, _pDouble1->getSize());
        }

        return iErr;
    }

    if (_pDouble1->isScalar())
    {
        if (_pDouble2->getDims() > 2)
        {
            //not managed, call overload
            return 0;
        }

        double dblSavedR = 0;
        double dblSavedI = 0;
        Double *pdblTemp = NULL;

        int iRowResult = _pDouble2->getCols();
        int iColResult = _pDouble2->getRows();

        //in this case, we have to create a temporary square matrix
        pdblTemp = new Double(iRowResult, iRowResult, _pDouble1->isComplex());
        pdblTemp->setZeros();

        if (_pDouble1->isComplex())
        {
            dblSavedR = _pDouble1->getReal()[0];
            dblSavedI = _pDouble1->getImg()[0];
            for (int i = 0 ; i < iRowResult ; i++)
            {
                pdblTemp->set(i, i, dblSavedR);
                pdblTemp->setImg(i, i, dblSavedI);
            }
        }
        else
        {
            dblSavedR = _pDouble1->getReal()[0];
            for (int i = 0 ; i < iRowResult ; i++)
            {
                pdblTemp->set(i, i, dblSavedR);
            }
        }

        *_pDoubleOut = new Double(iRowResult, iColResult, _pDouble1->isComplex() || _pDouble2->isComplex());

        if ((*_pDoubleOut)->isComplex())
        {
            double dblRcond = 0;
            iErr = iRightDivisionOfComplexMatrix(
                       pdblTemp->getReal(), pdblTemp->getImg(), pdblTemp->getRows(), pdblTemp->getCols(),
                       _pDouble2->getReal(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
                       (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), iRowResult, iColResult, &dblRcond);
        }
        else
        {
            double dblRcond = 0;
            iErr = iRightDivisionOfRealMatrix(
                       pdblTemp->getReal(), pdblTemp->getRows(), pdblTemp->getCols(),
                       _pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getCols(),
                       (*_pDoubleOut)->getReal(), iRowResult, iColResult, &dblRcond);
        }
        delete pdblTemp;
        return iErr;
    }

    if (_pDouble1->getDims() > 2 || _pDouble2->getDims() > 2 || _pDouble1->getCols() != _pDouble2->getCols())
    {
        //not managed
        return 1;
    }

    *_pDoubleOut = new Double(_pDouble1->getRows(), _pDouble2->getRows(), _pDouble1->isComplex() || _pDouble2->isComplex());
    if ((*_pDoubleOut)->isComplex())
    {
        double dblRcond = 0;
        iErr = iRightDivisionOfComplexMatrix(
                   _pDouble1->getReal(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
                   _pDouble2->getReal(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
                   (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), _pDouble1->getRows(), _pDouble2->getRows(), &dblRcond);
    }
    else
    {
        double dblRcond = 0;
        iErr = iRightDivisionOfRealMatrix(
                   _pDouble1->getReal(), _pDouble1->getRows(), _pDouble1->getCols(),
                   _pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getCols(),
                   (*_pDoubleOut)->getReal(), _pDouble1->getRows(), _pDouble2->getRows(), &dblRcond);
    }

    return iErr;
}

int RDividePolyByDouble(Polynom* _pPoly, Double* _pDouble, Polynom** _pPolyOut)
{
    bool bComplex1  = _pPoly->isComplex();
    bool bComplex2  = _pDouble->isComplex();
    bool bScalar1   = _pPoly->getRows() == 1  && _pPoly->getCols() == 1;
    bool bScalar2   = _pDouble->getRows() == 1 && _pDouble->getCols() == 1;

    Polynom *pTemp = NULL; //use only if _pPoly is scalar and _pDouble not.

    int iRowResult  = 0;
    int iColResult = 0;
    int *piRank   = NULL;

    /* if(bScalar1 && bScalar2)
    {
    iRowResult = 1;
    iColResult = 1;

    piRank = new int[1];
    piRank[0] = _pPoly->get(0)->getRank();
    }
    else */

    if (bScalar2)
    {
        double dblDivR = _pDouble->get(0);
        double dblDivI = _pDouble->getImg(0);

        (*_pPolyOut) = _pPoly->clone()->getAs<Polynom>();
        if (_pDouble->isComplex())
        {
            (*_pPolyOut)->setComplex(true);
        }

        for (int i = 0 ; i < _pPoly->getSize() ; i++)
        {
            bool bComplex1 = _pPoly->isComplex();
            bool bComplex2 = _pDouble->isComplex();

            Double* pC = (*_pPolyOut)->get(i)->getCoef();

            if (bComplex1 == false && bComplex2 == false)
            {
                iRightDivisionRealMatrixByRealMatrix(pC->get(), 1, &dblDivR, 0, pC->get(), 1, pC->getSize());
            }
            else if (bComplex1 == true && bComplex2 == false)
            {
                iRightDivisionComplexMatrixByRealMatrix(pC->get(), pC->getImg(), 1, &dblDivR, 0, pC->get(), pC->getImg(), 1, pC->getSize());
            }
            else if (bComplex1 == false && bComplex2 == true)
            {
                iRightDivisionRealMatrixByComplexMatrix(pC->get(), 1, &dblDivR, &dblDivI, 0, pC->get(), pC->getImg(), 1, pC->getSize());
            }
            else if (bComplex1 == true && bComplex2 == true)
            {
                iRightDivisionComplexMatrixByComplexMatrix(pC->get(), pC->getImg(), 1, &dblDivR, &dblDivI, 0, pC->get(), pC->getImg(), 1, pC->getSize());
            }
        }

        return 0;
    }

    if (bScalar1)
    {
        //in this case, we have to create a temporary square polinomial matrix
        iRowResult = _pDouble->getCols();
        iColResult = _pDouble->getRows();

        piRank = new int[iRowResult * iRowResult];
        for (int i = 0 ; i < iRowResult * iRowResult ; i++)
        {
            piRank[i] = _pPoly->getMaxRank();
        }

        pTemp = new Polynom(_pPoly->getVariableName(), iRowResult, iRowResult, piRank);
        if (bComplex1 || bComplex2)
        {
            pTemp->setComplex(true);
        }

        Double *pdblData = _pPoly->get(0)->getCoef();
        for (int i = 0 ; i < iRowResult ; i++)
        {
            pTemp->setCoef(i, i, pdblData);
        }
    }

    (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), iRowResult, iColResult, piRank);
    delete[] piRank;
    if (bComplex1 || bComplex2)
    {
        (*_pPolyOut)->setComplex(true);
    }

    if (bScalar2)
    {
        //[p] * cst
        for (int i = 0 ; i < _pPoly->getSize() ; i++)
        {
            SinglePoly *pPolyIn   = _pPoly->get(i);
            double* pRealIn  = pPolyIn->getCoef()->getReal();
            double* pImgIn  = pPolyIn->getCoef()->getImg();

            SinglePoly *pPolyOut  = (*_pPolyOut)->get(i);
            double* pRealOut = pPolyOut->getCoef()->getReal();
            double* pImgOut  = pPolyOut->getCoef()->getImg();

            if (bComplex1 == false && bComplex2 == false)
            {
                iRightDivisionRealMatrixByRealMatrix(
                    pRealIn, 1,
                    _pDouble->getReal(), 0,
                    pRealOut, 1, pPolyOut->getRank());
            }
            else if (bComplex1 == false && bComplex2 == true)
            {
                iRightDivisionRealMatrixByComplexMatrix(
                    pRealIn, 1,
                    _pDouble->getReal(), _pDouble->getImg(), 0,
                    pRealOut, pImgOut, 1, pPolyOut->getRank());
            }
            else if (bComplex1 == true && bComplex2 == false)
            {
                iRightDivisionComplexMatrixByRealMatrix(
                    pRealIn, pImgIn, 1,
                    _pDouble->getReal(), 0,
                    pRealOut, pImgOut, 1, pPolyOut->getRank());
            }
            else if (bComplex1 == true && bComplex2 == true)
            {
                iRightDivisionComplexMatrixByComplexMatrix(
                    pRealIn, pImgIn, 1,
                    _pDouble->getReal(), _pDouble->getImg(), 0,
                    pRealOut, pImgOut, 1, pPolyOut->getRank());
            }
        }
    }
    else if (bScalar1)
    {
        Double *pResultCoef = new Double(iRowResult, iColResult, (*_pPolyOut)->isComplex());
        double *pReal    = pResultCoef->getReal();
        double *pImg    = pResultCoef->getImg();

        for (int i = 0 ; i < pTemp->get(0)->getRank() ; i++)
        {
            Double *pCoef    = pTemp->extractCoef(i);
            Double *pResultCoef = new Double(iRowResult, iColResult, pCoef->isComplex());
            double *pReal    = pResultCoef->getReal();
            double *pImg    = pResultCoef->getImg();

            if (bComplex1 == false && bComplex2 == false)
            {
                double dblRcond = 0;
                iRightDivisionOfRealMatrix(
                    pCoef->getReal(), iRowResult, iRowResult,
                    _pDouble->getReal(), _pDouble->getRows(), _pDouble->getCols(),
                    pReal, iRowResult, iColResult, &dblRcond);
            }
            else
            {
                double dblRcond = 0;
                iRightDivisionOfComplexMatrix(
                    pCoef->getReal(), pCoef->getImg(), iRowResult, iRowResult,
                    _pDouble->getReal(), _pDouble->getImg(), _pDouble->getRows(), _pDouble->getCols(),
                    pReal, pImg, iRowResult, iColResult, &dblRcond);
            }

            (*_pPolyOut)->insertCoef(i, pResultCoef);
            delete pCoef;
        }
        delete pResultCoef;
    }
    return 0;
}

int RDivideDoubleByPoly(Double* _pDouble, Polynom* _pPoly, Polynom** _pPolyOut)
{
    return 0;
}

int DotRDivideDoubleByDouble(Double* _pDouble1, Double* _pDouble2, Double** _pDoubleOut)
{
    int iErr        = 0;
    bool bComplex1  = _pDouble1->isComplex();
    bool bComplex2  = _pDouble2->isComplex();
    bool bScalar1   = _pDouble1->isScalar();
    bool bScalar2   = _pDouble2->isScalar();

    if (bScalar1)
    {
        //x ./ Y
        int iInc1       = 0;
        int iInc2       = 1;
        int iIncOut     = 1;
        int iSizeResult = _pDouble2->getSize();

        *_pDoubleOut    = new Double(_pDouble2->getDims(), _pDouble2->getDimsArray(), bComplex1 || bComplex2);

        if (bComplex1 == false && bComplex2 == false)
        {
            // r ./ R
            iErr = iRightDivisionRealMatrixByRealMatrix(
                       _pDouble1->getReal(), iInc1,
                       _pDouble2->getReal(), iInc2,
                       (*_pDoubleOut)->getReal(), iIncOut, iSizeResult);
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            // r ./ C
            iErr = iRightDivisionRealMatrixByComplexMatrix(
                       _pDouble1->getReal(), iInc1,
                       _pDouble2->getReal(), _pDouble2->getImg(), iInc2,
                       (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), iIncOut, iSizeResult);
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            // c ./ R
            iErr = iRightDivisionComplexMatrixByRealMatrix(
                       _pDouble1->getReal(), _pDouble1->getImg(), iInc1,
                       _pDouble2->getReal(), iInc2,
                       (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), iIncOut, iSizeResult);
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            // c ./ C
            iErr = iRightDivisionComplexMatrixByComplexMatrix(
                       _pDouble1->getReal(), _pDouble1->getImg(), iInc1,
                       _pDouble2->getReal(), _pDouble2->getImg(), iInc2,
                       (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), iIncOut, iSizeResult);
        }
    }
    else if (bScalar2)
    {
        //X ./ y
        int iInc1       = 1;
        int iInc2       = 0;
        int iIncOut     = 1;
        int iSizeResult = _pDouble1->getSize();

        *_pDoubleOut    = new Double(_pDouble1->getDims(), _pDouble1->getDimsArray(), bComplex1 || bComplex2);

        if (bComplex1 == false && bComplex2 == false)
        {
            // r ./ R
            iErr = iRightDivisionRealMatrixByRealMatrix(
                       _pDouble1->getReal(), iInc1,
                       _pDouble2->getReal(), iInc2,
                       (*_pDoubleOut)->getReal(), iIncOut, iSizeResult);
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            // r ./ C
            iErr = iRightDivisionRealMatrixByComplexMatrix(
                       _pDouble1->getReal(), iInc1,
                       _pDouble2->getReal(), _pDouble2->getImg(), iInc2,
                       (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), iIncOut, iSizeResult);
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            // c ./ R
            iErr = iRightDivisionComplexMatrixByRealMatrix(
                       _pDouble1->getReal(), _pDouble1->getImg(), iInc1,
                       _pDouble2->getReal(), iInc2,
                       (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), iIncOut, iSizeResult);
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            // c ./ C
            iErr = iRightDivisionComplexMatrixByComplexMatrix(
                       _pDouble1->getReal(), _pDouble1->getImg(), iInc1,
                       _pDouble2->getReal(), _pDouble2->getImg(), iInc2,
                       (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), iIncOut, iSizeResult);
        }
    }
    else
    {
        //X ./ Y
        //check dimension compatibilities ( same number of dimension and same size for each dimension
        int iDims1      = _pDouble1->getDims();
        int* piDims1    = _pDouble1->getDimsArray();
        int iDims2      = _pDouble2->getDims();
        int* piDims2    = _pDouble2->getDimsArray();

        if (iDims1 != iDims2)
        {
            return 1;
        }

        for (int i = 0 ; i < iDims1 ; i++)
        {
            if (piDims1[i] != piDims2[i])
            {
                return 1;
            }
        }

        (*_pDoubleOut) = new Double(iDims2, piDims2, bComplex1 || bComplex2);

        int iErr        = 0;
        int iInc1       = 1;
        int iInc2       = 1;
        int iIncOut     = 1;
        int iSizeResult = _pDouble1->getSize();

        if (bComplex1 == false && bComplex2 == false)
        {
            // r ./ R
            iErr = iRightDivisionRealMatrixByRealMatrix(
                       _pDouble1->getReal(), iInc1,
                       _pDouble2->getReal(), iInc2,
                       (*_pDoubleOut)->getReal(), iIncOut, iSizeResult);
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            // r ./ C
            iErr = iRightDivisionRealMatrixByComplexMatrix(
                       _pDouble1->getReal(), iInc1,
                       _pDouble2->getReal(), _pDouble2->getImg(), iInc2,
                       (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), iIncOut, iSizeResult);
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            // c ./ R
            iErr = iRightDivisionComplexMatrixByRealMatrix(
                       _pDouble1->getReal(), _pDouble1->getImg(), iInc1,
                       _pDouble2->getReal(), iInc2,
                       (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), iIncOut, iSizeResult);
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            // c ./ C
            iErr = iRightDivisionComplexMatrixByComplexMatrix(
                       _pDouble1->getReal(), _pDouble1->getImg(), iInc1,
                       _pDouble2->getReal(), _pDouble2->getImg(), iInc2,
                       (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), iIncOut, iSizeResult);
        }
    }
    return iErr;
}

int DotRDividePolyByDouble(Polynom* _pPoly1, Double* _pDouble2, Polynom** _pPolyOut)
{
    int iErr        = 0;
    bool bComplex1  = _pPoly1->isComplex();
    bool bComplex2  = _pDouble2->isComplex();

    //X ./ Y
    //check dimension compatibilities ( same number of dimension and same size for each dimension
    int iDims1      = _pPoly1->getDims();
    int* piDims1    = _pPoly1->getDimsArray();
    int iMaxRank    = _pPoly1->getMaxRank();
    int iSizePoly   = _pPoly1->getSize();
    int iDims2      = _pDouble2->getDims();
    int* piDims2    = _pDouble2->getDimsArray();

    if (iDims1 != iDims2)
    {
        return 1;
    }

    for (int i = 0 ; i < iDims1 ; i++)
    {
        if (piDims1[i] != piDims2[i])
        {
            return 1;
        }
    }

    // compute output ranks
    int* piRanks = new int[iSizePoly];
    for(int i = 0; i < iSizePoly; i++)
    {
        piRanks[i] = iMaxRank;
    }

    // create output and working table
    (*_pPolyOut) = new Polynom(_pPoly1->getVariableName(), iDims2, piDims2, piRanks);
    delete[] piRanks;
    Double* pDblCoefOut = new Double(_pPoly1->getRows(), _pPoly1->getCols() * iMaxRank, bComplex1 || bComplex2);
    double* pdblCoef2   = new double[_pPoly1->getRows() * _pPoly1->getCols() * iMaxRank];
    Double* pDblCoef1   = _pPoly1->getCoef();

    int iZero = 0;
    double* pdbl = _pDouble2->get();
    for(int i = 0; i < iSizePoly; i++)
    {
        C2F(dcopy)(&iMaxRank, pdbl + i, &iZero, pdblCoef2 + i, &iSizePoly);
    }

    int iInc1       = 1;
    int iInc2       = 1;
    int iIncOut     = 1;
    int iSizeResult = pDblCoefOut->getSize();

    if (bComplex1 == false && bComplex2 == false)
    {
        // r ./ R
        iErr = iRightDivisionRealMatrixByRealMatrix(
                   pDblCoef1->getReal(), iInc1,
                   pdblCoef2, iInc2,
                   pDblCoefOut->getReal(), iIncOut, iSizeResult);
    }
    else if (bComplex1 == false && bComplex2 == true)
    {
        // r ./ C
//        iErr = iRightDivisionRealMatrixByComplexMatrix(
//                   _pDouble1->getReal(), iInc1,
//                   _pDouble2->getReal(), _pDouble2->getImg(), iInc2,
//                   (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), iIncOut, iSizeResult);

        // waiting for polynom rewrite about complex
        iErr = 10;
    }
    else if (bComplex1 == true && bComplex2 == false)
    {
        // c ./ R
//        iErr = iRightDivisionComplexMatrixByRealMatrix(
//                   _pDouble1->getReal(), _pDouble1->getImg(), iInc1,
//                   _pDouble2->getReal(), iInc2,
//                   (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), iIncOut, iSizeResult);

        // waiting for polynom rewrite about complex
        iErr = 10;
    }
    else if (bComplex1 == true && bComplex2 == true)
    {
        // c ./ C
//        iErr = iRightDivisionComplexMatrixByComplexMatrix(
//                   _pDouble1->getReal(), _pDouble1->getImg(), iInc1,
//                   _pDouble2->getReal(), _pDouble2->getImg(), iInc2,
//                   (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), iIncOut, iSizeResult);

        // waiting for polynom rewrite about complex
        iErr = 10;
    }

    (*_pPolyOut)->setCoef(pDblCoefOut);
    (*_pPolyOut)->updateRank();

    delete pDblCoefOut;
    delete pDblCoef1;
    delete[] pdblCoef2;

    return iErr;
}
