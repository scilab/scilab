      subroutine watan(xr,xi,yr,yi)
*!purpose
*     watan compute the arctangent of a complex number
*!calling list
*     subroutine watan(xr,xi,yr,yi)
*     double precision xr,xi,yr,yi
*
*     xr,xi: real and imaginary parts of the complex number
*     yr,yi: real and imaginary parts of the result
*            yr,yi may have the same memory cases than xr et xi
*
*     Copyright (C) 2001 Bruno Pincon and Lydia van Dijk
*
*!functions used
*     pythag
*     ysptrk  (at the end of this file)
*
*     Written by Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr> so
*     as to get more precision if |z| is big.  Also to fix the
*     behavior at the singular point and at the branches cuts.
*     Polished by Lydia van Dijk 
*     <lvandijk@hammersmith-consulting.com>
*
*  algorithm : noting z = a + i*b, we have:
*  ---------            
*        Z = yr + yi*b = arctan(z) = (i/2) * log( (i+z)/(i-z) )
*              
*     This function has two branch points at +i and -i and the
*     chosen branch cuts are the two half-straight lines
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
*     I do not know whether this is a good choice as the first
*     one does not correspond to Scilab's the old atan(z).
*
*     Basic evaluation: if we write (i+z)/(i-z) using
*     z = a + i*b, we get:
*
*     i+z    1-(a**2+b**2) + i*(2a)
*     --- =  ---------------------- 
*     i-z       a**2 + (1-b)**2
*
*     then, with r = |z| = sqrt(a**2 + b**2):
*  
*     yr = 0.5 * Arg(1-r**2 + i*2)
*        = 0.5 * atan2(2a, (1-r)*(1+r))                 (3)
*
*     Expression (1-r)*(1+r) may be a little superior 
*     to 1-r**2 for floating point evaluation.
*     
*     After some math:
*             
*     yi = 0.25 * log( (a**2 + (b + 1)**2) /
*                      (a**2 + (b - 1)**2) )            (4)     
*               
*     Evaluation for "big" |z|
*     ------------------------

*     If |z| is big, the direct evaluation of yi by (4) may
*     suffer of cancellation.  Noting that
*     s = 2 b / (1 + |z|**2), we have:
*
*     yi = 0.25 log ( (1 + s)/(1 - s) )
*
*                           3        5        7
*     yi = 0.5 * ( s + 1/3 s  + 1/5 s  + 1/7 s  +
*
*                         9         11    13  
*                    1/9 s  + 1/11 s + O(s  ))
*
*                               2        4        6  
*     yi near 0.5 * s (1 + 1/3 s  + 1/5 s  + 1/7 s  +
*
*                             8         10
*                        1/9 s  + 1/11 s  )             (5)
*     
*     Depending the size of |s| we must switch to a special
*     evaluation to avoid cancellation.  This is done by
*     function ysptrk.  The threshold value is chosen so that
*     adding a supplementary term in the preceding expansion
*     changes nothing (for details see ysptrk): roughly this
*     corresponds to 1/13 s**12 <= epsm and thus SLIM = 0.056.
*     Now, some experience is necessary to validate or correct
*     this:
*
*     - Is (4) precise enough for |s| near SLIM?  If not, SLIM
*       must be larger, but we may certainly add another term,
*       1/13 s**12, to (3).

*     - is the evaluation of (3) accurate enough?  Other
*       possibilities are
*       (a) to do a Chebychev interpolation or
*       (b) to use a magic formula for log(1+x) (see comments
*           in ysptrk).
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
*     this formula may be used up to a**2 + 4 = 4 so that the
*     threshold value may be larger.
*
      implicit none
*
      include '../stack.h'
*
      double precision xr, xi, yr, yi
      external         pythag, ysptrk
      double precision pythag, ysptrk
*
      double precision a, b, r, s, SLIM, ALIM, LN2
      parameter (SLIM = 0.056d0,
     $           ALIM   = 1.d-150,
     $           LN2    = 0.69314718055994531d0)

*     Avoid problems due to sharing the same memory locations by
*     xr, yr and xi, yi.
      a = xr
      b = xi
*
      if (b .eq. 0d0) then
*         z is real
          yr = atan(xr)
          yi = 0d0
      else
*         z is complex
*         (1) Compute the imaginary part of arctan(z)
          r = pythag(a, b)
          s = 2d0 * b / (1d0 + r*r)
          if (abs(s) .lt. SLIM) then
*             s is small: |s| < SLIM  <=>
*             |z| outside the following disks:
*             D+ = D(center = [0;  1/slim], radius = 1/slim**2 - 1) if b > 0
*             D- = D(center = [0; -1/slim], radius = 1/slim**2 - 1) if b < 0
*             use the special trick (5)
              yi = ysptrk(s)
          else
*             |s| >= SLIM  =>  |z| is small;
              if ((abs(b) .eq. 1d0) .and. (abs(a) .le. ALIM)) then
*                 z is very near +- i : use formula (6)
                  yi = sign(0.5D0, b) * (LN2 - log(abs(a)))
              else
*                 use formula (4)
                  yi = 0.25d0 * log( (a*a + (b + 1d0)*(b + 1d0)) /
     $                               (a*a + (b - 1d0)*(b - 1d0)) )
              endif
          endif

*         (2) Compute the real part of arctan(z)
          if (a .eq. 0d0) then
*             z is purely imaginary
              if (b .gt. 1d0) then
*                 got pi/2 in hoping that it will be the same than 
*                 the scilab %pi/2 which is not sure by using
*                 a parameter constant
                  yr = 2d0 * atan(1d0)
              elseif (b .lt. -1d0) then
*                 got -pi/2
                  yr = -2d0 * atan(1d0)
              elseif (abs(b) .eq. 1d0) then
*                 Got a Nan with 0/0
                  yr = (a - a) / (a - a)
*     yr is NaN now, and |yi| is Inf.  If you want yi to be NaN
*     too, uncomment the next line.
*                  yi = yr
              else
                  yr = 0d0
              endif
          else
              yr = 0.5d0 * atan2(2d0*a, (1d0 + r)*(1d0 - r))
          endif
      endif
      end


      double precision function ysptrk(s)
      implicit none
      double precision s
*
*     Compute:
* 
*     v = 0.25 log ( (1 + s)/(1 - s) )
*
*     for small s, this is for |s| < SLIM
* 
*                         3        5        7        9
*     t = 0.5 *( s + 1/3 s  + 1/5 s  + 1/7 s  + 1/9 s 
*
*                        11     13
*                + 1/11 s  + O(s  ))
*
*     t = 0.5 s * ( 1 + 1/3 s**2 + 1/5 s**4 + 1/7 s**6 + 1/9 s**8 +
*                   1/11 s**10 + O(s**12))
*          
*     So:
*     (1) If 1/3 s**2 <= epsm
*         <=> |s| <= sqrt(3*epsm) = E1 (approximately 1.825d-8),
*         we have numerically 1 + 1/3 s**2 = 1 and the
*         evaluation reduces to:
*
*             yi = s/2  for |s| <= E1
* 
*     (2) For larger |s| we do not need the term
*         1/5 s**4 ad 1 + * 1/5 s**4 reduces to 1 in machine,
*         this is, if |s| <= E2 = (5*epsm)**(1/4)
*         (approximately 1.5349d-4),
*
*             yi = 0.5 s *( 1 + 1/3 s**2)   for E1 < |s| <= E2
*
*     (3) The next limit is E3 = (7 epsm)**(1/6)
*         (approximately 3.032d-3):
*
*             yi = 0.5 s *( 1 + 1/3 s**2 + 1/5 s**4)
*                                       for E2 < |s| <= E3
*
*     (4) We evaluate the expansion up to 1/11 u**10 for larger
*         |s|
*
*     Remark: There are some nice tricks to evaluate log(1+x)
*     with precision for small |x| on a IEEE 754 machine (see
*     the Golberg 's paper "What every scientist should know
*     about floating point arihmetic") and with those tricks
*     this function may be written simpler (with t =
*     0.5*(log(1+s)-log(1-s)), but the compilation of such
*     tricks does not suffer any -O flag and I do not know how
*     to do that within Scilab.
*                  
      double precision as, s2, E1, E2, E3
      parameter (E1 = 1.825d-8, E2 = 1.5349d-4, E3 = 3.032d-3)
      double precision C3, C5, C7, C9, C11
      parameter (C3  = 1d0 / 3d0,
     $           C5  = 1d0 / 5d0,
     $           C7  = 1d0 / 7d0,
     $           C9  = 1d0 / 9d0,
     $           C11 = 1d0 / 11d0)
      
      as = abs(s)
      s2 = s * s
      if (as .le. E1) then
          ysptrk = 0.5d0 * s
      elseif (as .le. E2) then
          ysptrk = 0.5d0 * s * (1d0 + C3*s2)
      elseif (as .le. E3) then
          ysptrk = 0.5d0 * s * (1d0 + s2*(C3 + C5*s2))
      else
          ysptrk = 0.5d0 * s * 
     $             (1.d0 + s2*(C3 + s2*(C5 + s2*(C7 + s2*(C9+C11*s2)))))
      endif
      end

