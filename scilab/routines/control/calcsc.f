      subroutine calcsc(type)
c this routine calculates scalar quantities used to
c compute the next k polynomial and new estimates of
c the quadratic coefficients.
c type - integer variable set here indicating how the
c calculations are normalized to avoid overflow
      common /gloglo/ p, qp, k, qk, svk, sr, si, u,
     * v, a, b, c, d, a1, a2, a3, a6, a7, e, f, g,
     * h, szr, szi, lzr, lzi, eta, are, mre, n, nn
      double precision p(101), qp(101), k(101),
     * qk(101), svk(101), sr, si, u, v, a, b, c, d,
     * a1, a2, a3, a6, a7, e, f, g, h, szr, szi,
     * lzr, lzi
      real eta, are, mre
      integer n, nn
      integer type
c synthetic division of k by the quadratic 1,u,v
      call quadsd(n, u, v, k(1), qk(1), c, d)
      if (abs(c).gt.abs(k(n))*100.*eta) go to 10
      if (abs(d).gt.abs(k(n-1))*100.*eta) go to 10
      type = 3
c type=3 indicates the quadratic is almost a factor
c of k
      return
   10 if (abs(d).lt.abs(c)) go to 20
      type = 2
c type=2 indicates that all formulas are divided by d
      e = a/d
      f = c/d
      g = u*b
      h = v*b
      a3 = (a+g)*e + h*(b/d)
      a1 = b*f - a
      a7 = (f+u)*a +h
      return
   20 type = 1
c type=1 indicates that all formulas are divided by c
      e = a/c
      f = d/c
      g = u*e
      h = v*b
      a3 = a*e + (h/c+g)*b
      a1 = b - a*(d/c)
      a7 = a + g*d + h*f
      return
      end
