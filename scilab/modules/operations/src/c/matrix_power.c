/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "matrix_power.h"
#include "matrix_multiplication.h"
//#include "unsfdcopy.h"

#include "elementary_functions.h"
#include "elem_common.h"

/*
r : real part
c : imaginary part
E : element of
R : real
Z : integer
C : complex
* : less 0
+ : >= 0
- : <= 0
*/

/*ddpowe*/
int iPowerRealScalarByRealScalar(
		double _dblReal1, 
		double _dblReal2, 
		double *_pdblRealOut, double *_pdblImgOut, int *_piComplex)
{
	//exposant is an integer
	if((int)_dblReal2 == _dblReal2)
	{//dipowe
		int iReal2 = (int)_dblReal2;
		
		if(iReal2 == 1)
		{//R ^ 1
			*_pdblRealOut = _dblReal1;
			*_pdblImgOut	= 0;
			*_piComplex		= 0;
		}
		else if(iReal2 == 0)
		{//R ^ 0
			*_pdblRealOut = 1;
			*_pdblImgOut	= 0;
			*_piComplex		= 0;
		}
		else if(iReal2 < 0)
		{//R ^ Z-
			if(_dblReal1 != 0)
			{//R* ^ Z-
				*_pdblRealOut = pow(_dblReal1, iReal2);
				*_pdblImgOut	= 0;
				*_piComplex		= 0;
			}
			else
			{//0 ^ 0
				//FIXME : ieee 
				//generate +Inf
				double dblZero	= 0.0;
				*_pdblRealOut		= 1.0/(dblZero);
				*_pdblImgOut		= 0;
				*_piComplex			= 0;
			}
		}
		else
		{//R ^ Z*+ ( N* )
			*_pdblRealOut = pow(_dblReal1, iReal2);
			*_pdblImgOut	= 0;
			*_piComplex		= 0;
		}
	}
	else
	{
		if(_dblReal1 > 0)
		{//R*+ ^ R
			*_pdblRealOut = pow(_dblReal1, _dblReal2);
			*_pdblImgOut	= 0;
			*_piComplex		= 0;
		}
		else if(_dblReal1 < 0)
		{//R*- ^ R
			double dblRealTemp	= 0;
			double dblImgTemp		= 0;

			wlog(_dblReal1, 0, &dblRealTemp, &dblImgTemp);
			dblRealTemp					= dexps(dblRealTemp * _dblReal2);
			dblImgTemp					= dblImgTemp * _dblReal2;
			*_pdblRealOut				= dblRealTemp * dcoss(dblImgTemp);
			*_pdblImgOut				= dblRealTemp * dsins(dblImgTemp);
			*_piComplex					= 1;
		}
		else if(_dblReal1 == 0)
		{//0 ^ R
			if(_dblReal2 < 0)
			{//0 ^ R*-
				//FIXME : ieee 
				//generate +Inf
				double dblZero	= 0.0;
				*_pdblRealOut		= 1.0/(dblZero);
				*_pdblImgOut		= 0;
				*_piComplex			= 0;
			}
			else if(_dblReal2 == 0)
			{//0 ^ 0
				//never call, pass in R ** 0 before
				*_pdblRealOut		= 1;
				*_pdblImgOut		= 0;
				*_piComplex			= 0;
			}
			else if(_dblReal2 > 0)
			{//0 ^ R*+
				*_pdblRealOut		= 0;
				*_pdblImgOut		= 0;
				*_piComplex			= 0;
			}
			else //_dblReal2 is NaN
			{
				*_pdblRealOut		= _dblReal2;
				*_pdblImgOut		= 0;
				*_piComplex			= 0;
			}
		}
		else //_dblReal1 is NaN
		{
			*_pdblRealOut		= _dblReal1;
			*_pdblImgOut		= 0;
			*_piComplex			= 0;
		}
	}
	return 0;
}

/*dwpowe*/
int iPowerRealScalarByComplexScalar(
		double _dblReal1, 
		double _dblReal2, double _dblImg2, 
		double* _pdblRealOut, double* _pdblImgOut)
{
	if(_dblImg2 == 0)
	{//R ^ R
		int iComplex = 0;
		iPowerRealScalarByRealScalar(
			_dblReal1, 
			_dblReal2, 
			_pdblRealOut, _pdblImgOut, &iComplex);
	}
	else
	{//R ^ C
		if(_dblReal1 != 0)
		{//R* ^ C
			double dblRealTemp	= 0;
			double dblImgTemp		= 0;

			wlog(_dblReal1, 0, &dblRealTemp, &dblImgTemp);
			C2F(wmul)(&dblRealTemp, &dblImgTemp, &_dblReal2, &_dblImg2, &dblRealTemp, &dblImgTemp);
			dblRealTemp					= dexps(dblRealTemp);
			*_pdblRealOut				= dblRealTemp * dcoss(dblImgTemp);
			*_pdblImgOut				= dblRealTemp * dsins(dblImgTemp);
		}
		else
		{//0 ^ C
			if(_dblReal2 > 0)
			{//0 ^ (r E R*+ ) & ( c E R )
				*_pdblRealOut				= 0;
				*_pdblImgOut				= 0;
			}
			else if(_dblReal2 < 0)
			{//0 ^ (r E R*- ) & ( c E R )
				//FIXME : ieee 
				//generate +Inf
				double dblZero	= 0.0;
				*_pdblRealOut		= 1.0/(dblZero);
				*_pdblImgOut		= 0;
			}
			else //_dblReal2 == 0, NaN
			{//0 ^ (r = 0 ) & ( c E R )
				*_pdblRealOut				= 1;
				*_pdblImgOut				= 0;
			}
		}
	}
	return 0;
}

/*wdpowe*/
int iPowerComplexScalarByRealScalar(
		double _dblReal1, double _dblImg1, 
		double _dblReal2, 
		double* _pdblRealOut, double* _pdblImgOut)
{
	if((int)_dblReal2 == _dblReal2)
	{//C ^ Z
		if(_dblReal2 == 0)
		{//C ^ 0
			*_pdblRealOut				= 1;
			*_pdblImgOut				= 0;
		}
		else if(_dblReal2 < 0)
		{//C ^ Z*-
			if(dabss(_dblReal1) + dabss(_dblImg1) != 0) //_dblReal1 != 0 || _dblImg1 != 0 ?
			{
				int i = 0;
				double dblOne				= 1;
				double dblZero			= 0;
				double dblRealTemp	= 0;
				double dblImgTemp		= 0;

				C2F(wdiv)(&dblOne, &dblZero, &_dblReal1, &_dblImg1, _pdblRealOut, _pdblImgOut);

				dblRealTemp					= *_pdblRealOut;
				dblImgTemp					= *_pdblImgOut;

				for(i = 2 ; i <= dabss(_dblReal2) ; i++)
				{
					C2F(wmul)(&dblRealTemp, &dblImgTemp, _pdblRealOut, _pdblImgOut, _pdblRealOut, _pdblImgOut);
				}
			}
			else
			{
				//FIXME : ieee 
				//generate +Inf
				double dblZero	= 0.0;
				*_pdblRealOut		= 1.0/(dblZero);
				*_pdblImgOut		= 0;
			}
		}
		else
		{//C ^ Z*+
			int i								= 0;
			double dblRealTemp	= 0;
			double dblImgTemp		= 0;

			*_pdblRealOut			= _dblReal1;
			*_pdblImgOut			= _dblImg1;
			dblRealTemp				= *_pdblRealOut;
			dblImgTemp				= *_pdblImgOut;

			for(i = 2 ; i <= dabss(_dblReal1) ; i++)
			{
				C2F(wmul)(&dblRealTemp, &dblImgTemp, _pdblRealOut, _pdblImgOut, _pdblRealOut, _pdblImgOut);
			}
		}
	}
	else
	{
		if(dabss(_dblReal1) + dabss(_dblImg1) != 0)
		{//C ^ R
			double dblRealTemp	= 0;
			double dblImgTemp		= 0;

			wlog(_dblReal1, _dblImg1, &dblRealTemp, &dblImgTemp);
			dblRealTemp					= dexps(dblRealTemp * _dblReal2);
			dblImgTemp					= dblImgTemp * _dblReal2;
			*_pdblRealOut				= dblRealTemp * dcoss(dblImgTemp);
			*_pdblImgOut				= dblRealTemp * dsins(dblImgTemp);
		}
		else
		{
			if(_dblReal2 > 0)
			{//0 ^ R*+
				*_pdblRealOut		= 0;
				*_pdblImgOut		= 0;
			}
			else if(_dblReal2 < 0)
			{//0 ^ R*-
				//FIXME : ieee 
				//generate +Inf
				double dblZero	= 0.0;
				*_pdblRealOut		= 1.0/(dblZero);
				*_pdblImgOut		= 0;
			}
			else
			{//0 ^ 0
				*_pdblRealOut		= 1;
				*_pdblImgOut		= 0;
			}
		}
	}
	return 0;
}

/*wwpowe*/
int iPowerComplexScalarByComplexScalar(
		double _dblReal1, double _dblImg1, 
		double _dblReal2, double _dblImg2, 
		double* _pdblRealOut, double* _pdblImgOut)
{
	if(_dblImg2 == 0)
	{//C ^ R
		iPowerComplexScalarByRealScalar(
				_dblReal1, _dblImg1, 
				_dblReal2, 
				_pdblRealOut, _pdblImgOut);
	}
	else
	{//C ^ C
		if(dabss(_dblReal1) + dabss(_dblImg1) != 0)
		{// ! 0 ^ C
			double dblRealTemp	= 0;
			double dblImgTemp		= 0;

			wlog(_dblReal1, _dblImg1, &dblRealTemp, &dblImgTemp);
			C2F(wmul)(&dblRealTemp, &dblImgTemp, &_dblReal2, &_dblImg2, &dblRealTemp, &dblImgTemp);
			dblRealTemp					= dexps(dblRealTemp);
			*_pdblRealOut				= dblRealTemp * dcoss(dblImgTemp);
			*_pdblImgOut				= dblRealTemp * dsins(dblImgTemp);
		}
		else
		{// 0 ^ C
			//FIXME : ieee 
			//generate +Inf
			double dblZero	= 0.0;
			*_pdblRealOut		= 1.0/(dblZero);
			*_pdblImgOut		= 0;
		}
	}
	return 0;
}

/*s .^ []*/
int iPowerRealScalarByRealMatrix(
		double _dblReal1,
		double* _pdblReal2, int _iRows2, int _iCols2,
		double* _pdblRealOut,	double* _pdblImgOut, int *_iComplex)
{
	int i = 0;
	for(i = 0 ; i < _iRows2 * _iCols2 ; i++)
	{
		int iComplex = 0;
		iPowerRealScalarByRealScalar(
				_dblReal1, _pdblReal2[i], &_pdblRealOut[i], &_pdblImgOut[i], &iComplex);
		*_iComplex |= iComplex;
	}
	return 0;
}


int iPowerComplexScalarByRealMatrix(
		double _dblReal1, double _dblImg1,
		double* _pdblReal2, int _iRows2, int _iCols2,
		double* _pdblRealOut,	double* _pdblImgOut)
{
	int i = 0;
	for(i = 0 ; i < _iRows2 * _iCols2 ; i++)
	{
		iPowerComplexScalarByRealScalar(
				_dblReal1, _dblImg1, _pdblReal2[i], &_pdblRealOut[i], &_pdblImgOut[i]);
	}
	return 0;
}

int iPowerRealScalarByComplexMatrix(
		double _dblReal1,
		double* _pdblReal2, double* _pdblImg2, int _iRows2, int _iCols2,
		double* _pdblRealOut,	double* _pdblImgOut)
{
	int i = 0;
	for(i = 0 ; i < _iRows2 * _iCols2 ; i++)
	{
		iPowerRealScalarByComplexScalar(
				_dblReal1, _pdblReal2[i], _pdblImg2[i], &_pdblRealOut[i], &_pdblImgOut[i]);
	}
	return 0;
}

int iPowerComplexScalarByComplexMatrix(
		double _dblReal1, double _dblImg1,
		double* _pdblReal2, double* _pdblImg2, int _iRows2, int _iCols2,
		double* _pdblRealOut,	double* _pdblImgOut)
{
	int i = 0;
	for(i = 0 ; i < _iRows2 * _iCols2 ; i++)
	{
		iPowerComplexScalarByComplexScalar(
				_dblReal1, _dblImg1, _pdblReal2[i], _pdblImg2[i], &_pdblRealOut[i], &_pdblImgOut[i]);
	}
	return 0;
}

//Square Matrix ^ Scalar 
int iPowerRealSquareMatrixByRealScalar(
		double* _pdblReal1, int _iRows1, int _iCols1,
		double _dblReal2,
		double* _pdblRealOut,	double* _pdblImgOut, int *_iComplex)
{
	int iExpRef = (int)_dblReal2;
	if((int)_dblReal2 == _dblReal2) //integer exponent
	{
		if(iExpRef == 1)
		{
			int iSize = _iRows1 * _iCols1;
			int iOne = 1;
			C2F(dcopy)(&iSize, _pdblReal1, &iOne, _pdblRealOut, &iOne);
		}
		else if(iExpRef == 0)
		{
			int iSize				= _iRows1 * _iCols1;
			int iOne				= 1;
			double dblOne		= 1;
			double dblZero	= 0;
			int iRowp1			= _iRows1 + 1;

			if(C2F(dasum)(&iSize, _pdblReal1, &iOne) == 0)
			{//Invalid exponent
				return 1;
			}
			C2F(dset)(&iSize, &dblZero, _pdblRealOut, &iOne);
			C2F(dset)(&_iRows1, &dblOne, _pdblRealOut, &iRowp1);
		}
		else
		{
			int iExp	= 0;
			int iRow	= 0;
			int iCol	= 0;
			int iSize	= _iRows1 * _iCols1;
			int iOne	= 1;

			/*temporary work space*/
			double *pWork2 = (double*)malloc(sizeof(double) * iSize);
			double *pWork3 = (double*)malloc(sizeof(double) * _iRows1);

			if(iExpRef < 0)
			{
				//call matrix invetion

				iExpRef = - iExpRef;
			}
			
			C2F(dcopy)(&iSize, _pdblReal1, &iOne, _pdblRealOut, &iOne);
			C2F(dcopy)(&iSize, _pdblReal1, &iOne, pWork2, &iOne);
			//l1 -> l2
			for(iExp = 1 ; iExp < iExpRef ; iExp++)
			{
				for(iCol = 0 ; iCol < _iCols1 ; iCol++)
				{
					double *pPtr = _pdblRealOut + iCol * _iCols1;
					C2F(dcopy)(&_iRows1, pPtr, &iOne, pWork3, &iOne);
					//ls -> l3
					for(iRow = 0 ; iRow < _iRows1 ; iRow++)
					{
						int iOffset = iRow + iCol * _iRows1;
						pPtr = pWork2 + iRow;
						_pdblRealOut[iOffset] = C2F(ddot)(&_iRows1, pPtr, &_iRows1, pWork3, &iOne);
					}//for
				}//for
			}//for
			free(pWork2);
			free(pWork3);
		}//if(iExpRef != 1 && != 0)
	}
	else
	{//floating point exponent
	}

	*_iComplex = 0;
	return 0;
}

EXTERN_OP int iPowerRealSquareMatrixByComplexScalar(
		double* _pdblReal1, int _iRows1, int _iCols1,
		double _dblReal2, double _dblImg2,
		double* _pdblRealOut,	double* _pdblImgOut)
{
	return 0;
}

EXTERN_OP int iPowerComplexSquareMatrixByRealScalar(
		double* _pdblReal1, double* _pdblImg1, int _iRows1, int _iCols1,
		double _dblReal2,
		double* _pdblRealOut,	double* _pdblImgOut)
{
	int iExpRef = (int)_dblReal2;
	if((int)_dblReal2 == _dblReal2) //integer exponent
	{
		if(iExpRef == 1)
		{
			int iSize = _iRows1 * _iCols1;
			int iOne = 1;
			C2F(dcopy)(&iSize, _pdblReal1, &iOne, _pdblRealOut, &iOne);
		}
		else if(iExpRef == 0)
		{
			int iSize				= _iRows1 * _iCols1;
			int iOne				= 1;
			double dblOne		= 1;
			double dblZero	= 0;
			int iRowp1			= _iRows1 + 1;

			if(C2F(dasum)(&iSize, _pdblReal1, &iOne) == 0)
			{//Invalid exponent
				return 1;
			}
			C2F(dset)(&iSize, &dblZero, _pdblRealOut, &iOne);
			C2F(dset)(&_iRows1, &dblOne, _pdblRealOut, &iRowp1);
		}
		else
		{
/*			int iSize	= _iRows1 * _iCols1;
			int iOne	= 1;
			int iExp	= 0;

//			C2F(dcopy)(&iSize, _pdblReal1, &iOne, _pdblRealOut, &iOne);
//			C2F(unsfdcopy)(&iSize, _pdblReal1, &iOne, _pdblRealOut, &iOne);
			memcpy(_pdblRealOut, _pdblReal1, iSize*sizeof(double));

			for(iExp = 1 ; iExp < iExpRef ; iExp++)
			{
				iMultiRealMatrixByRealMatrix(
					_pdblRealOut, _iRows1, _iCols1,
					_pdblRealOut, _iRows1, _iCols1,
					_pdblRealOut);
			}
*/
			int iSize	= _iRows1 * _iCols1;
			int iExp	= 0;
			int iRow	= 0;
			int iCol	= 0;
			int iOne	= 1;

			//temporary work space
			double *pWorkReal2	= (double*)malloc(sizeof(double) * iSize);
			double *pWorkImg2		= (double*)malloc(sizeof(double) * iSize);
			double *pWorkReal3	= (double*)malloc(sizeof(double) * _iRows1);
			double *pWorkImg3		= (double*)malloc(sizeof(double) * _iRows1);

			if(iExpRef < 0)
			{
				//call matrix invetion

				iExpRef = - iExpRef;
			}

			//copy In to Out
			C2F(dcopy)(&iSize, _pdblReal1,	&iOne, _pdblRealOut,	&iOne);
			C2F(dcopy)(&iSize, _pdblImg1,		&iOne, _pdblImgOut,		&iOne);

			C2F(dcopy)(&iSize, _pdblReal1,	&iOne, pWorkReal2,		&iOne);
			C2F(dcopy)(&iSize, _pdblImg1,		&iOne, pWorkImg2,			&iOne);

			//l1 -> l2
			for(iExp = 1 ; iExp < iExpRef ; iExp++)
			{
				for(iCol = 0 ; iCol < _iCols1 ; iCol++)
				{
					double *pPtrReal = _pdblRealOut + iCol * _iCols1;
					double *pPtrImg	 = _pdblImgOut + iCol * _iCols1;
					C2F(dcopy)(&_iRows1, pPtrReal, &iOne, pWorkReal3, &iOne);
					C2F(dcopy)(&_iRows1, pPtrImg, &iOne, pWorkImg3, &iOne);
					//ls -> l3
					for(iRow = 0 ; iRow < _iRows1 ; iRow++)
					{
						int iOffset = iRow + iCol * _iRows1;
						pPtrReal = pWorkReal2 + iRow;
						pPtrImg = pWorkImg2 + iRow;

						_pdblRealOut[iOffset] = C2F(ddot)(&_iRows1, pPtrReal, &_iRows1, pWorkReal3, &iOne)
																	- C2F(ddot)(&_iRows1, pPtrImg,	&_iRows1, pWorkImg3,	&iOne);
						_pdblImgOut[iOffset]	= C2F(ddot)(&_iRows1, pPtrReal, &_iRows1, pWorkImg3, &iOne)
																	+ C2F(ddot)(&_iRows1, pPtrImg,	&_iRows1, pWorkReal3,	&iOne);
					}//for
				}//for
			}//for
			free(pWorkReal2);
			free(pWorkImg2);
			free(pWorkReal3);
			free(pWorkImg3);
		
		}//if(iExpRef != 1 && != 0)
	}
	else
	{//floating point exponent
	}
	return 0;
}

EXTERN_OP int iPowerComplexSquareMatrixByComplexScalar(
		double* _pdblReal1, double* _pdblImg1, int _iRows1, int _iCols1,
		double _dblReal2, double _dblImg2,
		double* _pdblRealOut,	double* _pdblImgOut)
{
	return 0;
}
