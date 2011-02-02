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
#include "matrix_right_division.h"
#include "localization.h"
#include "charEncoding.h"
}

using namespace types;

InternalType *GenericRDivide(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{

    InternalType *pResult = NULL;
    GenericType::RealType TypeL = _pLeftOperand->getType();
    GenericType::RealType TypeR = _pRightOperand->getType();

    /*
    ** DOUBLE / DOUBLE
    */
    if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
    {
        Double *pL			= _pLeftOperand->getAs<Double>();
        Double *pR			= _pRightOperand->getAs<Double>();

        int iResult = DivideDoubleByDouble(pL, pR, (Double**)&pResult);
        if(iResult)
        {//manage errors
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
        return pResult;
    }

    /*
    ** POLY / DOUBLE
    */
    else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
    {
        Polynom *pL          = _pLeftOperand->getAs<types::Polynom>();
        Double *pR              = _pRightOperand->getAs<Double>();

        int iResult = DividePolyByDouble(pL, pR, (Polynom**)&pResult);
        if(iResult)
        {//manage errors
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** DOUBLE / POLY
    */
    else if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealPoly)
    {
        Double *pL      = _pLeftOperand->getAs<Double>();
        Polynom *pR  = _pRightOperand->getAs<types::Polynom>();

        int iResult = DivideDoubleByPoly(pL, pR, (Polynom**)&pResult);
        if(iResult)
        {//manage errors
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
        return pResult;
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return NULL;
}

int DivideDoubleByDouble(Double *_pDouble1, Double *_pDouble2, Double **_pDoubleOut)
{
    bool bComplex1		= _pDouble1->isComplex();
    bool bComplex2		= _pDouble2->isComplex();
    bool bScalar1			= _pDouble1->getRows() == 1 && _pDouble1->getCols() == 1;
    bool bScalar2			= _pDouble2->getRows() == 1 && _pDouble2->getCols() == 1;

    //check finite values of _pDouble1 and _pDouble2
    if(isDoubleFinite(_pDouble1) == false || isDoubleFinite(_pDouble2) == false)
    {
        return 2;
    }

    if(_pDouble1->getSize() == 0 || _pDouble2->getSize() == 0)
    {
        //return an empty matrix of double
        *_pDoubleOut	= Double::Empty();
    }
    else if(bScalar2)
    {//[] / x
        int iErr				= 0;
        int iRowResult	= _pDouble1->getRows();
        int iColResult	= _pDouble1->getCols();
        int iInc1				= 1;
        int iInc2				= 0;
        int iSizeResult	= iRowResult * iColResult;

        *_pDoubleOut = new Double(iRowResult, iColResult, _pDouble1->isComplex() || _pDouble2->isComplex());

        if(bComplex1 == false && bComplex2 == false)
        {// Real1 \ Real2 -> Real2 / Real1
            iErr = iRightDivisionRealMatrixByRealMatrix(
                _pDouble1->getReal(), iInc1, 
                _pDouble2->getReal(), iInc2, 
                (*_pDoubleOut)->getReal(), 1, iSizeResult);
        }
        else if(bComplex1 == false && bComplex2 == true)
        {// Real \ Complex -> Complex / Real
            iErr = iRightDivisionRealMatrixByComplexMatrix(
                _pDouble1->getReal(), iInc1, 
                _pDouble2->getReal(), _pDouble2->getImg(), iInc2, 
                (*_pDoubleOut)->getReal(),	(*_pDoubleOut)->getImg(), 1, iSizeResult);
        }
        else if(bComplex1 == true && bComplex2 == false)
        {// Complex \ Real -> Real / Complex
            iErr = iRightDivisionComplexMatrixByRealMatrix(
                _pDouble1->getReal(), _pDouble1->getImg(), iInc1, 
                _pDouble2->getReal(), iInc2,
                (*_pDoubleOut)->getReal(),	(*_pDoubleOut)->getImg(), 1, iSizeResult);
        }
        else if(bComplex1 == true && bComplex2 == true)
        {// Complex \ Complex
            iErr = iRightDivisionComplexMatrixByComplexMatrix(
                _pDouble1->getReal(), _pDouble1->getImg(), iInc1,
                _pDouble2->getReal(), _pDouble2->getImg(), iInc2, 
                (*_pDoubleOut)->getReal(),	(*_pDoubleOut)->getImg(), 1, iSizeResult);
        }
    }
    else if(bScalar1)
    {
        double dblSavedR = 0;
        double dblSavedI = 0;
        Double *pdblTemp = NULL;

        int iRowResult = _pDouble2->getCols();
        int iColResult = _pDouble2->getRows();

        //in this case, we have to create a temporary square matrix
        pdblTemp	= new Double(iRowResult, iRowResult, _pDouble1->isComplex());
        pdblTemp->setZeros();

        if(_pDouble1->isComplex())
        {
            dblSavedR = _pDouble1->getReal()[0];
            dblSavedI = _pDouble1->getImg()[0];
            for(int i = 0 ; i < iRowResult ; i++)
            {
                pdblTemp->set(i, i, dblSavedR);
                pdblTemp->setImg(i, i, dblSavedI);
            }
        }
        else
        {
            dblSavedR = _pDouble1->getReal()[0];
            for(int i = 0 ; i < iRowResult ; i++)
            {
                pdblTemp->set(i, i, dblSavedR);
            }
        }

        *_pDoubleOut = new Double(iRowResult, iColResult, _pDouble1->isComplex() || _pDouble2->isComplex());

        if((*_pDoubleOut)->isComplex())
        {
            double dblRcond = 0;
            iRightDivisionOfComplexMatrix(
                pdblTemp->getReal(), pdblTemp->getImg(), pdblTemp->getRows(), pdblTemp->getCols(),
                _pDouble2->getReal(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), iRowResult, iColResult, &dblRcond);
        }
        else
        {
            double dblRcond = 0;
            iRightDivisionOfRealMatrix(
                pdblTemp->getReal(), pdblTemp->getRows(), pdblTemp->getCols(),
                _pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->getReal(), iRowResult, iColResult, &dblRcond);
        }
        delete pdblTemp;
    }
    else if(_pDouble1->getCols() == _pDouble2->getCols())
    {
        *_pDoubleOut = new Double(_pDouble1->getRows(), _pDouble2->getRows(), _pDouble1->isComplex() || _pDouble2->isComplex());
        if((*_pDoubleOut)->isComplex())
        {
            double dblRcond = 0;
            iRightDivisionOfComplexMatrix(
                _pDouble1->getReal(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
                _pDouble2->getReal(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), _pDouble1->getRows(), _pDouble2->getRows(), &dblRcond);
        }
        else
        {
            double dblRcond = 0;
            iRightDivisionOfRealMatrix(
                _pDouble1->getReal(), _pDouble1->getRows(), _pDouble1->getCols(),
                _pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getCols(),
                (*_pDoubleOut)->getReal(), _pDouble1->getRows(), _pDouble2->getRows(), &dblRcond);
        }
    }
    else
    {
        //Bad dimensions
        return 1;
    }

    return 0;
}

int DividePolyByDouble(Polynom* _pPoly, Double* _pDouble, Polynom** _pPolyOut)
{
    bool bComplex1		= _pPoly->isComplex();
    bool bComplex2		= _pDouble->isComplex();
    bool bScalar1			= _pPoly->getRows() == 1		&& _pPoly->getCols() == 1;
    bool bScalar2			= _pDouble->getRows() == 1 && _pDouble->getCols() == 1;

    Polynom *pTemp	= NULL; //use only is _pPoly is scalar and _pDouble not.

    int iRowResult 	= 0;
    int iColResult	= 0;
    int *piRank			= NULL;

    /*	if(bScalar1 && bScalar2)
    {
    iRowResult = 1;
    iColResult = 1;

    piRank = new int[1];
    piRank[0]	= _pPoly->get(0)->getRank();
    }
    else */if(bScalar2)
    {
        iRowResult = _pPoly->getRows();
        iColResult = _pPoly->getCols();

        piRank = new int[iRowResult * iColResult];
        for(int i = 0 ; i < iRowResult * iColResult ; i++)
        {
            piRank[i] = _pPoly->get(i)->getRank();
        }
    }
    else if(bScalar1)
    {
        //in this case, we have to create a temporary square polinomial matrix
        iRowResult = _pDouble->getCols();
        iColResult = _pDouble->getRows();

        piRank = new int[iRowResult * iRowResult];
        for(int i = 0 ; i < iRowResult * iRowResult ; i++)
        {
            piRank[i] = _pPoly->getMaxRank();
        }

        pTemp = new Polynom(_pPoly->getVariableName(), iRowResult, iRowResult, piRank);
        if(bComplex1 || bComplex2)
        {
            pTemp->setComplex(true);
        }

        Double *pdblData = _pPoly->get(0)->getCoef();
        for(int i = 0 ; i < iRowResult ; i++)
        {
            pTemp->setCoef(i,i, pdblData);
        }
    }
    /*	else if(pR->getSize() == 1)
    {
    iRowResult = pL->getRows();
    iColResult = pL->getCols();

    piRank = new int[iRowResult * iColResult];
    for(int i = 0 ; i < iRowResult * iColResult ; i++)
    {
    piRank[i] = pL->get(i)->getRank();
    }
    }
    else if(pL->getRows() == pR->getRows() && pL->getCols() == pR->getCols())
    {//Je ne sais pas encore comment ca marche ce machin la !!!
    iRowResult = pR->getRows();
    iColResult = pR->getCols();
    }
    */

    (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), iRowResult, iColResult, piRank);
    delete[] piRank;
    if(bComplex1 || bComplex2)
    {
        (*_pPolyOut)->setComplex(true);
    }

    if(bScalar2)
    {//[p] * cst
        for(int i = 0 ; i < _pPoly->getSize() ; i++)
        {
            SinglePoly *pPolyIn			= _pPoly->get(i);
            double* pRealIn		= pPolyIn->getCoef()->getReal();
            double* pImgIn		= pPolyIn->getCoef()->getImg();

            SinglePoly *pPolyOut		= (*_pPolyOut)->get(i);
            double* pRealOut	= pPolyOut->getCoef()->getReal();
            double* pImgOut		= pPolyOut->getCoef()->getImg();

            if(bComplex1 == false && bComplex2 == false)
            {
                iRightDivisionRealMatrixByRealMatrix(
                    pRealIn, 1,
                    _pDouble->getReal(),	0, 
                    pRealOut, 1, pPolyOut->getRank());
            }
            else if(bComplex1 == false && bComplex2 == true)
            {
                iRightDivisionRealMatrixByComplexMatrix(
                    pRealIn, 1,
                    _pDouble->getReal(),	_pDouble->getImg(), 0, 
                    pRealOut,	pImgOut, 1, pPolyOut->getRank());
            }
            else if(bComplex1 == true && bComplex2 == false)
            {
                iRightDivisionComplexMatrixByRealMatrix(
                    pRealIn, pImgIn, 1,
                    _pDouble->getReal(), 0, 
                    pRealOut, pImgOut, 1, pPolyOut->getRank());
            }
            else if(bComplex1 == true && bComplex2 == true)
            {
                iRightDivisionComplexMatrixByComplexMatrix(
                    pRealIn, pImgIn, 1,
                    _pDouble->getReal(),	_pDouble->getImg(), 0, 
                    pRealOut, pImgOut, 1, pPolyOut->getRank());
            }
        }
    }
    else if(bScalar1)
    {
        Double *pResultCoef = new Double(iRowResult, iColResult, (*_pPolyOut)->isComplex());
        double *pReal				= pResultCoef->getReal();
        double *pImg				= pResultCoef->getImg();

        for(int i = 0 ; i < pTemp->get(0)->getRank() ; i++)
        {
            Double *pCoef				= pTemp->extractCoef(i);
            Double *pResultCoef = new Double(iRowResult, iColResult, pCoef->isComplex());
            double *pReal				= pResultCoef->getReal();
            double *pImg				= pResultCoef->getImg();

            if(bComplex1 == false && bComplex2 == false)
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

int DivideDoubleByPoly(Double* _pDouble, Polynom* _pPoly, Polynom** _pPolyOut)
{
    return 0;
}
