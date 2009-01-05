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
extern "C"
{
	#include "matrix_right_division.h"
}

int DivideDoubleByDouble(Double *_pDouble1, Double *_pDouble2, Double *_pDoubleOut)
{
	bool bComplex1		= _pDouble1->isComplex();
	bool bComplex2		= _pDouble2->isComplex();
	bool bScalar1			= _pDouble1->rows_get() == 1 && _pDouble1->cols_get() == 1;
	bool bScalar2			= _pDouble2->rows_get() == 1 && _pDouble2->cols_get() == 1;
	int	iSize1				= _pDouble1->size_get();
	int	iSize2				= _pDouble2->size_get();
	int iRet					= 0;

	std::cout << "Double1 : " << _pDouble1->toString(10,100) << std::endl;
	std::cout << "Double2 : " << _pDouble2->toString(10,100) << std::endl;
	//check finite values of _pDouble1
	if(isDoubleFinite(_pDouble1) == false)
	{
		std::cout << "isDoubleFinite(_pDouble1) == false" << std::endl;
		return 2;
	}

	//check finite values of _pDouble1
	if(isDoubleFinite(_pDouble2) == false)
	{
		std::cout << "isDoubleFinite(_pDouble2) == false" << std::endl;
		return 2;
	}

	if(bScalar2)
	{//Divide by a scalar

	}
	else
	{
		//Divide by a matrix or vector
		if(bComplex1 == false && bComplex2 == false)
		{
			double dblRcond = 0;
			iRet = iRightDivisionOfRealMatrix(
					_pDouble1->real_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
					_pDouble2->real_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
					_pDoubleOut->real_get(), _pDouble2->rows_get(), _pDouble1->rows_get(), &dblRcond);

			std::cout << "DoubleOut : " << _pDoubleOut->toString(10,100) << std::endl;
			std::cout << "dblRcond : " << dblRcond << std::endl;
		}
		else //bComplex1 == true || bComplex2 == true
		{
/*				double dblRcond = 0;
				iRet = iRightDivisionOfComplexMatrix(
					pReal1, pImg1, iRows1, iCols1,
					pReal2, pImg2, iRows2, iCols2,
					pReturnReal, pReturnImg, iRows1, iRows2, &dblRcond);
*/		}
	}
	return 0;
}


