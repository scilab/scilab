/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
#include "elem_common.h"
#include "log.h"
/*log*/
double dlogs(double _dblVal)
{
    return log(_dblVal);
}


double dlog1ps(double _dblVal)
{
    double dblA = -1.0 / 3.0;
    double dblB	= 0.5;

    if (_dblVal < -1)
    {
        //got NaN
        return (_dblVal - _dblVal) / (_dblVal - _dblVal); //NaN
    }
    else if (dblA <= _dblVal && _dblVal <= dblB)
    {
        //use the function log((1+g)/(1-g)) with g = x/(x + 2)
        double dblTemp = 0;
        dblTemp = _dblVal / ( _dblVal + 2);
        return lnp1m1(dblTemp);
    }
    else
    {
        //use the standard formula
        return dlogs(_dblVal + 1);
    }
}

/*log complex*/
void wlog(double _dblRealIn, double _dblImgIn, double* _pdblRealOut, double* _pdblImgOut)
{
    static double sdblR2	= 1.41421356237309504;

    double dblRMax = getOverflowThreshold();
    double dblLInf = sqrt(getUnderflowThreshold());
    double dblLSup = sqrt(0.5 * dblRMax);

    double dblAbsReal	= fabs(_dblRealIn);
    double dblAbsImg	= fabs(_dblImgIn);

    *_pdblImgOut = datan2s(_dblImgIn, _dblRealIn);

    if (_dblImgIn > _dblRealIn)
    {
        //switch Real part and Imaginary part
        double dblTemp	= dblAbsReal;
        dblAbsReal		= dblAbsImg;
        dblAbsImg		= dblTemp;
    }

    if ((0.5 <= dblAbsReal) && (dblAbsReal <= sdblR2))
    {
        *_pdblRealOut = 0.5 * dlog1ps((dblAbsReal - 1) * (dblAbsReal + 1) + dblAbsImg * dblAbsImg);
    }
    else if (dblLInf < dblAbsImg && dblAbsReal < dblLSup)
    {
        *_pdblRealOut = 0.5 * dlogs(dblAbsReal * dblAbsReal + dblAbsImg * dblAbsImg);
    }
    else if (dblAbsReal > dblRMax)
    {
        *_pdblRealOut = dblAbsReal;
    }
    else
    {
        double dblTemp = dpythags(dblAbsReal, dblAbsImg);
        if (dblTemp <= dblRMax)
        {
            *_pdblRealOut = dlogs(dblTemp);
        }
        else //handle rare spurious overflow with :
        {
            double dblTemp2 = dblAbsImg / dblAbsReal;
            *_pdblRealOut = dlogs(dblAbsReal) + 0.5 * dlog1ps(dblTemp2 * dblTemp2);
        }
    }
}

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
    static double sdblC3	= 2.0 / 3.0;
    static double sdblC5	= 2.0 / 5.0;

    double dblS2 = _dblVar * _dblVar;
    if ( dabss(_dblVar) <= sdblE)
    {
        return _dblVar * (2 + dblS2 * (sdblC3 + sdblC5 * dblS2));
    }
    else
    {
        return _dblVar * (2 + dblS2 * (sdblD3 + dblS2 * (sdblD5 + dblS2 * (sdblD7 + dblS2 * (sdblD9 + dblS2 * (sdblD11 + dblS2 * (sdblD13 + dblS2 * sdblD15)))))));
    }
}
