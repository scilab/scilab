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
#include "numericconstants_interface.h"

/*tan*/
double dtans(double _dblVal)
{
    return tan(_dblVal);
}

/*
ALGORITHM
   based on the formula :

                    0.5 sin(2 xr) +  i 0.5 sinh(2 xi)
   tan(xr + i xi) = ---------------------------------
                        cos(xr)^2  + sinh(xi)^2

   noting  d = cos(xr)^2 + sinh(xi)^2, we have :

           yr = 0.5 * sin(2 * xr) / d       (1)

           yi = 0.5 * sinh(2 * xi) / d      (2)

   to avoid spurious overflows in computing yi with
   formula (2) (which results in NaN for yi)
   we use also the following formula :

           yi = sign(xi)   when |xi| > LIM  (3)

   Explanations for (3) :

   we have d = sinh(xi)^2 ( 1 + (cos(xr)/sinh(xi))^2 ),
   so when :

      (cos(xr)/sinh(xi))^2 < epsm   ( epsm = max relative error
                                     for coding a real in a f.p.
                                     number set F(b,p,emin,emax)
                                        epsm = 0.5 b^(1-p) )
   which is forced  when :

       1/sinh(xi)^2 < epsm   (4)
   <=> |xi| > asinh(1/sqrt(epsm)) (= 19.06... in ieee 754 double)

   sinh(xi)^2 is a good approximation for d (relative to the f.p.
   arithmetic used) and then yr may be approximate with :

    yr = cosh(xi)/sinh(xi)
       = sign(xi) (1 + exp(-2 |xi|))/(1 - exp(-2|xi|))
       = sign(xi) (1 + 2 u + 2 u^2 + 2 u^3 + ...)

   with u = exp(-2 |xi|)). Now when :

       2 exp(-2|xi|) < epsm  (2)
   <=> |xi| > 0.5 * log(2/epsm) (= 18.71... in ieee 754 double)

   sign(xi)  is a good approximation for yr.

   Constraint (1) is stronger than (2) and we take finaly

   LIM = 1 + log(2/sqrt(epsm))

   (log(2/sqrt(epsm)) being very near asinh(1/sqrt(epsm))

AUTHOR
   Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>

TRANSLATE TO C
	Antoine Elias

	PARAMETER :
	Input :
	xr	-> _dblRealIn
	xi	-> _dblImgIn
	yr	-> _pdblRealOut
	yi	-> _pdblImgOut
*/
void ztans(double _dblRealIn, double _dblImgIn, double* _pdblRealOut, double* _pdblImgOut)
{
    double dblTemp = 0;
    double dblLim = 1 + dlogs(2.0 / dsqrts(nc_eps()));

    dblTemp			= pow(dcoss(_dblRealIn), 2) + pow(dsinhs(_dblImgIn), 2);
    *_pdblRealOut	= 0.5 * dsins(2 * _dblRealIn) / dblTemp;
    if (dabss(_dblImgIn) < dblLim)
    {
        *_pdblImgOut = 0.5 * dsinhs(2 * _dblImgIn) / dblTemp;
    }
    else
    {
        *_pdblImgOut = dsigns(1, _dblImgIn);
    }
}
