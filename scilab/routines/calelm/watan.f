      subroutine watan(xr,xi,yr,yi)
*
*     PURPOSE
*        watan compute the arctangent of a complex number
*         y = yr + i yi = atan(x), x = xr + i xi
*
*     CALLING LIST / PARAMETERS
*        subroutine watan(xr,xi,yr,yi)
*        double precision xr,xi,yr,yi
*
*        xr,xi: real and imaginary parts of the complex number
*        yr,yi: real and imaginary parts of the result
*               yr,yi may have the same memory cases than xr et xi
*
*     COPYRIGHT (C) 2001 Bruno Pincon and Lydia van Dijk
*        Written by Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr> so
*        as to get more precision.  Also to fix the
*        behavior at the singular points and at the branch cuts.
*        Polished by Lydia van Dijk 
*        <lvandijk@hammersmith-consulting.com>
*     
*     CHANGES : - (Bruno on 2001 May 22) for ysptrk use a 
*                 minimax polynome to enlarge the special
*                 evaluation zone |s| < SLIM. Also rename
*                 this function as lnp1m1.
*               - (Bruno on 2001 June 7) better handling
*                 of spurious over/underflow ; remove
*                 the call to pythag ; better accuracy
*                 in the real part for z near +-i
*
*     EXTERNALS FUNCTIONS
*        dlamch
*        lnp1m1  (at the end of this file)
*
*     ALGORITHM : noting z = a + i*b, we have:
*        Z = yr + yi*b = arctan(z) = (i/2) * log( (i+z)/(i-z) )
*              
*     This function has two branch points at +i and -i and the
*     chosen  branch cuts are the two half-straight lines
*     D1 = [i, i*oo) and D2 = (-i*oo, i].  The function is then
*     analytic in C \ (D1 U D2)). 
*
*     From the definition it follows that:
*
*        yr = 0.5 Arg ( (i+z)/(i-z) )                   (1)
*        yi = 0.5 log (|(i+z)/(i-z)|)                   (2)
*
*     so lim (z -> +- i) yr = undefined (and Nan is logical)
*        lim (z -> +i)   yi = +oo
*        lim (z -> -i)   yi = -oo
*
*     The real part of arctan(z) is discontinuous across D1 and D2
*     and we impose the following definitions:
*         if imag(z) > 1 then
*             Arg(arctan(z)) =  pi/2 (=lim real(z) -> 0+)
*         if imag(z) < 1 then
*             Arg(arctan(z)) = -pi/2 (=lim real(z) -> 0-)
*                 
*
*     Basic evaluation: if we write (i+z)/(i-z) using
*     z = a + i*b, we get:
*
*     i+z    1-(a**2+b**2) + i*(2a)
*     --- =  ---------------------- 
*     i-z       a**2 + (1-b)**2
*
*     then, with r2 = |z|^2 = a**2 + b**2 :
*  
*     yr = 0.5 * Arg(1-r2 + (2*a)*i)
*        = 0.5 * atan2(2a, (1-r2))                      (3)
*
*     This formula is changed when r2 > RMAX (max pos float)
*     and also when |1-r2| and |a| are near 0 (see comments
*     in the code).
*
*     After some math:
*             
*     yi = 0.25 * log( (a**2 + (b + 1)**2) /
*                      (a**2 + (b - 1)**2) )            (4)     
*               
*     Evaluation for "big" |z|
*     ------------------------
*
*     If |z| is "big", the direct evaluation of yi by (4) may
*     suffer of innaccuracies and of spurious overflow.  Noting 
*     that  s = 2 b / (1 + |z|**2), we have:
*
*     yi = 0.25 log ( (1 + s)/(1 - s) )                 (5)
*
*                                3        5    
*     yi = 0.25*( 2 * ( s + 1/3 s  + 1/5 s  + ... ))
*
*     yi = 0.25 * lnp1m1(s)    if  |s| < SLIM
*
*     So if |s| is less than SLIM we switch to a special
*     evaluation done by the function lnp1m1. The 
*     threshold value SLIM is choosen by experiment 
*     (with the Pari-gp software). For |s| 
*     "very small" we used a truncated taylor dvp, 
*     else a minimax polynome (see lnp1m1).
*
*     To avoid spurious overflows (which result in spurious
*     underflows for s) in computing s with s= 2 b / (1 + |z|**2)
*     when |z|^2 > RMAX (max positive float) we use :
*
*            s = 2d0 / ( (a/b)*a + b )
*     
*     but if |b| = Inf  this formula leads to NaN when
*     |a| is also Inf. As we have :
*
*            |s| <= 2 / |b|
*
*     we impose simply : s = 0  when |b| = Inf
*
*     Evaluation for z very near to i or -i:
*     --------------------------------------
*     Floating point numbers of the form a+i or a-i with 0 <
*     a**2 < tiny (approximately 1d-308) may lead to underflow
*     (i.e., a**2 = 0) and the logarithm will break formula (4).
*     So we switch to the following formulas:
*
*     If b = +-1 and |a| < sqrt(tiny) approximately 1d-150 (say)
*     then (by using that a**2 + 4 = 4 in machine for such a):
*         
*         yi = 0.5 * log( 2/|a| )   for b=1
* 
*         yi = 0.5 * log( |a|/2 )   for b=-1
*
*     finally: yi = 0.5 * sign(b) * log( 2/|a| )     
*              yi = 0.5 * sign(b) * (log(2) - log(|a|)) (6)
*
*     The last trick is to avoid overflow for |a|=tiny!  In fact
*     this formula may be used until a**2 + 4 = 4 so that the
*     threshold value may be larger.
*
      implicit none
*
      include '../stack.h'
*
      double precision xr, xi, yr, yi

*     EXTERNAL
      external         dlamch, lnp1m1
      double precision dlamch, lnp1m1
*
      double precision a, b, r2, s, SLIM, ALIM, TOL, LN2
      parameter (SLIM = 0.2d0,
     $           ALIM = 1.d-150,
     $           TOL  = 0.3d0,
     $           LN2  = 0.69314718055994531d0)

*     STATIC VAR
      logical first
      save    first
      data    first /.true./
      double precision RMAX, HALFPI
      save             RMAX, HALFPI

      if (first) then
         RMAX = dlamch('O')
         first = .false.
         HALFPI = 2.d0*atan(1.d0)
      endif

*     Avoid problems due to sharing the same memory locations by
*     xr, yr and xi, yi.
      a = xr
      b = xi
*
      if (b .eq. 0d0) then
*        z is real
         yr = atan(xr)
         yi = 0d0
      else
*        z is complex
*        (1) Compute the imaginary part of arctan(z)
         r2 = a*a + b*b
         if (r2 .gt. RMAX) then
            if ( abs(b) .gt. RMAX ) then
*              |b| is Inf => s = 0
               s = 0.d0
            else
*              try to avoid the spurious underflow in s when |b| is not
*              negligible with respect to |a|
               s = 1d0 / ( ((0.5d0*a)/b)*a + 0.5d0*b )
            endif
         else
            s = 2d0*b / (1d0 + r2)
         endif

         if (abs(s) .lt. SLIM) then
*           s is small: |s| < SLIM  <=>  |z| outside the following disks:
*           D+ = D(center = [0;  1/slim], radius = sqrt(1/slim**2 - 1)) if b > 0
*           D- = D(center = [0; -1/slim], radius = sqrt(1/slim**2 - 1)) if b < 0
*           use the special evaluation of log((1+s)/(1-s)) (5)
            yi = 0.25d0*lnp1m1(s)
         else
*           |s| >= SLIM  => |z| is inside D+ or D-
            if ((abs(b) .eq. 1d0) .and. (abs(a) .le. ALIM)) then
*              z is very near +- i : use formula (6)
               yi = sign(0.5d0, b) * (LN2 - log(abs(a)))
            else
*              use formula (4)
               yi = 0.25d0 * log( (a*a + (b + 1d0)*(b + 1d0)) /
     $                            (a*a + (b - 1d0)*(b - 1d0)) )
            endif
         endif

*        (2) Compute the real part of arctan(z)
         if (a .eq. 0d0) then
*           z is purely imaginary
            if (abs(b) .gt. 1d0) then
*              got sign(b) * pi/2
               yr = sign(1d0,b) * HALFPI
            elseif (abs(b) .eq. 1d0) then
*              got a Nan with 0/0
               yr = (a - a) / (a - a)
            else
               yr = 0d0
            endif
         elseif (r2 > RMAX) then
*           yr is necessarily very near sign(a)* pi/2 
            yr = sign(1.d0, a) * HALFPI
         elseif ( abs(1.d0 - r2) + abs(a) .le.TOL ) then
*           |b| is very near 1 (and a is near 0)  some
*           cancellation occur in the (next) generic formula 
            yr = 0.5d0 * atan2(2d0*a, (1.d0-b)*(1.d0+b) - a*a)
         else
*           generic formula
            yr = 0.5d0 * atan2(2d0*a, 1d0 - r2)
         endif
      endif
      end


      double precision function lnp1m1(s)
      implicit none
      double precision s
*
*     PURPOSE :  Compute   v = log ( (1 + s)/(1 - s) )
*        for small s, this is for |s| < SLIM = 0.20
*
*     ALGORITHM : 
*     1/ if |s| is "very small" we use a truncated
*        taylor dvp (by keeping 3 terms) from : 
*                               2        4          6
*        t = 2 * s * ( 1 + 1/3 s  + 1/5 s  + [ 1/7 s  + ....] )
*                               2        4      
*        t = 2 * s * ( 1 + 1/3 s  + 1/5 s  + er)
*          
*        The limit E until we use this formula may be simply
*        gotten so that the negliged part er is such that :
*                                    2        4
*        (#) er <= epsm * ( 1 + 1/3 s  + 1/5 s )   for all |s|<= E
*
*        As  er  = 1/7 s^6 + 1/9 s^8 + ... 
*            er <= 1/7 * s^6 ( 1 + s^2 + s^4 + ...) = 1/7  s^6/(1-s^2)
*
*        the inequality (#) is forced if :
*
*        1/7  s^6 / (1-s^2)  <= epsm * ( 1 + 1/3 s^2  + 1/5 s^4 )
*
*        s^6 <= 7 epsm * (1 - 2/3 s^2 - 3/15 s^4 - 1/5 s^6)
*
*        So that E is very near (7 epsm)^(1/6) (approximately 3.032d-3):
*
*     2/ For larger |s| we used a minimax polynome :
*
*        yi = s * (2  + d3 s^3 + d5 s^5 .... + d13 s^13 + d15 s^15)
*
*        This polynome was computed (by some remes algorithm) following 
*        (*) the sin(x) example (p 39) of the book :
*       
*         "ELEMENTARY FUNCTIONS"
*         "Algorithms and implementation"
*         J.M. Muller (Birkhauser)
*
*        (*) without the additionnal raffinement to get the first coefs
*         very near floating point numbers)
*
      double precision s2
      double precision E, C3, C5
      parameter       (E = 3.032d-3, C3  = 2d0 / 3d0, C5  = 2d0 / 5d0)

*     minimax poly coefs
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

*
*   a log(1+x) function for scilab .... 
*                                      
*
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
*        got NaN
         logp1 = (x - x)/(x - x)
      elseif ( a .le. x .and. x .le. b ) then
*        use the function log((1+g)/(1-g)) with g = x/(x + 2)
         g = x/(x + 2.d0)
         logp1 = lnp1m1(g)
      else
*        use the standard formula
         logp1 = log(x + 1.d0)
      endif

      end












