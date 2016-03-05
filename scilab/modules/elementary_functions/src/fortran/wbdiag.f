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
      subroutine wbdiag(lda,n,ar,ai,rmax,er,ei,bs,xr,xi,
     *     yr,yi,scale,job,fail)
c
c!purpose
c   wbdiag reduces a matrix a to block diagonal form by first
c   reducing it to triangular form by comqr3 and then by
c   solving the matrix equation -a11*p+p*a22=a12 to introduce zeros
c   above the diagonal.
c   right transformation is factored : p*d*u*y ;where:
c     p is a permutation matrix and d positive diagonal matrix,
c     p and d are given by scale
c     u is orthogonal and y block upper triangular with identity
c     blocks on the diagonal
c
c!calling sequence
c
c     subroutine wbdiag(lda,n,ar,ai,rmaxr,er,ei,bs,xr,xi,
c    * yr,yi,scale,job,fail)
c
c     integer lda, n, bs, job
c     double precision ar,ai,er,ei,xr,xi,yr,yi,rmax,scale(n)
c     dimension ar(lda,n),ai(lda,n)
c     dimension xr(lda,n),xi(lda,n),yr(lda,n),yi(lda,n),
c               er(n),ei(n),bs(n)
c     logical fail
c
c   starred parameters are altered by the subroutine
c
c
c  *ar,ai    an array that initially contains the n x n matrix
c            to be reduced. on return,  see job
c
c   lda      the leading dimension of array a. and array x,y.
c
c   n        the order of the matrices a,x,y
c
c   rmax     the maximum size allowed for any element of the
c            transformations.
c
c  *er       a singly subscripted real array containing the real
c            parts of the eigenvalues.
c
c  *ei       a singly subscripted real array containing the imaginary
c            parts of the eigenvalues.
c
c  *bs       a singly subscripted integer array that contains block
c            structure information.  if there is a block of order
c            k starting at a(l,l) in the output matrix a, then
c            bs(l) contains the positive integer k, bs(l+1) contains
c            -(k-1), bs(l+2) = -(k-2), ..., bs(l+k-1) = -1.
c            thus a positive integer in the l-th entry of bs
c            indicates a new block of order bs(l) starting at a(l,l).
c
c  *xr,xi    contains,  either right reducing transformation u*y,
c            either orthogonal tranformation u (see job)
c
c  *yr,yi    contains the inverse reducing matrix transformation
c               or y matrix (see job)
c
c  *scale    contains the scale factor and definitions of p and d
c            size(n)
c
c   job      integer parametre specifying outputed transformations
c            job=0 : a contains block diagonal form
c                    x right transformation
c                    y dummy variable
c            job=1:like job=0 and y contain x**-1
c            job=2 a contains block diagonal form
c                  x contains u and y contain y
c            job=3: a contains:
c                      -block diagonal form in the diagonal blocks
c                      -a factorization of y in the upper triangular
c                   x contains u
c                   y dummy
c  *fail     a logical variable which is false on normal return and
c            true if there is any error in wbdiag.
c
c
c!auxiliary routines
c     corth cortr comqr3 cbal balbak (eispack)
c     wexchn  (eispack.extensions)
c     dset ddot (blas)
c     wshrsl dad
c
      integer lda, n,  bs, job
      double precision ar,ai,er,ei,xr,xi,yr,yi,rmax,scale(n)
      dimension ar(lda,n),ai(lda,n),xr(lda,n),xi(lda,n)
      dimension yr(lda,n),yi(lda,n),er(n),ei(n),bs(n)
      logical fail,fails
c
      double precision c,cav,d,rav,temp,zero,one,mone,ddot,eps
      double precision dlamch
      integer da11,da22,i,j,k,km1,l11,l22,l22m1,err
      integer low,igh
c      character*100 cw
c      integer iw(200)
      data zero, one, mone /0.0d+0,1.0d+0,-1.0d+0/
c
c
      fail = .true.
c
c     compute l1 norm of a
c     
      eps=0.0d0
      do 11 j=1,n
         temp=0.0d0
         do 10 i=1,n
            temp=temp+abs(ar(i,j))+abs(ai(i,j))
 10      continue
         eps=max(eps,temp)
 11   continue
      if (eps.eq.0.0d0) eps=1.0d0
      eps=dlamch('p')*eps
c
c     convert a to upper hessenberg form.
c
      call cbal(lda,n,ar,ai,low,igh,scale)
      call corth(lda,n,1,n,ar,ai,er,ei)
      call cortr(lda, n, 1, n, ar, ai, er, ei, xr, xi)
c
c     convert a to upper triangular form by qr method.
c
      call comqr3(lda,n,1,n,ar,ai,er,ei,xr,xi,err,11)
c
c     check to see if comqr3 failed in computing any eigenvalue
c
c
      if(err.gt.1) go to 600
c
c     reduce a to block diagonal form
c
c     segment a into 4 matrices: a11, a 1 x 1 block
c     whose (1,1)-element is at a(l11,l11))  a22, a 1 x 1
c     block whose (1,1)-element is at a(l22,l22)) a12,
c     a 1 x 1 block whose (1,1)-element is at a(l11,l22))
c     and a21, a 1 x 1 block = 0 whose (1,1)-
c     element is at a(l22,l11).
c
c
c
c     this loop uses l11 as loop index and splits off a block
c     starting at a(l11,l11).
c
c
      l11 = 1
   40 continue
c      call wmdsp(ar,ai,n,n,n,10,1,80,6,cw,iw)
      if (l11.gt.n) go to 350
      l22 = l11
c
c       this loop uses da11 as loop variable and attempts to split
c       off a block of size da11 starting at a(l11,l11)
c
   50 continue
      if (l22.ne.l11) go to 60
      da11 = 1
      l22 = l11 + 1
      l22m1 = l22 - 1
      go to 240
   60 continue
c
c
c           compute the average of the eigenvalues in a11
c
      rav = zero
      cav = zero
      do 70 i=l11,l22m1
         rav = rav + er(i)
         cav = cav + abs(ei(i))
   70 continue
      rav = rav/dble(real(da11)   )
      cav = cav/dble(real(da11)   )
c
c           loop on eigenvalues of a22 to find the one closest to the av
c
      d = (rav-er(l22))**2 + (cav-ei(l22))**2
      k = l22
      l = l22  + 1
   80 continue
      if (l.gt.n) go to 100
      c = (rav-er(l))**2 + (cav-ei(l))**2
      if (c.ge.d) go to 90
      k = l
      d = c
   90 continue
      l = l  + 1
      go to 80
  100 continue
c
c
c           loop to move the eigenvalue just located
c           into first position of block a22.
c
c
c             the block we're moving to add to a11 is a 1 x 1
c
  110 continue
      if (k.eq.l22) go to 230
      km1 = k - 1
      call wexchn(ar,ai,xr,xi,n, km1, fail, lda, lda)
      if (fail) go to 600
      temp = er(k)
      er(k) = er(km1)
      er(km1) = temp
      temp = ei(k)
      ei(k) = ei(km1)
      ei(km1) = temp
      k = km1
      if (k.le.l22) go to 230
      go to 110
c
  230 continue
      da11 = da11 + 1
      l22 = l11 + da11
      l22m1 = l22 - 1
  240 continue
      if (l22.gt.n) go to 290
c
c       attempt to split off a block of size da11.
c
      da22 = n - l22 + 1
c
c       save a12 in its transpose form in block a21.
c
      do 260 j=l11,l22m1
         do 250 i=l22,n
            ar(i,j) = ar(j,i)
            ai(i,j) = ai(j,i)
  250    continue
  260 continue
c
c
c       convert a11 to lower quasi-triangular and multiply it by -1 and
c       a12 appropriately (for solving -a11*p+p*a22=a12).
c
c      write(6,'(''da11='',i2,''da22='',i2)') da11,da22
c      write(6,'(''a'')')
c      call wmdsp(ar,ai,n,n,n,10,1,80,6,cw,iw)
      call dad(ar, lda, l11, l22m1, l11, n, one, 0)
      call dad(ar, lda, l11, l22m1, l11, l22m1, mone, 1)
      call dad(ai, lda, l11, l22m1, l11, n, one, 0)
      call dad(ai, lda, l11, l22m1, l11, l22m1, mone, 1)
c
c       solve -a11*p + p*a22 = a12.
c
      call wshrsl(ar(l11,l11),ai(l11,l11), ar(l22,l22),ai(l22,l22),
     1 ar(l11,l22),ai(l11,l22),da11,da22,lda,lda,lda,eps,rmax,fails)
      if (.not.fails) go to 290
c
c       change a11 back to upper quasi-triangular.
c
      call dad(ar, lda, l11, l22m1, l11, l22m1, one, 1)
      call dad(ar, lda, l11, l22m1, l11, l22m1, mone, 0)
      call dad(ai, lda, l11, l22m1, l11, l22m1, one, 1)
      call dad(ai, lda, l11, l22m1, l11, l22m1, mone, 0)
c      write(6,'(''failed a'')')
c      call wmdsp(ar,ai,n,n,n,10,1,80,6,cw,iw)
c
c       was unable to solve for p - try again
c
c
c       move saved a12 back into its correct position.
c
      do 280 j=l11,l22m1
         do 270 i=l22,n
            ar(j,i) = ar(i,j)
            ar(i,j) = zero
            ai(j,i) = ai(i,j)
            ai(i,j) = zero
  270    continue
  280 continue
c
c
      go to 50
  290 continue
c
c     change solution to p to proper form.
c
      if (l22.gt.n) go to 300
      call dad(ar, lda, l11, l22m1, l11, n, one, 0)
      call dad(ar, lda, l11, l22m1, l11, l22m1, mone, 1)
      call dad(ai, lda, l11, l22m1, l11, n, one, 0)
      call dad(ai, lda, l11, l22m1, l11, l22m1, mone, 1)
c      write(6,'(''not failed a'')')
c      call wmdsp(ar,ai,n,n,n,10,1,80,6,cw,iw)
c
c
c     store block size in array da11s.
c
  300 bs(l11) = da11
      j = da11 - 1
      if (j.eq.0) go to 320
      do 310 i=1,j
         l11pi = l11 + i
         bs(l11pi) = -(da11-i)
  310 continue
  320 continue
      l11 = l22
      go to 40
  350 continue
      fail=.false.
c
c     set transformations matrices as required
c
      if(job.eq.3) return
c
c compute inverse transformation
      if(job.ne.1) goto 450
      do 410 i=1,n
      do 410 j=1,n
      yr(i,j)=xr(j,i)
      yi(i,j)=-xi(j,i)
  410 continue
      l22=1
  420 l11=l22
      l22=l11+bs(l11)
      if(l22.gt.n) goto 431
      l22m1=l22-1
      do 430 i=l11,l22m1
      do 430 j=1,n
      yr(i,j)=yr(i,j)-ddot(n-l22m1,ar(i,l22),lda,yr(l22,j),1)
     1               +ddot(n-l22m1,ai(i,l22),lda,yi(l22,j),1)
      yi(i,j)=yi(i,j)-ddot(n-l22m1,ar(i,l22),lda,yi(l22,j),1)
     1               -ddot(n-l22m1,ai(i,l22),lda,yr(l22,j),1)
  430 continue
      goto 420
c
c in-lines back-tranfc in-lines right transformations of xi
  431 continue
      if (igh .ne. low) then
         do 435 j=low,igh
            temp=1.0d+00/scale(j)
            do 434 i=1,n
               yr(i,j)=yr(i,j)*temp
               yi(i,j)=yi(i,j)*temp
  434       continue
  435    continue
      endif
      do 445 ii=1,n
         i=ii
         if (i.ge.low .and. i.le.igh) goto 445
         if (i.lt.low) i=low-ii
         k=scale(i)
         if (k.eq.i) goto 445
         do 444 j=1,n
            temp=yr(j,i)
            yr(j,i)=yr(j,k)
            yr(j,k)=temp
            temp=yi(j,i)
            yi(j,i)=yi(j,k)
            yi(j,k)=temp
  444    continue
  445 continue
c
c
  450 continue
      if(job.eq.2) goto 500
c compute right transformation
      l22=1
  460 l11=l22
      l22=l11+bs(l11)
      if(l22.gt.n) goto 480
      do 470 j=l22,n
      do 470 i=1,n
      xr(i,j)=xr(i,j)+ddot(l22-l11,xr(i,l11),lda,ar(l11,j),1)
     1               -ddot(l22-l11,xi(i,l11),lda,ai(l11,j),1)
      xi(i,j)=xi(i,j)+ddot(l22-l11,xr(i,l11),lda,ai(l11,j),1)
     1               +ddot(l22-l11,xi(i,l11),lda,ar(l11,j),1)
  470 continue
      goto 460
c
  480 continue
      call balbak( lda, n, low, igh, scale, n, xr)
      call balbak( lda, n, low, igh, scale, n, xi)
      goto 550
c
c extract non orthogonal tranformation from a
  500 continue
      do 510 j=1,n
      call dset(n,zero,yr(1,j),1)
      call dset(n,zero,yi(1,j),1)
  510 continue
      call dset(n,one,yr(1,1),lda+1)
      call dset(n,one,yi(1,1),lda+1)
      l22=1
  520 l11=l22
      if(l11.gt.n) goto 550
      l22=l11+bs(l11)
      do 530 j=l22,n
      do 530 i=1,n
      yr(i,j)=yr(i,j)+ddot(l22-l11,yr(i,l11),lda,ar(l11,j),1)
     1               -ddot(l22-l11,yi(i,l11),lda,ai(l11,j),1)
      yi(i,j)=yi(i,j)+ddot(l22-l11,yr(i,l11),lda,ai(l11,j),1)
     1               +ddot(l22-l11,yi(i,l11),lda,ar(l11,j),1)
  530 continue
      goto 520
c
c set zeros in the matrix a
  550 l11=1
  560 l22=l11+bs(l11)
      if(l22.gt.n) return
      l22m1=l22-1
      do 570 j=l11,l22m1
      call dset(n-l22m1,zero,ar(j,l22),lda)
      call dset(n-l22m1,zero,ar(l22,j),1)
      call dset(n-l22m1,zero,ai(j,l22),lda)
      call dset(n-l22m1,zero,ai(l22,j),1)
  570 continue
      l11=l22
      goto 560
c
c     error return.
c
  600 continue
      fail = .true.
      return
      end
      
