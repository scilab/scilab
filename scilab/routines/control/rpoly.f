      subroutine rpoly(op, degree, zeror, zeroi, fail)
c!purpose
c finds the zeros of a real polynomial
c!calling sequence
c op  - double precision vector of coefficients in
c       order of decreasing powers.
c degree   - integer degree of polynomial.
c zeror, zeroi - output double precision vectors of
c                real and imaginary parts of the
c                zeros.
c fail  - output parameter, 
c        2 if  leading coefficient is zero 
c        1 for non convergence or if rpoly
c         has found fewer than degree zeros.
c         in the latter case degree is reset to
c         the number of zeros found.
c        3 if degree>100
c!comments
c to change the size of polynomials which can be
c solved, reset the dimensions of the arrays in the
c common area and in the following declarations.
c the subroutine uses single precision calculations
c for scaling, bounds and error calculations. all
c calculations for the iterations are done in double
c precision.
c!
      external dlamch,slamch
      double precision dlamch
      real slamch
c
      common /gloglo/ p, qp, k, qk, svk, sr, si, u,
     * v, a, b, c, d, a1, a2, a3, a6, a7, e, f, g,
     * h, szr, szi, lzr, lzi, eta, are, mre, n, nn
      double precision p(101), qp(101), k(101),
     * qk(101), svk(101), sr, si, u, v, a, b, c, d,
     * a1, a2, a3, a6, a7, e, f, g, h, szr, szi,
     * lzr, lzi
      double precision op(*), temp(101),
     * zeror(*), zeroi(*), t, aa, bb, cc,factor
      real ptt(101), lo, maxi, mini, xx, yy, cosr,
     * sinr, xxx, x, sc, bnd, xm, ff, df, dx, infin,
     * smalno, base,eta,are,mre
      integer degree, cnt, nz, i, j, jj, nm1, n, nn
      logical  zerok
      integer fail
c
      if(degree.gt.100) goto 300
c the following statements set machine constants used
c in various parts of the program. the meaning of the
c four constants are...
c eta     the maximum relative representation error
c         which can be described as the smallest
c         positive floating point number such that
c         1.do+eta is greater than 1.
c infiny  the largest floating-point number.
c smalno  the smallest positive floating-point number
c         if the exponent range differs in single and
c         double precision then smalno and infin
c         should indicate the smaller range.
c base    the base of the floating-point number
c         system used.
      smalno=slamch('u')
      infin=slamch('o')
      base=slamch('b')
      eta=real(dlamch('p'))
c are and mre refer to the unit error in + and *
c respectively. they are assumed to be the same as
c eta.
      are = eta
      mre = eta
      lo = smalno/eta
c initialization of constants for shift rotation
      xx = 0.707106780d+0
      yy = -xx
      cosr = -0.0697564740d+0
      sinr =  0.997564050d+0
      fail = 0.
      n = degree
      nn = n + 1
c algorithm fails if the leading coefficient is zero.
      if (op(1).ne.0.0d+0) go to 10
      fail = 2
      degree = 0
      return
c make a copy of the coefficients
   10 do 20 i=1,nn
        p(i) = op(i)
   20 continue
c remove the zeros at the origin if any
   30 if (p(nn).ne.0.0d+0) go to 40
      j = degree - n + 1
      zeror(j) = 0.0d+0
      zeroi(j) = 0.0d+0
      nn = nn - 1
      n = n - 1
      go to 30
c start the algorithm for one zero
   40 if (n.gt.2) go to 60
      if (n.lt.1) return
c calculate the final zero or pair zeros
      if (n.eq.2) go to 50
      zeror(degree) = - p(2)/p(1)
      zeroi(degree) = 0.0d+0
      return
   50 call quad(p(1), p(2), p(3), zeror(degree-1),
     * zeroi(degree-1), zeror(degree), zeroi(degree))
      return
c find largest and smallest moduli of coefficients.
   60 maxi = 0.
      mini = infin
      do 70 i=1,nn
        x = abs(real(p(i)))
        if (x.gt.maxi) maxi = x
        if (x.ne.0. .and. x.lt.mini) mini = x
   70 continue
C      maxi=min(infin,maxi) bug "f77 -mieee-with-inexact"
      if (infin.lt.maxi) maxi=infin
c scale if there are large or very small coefficients
c computes a scale factor to multiply the
c coefficients of the polynomial. the scaling is done
c to avoid overflow and to avoid undetected underflow
c interfering with the convergence criterion.
c the factor is a power of the base
      sc = lo/mini
      if (sc.gt.1.0) go to 80
      if (maxi.lt.10.) go to 110
      if (sc.eq.0.) sc = smalno
      go to 90
   80 if (infin/sc.lt.maxi) go to 110
   90 l = log(sc)/log(base) + .5
      factor = (base*1.0d+0)**l
      if (factor.eq.1.0d+0) go to 110
      do 100 i=1,nn
        p(i) = factor*p(i)
  100 continue
c compute lower bound on moduli of zeros.
  110 do 120 i=1,nn
c        ptt(i) = min(infin,abs(real(p(i)))) bug "f77 -mieee-with-inexact"
         ptt(i) = abs(real(p(i)))
         if (infin.lt.abs(real(p(i)))) ptt(i)=infin
  120 continue
      ptt(nn) = -ptt(nn)
c compute upper estimate of bound
      x = exp((log(-ptt(nn))-log(ptt(1)))/real(n))
      if (ptt(n).eq.0.) go to 130
c if newton step at the origin is better, use it.
      xm = -ptt(nn)/ptt(n)
      if (xm.lt.x) x = xm
c chop the interval (0,x) until ff .le. 0
  130 xm = x*.1
      ff = ptt(1)
      do 140 i=2,nn
        ff = ff*xm + ptt(i)
  140 continue
      if (ff.le.0) go to 150
      if(ff.gt.infin) goto 310
      x = xm
      go to 130
  150 dx = x
c do newton iteration until x converges to two
c decimal places
  160 if (abs(dx/x).le..005) go to 180
      ff = ptt(1)
      df = ff
      do 170 i=2,n
        ff = ff*x + ptt(i)
        df = df*x + ff
  170 continue
      ff = ff*x + ptt(nn)
      if(ff.gt.infin) goto 310
      dx = ff/df
      x = x - dx
      go to 160
  180 bnd = x
c compute the derivative as the intial k polynomial
c and do 5 steps with no shift
      nm1 = n - 1
      do 190 i=2,n
        k(i) = real(nn-i)*p(i)/real(n)
  190 continue
      k(1) = p(1)
      aa = p(nn)
      bb = p(n)
      zerok = k(n).eq.0.0d+0
      do 230 jj=1,5
        cc = k(n)
        if (zerok) go to 210
c use scaled form of recurrence if value of k at 0 is
c nonzero
        t = -aa/cc
        do 200 i=1,nm1
          j = nn - i
          k(j) = t*k(j-1) + p(j)
  200   continue
        k(1) = p(1)
        zerok = abs(k(n)).le.abs(bb)*eta*10.
        go to 230
c use unscaled form form of recurrence
  210   do 220 i=1,nm1
          j = nn - i
          k(j) = k(j-1)
  220   continue
        k(1) = 0.0d+0
        zerok = k(n).eq.0.0d+0
  230 continue
c save k for restarts with new shifts
      do 240 i=1,n
        temp(i) = k(i)
  240 continue
c loop to select the quadratic  corresponding to each
c new shift
      do 280 cnt=1,20
c quadratic corresponds to a double shift to a
c non-real point and its complex conjugate. the point
c has modulus bnd and amplitude rotated by 94 degrees
c from the previous shift
        xxx = cosr*xx - sinr*yy
        yy = sinr*xx + cosr*yy
        xx = xxx
        sr = bnd*xx
        si = bnd*yy
        u = -2.0d+0*sr
        v = bnd
c second stage calculation, fixed quadratic
        call fxshfr(20*cnt, nz)
        if (nz.eq.0) go to 260
c the second stage jumps directly to one of the third
c stage iterations and returns here if successful.
c deflate the polynomial, store the zero or zeros and
c return to the main algorithm.
        j = degree - n + 1
        zeror(j) = szr
        zeroi(j) = szi
        nn = nn - nz
        n = nn - 1
        do 250 i=1,nn
          p(i) = qp(i)
  250   continue
        if (nz.eq.1) go to 40
        zeror(j+1) = lzr
        zeroi(j+1) = lzi
        go to 40
c if the iteration is unsuccessful another quadratic
c is chosen after restoring k
  260   do 270 i=1,n
           k(i) = temp(i)
  270   continue
  280 continue
c return with failure if no convergence with 20
c shifts
      fail = 1
      degree = degree - n
      return
  300 fail=3
      return
 310  fail=1
      return
      end
