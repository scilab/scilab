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

#include <stdio.h>

#include "types_substraction.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "int.hxx"
#include "sparse.hxx"
#include "generic_operations.hxx"

extern "C"
{
#include "matrix_substraction.h"
#include "elem_common.h" //dset
}

using namespace types;

//define arrays on operation functions
static subtract_function pSubtractfunction[types::InternalType::IdLast][types::InternalType::IdLast] = {NULL};

void fillSubtractFunction()
{
#define scilab_fill_subtract(id1, id2, func, typeIn1, typeIn2, typeOut) \
    pSubtractfunction[types::InternalType::Id ## id1][types::InternalType::Id ## id2] = (subtract_function)&subtract_##func<typeIn1, typeIn2, typeOut>
}

InternalType* GenericMinus(InternalType* _pLeftOperand, InternalType* _pRightOperand)
{
    InternalType *pResult = NULL;

    // InternalType - [] and [] - []
    if (_pRightOperand->isDouble() && _pRightOperand->getAs<Double>()->isEmpty())
    {
        return _pLeftOperand->clone();
    }

    // [] - InternalType
    if (_pLeftOperand->isDouble() && _pLeftOperand->getAs<Double>()->isEmpty())
    {
        if (_pRightOperand->isSparse())
        {
            InternalType* pSpOut = NULL;
            Sparse* pSp = new Sparse(1, 1);
            pSp->set(0, 0, -1.0);
            pSpOut = GenericTimes((InternalType*)pSp, _pRightOperand);
            delete pSp;
            return pSpOut;
        }

        Double pDblZero = Double(0);
        return GenericMinus((InternalType*)&pDblZero, _pRightOperand);
    }

    /*
    ** DOUBLE - DOUBLE
    */
    if (_pLeftOperand->isDouble() && _pRightOperand->isDouble())
    {
        Double *pL = _pLeftOperand->getAs<Double>();
        Double *pR = _pRightOperand->getAs<Double>();

        int iResult = SubstractDoubleToDouble(pL, pR, (Double**)&pResult);
        if (iResult != 0)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
        return pResult;
    }

    /*
    ** DOUBLE - POLY
    */
    else if (_pLeftOperand->isDouble() && _pRightOperand->isPoly())
    {
        Double *pL              = _pLeftOperand->getAs<Double>();
        Polynom *pR          = _pRightOperand->getAs<types::Polynom>();

        int iResult = SubstractPolyToDouble(pL, pR, (Polynom**)&pResult);
        if (iResult != 0)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
        return pResult;
    }

    /*
    ** POLY - DOUBLE
    */
    else if (_pLeftOperand->isPoly() && _pRightOperand->isDouble())
    {
        Polynom *pL   = _pLeftOperand->getAs<types::Polynom>();
        Double *pR    = _pRightOperand->getAs<Double>();

        int iResult = SubstractDoubleToPoly(pL, pR, (Polynom**)&pResult);
        if (iResult != 0)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
        return pResult;
    }

    /*
    ** POLY - POLY
    */
    else if (_pLeftOperand->isPoly() && _pRightOperand->isPoly())
    {
        Polynom *pL   = _pLeftOperand->getAs<types::Polynom>();
        Polynom *pR   = _pRightOperand->getAs<types::Polynom>();

        int iResult = SubstractPolyToPoly(pL, pR, (Polynom**)&pResult);
        if (iResult != 0)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
        return pResult;
    }

    /*
    ** SPARSE - SPARSE
    */
    else if (_pLeftOperand->isSparse() && _pRightOperand->isSparse())
    {
        Sparse *pL = _pLeftOperand->getAs<Sparse>();
        Sparse *pR = _pRightOperand->getAs<Sparse>();

        int iResult = SubstractSparseToSparse(pR, pL, (GenericType**)&pResult);
        if (iResult != 0)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** SPARSE - DOUBLE
    */
    else if (_pLeftOperand->isSparse() && _pRightOperand->isDouble())
    {
        Sparse *pL = _pLeftOperand->getAs<Sparse>();
        Double *pR = _pRightOperand->getAs<Double>();

        int iResult = SubstractDoubleToSparse(pR, pL, (GenericType**)&pResult);
        if (iResult != 0)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** DOUBLE - SPARSE
    */
    else if (_pLeftOperand->isDouble() && _pRightOperand->isSparse())
    {
        Double *pL = _pLeftOperand->getAs<Double>();
        Sparse *pR = _pRightOperand->getAs<Sparse>();

        int iResult = SubstractSparseToDouble(pR, pL, (GenericType**)&pResult);
        if (iResult != 0)
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

int SubstractDoubleToDouble(Double* _pDouble1, Double* _pDouble2, Double** _pDoubleOut)
{
    bool bComplex1  = _pDouble1->isComplex();
    bool bComplex2  = _pDouble2->isComplex();

    if (_pDouble1->isIdentity())
    {
        if (_pDouble2->getDims() > 2)
        {
            //unable to substract identity matrix and greater than 2 dimensions matrix
            return 1;
        }

        (*_pDoubleOut) = new Double(_pDouble2->getRows(), _pDouble2->getCols(), bComplex1 || bComplex2);

        if (bComplex1 == false && bComplex2 == false)
        {
            iSubstractRealIdentityToRealMatrix(
                _pDouble1->get(0),
                _pDouble2->get(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->get());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            iSubstractRealIdentityToComplexMatrix(
                _pDouble1->get(0),
                _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            iSubstractComplexIdentityToRealMatrix(
                _pDouble1->get(0), _pDouble1->getImg(0),
                _pDouble2->get(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            iSubstractComplexIdentityToComplexMatrix(
                _pDouble1->get(0), _pDouble1->getImg(0),
                _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }

        //return opposite
        double* pDbl = (*_pDoubleOut)->get();
        if ((*_pDoubleOut)->isComplex())
        {
            double* pDblImg = (*_pDoubleOut)->getImg();
            for (int i = 0 ; i < _pDouble2->getSize() ; i++)
            {
                pDbl[i] = -pDbl[i];
                pDblImg[i] = -pDblImg[i];
            }
        }
        else
        {
            for (int i = 0 ; i < _pDouble2->getSize() ; i++)
            {
                pDbl[i] = -pDbl[i];
            }
        }
        return 0;
    }

    if (_pDouble2->isIdentity())
    {
        if (_pDouble1->getDims() > 2)
        {
            //unable to substract identity matrix and greater than 2 dimensions matrix
            return 1;
        }

        (*_pDoubleOut) = new Double(_pDouble1->getRows(), _pDouble1->getCols(), bComplex1 || bComplex2);
        if (bComplex1 == false && bComplex2 == false)
        {
            iSubstractRealIdentityToRealMatrix(
                _pDouble2->get(0),
                _pDouble1->get(), _pDouble1->getRows(), _pDouble1->getCols(),
                (*_pDoubleOut)->get());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            iSubstractComplexIdentityToRealMatrix(
                _pDouble2->get(0), _pDouble2->getImg(0),
                _pDouble1->get(), _pDouble1->getRows(), _pDouble1->getCols(),
                (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            iSubstractRealIdentityToComplexMatrix(
                _pDouble2->get(0),
                _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
                (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            iSubstractComplexIdentityToComplexMatrix(
                _pDouble2->get(0), _pDouble2->getImg(0),
                _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
                (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }

        return 0;
    }

    if (_pDouble1->isScalar())
    {
        //add pL with each element of pR
        (*_pDoubleOut) = new Double(_pDouble2->getDims(), _pDouble2->getDimsArray(), bComplex1 || bComplex2);
        if (bComplex1 == false && bComplex2 == false)
        {
            iSubstractRealMatrixToRealScalar(
                _pDouble2->get(), _pDouble2->getDimsArray(), _pDouble2->getDims(),
                _pDouble1->get(0),
                (*_pDoubleOut)->get());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            iSubstractComplexMatrixToRealScalar(
                _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getDimsArray(), _pDouble2->getDims(),
                _pDouble1->get(0),
                (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            iSubstractRealMatrixToComplexScalar(
                _pDouble2->get(), _pDouble2->getDimsArray(), _pDouble2->getDims(),
                _pDouble1->get(0), _pDouble1->getImg(0),
                (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            iSubstractComplexMatrixToComplexScalar(
                _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getDimsArray(), _pDouble2->getDims(),
                _pDouble1->get(0), _pDouble1->getImg(0),
                (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }

        return 0;
    }

    if (_pDouble2->isScalar())
    {
        //add pL with each element of pR
        (*_pDoubleOut) = new Double(_pDouble1->getDims(), _pDouble1->getDimsArray(), bComplex1 || bComplex2);
        if (bComplex1 == false && bComplex2 == false)
        {
            iSubstractRealScalarToRealMatrix(
                _pDouble2->get(0),
                _pDouble1->get(), _pDouble1->getDimsArray(), _pDouble1->getDims(),
                (*_pDoubleOut)->get());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            iSubstractComplexScalarToRealMatrix(
                _pDouble2->get(0), _pDouble2->getImg(0),
                _pDouble1->get(), _pDouble1->getDimsArray(), _pDouble1->getDims(),
                (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            iSubstractRealScalarToComplexMatrix(
                _pDouble2->get(0),
                _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getDimsArray(), _pDouble1->getDims(),
                (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            iSubstractComplexScalarToComplexMatrix(
                _pDouble2->get(0), _pDouble2->getImg(0),
                _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getDimsArray(), _pDouble1->getDims(),
                (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }

        return 0;
    }


    //add pL and pR element wise

    int iDims1 = _pDouble1->getDims();
    int iDims2 = _pDouble2->getDims();

    if (iDims1 != iDims2)
    {
        return 1;
    }

    int* piDims1    = _pDouble1->getDimsArray();
    int* piDims2    = _pDouble2->getDimsArray();
    for (int i = 0 ; i < _pDouble1->getDims() ; i++)
    {
        if (piDims1[i] != piDims2[i])
        {
            return 1;
        }
    }

    (*_pDoubleOut) = new Double(iDims2, piDims2, bComplex1 || bComplex2);
    if (bComplex1 == false && bComplex2 == false)
    {
        iSubstractRealMatrixToRealMatrix(
            _pDouble2->get(),
            _pDouble1->get(), piDims1, iDims1,
            (*_pDoubleOut)->get());
    }
    else if (bComplex1 == false && bComplex2 == true)
    {
        iSubstractComplexMatrixToRealMatrix(
            _pDouble2->get(), _pDouble2->getImg(),
            _pDouble1->get(), piDims1, iDims1,
            (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
    }
    else if (bComplex1 == true && bComplex2 == false)
    {
        iSubstractRealMatrixToComplexMatrix(
            _pDouble2->get(),
            _pDouble1->get(), _pDouble1->getImg(), piDims1, iDims1,
            (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
    }
    else if (bComplex1 == true && bComplex2 == true)
    {
        iSubstractComplexMatrixToComplexMatrix(
            _pDouble2->get(), _pDouble2->getImg(),
            _pDouble1->get(), _pDouble1->getImg(), piDims1, iDims1,
            (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
    }

    return 0;
}

//1 - %s
int SubstractPolyToDouble(Double *_pDouble, Polynom *_pPoly, Polynom** _pPolyOut)
{
    double *pInDblR = _pDouble->get();
    double *pInDblI = _pDouble->getImg();

    if (_pDouble->isScalar())
    {
        //Clone original poly
        (*_pPolyOut) = _pPoly->clone()->getAs<Polynom>();

        if (_pDouble->isComplex())
        {
            (*_pPolyOut)->setComplex(true);
        }

        for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
        {
            SinglePoly *pOutPoly    = (*_pPolyOut)->get(i);
            double *pOutPolyR       = pOutPoly->get();

            pOutPolyR[0] = pInDblR[0] - pOutPolyR[0];

            for (int j = 1 ; j < pOutPoly->getSize() ; j++)
            {
                pOutPolyR[j] = - pOutPolyR[j];
            }
        }

        if ((*_pPolyOut)->isComplex())
        {
            for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
            {
                SinglePoly *pOutPoly    = (*_pPolyOut)->get(i);
                double *pOutPolyI       = pOutPoly->getImg();

                pOutPolyI[0]            = (pInDblI == NULL ? 0 : pInDblI[0]) - (pOutPolyI == NULL ? 0 : pOutPolyI[0]);

                for (int j = 1 ; j < pOutPoly->getSize() ; j++)
                {
                    pOutPolyI[j] = - pOutPolyI[j];
                }
            }
        }

        return 0;
    }

    if (_pPoly->isScalar())
    {
        //[] - %s
        int *piRank = new int[_pDouble->getSize()];
        for (int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            piRank[i] = _pPoly->get(0)->getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pDouble->getDims(), _pDouble->getDimsArray(), piRank);

        for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
        {
            SinglePoly *pInPoly   = _pPoly->get(0);
            SinglePoly *pOutPoly  = (*_pPolyOut)->get(i);

            double *pInPolyR = pInPoly->get();
            double *pOutPolyR = pOutPoly->get();

            pOutPolyR[0] = pInDblR[i] - pInPolyR[0];

            for (int j = 1 ; j < pOutPoly->getSize() ; j++)
            {
                pOutPolyR[j] = - pInPolyR[j];
            }
        }

        if (_pPoly->isComplex() || _pDouble->isComplex())
        {
            (*_pPolyOut)->setComplex(true);
            for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
            {
                SinglePoly *pInPoly   = _pPoly->get(0);
                SinglePoly *pOutPoly  = (*_pPolyOut)->get(i);

                double *pInPolyI = pInPoly->getImg();
                double *pOutPolyI = pOutPoly->getImg();

                pOutPolyI[0] = (pInDblI != NULL ? pInDblI[i] : 0) - (pInPolyI != NULL ? pInPolyI[0] : 0);

                for (int j = 1 ; j < pOutPoly->getSize() ; j++)
                {
                    pOutPolyI[j] = -pInPolyI[j];
                }
            }
        }

        return 0;
    }

    //check dimension compatibilities
    int iLeftDims = _pDouble->getDims();
    int iRightDims = _pPoly->getDims();

    if (iLeftDims != iRightDims)
    {
        return 1;
    }

    int* piLeftDims = _pDouble->getDimsArray();
    int* piRightDims = _pPoly->getDimsArray();

    for (int i = 0 ; i < iLeftDims ; i++)
    {
        if (piLeftDims[i] != piRightDims[i])
        {
            return 1;
        }
    }


    //Clone original poly
    (*_pPolyOut) = _pPoly->clone()->getAs<Polynom>();

    for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
    {
        SinglePoly *pOutPoly    = (*_pPolyOut)->get(i);
        double *pOutPolyR       = pOutPoly->get();

        pOutPolyR[0] = pInDblR[i] - pOutPolyR[0];

        for (int j = 1 ; j < pOutPoly->getSize() ; j++)
        {
            pOutPolyR[j] = - pOutPolyR[j];
        }
    }

    if (_pPoly->isComplex() || _pDouble->isComplex())
    {
        (*_pPolyOut)->setComplex(true);
        for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
        {
            SinglePoly *pOutPoly  = (*_pPolyOut)->get(i);
            double *pOutPolyI = pOutPoly->getImg();

            pOutPolyI[0] = (pInDblI != NULL ? pInDblI[i] : 0) - (pOutPolyI != NULL ? pOutPolyI[0] : 0);

            for (int j = 1 ; j < pOutPoly->getSize() ; j++)
            {
                pOutPolyI[j] = - pOutPolyI[j];
            }
        }
    }

    return 0;
}

//%s - 1
int SubstractDoubleToPoly(Polynom *_pPoly, Double *_pDouble, Polynom **_pPolyOut)
{
    double *pInDblR   = _pDouble->get();
    double *pInDblI   = _pDouble->getImg();

    if (_pDouble->isScalar())
    {
        //Clone original poly
        (*_pPolyOut) = _pPoly->clone()->getAs<Polynom>();
        for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
        {
            SinglePoly *pOutPoly    = (*_pPolyOut)->get(i);
            double *pOutPolyR       = pOutPoly->get();
            pOutPolyR[0]   -= pInDblR[0];
        }

        if ((*_pPolyOut)->isComplex() || _pDouble->isComplex())
        {
            (*_pPolyOut)->setComplex(true);
            for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
            {
                SinglePoly *pOutPoly    = (*_pPolyOut)->get(i);
                double *pOutPolyI       = pOutPoly->getImg();

                pOutPolyI[0]            -=  (pInDblI == NULL ? 0 : pInDblI[0]);
            }
        }
    }
    else if (_pPoly->isScalar())
    {
        int *piRank = new int[_pDouble->getSize()];
        for (int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            piRank[i] = _pPoly->get(0)->getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pDouble->getDims(), _pDouble->getDimsArray(), piRank);
        (*_pPolyOut)->setComplex(_pPoly->isComplex() || _pDouble->isComplex());

        if (_pPoly->isComplex())
        {
            SinglePoly *pInPoly = _pPoly->get(0);
            double *pInPolyR    = pInPoly->get();
            double *pInPolyI    = pInPoly->getImg();
            for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
            {
                SinglePoly *pOutPoly    = (*_pPolyOut)->get(i);
                double *pOutPolyR       = pOutPoly->get();
                double *pOutPolyI       = pOutPoly->getImg();

                pOutPolyR[0]            = pInPolyR[0] - pInDblR[i];
                pOutPolyI[0]            = pInPolyI[0];
                for (int j = 1 ; j < pOutPoly->getSize() ; j++)
                {
                    pOutPolyR[j]        = pInPolyR[j];
                    pOutPolyI[j]        = pInPolyI[j];
                }
            }
        }
        else if (_pDouble->isComplex())
        {
            SinglePoly *pInPoly = _pPoly->get(0);
            double *pInPolyR    = pInPoly->get();
            for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
            {
                SinglePoly *pOutPoly    = (*_pPolyOut)->get(i);
                double *pOutPolyR       = pOutPoly->get();
                double *pOutPolyI       = pOutPoly->getImg();

                pOutPolyR[0]            = pInPolyR[0] - pInDblR[i];
                pOutPolyI[0]            = - pInDblI[i];
                for (int j = 1 ; j < pOutPoly->getSize() ; j++)
                {
                    pOutPolyR[j]        = pInPolyR[j];
                    pOutPolyI[j]        = 0;
                }
            }
        }
        else
        {
            SinglePoly *pInPoly = _pPoly->get(0);
            double *pInPolyR    = pInPoly->get();
            for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
            {
                SinglePoly *pOutPoly    = (*_pPolyOut)->get(i);
                double *pOutPolyR       = pOutPoly->get();

                pOutPolyR[0]            = pInPolyR[0] - pInDblR[i];
                for (int j = 1 ; j < pOutPoly->getSize() ; j++)
                {
                    pOutPolyR[j]        = pInPolyR[j];
                }
            }
        }

        delete[] piRank;
    }
    else
    {
        //check dimension compatibilities
        int iLeftDims = _pDouble->getDims();
        int iRightDims = _pPoly->getDims();

        if (iLeftDims != iRightDims)
        {
            return 1;
        }

        int* piLeftDims = _pDouble->getDimsArray();
        int* piRightDims = _pPoly->getDimsArray();

        for (int i = 0 ; i < iLeftDims ; i++)
        {
            if (piLeftDims[i] != piRightDims[i])
            {
                return 1;
            }
        }

        //Clone original poly
        (*_pPolyOut) = _pPoly->clone()->getAs<Polynom>();
        for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
        {
            SinglePoly *pOutPoly    = (*_pPolyOut)->get(i);
            double *pOutPolyR       = pOutPoly->get();
            pOutPolyR[0]            -= pInDblR[i];
        }

        if (_pPoly->isComplex() || _pDouble->isComplex())
        {
            (*_pPolyOut)->setComplex(true);
            for (int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
            {
                SinglePoly *pOutPoly    = (*_pPolyOut)->get(i);
                double *pOutPolyI       = pOutPoly->getImg();

                pOutPolyI[0]            -= (pInDblI != NULL ? pInDblI[i] : 0);
            }
        }
    }
    return 0;
}

int SubstractPolyToPoly(Polynom *_pPoly1, Polynom *_pPoly2, Polynom **_pPolyOut)
{
    if (_pPoly1->isScalar())
    {
        SinglePoly* p1Coef  = _pPoly1->get(0);
        int iRank1          = p1Coef->getRank();
        int* pRank2         = new int[_pPoly2->getSize()];
        int* pRankOut       = new int[_pPoly2->getSize()];

        _pPoly2->getRank(pRank2);
        for (int i = 0 ; i < _pPoly2->getSize() ; i++)
        {
            pRankOut[i] = std::max(iRank1, pRank2[i]);
        }

        (*_pPolyOut) = new Polynom(_pPoly2->getVariableName(), _pPoly2->getDims(), _pPoly2->getDimsArray(), pRankOut);
        (*_pPolyOut)->setComplex(_pPoly1->isComplex() || _pPoly2->isComplex());

        //Result P1(0) + P2(i)
        double* p1R = p1Coef->get();
        if ((*_pPolyOut)->isComplex())
        {
            double* p1I = p1Coef->getImg();
            for (int i = 0 ; i < _pPoly2->getSize() ; i++)
            {
                SinglePoly* p2Coef   = _pPoly2->get(i);
                double* p2R          = p2Coef->get();
                double* p2I          = p2Coef->getImg();

                SinglePoly* pOutCoef = (*_pPolyOut)->get(i);
                double* pOutR        = pOutCoef->get();
                double* pOutI        = pOutCoef->getImg();

                for (int j = 0 ; j < pRankOut[i] + 1 ; j++)
                {
                    if (j > iRank1)
                    {
                        pOutR[j] = - p2R[j];
                        pOutI[j] = - (p2I ? p2I[j] : 0);
                    }
                    else if (j > pRank2[i])
                    {
                        pOutR[j] = p1R[j];
                        pOutI[j] = (p1I ? p1I[j] : 0);
                    }
                    else
                    {
                        pOutR[j] = p1R[j] - p2R[j];
                        pOutI[j] = (p1I ? p1I[j] : 0) - (p2I ? p2I[j] : 0);
                    }
                }
            }
        }
        else
        {
            for (int i = 0 ; i < _pPoly2->getSize() ; i++)
            {
                SinglePoly* p2Coef   = _pPoly2->get(i);
                double* p2R          = p2Coef->get();

                SinglePoly* pOutCoef = (*_pPolyOut)->get(i);
                double* pOutR        = pOutCoef->get();

                for (int j = 0 ; j < pRankOut[i] + 1 ; j++)
                {
                    if (j > iRank1)
                    {
                        pOutR[j] = - p2R[j];
                    }
                    else if (j > pRank2[i])
                    {
                        pOutR[j] = p1R[j];
                    }
                    else
                    {
                        pOutR[j] = p1R[j] - p2R[j];
                    }
                }
            }
        }

        delete[] pRankOut;
        delete[] pRank2;
        (*_pPolyOut)->updateRank();
        return 0;
    }

    if (_pPoly2->isScalar())
    {
        //size(p2) == 1
        int *pRank1     = new int[_pPoly1->getSize()];
        int iRank2      = _pPoly2->get(0)->getRank();
        int *pRankOut   = new int[_pPoly1->getSize()];

        _pPoly1->getRank(pRank1);
        for (int i = 0 ; i < _pPoly1->getSize() ; i++)
        {
            pRankOut[i] = std::max(pRank1[i], iRank2);
        }

        (*_pPolyOut) = new Polynom(_pPoly1->getVariableName(), _pPoly1->getDims(), _pPoly1->getDimsArray(), pRankOut);
        (*_pPolyOut)->setComplex(_pPoly1->isComplex() || _pPoly2->isComplex());

        //Result P1(i) + P2(0)
        SinglePoly *p2Coef          = _pPoly2->get(0);
        double *p2R                 = p2Coef->get();

        if ((*_pPolyOut)->isComplex())
        {
            double *p2I                 = p2Coef->getImg();
            for (int i = 0 ; i < _pPoly1->getSize() ; i++)
            {
                SinglePoly *p1Coef      = _pPoly1->get(i);
                double *p1R             = p1Coef->get();
                double *p1I             = p1Coef->getImg();

                SinglePoly *pOutCoef    = (*_pPolyOut)->get(i);
                double *pOutR           = pOutCoef->get();
                double *pOutI           = pOutCoef->getImg();

                for (int j = 0 ; j < pRankOut[i] + 1 ; j++)
                {
                    if (j > pRank1[j])
                    {
                        pOutR[j] = - p2R[j];
                        pOutI[j] = - (p2I ? p2I[j] : 0);
                    }
                    else if (j > iRank2)
                    {
                        pOutR[j] = p1R[j];
                        pOutI[j] = (p1I ? p1I[j] : 0);
                    }
                    else
                    {
                        pOutR[j] = p1R[j] - p2R[j];
                        pOutI[j] = (p1I ? p1I[j] : 0) - (p2I ? p2I[j] : 0);
                    }
                }
            }
        }
        else
        {
            for (int i = 0 ; i < _pPoly1->getSize() ; i++)
            {
                SinglePoly *p1Coef      = _pPoly1->get(i);
                double *p1R             = p1Coef->get();

                SinglePoly *pOutCoef    = (*_pPolyOut)->get(i);
                double *pOutR           = pOutCoef->get();

                for (int j = 0 ; j < pRankOut[i] + 1 ; j++)
                {
                    if (j > pRank1[j])
                    {
                        pOutR[j] = - p2R[j];
                    }
                    else if (j > iRank2)
                    {
                        pOutR[j] = p1R[j];
                    }
                    else
                    {
                        pOutR[j] = p1R[j] - p2R[j];
                    }
                }
            }
        }

        delete[] pRankOut;
        delete[] pRank1;
        (*_pPolyOut)->updateRank();
        return 0;
    }

    //check dimension compatibilities
    int iDims1  = _pPoly1->getDims();
    int iDims2  = _pPoly2->getDims();

    if (iDims1 != iDims2)
    {
        return 1;
    }

    int* piDims1    = _pPoly1->getDimsArray();
    int* piDims2    = _pPoly2->getDimsArray();

    for (int i = 0 ; i < iDims1 ; i++)
    {
        if (piDims1[i] != piDims2[i])
        {
            return 1;
        }
    }

    int *pRankOut   = new int[_pPoly1->getSize()];
    int *pRank1     = new int[_pPoly1->getSize()];
    int *pRank2     = new int[_pPoly2->getSize()];

    _pPoly1->getRank(pRank1);
    _pPoly2->getRank(pRank2);
    for (int i = 0 ; i < _pPoly1->getSize() ; i++)
    {
        pRankOut[i] = std::max(pRank1[i], pRank2[i]);
    }

    (*_pPolyOut) = new Polynom(_pPoly2->getVariableName(), iDims1, piDims1, pRankOut);
    (*_pPolyOut)->setComplex(_pPoly1->isComplex() || _pPoly2->isComplex());

    //Result P1(i) + P2(i)
    for (int i = 0 ; i < _pPoly1->getSize() ; i++)
    {
        double *p1R     = _pPoly1->get(i)->get();
        double *p2R     = _pPoly2->get(i)->get();
        double *pOutR   = (*_pPolyOut)->get(i)->get();
        int iMin        = std::min(pRank1[i], pRank2[i]);
        int iMax        = std::max(pRank1[i], pRank2[i]);

        for (int j = 0 ; j < iMin + 1 ; j++)
        {
            pOutR[j]    = p1R[j] - p2R[j];
        }

        double *pTemp   = NULL;
        int iCoef       = 1;
        if (pRank1[i] > pRank2[i])
        {
            pTemp       = p1R;
            iCoef       = 1;
        }
        else
        {
            pTemp       = p2R;
            iCoef       = -1;
        }

        for (int j = iMin + 1 ; j < iMax + 1 ; j++)
        {
            pOutR[j]    = pTemp[j] * iCoef;
        }

        if ((*_pPolyOut)->isComplex())
        {
            double *p1I     = _pPoly1->get(i)->getImg();
            double *p2I     = _pPoly2->get(i)->getImg();
            double *pOutI   = (*_pPolyOut)->get(i)->getImg();

            for (int j = 0 ; j < iMin + 1 ; j++)
            {
                pOutI[j]    = (p1I == NULL ? 0 : p1I[j]) - (p2I == NULL ? 0 : p2I[j]);
            }

            for (int j = iMin + 1 ; j < iMax + 1 ; j++)
            {
                pOutI[j]  = pTemp[j] * iCoef;
            }
        }
    }

    delete[] pRankOut;
    delete[] pRank1;
    delete[] pRank2;

    (*_pPolyOut)->updateRank();
    return 0;
}

//_pSparse2 - _pSparse1
int SubstractSparseToSparse(Sparse* _pSparse1, Sparse* _pSparse2, GenericType **_pSparseOut)
{
    //check scalar hidden in a sparse ;)
    if (_pSparse1->getRows() == 1 && _pSparse1->getCols() == 1)
    {
        //do scalar - sp
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

        SubstractSparseToDouble(_pSparse2, pDbl, (GenericType**)_pSparseOut);
        delete pDbl;
        return 0;
    }

    if (_pSparse2->getRows() == 1 && _pSparse2->getCols() == 1)
    {
        //do sp - scalar
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

        SubstractDoubleToSparse(pDbl, _pSparse1, (GenericType**)_pSparseOut);
        delete pDbl;
        return 0;
    }

    if (_pSparse1->getRows() != _pSparse2->getRows() || _pSparse1->getCols() != _pSparse2->getCols())
    {
        //dimensions not match
        return 1;
    }

    if (_pSparse1->nonZeros() == 0)
    {
        //sp - sp([])
        *_pSparseOut = new Sparse(*_pSparse2);
        return 0;
    }

    if (_pSparse2->nonZeros() == 0)
    {
        //sp([]) - sp
        Sparse* pOut = new Sparse(*_pSparse1);
        pOut->opposite();
        *_pSparseOut = pOut;
        return 0;
    }

    //copy _pSparse1 in _pSparseOut
    *_pSparseOut = _pSparse2->substract(*_pSparse1);
    return 0;
}

int SubstractSparseToDouble(Sparse* _pSparse, Double* _pDouble, GenericType **_pOut)
{
    //D - SP
    int iOne = 1; //fortran
    bool bComplex1 = _pSparse->isComplex();
    bool bComplex2 = _pDouble->isComplex();

    if (_pDouble->isIdentity())
    {
        //convert to sp
        Sparse* pS = new Sparse(_pSparse->getRows(), _pSparse->getCols(), _pDouble->isComplex());
        if (pS->isComplex())
        {
            for (int i = 0 ; i < std::min(_pSparse->getRows() , _pSparse->getCols()) ; i++)
            {
                pS->set(i, i, std::complex<double>(_pDouble->get(0), _pDouble->getImg(0)));
            }
        }
        else
        {
            for (int i = 0 ; i < std::min(_pSparse->getRows() , _pSparse->getCols()) ; i++)
            {
                pS->set(i, i, _pDouble->get(0));
            }
        }

        SubstractSparseToSparse(_pSparse, pS, _pOut);
        delete pS;
        return 0;
    }

    if (_pSparse->isScalar() && _pDouble->isScalar())
    {
        //d - sp
        Double* pRes = _pDouble->clone()->getAs<Double>();
        pRes->setComplex(bComplex1 | bComplex2);
        if (bComplex1)
        {
            std::complex<double> dbl = _pSparse->getImg(0, 0);
            pRes->set(0, pRes->get(0) - dbl.real());
            pRes->setImg(0, pRes->getImg(0) - dbl.imag());
        }
        else
        {
            pRes->set(0, pRes->get(0) - _pSparse->get(0, 0));
        }

        *_pOut = pRes;
        return 0;
    }

    if (_pDouble->isScalar())
    {
        //d - SP
        Double* pRes = new Double(_pSparse->getRows(), _pSparse->getCols(), bComplex1 | bComplex2);
        int iSize = _pSparse->getSize();
        double dblVal = _pDouble->get(0);
        C2F(dset)(&iSize, &dblVal, pRes->get(), &iOne);
        if (bComplex2)
        {
            double dblValI = _pDouble->getImg(0);
            C2F(dset)(&iSize, &dblValI, pRes->getImg(), &iOne);
        }
        else if (bComplex1)
        {
            //initialize imag part at 0
            double dblValI = 0;
            C2F(dset)(&iSize, &dblValI, pRes->getImg(), &iOne);
        }

        int nonZeros = static_cast<int>(_pSparse->nonZeros());
        int* pRows = new int[nonZeros * 2];
        _pSparse->outputRowCol(pRows);
        int* pCols = pRows + nonZeros;

        if (bComplex1)
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                std::complex<double> dbl = _pSparse->getImg(iRow, iCol);
                pRes->set(iRow, iCol, pRes->get(iRow, iCol) - dbl.real());
                pRes->setImg(iRow, iCol, pRes->getImg(iRow, iCol) - dbl.imag());
            }
        }
        else
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                pRes->set(iRow, iCol, pRes->get(iRow, iCol) - _pSparse->get(iRow, iCol));
            }
        }
        *_pOut = pRes;

        //clear
        delete[] pRows;
        return 0;
    }

    if (_pSparse->isScalar())
    {
        //D - sp
        Double* pRes = _pDouble->clone()->getAs<Double>();
        pRes->setComplex(bComplex1 | bComplex2);

        if (bComplex1)
        {
            double* pReal = pRes->get();
            double* pImg = pRes->getImg();
            for (int i = 0 ; i < pRes->getSize() ; i++)
            {
                std::complex<double> dbl = _pSparse->getImg(0, 0);
                pReal[i] -= dbl.real();
                pImg[i] -= dbl.imag();
            }
        }
        else
        {
            double* pReal = pRes->get();
            for (int i = 0 ; i < pRes->getSize() ; i++)
            {
                pReal[i] -= _pSparse->get(0, 0);
            }
        }
        *_pOut = pRes;
        return 0;
    }


    if (_pSparse->getRows() == _pDouble->getRows() && _pSparse->getCols() == _pDouble->getCols())
    {
        //D - SP
        Double* pRes = _pDouble->clone()->getAs<Double>();
        pRes->setComplex(bComplex1 | bComplex2);

        int nonZeros = static_cast<int>(_pSparse->nonZeros());
        int* pRows = new int[nonZeros * 2];
        _pSparse->outputRowCol(pRows);
        int* pCols = pRows + nonZeros;

        if (bComplex1)
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                std::complex<double> dbl = _pSparse->getImg(iRow, iCol);
                pRes->set(iRow, iCol, pRes->get(iRow, iCol) - dbl.real());
                pRes->setImg(iRow, iCol, pRes->getImg(iRow, iCol) - dbl.imag());
            }
        }
        else
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                int iRow = static_cast<int>(pRows[i]) - 1;
                int iCol = static_cast<int>(pCols[i]) - 1;
                pRes->set(iRow, iCol, pRes->get(iRow, iCol) - _pSparse->get(iRow, iCol));
            }
        }

        //clear
        delete[] pRows;
        *_pOut = pRes;
        return 0;
    }
    return 1;
}

int SubstractDoubleToSparse(Double* _pDouble, Sparse* _pSparse, GenericType **_pOut)
{
    //SP - D => -(D - SP)
    int iRet = SubstractSparseToDouble(_pSparse, _pDouble, _pOut);
    if (iRet)
    {
        return iRet;
    }

    //compute opposite of result
    if ((*_pOut)->isDouble())
    {
        Double* pD          = (*_pOut)->getAs<Double>();
        int iSize           = pD->getSize();
        double dblMinusOne  = -1;
        int iOne            = 1;

        C2F(dscal)(&iSize, &dblMinusOne, pD->get(), &iOne);
        if (pD->isComplex())
        {
            C2F(dscal)(&iSize, &dblMinusOne, pD->getImg(), &iOne);
        }
    }
    else if ((*_pOut)->isSparse())
    {
        Sparse* pS = (*_pOut)->getAs<Sparse>();
        pS->opposite();
    }
    else
    {
        return 1;
    }

    return 0;
}

