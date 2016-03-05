/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
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

#include "types_ldivide.hxx"
#include "types_divide.hxx"
#include "types_finite.hxx"

extern "C"
{
#include "matrix_left_division.h"
#include "sciprint.h"
#include "localization.h"
#include "charEncoding.h"
}

using namespace types;

InternalType *GenericLDivide(InternalType *_pLeftOperand, InternalType *_pRightOperand)
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
    ** DOUBLE \ DOUBLE
    */
    if (TypeL == GenericType::ScilabDouble && TypeR == GenericType::ScilabDouble)
    {
        Double *pL  = _pLeftOperand->getAs<Double>();
        Double *pR  = _pRightOperand->getAs<Double>();

        iResult = LDivideDoubleByDouble(pL, pR, (Double**)&pResult);
    }

    /*
    ** DOUBLE \ SPARSE
    */
    if (TypeL == GenericType::ScilabDouble && TypeR == GenericType::ScilabSparse)
    {
        Double *pL = _pLeftOperand->getAs<Double>();
        Sparse *pR = _pRightOperand->getAs<Sparse>();

        iResult = RDivideSparseByDouble(pR, pL, &pResult);
    }

    //manage errors
    if (iResult)
    {
        switch (iResult)
        {
            case 1 :
                throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
            case 2 :
                throw ast::InternalError(_W("With NaN or Inf a left division by scalar expected.\n"));
            case 3 :
                throw ast::InternalError(_W("Left division by zero...\n"));
            case 4 :
                sciprint(_("Warning : Left division by zero...\n"));
                break;
            default :
                sciprint(_("Operator \\ : Error %d not yet managed.\n"), iResult);
        }
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return pResult;
}

int LDivideDoubleByDouble(Double *_pDouble1, Double *_pDouble2, Double **_pDoubleOut)
{
    types::Double* pDblTmp = NULL;
    int iErr = 0;

    //check finite values of _pDouble1 and _pDouble2
    if (isDoubleFinite(_pDouble1) == false || isDoubleFinite(_pDouble2) == false)
    {
        if (_pDouble1->isScalar() == false)
        {
            return 2;
        }
    }

    if (_pDouble1->isScalar())
    {
        //x \ Y => Y / x
        return RDivideDoubleByDouble(_pDouble2, _pDouble1, _pDoubleOut);
    }

    if (_pDouble2->isScalar())
    {
        //X \ y => X \ (eye() * y)
        pDblTmp = new types::Double(_pDouble1->getRows(), _pDouble1->getRows(), _pDouble2->isComplex());
        double dblScalarReal = _pDouble2->get(0);
        double* pdblReal = pDblTmp->get();
        int iSize = pDblTmp->getSize();
        int iRowsP1 = pDblTmp->getRows() + 1;

        memset(pdblReal, 0x00, iSize * sizeof(double));
        if (_pDouble2->isComplex())
        {
            double dblScalarImag = _pDouble2->getImg(0);
            double* pdblImag = pDblTmp->getImg();
            memset(pdblImag, 0x00, iSize * sizeof(double));
            for (int i = 0; i < iSize; i += iRowsP1)
            {
                pdblReal[i] = dblScalarReal;
                pdblImag[i] = dblScalarImag;
            }
        }
        else
        {
            for (int i = 0; i < iSize; i += iRowsP1)
            {
                pdblReal[i] = dblScalarReal;
            }
        }

        _pDouble2 = pDblTmp;
    }

    if (_pDouble1->getDims() > 2 || _pDouble2->getDims() > 2 || _pDouble1->getRows() != _pDouble2->getRows())
    {
        //not managed
        return 0;
    }

    *_pDoubleOut = new Double(_pDouble1->getCols(), _pDouble2->getCols(), _pDouble1->isComplex() || _pDouble2->isComplex());
    if ((*_pDoubleOut)->isComplex())
    {
        double dblRcond = 0;
        iErr = iLeftDivisionOfComplexMatrix(
                   _pDouble1->getReal(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
                   _pDouble2->getReal(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
                   (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), (*_pDoubleOut)->getRows(), (*_pDoubleOut)->getCols(), &dblRcond);
    }
    else
    {
        double dblRcond = 0;
        iErr = iLeftDivisionOfRealMatrix(
                   _pDouble1->getReal(), _pDouble1->getRows(), _pDouble1->getCols(),
                   _pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getCols(),
                   (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getRows(), (*_pDoubleOut)->getCols(), &dblRcond);
    }

    if (pDblTmp)
    {
        delete pDblTmp;
    }

    return iErr;
}

