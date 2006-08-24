      subroutine bdiag(lda,n,a,epsshr,rmax,er,ei,bs,x,xi,scale,
     1                 job,fail)
c
c!purpose
c   dbdiag reduces a matrix a to block diagonal form by first
c   reducing it to quasi-triangular form by hqror2 and then by
c   solving the matrix equation -a11*p+p*a22=a12 to introduce zeros
c   above the diagonal.
c   right transformation is factored : p*d*u*y ;where:
c     p is a permutation matrix and d positive diagonal matrix
c     u is orthogonal and y block upper triangular with identity
c     blocks on the diagonal
c
c!calling sequence
c
c     subroutine bdiag(lda,n,a,epsshr,rmax,er,ei,bs,x,xi,scale,
c    * job,fail)
c
c     integer lda, n,  bs, job
c     double precision a,er,ei,x,xi,rmax,epsshr,scale
c     dimension a(lda,n),x(lda,n),xi(lda,n),er(n),ei(n),bs(n)
c     dimension scale(n)
c     logical fail
c
c   starred parameters are altered by the subroutine
c
c
c  *a        an array that initially contains the m x n matrix
c            to be reduced. on return,  see job
c
c   lda      the leading dimension of array a. and array x,xi.
c
c   n        the order of the matrices a,x,xi
c
c   epsshr   the minimal conditionnement allowed for linear sytems
c
c   rmax     the maximum size allowed for any element of the
c            transformations.
c
c  *er       a singly subscripted real array containing the real
c            parts of the eigenvalues.
c
c  *ei       a singly subscripted real array containg the imaginary
c            parts of the eigenvalues.
c
c  *bs       a singly subscripted integer array that contains block
c            structure information.  if there is a block of order
c            k starting at a(l,l) in the output matrix a, then
c            bs(l) contains the positive integer k, bs(l+1) contains
c            -(k-1), bs(la+2) = -(k-2), ..., bs(l+k-1) = -1.
c            thus a positive integer in the l-th entry of bs
c            indicates a new block of order bs(l) starting at a(l,l).
c
c  *x        contains,  either right reducing transformation u*y,
c            either orthogonal tranformation u (see job)
c
c  *xi       xi contains the inverse reducing matrix transformation
c               or y matrix (see job)
c
c  *scale    contains the scale factor and definitions of p size(n)
c
c   job      integer parametre specifying outputed transformations
c            job=0 : a contains block diagonal form
c                    x right transformation
c                    xi dummy variable
c            job=1:like job=0 and xi contain x**-1
c            job=2 a contains block diagonal form
c                  x contains u and xi contain y
c            job=3: a contains:
c                      -block diagonal form in the diagonal blocks
c                      -a factorisation of y in the upper triangular
c                   x contains u
c                   xi dummy
c  *fail     a logical variable which is false on normal return and
c            true if there is any error in bdiag.
c
c
c!auxiliary routines
c     orthes ortran (eispack)
c     hqror2 exch split (eispack.extensions)
c     dset ddot (blas)
c     real dble abs (fortran)
c     shrslv dad
c
c!
c
      integer lda, n,  bs, job
      double precision a,er,ei,x,xi,rmax,epsshr,scale(n)
      dimension a(lda,n),x(lda,n),xi(lda,n),er(n),ei(n),bs(n)
      logical fail,fails
c
      double precision c,cav,d,e1,e2,rav,temp,zero,one,mone,ddot,eps
      double precision dlamch
      integer da11,da22,i,j,k,km1,km2,l11,l22,l22m1,nk,ino
      integer low,igh
      data zero, one, mone /0.0d+0,1.0d+0,-1.0d+0/
c
c
      fail = .false.
      fails= .true.
      ino  =  -1
c
c     compute eps the l1 norm of the a matrix
c
      eps=0.0d0
      do 11 j=1,n
         temp=0.0d0
         do 10 i=1,n
            temp=temp+abs(a(i,j))
 10      continue
         eps=max(eps,temp)
 11   continue
      if (eps.eq.0.0d0) eps=1.0d0
      eps=dlamch('p')*eps


c
c     convert a to upper hessenberg form.
c
      call balanc(lda, n,   a, low, igh, scale)
      call orthes(lda, n, low, igh,   a, er)
      call ortran(lda, n, low, igh,   a, er, x)
c
c     convert a to quasi-upper triangular form by qr method.
c
      call hqror2(lda,n,1,n,a,er,ei,x,ierr,21)
c
c     check to see if hqror2 failed in computing any eigenvalue
c
c
      if(ierr.gt.1) goto 600
c
c     reduce a to block diagonal form
c
c
c     segment a into 4 matrices: a11, a da11 x da11 block
c     whose (1,1)-element is at a(l11,l11))  a22, a da22 x da22
c     block whose (1,1)-element is at a(l22,l22)) a12,
c     a da11 x da22 block whose (1,1)-element is at a(l11,l22))
c     and a21, a da22 x da11 block = 0 whose (1,1)-
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
      if (l11.gt.n) go to 350
      l22 = l11
c
c       this loop uses da11 as loop variable and attempts to split
c       off a block of size da11 starting at a(l11,l11)
c
   50 continue
      if (l22.ne.l11) go to 60
      da11 = 1
      if(l11 .eq. n) go to 51
      if(abs(a(l11+1,l11)) .gt.eps ) then
         da11 = 2
      endif
   51 continue
      l22 = l11 + da11
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
      l = l22 + 1
      if(l22 .eq. n) go to 71
      if(abs(a(l22+1,l22)) .gt. eps) l = l22 + 2
   71 continue
   80 continue
      if (l.gt.n) go to 100
      c = (rav-er(l))**2 + (cav-ei(l))**2
      if (c.ge.d) go to 90
      k = l
      d = c
   90 continue
      l = l + 1
      if(l.gt.n) go to 100
      if (abs(a(l,l-1)).gt.eps) l=l+1
      go to 80
  100 continue
c
c
c           loop to move the eigenvalue just located
c           into first position of block a22.
c
      if (k.eq.n) goto 105
      if (abs(a(k+1,k)).gt.eps) go to 150
c
c             the block we're moving to add to a11 is a 1 x 1
c
  105 nk = 1
  110 continue
      if (k.eq.l22) go to 230
      km1 = k - 1
      if (abs(a(km1,k-2)).lt.eps) go to 140
c
c             we're swapping the closest block with a 2 x 2
c
      km2 = k - 2
      call exch(lda,n,a, x, km2, 2, 1)
c
c             try to split this block into 2 real eigenvalues
c
      call split(a, x, n, km1, e1, e2, lda, lda)
      if (a(k,km1).eq.zero) go to 120
c
c             block is still complex.
c
      er(km2) = er(k)
      ei(km2) = zero
      er(k) = e1
      er(km1) = e1
      ei(km1) = e2
      ei(k) = -e2
      go to 130
c
c             complex block split into two real eigenvalues.
c
  120 continue
      er(km2) = er(k)
      er(km1) = e1
      er(k) = e2
      ei(km2) = zero
      ei(km1) = zero
  130 k = km2
      if (k.le.l22) go to 230
      go to 110
c
c
c             we're swapping the closest block with a 1 x 1.
c
  140 continue
      call exch(lda,n,a, x, km1, 1, 1)
      temp = er(k)
      er(k) = er(km1)
      er(km1) = temp
      k = km1
      if (k.le.l22) go to 230
      go to 110
c
c             the block we're moving to add to a11 is a 2 x 2.
c
  150 continue
      nk = 2
  160 continue
      if (k.eq.l22) go to 230
      km1 = k - 1
      if (abs(a(km1,k-2)).lt.eps) goto 190
c
c             we're swapping the closest block with a 2 x 2 block.
c
      km2 = k - 2
      call exch(lda,n,a, x, km2, 2, 2)
c
c             try to split swapped block into two reals.
c
      call split(a, x, n, k, e1, e2, lda, lda)
      er(km2) = er(k)
      er(km1) = er(k+1)
      ei(km2) = ei(k)
      ei(km1) = ei(k+1)
      if (a(k+1,k).eq.zero) go to 170
c
c             still complex block.
c
      er(k) = e1
      er(k+1) = e1
      ei(k) = e2
      ei(k+1) = -e2
      go to 180
c
c             two real roots.
c
  170 continue
      er(k) = e1
      er(k+1) = e2
      ei(k) = zero
      ei(k+1) = zero
  180 continue
      k = km2
      if (k.eq.l22) go to 210
      go to 160
c
c             we're swapping the closest block with a 1 x 1.
c
  190 continue
      call exch(lda,n,a, x, km1, 1, 2)
      er(k+1) = er(km1)
      er(km1) = er(k)
      ei(km1) = ei(k)
      ei(k) = ei(k+1)
      ei(k+1) = zero
      go to 200
c
  200 continue
      k = km1
      if (k.eq.l22) go to 210
      go to 160
c
c             try to split relocated complex block.
c
  210 continue
      call split(a, x, n, k, e1, e2, lda, lda)
      if (a(k+1,k).eq.zero) go to 220
c
c             still complex.
c
      er(k) = e1
      er(k+1) = e1
      ei(k) = e2
      ei(k+1) = -e2
      go to 230
c
c             split into two real eigenvalues.
c
  220 continue
      er(k) = e1
      er(k+1) = e2
      ei(k) = zero
      ei(k+1) = zero
c
  230 continue
      da11 = da11 + nk
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
            a(i,j) = a(j,i)
  250    continue
  260 continue
c
c
c       convert a11 to lower quasi-triangular and multiply it by -1 and
c       a12 appropriately (for solving -a11*p+p*a22=a12).
c
      call dad(a, lda, l11, l22m1, l11, n, one, 0)
      call dad(a, lda, l11, l22m1, l11, l22m1, mone, 1)
c
c       solve -a11*p + p*a22 = a12.
c
      call shrslv(a(l11,l11), a(l22,l22), a(l11,l22), da11,
     * da22, lda, lda, lda, eps,epsshr,rmax, fails)
      if (.not.fails) go to 290
c
c       change a11 back to upper quasi-triangular.
c
      call dad(a, lda, l11, l22m1, l11, l22m1, one, 1)
      call dad(a, lda, l11, l22m1, l11, l22m1, mone, 0)
c
c       was unable to solve for p - try again
c
c
c       move saved a12 back into its correct position.
c
      do 280 j=l11,l22m1
         do 270 i=l22,n
            a(j,i) = a(i,j)
            a(i,j) = zero
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
      call dad(a, lda, l11, l22m1, l11, n, one, 0)
      call dad(a, lda, l11, l22m1, l11, l22m1, mone, 1)

c
c     store block size in array bs.
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
      xi(i,j)=x(j,i)
  410 continue
      l22=1
  420 l11=l22
      l22=l11+bs(l11)
      if(l22.gt.n) goto 431
      l22m1=l22-1
      do 430 i=l11,l22m1
      do 430 j=1,n
      xi(i,j)=xi(i,j)-ddot(n-l22m1,a(i,l22),lda,xi(l22,j),1)
  430 continue
      goto 420
c in-lines back-tranfc in-lines right transformations of xi
  431 continue
      if (igh .ne. low) then
         do 435 j=low,igh
            temp=1.0d+00/scale(j)
            do 434 i=1,n
               xi(i,j)=xi(i,j)*temp
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
            temp=xi(j,i)
            xi(j,i)=xi(j,k)
            xi(j,k)=temp
  444    continue
  445 continue
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
      x(i,j)=x(i,j)+ddot(l22-l11,x(i,l11),lda,a(l11,j),1)
  470 continue
      goto 460
c
  480 continue
      call balbak( lda, n, low, igh, scale, n, x)
      goto 550
c
c extract non orthogonal tranformation from a
  500 continue
      do 510 j=1,n
      call dset(n,zero,xi(1,j),1)
  510 continue
      call dset(n,one,xi(1,1),lda+1)
      l22=1
  520 l11=l22
      if(l11.gt.n) goto 550
      l22=l11+bs(l11)
      do 530 j=l22,n
      do 530 i=1,n
      xi(i,j)=xi(i,j)+ddot(l22-l11,xi(i,l11),lda,a(l11,j),1)
  530 continue
      goto 520
c
c set zeros in the matrix a
  550 l11=1
  560 l22=l11+bs(l11)
      if(l22.gt.n) return
      l22m1=l22-1
      do 570 j=l11,l22m1
      call dset(n-l22m1,zero,a(j,l22),lda)
      call dset(n-l22m1,zero,a(l22,j),1)
  570 continue
      l11=l22
      goto 560
c
c     error return.
c
  600 continue
      fail = .true.
c
      end
