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

/*asin*/
double dasins(double _dblVal)
{
	return asin(_dblVal);
}

/*This fonction is a translation of fortran wasin write by Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
*     REFERENCE
*        This is a Fortran-77 translation of an algorithm by
*        T.E. Hull, T. F. Fairgrieve and P.T.P. Tang which
*        appears in their article :
*          "Implementing the Complex Arcsine and Arccosine
*           Functions Using Exception Handling", ACM, TOMS,
*           Vol 23, No. 3, Sept 1997, p. 299-335
*/
void wasin(double _dblReal, double _dblImg, double *_pdblReal, double *_pdblImg)
{
	static double sdblPi_2		= 1.5707963267948966192313216;
	static double sdblLn2		= 0.6931471805599453094172321;
	static double sdblAcross	= 1.5;
	static double sdblBcross	= 0.6417;

	double dblLsup = sqrt(getOverflowThreshold())/8.0;
	double dblLinf = 4 * sqrt(getUnderflowThreshold());
	double dblEpsm = sqrt(getRelativeMachinePrecision());

	double dblAbsReal	= fabs(_dblReal);
	double dblAbsImg	= fabs(_dblImg);
	int iSignReal		= _dblReal < 0 ? -1 : 1;
	int iSignImg		= _dblImg < 0 ? -1 : 1;

	double dblR = 0, dblS = 0, dblA = 0, dblB = 0;

	double dblTemp = 0;

	if( Min(dblAbsReal, dblAbsImg) > dblLinf && Max(dblAbsReal, dblAbsImg) <= dblLsup)
	{//we are in the safe region
		dblR = sqrt( pow((dblAbsReal + 1 ), 2) + pow(dblAbsImg, 2));
		dblS = sqrt( pow((dblAbsReal - 1 ), 2) + pow(dblAbsImg, 2));
		dblA = 0.5 * ( dblR + dblS );
		dblB = dblAbsReal / dblA;


	//compute the real part
		if(dblB <= sdblBcross)
			*_pdblReal = asin(dblB);
		else if( dblAbsReal <= 1)
			*_pdblReal = atan(dblAbsReal / sqrt( 0.5 * (dblA + dblAbsReal) * ( pow(dblAbsImg, 2) / (dblR + (dblAbsReal + 1)) + (dblS + (1 - dblAbsReal)))));
		else
			*_pdblReal = atan(dblAbsReal / (dblAbsImg * sqrt(0.5 * ((dblA + dblAbsReal) / (dblR + (dblAbsReal + 1)) + (dblA + dblAbsReal) / (dblS + (dblAbsReal-1))))));

		//compute the imaginary part
		if(dblA <= sdblAcross)
		{
			double dblImg1 = 0;

			if(dblAbsReal < 1)
				//Am1 = 0.5d0*((y**2)/(R+(x+1.d0))+(y**2)/(S+(1.d0-x)))
				dblImg1 = 0.5 * (pow(dblAbsImg, 2) / (dblR + (dblAbsReal + 1)) + pow(dblAbsImg, 2) / (dblS + (1 - dblAbsReal)));
			else
				//Am1 = 0.5d0*((y**2)/(R+(x+1.d0))+(S+(x-1.d0)))
				dblImg1 = 0.5 * (pow(dblAbsImg, 2) / (dblR + (dblAbsReal + 1)) + (dblS + (dblAbsReal - 1)));
			//ai = logp1(Am1 + sqrt(Am1*(A+1.d0)))
			dblTemp = dblImg1 + sqrt(dblImg1 * (dblA + 1));
			*_pdblImg = F2C(logp1)(&dblTemp);
		}
		else
			//ai = log(A + sqrt(A**2 - 1.d0))
			*_pdblImg = log(dblA + sqrt(pow(dblA, 2) - 1)); 
	}
	else
	{//evaluation in the special regions ...
		if(dblAbsImg <= dblEpsm * fabs(dblAbsReal - 1))
		{
			if(dblAbsReal < 1)
			{
				*_pdblReal	= asin(dblAbsReal);
				*_pdblImg	= dblAbsImg / sqrt((1 + dblAbsReal) * (1 - dblAbsReal));
			}
			else
			{
				*_pdblReal = sdblPi_2;
				if(dblAbsReal <= dblLsup)
				{
					dblTemp		= (dblAbsReal - 1) + sqrt((dblAbsReal - 1) * (dblAbsReal + 1));
					*_pdblImg	= F2C(logp1)(&dblTemp);
				}
				else
					*_pdblImg	= sdblLn2 + log(dblAbsReal);
			}
		}
		else if(dblAbsImg < dblLinf)
		{
			*_pdblReal	= sdblPi_2 - sqrt(dblAbsImg);
			*_pdblImg	= sqrt(dblAbsImg);
		}
		else if((dblEpsm * dblAbsImg - 1 >= dblAbsReal))
		{
			*_pdblReal	= dblAbsReal * dblAbsImg;
			*_pdblImg	= sdblLn2 + log(dblAbsReal);
		}
		else if(dblAbsReal > 1)
		{
			*_pdblReal	= atan(dblAbsReal / dblAbsImg);
			dblTemp		= pow((dblAbsReal / dblAbsImg), 2);
			*_pdblImg	= sdblLn2 + log(dblAbsReal) + 0.5 * F2C(logp1)(&dblTemp);
		}
		else
		{
			double dblTemp2 = sqrt(1 + pow(dblAbsImg, 2));
			*_pdblReal	= dblAbsReal / dblTemp2;
			dblTemp		= 2 * dblAbsImg * (dblAbsImg + dblTemp2);
			*_pdblImg	= 0.5 * F2C(logp1)(&dblTemp);
		}
	}
	*_pdblReal *= iSignReal;
	*_pdblImg *= iSignImg;
}