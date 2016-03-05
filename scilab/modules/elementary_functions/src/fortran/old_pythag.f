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

      double precision function pythag(a, b)
*
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
*
*     AUTHOR
*        Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>, 
*        Thanks to Lydia van Dijk <lvandijk@hammersmith-consulting.com>
*
      implicit none

*     PARAMETERS
      double precision a, b

*     EXTERNAL FUNCTIONS
      integer          isanan
      external         isanan
      double precision dlamch
      external         dlamch

*     CONSTANTS
      double precision r2, r2p1, t2p1
*     These constants depend upon the floating point arithmetic of the
*     machine.  Here, we give them assuming radix 2 and a 53 bits wide
*     mantissa, correspond to IEEE 754 double precision format.  YOU
*     MUST RE-COMPUTE THESE CONSTANTS FOR A MACHINE THAT HAS DIFFERENT
*     CHARACTERISTIC!
*
*     (1) r2 must approximate sqrt(2) to machine precision.  The near
*         floating point from sqrt(2) is exactly:
*
*              r2 = (1.0110101000001001111001100110011111110011101111001101)_2
*                 = (1.4142135623730951454746218587388284504413604736328125)_10
*         sqrt(2) = (1.41421356237309504880168872420969807856967187537694807317...)_10
*
*     (2) r2p1 must approximate 1+sqrt(2) to machine precision.
*         The near floating point is exactly:
*
*          r2p1 = (10.011010100000100111100110011001111111001110111100110)_2
*               = (2.41421356237309492343001693370752036571502685546875)_10
*     sqrt(2)+1 = (2.41421356237309504880168872420969807856967187537694...)_10
*
*     (3) t2p1 must approximate 1+sqrt(2)-r2p1 to machine precision, 
*         this is
*                 1.25371671790502177712854645019908198073176679... 10^(-16)
*         and the near float is exactly:  
*                 (5085679199899093/40564819207303340847894502572032)_10
*          t2p1 = (1.253716717905021735741793363204945859....)_10
*
      parameter (  r2 = 1.41421356237309504d0, 
     $           r2p1 = 2.41421356237309504d0,
     $           t2p1 = 1.25371671790502177d-16)
*     LOCAL VARIABLES
      double precision x, y
      double precision s, t, temp

*     STATIC VARIABLES
      double precision rmax
      save             rmax

      logical          first
      save             first
      data             first /.true./


*     TEXT
*     Initialize rmax with computed largest non-overflowing number
      if (first) then
          rmax = dlamch('o')
          first = .false.
      endif

*     Test for arguments being NaN
      if (isanan(a) .eq. 1) then
          pythag = a
          return
      endif
      if (isanan(b) .eq. 1) then
          pythag = b
          return
      endif

      x = abs(a)
      y = abs(b)

*     Order x and y such that 0 <= y <= x
      if (x .lt. y) then
          temp = x
          x = y
          y = temp
      endif

*     Test for overflowing x
      if (x .gt. rmax) then
          pythag = x
          return
      endif

*     Handle generic case 
      t = x - y
      if (t .ne. x) then
          if (t .gt. y) then
*             2 < x/y < 2/epsm
              s = x / y
              s = s + sqrt(1d0 + s*s)
          else
*             1 <= x/y <= 2
              s = t / y
              t = (2d0 + s) * s
              s = ( ( t2p1 + t/(r2 + sqrt(2d0 + t)) ) + s ) + r2p1
          endif
          pythag = x + y/s
      else
          pythag = x
      endif
      end



