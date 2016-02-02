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

      subroutine wsqrt(xr,xi,yr,yi)
*
*     PURPOSE
*        wsqrt compute the square root of a complex number
*        y = yr + i yi = sqrt(x), x = xr + i xi
*
*     CALLING LIST / PARAMETERS
*        subroutine wsqrt(xr,xi,yr,yi)
*        double precision xr,xi,yr,yi
*
*        xr,xi: real and imaginary parts of the complex number
*        yr,yi: real and imaginary parts of the result
*               yr,yi may have the same memory cases than xr et xi
*
*     ALGORITHM
*        essentially the classic one which consists in
*        choosing the good formula such as avoid cancellation ;
*        Also rare spurious overflow are treated with a
*        "manual" method. For some more "automated" methods
*        (but currently difficult to implement in a portable
*        way) see :
*
*          Hull, Fairgrieve, Tang,
*          "Implementing Complex Elementary Functions Using
*          Exception Handling", ACM TOMS, Vol. 20 (1994), pp 215-244
*
*        for xr > 0 :
*          yr = sqrt(2( xr + sqrt( xr^2 + xi^2)) )/ 2
*          yi = xi / sqrt(2(xr + sqrt(xr^2 + xi^2)))
*
*        and for xr < 0 :
*          yr = |xi| / sqrt( 2( -xr + sqrt( xr^2 + xi^2 )) )
*          yi = sign(xi) sqrt(2(-xr + sqrt( xr^2 + xi^2))) / 2
*
*        for xr = 0 use
*          yr = sqrt(0.5)*sqrt(|xi|)  when |xi| is such that 0.5*|xi| may underflow
*             = sqrt(0.5*|xi|)        else
*          yi = sign(xi) yr
*
*        Noting t = sqrt( 2( |xr| + sqrt( xr^2 + yr^2)) )
*                 = sqrt( 2( |xr| + pythag(xr,xi) ) )
*        it comes :
*
*          for xr > 0   |  for xr < 0
*         --------------+---------------------
*           yr = 0.5*t  |  yr =  |xi| / t
*           yi = xi / t |  yi = sign(xi)*0.5* t
*
*        as the function pythag must not underflow (and overflow only
*        if sqrt(x^2+y^2) > rmax) only spurious (rare) case of overflow
*        occurs in which case a scaling is done.
*
*     AUTHOR
*        Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
*
      implicit none

*     PARAMETER
      double precision xr, xi, yr, yi

*     LOCAL VAR
      double precision a, b, t
*     EXTERNAL
      double precision pythag, dlamch
      external         pythag, dlamch
      integer          isanan
      external         isanan

*     STATIC VAR
      logical first
 	double precision RMAX, BRMIN
      save    first
      data    first /.true./
      save             RMAX, BRMIN


      if (first) then
         RMAX = dlamch('O')
         BRMIN = 2.d0*dlamch('U')
         first = .false.
      endif

      a = xr
      b = xi

      if ( a .eq. 0.d0 ) then
*        pure imaginary case
         if ( abs(b) .ge. BRMIN ) then
            yr = sqrt(0.5d0*abs(b))
         else
            yr = sqrt(abs(b))*sqrt(0.5d0)
         endif
         yi = sign(1.d0, b)*yr

      elseif ( abs(a).le.RMAX .and. abs(b).le.RMAX ) then
*        standard case : a (not zero) and b are finite
         t = sqrt(2.d0*(abs(a) + pythag(a,b)))
*        overflow test
         if ( t .gt. RMAX ) goto 100
*        classic switch to get the stable formulas
         if ( a .ge. 0.d0 ) then
            yr = 0.5d0*t
            yi = b/t
         else
            yr = abs(b)/t
            if ( b .ge. 0 ) then
               yi = 0.5d0*t
            else
               yi = -0.5d0*t
            endif
         endif
      else
*        Here we treat the special cases where a and b are +- 00 or NaN.
*        The following is the treatment recommended by the C99 standard
*        with the simplification of returning NaN + i NaN if the
*        the real part or the imaginary part is NaN (C99 recommends
*        something more complicated)
         if ( (isanan(a) .eq. 1) .or. (isanan(b) .eq. 1) ) then
*           got NaN + i NaN
            yr = a + b
            yi = yr
         elseif ( abs(b) .gt. RMAX ) then
*           case a +- i oo -> result must be +oo +- i oo  for all a (finite or not)
            yr = abs(b)
            yi = b
         elseif ( a .lt. -RMAX ) then
*           here a is -Inf and b is finite
            yr = 0.d0
            if ( b .ge. 0 ) then
               yi = 1.d0*abs(a)
            else
               yi = -1.d0*abs(a)
            endif
         else
*           here a is +Inf and b is finite
            yr = a
            yi = 0.d0
         endif
      endif

      return

*     handle (spurious) overflow by scaling a and b
 100  continue
      a = a/16.d0
      b = b/16.d0
      t = sqrt(2.d0*(abs(a) + pythag(a,b)))
      if ( a .ge. 0.d0 ) then
         yr = 2.d0*t
         yi = 4.d0*b/t
      else
         yr = 4.d0*abs(b)/t
         if ( b .ge. 0 ) then
            yi = 2.d0*t
         else
            yi = -2.d0*t
         endif
      endif

      end



