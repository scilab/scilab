#include "basic_functions.h"


/*atan*/
double datans(double _dblVal)
{
	return atan(_dblVal);
}

/*atan2*/
double datan2s(double _dblValX, double _dblValY)
{
	return atan2(_dblValX, _dblValY);
}

/*
watan compute the arctangent of a complex number
	COPYRIGHT (C) 2001 Bruno Pincon and Lydia van Dijk
	Written by Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr> so
	as to get more precision.  Also to fix the
	behavior at the singular points and at the branch cuts.
	Polished by Lydia van Dijk 
	<lvandijk@hammersmith-consulting.com>

*/
void watan(double _dblReal, double _dblImg, double *_pdblReal, double *_pdblImg)
{
	static double sdblSlim		= 0.2;
	static double sdblAlim		= 1E-150;
	static double sdblTol		= 0.3;
	static double sdblLn2		= 0.6931471805599453094172321;

	double dblRMax				= F2C(dlamch)("O",1L);
	double dblPi_2				= 2.0 * datans(1);


	//Temporary variables
	double dblR2 = 0;
	double dblS = 0;


	if(_dblImg == 0)
	{
		*_pdblReal	= datans(_dblReal);
		*_pdblImg	= 0;
	}
	else
	{
		dblR2 = _dblReal * _dblReal + _dblImg * _dblImg; // Oo
		if(dblR2 > dblRMax)
		{
			if( dabss(_dblImg) > dblRMax)
				dblS = 0;
			else
				dblS = 1 / (((0.5 * _dblReal) / _dblImg) * _dblReal + 0.5 * _dblImg );
		}
		else
			dblS = (2 * _dblImg) / (1+dblR2);

		if(dabss(dblS) < sdblSlim)
		{
			/*
			s is small: |s| < SLIM  <=>  |z| outside the following disks:
			D+ = D(center = [0;  1/slim], radius = sqrt(1/slim**2 - 1)) if b > 0
			D- = D(center = [0; -1/slim], radius = sqrt(1/slim**2 - 1)) if b < 0
			use the special evaluation of log((1+s)/(1-s)) (5)
			*/
			*_pdblImg = lnp1m1(dblS) * 0.25;
		}
		else
		{
			if(dabss(dblS) == 1 && dabss(_dblReal) <= sdblAlim)
			{
				//|s| >= SLIM  => |z| is inside D+ or D-
				*_pdblImg = dsigns(0.5,_dblImg) * ( sdblLn2 - log(dabss(_dblReal)));
			}
			else
			{
				*_pdblImg = 0.25 * log((pow(_dblReal,2) + pow((_dblImg + 1),2)) / pow(_dblReal,2) + pow((_dblImg - 1),2));
			}
		}
		if(_dblReal == 0)
		{//z is purely imaginary
			if( dabss(_dblImg) > 1)
			{//got sign(b) * pi/2
				*_pdblReal = dsigns(1, _dblImg) * dblPi_2;
			}
			else if( dabss(_dblImg) == 1)
			{//got a Nan with 0/0
				*_pdblReal = (_dblReal - _dblReal) / (_dblReal - _dblReal); // Oo
			}
			else
				*_pdblReal = 0;
		}
		else if(dblR2 > dblRMax)
		{//_pdblImg is necessarily very near sign(a)* pi/2 
			*_pdblReal = dsigns(1, _dblReal) * dblPi_2;
		}
		else if(dabss(1 - dblR2) + dabss(_dblReal) <= sdblTol)
		{//|b| is very near 1 (and a is near 0)  some cancellation occur in the (next) generic formula 
			*_pdblReal = 0.5 * atan2(2 * _dblReal, (1-_dblImg) * (1 + _dblImg) - pow(_dblReal,2));
		}
		else
			*_pdblReal = 0.5 * atan2(2 * _dblReal, 1 - dblR2);
	}
}