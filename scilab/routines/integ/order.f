C/MEMBR ADD NAME=ORDER,SSI=0
      subroutine order(limit, last, maxerr, ermax, elist, iord,liord,
     * nrmax)
c
c     based on quadpack routine order
c     ******************************************************
c
c           purpose
c              this routine maintains the descending ordering
c              in the list of the local error estimates
c              resulting from the interval subdivision
c              process. at each call two error estimates
c              are inserted using the sequential search
c              method . top-down for the largest error
c              estimate,  bottom-up for the smallest error
c              estimate.
c
c           calling sequence
c              call order
c              (limit,last,maxerr,ermax,elist,iord,liord,nrmax)
c
c             parameters (meaning at output)
c              limit  - maximum number of error estimates the list
c                       can contain
c
c              last   - number of error estimates currently
c                       in the list. elist(last) contains
c                       the smallest error estimate.
c
c              maxerr - maxerr points to the nrmax-th largest error
c                       estimate currently in the list.
c
c              ermax  - nrmax-th largest error estimate
c                       ermax = elist(maxerr)
c
c              elist  - array of dimension last containing
c                       the error estimates
c
c              iord   - array containing pointers to elist so
c                       that iord(1) points to the largest
c                       error estimate,...,iord(last) to the
c                       smallest error estimate
c
c              liord  - dimension of iord
c
c              nrmax  - maxerr = iord(nrmax)
c
c     ******************************************************
c
c     .. scalar arguments ..
      double precision ermax
      integer last, limit, liord, maxerr, nrmax
c     .. array arguments ..
      double precision elist(last)
      integer iord(liord)
c     ..
c     .. scalars in common ..
      integer jupbnd
c     ..
c     .. local scalars ..
      double precision errmax, errmin
      integer i, ibeg, ido, isucc, j, jbnd, k
c     ..
      common /dqa001/ jupbnd
c
c            check whether the list contains more than
c            two error estimates
c
      if (last.gt.2) go to 20
      iord(1) = 1
      iord(2) = 2
      go to 180
c
c           this part of the routine is only executed
c           if, due to a difficult integrand, subdivision
c           increased the error estimate. in the normal case
c           the insert procedure should start after the
c           nrmax-th largest error estimate.
c
   20 errmax = elist(maxerr)
      if (nrmax.eq.1) go to 60
      ido = nrmax - 1
      do 40 i=1,ido
         isucc = iord(nrmax-1)
         if (errmax.le.elist(isucc)) go to 60
         iord(nrmax) = isucc
         nrmax = nrmax - 1
   40 continue
c
c           compute the number of elements in the list to
c           be maintained in descending order. this number
c           depends on the number of subdivisions still
c           allowed
c
   60 jupbnd = last
      if (last.gt.(limit/2+2)) jupbnd = limit + 3 - last
      errmin = elist(last)
c
c           insert errmax by traversing the list top-down
c           starting comparison from the element
c           elist(iord(nrmax+1))
c
      jbnd = jupbnd - 1
      ibeg = nrmax + 1
      if (ibeg.gt.jbnd) go to 100
      do 80 i=ibeg,jbnd
         isucc = iord(i)
         if (errmax.ge.elist(isucc)) go to 120
         iord(i-1) = isucc
   80 continue
  100 iord(jbnd) = maxerr
      iord(jupbnd) = last
      go to 180
c
c           insert errmin by traversing the list bottom-up
c
  120 iord(i-1) = maxerr
      k = jbnd
      do 140 j=i,jbnd
         isucc = iord(k)
         if (errmin.lt.elist(isucc)) go to 160
         iord(k+1) = isucc
         k = k - 1
  140 continue
      iord(i) = last
      go to 180
  160 iord(k+1) = last
c
c           set maxerr and ermax
c
  180 maxerr = iord(nrmax)
      ermax = elist(maxerr)
      return
      end
