      subroutine quadit(uu, vv, nz)
c variable-shift k-polynomial iteration for a
c quadratic factor converges only if the zeros are
c equimodular or nearly so.
c uu,vv - coefficients of starting quadratic
c nz - number of zero found
      common /gloglo/ p, qp, k, qk, svk, sr, si, u,
     * v, a, b, c, d, a1, a2, a3, a6, a7, e, f, g,
     * h, szr, szi, lzr, lzi, eta, are, mre, n, nn
       double precision p(101), qp(101), k(101),
     * qk(101), svk(101), sr, si, u, v, a, b, c, d,
     * a1, a2, a3, a6, a7, e, f, g, h, szr, szi,
     * lzr, lzi
      real eta, are, mre
      integer n, nn
      double precision ui, vi, uu, vv
      real mp, omp, ee, relstp, t, zm
      integer nz, type, i, j
      logical tried
      nz = 0
      tried = .false.
      u = uu
      v = vv
      j = 0
c main loop
   10 call quad(1.0d+0, u, v, szr, szi, lzr, lzi)
c return if roots of the quadratic are real and not
c close to multiple or nearly equal and  of opposite
c sign
      if (abs(abs(szr)-abs(lzr)).gt.0.010d+0*
     * abs(lzr)) return
c evaluate polynomial by quadratic synthetic division
      call quadsd(nn, u, v, p(1), qp(1), a, b)
      mp = abs(a-szr*b) + abs(szi*b)
c compute a rigorous  bound on the rounding error in
c evaluting p
      zm = sqrt(abs(real(v)))
      ee = 2.*abs(real(qp(1)))
      t = -szr*b
      do 20 i=2,n
        ee = ee*zm + abs(real(qp(i)))
   20 continue
      ee = ee*zm + abs(real(a)+t)
      ee = (5.*mre+4.*are)*ee - (5.*mre+2.*are)*
     * (abs(real(a)+t)+abs(real(b))*zm) +
     * 2.*are*abs(t)
c iteration has converged sufficienty if the
c polynomial value is less than 20 times this bound
      if (mp.gt.20.*ee) go to 30
      nz = 2
      return
   30 j = j + 1
c stop iteration after 20 steps
      if (j.gt.20) return
      if (j.lt.2) go to 50
      if (relstp.gt..01 .or. mp.lt.omp .or. tried)
     * go to 50
c a cluster appears to be stalling the convergence.
c five fixed shift steps are taken with a u,v close
c to the cluster
      if (relstp.lt.eta) relstp = eta
      relstp = sqrt(relstp)
      u = u - u*relstp
      v = v + v*relstp
      call quadsd(nn, u, v, p(1), qp(1), a, b)
      do 40 i=1,5
        call calcsc(type)
        call nextk(type)
   40 continue
      tried = .true.
      j = 0
   50 omp = mp
c calculate next k polynomial and new u and v
      call calcsc(type)
      call nextk(type)
      call calcsc(type)
      call newest(type, ui, vi)
c if vi is zero the iteration is not converging
      if (vi.eq.0.0d+0) return
      relstp = abs((vi-v)/vi)
      u = ui
      v = vi
      go to 10
      end
