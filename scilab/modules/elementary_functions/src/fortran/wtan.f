c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) Bruno Pincon
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.

      subroutine wtan(xr,xi,yr,yi)
*
*     PURPOSE
*        wtan compute the tangent of a complex number
*        y = yr + i yi = tan(x), x = xr + i xi
*
*     CALLING LIST / PARAMETERS
*        subroutine wtan(xr,xi,yr,yi)
*        double precision xr,xi,yr,yi
*
*        xr,xi: real and imaginary parts of the complex number
*        yr,yi: real and imaginary parts of the result
*               yr,yi may have the same memory cases than xr et xi
*
*     ALGORITHM
*        based on the formula :
*
*                         0.5 sin(2 xr) +  i 0.5 sinh(2 xi)
*        tan(xr + i xi) = ---------------------------------
*                             cos(xr)^2  + sinh(xi)^2
*        
*        noting  d = cos(xr)^2 + sinh(xi)^2, we have :
*
*                yr = 0.5 * sin(2 * xr) / d       (1)
*
*                yi = 0.5 * sinh(2 * xi) / d      (2)
*
*        to avoid spurious overflows in computing yi with 
*        formula (2) (which results in NaN for yi)
*        we use also the following formula :
*
*                yi = sign(xi)   when |xi| > LIM  (3)
*
*        Explanations for (3) :
*
*        we have d = sinh(xi)^2 ( 1 + (cos(xr)/sinh(xi))^2 ), 
*        so when : 
*
*           (cos(xr)/sinh(xi))^2 < epsm   ( epsm = max relative error
*                                          for coding a real in a f.p.
*                                          number set F(b,p,emin,emax) 
*                                             epsm = 0.5 b^(1-p) )
*        which is forced  when :
*
*            1/sinh(xi)^2 < epsm   (4) 
*        <=> |xi| > asinh(1/sqrt(epsm)) (= 19.06... in ieee 754 double)
*
*        sinh(xi)^2 is a good approximation for d (relative to the f.p. 
*        arithmetic used) and then yr may be approximate with :
*
*         yr = cosh(xi)/sinh(xi) 
*            = sign(xi) (1 + exp(-2 |xi|))/(1 - exp(-2|xi|)) 
*            = sign(xi) (1 + 2 u + 2 u^2 + 2 u^3 + ...)
*
*        with u = exp(-2 |xi|)). Now when :
*
*            2 exp(-2|xi|) < epsm  (2)  
*        <=> |xi| > 0.5 * log(2/epsm) (= 18.71... in ieee 754 double)
* 
*        sign(xi)  is a good approximation for yr.
*
*        Constraint (1) is stronger than (2) and we take finaly 
*
*        LIM = 1 + log(2/sqrt(epsm)) 
*
*        (log(2/sqrt(epsm)) being very near asinh(1/sqrt(epsm))
*
*     AUTHOR
*        Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
*
      implicit none

*     PARAMETER
      double precision xr, xi, yr, yi

*     LOCAL VAR
      double precision sr, si, d
*     EXTERNAL
      double precision dlamch
      external         dlamch

*     STATIC VAR
      logical first
	double precision LIM

      save    first
      data    first /.true./
            save             LIM
      

      if (first) then
*        epsm is gotten with dlamch('e')
         LIM = 1.d0 + log(2.d0/sqrt(dlamch('e')))
         first = .false.
      endif

*     (0) avoid memory pb ...
      sr = xr
      si = xi

*     (1) go on ....
      d = cos(sr)**2 + sinh(si)**2
      yr= 0.5d0*sin(2.d0*sr)/d
      if (abs(si) .lt. LIM) then
         yi=0.5d0*sinh(2.d0*si)/d
      else
         yi=sign(1.d0,si)
      endif 

      end




