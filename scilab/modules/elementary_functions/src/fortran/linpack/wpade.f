c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
C/MEMBR ADD NAME=WPADE,SSI=0
c
      subroutine wpade(ar,ai,ia,n,ear,eai,iea,alpha,w,ipvt,ierr)
c
c!purpose
c      compute the pade approximants of the exponential of a complex
c      matrix a. we scale a until the spectral radius of a*2**-m
c      are smaler than one.
c
c!calling sequence
c
c     subroutine wpade(ar,ai,ia,n,ear,eai,iea,alpha,w,ipvt,ierr)
c
c     integer ia,n,iea,ipvt,ierr
c     double precision ar,ai,ear,eai,alpha,w
c     dimension ar(ia,n),ai(ia,n),ear(iea,n),eai(iea,n),w(*),ipvt(*)
c
c      ar,ai     : array containing the matrix a
c      ia        : the leading dimension of arrays a.
c      n         : the order of the matrices a,ea .
c      ear,eai   : the  array that contains the n*n
c                  matrix exp(a).
c      iea       : the leading dimension of array ea.
c      alpha     : variable containing the maximum
c                  norm of the eigenvalues of a.
c      w        : workspace array of size 4*n +4*n*n
c      ipvt      : integer workspace of size n
c      ierr      : error indicator
c                  ierr= 0 if normal return
c                      =-4 if alpha is to big for any accuracy.
c
c     common /dcoeff/ c, ndng
c     double precision c(41)
c     integer ndng
c
c     c          : array containing on return pade coefficients
c     ndng       : on first call ndng must be set to -1,on return
c                contains degree of pade approximant
c
c!auxiliary routines
c       wclmat  coef wcerr (j. roche)
c       wmmul dmcopy (blas.extension)
c       wgeco wgesl (linpack.extension)
c       sqrt (fortran)
c!
c
      integer ia,n,iea,ipvt,ierr
      double precision ar,ai,ear,eai,alpha,w
      dimension ar(ia,n),ai(ia,n),ear(iea,n),eai(iea,n),w(*),ipvt(*)
c
cDEC$ IF DEFINED (FORDLL)
cDEC$ ATTRIBUTES DLLIMPORT:: /dcoeff/
cDEC$ ENDIF
      common /dcoeff/ c, ndng
c internal variables
      integer i,j,k,m,ndng,maxc,n2
      double precision rcond,c,efact,two,zero,norm,one
      dimension c(41)
c
      data zero, one, two, maxc /0.0d+0,1.0d+0,2.0d+0,10/
      n2=n*n
      kr=1
      ki=kr+n2
      kw=ki+n2
c
      if (ndng.ge.0) go to 10
c
c compute de pade's approximants type which is necessary to obtain
c machine precision
c
      call coef(ierr)
      if(ierr.ne.0) goto 170
   10 m = 0
      efact = one
      if (alpha.le.1.0d+0) go to 90
      do 20 i=1,maxc
         m = m + 1
         efact = efact*two
         if (alpha.le.efact) go to 60
   20 continue
      ierr = -4
      go to 170
   30 m = m + 1
      efact = efact*two
      do 50 i=1,n
         do 40 j=1,n
            ar(i,j) = ar(i,j)/two
            ai(i,j) = ai(i,j)/two
   40    continue
   50 continue
      norm = norm/two
      go to 115
c
c we find a matrix a'=a*2-m whith a spectral radius smaller than one.
c
   60 do 80 i=1,n
         do 70 j=1,n
            ar(i,j) = ar(i,j)/efact
            ai(i,j) = ai(i,j)/efact
   70    continue
   80 continue
   90 continue
c
c
      call wcerr(ar,ai,w,ia,n,ndng,m,maxc)
c
c
      norm = zero
      do 110 i=1,n
         alpha = zero
         do 100 j=1,n
            alpha = alpha + abs(ar(i,j)) + abs(ai(i,j))
  100    continue
         if (alpha.gt.norm) norm = alpha
  110 continue
c
c compute the inverse of the denominator of dpade's approximants.
c
  115 continue
      do 130 i=1,n
         do 120 j=1,n
            ear(i,j) = -ar(i,j)
            eai(i,j) = -ai(i,j)
  120    continue
  130 continue
      call wclmat(iea,n,ear,eai,w(kr),w(ki),n,w(kw),c,ndng)
c
c compute de l-u decomposition of n (-a) and the condition number
c                                  pp
c
      call wgeco(w(kr),w(ki), n, n, ipvt, rcond, w(kw),w(kw+n))
c
      rcond=rcond**4
      if ((rcond+one .le. one) .and. ((norm.gt.one) .and.
     * (m.lt.maxc))) go to 30
c
c compute the numerator of dpade's approximants.
c
      call wclmat(ia, n, ar,ai,ear,eai, iea, w(kw), c, ndng)
c
c compute the dpade's approximants by
c
c      n (-a) x=n (a)
c      pp      pp
c
      do 150 j=1,n
         call wgesl(w(kr),w(ki), n, n, ipvt, ear(1,j),eai(1,j), 0)
  150 continue
      if (m.eq.0) go to 170
c
c remove the effects of normalization.
c
      do 160 k=1,m
        call wmmul(ear,eai,iea,ear,eai,iea,w(kr),w(ki),n,n,n,n)
        call dmcopy(w(kr),n,ear,iea,n,n)
        call dmcopy(w(ki),n,eai,iea,n,n)
  160 continue
  170 continue
      return
      end
