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
     PURPOSE
        watan compute the arctangent of a complex number
         y = yr + i yi = atan(x), x = xr + i xi

     CALLING LIST / PARAMETERS
        subroutine watan(xr,xi,yr,yi)
        double precision xr,xi,yr,yi

        xr,xi: real and imaginary parts of the complex number
        yr,yi: real and imaginary parts of the result
               yr,yi may have the same memory cases than xr et xi

     COPYRIGHT (C) 2001 Bruno Pincon and Lydia van Dijk
        Written by Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr> so
        as to get more precision.  Also to fix the
        behavior at the singular points and at the branch cuts.
        Polished by Lydia van Dijk
        <lvandijk@hammersmith-consulting.com>

     CHANGES : - (Bruno on 2001 May 22) for ysptrk use a
                 minimax polynome to enlarge the special
                 evaluation zone |s| < SLIM. Also rename
                 this function as lnp1m1.
               - (Bruno on 2001 June 7) better handling
                 of spurious over/underflow ; remove
                 the call to pythag ; better accuracy
                 in the real part for z near +-i

     EXTERNALS FUNCTIONS
        dlamch
        lnp1m1  (at the end of this file)

     ALGORITHM : noting z = a + i*b, we have:
        Z = yr + yi*b = arctan(z) = (i/2) * log( (i+z)/(i-z) )

     This function has two branch points at +i and -i and the
     chosen  branch cuts are the two half-straight lines
     D1 = [i, i*oo) and D2 = (-i*oo, i].  The function is then
     analytic in C \ (D1 U D2)).

     From the definition it follows that:

        yr = 0.5 Arg ( (i+z)/(i-z) )                   (1)
        yi = 0.5 log (|(i+z)/(i-z)|)                   (2)

     so lim (z -> +- i) yr = undefined (and Nan is logical)
        lim (z -> +i)   yi = +oo
        lim (z -> -i)   yi = -oo

     The real part of arctan(z) is discontinuous across D1 and D2
     and we impose the following definitions:
         if imag(z) > 1 then
             Arg(arctan(z)) =  pi/2 (=lim real(z) -> 0+)
         if imag(z) < 1 then
             Arg(arctan(z)) = -pi/2 (=lim real(z) -> 0-)


     Basic evaluation: if we write (i+z)/(i-z) using
     z = a + i*b, we get:

     i+z    1-(a**2+b**2) + i*(2a)
     --- =  ----------------------
     i-z       a**2 + (1-b)**2

     then, with r2 = |z|^2 = a**2 + b**2 :

     yr = 0.5 * Arg(1-r2 + (2*a)*i)
        = 0.5 * atan2(2a, (1-r2))                      (3)

     This formula is changed when r2 > RMAX (max pos float)
     and also when |1-r2| and |a| are near 0 (see comments
     in the code).

     After some math:

     yi = 0.25 * log( (a**2 + (b + 1)**2) /
                      (a**2 + (b - 1)**2) )            (4)

     Evaluation for "big" |z|
     ------------------------

     If |z| is "big", the direct evaluation of yi by (4) may
     suffer of innaccuracies and of spurious overflow.  Noting
     that  s = 2 b / (1 + |z|**2), we have:

     yi = 0.25 log ( (1 + s)/(1 - s) )                 (5)

                                3        5
     yi = 0.25*( 2 * ( s + 1/3 s  + 1/5 s  + ... ))

     yi = 0.25 * lnp1m1(s)    if  |s| < SLIM

     So if |s| is less than SLIM we switch to a special
     evaluation done by the function lnp1m1. The
     threshold value SLIM is chosen by experiment
     (with the Pari-gp software). For |s|
     "very small" we used a truncated taylor dvp,
     else a minimax polynome (see lnp1m1).

     To avoid spurious overflows (which result in spurious
     underflows for s) in computing s with s= 2 b / (1 + |z|**2)
     when |z|^2 > RMAX (max positive float) we use :

            s = 2d0 / ( (a/b)*a + b )

     but if |b| = Inf  this formula leads to NaN when
     |a| is also Inf. As we have :

            |s| <= 2 / |b|

     we impose simply : s = 0  when |b| = Inf

     Evaluation for z very near to i or -i:
     --------------------------------------
     Floating point numbers of the form a+i or a-i with 0 <
     a**2 < tiny (approximately 1d-308) may lead to underflow
     (i.e., a**2 = 0) and the logarithm will break formula (4).
     So we switch to the following formulas:

     If b = +-1 and |a| < sqrt(tiny) approximately 1d-150 (say)
     then (by using that a**2 + 4 = 4 in machine for such a):

         yi = 0.5 * log( 2/|a| )   for b=1

         yi = 0.5 * log( |a|/2 )   for b=-1

     finally: yi = 0.5 * sign(b) * log( 2/|a| )
              yi = 0.5 * sign(b) * (log(2) - log(|a|)) (6)

     The last trick is to avoid overflow for |a|=tiny!  In fact
     this formula may be used until a**2 + 4 = 4 so that the
     threshold value may be larger.
*/
void watan(double _dblReal, double _dblImg, double *_pdblReal, double *_pdblImg)
{
    static double sdblSlim		= 0.2;
    static double sdblAlim		= 1E-150;
    static double sdblTol		= 0.3;
    static double sdblLn2		= 0.6931471805599453094172321;

    double dblRMax				= getOverflowThreshold();
    double dblPi_2				= 2.0 * datans(1);


    //Temporary variables
    double dblR2 = 0;
    double dblS = 0;


    if (_dblImg == 0)
    {
        *_pdblReal	= datans(_dblReal);
        *_pdblImg	= 0;
    }
    else
    {
        dblR2 = _dblReal * _dblReal + _dblImg * _dblImg; // Oo
        if (dblR2 > dblRMax)
        {
            if ( dabss(_dblImg) > dblRMax)
            {
                dblS = 0;
            }
            else
            {
                dblS = 1 / (((0.5 * _dblReal) / _dblImg) * _dblReal + 0.5 * _dblImg );
            }
        }
        else
        {
            dblS = (2 * _dblImg) / (1 + dblR2);
        }

        if (dabss(dblS) < sdblSlim)
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
            if (dabss(dblS) == 1 && dabss(_dblReal) <= sdblAlim)
            {
                //|s| >= SLIM  => |z| is inside D+ or D-
                *_pdblImg = dsigns(0.5, _dblImg) * ( sdblLn2 - log(dabss(_dblReal)));
            }
            else
            {
                *_pdblImg = 0.25 * log((pow(_dblReal, 2) + pow((_dblImg + 1), 2)) / (pow(_dblReal, 2) + pow((_dblImg - 1), 2)));
            }
        }
        if (_dblReal == 0)
        {
            //z is purely imaginary
            if ( dabss(_dblImg) > 1)
            {
                //got sign(b) * pi/2
                *_pdblReal = dsigns(1, _dblImg) * dblPi_2;
            }
            else if ( dabss(_dblImg) == 1)
            {
                //got a Nan with 0/0
                *_pdblReal = (_dblReal - _dblReal) / (_dblReal - _dblReal); // Oo
            }
            else
            {
                *_pdblReal = 0;
            }
        }
        else if (dblR2 > dblRMax)
        {
            //_pdblImg is necessarily very near sign(a)* pi/2
            *_pdblReal = dsigns(1, _dblReal) * dblPi_2;
        }
        else if (dabss(1 - dblR2) + dabss(_dblReal) <= sdblTol)
        {
            //|b| is very near 1 (and a is near 0)  some cancellation occur in the (next) generic formula
            *_pdblReal = 0.5 * atan2(2 * _dblReal, (1 - _dblImg) * (1 + _dblImg) - pow(_dblReal, 2));
        }
        else
        {
            *_pdblReal = 0.5 * atan2(2 * _dblReal, 1 - dblR2);
        }
    }
}