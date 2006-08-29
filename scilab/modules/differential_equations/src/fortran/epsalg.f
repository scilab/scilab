C/MEMBR ADD NAME=EPSALG,SSI=0
      subroutine epsalg(n, epstab, result, abserr, res3la, nres)
c
c     based on quadpack routine epsalg
c     ******************************************************
c
c           purpose
c              the routine transforms a given sequence of
c              approximations, by means of the epsilon
c              algorithm of p. wynn.
c
c              an estimate of the absolute error is also given.
c              the condensed epsilon table is computed. only those
c              elements needed for the computation of the
c              next diagonal are preserved.
c
c           calling sequence
c              call epsalg (n,epstab,result,abserr,res3la,nres)
c
c           parameters
c              n      - epstab(n) contains the new element in the
c                       first column of the epsilon table.
c
c              epstab - one dimensional array containing the
c                       elements of the two lower diagonals of
c                       the triangular epsilon table.
c                       the elements are numbered starting at the
c                       right-hand corner of the triangle.
c                       the dimension should be at least n+2.
c
c              result - resulting approximation to the integral
c
c              abserr - estimate of the absolute error computed from
c                       result and the 3 previous /results/
c
c              res3la - array containing the last 3 /results/
c
c              nres   - number of calls to the routine
c                       (should be zero at first call)
c
c     ******************************************************
c     .. scalar arguments ..
      double precision abserr, result
      integer n, nres
c     .. array arguments ..
      double precision epstab(52), res3la(3)
c     ..
c     .. local scalars ..
      double precision delta1, delta2, delta3, e0, e1, e1abs, e2, e3,
     * epmach,epsinf, err1, err2, err3, error, oflow, res, ss, tol1,
     * tol2,tol3
      integer i, ib2, ib, ie, ind, k1, k2, k3, limexp, newelm, num
c     .. function references ..
      double precision dlamch
c     ..
c
c            machine dependent constants
c             -------------------------
c            /limexp/ is the maximum number of elements the epsilon
c            table can contain. if this number is reached, the upper
c            diagonal of the epsilon table is deleted.
c
      data limexp /50/
      epmach=dlamch('p')
      oflow=dlamch('o')
c
c           list of major variables
c           -----------------------
c           e0     - the 4 elements on which the
c           e1       computation of a new element in
c           e2       the epsilon table is based
c           e3                 e0
c                        e3    e1    new
c                              e2
c           newelm - number of elements to be computed in the new
c                    diagonal
c           error  - error = abs(e1-e0)+abs(e2-e1)+abs(new-e2)
c           result - the element in the new diagonal with least
c                    error
c
      nres = nres + 1
      abserr = oflow
      result = epstab(n)
      if (n.lt.3) go to 200
      epstab(n+2) = epstab(n)
      newelm = (n-1)/2
      epstab(n) = oflow
      num = n
      k1 = n
      do 80 i=1,newelm
         k2 = k1 - 1
         k3 = k1 - 2
         res = epstab(k1+2)
         e0 = epstab(k3)
         e1 = epstab(k2)
         e2 = res
         e1abs = abs(e1)
         delta2 = e2 - e1
         err2 = abs(delta2)
         tol2 = max(abs(e2),e1abs)*epmach
         delta3 = e1 - e0
         err3 = abs(delta3)
         tol3 = max(e1abs,abs(e0))*epmach
         if (err2.gt.tol2 .or. err3.gt.tol3) go to 20
c
c           if e0, e1 and e2 are equal to within machine
c           accuracy, convergence is assumed
c           result = e2
c           abserr = abs(e1-e0)+abs(e2-e1)
c
         result = res
         abserr = err2 + err3
         go to 200
   20    e3 = epstab(k1)
         epstab(k1) = e1
         delta1 = e1 - e3
         err1 = abs(delta1)
         tol1 = max(e1abs,abs(e3))*epmach
c
c           if two elements are very close to each other, omit
c           a part of the table by adjusting the value of n
c
         if (err1.lt.tol1 .or. err2.lt.tol2 .or. err3.lt.tol3) goto 40
         ss = 0.10d+01/delta1 + 0.10d+01/delta2 - 0.10d+01/delta3
         epsinf = abs(ss*e1)
c
c           test to detect irregular behaviour in the table, and
c           eventually omit a part of the table adjusting the value
c           of n
c
         if (epsinf.gt.0.10d-03) go to 60
   40    n = i + i - 1
         go to 100
c
c           compute a new element and eventually adjust
c           the value of result
c
   60    res = e1 + 0.10d+01/ss
         epstab(k1) = res
         k1 = k1 - 2
         error = err2 + abs(res-e2) + err3
         if (error.gt.abserr) go to 80
         abserr = error
         result = res
   80 continue
c
c           shift the table
c
  100 if (n.eq.limexp) n = 2*(limexp/2) - 1
      ib = 1
      if ((num/2)*2.eq.num) ib = 2
      ie = newelm + 1
      do 120 i=1,ie
         ib2 = ib + 2
         epstab(ib) = epstab(ib2)
         ib = ib2
  120 continue
      if (num.eq.n) go to 160
      ind = num - n + 1
      do 140 i=1,n
         epstab(i) = epstab(ind)
         ind = ind + 1
  140 continue
  160 if (nres.ge.4) go to 180
      res3la(nres) = result
      abserr = oflow
      go to 200
c
c           compute error estimate
c
  180 abserr = abs(result-res3la(3)) + abs(result-res3la(2)) +
     *abs(result-res3la(1))
      res3la(1) = res3la(2)
      res3la(2) = res3la(3)
      res3la(3) = result
  200 abserr = max(abserr,5.0d+00*epmach*abs(result))
      return
      end
