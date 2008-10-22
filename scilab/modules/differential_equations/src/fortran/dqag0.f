      subroutine dqag0(f, a, b, epsabs, epsrel, result, abserr,work,
     * lwork, iwork, liwork, ifail)
c
c     based on quadpack routine
c
c     dqag0 is a general purpose integrator which calculates
c     an approximation to the integral of a function over a finite
c     interval (a,b)
c
c     dqag0 itself is essentially a dummy routine whose function is to
c     partition the work arrays work and iwork for use by dqags.
c     work is partitioned into 4 arrays each of size lwork/4.
c     iwork is a single array in dqags.
c
c     .. scalar arguments ..
      double precision a, abserr, b, epsabs, epsrel, result
      integer ifail, liwork, lwork
c     .. array arguments ..
      double precision work(lwork)
      integer iwork(liwork)
c     .. function arguments ..
      double precision f
c     ..
c     .. local scalars ..
c
      integer ibl, iel, ier, irl, limit
c     .. function references ..
c     .. subroutine references ..
c     dqags
c     ..
      external f
c     check that minimum workspace requirements are met
      if (lwork.lt.4) go to 20
      if (liwork.lt.lwork/8+2) go to 20
c     limit = upper bound on number of subintervals
      limit = lwork/4
c     set up base addresses for work arrays
      ibl = limit + 1
      iel = limit + ibl
      irl = limit + iel
c     perform integration
      call dqags(f, a, b, abs(epsabs), abs(epsrel), work(1),work(ibl)
     *, work(iel), work(irl), limit, iwork, liwork,result, abserr, ier)
      if (ier.ne.0) go to 40
      ifail = 0
      go to 60
c     error 6 = insufficient workspace
   20 ier = 6
   40 ifail = 1
   60 return
      end
