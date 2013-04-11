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

#include "types_addition.hxx"
#include "core_math.h"

#include "scilabexception.hxx"

extern "C"
{
#include <stdio.h>
#include "matrix_addition.h"
#include "localization.h"
#include "charEncoding.h"
#include "os_swprintf.h"
#include "elem_common.h" //dset
}

using namespace types;

InternalType *GenericPlus(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;

    if (_pLeftOperand->isDouble() && _pLeftOperand->getAs<Double>()->isEmpty())
    {
        return _pRightOperand->clone();
    }

    if (_pRightOperand->isDouble() && _pRightOperand->getAs<Double>()->isEmpty())
    {
        return _pLeftOperand->clone();
    }

    /*
    ** DOUBLE + DOUBLE
    */
    if (_pLeftOperand->isDouble() && _pRightOperand->isDouble())
    {
        Double *pL = (Double*)_pLeftOperand;
        Double *pR = (Double*)_pRightOperand;

        int iResult = AddDoubleToDouble(pL, pR, (Double**)&pResult);
        if (iResult != 0)
        {
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"), L"+", pL->DimToString().c_str(), pR->DimToString().c_str());
            throw ast::ScilabError(pMsg);
        }
        return pResult;
    }

    // FIXME: Overload or dedicated function.
    //    else if(TypeL == GenericType::RealBool && TypeR == GenericType::RealBool)
    //    {
    //        //nothing to do, all in macro : %b_+_b
    //    }

    /*
    ** STRING + STRING
    */
    else if (_pLeftOperand->isString() && _pRightOperand->isString())
    {
        String *pL = _pLeftOperand->getAs<String>();
        String *pR = _pRightOperand->getAs<String>();

        int iResult = AddStringToString(pL, pR, (String**)&pResult);

        if (iResult != 0)
        {
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"), L"+", pL->DimToString().c_str(), pR->DimToString().c_str());
            throw ast::ScilabError(pMsg);
        }
        return pResult;
    }

    // FIXME: Overload or dedicated function.
    //    else if(TypeL == GenericType::RealInt && TypeR == GenericType::RealInt)
    //    {
    //    }

    /*
    ** DOUBLE + POLY
    */
    else if (_pLeftOperand->isDouble() && _pRightOperand->isPoly())
    {
        Double *pL  = _pLeftOperand->getAs<Double>();
        Polynom *pR = _pRightOperand->getAs<Polynom>();

        int iResult = AddDoubleToPoly(pR, pL, (Polynom**)&pResult);
        if (iResult != 0)
        {
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"),  L"+", pL->DimToString().c_str(), pR->DimToString().c_str());
            throw ast::ScilabError(pMsg);
        }
        return pResult;
    }

    /*
    ** POLY + DOUBLE
    */
    else if (_pLeftOperand->isPoly() && _pRightOperand->isDouble())
    {
        Double *pR  = _pRightOperand->getAs<Double>();
        Polynom *pL = _pLeftOperand->getAs<Polynom>();

        int iResult = AddDoubleToPoly(pL, pR, (Polynom**)&pResult);
        if (iResult != 0)
        {
            wchar_t pMsg[bsiz];
            os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"),  L"+", pL->DimToString().c_str(), pR->DimToString().c_str());
            throw ast::ScilabError(pMsg);
        }
        return pResult;
    }

    /*
    ** POLY + POLY
    */
    else if (_pLeftOperand->isPoly() && _pRightOperand->isPoly())
    {
        Polynom *pL = _pLeftOperand->getAs<Polynom>();
        Polynom *pR = _pRightOperand->getAs<Polynom>();

        int iResult = AddPolyToPoly(pL, pR, (Polynom**)&pResult);
        if (iResult != 0)
        {
            if (iResult == 1)
            {
                wchar_t pMsg[bsiz];
                os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"),  L"+", pL->DimToString().c_str(), pR->DimToString().c_str());
                throw ast::ScilabError(pMsg);
            }
            else if (iResult == 2)
            {
                std::wostringstream os;
                os << _W("variables don't have the same formal variable");
                //os << ((Location)e.right_get().location_get()).location_getString() << std::endl;
                throw ast::ScilabError(os.str());
            }
        }

        return pResult;
    }

    /*
    ** SPARSE + SPARSE
    */
    else if (_pLeftOperand->isSparse() && _pRightOperand->isSparse())
    {
        Sparse *pL = _pLeftOperand->getAs<Sparse>();
        Sparse *pR = _pRightOperand->getAs<Sparse>();

        int iResult = AddSparseToSparse(pL, pR, (Sparse**)&pResult);
        if (iResult != 0)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** SPARSE + DOUBLE
    */
    else if (_pLeftOperand->isSparse() && _pRightOperand->isDouble())
    {
        Sparse *pL = _pLeftOperand->getAs<Sparse>();
        Double *pR = _pRightOperand->getAs<Double>();

        int iResult = AddDoubleToSparse(pR, pL, (GenericType**)&pResult);
        if (iResult != 0)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** DOUBLE + SPARSE
    */
    else if (_pLeftOperand->isDouble() && _pRightOperand->isSparse())
    {
        Double *pL = _pLeftOperand->getAs<Double>();
        Sparse *pR = _pRightOperand->getAs<Sparse>();

        int iResult = AddSparseToDouble(pR, pL, (GenericType**)&pResult);
        if (iResult != 0)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** DOUBLE + STRING
    */
    else if (_pLeftOperand->isDouble() && _pRightOperand->isString())
    {
        if (_pLeftOperand->getAs<Double>()->getSize() == 0)
        {
            //[] + "" -> ""
            return _pRightOperand->clone();
        }
        else
        {
            // Don't know how to manage this Addition : Return NULL will Call Overloading.
            return NULL;
        }
    }

    /*
    ** STRING + DOUBLE
    */
    else if (_pLeftOperand->isString() && _pRightOperand->isDouble())
    {
        if (_pRightOperand->getAs<Double>()->getSize() == 0)
        {
            //"text" + [] -> ""
            return _pLeftOperand->clone();
        }
        else
        {
            // Don't know how to manage this Addition :  Return NULL will Call Overloading.
            return NULL;
        }
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return NULL;
}


int AddDoubleToDouble(Double *_pDouble1, Double *_pDouble2, Double** _pDoubleOut)
{
    if (_pDouble1->isIdentity())
    {
        int iDims2 = _pDouble2->getDims();
        if (iDims2 > 2)
        {
            //unable to add identity matrix and greater than 2 dimensions matrix
            return 1;
        }

        bool bComplex1 = _pDouble1->isComplex();
        bool bComplex2 = _pDouble2->isComplex();

        (*_pDoubleOut) = new Double(_pDouble2->getRows(), _pDouble2->getCols(), bComplex1 || bComplex2);

        if (bComplex1 == false && bComplex2 == false)
        {
            iAddRealIdentityToRealMatrix(
                _pDouble1->getReal()[0],
                _pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->getReal());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            iAddRealIdentityToComplexMatrix(
                _pDouble1->getReal()[0],
                _pDouble2->getReal(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            iAddComplexIdentityToRealMatrix(
                _pDouble1->getReal()[0], _pDouble1->getImg()[0],
                _pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            iAddComplexIdentityToComplexMatrix(
                _pDouble1->getReal()[0], _pDouble1->getImg()[0],
                _pDouble2->getReal(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
        }

        return 0;
    }

    if (_pDouble2->isIdentity())
    {
        int iDims1 = _pDouble1->getDims();
        if (iDims1 > 2)
        {
            //unable to multiply identity matrix and greater than 2 dimensions matrix
            return 1;
        }

        bool bComplex1 = _pDouble1->isComplex();
        bool bComplex2 = _pDouble2->isComplex();

        (*_pDoubleOut) = new Double(_pDouble1->getRows(), _pDouble1->getCols(), bComplex1 || bComplex2);
        if (bComplex1 == false && bComplex2 == false)
        {
            iAddRealIdentityToRealMatrix(
                _pDouble2->getReal()[0],
                _pDouble1->getReal(), _pDouble1->getRows(), _pDouble1->getCols(),
                (*_pDoubleOut)->getReal());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            iAddComplexIdentityToRealMatrix(
                _pDouble2->getReal()[0], _pDouble2->getImg()[0],
                _pDouble1->getReal(), _pDouble1->getRows(), _pDouble1->getCols(),
                (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            iAddRealIdentityToComplexMatrix(
                _pDouble2->getReal()[0],
                _pDouble1->getReal(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
                (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            iAddComplexIdentityToComplexMatrix(
                _pDouble2->getReal()[0], _pDouble2->getImg()[0],
                _pDouble1->getReal(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
                (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
        }
        return 0;
    }

    bool bScalar1 = _pDouble1->isScalar();
    bool bScalar2 = _pDouble2->isScalar();

    if (bScalar1 && bScalar2)
    {
        bool bComplex1 = _pDouble1->isComplex();
        bool bComplex2 = _pDouble2->isComplex();

        if (bComplex1 == false && bComplex2 == false)
        {
            (*_pDoubleOut) = new Double(_pDouble1->get(0) + _pDouble2->get(0));
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            (*_pDoubleOut) = new Double(_pDouble1->get(0) + _pDouble2->get(0), _pDouble2->getImg(0));
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            (*_pDoubleOut) = new Double(_pDouble1->get(0) + _pDouble2->get(0), _pDouble1->getImg(0));
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            (*_pDoubleOut) = new Double(_pDouble1->get(0) + _pDouble2->get(0), _pDouble1->getImg(0) + _pDouble2->getImg(0));
        }

        return 0;
    }

    if (bScalar1)
    {
        //add pL with each element of pR
        bool bComplex1  = _pDouble1->isComplex();
        bool bComplex2  = _pDouble2->isComplex();
        int iDims2      = _pDouble2->getDims();
        int* piDims2    = _pDouble2->getDimsArray();

        (*_pDoubleOut) = new Double(iDims2, piDims2, bComplex1 || bComplex2);
        if (bComplex1 == false && bComplex2 == false)
        {
            iAddRealScalarToRealMatrix(
                _pDouble1->getReal()[0],
                _pDouble2->getReal(), piDims2, iDims2,
                (*_pDoubleOut)->getReal());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            iAddRealScalarToComplexMatrix(
                _pDouble1->getReal()[0],
                _pDouble2->getReal(), _pDouble2->getImg(), piDims2, iDims2,
                (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            iAddComplexScalarToRealMatrix(
                _pDouble1->getReal()[0], _pDouble1->getImg()[0],
                _pDouble2->getReal(), piDims2, iDims2,
                (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            iAddComplexScalarToComplexMatrix(
                _pDouble1->getReal()[0], _pDouble1->getImg()[0],
                _pDouble2->getReal(), _pDouble2->getImg(), piDims2, iDims2,
                (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
        }

        return 0;
    }

    if (bScalar2)
    {
        //add pL with each element of pR
        bool bComplex1  = _pDouble1->isComplex();
        bool bComplex2  = _pDouble2->isComplex();
        int iDims1      = _pDouble1->getDims();
        int* piDims1    = _pDouble1->getDimsArray();

        (*_pDoubleOut) = new Double(iDims1, piDims1, bComplex1 || bComplex2);
        if (bComplex1 == false && bComplex2 == false)
        {
            iAddRealScalarToRealMatrix(
                _pDouble2->getReal()[0],
                _pDouble1->getReal(), piDims1, iDims1,
                (*_pDoubleOut)->getReal());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            iAddComplexScalarToRealMatrix(
                _pDouble2->getReal()[0], _pDouble2->getImg()[0],
                _pDouble1->getReal(), piDims1, iDims1,
                (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            iAddRealScalarToComplexMatrix(
                _pDouble2->getReal()[0],
                _pDouble1->getReal(), _pDouble1->getImg(), piDims1, iDims1,
                (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            iAddComplexScalarToComplexMatrix(
                _pDouble2->getReal()[0], _pDouble2->getImg()[0],
                _pDouble1->getReal(), _pDouble1->getImg(), piDims1, iDims1,
                (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
        }

        return 0;
    }

    int iDims1 = _pDouble1->getDims();
    int iDims2 = _pDouble2->getDims();

    //check dimension compatibilities ( same number of dimension and same size for each dimension
    if (iDims1 != iDims2)
    {
        return 1;
    }

    int* piDims1 = _pDouble1->getDimsArray();
    int* piDims2 = _pDouble2->getDimsArray();

    for (int i = 0 ; i < iDims1 ; i++)
    {
        if (piDims1[i] != piDims2[i])
        {
            return 1;
        }
    }

    bool bComplex1 = _pDouble1->isComplex();
    bool bComplex2 = _pDouble2->isComplex();

    (*_pDoubleOut) = new Double(iDims2, piDims2, bComplex1 || bComplex2);
    if (bComplex1 == false && bComplex2 == false)
    {
        iAddRealMatrixToRealMatrix(
            _pDouble1->getReal(),
            _pDouble2->getReal(), piDims2, iDims2,
            (*_pDoubleOut)->getReal());
    }
    else if (bComplex1 == false && bComplex2 == true)
    {
        iAddRealMatrixToComplexMatrix(
            _pDouble1->getReal(),
            _pDouble2->getReal(), _pDouble2->getImg(), piDims2, iDims2,
            (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
    }
    else if (bComplex1 == true && bComplex2 == false)
    {
        iAddRealMatrixToComplexMatrix(
            _pDouble2->getReal(),
            _pDouble1->getReal(), _pDouble1->getImg(), piDims1, iDims1,
            (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
    }
    else if (bComplex1 == true && bComplex2 == true)
    {
        iAddComplexMatrixToComplexMatrix(
            _pDouble1->getReal(), _pDouble1->getImg(),
            _pDouble2->getReal(), _pDouble2->getImg(), piDims2, iDims2,
            (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
    }

    return 0;
}

int AddDoubleToPoly(Polynom *_pPoly, Double *_pDouble, Polynom ** _pPolyOut)
{
    bool bComplex1 = _pPoly->isComplex();
    bool bComplex2 = _pDouble->isComplex();

    double *pInDblR = _pDouble->getReal();
    double *pInDblI = _pDouble->getImg();

    if (_pPoly->isScalar())
    {
        int *piRank = new int[_pDouble->getSize()];
        for (int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            piRank[i] = _pPoly->get(0)->getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pDouble->getDims(), _pDouble->getDimsArray(), piRank);
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
        {
            SinglePoly *pInPoly  = _pPoly->get(0);
            SinglePoly *pOutPoly = (*_pPolyOut)->get(i);
            double *pInPolyR     = pInPoly->getCoef()->getReal();
            double *pOutPolyR    = pOutPoly->getCoef()->getReal();

            pOutPolyR[0] = pInDblR[i] + pInPolyR[0];

            for (int j = 1 ; j < pInPoly->getRank() ; j++)
            {
                pOutPolyR[j] = pInPolyR[j];
            }
        }

        if ((*_pPolyOut)->isComplex())
        {
            for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
            {
                SinglePoly *pInPoly  = _pPoly->get(0);
                SinglePoly *pOutPoly = (*_pPolyOut)->get(i);
                double *pInPolyI     = pInPoly->getCoef()->getImg();
                double *pOutPolyI    = pOutPoly->getCoef()->getImg();

                pOutPolyI[0] = (pInDblI != NULL ? pInDblI[i] : 0) + (pInPolyI != NULL ? pInPolyI[0] : 0);

                for (int j = 1 ; j < pInPoly->getRank() ; j++)
                {
                    pOutPolyI[j] = (pInPolyI != NULL ? pInPolyI[j] : 0);
                }
            }
        }

        return 0;
    }

    if (_pDouble->isScalar())
    {
        (*_pPolyOut) = _pPoly->clone()->getAs<Polynom>();

        if (bComplex1 && bComplex2)
        {
            for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
            {
                SinglePoly *pSPOut   = (*_pPolyOut)->get(i);
                double *pOutPolyR    = pSPOut->getCoef()->getReal();
                double *pOutPolyI    = pSPOut->getCoef()->getImg();

                pOutPolyR[0] += pInDblR[0];
                pOutPolyI[0] += pInDblI[0];
            }
        }
        else if (bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
            for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
            {
                SinglePoly *pSPOut   = (*_pPolyOut)->get(i);
                double *pOutPolyR    = pSPOut->getCoef()->getReal();
                double *pOutPolyI    = pSPOut->getCoef()->getImg();

                pOutPolyR[0] += pInDblR[0];
                pOutPolyI[0] = pInDblI[0];
            }
        }
        else
        {
            for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
            {
                SinglePoly *pSPOut = (*_pPolyOut)->get(i);
                double *pOutPolyR  = pSPOut->getCoef()->getReal();

                pOutPolyR[0] += pInDblR[0];
            }
        }

        return 0;
    }

    int iDims1 = _pPoly->getDims();
    int iDims2 = _pDouble->getDims();

    if (iDims1 != iDims2)
    {
        return 1;
    }

    int* piDims1 = _pPoly->getDimsArray();
    int* piDims2 = _pDouble->getDimsArray();

    for (int i = 0 ; i < iDims1 ; i++)
    {
        if (piDims1[i] != piDims2[i])
        {
            return 1;
        }
    }

    (*_pPolyOut) = _pPoly->clone()->getAs<Polynom>();
    if (bComplex1 && bComplex2)
    {
        for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
        {
            SinglePoly *pSPOut   = (*_pPolyOut)->get(i);
            double *pOutPolyR    = pSPOut->getCoef()->getReal();
            double *pOutPolyI    = pSPOut->getCoef()->getImg();

            pOutPolyR[0] += pInDblR[i];
            pOutPolyI[0] += pInDblI[i];
        }
    }
    else if (bComplex2)
    {
        (*_pPolyOut)->setComplex(true);
        for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
        {
            SinglePoly *pSPOut   = (*_pPolyOut)->get(i);
            double *pOutPolyR    = pSPOut->getCoef()->getReal();
            double *pOutPolyI    = pSPOut->getCoef()->getImg();

            pOutPolyR[0] += pInDblR[i];
            pOutPolyI[0] = pInDblI[i];
        }
    }
    else
    {
        for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
        {
            SinglePoly *pSPOut = (*_pPolyOut)->get(i);
            double *pOutPolyR  = pSPOut->getCoef()->getReal();

            pOutPolyR[0] += pInDblR[i];
        }
    }

    return 0;
}

int AddPolyToPoly(Polynom* _pPoly1, Polynom* _pPoly2, Polynom ** _pPolyOut)
{
    if (_pPoly1->getVariableName() != _pPoly2->getVariableName())
    {
        return 2;
    }

    if (_pPoly1->isScalar())
    {
        int *pRank = new int[_pPoly2->getSize()];
        int *pRank1 = new int[_pPoly2->getSize()];
        int *pRank2 = new int[_pPoly2->getSize()];
        bool bComplex1 = _pPoly1->isComplex();
        bool bComplex2 = _pPoly2->isComplex();

        memset(pRank1, 0x00, _pPoly2->getSize() * sizeof(int));

        _pPoly1->getRank(pRank1);
        _pPoly2->getRank(pRank2);
        for (int i = 0 ; i < _pPoly2->getSize() ; i++)
        {
            pRank[i] = Max(pRank1[0], pRank2[i]);
        }

        (*_pPolyOut) = new Polynom(_pPoly2->getVariableName(), _pPoly2->getDims(), _pPoly2->getDimsArray(), pRank);
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        //Result P1(0) + P2(i)
        Double *pCoef1 = _pPoly1->get(0)->getCoef();
        double *p1R   = pCoef1->getReal();
        double *p1I   = pCoef1->getImg();
        for (int i = 0 ; i < _pPoly2->getSize() ; i++)
        {
            Double *pCoef2 = _pPoly2->get(i)->getCoef();
            double *p2R   = pCoef2->getReal();
            double *p2I   = pCoef2->getImg();

            Double *pCoefR = (*_pPolyOut)->get(i)->getCoef();
            double *pRR   = pCoefR->getReal();
            double *pRI   = pCoefR->getImg();

            for (int j = 0 ; j < Min(pRank1[0], pRank2[i]) ; j++)
            {
                pRR[j] = p1R[j] + p2R[j];
            }

            double *pTemp = (pRank1[0] > pRank2[i] ? p1R : p2R);
            for (int j = Min(pRank1[0], pRank2[i]) ; j < Max(pRank1[0], pRank2[i]) ; j++)
            {
                pRR[j] = pTemp[j];
            }

            if ((*_pPolyOut)->isComplex())
            {
                for (int j = 0 ; j < Min(pRank1[0], pRank2[i]) ; j++)
                {
                    pRI[j] = (p1I == NULL ? 0 : p1I[j]) + (p2I == NULL ? 0 : p2I[j]);
                }

                double *pTemp = (pRank1[0] > pRank2[i] ? p1I : p2I);
                for (int j = Min(pRank1[0], pRank2[i]) ; j < Max(pRank1[0], pRank2[i]); j++)
                {
                    pRI[j] = pTemp == NULL ? 0 : pTemp[j];
                }
            }
        }

        delete[] pRank;
        delete[] pRank1;
        delete[] pRank2;
        return 0;
    }

    if (_pPoly2->isScalar())
    {
        int *pRank = new int[_pPoly1->getSize()];
        int *pRank1 = new int[_pPoly1->getSize()];
        int *pRank2 = new int[_pPoly1->getSize()];
        bool bComplex1 = _pPoly1->isComplex();
        bool bComplex2 = _pPoly2->isComplex();

        memset(pRank2, 0x00, _pPoly1->getSize() * sizeof(int));

        _pPoly1->getRank(pRank1);
        _pPoly2->getRank(pRank2);
        for (int i = 0 ; i < _pPoly1->getSize() ; i++)
        {
            pRank[i] = Max(pRank1[i], pRank2[0]);
        }

        (*_pPolyOut) = new Polynom(_pPoly1->getVariableName(), _pPoly1->getDims(), _pPoly1->getDimsArray(), pRank);
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        //Result P1(i) + P2(0)
        Double *pCoef2 = _pPoly2->get(0)->getCoef();
        double *p2R   = pCoef2->getReal();
        double *p2I   = pCoef2->getImg();

        for (int i = 0 ; i < _pPoly1->getSize() ; i++)
        {
            Double *pCoef1 = _pPoly1->get(i)->getCoef();
            double *p1R   = pCoef1->getReal();
            double *p1I   = pCoef1->getImg();

            Double *pCoefR = (*_pPolyOut)->get(i)->getCoef();
            double *pRR   = pCoefR->getReal();
            double *pRI   = pCoefR->getImg();

            for (int j = 0 ; j < Min(pRank1[i], pRank2[0]) ; j++)
            {
                pRR[j] = p1R[j] + p2R[j];
            }

            double *pTemp = (pRank1[i] > pRank2[0] ? p1R : p2R);
            for (int j = Min(pRank1[i], pRank2[0]) ; j < Max(pRank1[i], pRank2[0]) ; j++)
            {
                pRR[j] = pTemp[j];
            }

            if ((*_pPolyOut)->isComplex())
            {
                for (int j = 0 ; j < Min(pRank1[i], pRank2[0]) ; j++)
                {
                    pRI[j] = (p1I == NULL ? 0 : p1I[j]) + (p2I == NULL ? 0 : p2I[j]);
                }

                double *pTemp = (pRank1[i] > pRank2[0] ? p1I : p2I);
                for (int j = Min(pRank1[i], pRank2[0]) ; j < Max(pRank1[i], pRank2[0]); j++)
                {
                    pRI[j] = pTemp == NULL ? 0 : pTemp[j];
                }
            }
        }

        delete[] pRank;
        delete[] pRank1;
        delete[] pRank2;
        return 0;
    }

    int iDims1 = _pPoly1->getDims();
    int iDims2 = _pPoly2->getDims();

    if (iDims1 != iDims2)
    {
        return 1;
    }

    int* piDims1 = _pPoly1->getDimsArray();
    int* piDims2 = _pPoly2->getDimsArray();

    for (int i = 0 ; i < iDims1 ; i++)
    {
        if (piDims1[i] != piDims2[i])
        {
            return 1;
        }
    }

    int *pRank = new int[_pPoly1->getSize()];
    int *pRank1 = new int[_pPoly1->getSize()];
    int *pRank2 = new int[_pPoly2->getSize()];
    bool bComplex1 = _pPoly1->isComplex();
    bool bComplex2 = _pPoly2->isComplex();

    _pPoly1->getRank(pRank1);
    _pPoly2->getRank(pRank2);
    for (int i = 0 ; i < _pPoly1->getSize() ; i++)
    {
        pRank[i] = Max(pRank1[i], pRank2[i]);
    }

    (*_pPolyOut) = new Polynom(_pPoly2->getVariableName(), _pPoly1->getDims(), _pPoly1->getDimsArray(), pRank);
    if (_pPoly1->isComplex() || _pPoly2->isComplex())
    {
        (*_pPolyOut)->setComplex(true);
    }

    if (bComplex1 == false && bComplex2 == false)
    {
        for (int i = 0 ; i < _pPoly1->getSize() ; i++)
        {
            iAddRealPolyToRealPoly(
                _pPoly1->get(i)->getCoef()->getReal(), pRank1[i],
                _pPoly2->get(i)->getCoef()->getReal(), pRank2[i],
                (*_pPolyOut)->get(i)->getCoef()->getReal(), pRank[i]);
        }
    }
    else if (bComplex1 == false && bComplex2 == true)
    {
        for (int i = 0 ; i < _pPoly1->getSize() ; i++)
        {
            iAddRealPolyToComplexPoly(
                _pPoly1->get(i)->getCoef()->getReal(), pRank1[i],
                _pPoly2->get(i)->getCoef()->getReal(), _pPoly2->get(i)->getCoef()->getImg(), pRank2[i],
                (*_pPolyOut)->get(i)->getCoef()->getReal(), (*_pPolyOut)->get(i)->getCoef()->getImg(), pRank[i]);
        }
    }
    else if (bComplex1 == true && bComplex2 == false)
    {
        for (int i = 0 ; i < _pPoly1->getSize() ; i++)
        {
            iAddRealPolyToComplexPoly(
                _pPoly2->get(i)->getCoef()->getReal(), pRank2[i],
                _pPoly1->get(i)->getCoef()->getReal(), _pPoly1->get(i)->getCoef()->getImg(), pRank1[i],
                (*_pPolyOut)->get(i)->getCoef()->getReal(), (*_pPolyOut)->get(i)->getCoef()->getImg(), pRank[i]);
        }
    }
    else if (bComplex1 == true && bComplex2 == true)
    {
        for (int i = 0 ; i < _pPoly1->getSize() ; i++)
        {
            iAddComplexPolyToComplexPoly(
                _pPoly1->get(i)->getCoef()->getReal(), _pPoly1->get(i)->getCoef()->getImg(), pRank1[i],
                _pPoly2->get(i)->getCoef()->getReal(), _pPoly2->get(i)->getCoef()->getImg(), pRank2[i],
                (*_pPolyOut)->get(i)->getCoef()->getReal(), (*_pPolyOut)->get(i)->getCoef()->getImg(), pRank[i]);
        }
    }

    delete[] pRank;
    delete[] pRank1;
    delete[] pRank2;

    if ((*_pPolyOut) != NULL)
    {
        (*_pPolyOut)->updateRank();
    }

    //if pResult == NULL -> incompatible dimensions
    return 0;
}

int AddStringToString(String *_pString1, String *_pString2, String **_pStringOut)
{
    if (_pString1->isScalar())
    {
        //concat pL with each element of pR
        (*_pStringOut)  = new String(_pString2->getDims(), _pString2->getDimsArray());

        int iCommonLen = (int)wcslen(_pString1->get(0));

        for (int i = 0 ; i < _pString2->getSize() ; i++)
        {
            int iLen = (int)wcslen(_pString2->get(i));
            wchar_t* psz = new wchar_t[iLen + iCommonLen + 1];
            memset(psz, 0x00, (iLen + iCommonLen + 1) * sizeof(wchar_t));
            memcpy(psz, _pString1->get(0), iCommonLen * sizeof(wchar_t));
            memcpy(psz + iCommonLen, _pString2->get(i), iLen * sizeof(wchar_t));
            (*_pStringOut)->set(i, psz);
            delete[] psz;
        }
        return 0;
    }

    if (_pString2->isScalar())
    {
        //concat each element of pL with pR
        (*_pStringOut)  = new String(_pString1->getDims(), _pString1->getDimsArray());
        int iCommonLen = (int)wcslen(_pString2->get(0));

        for (int i = 0 ; i < _pString1->getSize() ; i++)
        {
            int iLen = (int)wcslen(_pString1->get(i));
            wchar_t* psz = new wchar_t[iLen + iCommonLen + 1];
            memset(psz, 0x00, (iLen + iCommonLen + 1) * sizeof(wchar_t));

            memcpy(psz, _pString1->get(i), iLen * sizeof(wchar_t));
            memcpy(psz + iLen, _pString2->get(0), iCommonLen * sizeof(wchar_t));

            (*_pStringOut)->set(i, psz);
            delete[] psz;
        }

        return 0;
    }

    int iDims1 = _pString1->getDims();
    int iDims2 = _pString2->getDims();

    if (iDims1 != iDims2)
    {
        return 1;
    }

    int* piDims1 = _pString1->getDimsArray();
    int* piDims2 = _pString2->getDimsArray();

    for (int i = 0 ; i < iDims1 ; i++)
    {
        if (piDims1[i] != piDims2[i])
        {
            return 1;
        }
    }

    (*_pStringOut) = new String(_pString1->getDims(), _pString1->getDimsArray());
    for (int i = 0 ; i < _pString1->getSize() ; i++)
    {
        int iLenL = (int)wcslen(_pString1->get(i));
        int iLenR = (int)wcslen(_pString2->get(i));
        wchar_t* psz = new wchar_t[iLenL + iLenR + 1];
        memset(psz, 0x00, (iLenL + iLenR + 1) * sizeof(wchar_t));

        memcpy(psz          , _pString1->get(i), iLenL * sizeof(wchar_t));
        memcpy(psz + iLenL  , _pString2->get(i), iLenR * sizeof(wchar_t));

        (*_pStringOut)->set(i, psz);
        delete[] psz;
    }

    return 0;
}

int AddSparseToSparse(Sparse* sp1, Sparse* sp2, Sparse** pSpRes)
{
    //check scalar hidden in a sparse ;)
    if (sp1->getRows() == 1 && sp1->getCols() == 1)
    {
        //do scalar + sp
        Double* pDbl = NULL;
        if (sp1->isComplex())
        {
            std::complex<double> dbl = sp1->getImg(0, 0);
            pDbl = new Double(dbl.real(), dbl.imag());
        }
        else
        {
            pDbl = new Double(sp1->get(0, 0));
        }

        AddSparseToDouble(sp2, pDbl, (GenericType**)pSpRes);
        delete pDbl;
        return 0;
    }

    if (sp2->getRows() == 1 && sp2->getCols() == 1)
    {
        //do sp + scalar
        Double* pDbl = NULL;
        if (sp2->isComplex())
        {
            std::complex<double> dbl = sp2->getImg(0, 0);
            pDbl = new Double(dbl.real(), dbl.imag());
        }
        else
        {
            pDbl = new Double(sp2->get(0, 0));
        }

        AddSparseToDouble(sp1, pDbl, (GenericType**)pSpRes);
        delete pDbl;
        return 0;
    }

    if (sp1->getRows() != sp2->getRows() || sp1->getCols() != sp2->getCols())
    {
        //dimensions not match
        return 1;
    }

    if (sp1->nonZeros() == 0)
    {
        //sp([]) + sp
        *pSpRes = new Sparse(*sp2);
        return 0;
    }

    if (sp2->nonZeros() == 0)
    {
        //sp + sp([])
        *pSpRes = new Sparse(*sp1);
        return 0;
    }

    //copy sp1 in pSpRes
    *pSpRes = sp1->add(*sp2);
    return 0;
}

int AddSparseToDouble(Sparse* sp, Double* d, GenericType** pDRes)
{
    int iOne = 1; //fortran
    bool bComplex1 = sp->isComplex();
    bool bComplex2 = d->isComplex();

    if (d->isIdentity())
    {
        //convert to sp
        Sparse* pS = new Sparse(sp->getRows(), sp->getCols(), d->isComplex());
        if (pS->isComplex())
        {
            for (int i = 0 ; i < Min(sp->getRows() , sp->getCols()) ; i++)
            {
                pS->set(i, i, std::complex<double>(d->get(0), d->getImg(0)));
            }
        }
        else
        {
            for (int i = 0 ; i < Min(sp->getRows() , sp->getCols()) ; i++)
            {
                pS->set(i, i, d->get(0));
            }
        }

        AddSparseToSparse(sp, pS, (Sparse**)pDRes);
        delete pS;
        return 0;
    }

    if (sp->isScalar() && d->isScalar())
    {
        //sp + d
        Double* pRes = d->clone()->getAs<Double>();
        pRes->setComplex(bComplex1 | bComplex2);
        if (bComplex1)
        {
            std::complex<double> dbl = sp->getImg(0, 0);
            pRes->set(0, pRes->get(0) + dbl.real());
            pRes->setImg(0, pRes->getImg(0) + dbl.imag());
        }
        else
        {
            pRes->set(0, pRes->get(0) + sp->get(0, 0));
        }

        *pDRes = pRes;
        return 0;
    }

    if (d->isScalar())
    {
        //SP + d
        Double* pRes = new Double(sp->getRows(), sp->getCols(), bComplex1 | bComplex2);
        int iSize = sp->getSize();
        double dblVal = d->get(0);
        C2F(dset)(&iSize, &dblVal, pRes->get(), &iOne);
        if (bComplex2)
        {
            double dblValI = d->getImg(0);
            C2F(dset)(&iSize, &dblValI, pRes->getImg(), &iOne);
        }
        else if (bComplex1)
        {
            //initialize imag part at 0
            double dblValI = 0;
            C2F(dset)(&iSize, &dblValI, pRes->getImg(), &iOne);
        }

        int nonZeros = static_cast<int>(sp->nonZeros());
        double* pRows = new double[nonZeros * 2];
        sp->outputRowCol(pRows);
        double* pCols = pRows + nonZeros;

        double* pNonZeroR = new double[nonZeros];
        double* pNonZeroI = new double[nonZeros];
        sp->outputValues(pNonZeroR, pNonZeroI);

        if (bComplex1)
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                std::complex<double> dbl = sp->getImg(iRow, iCol);
                pRes->set(iRow, iCol, pRes->get(iRow, iCol) + dbl.real());
                pRes->setImg(iRow, iCol, pRes->getImg(iRow, iCol) + dbl.imag());
            }
        }
        else
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                pRes->set(iRow, iCol, pRes->get(iRow, iCol) + sp->get(iRow, iCol));
            }
        }
        *pDRes = pRes;

        //clear
        delete[] pRows;
        delete[] pNonZeroR;
        delete[] pNonZeroI;

        return 0;
    }

    if (sp->isScalar())
    {
        //sp + D
        Double* pRes = d->clone()->getAs<Double>();
        pRes->setComplex(bComplex1 | bComplex2);

        if (bComplex1)
        {
            double* pReal = pRes->get();
            double* pImg = pRes->getImg();
            for (int i = 0 ; i < pRes->getSize() ; i++)
            {
                std::complex<double> dbl = sp->getImg(0, 0);
                pReal[i] += dbl.real();
                pImg[i] += dbl.imag();
            }
        }
        else
        {
            double* pReal = pRes->get();
            for (int i = 0 ; i < pRes->getSize() ; i++)
            {
                pReal[i] += sp->get(0, 0);
            }
        }

        *pDRes = pRes;
        return 0;
    }


    if (sp->getRows() == d->getRows() && sp->getCols() == d->getCols())
    {
        //SP + D
        Double* pRes = d->clone()->getAs<Double>();
        pRes->setComplex(bComplex1 | bComplex2);

        int nonZeros = static_cast<int>(sp->nonZeros());
        double* pRows = new double[nonZeros * 2];
        sp->outputRowCol(pRows);
        double* pCols = pRows + nonZeros;

        double* pNonZeroR = new double[nonZeros];
        double* pNonZeroI = new double[nonZeros];
        sp->outputValues(pNonZeroR, pNonZeroI);

        if (bComplex1)
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                std::complex<double> dbl = sp->getImg(iRow, iCol);
                pRes->set(iRow, iCol, pRes->get(iRow, iCol) + dbl.real());
                pRes->setImg(iRow, iCol, pRes->getImg(iRow, iCol) + dbl.imag());
            }
        }
        else
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                pRes->set(iRow, iCol, pRes->get(iRow, iCol) + sp->get(iRow, iCol));
            }
        }

        //clear
        delete[] pRows;
        delete[] pNonZeroR;
        delete[] pNonZeroI;
        *pDRes = pRes;
        return 0;
    }
    return 1;
}

int AddDoubleToSparse(Double* d, Sparse* sp, GenericType** pDRes)
{
    /* uses commutativity */
    return AddSparseToDouble(sp, d, pDRes);
}
