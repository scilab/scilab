/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "elem_common.h"


/*sqrt*/
double dsqrts(double _dblVal)
{
    return sqrt(_dblVal);
}

/*sqrt complex*/
void zsqrts(double _dblRealIn, double _dblImgIn, double* _pdblRealOut, double *_pdblImgOut)
{
	double dblRMax		= getOverflowThreshold();
	double dblBRMin		= 2 * getUnderflowThreshold();

	if(_dblRealIn == 0)
	{//pure imaginary case
		if(dabss(_dblImgIn >= dblBRMin))
			*_pdblRealOut = dsqrts(0.5 * dabss(_dblImgIn));
		else
			*_pdblRealOut = dsqrts(dabss(_dblImgIn)) * dsqrts(0.5);

		*_pdblImgOut = dsigns(1, _dblImgIn) * *_pdblRealOut;
	}
	else if( dabss(_dblRealIn) <= dblRMax && dabss(_dblImgIn) <= dblRMax)
	{//standard case : a (not zero) and b are finite
		double dblTemp = dsqrts(2 * (dabss(_dblRealIn) + dpythags(_dblRealIn, _dblImgIn)));
		//overflow test
		if(dblTemp > dblRMax)
		{//*     handle (spurious) overflow by scaling a and b
			double dblRealTemp = _dblRealIn / 16;
			double dblImgTemp = _dblImgIn / 16;
			dblTemp = dsqrts(2 * (dabss(_dblRealIn) + dpythags(_dblRealIn, dblImgTemp)));
			if(dblRealTemp >= 0)
			{
				*_pdblRealOut	= 2 * dblTemp;
				*_pdblImgOut	= 4 * dblImgTemp / dblTemp;
			}
			else
			{
				*_pdblRealOut	= 4 * dabss(dblImgTemp) / dblTemp;
				*_pdblImgOut	= dsigns(2, dblImgTemp) * dblTemp;
			}
		}
		else if(_dblRealIn >= 0) //classic switch to get the stable formulas
		{
			*_pdblRealOut	= 0.5 * dblTemp;
			*_pdblImgOut	= _dblImgIn / dblTemp;
		}
		else
		{
			*_pdblRealOut	= dabss(_dblImgIn) / dblTemp;
			*_pdblImgOut	= dsigns(0.5, _dblImgIn) * dblTemp;
		}
	}
	else
	{
		//Here we treat the special cases where a and b are +- 00 or NaN. 
		//The following is the treatment recommended by the C99 standard
		//with the simplification of returning NaN + i NaN if the
		//the real part or the imaginary part is NaN (C99 recommends
		//something more complicated)

		if(ISNAN(_dblRealIn) == 1 || ISNAN(_dblImgIn) == 1)
		{//got NaN + i NaN
			*_pdblRealOut	= _dblRealIn + _dblImgIn;
			*_pdblImgOut	= *_pdblRealOut;
		}
		else if( dabss(_dblImgIn) > dblRMax)
		{//case a +- i oo -> result must be +oo +- i oo  for all a (finite or not)
			*_pdblRealOut	= dabss(_dblImgIn);
			*_pdblImgOut	= _dblImgIn;
		}
		else if(_dblRealIn < -dblRMax)
		{//here a is -Inf and b is finite
			*_pdblRealOut	= 0;
			*_pdblImgOut	= dsigns(1, _dblImgIn) * dabss(_dblRealIn);
		}
		else
		{//here a is +Inf and b is finite
			*_pdblRealOut	= _dblRealIn;
			*_pdblImgOut	= 0;
		}
	}
}
