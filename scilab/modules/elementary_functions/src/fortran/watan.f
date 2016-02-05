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
c
      subroutine watan(xr,xi,yr,yi)
c
c     PURPOSE
c        watan compute the arctangent of a complex number
c         y = yr + i yi = atan(x), x = xr + i xi
c
c     CALLING LIST / PARAMETERS
c        subroutine watan(xr,xi,yr,yi)
c        double precision xr,xi,yr,yi
c
c        xr,xi: real and imaginary parts of the complex number
c        yr,yi: real and imaginary parts of the result
c               yr,yi may have the same memory cases than xr et xi
c
c     COPYRIGHT (C) 2001 Bruno Pincon and Lydia van Dijk
c        Written by Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr> so
c        as to get more precision.  Also to fix the
c        behavior at the singular points and at the branch cuts.
c        Polished by Lydia van Dijk
c        <lvandijk@hammersmith-consulting.com>
c
c     CHANGES : - (Bruno on 2001 May 22) for ysptrk use a
c                 minimax polynome to enlarge the special
c                 evaluation zone |s| < SLIM. Also rename
c                 this function as lnp1m1.
c               - (Bruno on 2001 June 7) better handling
c                 of spurious over/underflow ; remove
c                 the call to pythag ; better accuracy
c                 in the real part for z near +-i
c
c     EXTERNALS FUNCTIONS
c        dlamch
c        lnp1m1  (at the end of this file)
c
c     ALGORITHM : noting z = a + i*b, we have:
c        Z = yr + yi*b = arctan(z) = (i/2) * log( (i+z)/(i-z) )
c
c     This function has two branch points at +i and -i and the
c     chosen  branch cuts are the two half-straight lines
c     D1 = [i, i*oo) and D2 = (-i*oo, i].  The function is then
c     analytic in C \ (D1 U D2)).
c
c     From the definition it follows that:
c
c        yr = 0.5 Arg ( (i+z)/(i-z) )                   (1)
c        yi = 0.5 log (|(i+z)/(i-z)|)                   (2)
c
c     so lim (z -> +- i) yr = undefined (and Nan is logical)
c        lim (z -> +i)   yi = +oo
c        lim (z -> -i)   yi = -oo
c
c     The real part of arctan(z) is discontinuous across D1 and D2
c     and we impose the following definitions:
c         if imag(z) > 1 then
c             Arg(arctan(z)) =  pi/2 (=lim real(z) -> 0+)
c         if imag(z) < 1 then
c             Arg(arctan(z)) = -pi/2 (=lim real(z) -> 0-)
c
c
c     Basic evaluation: if we write (i+z)/(i-z) using
c     z = a + i*b, we get:
c
c     i+z    1-(a**2+b**2) + i*(2a)
c     --- =  ----------------------
c     i-z       a**2 + (1-b)**2
c
c     then, with r2 = |z|^2 = a**2 + b**2 :
c
c     yr = 0.5 * Arg(1-r2 + (2*a)*i)
c        = 0.5 * atan2(2a, (1-r2))                      (3)
c
c     This formula is changed when r2 > RMAX (max pos float)
c     and also when |1-r2| and |a| are near 0 (see comments
c     in the code).
c
c     After some math:
c
c     yi = 0.25 * log( (a**2 + (b + 1)**2) /
c                      (a**2 + (b - 1)**2) )            (4)
c
c     Evaluation for "big" |z|
c     ------------------------
c
c     If |z| is "big", the direct evaluation of yi by (4) may
c     suffer of innaccuracies and of spurious overflow.  Noting
c     that  s = 2 b / (1 + |z|**2), we have:
c
c     yi = 0.25 log ( (1 + s)/(1 - s) )                 (5)
c
c                                3        5
c     yi = 0.25*( 2 * ( s + 1/3 s  + 1/5 s  + ... ))
c
c     yi = 0.25 * lnp1m1(s)    if  |s| < SLIM
c
c     So if |s| is less than SLIM we switch to a special
c     evaluation done by the function lnp1m1. The
c     threshold value SLIM is chosen by experiment
c     (with the Pari-gp software). For |s|
c     "very small" we used a truncated taylor dvp,
c     else a minimax polynome (see lnp1m1).
c
c     To avoid spurious overflows (which result in spurious
c     underflows for s) in computing s with s= 2 b / (1 + |z|**2)
c     when |z|^2 > RMAX (max positive float) we use :
c
c            s = 2d0 / ( (a/b)*a + b )
c
c     but if |b| = Inf  this formula leads to NaN when
c     |a| is also Inf. As we have :
c
c            |s| <= 2 / |b|
c
c     we impose simply : s = 0  when |b| = Inf
c
c     Evaluation for z very near to i or -i:
c     --------------------------------------
c     Floating point numbers of the form a+i or a-i with 0 <
c     a**2 < tiny (approximately 1d-308) may lead to underflow
c     (i.e., a**2 = 0) and the logarithm will break formula (4).
c     So we switch to the following formulas:
c
c     If b = +-1 and |a| < sqrt(tiny) approximately 1d-150 (say)
c     then (by using that a**2 + 4 = 4 in machine for such a):
c
c         yi = 0.5 * log( 2/|a| )   for b=1
c
c         yi = 0.5 * log( |a|/2 )   for b=-1
c
c     finally: yi = 0.5 * sign(b) * log( 2/|a| )
c              yi = 0.5 * sign(b) * (log(2) - log(|a|)) (6)
c
c     The last trick is to avoid overflow for |a|=tiny!  In fact
c     this formula may be used until a**2 + 4 = 4 so that the
c     threshold value may be larger.
c
      implicit none
c
c
      double precision xr, xi, yr, yi

c     EXTERNAL
      external         dlamch, lnp1m1
      double precision dlamch, lnp1m1
c
      double precision a, b, r2, s, SLIM, ALIM, TOL, LN2
      parameter (SLIM = 0.2d0,
     $           ALIM = 1.d-150,
     $           TOL  = 0.3d0,
     $           LN2  = 0.69314718055994531d0)

c     STATIC VAR
      logical first
      double precision RMAX, HALFPI

      save    first
      data    first /.true./
	save             RMAX, HALFPI

      if (first) then
         RMAX = dlamch('O')
         first = .false.
         HALFPI = 2.d0*atan(1.d0)
      endif

c     Avoid problems due to sharing the same memory locations by
c     xr, yr and xi, yi.
      a = xr
      b = xi
c
      if (b .eq. 0d0) then
c        z is real
         yr = atan(xr)
         yi = 0d0
      else
c        z is complex
c        (1) Compute the imaginary part of arctan(z)
         r2 = a*a + b*b
         if (r2 .gt. RMAX) then
            if ( abs(b) .gt. RMAX ) then
c              |b| is Inf => s = 0
               s = 0.d0
            else
c              try to avoid the spurious underflow in s when |b| is not
c              negligible with respect to |a|
               s = 1d0 / ( ((0.5d0*a)/b)*a + 0.5d0*b )
            endif
         else
            s = 2d0*b / (1d0 + r2)
         endif

         if (abs(s) .lt. SLIM) then
c           s is small: |s| < SLIM  <=>  |z| outside the following disks:
c           D+ = D(center = [0;  1/slim], radius = sqrt(1/slim**2 - 1)) if b > 0
c           D- = D(center = [0; -1/slim], radius = sqrt(1/slim**2 - 1)) if b < 0
c           use the special evaluation of log((1+s)/(1-s)) (5)
            yi = 0.25d0*lnp1m1(s)
         else
c           |s| >= SLIM  => |z| is inside D+ or D-
            if ((abs(b) .eq. 1d0) .and. (abs(a) .le. ALIM)) then
c              z is very near +- i : use formula (6)
               yi = sign(0.5d0, b) * (LN2 - log(abs(a)))
            else
c              use formula (4)
               yi = 0.25d0 * log( (a*a + (b + 1d0)*(b + 1d0)) /
     $                            (a*a + (b - 1d0)*(b - 1d0)) )
            endif
         endif

c        (2) Compute the real part of arctan(z)
         if (a .eq. 0d0) then
c           z is purely imaginary
            if (abs(b) .gt. 1d0) then
c              got sign(b) * pi/2
               yr = sign(1d0,b) * HALFPI
            elseif (abs(b) .eq. 1d0) then
c              got a Nan with 0/0
               yr = (a - a) / (a - a)
            else
               yr = 0d0
            endif
         elseif (r2 .gt. RMAX) then
c           yr is necessarily very near sign(a)* pi/2
            yr = sign(1.d0, a) * HALFPI
         elseif ( abs(1.d0 - r2) + abs(a) .le.TOL ) then
c           |b| is very near 1 (and a is near 0)  some
c           cancellation occur in the (next) generic formula
            yr = 0.5d0 * atan2(2d0*a, (1.d0-b)*(1.d0+b) - a*a)
         else
c           generic formula
            yr = 0.5d0 * atan2(2d0*a, 1d0 - r2)
         endif
      endif
      end


      double precision function lnp1m1(s)
      implicit none
      double precision s
c
c     PURPOSE :  Compute   v = log ( (1 + s)/(1 - s) )
c        for small s, this is for |s| < SLIM = 0.20
c
c     ALGORITHM :
c     1/ if |s| is "very small" we use a truncated
c        taylor dvp (by keeping 3 terms) from :
c                               2        4          6
c        t = 2 * s * ( 1 + 1/3 s  + 1/5 s  + [ 1/7 s  + ....] )
c                               2        4
c        t = 2 * s * ( 1 + 1/3 s  + 1/5 s  + er)
c
c        The limit E until we use this formula may be simply
c        gotten so that the negliged part er is such that :
c                                    2        4
c        (#) er <= epsm * ( 1 + 1/3 s  + 1/5 s )   for all |s|<= E
c
c        As  er  = 1/7 s^6 + 1/9 s^8 + ...
c            er <= 1/7 * s^6 ( 1 + s^2 + s^4 + ...) = 1/7  s^6/(1-s^2)
c
c        the inequality (#) is forced if :
c
c        1/7  s^6 / (1-s^2)  <= epsm * ( 1 + 1/3 s^2  + 1/5 s^4 )
c
c        s^6 <= 7 epsm * (1 - 2/3 s^2 - 3/15 s^4 - 1/5 s^6)
c
c        So that E is very near (7 epsm)^(1/6) (approximately 3.032d-3):
c
c     2/ For larger |s| we used a minimax polynome :
c
c        yi = s * (2  + d3 s^3 + d5 s^5 .... + d13 s^13 + d15 s^15)
c
c        This polynome was computed (by some remes algorithm) following
c        (*) the sin(x) example (p 39) of the book :
c
c         "ELEMENTARY FUNCTIONS"
c         "Algorithms and implementation"
c         J.M. Muller (Birkhauser)
c
c        (*) without the additional raffinement to get the first coefs
c         very near floating point numbers)
c
      double precision s2
      double precision E, C3, C5
      parameter       (E = 3.032d-3, C3  = 2d0 / 3d0, C5  = 2d0 / 5d0)

c     minimax poly coefs
      double precision D3, D5, D7, D9, D11, D13, D15
      parameter (
     $   D3 = 0.66666666666672679472d0, D5 = 0.39999999996176889299d0,
     $   D7 = 0.28571429392829380980d0, D9 = 0.22222138684562683797d0,
     $   D11= 0.18186349187499222459d0, D13= 0.15250315884469364710d0,
     $   D15= 0.15367270224757008114d0 )

      s2 = s * s
      if (abs(s) .le. E) then
          lnp1m1 = s * (2d0 + s2*(C3 + C5*s2))
      else
          lnp1m1 = s * (2.d0 + s2*(D3 + s2*(D5 + s2*(
     $             D7 + s2*(D9 + s2*(D11 + s2*(D13 + s2*D15)))))))
      endif
      end

c
c   a log(1+x) function for scilab ....
c
c
      double precision function logp1(x)
      implicit none
      double precision x

      double precision g
      double precision a, b
      parameter      ( a = -1d0/3d0,
     $                 b =  0.5d0 )

      double precision lnp1m1
      external         lnp1m1

      if ( x .lt. -1.d0 ) then
c        got NaN
         logp1 = (x - x)/(x - x)
      elseif ( a .le. x .and. x .le. b ) then
c        use the function log((1+g)/(1-g)) with g = x/(x + 2)
         g = x/(x + 2.d0)
         logp1 = lnp1m1(g)
      else
c        use the standard formula
         logp1 = log(x + 1.d0)
      endif

      end












