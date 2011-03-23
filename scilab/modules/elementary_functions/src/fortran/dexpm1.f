      subroutine dexpm1(ia,n,a,ea,iea,w,iw,ierr)
c
c!purpose
c      compute the exponential of a matrix a by the pade's
c      approximants(subroutine pade).a block diagonalization
c         is performed prior call pade.
c!calling sequence
c     subroutine dexpm1(ia,n,a,ea,iea,w,iw,ierr)
c
c     integer ia,n,iw,ierr
c     double precision a,ea,w
c     dimension a(ia,n),ea(iea,n),w(*),iw(*)
c
c      ia: the leading dimension of array a.
c      n: the order of the matrices a,ea,x,xi .
c      a: the real double precision array that contains the n*n matrix a
c      ea: the array that contains the n*n exponential of a.
c      iea : the leading dimension of array ea
c      w : work space array of size: n*(2*ia+2*n+5)
c      iw : integer work space array of size 2*n
c      ierr: =0 if the prosessus is normal.
c               =-1 if n>ia.
c               =-2 if the block diagonalization is not possible.
c               =-4 if the subroutine dpade can not computes exp(a)
c
c!auxiliary routines
c     exp abs sqrt dble real (fortran)
c     bdiag (eispack.extension)
c     balanc balinv (eispack)
c     dmmul (blas.extension)
c     pade
c! originator
c     j roche laboratoire d'automatique de grenoble
c!
      integer ia,n,iw,ierr
      double precision a,ea,w
      dimension a(ia,*),ea(iea,*),w(*),iw(*)
c internal variables
c
      integer i,j,k,ni,nii,ndng
      double precision anorm,alpha,bvec,bbvec,rn,zero,c(41)
      logical fail
c
cDEC$ IF DEFINED (FORDLL)
cDEC$ ATTRIBUTES DLLIMPORT:: /dcoeff/
cDEC$ ENDIF
      common /dcoeff/ c,ndng
      data zero /0.0d+0/
      ndng=-1
c
      ierr=0
      kscal=1
      kx=kscal+n
      kxi=kx+n*ia
      ker=kxi+n*ia
      kei=ker+n
      kw=kei+n
c
      kbs=1
      kiw=kbs+n
c
      if (n.gt.ia) go to 110
c
c  balance the matrix a
c
c
c  compute the norm one of a.
c
      anorm = 0.0d+0
      do 20 j=1,n
         alpha = zero
         do 10 i=1,n
            alpha = alpha + abs(a(i,j))
   10    continue
         if (alpha.gt.anorm) anorm = alpha
   20 continue
      if (anorm.eq.0.0d0) then
c     null matrix special case (Serge Steer 96)
         do 21 j=1,n
            call dset(n,0.0d+0,ea(j,1),iea)
            ea(j,j)=1.0d0
 21      continue
         return
      endif
      anorm=max(anorm,1.0d0)
c
c call bdiag whith rmax equal to the norm one of matrix a.
c
      call bdiag(ia,n,a,0.0d+0,anorm,w(ker),w(kei),
     * iw(kbs),w(kx),w(kxi),w(kscal),1,fail)
      if (fail) go to 120
      do 25 j=1,n
      call dset(n,0.0d+0,ea(j,1),iea)
   25 continue
c
c  compute the pade's approximants of the block.
c block origin is shifted before calling pade.
c
      ni = 1
      k = 0
c
c  loop on the block.
c
   30 k = k + ni
      if (k.gt.n) go to 100
      ni = iw(kbs-1+k)
      if (ni.eq.1) go to 90
      nii = k + ni - 1
      bvec = zero
      do 40 i=k,nii
         bvec = bvec + w(ker-1+i)
   40 continue
      bvec = bvec/dble(real(ni))
      do 50 i=k,nii
         w(ker-1+i) = w(ker-1+i) - bvec
         a(i,i) = a(i,i) - bvec
   50 continue
      alpha = zero
      do 60 i=k,nii
         rn = w(ker-1+i)**2 + w(kei-1+i)**2
         rn = sqrt(rn)
         if (rn.gt.alpha) alpha = rn
   60 continue
c
c call pade subroutine.
c
      call pade(a(k,k),ia,ni,ea(k,k),iea,alpha,w(kw),iw(kiw),
     1   ierr)
      if (ierr.lt.0) go to 130
c
c remove the effect of origin shift on the block.
c
      bbvec = exp(bvec)
      do 80 i=k,nii
         do 70 j=k,nii
            ea(i,j) = ea(i,j)*bbvec
   70    continue
   80 continue
      go to 30
   90 ea(k,k) = exp(a(k,k))
      go to 30
c
c end of loop.
c
  100 continue
c
c remove the effect of block diagonalization.
c
      call dmmul(w(kx),ia,ea,iea,w(kw),n,n,n,n)
      call dmmul(w(kw),n,w(kxi),ia,ea,iea,n,n,n)
c
c remove the effects of balance
c
c
c error output
c
      go to 130
  110 ierr = -1
      go to 130
  120 ierr = -2
  130 continue
      return
      end
