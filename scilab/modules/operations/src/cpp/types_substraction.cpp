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

#include "types_substraction.hxx"
#include "scilabexception.hxx"
#include "core_math.h"

extern "C"
{
	#include "matrix_substraction.h"
    #include "localization.h"
    #include "charEncoding.h"
}

InternalType* GenericMinus(InternalType* _pLeftOperand, InternalType* _pRightOperand)
{
    InternalType *pResult = NULL;
    GenericType::RealType TypeL = _pLeftOperand->getType();
    GenericType::RealType TypeR = _pRightOperand->getType();

    /*
    ** DOUBLE - DOUBLE
    */
    if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
    {
        Double *pL = _pLeftOperand->getAs<Double>();
        Double *pR = _pRightOperand->getAs<Double>();

        int iResult = SubstractDoubleToDouble(pL, pR, (Double**)&pResult);
        if(iResult != 0)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
        return pResult;
    }

    /*
    ** DOUBLE - POLY
    */
    else if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealPoly)
    {
        Double *pL              = _pLeftOperand->getAs<Double>();
        Polynom *pR          = _pRightOperand->getAs<types::Polynom>();

        int iResult = SubstractPolyToDouble(pL, pR, (Polynom**)&pResult);
        if(iResult != 0)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
        return pResult;
    }

    /*
    ** POLY - DOUBLE
    */
    else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
    {
        Polynom *pL			= _pLeftOperand->getAs<types::Polynom>();
        Double *pR				= _pRightOperand->getAs<Double>();

        int iResult = SubstractDoubleToPoly(pL, pR, (Polynom**)&pResult);
        if(iResult != 0)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
        return pResult;
    }

    /*
    ** POLY - POLY
    */
    else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealPoly)
    {
        Polynom *pL			= _pLeftOperand->getAs<types::Polynom>();
        Polynom *pR			= _pRightOperand->getAs<types::Polynom>();

        int iResult = SubstractPolyToPoly(pL, pR, (Polynom**)&pResult);
        if(iResult != 0)
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
        (*_pDoubleOut)	= _pDouble2;
        double* pReal = (*_pDoubleOut)->getReal();
        for(int i = 0 ; i < (*_pDoubleOut)->getSize() ; i++)
        {
            pReal[i] *= -1;
        }

        if(bComplex2)
        {
            double* pImg = (*_pDoubleOut)->getImg();
            for(int i = 0 ; i < (*_pDoubleOut)->getSize() ; i++)
            {
                pImg[i] *= -1;
            }
        }
	}
	else if(bEmpty2)
	{
        (*_pDoubleOut)	= dynamic_cast<Double*>(_pDouble1->clone());
	}
	else if(bIdentity1)
	{
        if(iDims2 > 2)
        {//unable to substract identity matrix and greater than 2 dimensions matrix
            return 1;
        }

        (*_pDoubleOut)	= new Double(_pDouble2->getRows(), _pDouble2->getCols(), bComplex1 || bComplex2);

        if(bComplex1 == false && bComplex2 == false)
		{
			iSubstractRealIdentityToRealMatrix(
					_pDouble1->getReal()[0],
					_pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getCols(),
					(*_pDoubleOut)->getReal());
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			iSubstractRealIdentityToComplexMatrix(
					_pDouble1->getReal()[0],
					_pDouble2->getReal(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iSubstractComplexIdentityToRealMatrix(
					_pDouble1->getReal()[0], _pDouble1->getImg()[0],
					_pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getCols(),
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			iSubstractComplexIdentityToComplexMatrix(
					_pDouble1->getReal()[0], _pDouble1->getImg()[0],
					_pDouble2->getReal(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
	}
	else if(bIdentity2)
	{
        if(iDims1 > 2)
        {//unable to substract identity matrix and greater than 2 dimensions matrix
            return 1;
        }

        (*_pDoubleOut) = new Double(_pDouble1->getRows(), _pDouble1->getCols(), bComplex1 || bComplex2);
		if(bComplex1 == false && bComplex2 == false)
		{
			iSubstractRealIdentityToRealMatrix(
					_pDouble2->getReal()[0],
					_pDouble1->getReal(), _pDouble1->getRows(), _pDouble1->getCols(),
					(*_pDoubleOut)->getReal());
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			iSubstractComplexIdentityToRealMatrix(
					_pDouble2->getReal()[0], _pDouble2->getImg()[0],
					_pDouble1->getReal(), _pDouble1->getRows(), _pDouble1->getCols(),
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iSubstractRealIdentityToComplexMatrix(
					_pDouble2->getReal()[0],
					_pDouble1->getReal(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			iSubstractComplexIdentityToComplexMatrix(
					_pDouble2->getReal()[0], _pDouble2->getImg()[0],
					_pDouble1->getReal(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
	}
	else if(bScalar1)
	{//add pL with each element of pR
        (*_pDoubleOut) = new Double(iDims2, piDims2, bComplex1 || bComplex2);
		if(bComplex1 == false && bComplex2 == false)
		{
			iSubstractRealMatrixToRealScalar(
					_pDouble2->getReal(), piDims2, iDims2,
					_pDouble1->getReal()[0],
					(*_pDoubleOut)->getReal());
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			iSubstractComplexMatrixToRealScalar(
					_pDouble2->getReal(), _pDouble2->getImg(), piDims2, iDims2,
					_pDouble1->getReal()[0],
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iSubstractRealMatrixToComplexScalar(
					_pDouble2->getReal(), piDims2, iDims2,
					_pDouble1->getReal()[0], _pDouble1->getImg()[0],
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			iSubstractComplexMatrixToComplexScalar(
					_pDouble2->getReal(), _pDouble2->getImg(), piDims2, iDims2,
					_pDouble1->getReal()[0], _pDouble1->getImg()[0],
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
	}
	else if(bScalar2)
	{//add pL with each element of pR
        (*_pDoubleOut) = new Double(iDims1, piDims1, bComplex1 || bComplex2);
		if(bComplex1 == false && bComplex2 == false)
		{
			iSubstractRealScalarToRealMatrix(
					_pDouble2->getReal()[0],
					_pDouble1->getReal(), piDims1, iDims1,
					(*_pDoubleOut)->getReal());
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			iSubstractComplexScalarToRealMatrix(
					_pDouble2->getReal()[0], _pDouble2->getImg()[0],
					_pDouble1->getReal(), piDims1, iDims1,
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iSubstractRealScalarToComplexMatrix(
					_pDouble2->getReal()[0],
					_pDouble1->getReal(), _pDouble1->getImg(), piDims1, iDims1,
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			iSubstractComplexScalarToComplexMatrix(
					_pDouble2->getReal()[0], _pDouble2->getImg()[0],
					_pDouble1->getReal(), _pDouble1->getImg(), piDims1, iDims1,
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
	}
	else
	{//add pL and pR element wise
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
			iSubstractRealMatrixToRealMatrix(
					_pDouble2->getReal(),
					_pDouble1->getReal(), piDims1, iDims1,
					(*_pDoubleOut)->getReal());
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			iSubstractComplexMatrixToRealMatrix(
					_pDouble2->getReal(), _pDouble2->getImg(),
					_pDouble1->getReal(), piDims1, iDims1,
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iSubstractRealMatrixToComplexMatrix(
					_pDouble2->getReal(),
					_pDouble1->getReal(), _pDouble1->getImg(), piDims1, iDims1,
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			iSubstractComplexMatrixToComplexMatrix(
					_pDouble2->getReal(), _pDouble2->getImg(),
					_pDouble1->getReal(), _pDouble1->getImg(), piDims1, iDims1,
					(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
	}
	return 0;
}

int SubstractPolyToDouble(Double *_pDouble, Polynom *_pPoly, Polynom** _pPolyOut)
{
	double *pInDblR			= _pDouble->getReal();
	double *pInDblI			= _pDouble->getImg();

	if(_pDouble->getSize() == 1)
	{
		(*_pPolyOut) = new Polynom();
		//Create new SinglePoly
		**_pPolyOut = *_pPoly;

		if(_pPoly->isComplex() || _pDouble->isComplex())
		{
			(*_pPolyOut)->setComplex(true);
		}

		for(int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
		{
			SinglePoly *pInPoly			= _pPoly->get(i);
			SinglePoly *pOutPoly		= (*_pPolyOut)->get(i);
			double *pInPolyR	= pInPoly->getCoef()->getReal();
			double *pOutPolyR	= pOutPoly->getCoef()->getReal();

			pOutPolyR[0]			= pInDblR[0] - pInPolyR[0];

			for(int j = 1 ; j < pInPoly->getRank() ; j++)
			{
				pOutPolyR[j] = -pOutPolyR[j];
			}
		}

		if((*_pPolyOut)->isComplex())
		{
			(*_pPolyOut)->setComplex(true);
			for(int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
			{
				SinglePoly *pInPoly			= _pPoly->get(i);
				SinglePoly *pOutPoly		= (*_pPolyOut)->get(i);
				double *pInPolyI	= pInPoly->getCoef()->getImg();
				double *pOutPolyI	= pOutPoly->getCoef()->getImg();

				pOutPolyI[0]		= (pInDblI == NULL ? 0 : pInDblI[0]) - (pInPolyI == NULL ? 0 : pInPolyI[0]);

				for(int j = 1 ; j < pInPoly->getRank() ; j++)
				{
					pOutPolyI[j] = -pOutPolyI[j];
				}
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

			pOutPolyR[0] = pInDblR[i] - pInPolyR[0];

			for(int j = 1 ; j < pInPoly->getRank() ; j++)
			{
				pOutPolyR[j] = -pOutPolyR[j];
			}
		}

		if(_pPoly->isComplex() || _pDouble->isComplex())
		{
			(*_pPolyOut)->setComplex(true);
			for(int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
			{
				SinglePoly *pInPoly			= _pPoly->get(i);
				SinglePoly *pOutPoly		= (*_pPolyOut)->get(i);

				double *pInPolyI	= pInPoly->getCoef()->getImg();
				double *pOutPolyI	= pOutPoly->getCoef()->getImg();

				pOutPolyI[0] = (pInDblI != NULL ? pInDblI[i] : 0) - (pOutPolyI != NULL ? pOutPolyI[0] : 0);

				for(int j = 1 ; j < pInPoly->getRank() ; j++)
				{
					pOutPolyI[j] = -pOutPolyI[j];
				}
			}
		}
	}
	else if(_pPoly->getSize() == 1)
	{//cas balaise
		int *piRank = new int[_pDouble->getSize()];
		for(int i = 0 ; i < _pDouble->getSize() ; i++)
		{
			piRank[i] = _pPoly->get(0)->getRank();
		}

		(*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pDouble->getRows(), _pDouble->getCols(), piRank);

		for(int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
		{
			SinglePoly *pInPoly			= _pPoly->get(0);
			SinglePoly *pOutPoly		= (*_pPolyOut)->get(i);

			double *pInPolyR	= pInPoly->getCoef()->getReal();
			double *pOutPolyR	= pOutPoly->getCoef()->getReal();

			pOutPolyR[0] = pInDblR[i] - pInPolyR[0];

			for(int j = 1 ; j < pOutPoly->getRank() ; j++)
			{
				pOutPolyR[j] = -pInPolyR[j];
			}
		}

		if(_pPoly->isComplex() || _pDouble->isComplex())
		{
			(*_pPolyOut)->setComplex(true);
			for(int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
			{
				SinglePoly *pInPoly			= _pPoly->get(0);
				SinglePoly *pOutPoly		= (*_pPolyOut)->get(i);

				double *pInPolyI	= pInPoly->getCoef()->getImg();
				double *pOutPolyI	= pOutPoly->getCoef()->getImg();

				pOutPolyI[0] = (pInDblI != NULL ? pInDblI[i] : 0) - (pInPolyI != NULL ? pInPolyI[0] : 0);

				for(int j = 1 ; j < pOutPoly->getRank() ; j++)
				{
					pOutPolyI[j] = -pInPolyI[j];
				}
			}
		}
	}
	else
	{
		return 1;
	}

	return 0;
}

int SubstractDoubleToPoly(Polynom *_pPoly,Double *_pDouble, Polynom **_pPolyOut)
{
	double *pInDblR			= _pDouble->getReal();
	double *pInDblI			= _pDouble->getImg();

	if(_pDouble->getSize() == 1)
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

			pOutPolyR[0]			= pInPolyR[0] - pInDblR[0];
		}

		if(_pPoly->isComplex() || _pDouble->isComplex())
		{
			(*_pPolyOut)->setComplex(true);
			for(int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
			{
				SinglePoly *pInPoly			= _pPoly->get(i);
				SinglePoly *pOutPoly		= (*_pPolyOut)->get(i);
				double *pInPolyI	= pInPoly->getCoef()->getImg();
				double *pOutPolyI	= pOutPoly->getCoef()->getImg();

				pOutPolyI[0]		=  (pInPolyI == NULL ? 0 : pInPolyI[0]) - (pInDblI == NULL ? 0 : pInDblI[0]);
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
			double *pInPolyI	= pInPoly->getCoef()->getImg();
			double *pOutPolyR	= pOutPoly->getCoef()->getReal();
			double *pOutPolyI	= pOutPoly->getCoef()->getImg();

			pOutPolyR[0] = pInPolyR[0] - pInDblR[i];

			for(int j = 1 ; j < pInPoly->getRank() ; j++)
			{
				pOutPolyR[j] = pOutPolyR[j];
			}
		}

		if(_pPoly->isComplex() || _pDouble->isComplex())
		{
			(*_pPolyOut)->setComplex(true);
			for(int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
			{
				SinglePoly *pInPoly			= _pPoly->get(i);
				SinglePoly *pOutPoly		= (*_pPolyOut)->get(i);

				double *pInPolyI	= pInPoly->getCoef()->getImg();
				double *pOutPolyI	= pOutPoly->getCoef()->getImg();

				pOutPolyI[0] = (pInPolyI != NULL ? pInPolyI[0] : 0) - (pInDblI != NULL ? pInDblI[i] : 0);

				for(int j = 1 ; j < pInPoly->getRank() ; j++)
				{
					pOutPolyI[j] = pOutPolyI[j];
				}
			}
		}
	}
	else if(_pPoly->getSize() == 1)
	{
		int *piRank = new int[_pDouble->getSize()];
		for(int i = 0 ; i < _pDouble->getSize() ; i++)
		{
			piRank[i] = _pPoly->get(0)->getRank();
		}

		(*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pDouble->getRows(), _pDouble->getCols(), piRank);

		for(int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
		{
			SinglePoly *pInPoly			= _pPoly->get(0);
			SinglePoly *pOutPoly		= (*_pPolyOut)->get(i);

			double *pInPolyR	= pInPoly->getCoef()->getReal();
			double *pInPolyI	= pInPoly->getCoef()->getImg();
			double *pOutPolyR	= pOutPoly->getCoef()->getReal();
			double *pOutPolyI	= pOutPoly->getCoef()->getImg();

			pOutPolyR[0] = pInPolyR[0] - pInDblR[i];

			for(int j = 1 ; j < pOutPoly->getRank() ; j++)
			{
				pOutPolyR[j] = pInPolyR[j];
			}
		}

		if(_pPoly->isComplex() || _pDouble->isComplex())
		{
			(*_pPolyOut)->setComplex(true);
			for(int i = 0 ; i < (*_pPolyOut)->getSize() ; i++)
			{
				SinglePoly *pInPoly			= _pPoly->get(0);
				SinglePoly *pOutPoly		= (*_pPolyOut)->get(i);

				double *pInPolyI	= pInPoly->getCoef()->getImg();
				double *pOutPolyI	= pOutPoly->getCoef()->getImg();

				pOutPolyI[0] = (pInPolyI != NULL ? pInPolyI[0] : 0) - (pInDblI != NULL ? pInDblI[i] : 0);

				for(int j = 1 ; j < pOutPoly->getRank() ; j++)
				{
					pOutPolyI[j] = pInPolyI[j];
				}
			}
		}
	}
	else
	{
		return 1;
	}
	return 0;
}

int SubstractPolyToPoly(Polynom *_pPoly1, Polynom *_pPoly2, Polynom **_pPolyOut)
{
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

		//Result P1(i) + P2(i)
		for(int i = 0 ; i < _pPoly1->getSize() ; i++)
		{
			Double *pCoef1	= _pPoly1->get(i)->getCoef();
			double *p1R			= pCoef1->getReal();

			Double *pCoef2	= _pPoly2->get(i)->getCoef();
			double *p2R			= pCoef2->getReal();

			Double *pCoefR	= (*_pPolyOut)->get(i)->getCoef();
			double *pRR			= pCoefR->getReal();

			for(int j = 0 ; j < Min(pRank1[i], pRank2[i]) ; j++)
			{
				pRR[j] = p1R[j] - p2R[j];
			}

			double *pTemp = NULL;
			int iCoef			= 1;
			if(pRank1[i] > pRank2[i])
			{
				pTemp = p1R;
				iCoef = 1;
			}
			else
			{
				pTemp = p2R;
				iCoef = -1;
			}
			for(int j = Min(pRank1[i], pRank2[i]) ; j < Max(pRank1[i], pRank2[i]) ; j++)
			{
				pRR[j] = pTemp[j] * iCoef;
			}

			if((*_pPolyOut)->isComplex())
			{
				double *p1I			= pCoef1->getImg();
				double *p2I			= pCoef2->getImg();
				double *pRI			= pCoefR->getImg();

				for(int j = 0 ; j < Min(pRank1[i], pRank2[i]) ; j++)
				{
					pRI[j] = (p1I == NULL ? 0 : p1I[j]) - (p2I == NULL ? 0 : p2I[j]);
				}

				double *pTemp = NULL;
				int iCoef			= 1;
				if(pRank1[i] > pRank2[i])
				{
					pTemp = p1I;
					iCoef = 1;
				}
				else
				{
					pTemp = p2I;
					iCoef = -1;
				}
				for(int j = Min(pRank1[i], pRank2[i]) ; j < Max(pRank1[i], pRank2[i]) ; j++)
				{
					pRR[j] = pTemp[j] * iCoef;
				}
			}
		}

		delete[] pRank;
		delete[] pRank1;
		delete[] pRank2;
	}
	else if(_pPoly1->getSize() == 1)
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

		for(int i = 0 ; i < _pPoly2->getSize() ; i++)
		{
			Double *pCoef2	= _pPoly2->get(i)->getCoef();
			double *p2R			= pCoef2->getReal();

			Double *pCoefR	= (*_pPolyOut)->get(i)->getCoef();
			double *pRR			= pCoefR->getReal();

			for(int j = 0 ; j < pRank[i] ; j++)
			{
				pRR[j] = p1R[j] - p2R[j];
			}

			if((*_pPolyOut)->isComplex())
			{
				double *p1I			= pCoef1->getImg();
				double *p2I			= pCoef2->getImg();
				double *pRI			= pCoefR->getImg();

				for(int j = 0 ; j < pRank[i] ; j++)
				{
					pRI[j] = (p1I == NULL ? 0 : p1I[j]) - (p2I == NULL ? 0 : p2I[j]);
				}
			}
		}

		delete[] pRank;
		delete[] pRank1;
		delete[] pRank2;
	}
	else if(_pPoly2->getSize() == 1)
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

		for(int i = 0 ; i < _pPoly1->getSize() ; i++)
		{
			Double *pCoef1	= _pPoly1->get(i)->getCoef();
			double *p1R			= pCoef1->getReal();

			Double *pCoefR	= (*_pPolyOut)->get(i)->getCoef();
			double *pRR			= pCoefR->getReal();

			for(int j = 0 ; j < pRank[i] ; j++)
			{
				pRR[j] = p1R[j] - p2R[j];
			}

			if((*_pPolyOut)->isComplex())
			{
				double *p2I			= pCoef2->getImg();
				double *p1I			= pCoef1->getImg();
				double *pRI			= pCoefR->getImg();
				for(int j = 0 ; j < pRank[i] ; j++)
				{
					pRI[j] = (p1I == NULL ? 0 : p1I[j]) - (p2I == NULL ? 0 : p2I[j]);
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

	return 0;
}

