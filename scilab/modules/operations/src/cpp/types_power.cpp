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

#include "types_power.hxx"

extern "C"
{
	#include "operation_f.h"
	#include "matrix_power.h"
}

int PowerDoubleByDouble(Double* _pDouble1, Double* _pDouble2, Double** _pDoubleOut)
{
	bool bComplex1		= _pDouble1->isComplex();
	bool bComplex2		= _pDouble2->isComplex();
	bool bScalar1			= _pDouble1->getRows() == 1 && _pDouble1->getCols() == 1;
	bool bScalar2			= _pDouble2->getRows() == 1 && _pDouble2->getCols() == 1;

	int iComplex = 1;

	if(bScalar1 && bScalar2)
	{//s ^ s
		*_pDoubleOut = new Double(1,1 , true);

		if(bComplex1 == false && bComplex2 == false)
		{
			iPowerRealScalarByRealScalar(
				_pDouble1->getReal()[0], 
				_pDouble2->getReal()[0],
				(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), &iComplex);
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			iPowerRealScalarByComplexScalar(
				_pDouble1->getReal()[0], 
				_pDouble2->getReal()[0], _pDouble2->getImg()[0],
				(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iPowerComplexScalarByRealScalar(
				_pDouble1->getReal()[0], _pDouble1->getImg()[0], 
				_pDouble2->getReal()[0],
				(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			iPowerComplexScalarByComplexScalar(
				_pDouble1->getReal()[0], _pDouble1->getImg()[0],
				_pDouble2->getReal()[0], _pDouble2->getImg()[0],
				(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
	}
	else if(bScalar1)
	{//s ^ []
		*_pDoubleOut = new Double(_pDouble2->getRows(), _pDouble2->getCols(), true);

		if(bComplex1 == false && bComplex2 == false)
		{
			iPowerRealScalarByRealMatrix(
				_pDouble1->getReal()[0], 
				_pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getCols(),
				(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), &iComplex);
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			iPowerRealScalarByComplexMatrix(
				_pDouble1->getReal()[0], 
				_pDouble2->getReal(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
				(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iPowerComplexScalarByRealMatrix(
				_pDouble1->getReal()[0], _pDouble1->getImg()[0], 
				_pDouble2->getReal(), _pDouble2->getRows(), _pDouble2->getCols(),
				(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			iPowerComplexScalarByComplexMatrix(
				_pDouble1->getReal()[0], _pDouble1->getImg()[0], 
				_pDouble2->getReal(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
				(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
	}
	else if(bScalar2 && ( _pDouble1->getRows() == 1 || _pDouble1->getCols() == 1 ))
	{//_pDouble1 is a vector and _pDouble is a scalar
		*_pDoubleOut = new Double(_pDouble1->getRows(),_pDouble1->getCols() , true);
		
		if(bComplex1 == false && bComplex2 == false)
		{
			for(int i = 0 ; i < (*_pDoubleOut)->getSize() ; i++)
			{
				iPowerRealScalarByRealScalar(
					_pDouble1->getReal()[i], 
					_pDouble2->getReal()[0],
					&(*_pDoubleOut)->getReal()[i], &(*_pDoubleOut)->getImg()[i], &iComplex);
			}
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			for(int i = 0 ; i < (*_pDoubleOut)->getSize() ; i++)
			{
				iPowerRealScalarByComplexScalar(
					_pDouble1->getReal()[i], 
					_pDouble2->getReal()[0], _pDouble2->getImg()[0],
					&(*_pDoubleOut)->getReal()[i], &(*_pDoubleOut)->getImg()[i]);
			}
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			for(int i = 0 ; i < (*_pDoubleOut)->getSize() ; i++)
			{
			iPowerComplexScalarByRealScalar(
				_pDouble1->getReal()[i], _pDouble1->getImg()[i], 
				_pDouble2->getReal()[0],
					&(*_pDoubleOut)->getReal()[i], &(*_pDoubleOut)->getImg()[i]);
			}
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			for(int i = 0 ; i < (*_pDoubleOut)->getSize() ; i++)
			{
			iPowerComplexScalarByComplexScalar(
				_pDouble1->getReal()[i], _pDouble1->getImg()[i],
				_pDouble2->getReal()[0], _pDouble2->getImg()[0],
					&(*_pDoubleOut)->getReal()[i], &(*_pDoubleOut)->getImg()[i]);
			}
		}
	}
	else if(bScalar2 && ( _pDouble1->getRows() == _pDouble1->getCols()))
	{//power of a square matrix by a scalar exponent.
		*_pDoubleOut = new Double(_pDouble1->getRows(),_pDouble1->getCols() , true);
		if(bComplex1 == false && bComplex2 == false)
		{
			iPowerRealSquareMatrixByRealScalar(
				_pDouble1->getReal(), _pDouble1->getRows(), _pDouble1->getCols(),
				_pDouble2->getReal()[0], 
				(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg(), &iComplex);
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iPowerComplexSquareMatrixByRealScalar(
				_pDouble1->getReal(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
				_pDouble2->getReal()[0], 
				(*_pDoubleOut)->getReal(), (*_pDoubleOut)->getImg());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
		}
	}

	if(iComplex == 0)
	{
		(*_pDoubleOut)->setComplex(false);
	}
	return 0;
}

int PowerPolyByDouble(Polynom* _pPoly, Double* _pDouble, Polynom** _pPolyOut)
{
	return 0;
}