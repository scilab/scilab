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

#include "types_multiplication.hxx"

extern "C"
{
	#include "matrix_multiplication.h"
}

Double* MultiplyDoubleByDouble(Double* _pDouble1, Double* _pDouble2)
{
	bool bComplex1		= _pDouble1->isComplex();
	bool bComplex2		= _pDouble2->isComplex();
	bool bScalar1			= _pDouble1->rows_get() == 1 && _pDouble1->cols_get() == 1;
	bool bScalar2			= _pDouble2->rows_get() == 1 && _pDouble2->cols_get() == 1;


	//Output variables
	Double *pResult		= NULL;
	int iRowsOut			= 0;
	int iColsOut			= 0;
	bool bComplexOut	= bComplex1 || bComplex2;
	double *pReal			= NULL;
	double *pImg			= NULL;

	if(bScalar1)
	{//cst*b
		iRowsOut				= _pDouble2->rows_get();
		iColsOut				= _pDouble2->cols_get();
		if(bComplexOut)
		{
			pResult 			= new Double(iRowsOut, iColsOut, &pReal, &pImg);
		}
		else
		{
			pResult 			= new Double(iRowsOut, iColsOut, &pReal);
		}

		if(bComplex1 == false && bComplex2 == false)
		{
			iMultiRealScalarByRealMatrix(_pDouble1->real_get(0,0), _pDouble2->real_get(), iRowsOut, iColsOut, pReal);
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			iMultiRealScalarByComplexMatrix(_pDouble1->real_get(0,0), _pDouble2->real_get(), _pDouble2->img_get(), iRowsOut, iColsOut, pReal, pImg);
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iMultiComplexScalarByRealMatrix(_pDouble1->real_get(0,0), _pDouble1->img_get(0,0), _pDouble2->real_get(), iRowsOut, iColsOut, pReal, pImg);
		}
		else //if(bComplex1 == true && bComplex2 == true)
		{
			iMultiComplexScalarByComplexMatrix(_pDouble1->real_get(0,0), _pDouble1->img_get(0,0), _pDouble2->real_get(), _pDouble2->img_get(), iRowsOut, iColsOut, pReal, pImg);
		}
	}
	else if(bScalar2)
	{//a * cst
		iRowsOut				= _pDouble1->rows_get();
		iColsOut				= _pDouble1->cols_get();
		
		if(bComplexOut)
		{
			pResult 			= new Double(iRowsOut, iColsOut, &pReal, &pImg);
		}
		else
		{
			pResult 			= new Double(iRowsOut, iColsOut, &pReal);
		}
		
		if(bComplex1 == false && bComplex2 == false)
		{//Real Matrix by Real Scalar
			iMultiRealScalarByRealMatrix(_pDouble2->real_get(0,0), _pDouble1->real_get(), iRowsOut, iColsOut, pReal);
		}
		else if(bComplex1 == false && bComplex2 == true)
		{//Real Matrix by Scalar Complex
			iMultiComplexScalarByRealMatrix(_pDouble2->real_get(0,0), _pDouble2->img_get(0,0), _pDouble1->real_get(), iRowsOut, iColsOut, pReal, pImg);
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iMultiRealScalarByComplexMatrix(_pDouble2->real_get(0,0), _pDouble1->real_get(), _pDouble1->img_get(), iRowsOut, iColsOut, pReal, pImg);
		}
		else //if(bComplex1 == true && bComplex2 == true)
		{
			iMultiComplexScalarByComplexMatrix(_pDouble2->real_get(0,0), _pDouble2->img_get(0,0), _pDouble1->real_get(), _pDouble1->img_get(), iRowsOut, iColsOut, pReal, pImg);
		}
	}
	else
	{//a * b
		iRowsOut				= _pDouble1->rows_get();
		iColsOut				= _pDouble2->cols_get();
		
		if(bComplexOut)
		{
			pResult 			= new Double(iRowsOut, iColsOut, &pReal, &pImg);
		}
		else
		{
			pResult 			= new Double(iRowsOut, iColsOut, &pReal);
		}

		if(bComplex1 == false && bComplex2 == false)
		{//Real Matrix by Real Matrix
			iMultiRealMatrixByRealMatrix(
									_pDouble1->real_get(), _pDouble1->rows_get(), _pDouble1->cols_get(), 
									_pDouble2->real_get(), _pDouble2->rows_get(), _pDouble2->cols_get(), 
									pReal);
		}
		else if(bComplex1 == false && bComplex2 == true)
		{//Real Matrix by Matrix Complex
			iMultiRealMatrixByComplexMatrix(
									_pDouble1->real_get(), _pDouble1->rows_get(), _pDouble1->cols_get(), 
									_pDouble2->real_get(), _pDouble2->img_get(), _pDouble2->rows_get(), _pDouble2->cols_get(), 
									pReal, pImg);
		}
		else if(bComplex1 == true && bComplex2 == false)
		{//Complex Matrix by Real Matrix
			iMultiComplexMatrixByRealMatrix(
									_pDouble1->real_get(), _pDouble1->img_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
									_pDouble2->real_get(), _pDouble2->rows_get(), _pDouble2->cols_get(), 
									pReal, pImg);
		}
		else //if(bComplex1 == true && bComplex2 == true)
		{//Complex Matrix by Complex Matrix
			iMultiComplexMatrixByComplexMatrix(
									_pDouble1->real_get(), _pDouble1->img_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
									_pDouble2->real_get(), _pDouble2->img_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
									pReal, pImg);
		}	
	}

	return pResult;
}

