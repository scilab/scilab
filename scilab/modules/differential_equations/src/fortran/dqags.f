      subroutine dqags(f, a, b, epsabs, epsrel, alist, blist,elist,
     * rlist, limit, iord, liord, result, abserr, ier)
c
c     based on quadpack routine dqags (formerly qags)
c     **********************************************************
c
c        purpose
c           the routine calculates an approximation
c           /result/ to a given definite integral   i =
c           integral of /f/ over (a,b), hopefully
c           satisfying following claim for accuracy .
c           abs(i-result) .le. max(epsabs,epsrel*abs(i)).
c
c          calling sequence
c           call dqags (f,a,b,epsabs,epsrel,alist,blist,elist,
c                        rlist,limit,iord,liord,result,abserr,ier)
c
c        parameters
c            f      - function subprogram defining the integrand
c                     function f(x). the actual name for f
c                     needs to be declared e x t e r n a l
c                     in the driver program
c
c            a      - lower limit of integration
c
c            b      - upper limit of integration
c
c            epsabs - absolute accuracy requested
c
c            epsrel - relative accuracy requested
c
c            alist,blist,elist,rlist
c                   - work arrays (functions described below)
c
c            limit  - upper bound for number of subintervals
c
c            iord   - work array
c
c            liord  - length of iord (at least limit/2 + 2)
c
c            result - approximation to the integral
c
c            abserr - estimate of the modulus of the absolute error,
c                     which should equal or exceed abs(i-result)
c
c            ier    - ier   = 0 normal and reliable
c                             termination of the routine.
c                             it is assumed that the
c                             requested  accuracy has been
c                             achieved.
c                   - ier   .ne. 0 abnormal termination of
c                             the routine. the estimates
c                             for integral and error are
c                             less reliable. it is assumed
c                             that the  requested accuracy
c                             has not been achieved.
c                         = 1 maximum number of subdivisions allowed
c                             has been achieved. the user can
c                             allow more sub divisions by
c                             increasing the dimensions of the
c                             work arrays work and iwork.
c                             however, this may
c                             yield no  improvement, and it
c                             is rather advised to have a
c                             close look at the integrand,
c                             in order to determine the
c                             integration  difficulties. if
c                             the position of a local
c                             difficulty can be determined
c                             (i.e.  singularity,
c                             discontinuity within the
c                             interval) one will probably
c                             gain from  splitting up the
c                             interval at this point and
c                             calling the integrator on the
c                             sub-ranges. if possible, an
c                             appropriate special-purpose
c                             integrator should be used
c                             which is designed for
c                             handling the type  of
c                             difficulty involved.
c                         = 2 the occurrence of roundoff
c                             error is detected which
c                             prevents the requested
c                             tolerance  from being
c                             achieved. the error may be
c                             under-estimated.
c                         = 3 extremely bad integrand behaviour
c                             occurs at some interior points of the
c                             integration interval.
c                         = 4 it is presumed that the requested
c                             tolerance cannot be achieved,
c                             and that the returned result
c                             is the best which can be
c                             obtained.
c                         = 5 the integral is probably divergent, or
c                             slowly convergent. it must be noted
c                             that divergency can occur
c                             with any other value of ier.
c                         = -1 an error occurs during the evaluation of f
c     **********************************************************
c     .. scalar arguments ..
      double precision a, abserr, b, epsabs, epsrel, result
      integer ier, limit, liord
c     .. array arguments ..
      double precision alist(limit), blist(limit), elist(limit),
     * rlist(limit)
      integer iord(liord)
c     .. function arguments ..
      double precision f
c     ..
c     .. scalars in common ..
      integer jupbnd
      integer       iero
      common/ierajf/iero
c     ..
c     .. local scalars ..
      double precision a1, a2, abseps, area12, area1, area2, area, b1,
     * b2,correc, defab1, defab2, defabs, dres, epmach, erlarg,erlast,
     * errbnd, errmax, erro12, error1, error2, errsum,ertest, oflow,
     * resabs, reseps, small, uflow
      integer id, ierro, iroff1, iroff2, iroff3, k, ksgn, ktmin,last1,
     * last, maxerr, nres, nrmax, numrl2
      logical extrap, noext
c     .. local arrays ..
      double precision res3la(3), rlist2(52)
c     .. function references ..
      double precision dlamch
c     .. subroutine references ..
c     order, epsalg, quarul
c     ..
      external f
      common /dqa001/ jupbnd
c
c            the dimension of /rlist2/ is determined by
c            data /limexp/ in subroutine epsalg (/rlist2/
c            should be of dimension (limexp+2) at least).
c
      epmach=dlamch('p')
      uflow=dlamch('u')
      oflow=dlamch('o')
      iero=0
c
c            list of major variables
c            -----------------------
c
c           alist     - list of left end-points of all subintervals
c                       considered up to now
c
c           blist     - list of right end-points of all subintervals
c                       considered up to now
c
c           rlist(i)  - approximation to the integral over
c                       (alist(i),blist(i))
c
c           rlist2    - array of dimension at least limexp+2
c                       containing the part of the epsilon table
c                       which is still needed for further
c                       computations
c
c           elist(i)  - error estimate applying to rlist(i)
c
c           maxerr    - pointer to the interval with largest error
c                       estimate
c
c           errmax    - elist(maxerr)
c
c           erlast    - error on the interval currently subdivided
c                       (before that subdivision has taken place)
c
c           area      - sum of the integrals over the subintervals
c
c           errsum    - sum of the errors over the subintervals
c
c           errbnd    - requested accuracy max(epsabs,epsrel*
c                       abs(result))
c
c           *****1    - variable for the left interval
c
c           *****2    - variable for the right interval
c
c           last      - index for subdivision
c
c           nres      - number of calls to the extrapolation routine
c
c           numrl2    - number of elements currently  in
c                       rlist2. if an appropriate
c                       approximation to the compounded
c                       integral has been obtained it is
c                       put in  rlist2(numrl2) after numrl2
c                       has been increased by one.
c
c           small     - length of the smallest interval considered
c                       up to now, multiplied by 1.5
c
c           erlarg    - sum of the errors over the intervals larger
c                       than the smallest interval
c                       considered up to now
c           extrap    - logical variable denoting that the
c                       routine is attempting to perform
c                       extrapolation.  i.e. before
c                       subdividing the smallest interval
c                       we try to decrease the value of
c                       erlarg
c           noext     - logical variable denoting that extrapolation
c                       is no longer allowed(/true/ value)
c
c           first approximation to the integral
c           -----------------------------------
c
      last1 = 1
      ier = 0
      ierro = 0
      call quarul(f, a, b, result, abserr, defabs, resabs)
      if(iero.gt.0) then
         ier=6
         return
      endif
c
c           test on accuracy
c
      dres = abs(result)
      errbnd = max(epsabs,epsrel*dres)
      if (abserr.le.1.0d+02*epmach*defabs .and. abserr.gt.errbnd)ier = 2
      if (limit.lt.2 .and. abserr.gt.errbnd) ier = 1
      if (ier.ne.0 .or. abserr.le.errbnd) go to 320
c
c           initialization
c           --------------
c
      alist(1) = a
      blist(1) = b
      rlist(1) = result
      rlist2(1) = result
      errmax = abserr
      maxerr = 1
      area = result
      errsum = abserr
      abserr = oflow
      nrmax = 1
      nres = 0
      numrl2 = 2
      ktmin = 0
      extrap = .false.
      noext = .false.
      iroff1 = 0
      iroff2 = 0
      iroff3 = 0
      ksgn = -1
      if (dres.ge.(0.10d+01-0.50d+02*epmach)*defabs) ksgn = 1
c
c           main do-loop
c           ------------
c
      if (limit.lt.2) go to 220
      do 200 last=2,limit
c
c           bisect the subinterval with the nrmax-th largest
c           error estimate
c
         last1 = last
         a1 = alist(maxerr)
         b1 = 0.50d+00*(alist(maxerr)+blist(maxerr))
         a2 = b1
         b2 = blist(maxerr)
         erlast = errmax
         call quarul(f, a1, b1, area1, error1, resabs, defab1)
         if(iero.gt.0) then
            ier=6
            return
         endif
         call quarul(f, a2, b2, area2, error2, resabs, defab2)
         if(iero.gt.0) then
            ier=6
            return
         endif
c
c           improve previous approximation of integral
c           and error and test for accuracy
c
         area12 = area1 + area2
         erro12 = error1 + error2
         errsum = errsum + erro12 - errmax
         area = area + area12 - rlist(maxerr)
         if (defab1.eq.error1 .or. defab2.eq.error2) go to 40
         if (abs(rlist(maxerr)-area12).gt.0.10d-04*abs(area12) .or.
     *   erro12.lt.0.990d+00*errmax) go to 20
         if (extrap) iroff2 = iroff2 + 1
         if (.not.extrap) iroff1 = iroff1 + 1
   20    if (last.gt.10 .and. erro12.gt.errmax) iroff3 = iroff3 + 1
   40    rlist(maxerr) = area1
         rlist(last) = area2
         errbnd = max(epsabs,epsrel*abs(area))
         if (errsum.le.errbnd) go to 280
c
c           test for roundoff error and eventually
c           set error flag
c
         if (iroff1+iroff2.ge.10 .or. iroff3.ge.20) ier = 2
         if (iroff2.ge.5) ierro = 3
c
c           set error flag in the case that the number of interval
c            bisections exceeds /limit/
c
         if (last.eq.limit) ier = 1
c
c           set error flag in the case of bad integrand behaviour
c           at interior points of integration range
c
         if (max(abs(a1),abs(b2)).le.(0.10d+01+0.10d+03*epmach)*
     *   (abs(a2)+0.10d+04*uflow)) ier = 4
         if (ier.ne.0) go to 220
c
c           append the newly-created intervals to the list
c
         if (error2.gt.error1) go to 60
         alist(last) = a2
         blist(maxerr) = b1
         blist(last) = b2
         elist(maxerr) = error1
         elist(last) = error2
         go to 80
   60    alist(maxerr) = a2
         alist(last) = a1
         blist(last) = b1
         rlist(maxerr) = area2
         rlist(last) = area1
         elist(maxerr) = error2
         elist(last) = error1
c
c           call subroutine order to maintain the
c           descending ordering in the list of error
c           estimates and select the subinterval with
c           nrmax-th largest error estimate (to be bisected
c           next)
c
   80    call order(limit, last, maxerr, errmax, elist, iord,liord,
     *    nrmax)
         if (last.eq.2) go to 180
         if (noext) go to 200
         erlarg = erlarg - erlast
         if (abs(b1-a1).gt.small) erlarg = erlarg + erro12
         if (extrap) go to 100
c
c           test whether the interval to be bisected next is the
c           smallest interval
c
         if (abs(blist(maxerr)-alist(maxerr)).gt.small) go to 200
         extrap = .true.
         nrmax = 2
  100    if (ierro.eq.3 .or. erlarg.le.ertest) go to 140
c
c           the smallest interval has the largest error.
c           before bisecting decrease the sum of the errors
c           over the larger intervals(erlarg) and perform
c           extrapolation
c
         id = nrmax
         do 120 k=id,jupbnd
            maxerr = iord(nrmax)
            errmax = elist(maxerr)
            if (abs(blist(maxerr)-alist(maxerr)).gt.small) go to 200
            nrmax = nrmax + 1
  120    continue
c
c           perform extrapolation
c
  140    numrl2 = numrl2 + 1
         rlist2(numrl2) = area
         call epsalg(numrl2, rlist2, reseps, abseps, res3la, nres)
         ktmin = ktmin + 1
         if (ktmin.gt.5 .and. abserr.lt.0.10d-02*errsum) ier = 5
         if (abseps.ge.abserr) go to 160
         ktmin = 0
         abserr = abseps
         result = reseps
         correc = erlarg
         ertest = max(epsabs,epsrel*abs(reseps))
         if (abserr.le.ertest) go to 220
c
c           prepare  bisection of the smallest interval
c
  160    if (numrl2.eq.1) noext = .true.
         if (ier.eq.5) go to 220
         maxerr = iord(1)
         errmax = elist(maxerr)
         nrmax = 1
         extrap = .false.
         small = small*0.50d+00
         erlarg = errsum
         go to 200
  180    small = abs(b-a)*0.3750d+00
         erlarg = errsum
         ertest = errbnd
         rlist2(2) = area
  200 continue
c
c           set  final result and error estimate
c           ------------------------------------
c
  220 if (abserr.eq.oflow) go to 280
      if (ier+ierro.eq.0) go to 260
      if (ierro.eq.3) abserr = abserr + correc
      if (ier.eq.0) ier = 3
      if (result.ne.0.0d+00.and .area. ne.0.0d+00) go to 240
      if (abserr.gt.errsum) go to 280
      if (area.eq.0.0d+00) go to 320
      go to 260
  240 if (abserr/abs(result).gt.errsum/abs(area)) go to 280
c
c           test on divergency
c
  260 if (ksgn.eq.-1 .and. max(abs(result),abs(area)).le.defabs*
     *0.10d-01) go to 320
      if (0.10d-01.gt.(result/area) .or. (result/area).gt.0.10d+03.or.
     * errsum.gt.abs(area)) ier = 6
      go to 320
c
c           compute global integral sum
c
  280 result = 0.0d+00
      do 300 k=1,last
         result = result + rlist(k)
  300 continue
      abserr = errsum
  320 if (ier.gt.2) ier = ier - 1
      iord(1) = 4*last1
      return
      end
