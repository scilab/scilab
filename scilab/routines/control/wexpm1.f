      subroutine wexpm1(n,ar,ai,ia,ear,eai,iea,w,iw,ierr)
c
c!purpose
c      compute the exponential of a complex matrix a by the pade's
c      approximants(subroutine pade).a block diagonalization
c         is performed prior call pade.
c!calling sequence
c     subroutine wexpm1(n,ar,ai,ia,ear,eai,iea,w,iw,ierr)
c
c     integer ia,n,iw,ierr
c     double precision ar,ai,ear,eai,w
c     dimension ar(ia,n),ai(ia,n),ear(iea,n),eai(iea,n),w(*),iw(*)
c
c      n: the order of the matrices a,ea, .
c      ar,ai :the  array that contains :the n*n matrix a
c      ia: the leading dimension of array a.
c      ear,eai: the array that contains the n*n exponential of a.
c      iea    :the leading dimension of ea
c      w : work space array of size: n*(4*ia+4*n+7)
c      iw : integer work space array of size 2*n
c      ierr: =0 if the prosessus is normal.
c               =-1 if n>ia.
c               =-2 if the block diagonalization is not possible.
c               =-4 if the subroutine dpade can not computes exp(a)
c
c!auxiliary routines
c     cos sin exp abs sqrt dble real (fortran)
c     wbdiag wbalin (eispack.extension)
c     cbal (eispack)
c     wmmul (blas.extension)
c     wpade
c! originator
c     S Steer INRIA  from dexpm1:
c     j roche laboratoire d'automatique de grenoble
c!
c     Copyright INRIA
      integer ia,n,iw,ierr
      double precision ar,ai,ear,eai,w
      dimension ar(ia,n),ai(ia,n),ear(iea,n),eai(iea,n),w(*),iw(*)
c internal variables
c
      integer i,j,k,ni,nii,ndng
      double precision anorm,alpha,bvecr,bveci,bbvec,rn,zero,c(41)
      logical fail
c
      common /dcoeff/ c,ndng
c
      data zero /0.0d+0/
      ndng=-1
c
      ierr=0
      nn=n*n
      kscal=1
      kxr=kscal+n
      kxi=kxr+n*ia
      kyr=kxi+n*ia
      kyi=kyr+n*ia
      ker=kyi+n*ia
      kei=ker+n
      kw=kei+n
c
      kbs=1
      kpvt=kbs+n
c
      if (n.gt.ia) go to 110
c
c  compute the norm one of a.
c
      anorm = 0.0d+0
      do 20 j=1,n
         alpha = zero
         do 10 i=1,n
            alpha = alpha + abs(ar(i,j)) + abs(ai(i,j))
   10    continue
         if (alpha.gt.anorm) anorm = alpha
   20 continue
      if (anorm.eq.0.0d0) then
c     null matrix special case (Serge Steer 96)
         do 21 j=1,n
            call dset(n,0.0d+0,ear(j,1),iea)
            call dset(n,0.0d+0,eai(j,1),iea)
            ear(j,j)=1.0d0
 21      continue
         return
      endif
      anorm=max(anorm,1.0d0)
c
c call wbdiag whith rmax equal to the norm one of matrix a.
c
      call wbdiag(ia,n,ar,ai,anorm,w(ker),w(kei),
     * iw(kbs),w(kxr),w(kxi),w(kyr),w(kyi),w(kscal),1,fail)
      if (fail) go to 120
c
c clear matrix ea
      do 25 j=1,n
      call dset(n,0.0d+0,ear(j,1),iea)
      call dset(n,0.0d+0,eai(j,1),iea)
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
      bvecr = zero
      bveci = zero
      do 40 i=k,nii
         bvecr = bvecr + w(ker-1+i)
         bveci = bveci + w(kei-1+i)
   40 continue
      bvecr = bvecr/dble(real(ni))
      bveci = bveci/dble(real(ni))
      do 50 i=k,nii
         w(ker-1+i) = w(ker-1+i) - bvecr
         w(kei-1+i) = w(kei-1+i) - bveci
         ar(i,i) = ar(i,i) - bvecr
         ai(i,i) = ai(i,i) - bveci
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
      call wpade(ar(k,k),ai(k,k),ia,ni,ear(k,k),eai(k,k),iea,
     *    alpha,w(kw),iw(kpvt),ierr)
      if (ierr.lt.0) go to 130
c
c remove the effect of origin shift on the block.
c
      bbvec = exp(bvecr)
      bvecr=bbvec*cos(bveci)
      bveci=bbvec*sin(bveci)
      do 80 i=k,nii
         do 70 j=k,nii
            bbvec = ear(i,j)*bvecr - eai(i,j)*bveci
            eai(i,j) = ear(i,j)*bveci + eai(i,j)*bvecr
            ear(i,j) = bbvec
   70    continue
   80 continue
      go to 30
   90 bbvec=exp(ar(k,k))
      ear(k,k) = bbvec * cos(ai(k,k))
      eai(k,k) = bbvec * sin(ai(k,k))
      go to 30
c
c end of loop.
c
  100 continue
c
c remove the effect of block diagonalization.
c
      call wmmul(w(kxr),w(kxi),ia,ear,eai,iea,w(kw),w(kw+nn),n,n,n,n)
      call wmmul(w(kw),w(kw+nn),n,w(kyr),w(kyi),ia,ear,eai,iea,n,n,n)
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
