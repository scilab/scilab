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
#include "matrix_power.h"
#include "matrix_right_division.h"

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
		{//C ^ R*-
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

				for(i = 1 ; i < dabss(_dblReal2) ; i++)
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
		{//C ^ R+
			int i								= 0;
			double dblRealTemp	= 0;
			double dblImgTemp		= 0;

			*_pdblRealOut			= _dblReal1;
			*_pdblImgOut			= _dblImg1;
			dblRealTemp				= *_pdblRealOut;
			dblImgTemp				= *_pdblImgOut;

			for(i = 1 ; i < dabss(_dblReal1) ; i++)
			{
				C2F(wmul)(&dblRealTemp, &dblImgTemp, _pdblRealOut, _pdblImgOut, _pdblRealOut, _pdblImgOut);
			}
		}
	}
	else
	{//C ^ R
		if(dabss(_dblReal1) + dabss(_dblImg1) != 0)
		{
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
			{//C ^ R*+
				*_pdblRealOut		= 0;
				*_pdblImgOut		= 0;
			}
			else if(_dblReal2 < 0)
			{//C ^ R*-
				//FIXME : ieee 
				//generate +Inf
				double dblZero	= 0.0;
				*_pdblRealOut		= 1.0/(dblZero);
				*_pdblImgOut		= 0;
			}
			else
			{//C ^ 0
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
		double* _pdblRealOut,	double* _pdblImgOut)
{
	return 0;
}


int iPowerImgScalarByRealMatrix(
		double _dblReal1, double _dblImg1,
		double* _pdblReal2, int _iRows2, int _iCols2,
		double* _pdblRealOut,	double* _pdblImgOut)
{
	return 0;
}

int iPowerRealScalarByComplexMatrix(
		double _dblReal1,
		double* _pdblReal2, double* _pdblImg2, int _iRows2, int _iCols2,
		double* _pdblRealOut,	double* _pdblImgOut)
{
	return 0;
}

int iPowerComplexScalarByComplexMatrix(
		double _dblReal1, double _dblImg1,
		double* _pdblReal2, double* _pdblImg2, int _iRows2, int _iCols2,
		double* _pdblRealOut,	double* _pdblImgOut)
{
	return 0;
}
