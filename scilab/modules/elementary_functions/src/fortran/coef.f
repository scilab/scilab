c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) Bruno Pincon
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.

      subroutine coef(ierr)
c!purpose
c     coef compute the lengh,and the coefficients of the
c     exponential pade approximant
c
c!calling sequence
c     subroutine coef(ierr)
c     common /dcoeff/ b,n
c
c     double precision b(41)
c     integer n,ierr
c     ierr error indicator : if ierr.ne.0 n is too large
c          machine precision can't be achieved
c
c     b    array containing pade coefficients
c
c     n    lengh of pade approximation
c
c!auxiliary routines
c     exp dble real mod (fortran)
c!originator
c      j.roche  - laboratoire d'automatique de grenoble
c!
      double precision b(41)
      integer n,ierr
c internal variables
      integer m, i, ir, id, ie, j, j1, n1, im1, ip1, k
      double precision a, b1, b2, b3, zero, one, two, cnst, half
      dimension a(41), m(21)
cDEC$ IF DEFINED (FORDLL)
cDEC$ ATTRIBUTES DLLIMPORT:: /dcoeff/
cDEC$ ENDIF
      common /dcoeff/ b, n
      data zero, one, two, cnst, half /0.0d+0,1.0d+0,2.0d+0,
     * 0.556930d+0,0.50d+0/
c
      ierr=0
c
c   determination of the pade approximants type
c
      n = 1
      b1 = exp(one)
      b3 = 6.
      b2 = b1/(b3*(cnst-one))
      b2 = abs(b2)
   10 if (b2+one.le.one) go to 20
      n = n + 1
      b3 = b3*(4.0d+0*dble(real(n))+two)
      b2 = b1/(b3*((dble(real(n))*cnst-one)**n))
      go to 10
   20 continue
      if(n.gt.40) ierr=n
      n=min(n,40)
c
c   compute the coefficients of pade approximants
c
      n1 = n + 1
      n2 = (n+2)/2
      a(1) = one
      a(2) = half
      do 30 i=2,n
         im1 = i - 1
         ip1 = i + 1
         a(ip1) = a(i)*dble(real(n-im1))/dble(real(i*(2*n-im1)
     *    ))
   30 continue
c
c   compute the coefficients of pade approximants in chebychef system
c
      do 40 i=1,n2
         m(i) = 0
   40 continue
      do 50 i=1,n1
         b(i) = zero
   50 continue
      m(1) = 1
      b(1) = a(1)
      b(2) = a(2)
      i = 0
      b3 = one
   60 i = i + 1
      b3 = b3*half
      ir = mod(i,2)
      id = (i+3)/2
      ie = id
      if (ir .eq. 0) then
         goto 70
      else
         goto 80
      endif
   70 m(id) = m(id) + m(id)
   80 m(id) = m(id) + m(id-1)
      id = id - 1
      if ((id-1) .eq. 0) goto 90
      goto 80
   90 j = i + 2
      j1 = j
      do 100 k=1,ie
         b2 = m(k)
         b1 = a(j1)*b2*b3
         b(j) = b(j) + b1
         j = j - 2
  100 continue
      if (n1-i.ne.2) go to 60
      return
      end
