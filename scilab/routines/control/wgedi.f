      subroutine wgedi(ar,ai,lda,n,ipvt,detr,deti,workr,worki,job)
c     Copyright INRIA
      integer lda,n,ipvt(*),job
      double precision ar(lda,*),ai(lda,*),detr(2),deti(2),workr(*),
     *                 worki(*)
c!purpose
c
c     wgedi computes the determinant and inverse of a matrix
c     using the factors computed by wgeco or wgefa.
c
c!calling sequence
c
c      subroutine wgedi(ar,ai,lda,n,ipvt,detr,deti,workr,worki,job)
c     on entry
c
c        a       double-complex(lda, n)
c                the output from wgeco or wgefa.
c
c        lda     integer
c                the leading dimension of the array  a .
c
c        n       integer
c                the order of the matrix  a .
c
c        ipvt    integer(n)
c                the pivot vector from wgeco or wgefa.
c
c        work    double-complex(n)
c                work vector.  contents destroyed.
c
c        job     integer
c                = 11   both determinant and inverse.
c                = 01   inverse only.
c                = 10   determinant only.
c
c     on return
c
c        a       inverse of original matrix if requested.
c                otherwise unchanged.
c
c        det     double-complex(2)
c                determinant of original matrix if requested.
c                otherwise not referenced.
c                determinant = det(1) * 10.0**det(2)
c                with  1.0 .le. cabs1(det(1) .lt. 10.0
c                or  det(1) .eq. 0.0 .
c
c     error condition
c
c        a division by zero will occur if the input factor contains
c        a zero on the diagonal and the inverse is requested.
c        it will not occur if the subroutines are called correctly
c        and if wgeco has set rcond .gt. 0.0 or wgefa has set
c        info .eq. 0 .
c
c!originator
c     linpack. this version dated 07/01/79 .
c     cleve moler, university of new mexico, argonne national lab.
c
c!auxiliary routines
c
c     blas waxpy,wscal,wswap
c     fortran abs,mod
c
c!
c     internal variables
c
      double precision tr,ti
      double precision ten
      integer i,j,k,kb,kp1,l,nm1
c
      double precision zdumr,zdumi
      double precision cabs1
      cabs1(zdumr,zdumi) = abs(zdumr) + abs(zdumi)
c
c     compute determinant
c
      if (job/10 .eq. 0) go to 80
         detr(1) = 1.0d+0
         deti(1) = 0.0d+0
         detr(2) = 0.0d+0
         deti(2) = 0.0d+0
         ten = 10.0d+0
         do 60 i = 1, n
            if (ipvt(i) .eq. i) go to 10
               detr(1) = -detr(1)
               deti(1) = -deti(1)
   10       continue
            call wmul(ar(i,i),ai(i,i),detr(1),deti(1),detr(1),deti(1))
c           ...exit
c        ...exit
            if (cabs1(detr(1),deti(1)) .eq. 0.0d+0) go to 70
   20       if (cabs1(detr(1),deti(1)) .ge. 1.0d+0) go to 30
               detr(1) = ten*detr(1)
               deti(1) = ten*deti(1)
               detr(2) = detr(2) - 1.0d+0
               deti(2) = deti(2) - 0.0d+0
            go to 20
   30       continue
   40       if (cabs1(detr(1),deti(1)) .lt. ten) go to 50
               detr(1) = detr(1)/ten
               deti(1) = deti(1)/ten
               detr(2) = detr(2) + 1.0d+0
               deti(2) = deti(2) + 0.0d+0
            go to 40
   50       continue
   60    continue
   70    continue
   80 continue
c
c     compute inverse(u)
c
      if (mod(job,10) .eq. 0) go to 160
         do 110 k = 1, n
            call wdiv(1.0d+0,0.0d+0,ar(k,k),ai(k,k),ar(k,k),ai(k,k))
            tr = -ar(k,k)
            ti = -ai(k,k)
            call wscal(k-1,tr,ti,ar(1,k),ai(1,k),1)
            kp1 = k + 1
            if (n .lt. kp1) go to 100
            do 90 j = kp1, n
               tr = ar(k,j)
               ti = ai(k,j)
               ar(k,j) = 0.0d+0
               ai(k,j) = 0.0d+0
               call waxpy(k,tr,ti,ar(1,k),ai(1,k),1,ar(1,j),ai(1,j),1)
   90       continue
  100       continue
  110    continue
c
c        form inverse(u)*inverse(l)
c
         nm1 = n - 1
         if (nm1 .lt. 1) go to 150
         do 140 kb = 1, nm1
            k = n - kb
            kp1 = k + 1
            do 120 i = kp1, n
               workr(i) = ar(i,k)
               worki(i) = ai(i,k)
               ar(i,k) = 0.0d+0
               ai(i,k) = 0.0d+0
  120       continue
            do 130 j = kp1, n
               tr = workr(j)
               ti = worki(j)
               call waxpy(n,tr,ti,ar(1,j),ai(1,j),1,ar(1,k),ai(1,k),1)
  130       continue
            l = ipvt(k)
            if (l .ne. k)
     *         call wswap(n,ar(1,k),ai(1,k),1,ar(1,l),ai(1,l),1)
  140    continue
  150    continue
  160 continue
      return
      end
