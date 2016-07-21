/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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

#include "types_divide.hxx"
#include "types_finite.hxx"

extern "C"
{
#include "matrix_right_division.h"
#include "sciprint.h"
#include "localization.h"
#include "charEncoding.h"
#include "configvariable_interface.h"
#include "elem_common.h"
}

using namespace types;

InternalType *GenericRDivide(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult       = NULL;
    GenericType::ScilabType TypeL = _pLeftOperand->getType();
    GenericType::ScilabType TypeR = _pRightOperand->getType();

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
    if (TypeL == GenericType::ScilabDouble && TypeR == GenericType::ScilabDouble)
    {
        Double *pL  = _pLeftOperand->getAs<Double>();
        Double *pR  = _pRightOperand->getAs<Double>();

        iResult = RDivideDoubleByDouble(pL, pR, (Double**)&pResult);
    }

    /*
    ** POLY / DOUBLE
    */
    else if (TypeL == GenericType::ScilabPolynom && TypeR == GenericType::ScilabDouble)
    {
        Polynom *pL = _pLeftOperand->getAs<types::Polynom>();
        Double *pR  = _pRightOperand->getAs<Double>();

        iResult = RDividePolyByDouble(pL, pR, (Polynom**)&pResult);
    }

    /*
    ** DOUBLE / POLY
    */
    else if (TypeL == GenericType::ScilabDouble && TypeR == GenericType::ScilabPolynom)
    {
        Double *pL  = _pLeftOperand->getAs<Double>();
        Polynom *pR = _pRightOperand->getAs<types::Polynom>();

        iResult = RDivideDoubleByPoly(pL, pR, (Polynom**)&pResult);
    }

    /*
    ** SPARSE / DOUBLE
    */
    else if (TypeL == GenericType::ScilabSparse && TypeR == GenericType::ScilabDouble)
    {
        Sparse *pL = _pLeftOperand->getAs<types::Sparse>();
        Double *pR = _pRightOperand->getAs<Double>();

        iResult = RDivideSparseByDouble(pL, pR, &pResult);
    }

    //manage errors
    if (iResult)
    {
        switch (iResult)
        {
            case 1 :
                throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
            case 2 :
                throw ast::InternalError(_W("With NaN or Inf a division by scalar expected.\n"));
            case 3 :
                throw ast::InternalError(_W("Division by zero...\n"));
            case 4 :
                if (getWarningMode())
                {
                    sciprint(_("Warning : Division by zero...\n"));
                }
                break;
            //            default : throw ast::InternalError(_W("Operator / : Error %d not yet managed.\n"), iResult);
            default :
                sciprint(_("Operator / : Error %d not yet managed.\n"), iResult);
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

        // x / eye() = x
        if (_pDouble2->isIdentity() )
        {
            *_pDoubleOut    = new Double(*_pDouble1);
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
        return 0;
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

    if (bScalar1 == false && bScalar2 == false)
    {
        // call overload
        return 0;
    }

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

            SinglePoly* pC = (*_pPolyOut)->get(i);

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
        int iMaxRank = _pPoly->getMaxRank();
        for (int i = 0 ; i < iRowResult * iRowResult ; i++)
        {
            piRank[i] = iMaxRank;
        }

        pTemp = new Polynom(_pPoly->getVariableName(), iRowResult, iRowResult, piRank);
        if (bComplex1 || bComplex2)
        {
            pTemp->setComplex(true);
        }

        SinglePoly *pdblData = _pPoly->get(0);
        for (int i = 0 ; i < iRowResult ; i++)
        {
            pTemp->set(i, i, pdblData);
        }
    }

    (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), iRowResult, iColResult, piRank);
    delete[] piRank;
    if (bComplex1 || bComplex2)
    {
        (*_pPolyOut)->setComplex(true);
    }

    if (bScalar1)
    {
        for (int i = 0 ; i < pTemp->get(0)->getSize() ; i++)
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
            delete pResultCoef;
        }
    }
    return 0;
}

int RDivideDoubleByPoly(Double* /*_pDouble*/, Polynom* /*_pPoly*/, Polynom** /*_pPolyOut*/)
{
    return 0;
}

int RDivideSparseByDouble(types::Sparse* _pSp, types::Double* _pDouble, InternalType** _pSpOut)
{
    if (_pDouble->isEmpty())
    {
        //sp / []
        *_pSpOut = Double::Empty();
        return 0;
    }

    if (_pDouble->isIdentity())
    {
        *_pSpOut = new Sparse(*_pSp);
        return 0;
    }

    size_t iSize = _pSp->nonZeros();
    int* Col = new int[iSize];
    int* Row = new int[_pSp->getRows()];
    _pSp->getColPos(Col);
    _pSp->getNbItemByRow(Row);
    int* iPositVal = new int[iSize];

    int idx = 0;
    for (int i = 0; i < _pSp->getRows(); i++)
    {
        for (int j = 0; j < Row[i]; j++)
        {
            iPositVal[idx] = (Col[idx] - 1) * _pSp->getRows() + i;
            ++idx;
        }
    }

    delete[] Col;
    delete[] Row;

    Double** pDbl = new Double*[iSize];
    Double** pDblSp = new Double*[iSize];
    double* pdbl = _pDouble->get();

    if (_pDouble->isScalar())
    {
        if (_pDouble->isComplex())
        {
            double* pdblImg = _pDouble->getImg();
            for (int i = 0; i < iSize; i++)
            {
                pDbl[i] = new Double(pdbl[0], pdblImg[0]);
                pDblSp[i] = new Double(_pSp->get(iPositVal[i]), _pSp->getImg(iPositVal[i]).imag());
            }
        }
        else
        {
            for (int i = 0; i < iSize; i++)
            {
                pDbl[i] = new Double(pdbl[0]);
                pDblSp[i] = new Double(_pSp->getReal(iPositVal[i]), _pSp->getImg(iPositVal[i]).imag());
            }
        }
    }
    else if (_pDouble->getSize() == iSize)
    {
        if (_pDouble->isComplex())
        {
            double* pdblImg = _pDouble->getImg();
            for (int i = 0; i < iSize; i++)
            {
                pDbl[i] = new Double(pdbl[i], pdblImg[i]);
                pDblSp[i] = new Double(_pSp->getReal(iPositVal[i]), _pSp->getImg(iPositVal[i]).imag());
            }
        }
        else
        {
            for (int i = 0; i < iSize; i++)
            {
                pDbl[i] = new Double(pdbl[i]);
                pDblSp[i] = new Double(_pSp->getReal(iPositVal[i]), _pSp->getImg(iPositVal[i]).imag());
            }
        }
    }
    else
    {
        delete[] pDbl;
        delete[] pDblSp;
        delete[] iPositVal;
        return 0;
    }

    Sparse* pSpTemp = new Sparse(_pSp->getRows(), _pSp->getCols(), _pSp->isComplex() || _pDouble->isComplex());
    pSpTemp->zero_set();

    Double* ppDblGet = NULL;
    int iResultat;
    for (int i = 0; i < iSize; i++)
    {
        if ((pDblSp[i]->get(0) != 0) || (pDblSp[i]->getImg(0) != 0))
        {
            iResultat = RDivideDoubleByDouble(pDblSp[i], pDbl[i], &ppDblGet);
            if (iResultat != 0)
            {
                for (int i = 0; i < iSize; ++i)
                {
                    delete pDbl[i];
                    delete pDblSp[i];
                }
                delete[] pDbl;
                delete[] pDblSp;
                delete[] iPositVal;
                delete pSpTemp;
                delete ppDblGet;
                return iResultat;
            }
            std::complex<double> cplx(ppDblGet->get(0), ppDblGet->getImg(0));
            pSpTemp->set(iPositVal[i], cplx, false);
            delete ppDblGet;
        }
    }

    pSpTemp->finalize();

    delete[] iPositVal;

    for (int i = 0; i < iSize; ++i)
    {
        delete pDbl[i];
        delete pDblSp[i];
    }

    delete[] pDbl;
    delete[] pDblSp;

    *_pSpOut = pSpTemp;
    return 0;
}
