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
	PURPOSE :  Compute   v = log ( (1 + s)/(1 - s) )
	for small s, this is for |s| < SLIM = 0.20

	ALGORITHM : 
	1/ if |s| is "very small" we use a truncated
	taylor dvp (by keeping 3 terms) from : 
	2        4          6
	t = 2 * s * ( 1 + 1/3 s  + 1/5 s  + [ 1/7 s  + ....] )
	2        4      
	t = 2 * s * ( 1 + 1/3 s  + 1/5 s  + er)

	The limit E until we use this formula may be simply
	gotten so that the negliged part er is such that :
	2        4
	(#) er <= epsm * ( 1 + 1/3 s  + 1/5 s )   for all |s|<= E

	As  er  = 1/7 s^6 + 1/9 s^8 + ... 
	er <= 1/7 * s^6 ( 1 + s^2 + s^4 + ...) = 1/7  s^6/(1-s^2)

	the inequality (#) is forced if :

	1/7  s^6 / (1-s^2)  <= epsm * ( 1 + 1/3 s^2  + 1/5 s^4 )

	s^6 <= 7 epsm * (1 - 2/3 s^2 - 3/15 s^4 - 1/5 s^6)

	So that E is very near (7 epsm)^(1/6) (approximately 3.032d-3):

	2/ For larger |s| we used a minimax polynome :

	yi = s * (2  + d3 s^3 + d5 s^5 .... + d13 s^13 + d15 s^15)

	This polynome was computed (by some remes algorithm) following 
	(*) the sin(x) example (p 39) of the book :

	"ELEMENTARY FUNCTIONS"
	"Algorithms and implementation"
	J.M. Muller (Birkhauser)

	(*) without the additionnal raffinement to get the first coefs
	very near floating point numbers)
*/
double lnp1m1(double _dblVar)
{
	static double sdblD3	= 0.66666666666672679472;
	static double sdblD5	= 0.39999999996176889299;
	static double sdblD7	= 0.28571429392829380980;
	static double sdblD9	= 0.22222138684562683797;
	static double sdblD11	= 0.18186349187499222459;
	static double sdblD13	= 0.15250315884469364710;
	static double sdblD15	= 0.15367270224757008114;
	static double sdblE		= 3.032E-3;
	static double sdblC3	= 2.0/3.0;
	static double sdblC5	= 2.0/5.0;

	double dblS2 = _dblVar * _dblVar;
	if( dabss(_dblVar) <= sdblE)
		return _dblVar * (2 + dblS2 * (sdblC3 + sdblC5 * dblS2));
	else
		return _dblVar * (2 + dblS2 * (sdblD3 + dblS2 * (sdblD5 + dblS2 * (sdblD7 + dblS2 * (sdblD9 + dblS2 * (sdblD11 + dblS2 * (sdblD13 + dblS2 * sdblD15)))))));
}


/*sqrt*/
double dsqrts(double _dblVal)
{
    return sqrt(_dblVal);
}

/*log*/
double dlogs(double _dblVal)
{
  return log(_dblVal);
}

/*sign*/
double dsigns(double _dblRef, double _dblVal)
{
	if( _dblVal >= 0)
		return _dblRef;
	else
		return -_dblRef;
}

/*up round*/
int dceils(double _dblVal)
{
	/*
	3.14 -> 3
	4,00 -> 4

	-3.14 -> -3
	-4.00 -> -4
	*/
	if((int)_dblVal == _dblVal)
		return (int)(_dblVal);

	if(_dblVal >= 0)
		return (int)(_dblVal + 1);

	return (int)(_dblVal);
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
void vDvmul(int _iNbElem, double* _piIn1, double* _piIn2, int _iIncIn1, int _iIncIn2, double* _piOut)
{
	int iIndex = 0;
	if(_iIncIn1 == 1 && _iIncIn2 == 1)
	{
		for(iIndex = 0; iIndex < _iNbElem ; iIndex++)
			_piOut[iIndex] = _piIn1[iIndex] * _piIn2[iIndex];
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
			_piOut[iIndex2] = _piIn1[iIndex1] * _piIn2[iIndex2];
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
		int iIndex1 = 1;
		int iIndex2 = 1;
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

	memset(_pdblOcc, 0x00, _iNbElemVal * sizeof(double));
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
			_pdblOcc[iIndex2 - 1]++;
			_pdblInd[iLoop]	= iIndex2;
		}
		else
		{
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
				_pdblOcc[iIndex1]++;
				_pdblInd[iLoop]	= iIndex1 + 1;
			}
			else if(_pdblX[iLoop] == _pdblVal[iIndex2])
			{
				_pdblOcc[iIndex2]++;
				_pdblInd[iLoop]	= iIndex2 + 1;
			}
			else
			{
				(*_pdblInfo)++;
				_pdblInd[iLoop] = 0;
			}
		}
		else
		{
			*_pdblInfo++;
			_pdblInd[iLoop] = 0;
		}
	}
}
