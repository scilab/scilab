      subroutine nextk(type)
c computes the next k polynomials using scalars
c computed in calcsc
      common /gloglo/ p, qp, k, qk, svk, sr, si, u,
     * v, a, b, c, d, a1, a2, a3, a6, a7, e, f, g,
     * h, szr, szi, lzr, lzi, eta, are, mre, n, nn
      double precision p(101), qp(101), k(101),
     * qk(101), svk(101), sr, si, u, v, a, b, c, d,
     * a1, a2, a3, a6, a7, e, f, g, h, szr, szi,
     * lzr, lzi
      real eta, are, mre
      integer n, nn
      double precision temp
      integer type
      if (type.eq.3) go to 40
      temp = a
      if (type.eq.1) temp = b
      if (abs(a1).gt.abs(temp)*eta*10.) go to 20
c if a1 is nearly zero then use a special form of the
c recurrence
      k(1) = 0.0d+0
      k(2) = -a7*qp(1)
      do 10 i=3,n
        k(i) = a3*qk(i-2) - a7*qp(i-1)
   10 continue
      return
c use scaled form of the recurrence
   20 a7 = a7/a1
      a3 = a3/a1
      k(1) = qp(1)
      k(2) = qp(2) -a7*qp(1)
      do 30 i=3,n
        k(i) = a3*qk(i-2) - a7*qp(i-1) + qp(i)
   30 continue
      return
c use unscaled form of the recurrence if type is 3
   40 k(1) = 0.0d+0
      k(2) = 0.0d+0
      do 50 i=3,n
        k(i) = qk(i-2)
   50 continue
      return
      end
