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
#include "basic_functions.h"

/*
*     PURPOSE
*        computes sqrt(a^2 + b^2) with accuracy and
*        without spurious underflow / overflow problems
*
*     MOTIVATION
*        This work was motivated by the fact that the original Scilab
*        PYTHAG, which implements Moler and Morrison's algorithm is slow.
*        Some tests showed that the Kahan's algorithm, is superior in
*        precision and moreover faster than the original PYTHAG.  The speed
*        gain partly comes from not calling DLAMCH.
*
*     REFERENCE
*        This is a Fortran-77 translation of an algorithm by William Kahan,
*        which appears in his article "Branch cuts for complex elementary
*        functions, or much ado about nothing's sign bit",
*        Editors: Iserles, A. and Powell, M. J. D.
*        in "States of the Art in Numerical Analysis"
*        Oxford, Clarendon Press, 1987
*        ISBN 0-19-853614-3
**     AUTHOR
*        Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>,
*        Thanks to Lydia van Dijk <lvandijk@hammersmith-consulting.com>
*/
double dpythags(double _dblVal1, double _dblVal2)
{
	double dblSqrt2		= 1.41421356237309504;
	double dblSqrt2p1	= 2.41421356237309504;
	double dblEsp		= 1.25371671790502177E-16;
	double dblRMax		= F2C(dlamch)("o",1L);

	double dblAbs1 = 0;
	double dblAbs2 = 0;
	double dblTemp = 0;

	if(ISNAN(_dblVal1) == 1)
		return _dblVal2;

	if(ISNAN(_dblVal2) == 1)
		return _dblVal1;

	dblAbs1 = dabss(_dblVal1);
	dblAbs2 = dabss(_dblVal2);

	//Order x and y such that 0 <= y <= x
	if(dblAbs1 < dblAbs2)
	{
		dblTemp = dblAbs1;
		dblAbs1 = dblAbs2;
		dblAbs2 = dblTemp;
	}

	//Test for overflowing x
	if( dblAbs1 >= dblRMax)
		return dblAbs1;

	//Handle generic case
	dblTemp = dblAbs1 - dblAbs2;
	if(dblTemp != dblAbs1)
	{
		double dblS = 0;
		if(dblTemp > dblAbs2)
		{
			dblS = dblAbs1 / dblAbs2;
			dblS += dsqrts(1 + dblS * dblS);
		}
		else
		{
			dblS	= dblTemp / dblAbs2;
			dblTemp = (2 + dblS) * dblS;
			dblS	= ((dblEsp + dblTemp / (dblSqrt2 + dsqrts(2 + dblTemp))) + dblS) + dblSqrt2p1;
		}
		return dblAbs1 + dblAbs2 / dblS;
	}
	else
		return dblAbs1;

	return 0;
}

/*sign*/
double dsigns(double _dblRef, double _dblVal)
{
	if( _dblVal >= 0)
		return _dblRef;
	else
		return -_dblRef;
}

double dsignsEx(double _dblVal)
{
	if(_dblVal == 0)
		return 0;
	if(_dblVal > 0)
		return 1;
	if(_dblVal < 0)
		return -1;
	return _dblVal;

}
/*up round*/
double dceils(double _dblVal)
{
	/*
	3.14 -> 3
	4,00 -> 4

	-3.14 -> -3
	-4.00 -> -4
	*/
	return ceil(_dblVal);
}

/*up round with precision*/
double dceilsEx(double _dblVal, int _iPrecision)
{
	double iPow = pow(10, _iPrecision);

	double dblTemp = _dblVal * iPow;
	dblTemp = dceils(dblTemp);

	return (double)dblTemp / iPow;

}

/*absolute complex sum*/
double wasums(int _iNbElem, double* _pdblReal, double* _pdblImg)
{
	double dblRetVal = 0;
	int iIndex = 0;

	for(iIndex = 0 ; iIndex < _iNbElem ; iIndex ++)
		dblRetVal += dabss(_pdblReal[iIndex]) + dabss(_pdblImg[iIndex]);

	return dblRetVal;
}

/*absolute sum*/
double dasums(int _iNbElem, double* _pdblReal)
{
	double dblRetVal = 0;
	int iIndex = 0;
	for(iIndex = 0 ; iIndex < _iNbElem ; iIndex++)
		dblRetVal += dabss(_pdblReal[iIndex]);
	return dblRetVal;

/*the original source code "Optimization" ?!?
   20 m = mod(n,6)
      if( m .eq. 0 ) go to 40
      do 30 i = 1,m
        dtemp = dtemp + dabs(dx(i))
   30 continue
      if( n .lt. 6 ) go to 60
   40 mp1 = m + 1
      do 50 i = mp1,n,6
        dtemp = dtemp + dabs(dx(i)) + dabs(dx(i + 1)) + dabs(dx(i + 2))
     *  + dabs(dx(i + 3)) + dabs(dx(i + 4)) + dabs(dx(i + 5))
   50 continue
   60 dasum = dtemp
*/

}

/*scales a vector by a constant*/
void ddscals(double* _pdblIn, int _iNbElem, double _dblMulti, double* _pdblOut)
{
	int iIndex = 0;
	for(iIndex = 0 ; iIndex < _iNbElem ; iIndex++)
	{
		_pdblOut[iIndex] = _dblMulti * _pdblIn[iIndex];
	}
}

/*scales a vector by a complex constant*/
void zwscals(double* _pdblRealIn, double* _pdblImgIn, int _iNbElem, double _dblMultiReal, double _dblMultiImg, double* _pdblRealOut, double* _pdblImgOut, int _iInc)
{
	int iIndex = 0;
	int iIndex2 = 0;
	for(iIndex = 0 ; iIndex < _iNbElem ; iIndex++)
	{
		zwmuls(_dblMultiReal, _dblMultiImg, _pdblRealIn[iIndex2], _pdblImgIn[iIndex2], &_pdblRealOut[iIndex2], &_pdblImgOut[iIndex2]);
		iIndex2 += _iInc;
	}
}


void zwmuls(double _dblReal1, double _dblImg1, double _dblReal2, double _dblImg2, double *_pRealOut, double *_pImgOut)
{
	*_pImgOut	= _dblReal1 * _dblImg2 + _dblImg1 * _dblReal2;
	*_pRealOut	= _dblReal1 * _dblReal2 - _dblImg1 * _dblImg2;
}

void zwmmuls(double* _pdblReal1	, double* _pdblImg1	, int _iRows1,
			 double* _pdblReal2	, double* _pdblImg2	, int _iRows2,
			 double *_pRealOut	, double *_pImgOut	, int _iRowsOut,
			 int _iColsOut		, int _iCols1		, int _iCols2)
{
	double dblOne		= 1;
	double dblMinusOne	= -1;
	double dblZero		= 0;

	//Cr <-  1*Ar*Br + 0*Cr

	F2C(dgemm)("N","N", &_iRowsOut, &_iCols2, &_iCols1, &dblOne		, _pdblReal1	, &_iRows1, _pdblReal2	, &_iRows2, &dblZero	, _pRealOut	, &_iRowsOut);
	//Cr <- -1*Ai*Bi + 1*Cr
	F2C(dgemm)("N","N", &_iRowsOut, &_iCols2, &_iCols1, &dblMinusOne	, _pdblImg1		, &_iRows1, _pdblImg2	, &_iRows2, &dblOne	, _pRealOut	, &_iRowsOut);
	//Ci <-  1*Ar*Bi + 0*Ci
	F2C(dgemm)("N","N", &_iRowsOut, &_iCols2, &_iCols1, &dblOne		, _pdblReal1	, &_iRows1, _pdblImg2	, &_iRows2, &dblZero	, _pImgOut	, &_iRowsOut);
	//Ci <-  1*Ai*Br + 1*Ci
	F2C(dgemm)("N","N", &_iRowsOut, &_iCols2, &_iCols1, &dblOne		, _pdblImg1		, &_iRows1, _pdblReal2	, &_iRows2, &dblOne	, _pImgOut	, &_iRowsOut);
}

/*Cumulative product*/
void vCupro(int _iNbElem, double* _piIn, double* _piOut)
{
	int iIndex = 0;
	double dblCoeff = 1;
	for(iIndex = 0 ; iIndex < _iNbElem ; iIndex++)
	{
		_piOut[iIndex] = _piIn[iIndex] * dblCoeff;
		dblCoeff = _piOut[iIndex];
	}
}

/*Cumulative product complex*/
void vCuproi(int _iNbElem, double* _piRealIn, double* _piImgIn, double* _piRealOut, double* _piImgOut)
{
	int iIndex = 0;
	double dblRealCoeff = 1, dblImgCoeff = 0;
	for(iIndex = 0 ; iIndex < _iNbElem ; iIndex++)
	{
		double dblTemp		= _piRealIn[iIndex];
		_piRealOut[iIndex]	= dblTemp * dblRealCoeff - dblImgCoeff * _piImgIn[iIndex];
		_piImgOut[iIndex]	= dblRealCoeff * _piImgIn[iIndex] + dblImgCoeff * dblTemp;
		dblRealCoeff		= _piRealOut[iIndex];
		dblImgCoeff			= _piImgOut[iIndex];
	}
}

/*
etant donne un vecteur _piIn1 et un vecteur _piIn2,
 cette subroutine fait:
               _piOut = _piIn1 * _piIn2
quand les deux increments sont egaux a 1, cette
subroutine emploie des boucles "epanouis". dans le cas ou
les increments sont negatifs, cette subroutine prend
les composantes en ordre inverse.
*/
void vDvmul(int _iNbElem, double* _pdblIn1, double* _pdblIn2, int _iIncIn1, int _iIncIn2, double* _pdblOut)
{
	int iIndex = 0;
	if(_iIncIn1 == 1 && _iIncIn2 == 1)
	{
		for(iIndex = 0; iIndex < _iNbElem ; iIndex++)
			_pdblOut[iIndex] = _pdblIn1[iIndex] * _pdblIn2[iIndex];
	}
	else
	{
		int iIndex1 = 0;
		int iIndex2 = 0;
		if(_iIncIn1 < 0)
			iIndex1 = (-_iNbElem + 1) * _iIncIn1 + 1;
		if(_iIncIn2 < 0)
			iIndex1 = (-_iNbElem + 1) * _iIncIn2 + 1;

		for(iIndex = 0; iIndex < _iNbElem ; iIndex++)
		{
			_pdblOut[iIndex2] = _pdblIn1[iIndex1] * _pdblIn2[iIndex2];
			iIndex1			+= _iIncIn1;
			iIndex2			+= _iIncIn2;
		}
	}
}
/* vDvmul complex*/
void vWvmul(int _iNbElem, double* _piRealIn1, double* _piImgIn1, double* _piRealIn2, double* _piImgIn2, int _iIncIn1, int _iIncIn2, double* _piRealOut, double* _piImgOut)
{
	int iIndex = 0;
	if(_iIncIn1 == 1 && _iIncIn2 == 1)
	{
		for(iIndex = 0; iIndex < _iNbElem ; iIndex++)
		{
			double dblTemp		= _piRealIn2[iIndex];
			_piRealOut[iIndex]	= dblTemp * _piRealIn1[iIndex] - _piImgIn2[iIndex] * _piImgIn1[iIndex];
			_piImgOut[iIndex]	= dblTemp * _piImgIn1[iIndex] + _piImgIn2[iIndex] * _piRealIn1[iIndex];
		}
	}
	else
	{
		int iIndex1 = 0;
		int iIndex2 = 0;
		if(_iIncIn1 < 0)
			iIndex1 = (-_iNbElem + 1) * _iIncIn1 + 1;
		if(_iIncIn2 < 0)
			iIndex1 = (-_iNbElem + 1) * _iIncIn2 + 1;

		for(iIndex = 0; iIndex < _iNbElem ; iIndex++)
		{
			double dblTemp	= _piRealIn2[iIndex2];
			_piRealOut[iIndex2] = dblTemp * _piRealIn1[iIndex1] - _piImgIn2[iIndex2] * _piImgIn1[iIndex1];
			_piImgOut[iIndex2]	= dblTemp * _piImgIn1[iIndex1] + _piImgIn2[iIndex2] * _piRealIn1[iIndex1];
			iIndex1			+= _iIncIn1;
			iIndex2			+= _iIncIn2;
		}
	}
}

/*Cumulative sum*/
void vCusum(int _iNbElem, double *_pdblIn, double *_pdblOut)
{
	double dblTemp = 0;
	int iIndex = 0;

	for(iIndex = 0 ; iIndex < _iNbElem ; iIndex++)
	{
		_pdblOut[iIndex] = dblTemp + _pdblIn[iIndex];
		dblTemp = _pdblOut[iIndex];
	}
}

/*Add two vectors*/
void vDadd(int _iNbElem, double* _piIn1, double* _piIn2, int _iIncIn1, int _iIncIn2, double* _piOut)
{
	int iIndex = 0;
	if(_iIncIn1 == 1 && _iIncIn2 == 1)
	{
		for(iIndex = 0; iIndex < _iNbElem ; iIndex++)
			_piOut[iIndex] = _piIn1[iIndex] + _piIn2[iIndex];
	}
	else
	{
		int iIndex1 = 1;
		int iIndex2 = 1;
		if(_iIncIn1 < 0)
			iIndex1 = (-_iNbElem + 1) * _iIncIn1 + 1;
		if(_iIncIn2 < 0)
			iIndex1 = (-_iNbElem + 1) * _iIncIn2 + 1;

		for(iIndex = 0; iIndex < _iNbElem ; iIndex++)
		{
			_piOut[iIndex2] = _piIn1[iIndex1] + _piIn2[iIndex2];
			iIndex1			+= _iIncIn1;
			iIndex2			+= _iIncIn2;
		}
	}
}

/*memset on vector*/
void vDset(int _iNbElem, double _dblVal, double* _pdblIn, int _iInc)
{
	int iIndex = 0;
	int iIndex2 = 0;
	if(_iInc < 0 )
		iIndex2 = (-_iNbElem+1) * _iInc + 1;

	for(iIndex = 0 ; iIndex < _iNbElem ; iIndex++)
	{
		_pdblIn[iIndex2] = _dblVal;
		iIndex2 += _iInc;
	}
}



/*
PURPOSE
   val(0..n) being an array (with strict increasing order and n >=1)
   representing intervals, this routine, by the mean of a
   dichotomic search, computes :

      a/ for each X(i) its interval number indX(i) :
                indX(i) = j if  X(i) in (val(j-1), val(j)]
                        = 1 if  X(i) = val(0)
                        = 0 if  X(i) is not in [val(0),val(n)]

      b/ the number of points falling in the interval j :

         occ(j) = # { X(i) such that X(i) in (val(j-1), val(j)] } for j>1
    and  occ(1) = # { X(i) such that X(i) in [val(0), val(1)] }

PARAMETERS
   inputs :
      m         integer
      X(1..m)   double float array
      n         integer
      val(0..n) double float array (val(0) < val(1) < ....)
   outputs
      indX(1..m) integer array
      occ(1..n)  integer array
      info       integer (number of X(i) not in [val(0), val(n)])

AUTHOR
   Bruno Pincon

TRANSLATION TO C
	Antoine Elias

	PARAMETERS
	Inputs :
		m			-> _iNbElemX
		X(1..m)		-> _pdblX[_iNbElemX]
		n			-> _iNbElemVal
		val(0..n)	-> _pdblVal[_iNbElemVal]

	Outputs :
		indX(1..m)	-> _pdblInd[_iNbElemX]
		occ(1..n)	-> _pdblOcc[_iNbElemVal]
		info		-> _pdblInfo
*/

void vDsearchC(double *_pdblX, int _iNbElemX, double *_pdblVal, int _iNbElemVal, double* _pdblInd, double *_pdblOcc, double *_pdblInfo)
{
	int iLoop = 0;

	if(_pdblOcc != NULL)
		memset(_pdblOcc, 0x00, _iNbElemVal * sizeof(double));

	if(_pdblInfo != NULL)
		*_pdblInfo	= 0;
	for(iLoop = 0 ; iLoop < _iNbElemX ; iLoop++)
	{
		if(_pdblVal[0] <= _pdblX[iLoop] && _pdblX[iLoop] <= _pdblVal[_iNbElemVal])
		{
			int iIndex = 0;
			int iIndex1 = 0;
			int iIndex2 = _iNbElemVal+1;
			while(iIndex2 - iIndex1 > 1)
			{
				iIndex = (iIndex1 + iIndex2) / 2;
				if( _pdblX[iLoop] <= _pdblVal[iIndex])
					iIndex2 = iIndex;
				else
					iIndex1 = iIndex;
			}

			_pdblInd[iLoop]	= iIndex2;
			if(_pdblOcc != NULL)
				_pdblOcc[iIndex2 - 1]++;
		}
		else
		{
			if(_pdblInfo != NULL)
				(*_pdblInfo)++;
			_pdblInd[iLoop] = 0;
		}
	}
}

/*
 PURPOSE
    val(1..n) being a strictly increasing array, this
    routines by the mean of a dichotomic search computes :

    a/ the number of occurences (occ(j)) of each value val(j)
       in the array X :

          occ(j) = #{ X(i) such that X(i) = val(j) }

    b/ the array indX :  if X(i) = val(j) then indX(i) = j
       (if X(i) is not in val then indX(i) = 0)

 PARAMETERS
    inputs :
       m         integer
       X(1..m)   double float array
       n         integer
       val(1..n) double float array (must be in a strict increasing order)
    outputs :
       occ(1..n)  integer array
       indX(1..m) integer array
       info       integer  (number of X(i) which are not in val(1..n))

 AUTHOR
    Bruno Pincon

TRANSLATION TO C
	Antoine Elias

	PARAMETERS
	Inputs :
		m			-> _iNbElemX
		X(1..m)		-> _pdblX[_iNbElemX]
		n			-> _iNbElemVal
		val(1..n)	-> _pdblVal[_iNbElemVal]

	Outputs :
		occ(1..n)	-> _pdblOcc[_iNbElemVal]
		indX(1..m)	-> _pdblInd[_iNbElemX]
		info		-> _pdblInfo
*/

void vDsearchD(double *_pdblX, int _iNbElemX, double *_pdblVal, int _iNbElemVal, double* _pdblInd, double *_pdblOcc, double *_pdblInfo)
{
	int iLoop = 0;

	if(_pdblOcc != NULL)
		memset(_pdblOcc, 0x00, _iNbElemVal * sizeof(double));
	*_pdblInfo	= 0;
	for(iLoop = 0 ; iLoop < _iNbElemX ; iLoop++)
	{
		if(_pdblVal[0] <= _pdblX[iLoop] && _pdblX[iLoop] <= _pdblVal[_iNbElemVal-1])
		{
			int iIndex = 0;
			int iIndex1 = 0;
			int iIndex2 = _iNbElemVal-1;
			while(iIndex2 - iIndex1 > 1)
			{
				iIndex = (iIndex1 + iIndex2) / 2;
				if( _pdblX[iLoop] < _pdblVal[iIndex])
					iIndex2 = iIndex;
				else
					iIndex1 = iIndex;
			}
			if(_pdblX[iLoop] == _pdblVal[iIndex1])
			{
				if(_pdblOcc != NULL)
					_pdblOcc[iIndex1]++;
				_pdblInd[iLoop]	= iIndex1 + 1;
			}
			else if(_pdblX[iLoop] == _pdblVal[iIndex2])
			{
				if(_pdblOcc != NULL)
					_pdblOcc[iIndex2]++;
				_pdblInd[iLoop]	= iIndex2 + 1;
			}
			else
			{
				if(_pdblInfo != NULL)
					(*_pdblInfo)++;
				_pdblInd[iLoop] = 0;
			}
		}
		else
		{
			if(_pdblInfo != NULL)
				*_pdblInfo++;
			_pdblInd[iLoop] = 0;
		}
	}
}

/*floor*/
double dfloors(double _dblVal)
{
	return floor(_dblVal);
}

/*frexp*/
double dfrexps(double _dblVal, double *_pdblExp)
{
	int iExp = 0;
	double dblCoef = 0;
	dblCoef = frexp(_dblVal, &iExp);
	*_pdblExp = iExp;
	return dblCoef;
}

double dblNearFloat(double _dblVal, double _dblMode)
{
	double dblSignMode	= 0;
	double dblSignVal	= 0;
	double dblAbsVal	= 0;

	double dblRMax		= F2C(dlamch)("o",1L);
	double dblRMin		= F2C(dlamch)("u",1L);
	double dblBase		= F2C(dlamch)("b",1L);
	int iP				= (int)F2C(dlamch)("n",1L);
	double dblLogBase	= dlogs(dblBase);

	double dblUlp		= pow(dblBase, 1 - iP);
	double dblTiny		= 0;
	BOOL bNorm			= FALSE;

	if(dblRMin / dblRMax != 0)
	{
		int iIndex = 0;
		bNorm	= TRUE;
		dblTiny	= dblRMin;
		for(iIndex = 1 ; iIndex < iP ; iIndex++)
			dblTiny	/= dblBase;
	}
	else
		bNorm = FALSE;

	dblSignMode	= dsigns(1, _dblMode);
	dblSignVal	= dsigns(1, _dblVal);
	dblAbsVal	= dabss(_dblVal);

	if(ISNAN(_dblVal) == 1) // NaN
	{
		return _dblVal;
	}
	else if(dblAbsVal > dblRMax) // +/- Inf
	{
		if(dblSignMode * dblSignVal < 0)
			return dblSignVal * dblRMax;
		else
			return _dblVal;
	}
	else if(dblAbsVal >= dblRMin)
	{
		//usual case : x is a normalised floating point number
		// 1/ got the exponent e and the exponent part ep = base^e
		int iE			= (int)(dlogs(dblAbsVal) / dblLogBase);
		double dblEp	= pow(dblBase, iE);
		double dblMant	= 0;

		//in case of xa very near RMAX an error in e (of + 1)
		//result in an overflow in ep
		if(iE > dblRMax)
			iE		-= 1;
		//also in case of xa very near RMIN and when denormalised
		//number are not used, an error in e (of -1) results in a
		//flush to 0 for ep.
		if(dblEp == 0)
			iE		-= 1;

		dblEp	= pow(dblBase, iE);

		// 2/ got the mantissa
		dblMant	= dblAbsVal / dblEp;

		// 3/ verify that 1 <= m < BASE
		if(dblMant < 1)
		{
			while(dblMant < 1)
			{
				dblMant *= dblBase;
				dblEp	/= dblBase;
			}
		}
		else if(dblMant >= dblBase)
		{
			while(dblMant < 1)
			{
				dblMant /= dblBase;
				dblEp	*= dblBase;
			}
		}

		// 4/ now compute the near float
		if(dblSignMode * dblSignVal < 0)
		{
			if( dblMant == 1)
				return dblSignVal * (dblMant - (dblUlp / dblBase )) * dblEp;
			else
				return dblSignVal * (dblMant - dblUlp) * dblEp;
		}
		else
			return dblSignVal * (dblMant + dblUlp) * dblEp;
	}
	else if(_dblVal == 0)
	{//case x = 0  nearfloat depends if denormalised numbers are used
		if(bNorm)
			return dblSignMode * dblTiny;
		else
			return dblSignMode * dblRMin;
	}
	else //x is a denormalised number
		return _dblVal + dblSignMode * dblTiny;
}

/*
purpose
    computes the product of the entries of a matrix according to flag
calling sequence
    subroutine dmprod(flag,a,na,m,n,v,nv)
    double precision a(na,n),v(*)
    integer na,n,m,nv
    integer flag
parameters
    flag : indicates operation to perform
           0 : returns in v(1) the product of all entries of a
           1 : returns in v(j) the product of jth column of a
           2 : returns in v(i) the product of ith row of a
    a    : array containing the a matrix
    na   : a matrix leading dimension
    m    : a matrix row dimension
    n    : a matrix column dimension
    v    : array containing the result, may be confused with a row or
           a column of the a matrix
           if flag==0 size(v)>=1
           if flag==1 size(v)>=n*nv
           if flag==1 size(v)>=m*nv
    nv   : increment between to consecutive entries ov v
*/
void vDmProd(int _iMode, double* _pdblIn, int _iLeadDim, int _iRows, int _iCols, double* _pdblOut, int _iInc)
{
	int iIndex = 0;
	int iLoop = 0;
	switch(_iMode)
	{
	case 0 :
		_pdblOut[0] = 1;
		vDvmul(_iRows * _iCols, _pdblIn, _pdblOut, 1, 0, _pdblOut);
		break;
	case 1 :
	{
		for(iLoop = 0 ; iLoop < _iCols ; iLoop++)
		{
			_pdblOut[iIndex] = 1;
			vDvmul(_iRows, _pdblIn + _iRows * iLoop, _pdblOut + iIndex, 1, 0, _pdblOut + iIndex);
			iIndex += _iInc;
		}
	}
		break;
	case 2 :
	{
		for(iLoop = 0 ; iLoop < _iRows ; iLoop++)
		{
			_pdblOut[iIndex] = 1;
			vDvmul(_iCols, _pdblIn + iLoop, _pdblOut + iIndex, _iRows, 0, _pdblOut + iIndex);
			iIndex += _iInc;
		}
	}
		break;
	}
}

/*
purpose
    computes the product of the entries of a complex matrix according to flag
calling sequence
    subroutine wmprod(flag,ar,ai,na,m,n,vr,vi,nv)
    double precision ar(na,n),ai(na,n),vr(*),vi(*)
    integer na,n,m,nv
    integer flag
parameters
    flag : indicates operation to perform
           0 : returns in v(1) the product of all entries of a
           1 : returns in v(j) the product of jth column of a
           2 : returns in v(i) the product of ith row of a
    a    : array containing the a matrix
    na   : a matrix leading dimension
    m    : a matrix row dimension
    n    : a matrix column dimension
    v    : array containing the result,
           vr (resp vi) may be confused with a row or
           a column of the ar (resp ai) matrix
           if flag==0 size(v)>=1
           if flag==1 size(v)>=n*nv
           if flag==1 size(v)>=m*nv
    nv   : increment between to consecutive entries ov v
*/
void vWDmProd(int _iMode, double* _pdblIn1, double* _pdblIn2, int _iLeadDim, int _iRows, int _iCols, double* _pdblOut1, double* _pdblOut2, int _iInc)
{
	int iIndex = 0;
	int iLoop = 0;
	switch(_iMode)
	{
	case 0 :
		_pdblOut1[iIndex] = 1;
		_pdblOut2[iIndex] = 0;
		for(iLoop = 0 ; iLoop < _iCols ; iLoop++)
			vWvmul(_iRows, _pdblIn1 + _iRows * iLoop, _pdblIn2 + _iRows * iLoop, _pdblOut1 + iIndex, _pdblOut2 + iIndex, 1, 0, _pdblOut1 + iIndex, _pdblOut2 + iIndex);
		break;
	case 1 :
		for(iLoop = 0 ; iLoop < _iCols ; iLoop++)
		{
			_pdblOut1[iIndex] = 1;
			_pdblOut2[iIndex] = 0;
			vWvmul(_iRows, _pdblIn1 + _iRows * iLoop, _pdblIn2 + _iRows * iLoop, _pdblOut1 + iIndex, _pdblOut2 + iIndex, 1, 0, _pdblOut1 + iIndex, _pdblOut2 + iIndex);
			iIndex += _iInc;
		}
		break;
	case 2 :
		for(iLoop = 0 ; iLoop < _iRows ; iLoop++)
		{
			_pdblOut1[iIndex] = 1;
			_pdblOut2[iIndex] = 0;
			vWvmul(_iCols, _pdblIn1 + iLoop, _pdblIn2 + iLoop, _pdblOut1 + iIndex, _pdblOut2 + iIndex, _iRows, 0, _pdblOut1 + iIndex, _pdblOut2 + iIndex);
			iIndex += _iInc;
		}
		break;
	}
}

double durands(int* _iVal)
{
	static int ia = 0, ic = 0, itwo = 2, m2 = 0, m = 0, mic = 0;
	static double halfm = 0, s = 0;

	if(m2 == 0)
	{
		m = 1;
		while(m > m2)
		{
			m2 = m;
			m = itwo * m2;
		}
		halfm = m2;

		ia = 8 * nint(halfm * datans(1) / 8) + 5;
		ic = 2 * nint(halfm * (0.5 - dsqrts(3) / 6)) + 1;
		mic = (m2 - ic) + m2;

		s = 0.5 / halfm;
	}

	*_iVal *= ia;

	if(*_iVal > mic)
		*_iVal = (*_iVal - m2) - m2;

	*_iVal += ic;

	if(*_iVal/2 > m2)
		*_iVal = (*_iVal - m2) - m2;

	if(*_iVal < 0)
		*_iVal = (*_iVal + m2) + m2;

	return (double)*_iVal * s;
}

int nint(double _iVal)
{
	return (int)(_iVal + 0.5);
}

/*aint return double round val */
double danints(double _dblVal)
{
	/* ATTENTION CECI N'EST PAS ENCORE COMPLETEMENT TESTE */
	/* 13/06/2008 ELAIS Antoine */

	double dblSign = dsigns(1, _dblVal);
	if(finite(dabss(_dblVal)) == 0)
	{//Infinite case !
		return _dblVal;
	}
	if(dblSign < 0)
		return ceil(_dblVal - 0.5);
	else
		return floor(_dblVal + 0.5);
	//return (double)(_dblVal + (0.5 * dblSign));
}


void ddmsums(int _iMode, double *_pdblIn, int _iRows, int _iCols, double *_pdblOut)
{
	int iIndex1		= 0;
	int iIndex2		= 0;

	switch(_iMode)
	{
	case BY_ALL : 
		_pdblOut[0] = 0;
		for(iIndex1 = 0 ; iIndex1 < _iRows * _iCols ; iIndex1++)
			_pdblOut[0] += _pdblIn[iIndex1];
		break;
	case BY_ROWS :
		for(iIndex2 = 0 ; iIndex2 < _iCols ; iIndex2++)
		{
			_pdblOut[iIndex2] = 0;
			for(iIndex1 = 0 ; iIndex1 < _iRows ; iIndex1++)
				_pdblOut[iIndex2] += _pdblIn[iIndex2 * _iRows + iIndex1];
		}
		break;
	case BY_COLS :
		for(iIndex2 = 0 ; iIndex2 < _iRows ; iIndex2++)
		{
			_pdblOut[iIndex2] = 0;
			for(iIndex1 = 0 ; iIndex1 < _iCols ; iIndex1++)
				_pdblOut[iIndex2] += _pdblIn[iIndex1 * _iRows + iIndex2];
		}
		break;
	}
}

void zdmsums(int _iMode, double *_pdblRealIn, double *_pdblImgIn, int _iRows, int _iCols, double *_pdblRealOut, double *_pdblImgOut)
{
	ddmsums(_iMode, _pdblRealIn, _iRows, _iCols, _pdblRealOut);
	ddmsums(_iMode, _pdblImgIn, _iRows, _iCols, _pdblImgOut);
}

void franck_matrix(int _iSize, double *_pData)
{
	int iIndex1		= 0;
	int iIndex2		= 0;
	double dblVal	= _iSize;

	memset(_pData, 0x00, sizeof(double) * _iSize * _iSize);
	_pData[0]		= dblVal;

	if(_iSize == 1)
		return;

	for(iIndex1 = 1 ; iIndex1 < _iSize ; iIndex1++)
	{
		dblVal = _iSize - iIndex1;
		_pData[(iIndex1-1) * _iSize + iIndex1] = dblVal;
		for(iIndex2 = 0 ; iIndex2 <= iIndex1 ; iIndex2++)
			_pData[iIndex1 * _iSize + iIndex2] = dblVal;
	}
}

void hilb_matrix(int _iSize, double *_pData)
{
	int iIndex1		= 0;
	int iIndex2		= 0;
	double dblVal	= _iSize;
	double dblTemp	= 0;

	for(iIndex1 = 0 ; iIndex1 < _iSize ; iIndex1++)
	{
		if(iIndex1 != 0)
			dblVal = ((_iSize - iIndex1) * dblVal * (_iSize + iIndex1)) / pow(iIndex1,2);
		dblTemp = dblVal * dblVal;

		_pData[iIndex1 * _iSize + iIndex1]	= dblTemp / ( 2 * iIndex1 + 1);
		if(iIndex1 == _iSize - 1)
			break;
		for(iIndex2 = iIndex1 + 1 ; iIndex2 < _iSize ; iIndex2++)
		{
			dblTemp = -((_iSize - iIndex2) * dblTemp * (_iSize + iIndex2)) / pow(iIndex2,2);
			_pData[iIndex1 * _iSize + iIndex2] = dblTemp / (iIndex1 + iIndex2 + 1);
			_pData[iIndex2 * _iSize + iIndex1] = _pData[iIndex1 * _iSize + iIndex2];
		}
	}
}

void magic_matrix(int _iSize, double *_pData)
{
	int iNewSize	= 0;
	int iIndex1		= 0;
	int iIndex2		= 0;
	int iUn			= 1;
	int iTemp1		= 0;
	int iTemp2		= 0;

	if(_iSize % 4 != 0)
	{
		int iRow	= 0;
		int iCol	= 0;
		if(_iSize % 2 == 0)
			iNewSize = _iSize / 2;
		if(_iSize % 2 != 0)
			iNewSize = _iSize;

		//odd order or upper corner of even order

		iRow		= 0;
		iCol		= iNewSize / 2;
		memset(_pData, 0x00, sizeof(double) * _iSize * _iSize);
		for(iIndex1 = 0 ; iIndex1 < iNewSize * iNewSize ; iIndex1++)
		{
			int iRowTemp	= 0;
			int iColTemp	= 0;

			_pData[iRow + iCol * _iSize] = iIndex1 + 1;
			iRowTemp		= iRow - 1;
			iColTemp		= iCol + 1;

			if(iRowTemp < 0)
				iRowTemp	= iNewSize - 1;
			if(iColTemp >= iNewSize)
				iColTemp	= 0;

			if(_pData[iRowTemp + iColTemp * _iSize] != 0)
			{
				iRowTemp	= iRow + 1;
				iColTemp	= iCol;
			}
			iRow		= iRowTemp;
			iCol		= iColTemp;
		}
		if(_iSize % 2 != 0)
			return;

		//rest of even order
		for(iIndex1 = 0 ; iIndex1 < iNewSize ; iIndex1++)
		{
			for(iIndex2 = 0 ; iIndex2 < iNewSize ; iIndex2++)
			{
				int iRow = iIndex1 + iNewSize;
				int iCol = iIndex2 + iNewSize;
				
				_pData[iIndex1 + iCol * _iSize] = _pData[iIndex1 + iIndex2 * _iSize] + 2 * iNewSize * iNewSize;
				_pData[iRow + iIndex2 * _iSize] = _pData[iIndex1 + iIndex2 * _iSize] + 3 * iNewSize * iNewSize;
				_pData[iRow + iCol * _iSize]	= _pData[iIndex1 + iIndex2 * _iSize] + iNewSize * iNewSize;
			}
		}
		if((iNewSize - 1) / 2 == 0)
			return;
		for(iIndex1 = 0 ; iIndex1 < (iNewSize - 1) / 2 ; iIndex1++)
			C2F(dswap)(&iNewSize, &_pData[iIndex1 * _iSize], &iUn, &_pData[iNewSize + iIndex1 * _iSize], &iUn);
		
		iTemp1	= (iNewSize + 1) / 2 - 1;
		iTemp2	= iTemp1 + iNewSize;
		C2F(dswap)(&iUn, &_pData[iTemp1], &iUn, &_pData[iTemp2], &iUn);
		C2F(dswap)(&iUn, &_pData[iTemp1 * _iSize + iTemp1], &iUn, &_pData[iTemp1 * _iSize + iTemp2], &iUn);
		iTemp1 = _iSize - (iNewSize - 3) / 2;
		if(iTemp1 > _iSize)
			return;
		for(iIndex1 = iTemp1 ; iIndex1 < _iSize ; iIndex1++)
			C2F(dswap)(&iNewSize, &_pData[iIndex1 * _iSize], &iUn, &_pData[iNewSize + iIndex1 * _iSize], &iUn);
	}
	else
	{
		int iVal = 1;
		for(iIndex1 = 0 ; iIndex1 < _iSize ; iIndex1++)
		{
			for(iIndex2 = 0 ; iIndex2 < _iSize ; iIndex2++)
			{
				_pData[iIndex2 * _iSize + iIndex1] = iVal;
				if(((iIndex1 + 1) % 4) / 2 == ((iIndex2 + 1) % 4) / 2)
					_pData[iIndex2 * _iSize + iIndex1] = _iSize * _iSize + 1 - iVal;
				iVal++;
			}
		}
	}
}

