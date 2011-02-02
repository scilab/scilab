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
}

InternalType *GenericPlus(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;
    GenericType::RealType TypeL = _pLeftOperand->getType();
    GenericType::RealType TypeR = _pRightOperand->getType();

    /*
    ** DOUBLE + DOUBLE
    */
    if(TypeR == GenericType::RealDouble && TypeL == GenericType::RealDouble)
    {
        Double *pL = _pLeftOperand->getAs<Double>();
        Double *pR = _pRightOperand->getAs<Double>();

        int iResult = AddDoubleToDouble(pL, pR, (Double**)&pResult);
        if(iResult != 0)
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
    else if(TypeL == GenericType::RealString && TypeR == GenericType::RealString)
    {
        String *pL = _pLeftOperand->getAs<types::String>();
        String *pR = _pRightOperand->getAs<types::String>();

        int iResult = AddStringToString(pL, pR, (String**)&pResult);

        if(iResult != 0)
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
    else if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealPoly)
    {
        Double *pL          = _pLeftOperand->getAs<Double>();
        Polynom *pR      = _pRightOperand->getAs<types::Polynom>();

        int iResult = AddDoubleToPoly(pR, pL, (Polynom**)&pResult);
        if(iResult != 0)
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
    else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
    {
        Double *pR	        = _pRightOperand->getAs<Double>();
        Polynom *pL      = _pLeftOperand->getAs<types::Polynom>();

        int iResult = AddDoubleToPoly(pL, pR, (Polynom**)&pResult);
        if(iResult != 0)
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
    else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealPoly)
    {
        Polynom *pL	= _pLeftOperand->getAs<types::Polynom>();
        Polynom *pR	= _pRightOperand->getAs<types::Polynom>();

        int iResult = AddPolyToPoly(pL, pR, (Polynom**)&pResult);
        if(iResult != 0)
        {
            if(iResult == 1)
            {
                wchar_t pMsg[bsiz];
                os_swprintf(pMsg, bsiz, _W("Error: operator %ls: Matrix dimensions must agree (op1 is %ls, op2 is %ls).\n"),  L"+", pL->DimToString().c_str(), pR->DimToString().c_str());
                throw ast::ScilabError(pMsg);
            }
            else if(iResult == 2)
            {
                std::wostringstream os;
                os << L"variables don't have the same formal variable";
                //os << ((Location)e.right_get().location_get()).location_getString() << std::endl;
                throw ast::ScilabError(os.str());
            }
        }
        return pResult;
    }

    /*
    ** DOUBLE + STRING
    */
    else if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealString)
    {
        if(_pLeftOperand->getAs<Double>()->getSize() == 0)
        {//[] + "" -> ""
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
    else if(TypeL == GenericType::RealString && TypeR == GenericType::RealDouble)
    {
        if(_pRightOperand->getAs<Double>()->getSize() == 0)
        {//"text" + [] -> ""
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
	bool bComplex1  = _pDouble1->isComplex();
	bool bComplex2  = _pDouble2->isComplex();
	bool bScalar1   = _pDouble1->isScalar();
	bool bScalar2   = _pDouble2->isScalar();
	bool bIdentity1 = _pDouble1->isIdentity();
	bool bIdentity2 = _pDouble2->isIdentity();
	bool bEmpty1    = _pDouble1->isEmpty();
	bool bEmpty2    = _pDouble2->isEmpty();

    int iDims1      = _pDouble1->getDims();
    int* piDims1    = _pDouble1->getDimsArray();
    int iDims2      = _pDouble2->getDims();
    int* piDims2    = _pDouble2->getDimsArray();

	if(bEmpty1)
	{
        (*_pDoubleOut)	= dynamic_cast<Double*>(_pDouble2->clone());
	}
	else if(bEmpty2)
	{
        (*_pDoubleOut)	= dynamic_cast<Double*>(_pDouble1->clone());
	}
	else if(bIdentity1)
	{
        if(iDims2 > 2)
        {//unable to add identity matrix and greater than 2 dimensions matrix
            return 1;
        }

        (*_pDoubleOut)	= new Double(_pDouble2->getRows(), _pDouble2->getCols(), bComplex1 || bComplex2);

		if(bComplex1 == false && bComplex2 == false)
		{
			iAddRealIdentityToRealMatrix(
					_pDouble1->getReal()[0],
					_pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getCols(),
					(*_pDoubleOut)->getReal());
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			iAddRealIdentityToComplexMatrix(
					_pDouble1->getReal()[0],
					_pDouble2->getReal(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iAddComplexIdentityToRealMatrix(
					_pDouble1->getReal()[0], _pDouble1->getImg()[0],
					_pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getCols(),
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			iAddComplexIdentityToComplexMatrix(
					_pDouble1->getReal()[0], _pDouble1->getImg()[0],
					_pDouble2->getReal(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
	}
	else if(bIdentity2)
	{
        if(iDims1 > 2)
        {//unable to multiply identity matrix and greater than 2 dimensions matrix
            return 1;
        }

        (*_pDoubleOut)	= new Double(_pDouble1->getRows(), _pDouble1->getCols(), bComplex1 || bComplex2);
		if(bComplex1 == false && bComplex2 == false)
		{
			iAddRealIdentityToRealMatrix(
					_pDouble2->getReal()[0],
					_pDouble1->getReal(), _pDouble1->getRows(), _pDouble1->getCols(),
					(*_pDoubleOut)->getReal());
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			iAddComplexIdentityToRealMatrix(
					_pDouble2->getReal()[0], _pDouble2->getImg()[0],
					_pDouble1->getReal(), _pDouble1->getRows(), _pDouble1->getCols(),
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iAddRealIdentityToComplexMatrix(
					_pDouble2->getReal()[0],
					_pDouble1->getReal(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			iAddComplexIdentityToComplexMatrix(
					_pDouble2->getReal()[0], _pDouble2->getImg()[0],
					_pDouble1->getReal(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
	}
	else if(bScalar1)
	{//add pL with each element of pR
        (*_pDoubleOut)	= new Double(iDims2, piDims2, bComplex1 || bComplex2);
		if(bComplex1 == false && bComplex2 == false)
		{
			iAddRealScalarToRealMatrix(
					_pDouble1->getReal()[0],
					_pDouble2->getReal(), piDims2, iDims2,
					(*_pDoubleOut)->getReal());
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			iAddRealScalarToComplexMatrix(
					_pDouble1->getReal()[0],
					_pDouble2->getReal(), _pDouble2->getImg(), piDims2, iDims2,
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iAddComplexScalarToRealMatrix(
					_pDouble1->getReal()[0], _pDouble1->getImg()[0],
					_pDouble2->getReal(), piDims2, iDims2,
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			iAddComplexScalarToComplexMatrix(
					_pDouble1->getReal()[0], _pDouble1->getImg()[0],
					_pDouble2->getReal(), _pDouble2->getImg(), piDims2, iDims2,
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
	}
	else if(bScalar2)
	{//add pL with each element of pR
        (*_pDoubleOut)	= new Double(iDims1, piDims1, bComplex1 || bComplex2);
		if(bComplex1 == false && bComplex2 == false)
		{
			iAddRealScalarToRealMatrix(
					_pDouble2->getReal()[0],
					_pDouble1->getReal(), piDims1, iDims1,
					(*_pDoubleOut)->getReal());
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			iAddComplexScalarToRealMatrix(
					_pDouble2->getReal()[0], _pDouble2->getImg()[0],
					_pDouble1->getReal(), piDims1, iDims1,
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iAddRealScalarToComplexMatrix(
					_pDouble2->getReal()[0],
					_pDouble1->getReal(), _pDouble1->getImg(), piDims1, iDims1,
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			iAddComplexScalarToComplexMatrix(
					_pDouble2->getReal()[0], _pDouble2->getImg()[0],
					_pDouble1->getReal(), _pDouble1->getImg(), piDims1, iDims1,
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
	}
	else 
    {
        //check dimension compatibilities ( same number of dimension and same size for each dimension
        if(iDims1 != iDims2)
        {
            return 1;
        }

        for(int i = 0 ; i < iDims1 ; i++)
        {
            if(piDims1[i] != piDims2[i])
            {
                return 1;
            }
        }

        (*_pDoubleOut)	= new Double(iDims2, piDims2, bComplex1 || bComplex2);
        if(bComplex1 == false && bComplex2 == false)
        {
            iAddRealMatrixToRealMatrix(
                _pDouble1->getReal(),
                _pDouble2->getReal(), piDims2, iDims2,
                (*_pDoubleOut)->getReal());
        }
        else if(bComplex1 == false && bComplex2 == true)
        {
            iAddRealMatrixToComplexMatrix(
                _pDouble1->getReal(),
                _pDouble2->getReal(), _pDouble2->getImg(), piDims2, iDims2,
                (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
        }
        else if(bComplex1 == true && bComplex2 == false)
        {
            iAddRealMatrixToComplexMatrix(
                _pDouble2->getReal(),
                _pDouble1->getReal(), _pDouble1->getImg(), piDims1, iDims1,
                (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
        }
        else if(bComplex1 == true && bComplex2 == true)
        {
            iAddComplexMatrixToComplexMatrix(
                _pDouble1->getReal(), _pDouble1->getImg(),
                _pDouble2->getReal(), _pDouble2->getImg(), piDims2, iDims2,
                (*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
        }
    }
	return 0;
}

int AddDoubleToPoly(Polynom *_pPoly, Double *_pDouble, Polynom ** _pPolyOut)
{
	bool bComplex1 			= _pPoly->isComplex();
	bool bComplex2 			= _pDouble->isComplex();
	bool bScalar1				= _pPoly->getRows() == 1 && _pPoly->getCols() == 1;
	bool bScalar2				= _pDouble->getRows() == 1 && _pDouble->getCols() == 1;

	double *pInDblR			= _pDouble->getReal();
	double *pInDblI			= _pDouble->getImg();

	if(bScalar1)
	{//cas balaise
		int *piRank = new int[_pDouble->getSize()];
		for(int i = 0 ; i < _pDouble->getSize() ; i++)
		{
			piRank[i] = _pPoly->get(0)->getRank();
		}

		(*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pDouble->getRows(), _pDouble->getCols(), piRank);
		if(_pDouble->isComplex() || _pPoly->isComplex())
		{
			(*_pPolyOut)->setComplex(true);
		}

		for(int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
		{
			SinglePoly *pInPoly			= _pPoly->get(0);
			SinglePoly *pOutPoly		= (*_pPolyOut)->get(i);
			double *pInPolyR	= pInPoly->getCoef()->getReal();
			double *pOutPolyR	= pOutPoly->getCoef()->getReal();

			pOutPolyR[0] = pInDblR[i] + pInPolyR[0];

			for(int j = 1 ; j < pInPoly->getRank() ; j++)
			{
				pOutPolyR[j] = pInPolyR[j];
			}
		}

		if((*_pPolyOut)->isComplex())
		{
			for(int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
			{
				SinglePoly *pInPoly			= _pPoly->get(0);
				SinglePoly *pOutPoly		= (*_pPolyOut)->get(i);
				double *pInPolyI	= pInPoly->getCoef()->getImg();
				double *pOutPolyI	= pOutPoly->getCoef()->getImg();

				pOutPolyI[0] = (pInDblI != NULL ? pInDblI[i] : 0) + (pInPolyI != NULL ? pInPolyI[0] : 0);

				for(int j = 1 ; j < pInPoly->getRank() ; j++)
				{
					pOutPolyI[j] = (pInPolyI != NULL ? pInPolyI[j] : 0);
				}
			}
		}
	}
	else if(bScalar2)
	{
		//Create new SinglePoly
		(*_pPolyOut) = new Polynom();
		//Copy the old one ( rank, size, ... but not coefficients
		**_pPolyOut = *_pPoly;

		for(int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
		{
			SinglePoly *pInPoly			= _pPoly->get(i);
			SinglePoly *pOutPoly		= (*_pPolyOut)->get(i);
			double *pInPolyR	= pInPoly->getCoef()->getReal();
			double *pOutPolyR	= pOutPoly->getCoef()->getReal();

			pOutPolyR[0]			= pInDblR[0] + pInPolyR[0];
		}

		if(bComplex1 || bComplex2)
		{
			(*_pPolyOut)->setComplex(true);
			for(int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
			{
				SinglePoly *pInPoly			= _pPoly->get(i);
				SinglePoly *pOutPoly		= (*_pPolyOut)->get(i);
				double *pInPolyI	= pInPoly->getCoef()->getImg();
				double *pOutPolyI	= pOutPoly->getCoef()->getImg();

				pOutPolyI[0]			= (pInDblI == NULL ? 0 : pInDblI[0]) + (pInPolyI == NULL ? 0 : pInPolyI[0]);
			}
		}
	}
	else if (_pDouble->getRows() == _pPoly->getRows() && _pDouble->getCols() == _pPoly->getCols())
	{
		(*_pPolyOut) = new Polynom();
		//Create new SinglePoly
		**_pPolyOut = *_pPoly;

		for(int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
		{
			SinglePoly *pInPoly			= _pPoly->get(i);
			SinglePoly *pOutPoly		= (*_pPolyOut)->get(i);
			double *pInPolyR	= pInPoly->getCoef()->getReal();
			double *pOutPolyR	= pOutPoly->getCoef()->getReal();

			pOutPolyR[0]			= pInDblR[i] + pInPolyR[0];
		}

		if(bComplex1 || bComplex2)
		{
			(*_pPolyOut)->setComplex(true);
			for(int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
			{
				SinglePoly *pInPoly			= _pPoly->get(i);
				SinglePoly *pOutPoly		= (*_pPolyOut)->get(i);
				double *pInPolyI	= pInPoly->getCoef()->getImg();
				double *pOutPolyI	= pOutPoly->getCoef()->getImg();

				pOutPolyI[0]			= (pInDblI != NULL ? pInDblI[i] : 0) + (pInPolyI != NULL ? pInPolyI[0] : 0);
			}
		}
	}
	else
	{
		return 1;
	}
	return 0;
}

int AddPolyToPoly(Polynom* _pPoly1, Polynom* _pPoly2, Polynom ** _pPolyOut)
{
	if(_pPoly1->getVariableName() != _pPoly2->getVariableName())
	{
		return 2;
	}

	bool bComplex1 			= _pPoly1->isComplex();
	bool bComplex2 			= _pPoly2->isComplex();
	bool bScalar1				= _pPoly1->getRows() == 1 && _pPoly1->getCols() == 1;
	bool bScalar2				= _pPoly2->getRows() == 1 && _pPoly2->getCols() == 1;
	//3 cases :
	//size(p1) == size(P2)
	if(_pPoly1->getRows() == _pPoly2->getRows() && _pPoly1->getCols() == _pPoly2->getCols())
	{
		int *pRank	= new int[_pPoly1->getSize()];
		int *pRank1	= new int[_pPoly1->getSize()];
		int *pRank2	= new int[_pPoly2->getSize()];

		_pPoly1->getRank(pRank1);
		_pPoly2->getRank(pRank2);
		for(int i = 0 ; i < _pPoly1->getSize() ; i++)
		{
			pRank[i] = Max(pRank1[i], pRank2[i]);
		}

		(*_pPolyOut) = new Polynom(_pPoly2->getVariableName(), _pPoly1->getRows(), _pPoly1->getCols(), pRank);
		if(_pPoly1->isComplex() || _pPoly2->isComplex())
		{
			(*_pPolyOut)->setComplex(true);
		}

		if(bComplex1 == false && bComplex2 == false)
		{
			for(int i = 0 ; i < _pPoly1->getSize() ; i++)
			{
				iAddRealPolyToRealPoly(
					_pPoly1->get(i)->getCoef()->getReal(), pRank1[i],
					_pPoly2->get(i)->getCoef()->getReal(), pRank2[i],
					(*_pPolyOut)->get(i)->getCoef()->getReal(), pRank[i]);
			}
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			for(int i = 0 ; i < _pPoly1->getSize() ; i++)
			{
				iAddRealPolyToComplexPoly(
					_pPoly1->get(i)->getCoef()->getReal(), pRank1[i],
					_pPoly2->get(i)->getCoef()->getReal(), _pPoly2->get(i)->getCoef()->getImg(), pRank2[i],
					(*_pPolyOut)->get(i)->getCoef()->getReal(), (*_pPolyOut)->get(i)->getCoef()->getImg(), pRank[i]);
			}
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			for(int i = 0 ; i < _pPoly1->getSize() ; i++)
			{
				iAddRealPolyToComplexPoly(
					_pPoly2->get(i)->getCoef()->getReal(), pRank2[i],
					_pPoly1->get(i)->getCoef()->getReal(), _pPoly1->get(i)->getCoef()->getImg(), pRank1[i],
					(*_pPolyOut)->get(i)->getCoef()->getReal(), (*_pPolyOut)->get(i)->getCoef()->getImg(), pRank[i]);
			}
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			for(int i = 0 ; i < _pPoly1->getSize() ; i++)
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
	}
	else if(bScalar1)
	{//size(p1) == 1
		int *pRank	= new int[_pPoly2->getSize()];
		int *pRank1	= new int[_pPoly2->getSize()];
		int *pRank2	= new int[_pPoly2->getSize()];
		memset(pRank1, 0x00, _pPoly2->getSize() * sizeof(int));

		_pPoly1->getRank(pRank1);
		_pPoly2->getRank(pRank2);
		for(int i = 0 ; i < _pPoly2->getSize() ; i++)
		{
			pRank[i] = Max(pRank1[0], pRank2[i]);
		}

		(*_pPolyOut) = new Polynom(_pPoly2->getVariableName(), _pPoly2->getRows(), _pPoly2->getCols(), pRank);
		if(_pPoly1->isComplex() || _pPoly2->isComplex())
		{
			(*_pPolyOut)->setComplex(true);
		}

		//Result P1(0) + P2(i)
		Double *pCoef1	= _pPoly1->get(0)->getCoef();
		double *p1R			= pCoef1->getReal();
		double *p1I			= pCoef1->getImg();
		for(int i = 0 ; i < _pPoly2->getSize() ; i++)
		{
			Double *pCoef2	= _pPoly2->get(i)->getCoef();
			double *p2R			= pCoef2->getReal();
			double *p2I			= pCoef2->getImg();

			Double *pCoefR	= (*_pPolyOut)->get(i)->getCoef();
			double *pRR			= pCoefR->getReal();
			double *pRI			= pCoefR->getImg();

			for(int j = 0 ; j < Min(pRank1[0], pRank2[i]) ; j++)
			{
				pRR[j] = p1R[j] + p2R[j];
			}

			double *pTemp = (pRank1[0] > pRank2[i] ? p1R : p2R);
			for(int j = Min(pRank1[0], pRank2[i]) ; j < Max(pRank1[0], pRank2[i]) ; j++)
			{
				pRR[j] = pTemp[j];
			}

			if((*_pPolyOut)->isComplex())
			{
				for(int j = 0 ; j < Min(pRank1[0], pRank2[i]) ; j++)
				{
					pRI[j] = (p1I == NULL ? 0 : p1I[j]) + (p2I == NULL ? 0 : p2I[j]);
				}

				double *pTemp = (pRank1[0] > pRank2[i] ? p1I : p2I);
				for(int j = Min(pRank1[0], pRank2[i]) ; j < Max(pRank1[0], pRank2[i]); j++)
				{
					pRI[j] = pTemp == NULL ? 0 : pTemp[j];
				}
			}
		}

		delete[] pRank;
		delete[] pRank1;
		delete[] pRank2;
	}
	else if(bScalar2)
	{//size(p2) == 1
		int *pRank	= new int[_pPoly1->getSize()];
		int *pRank1	= new int[_pPoly1->getSize()];
		int *pRank2	= new int[_pPoly1->getSize()];
		memset(pRank2, 0x00, _pPoly1->getSize() * sizeof(int));

		_pPoly1->getRank(pRank1);
		_pPoly2->getRank(pRank2);
		for(int i = 0 ; i < _pPoly1->getSize() ; i++)
		{
			pRank[i] = Max(pRank1[i], pRank2[0]);
		}

		(*_pPolyOut) = new Polynom(_pPoly1->getVariableName(), _pPoly1->getRows(), _pPoly1->getCols(), pRank);
		if(_pPoly1->isComplex() || _pPoly2->isComplex())
		{
			(*_pPolyOut)->setComplex(true);
		}

		//Result P1(i) + P2(0)
		Double *pCoef2	= _pPoly2->get(0)->getCoef();
		double *p2R			= pCoef2->getReal();
		double *p2I			= pCoef2->getImg();

		for(int i = 0 ; i < _pPoly1->getSize() ; i++)
		{
			Double *pCoef1	= _pPoly1->get(i)->getCoef();
			double *p1R			= pCoef1->getReal();
			double *p1I			= pCoef1->getImg();

			Double *pCoefR	= (*_pPolyOut)->get(i)->getCoef();
			double *pRR			= pCoefR->getReal();
			double *pRI			= pCoefR->getImg();

			for(int j = 0 ; j < Min(pRank1[i], pRank2[0]) ; j++)
			{
				pRR[j] = p1R[j] + p2R[j];
			}

			double *pTemp = (pRank1[i] > pRank2[0] ? p1R : p2R);
			for(int j = Min(pRank1[i], pRank2[0]) ; j < Max(pRank1[i], pRank2[0]) ; j++)
			{
				pRR[j] = pTemp[j];
			}

			if((*_pPolyOut)->isComplex())
			{
				for(int j = 0 ; j < Min(pRank1[i], pRank2[0]) ; j++)
				{
					pRI[j] = (p1I == NULL ? 0 : p1I[j]) + (p2I == NULL ? 0 : p2I[j]);
				}

				double *pTemp = (pRank1[i] > pRank2[0] ? p1I : p2I);
				for(int j = Min(pRank1[i], pRank2[0]) ; j < Max(pRank1[i], pRank2[0]); j++)
				{
					pRI[j] = pTemp == NULL ? 0 : pTemp[j];
				}
			}
		}

		delete[] pRank;
		delete[] pRank1;
		delete[] pRank2;
	}
	else
	{
		return 1;
	}

	if((*_pPolyOut) != NULL)
	{
		(*_pPolyOut)->updateRank();
	}

	//if pResult == NULL -> incompatible dimensions
	return 0;
}

int AddStringToString(String *_pString1, String *_pString2, String **_pStringOut)
{
	bool bScalar1				= _pString1->getRows() == 1 && _pString1->getCols() == 1;
	bool bScalar2				= _pString2->getRows() == 1 && _pString2->getCols() == 1;


	if(bScalar1)
	{//concat pL with each element of pR
		(*_pStringOut)  = new String(_pString2->getRows(), _pString2->getCols());
		int iCommonLen	=	(int)wcslen(_pString1->get(0,0));

		for(int i = 0 ; i < _pString2->getRows() ; i++)
		{
			for(int j = 0 ; j < _pString2->getCols() ; j++)
			{
				int iLen	= (int)wcslen(_pString2->get(i, j));
				wchar_t* psz = new wchar_t[iLen + iCommonLen + 1];
				memset(psz, 0x00, (iLen + iCommonLen + 1) * sizeof(wchar_t));
				memcpy(psz, _pString1->get(0,0), iCommonLen * sizeof(wchar_t));
				memcpy(psz + iCommonLen, _pString2->get(i, j), iLen * sizeof(wchar_t));
				(*_pStringOut)->set(i, j, psz);
				delete[] psz;
			}
		}
	}
	else if(bScalar2)
	{//concat each element of pL with pR
		(*_pStringOut)  = new String(_pString1->getRows(), _pString1->getCols());
		int iCommonLen	=	(int)wcslen(_pString2->get(0,0));

		for(int i = 0 ; i < _pString1->getRows() ; i++)
		{
			for(int j = 0 ; j < _pString1->getCols() ; j++)
			{
				int iLen = (int)wcslen(_pString1->get(i, j));
				wchar_t* psz = new wchar_t[iLen + iCommonLen + 1];
				memset(psz, 0x00, (iLen + iCommonLen + 1) * sizeof(wchar_t));

				memcpy(psz, _pString1->get(i, j), iLen * sizeof(wchar_t));
				memcpy(psz + iLen, _pString2->get(0,0), iCommonLen * sizeof(wchar_t));

				(*_pStringOut)->set(i, j, psz);
				delete[] psz;
			}
		}
	}
	else if(_pString1->getRows() == _pString2->getRows() && _pString1->getCols() == _pString2->getCols())
	{//concat pL with pR element wise
		(*_pStringOut) = new String(_pString1->getRows(), _pString1->getCols());
		for(int i = 0 ; i < _pString1->getRows() ; i++)
		{
			for(int j = 0 ; j < _pString1->getCols() ; j++)
			{
				int iLenL	= (int)wcslen(_pString1->get(i, j));
				int iLenR	= (int)wcslen(_pString2->get(i, j));
				wchar_t* psz = new wchar_t[iLenL + iLenR + 1];
				memset(psz, 0x00, (iLenL + iLenR + 1) * sizeof(wchar_t));

				memcpy(psz          , _pString1->get(i, j), iLenL * sizeof(wchar_t));
				memcpy(psz + iLenL  , _pString2->get(i, j), iLenR * sizeof(wchar_t));

				(*_pStringOut)->set(i, j, psz);
				delete[] psz;
			}
		}
	}
	else
	{
		return 1;
	}
	return 0;

}

