c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
      subroutine realit(sss, nz, iflag)
c variable-shift h polynomial iteration for a real
c zero.
c sss   - starting iterate
c nz    - number of zero found
c iflag - flag to indicate a pair of zero near real
c         axis.
      common /gloglo/ p, qp, k, qk, svk, sr, si, u,
     * v, a, b, c, d, a1, a2, a3, a6, a7, e, f, g,
     * h, szr, szi, lzr, lzi, eta, are, mre, n, nn
      double precision p(101), qp(101), k(101),
     * qk(101), svk(101), sr, si, u, v, a, b, c, d,
     * a1, a2, a3, a6, a7, e, f, g, h, szr, szi,
     * lzr, lzi
      real eta, are, mre
      integer n, nn
      double precision pv, kv, t, s, sss
      real ms, mp, omp, ee
      integer nz, iflag, i, j, nm1
      nm1 = n - 1
      nz = 0
      s = sss
      iflag = 0
      j = 0
c main loop
   10 pv = p(1)
c evaluate p at s
      qp(1) = pv
      do 20 i=2,nn
        pv = pv*s + p(i)
        qp(i) = pv
   20 continue
      mp = abs(pv)
c compute a rigorous bound on the error in evaluating
c p
      ms = abs(s)
      ee = (mre/(are+mre))*abs(real(qp(1)))
      do 30 i=2,nn
        ee = ee*ms + abs(real(qp(i)))
   30 continue
c iteration has converges sufficiently if the
c polynomial value is less yhan 20 times this bound
      if (mp.gt.20.*((are+mre)*ee-mre*mp)) go to 40
      nz = 1
      szr = s
      szi = 0.0d+0
      return
   40 j = j +1
c stop iteration after 10 steps
      if (j.gt.10) return
      if (j.lt.2) go to 50
      if (abs(t).gt..001*abs(s-t) .or. mp.le.omp)
     * go to 50
c a cluster of zeros near the real axis has been
c encountered return with iflag set to initiate a
c quadratic iteration
      iflag = 1
      sss = s
      return
c return if the polynomial value has increased
c significantly
   50 omp = mp
c compute t, the next polynomial, and the new iterate
      kv = k(1)
      qk(1) = kv
      do 60 i=2,n
        kv = kv*s + k(i)
        qk(i) = kv
   60 continue
      if (abs(kv).le.abs(k(n))*10.*eta) go to 80
c use the scaled form of the recurrence if the value
c of k at s is nonzero
      t = -pv/kv
      k(1) = qp(1)
      do 70 i=2,n
        k(i) = t*qk(i-1) + qp(i)
   70 continue
      go to 100
c use unscaled form
   80 k(1) = 0.0d+0
      do 90 i=2,n
        k(i) = qk(i-1)
   90 continue
  100 kv = k(1)
      do 110 i=2,n
        kv = kv*s +k(i)
  110 continue
      t = 0.0d+0
      if (abs(kv).gt.abs(k(n))*10.*eta) t = -pv/kv
      s = s + t
      go to 10
      end
