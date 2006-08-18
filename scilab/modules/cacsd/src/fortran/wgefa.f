      subroutine wgefa(ar,ai,lda,n,ipvt,info)
c     Copyright INRIA
      integer lda,n,ipvt(*),info
      double precision ar(lda,*),ai(lda,*)
c!purpose
c
c     wgefa factors a double-complex matrix by gaussian elimination.
c
c     wgefa is usually called by wgeco, but it can be called
c     directly with a saving in time if  rcond  is not needed.
c     (time for wgeco) = (1 + 9/n)*(time for wgefa) .
c
c!calling sequence
c
c      subroutine wgefa(ar,ai,lda,n,ipvt,info)
c     on entry
c
c        a       double-complex(lda, n)
c                the matrix to be factored.
c
c        lda     integer
c                the leading dimension of the array  a .
c
c        n       integer
c                the order of the matrix  a .
c
c     on return
c
c        a       an upper triangular matrix and the multipliers
c                which were used to obtain it.
c                the factorization can be written  a = l*u  where
c                l  is a product of permutation and unit lower
c                triangular matrices and  u  is upper triangular.
c
c        ipvt    integer(n)
c                an integer vector of pivot indices.
c
c        info    integer
c                = 0  normal value.
c                = k  if  u(k,k) .eq. 0.0 .  this is not an error
c                     condition for this subroutine, but it does
c                     indicate that wgesl or wgedi will divide by zero
c                     if called.  use  rcond  in wgeco for a reliable
c                     indication of singularity.
c
c!originator
c     linpack. this version dated 07/01/79 .
c     cleve moler, university of new mexico, argonne national lab.
c
c!auxiliary routines
c
c     blas waxpy,wscal,iwamax
c     fortran abs
c
c!
c     internal variables
c
      double precision tr,ti
      integer iwamax,j,k,kp1,l,nm1
c
      double precision zdumr,zdumi
      double precision cabs1
      cabs1(zdumr,zdumi) = abs(zdumr) + abs(zdumi)
c
c     gaussian elimination with partial pivoting
c
      info = 0
      nm1 = n - 1
      if (nm1 .lt. 1) go to 70
      do 60 k = 1, nm1
         kp1 = k + 1
c
c        find l = pivot index
c
         l = iwamax(n-k+1,ar(k,k),ai(k,k),1) + k - 1
         ipvt(k) = l
c
c        zero pivot implies this column already triangularized
c
         if (cabs1(ar(l,k),ai(l,k)) .eq. 0.0d+0) go to 40
c
c           interchange if necessary
c
            if (l .eq. k) go to 10
               tr = ar(l,k)
               ti = ai(l,k)
               ar(l,k) = ar(k,k)
               ai(l,k) = ai(k,k)
               ar(k,k) = tr
               ai(k,k) = ti
   10       continue
c
c           compute multipliers
c
            call wdiv(-1.0d+0,0.0d+0,ar(k,k),ai(k,k),tr,ti)
            call wscal(n-k,tr,ti,ar(k+1,k),ai(k+1,k),1)
c
c           row elimination with column indexing
c
            do 30 j = kp1, n
               tr = ar(l,j)
               ti = ai(l,j)
               if (l .eq. k) go to 20
                  ar(l,j) = ar(k,j)
                  ai(l,j) = ai(k,j)
                  ar(k,j) = tr
                  ai(k,j) = ti
   20          continue
               call waxpy(n-k,tr,ti,ar(k+1,k),ai(k+1,k),1,ar(k+1,j),
     *                    ai(k+1,j),1)
   30       continue
         go to 50
   40    continue
            info = k
   50    continue
   60 continue
   70 continue
      ipvt(n) = n
      if (cabs1(ar(n,n),ai(n,n)) .eq. 0.0d+0) info = n
      return
      end
