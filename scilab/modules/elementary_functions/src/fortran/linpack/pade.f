      subroutine pade(a,ia,n,ea,iea,alpha,wk,ipvt,ierr)
c
c!purpose
c      compute the pade approximants of the exponential of a
c      matrix a. we scale a until the spectral radius of a*2**-m
c      are smaler than one.
c
c!calling sequence
c
c     subroutine pade(a,ia,n,ea,iea,alpha,wk,ipvt,ierr)
c
c     integer ia,n,iea,ipvt,ierr
c     double precision a,ea,alpha,wk,
c     dimension a(ia,*),ea(iea,*),wk(*),ipvt(*)
c
c      a         : array containing the matrix a
c      ia        : the leading dimension of arrays a.
c      n         : the order of the matrices a,ea .
c      ea        : the  array that contains the n*n
c                  matrix exp(a).
c      iea       : the leading dimension of array ea.
c      alpha     : variable containing the maximun
c                  norm of the eigenvalues of a.
c      wk        : workspace array of size 2*n*(n+1)
c      ipvt      : integer workspace of size n
c      ierr      : error indicator
c                  ierr= 0 if normal return
c                      =-4 if alpha is to big for any accuracy.
c
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
c       dclmat  coef cerr (j. roche)
c       dmmul dmcopy (blas.extension)
c       dgeco dgesl (linpack)
c       sqrt (fortran)
c!
c
      integer ia,n,iea,ipvt,ierr
      double precision a,ea,alpha,wk
      dimension a(ia,*),ea(iea,*),wk(*),ipvt(*)
c internal variables
      integer i,j,k,m,ndng,maxc,n2
      double precision rcond,c,efact,two,zero,norm,one
      dimension c(41)
c
      common /dcoeff/ c, ndng
c
      data zero, one, two, maxc /0.0d+0,1.0d+0,2.0d+0,10/
      n2=n*n
c
      if (ndng.ge.0) go to 10
c
c compute de pade's aprroximants type which is necessary to obtain
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
            a(i,j) = a(i,j)/two
   40    continue
   50 continue
      norm = norm/two
      go to 115
c
c we find a matrix a'=a*2-m whith a spectral radius smaller than one.
c
   60 do 80 i=1,n
         do 70 j=1,n
            a(i,j) = a(i,j)/efact
   70    continue
   80 continue
   90 continue
c
c
      call cerr(a,  wk,  ia, n, ndng, m, maxc)
c
c
      norm = zero
      do 110 i=1,n
         alpha = zero
         do 100 j=1,n
            alpha = alpha + abs(a(i,j))
  100    continue
         if (alpha.gt.norm) norm = alpha
  110 continue
c
c compute the inverse of the denominator of dpade's approximants.
c
  115 continue
      do 130 i=1,n
         do 120 j=1,n
            ea(i,j) = -a(i,j)
  120    continue
  130 continue
      call dclmat(iea, n, ea, wk, n, wk(n2+1), c, ndng)
c
c compute de l-u decomposition of n (-a) and the condition numbwk(n2+1)
c                                  pp
c
      call dgeco(wk, n, n, ipvt, rcond, wk(n2+1))
c
      rcond=rcond*rcond*rcond*rcond
      if ((rcond+one.le.one) .and. ((norm.gt.one) .and.
     * (m.lt.maxc))) go to 30
c
c compute the numerator of dpade's approximants.
c
      call dclmat(ia, n, a, ea, iea, wk(n2+1), c, ndng)
c
c compute the dpade's approximants by
c
c      n (-a) x=n (a)
c      pp      pp
c
      do 150 j=1,n
         call dgesl(wk, n, n, ipvt, ea(1,j), 0)
  150 continue
      if (m.eq.0) go to 170
c
c remove the effects of normalization.
c
      do 160 k=1,m
        call dmmul(ea,iea,ea,iea,wk,n,n,n,n)
        call dmcopy(wk,n,ea,iea,n,n)
  160 continue
  170 continue
      return
      end
