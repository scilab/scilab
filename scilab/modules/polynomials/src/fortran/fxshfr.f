      subroutine fxshfr(l2, nz)
c computes up to  l2  fixed shift k-polynomials,
c testing for convergence in the linear or quadratic
c case. initiates one of the variable shift
c iterations and returns with the number of zeros
c found.
c l2 - limit of fixed shift steps
c nz - number of zeros found
      common /gloglo/ p, qp, k, qk, svk, sr, si, u,
     * v, a, b, c, d, a1, a2, a3, a6, a7, e, f, g,
     * h, szr, szi, lzr, lzi, eta, are, mre, n, nn
       double precision p(101), qp(101), k(101),
     * qk(101), svk(101), sr, si, u, v, a, b, c, d,
     * a1, a2, a3, a6, a7, e, f, g, h, szr, szi,
     * lzr, lzi
      real eta, are, mre
      integer n, nn
      double precision svu, svv, ui, vi, s
      real betas, betav, oss, ovv, ss, vv, ts, tv,
     * ots, otv, tvv, tss
      integer l2, nz, type, i, j, iflag
      logical vpass, spass, vtry, stry
      nz = 0
      betav = .25
      betas = .25
      oss = sr
      ovv = v
c evaluate polynomial by synthetic division
      call quadsd(nn, u, v, p(1), qp(1), a, b)
      call calcsc(type)
      do 80 j=1,l2
c calculate next k polynomial and estimate v
        call nextk(type)
        call calcsc(type)
        call newest(type, ui, vi)
        vv = vi
c estimate s
        ss = 0.
        if (k(n).ne.0.0d+0) ss = -p(nn)/k(n)
        tv = 1.
        ts = 1.
        if (j.eq.1 .or. type.eq.3) go to 70
c compute relative measures of convergence of s and v
c sequences
        if (vv.ne.0.) tv = abs((vv-ovv)/vv)
        if (ss.ne.0.) ts = abs((ss-oss)/ss)
c if decreasing, multiply two most recent
c convergence measures
        tvv = 1.
        if (tv.lt.otv) tvv = tv*otv
        tss = 1.
        if (ts.lt.ots) tss = ts*ots
c compare with convergence criteria
        vpass = tvv.lt.betav
        spass = tss.lt.betas
        if (.not.(spass .or. vpass)) go to 70
c at least one sequence has passed the convergence
c test. store variables before iterating
        svu = u
        svv = v
        do 10 i=1,n
          svk(i) = k(i)
   10   continue
        s = ss
c choose iteration according to the fastest
c converging sequence
        vtry = .false.
        stry = .false.
        if (spass .and. ((.not.vpass) .or.
     *   tss.lt.tvv)) go to 40
   20   call quadit(ui, vi, nz)
        if (nz.gt.0) return
c quadratic iteration has failed. flag that it has
c been tried and decrease the convergence criterion.
        vtry = .true.
        betav = betav*.25
c try linear iteration if it has not been tried and
c the s sequence is converging
        if (stry .or. (.not.spass)) go to 50
        do 30 i=1,n
          k(i) = svk(i)
   30   continue
   40   call realit(s, nz, iflag)
        if (nz.gt.0) return
c linear iteration has failed. flag that it has been
c tried and decrease the convergence criterion
        stry = .true.
        betas = betas*.25
        if (iflag.eq.0) go to 50
c if linear iteration signals an almost double real
c zero attempt quadratic interation
        ui = -(s+s)
        vi = s*s
        go to 20
c restore variables
   50   u = svu
        v = svv
        do 60 i=1,n
          k(i) = svk(i)
   60   continue
c try quadratic iteration if it has not been tried
c and the v sequence is converging
        if (vpass .and. (.not.vtry)) go to 20
c recompute qp and scalar values to continue the
c second stage
        call quadsd(nn, u, v, p(1), qp(1), a, b)
        call calcsc(type)
   70   ovv = vv
        oss = ss
        otv = tv
        ots = ts
   80 continue
      return
      end
