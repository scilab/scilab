/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric Delamarre
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

#include "types_kronecker.hxx"

extern "C" {
#include "matrix_kronecker.h"
}

// DOUBLE .*. DOUBLE
types::InternalType *GenericKrontimes(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand)
{
    types::Double *pResult = NULL;
    types::GenericType::ScilabType TypeL = _pLeftOperand->getType();
    types::GenericType::ScilabType TypeR = _pRightOperand->getType();

    if (TypeL == types::GenericType::ScilabDouble && TypeR == types::GenericType::ScilabDouble)
    {
        types::Double *pL = _pLeftOperand->getAs<types::Double>();
        types::Double *pR = _pRightOperand->getAs<types::Double>();

        if (pL->getDims() > 2 || pR->getDims() > 2)
        {
            return NULL;
        }

        int iResult = KroneckerMultiplyDoubleByDouble(pL, pR, &pResult);
        if (iResult)
        {
            throw ast::InternalError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    // Default case : Return NULL will Call Overloading.
    return NULL;
}

int KroneckerMultiplyDoubleByDouble(types::Double* _pDouble1, types::Double* _pDouble2, types::Double** _pDoubleOut)
{
    bool bComplex1 = _pDouble1->isComplex();
    bool bComplex2 = _pDouble2->isComplex();

    int iRowResult = _pDouble1->getRows() * _pDouble2->getRows();
    int iColResult = _pDouble1->getCols() * _pDouble2->getCols();

    //Output variables
    bool bComplexOut = bComplex1 || bComplex2;
    (*_pDoubleOut)   = new types::Double(iRowResult, iColResult, bComplexOut);

    if (bComplex1 && bComplex2) // Complex .*. Complex
    {
        vKronC( _pDouble1->getReal(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getRows(), _pDouble1->getCols(),
                _pDouble2->getReal(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), iRowResult);
    }
    else if (bComplex1) // Complex .*. Real
    {
        vKronR( _pDouble1->getReal(), _pDouble1->getRows(), _pDouble1->getRows(), _pDouble1->getCols(),
                _pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->getReal(), iRowResult);

        vKronR( _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getRows(), _pDouble1->getCols(),
                _pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->getImg(), iRowResult);
    }
    else if (bComplex2) // Real .*. Complex
    {
        vKronR( _pDouble1->getReal(), _pDouble1->getRows(), _pDouble1->getRows(), _pDouble1->getCols(),
                _pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->getReal(), iRowResult);

        vKronR( _pDouble1->getReal(), _pDouble1->getRows(), _pDouble1->getRows(), _pDouble1->getCols(),
                _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->getImg(), iRowResult);
    }
    else // Real .*. Real
    {
        vKronR( _pDouble1->getReal(), _pDouble1->getRows(), _pDouble1->getRows(), _pDouble1->getCols(),
                _pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->getReal(), iRowResult);
    }

    return 0; //No Error;
}

// DOUBLE ./. DOUBLE
types::InternalType *GenericKronrdivide(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand)
{
    // Default case : Return NULL will Call Overloading.
    return NULL;
}


// DOUBLE .\. DOUBLE
types::InternalType *GenericKronldivide(types::InternalType *_pLeftOperand, types::InternalType *_pRightOperand)
{
    // Default case : Return NULL will Call Overloading.
    return NULL;
}
