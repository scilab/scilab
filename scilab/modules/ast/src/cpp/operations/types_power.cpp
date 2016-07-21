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

#include "types_power.hxx"
#include "types_multiplication.hxx"

extern "C"
{
#include "operation_f.h"
#include "matrix_power.h"
#include "localization.h"
#include "charEncoding.h"
}

using namespace types;

InternalType *GenericPower(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;

    /*
    ** DOUBLE ^ DOUBLE
    ** DOUBLE ** DOUBLE
    */
    if (_pLeftOperand->isDouble() && _pRightOperand->isDouble())
    {
        Double *pL   = _pLeftOperand->getAs<Double>();
        Double *pR   = _pRightOperand->getAs<Double>();

        int iResult = PowerDoubleByDouble(pL, pR, (Double**)&pResult);
        if (iResult)
        {
            throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** POLY ^ DOUBLE
    ** POLY ** DOUBLE
    */
    if (_pLeftOperand->isPoly() && _pRightOperand->isDouble())
    {
        Polynom *pL   = _pLeftOperand->getAs<Polynom>();
        Double *pR    = _pRightOperand->getAs<Double>();

        int iResult = PowerPolyByDouble(pL, pR, &pResult);
        switch (iResult)
        {
            case 1 :
                throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
            case 2 :
                throw ast::InternalError(_W("Invalid exponent: expected real exponents.\n"));
            default:
                //OK
                break;
        }

        return pResult;
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return NULL;

}

InternalType *GenericDotPower(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;
    GenericType::ScilabType TypeL = _pLeftOperand->getType();
    GenericType::ScilabType TypeR = _pRightOperand->getType();

    /*
    ** DOUBLE .^ DOUBLE
    ** DOUBLE .** DOUBLE
    */
    if (TypeL == GenericType::ScilabDouble && TypeR == GenericType::ScilabDouble)
    {
        Double *pL  = _pLeftOperand->getAs<Double>();
        Double *pR  = _pRightOperand->getAs<Double>();

        int iResult = DotPowerDoubleByDouble(pL, pR, (Double**)&pResult);
        if (iResult)
        {
            throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }
    /*
    ** SPARSE  .^ DOUBLE
    ** SPARSE .** DOUBLE
    */
    if (TypeL == GenericType::ScilabSparse && TypeR == GenericType::ScilabDouble)
    {
        types::Sparse *pL = _pLeftOperand->getAs<types::Sparse>();
        Double *pR = _pRightOperand->getAs<Double>();
        int iResult = DotPowerSpaseByDouble(pL, pR, &pResult);
        if (iResult)
        {
            throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
        }
        return pResult;

    }

    /*
    ** POLY .^ DOUBLE
    ** POLY .** DOUBLE
    */
    if (TypeL == GenericType::ScilabPolynom && TypeR == GenericType::ScilabDouble)
    {
        Polynom *pL   = _pLeftOperand->getAs<Polynom>();
        Double *pR   = _pRightOperand->getAs<Double>();

        int iResult = DotPowerPolyByDouble(pL, pR, &pResult);
        switch (iResult)
        {
            case 1 :
                throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
            case 2 :
                throw ast::InternalError(_W("Invalid exponent: expected real exponents.\n"));
            default:
                //OK
                break;
        }
        return pResult;
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return NULL;

}

int PowerDoubleByDouble(Double* _pDouble1, Double* _pDouble2, Double** _pDoubleOut)
{
    bool bComplex1  = _pDouble1->isComplex();
    bool bComplex2  = _pDouble2->isComplex();
    bool bScalar1   = _pDouble1->isScalar();
    bool bScalar2   = _pDouble2->isScalar();

    int iComplex = 1;

    if (bScalar1)
    {
        if (bScalar2)
        {
            //s ^ s
            *_pDoubleOut = new Double(1, 1, true);

            if (bComplex1 == false && bComplex2 == false)
            {
                iPowerRealScalarByRealScalar(_pDouble1->get(0), _pDouble2->get(0), (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg(), &iComplex);
            }
            else if (bComplex1 == false && bComplex2 == true)
            {
                iPowerRealScalarByComplexScalar(_pDouble1->get(0), _pDouble2->get(0), _pDouble2->getImg(0), (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
            }
            else if (bComplex1 == true && bComplex2 == false)
            {
                iPowerComplexScalarByRealScalar(_pDouble1->get(0), _pDouble1->getImg(0), _pDouble2->get(0), (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
            }
            else if (bComplex1 == true && bComplex2 == true)
            {
                iPowerComplexScalarByComplexScalar(_pDouble1->get(0), _pDouble1->getImg(0), _pDouble2->get(0), _pDouble2->getImg(0), (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
            }

            if (iComplex == 0)
            {
                (*_pDoubleOut)->setComplex(false);
            }

            return 0;
        }
        else if (_pDouble2->isVector())
        {
            //s ^ Vector == s .^ Vector
            *_pDoubleOut = new Double(_pDouble2->getDims(), _pDouble2->getDimsArray(), true);

            if (bComplex1 == false && bComplex2 == false)
            {
                iPowerRealScalarByRealMatrix(
                    _pDouble1->get(0),
                    _pDouble2->get(), 1, _pDouble2->getSize(),
                    (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg(), &iComplex);
            }
            else if (bComplex1 == false && bComplex2 == true)
            {
                iPowerRealScalarByComplexMatrix(
                    _pDouble1->get(0),
                    _pDouble2->get(), _pDouble2->getImg(), 1, _pDouble2->getSize(),
                    (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
            }
            else if (bComplex1 == true && bComplex2 == false)
            {
                iPowerComplexScalarByRealMatrix(
                    _pDouble1->get(0), _pDouble1->getImg(0),
                    _pDouble2->get(), 1, _pDouble2->getSize(),
                    (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
            }
            else if (bComplex1 == true && bComplex2 == true)
            {
                iPowerComplexScalarByComplexMatrix(
                    _pDouble1->get(0), _pDouble1->getImg(0),
                    _pDouble2->get(), _pDouble2->getImg(), 1, _pDouble2->getSize(),
                    (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
            }

            if (iComplex == 0)
            {
                (*_pDoubleOut)->setComplex(false);
            }

            return 0;
        }
        else
        {
            // s ^ Matrix
            // call overload %s_pow
            return 0;
        }
    }

    if (bScalar2)
    {
        if (_pDouble1->isVector())
        {
            //_pDouble1 is a vector and _pDouble is a scalar
            *_pDoubleOut = new Double(_pDouble1->getDims(), _pDouble1->getDimsArray() , true);

            if (bComplex1 == false && bComplex2 == false)
            {
                for (int i = 0 ; i < (*_pDoubleOut)->getSize() ; i++)
                {
                    iPowerRealScalarByRealScalar(
                        _pDouble1->get(i),
                        _pDouble2->get(0),
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i], &iComplex);
                }
            }
            else if (bComplex1 == false && bComplex2 == true)
            {
                for (int i = 0 ; i < (*_pDoubleOut)->getSize() ; i++)
                {
                    iPowerRealScalarByComplexScalar(
                        _pDouble1->get(i),
                        _pDouble2->get(0), _pDouble2->getImg(0),
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }
            else if (bComplex1 == true && bComplex2 == false)
            {
                for (int i = 0 ; i < (*_pDoubleOut)->getSize() ; i++)
                {
                    iPowerComplexScalarByRealScalar(
                        _pDouble1->get(i), _pDouble1->getImg(i),
                        _pDouble2->get(0),
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }
            else if (bComplex1 == true && bComplex2 == true)
            {
                for (int i = 0 ; i < (*_pDoubleOut)->getSize() ; i++)
                {
                    iPowerComplexScalarByComplexScalar(
                        _pDouble1->get(i), _pDouble1->getImg(i),
                        _pDouble2->get(0), _pDouble2->getImg(0),
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }

            if (iComplex == 0)
            {
                (*_pDoubleOut)->setComplex(false);
            }

            return 0;
        }
        else if (_pDouble1->getRows() == _pDouble1->getCols())
        {
            //power of a square matrix by a scalar exponent.
            int iRet = 0;
            if (bComplex2)
            {
                // mange by overloading
                // Call %s_pow
                return 0;
            }

            *_pDoubleOut = new Double(_pDouble1->getRows(), _pDouble1->getCols() , true);
            if (bComplex1 == false)
            {
                iRet = iPowerRealSquareMatrixByRealScalar(
                           _pDouble1->get(), _pDouble1->getRows(), _pDouble1->getCols(),
                           _pDouble2->get(0),
                           (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg(), &iComplex);
            }
            else if (bComplex1 == true)
            {
                iRet = iPowerComplexSquareMatrixByRealScalar(
                           _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
                           _pDouble2->get(0),
                           (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
            }

            // call overload
            if (iRet == -1)
            {
                delete *_pDoubleOut;
                *_pDoubleOut = NULL;
                return 0;
            }

            if (iComplex == 0)
            {
                (*_pDoubleOut)->setComplex(false);
            }
        }
    }
    return 0;
}

int PowerPolyByDouble(Polynom* _pPoly, Double* _pDouble, InternalType** _pOut)
{
    bool bComplex1  = _pPoly->isComplex();
    bool bScalar1   = _pPoly->isScalar();
    double* bImg    = _pDouble->getImg();
    bool bNumericallyComplex1 = _pDouble->isNumericallyComplex();

    if (!bNumericallyComplex1)
    {
        return 2;
    }

    if (_pDouble->isEmpty())
    {
        //p ** []
        *_pOut = Double::Empty();
        return 0;
    }

    if (bScalar1)
    {
        //p ^ x or p ^ X
        int iRank   = 0;
        int* piRank = new int[_pDouble->getSize()];

        _pPoly->getRank(&iRank);
        for (int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            int iInputRank = (int)_pDouble->get(i);
            if (iInputRank < 0)
            {
                //call overload
                _pOut = NULL;
                delete[] piRank;
                return 0;
            }

            piRank[i] = iRank * iInputRank;
        }

        Polynom* pOut = new Polynom(_pPoly->getVariableName(), _pDouble->getRows(), _pDouble->getCols(), piRank);
        delete[] piRank;
        pOut->setComplex(bComplex1);

        for (int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            SinglePoly* pCoeffOut = pOut->get(i);

            int iCurrentRank    = 0;
            int iLoop           = (int)_pDouble->get(i);

            //initialize Out to 1
            pCoeffOut->set(0, 1);
            //get a copy of p
            Polynom* pP = _pPoly->clone()->getAs<Polynom>();
            pP->setComplex(_pPoly->isComplex());

            while (iLoop)
            {
                SinglePoly* ps = pP->get()[0];
                if (iLoop % 2)
                {
                    int iRank = pP->getMaxRank();
                    if (bComplex1)
                    {
                        C2F(wpmul1)(pCoeffOut->get(), pCoeffOut->getImg(), &iCurrentRank, ps->get(), ps->getImg(), &iRank, pCoeffOut->get(), pCoeffOut->getImg());
                    }
                    else
                    {
                        C2F(dpmul1)(pCoeffOut->get(), &iCurrentRank, ps->get(), &iRank, pCoeffOut->get());
                    }
                    iCurrentRank += iRank;
                }

                iLoop /= 2;
                if (iLoop)
                {
                    //p = p * p
                    Polynom* pTemp = NULL;
                    MultiplyPolyByPoly(pP, pP, &pTemp);
                    pP->killMe();
                    pP = pTemp;
                }
            }

            pP->killMe();
        }
        *_pOut = pOut;
    }
    return 0;
}

int DotPowerSpaseByDouble(Sparse* _pSp, Double* _pDouble, InternalType** _pOut)
{
    if (_pDouble->isEmpty())
    {
        //sp .^ []
        *_pOut = Double::Empty();
        return 0;
    }

    size_t iSize = _pSp->nonZeros();
    int* Col = new int[iSize];
    int* Row = new int[_pSp->getRows()];
    _pSp->getColPos(Col);
    _pSp->getNbItemByRow(Row);
    int* iPositVal = new int[iSize];

    int j = 0;
    for (int i = 0; i < iSize;  j++)
    {
        for (int k = 0; k < Row[j]; k++)
        {
            iPositVal[i] = (Col[i] - 1) * _pSp->getRows() + j;
            i++;
        }
    }

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
        delete[] pDblSp;
        delete[] pDbl;
        throw ast::InternalError(_W("Invalid exponent.\n"));
        return 1;
    }

    Sparse* pSpTemp = new Sparse(_pSp->getRows(), _pSp->getCols(), _pSp->isComplex() || _pDouble->isComplex());
    pSpTemp->zero_set();

    Double* ppDblGet = NULL;
    for (int i = 0; i < iSize; i++)
    {
        if ((pDblSp[i]->get(0) != 0) || (pDblSp[i]->getImg(0) != 0))
        {
            DotPowerDoubleByDouble(pDblSp[i], pDbl[i], &ppDblGet);
            std::complex<double> cplx(ppDblGet->get(0), ppDblGet->getImg(0));
            pSpTemp->set(iPositVal[i], cplx, false);
        }
    }

    delete[] Col;
    delete[] Row;
    delete[] iPositVal;
    for (int i = 0; i < iSize; i++)
    {
        delete pDbl[i];
        delete pDblSp[i];
    }
    delete[] pDbl;
    delete[] pDblSp;

    pSpTemp->finalize();
    *_pOut = pSpTemp;
    return 0;

}


int DotPowerPolyByDouble(Polynom* _pPoly, Double* _pDouble, InternalType** _pOut)
{
    if (_pDouble->isEmpty())
    {
        //p .^ []
        *_pOut = Double::Empty();
        return 0;
    }

    int iSize = _pPoly->getSize();
    if (_pPoly->isScalar())
    {
        return PowerPolyByDouble(_pPoly, _pDouble, _pOut);
    }

    Double** pDblPower = new Double*[iSize];
    double* pdblPower = _pDouble->get();
    if (_pDouble->isScalar())
    {
        if (pdblPower[0] < 0)
        {
            //call overload
            _pOut = NULL;
            delete[] pDblPower;
            return 0;
        }

        for (int i = 0; i < iSize; i++)
        {
            pDblPower[i] = new Double(pdblPower[0]);
        }
    }
    else if (_pDouble->getSize() == iSize)
    {
        for (int i = 0; i < iSize; i++)
        {
            if (pdblPower[i] < 0)
            {
                //call overload
                _pOut = NULL;
                delete[] pDblPower;
                return 0;
            }

            pDblPower[i] = new Double(pdblPower[i]);
        }
    }
    else
    {
        delete[] pDblPower;
        throw ast::InternalError(_W("Invalid exponent.\n"));
    }

    InternalType* pITTempOut    = NULL;
    Polynom* pPolyTemp          = new Polynom(_pPoly->getVariableName(), 1, 1);
    Polynom* pPolyOut           = new Polynom(_pPoly->getVariableName(), _pPoly->getDims(), _pPoly->getDimsArray());
    SinglePoly** pSPOut         = pPolyOut->get();
    SinglePoly** pSPTemp        = pPolyTemp->get();
    SinglePoly** pSP            = _pPoly->get();

    int iResult = 0;
    for (int i = 0; i < iSize; i++)
    {
        // set singlePoly of _pPoly in pPolyTemp without copy
        pSPTemp[0] = pSP[i];
        iResult = PowerPolyByDouble(pPolyTemp, pDblPower[i], &pITTempOut);
        if (iResult)
        {
            break;
        }

        // get singlePoly of pITTempOut and set it in pPolyOut without copy
        SinglePoly** pSPTempOut = pITTempOut->getAs<Polynom>()->get();
        pSPOut[i] = pSPTempOut[0];
        // increase ref to avoid the delete of pSPTempOut[0]
        // which are setted in pSPOut without copy.
        pSPOut[i]->IncreaseRef();
        delete pITTempOut;
        pSPOut[i]->DecreaseRef();
    }

    //delete exp
    for (int i = 0; i < iSize; i++)
    {
        delete pDblPower[i];
    }

    delete[] pDblPower;

    // delete temporary polynom
    // do not delete the last SinglePoly of _pPoly setted without copy in pPolyTemp
    pSPTemp[0]->IncreaseRef();
    delete pPolyTemp;
    pSP[iSize - 1]->DecreaseRef();

    switch (iResult)
    {
        case 1 :
        {
            delete pPolyOut;
            throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
        }
        case 2 :
        {
            delete pPolyOut;
            throw ast::InternalError(_W("Invalid exponent.\n"));
        }
        default:
            //OK
            break;
    }

    *_pOut = pPolyOut;
    return 0;
}

int DotPowerDoubleByDouble(Double* _pDouble1, Double* _pDouble2, Double** _pDoubleOut)
{
    int iResultComplex = 0;

    if (_pDouble1->isEmpty() || _pDouble2->isEmpty())
    {
        *_pDoubleOut = Double::Empty();
    }
    else if (_pDouble1->isIdentity())
    {
        return 1;
    }
    else if (_pDouble2->isIdentity())
    {
        *_pDoubleOut = dynamic_cast<Double*>(_pDouble1->clone());
    }
    else if (_pDouble1->isScalar())
    {
        //a .^ (b or B)
        *_pDoubleOut = new Double(_pDouble2->getDims() , _pDouble2->getDimsArray(), true);

        if (_pDouble1->isComplex())
        {
            double dblR1 = _pDouble1->get(0);
            double dblI1 = _pDouble1->getImg(0);

            if (_pDouble2->isComplex())
            {
                iResultComplex = 1;
                for (int i = 0 ; i < _pDouble2->getSize() ; i++)
                {
                    iPowerComplexScalarByComplexScalar(
                        dblR1, dblI1,
                        _pDouble2->get(i), _pDouble2->getImg(i),
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }
            else
            {
                iResultComplex = 1;
                for (int i = 0 ; i < _pDouble2->getSize() ; i++)
                {
                    iPowerComplexScalarByRealScalar(
                        dblR1, dblI1,
                        _pDouble2->get(i),
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }
        }
        else
        {
            double dblR1 = _pDouble1->get(0);
            if (_pDouble2->isComplex())
            {
                iResultComplex = 1;
                for (int i = 0 ; i < _pDouble2->getSize() ; i++)
                {
                    iPowerRealScalarByComplexScalar(
                        dblR1,
                        _pDouble2->get(i), _pDouble2->getImg(i),
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }
            else
            {
                for (int i = 0 ; i < _pDouble2->getSize() ; i++)
                {
                    int iComplex = 1;
                    iPowerRealScalarByRealScalar(
                        dblR1,
                        _pDouble2->get(i),
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i], &iComplex);
                    iResultComplex |= iComplex;
                }
            }
        }
    }
    else if (_pDouble2->isScalar())
    {
        //A .^ b
        *_pDoubleOut = new Double(_pDouble1->getDims() , _pDouble1->getDimsArray(), true);
        if (_pDouble1->isComplex())
        {
            double dblR2 = _pDouble2->get(0);
            if (_pDouble2->isComplex())
            {
                double dblI2 = _pDouble2->getImg(0);
                iResultComplex = 1;
                for (int i = 0 ; i < _pDouble1->getSize() ; i++)
                {
                    iPowerComplexScalarByComplexScalar(
                        _pDouble1->get(i), _pDouble1->getImg(i),
                        dblR2, dblI2,
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }
            else
            {
                double dblR2 = _pDouble2->get(0);
                iResultComplex = 1;
                for (int i = 0 ; i < _pDouble1->getSize() ; i++)
                {
                    iPowerComplexScalarByRealScalar(
                        _pDouble1->get(i), _pDouble1->getImg(i),
                        dblR2,
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }
        }
        else
        {
            double dblR2 = _pDouble2->get(0);
            if (_pDouble2->isComplex())
            {
                double dblI2 = _pDouble2->getImg(0);
                iResultComplex = 1;
                for (int i = 0 ; i < _pDouble1->getSize() ; i++)
                {
                    iPowerRealScalarByComplexScalar(
                        _pDouble1->get(i),
                        dblR2, dblI2,
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }
            else
            {
                for (int i = 0 ; i < _pDouble1->getSize() ; i++)
                {
                    int iComplex = 1;
                    iPowerRealScalarByRealScalar(
                        _pDouble1->get(i),
                        dblR2,
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i], &iComplex);
                    iResultComplex |= iComplex;
                }
            }
        }
    }
    else
    {
        //A .^ B
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

        (*_pDoubleOut) = new Double(iDims2, piDims2, true);

        if (_pDouble1->isComplex())
        {
            if (_pDouble2->isComplex())
            {
                iResultComplex = 1;
                for (int i = 0 ; i < _pDouble1->getSize() ; i++)
                {
                    iPowerComplexScalarByComplexScalar(
                        _pDouble1->get(i), _pDouble1->getImg(i),
                        _pDouble2->get(i), _pDouble2->getImg(i),
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }
            else
            {
                iResultComplex = 1;
                for (int i = 0 ; i < _pDouble1->getSize() ; i++)
                {
                    iPowerComplexScalarByRealScalar(
                        _pDouble1->get(i), _pDouble1->getImg(i),
                        _pDouble2->get(i),
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }
        }
        else
        {
            if (_pDouble2->isComplex())
            {
                iResultComplex = 1;
                for (int i = 0 ; i < _pDouble1->getSize() ; i++)
                {
                    iPowerRealScalarByComplexScalar(
                        _pDouble1->get(i),
                        _pDouble2->get(i), _pDouble2->getImg(i),
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i]);
                }
            }
            else
            {
                for (int i = 0 ; i < _pDouble1->getSize() ; i++)
                {
                    int iComplex = 1;
                    iPowerRealScalarByRealScalar(
                        _pDouble1->get(i),
                        _pDouble2->get(i),
                        &(*_pDoubleOut)->get()[i], &(*_pDoubleOut)->getImg()[i], &iComplex);
                    iResultComplex |= iComplex;
                }
            }
        }
    }

    if (iResultComplex == 0)
    {
        (*_pDoubleOut)->setComplex(false);
    }
    return 0;
}

