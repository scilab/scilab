      subroutine wgesl(ar,ai,lda,n,ipvt,br,bi,job)

      integer lda,n,ipvt(*),job
      double precision ar(lda,*),ai(lda,*),br(*),bi(*)
c!purpose
c
c     wgesl solves the double-complex system
c     a * x = b  or  ctrans(a) * x = b
c     using the factors computed by wgeco or wgefa.
c
c!calling sequence
c
c      subroutine wgesl(ar,ai,lda,n,ipvt,br,bi,job)
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
c        b       double-complex(n)
c                the right hand side vector.
c
c        job     integer
c                = 0         to solve  a*x = b ,
c                = nonzero   to solve  ctrans(a)*x = b  where
c                            ctrans(a)  is the conjugate transpose.
c
c     on return
c
c        b       the solution vector  x .
c
c     error condition
c
c        a division by zero will occur if the input factor contains a
c        zero on the diagonal.  technically this indicates singularity
c        but it is often caused by improper arguments or improper
c        setting of lda .  it will not occur if the subroutines are
c        called correctly and if wgeco has set rcond .gt. 0.0
c        or wgefa has set info .eq. 0 .
c
c     to compute  inverse(a) * c  where  c  is a matrix
c     with  p  columns
c           call wgeco(a,lda,n,ipvt,rcond,z)
c           if (rcond is too small) go to ...
c           do 10 j = 1, p
c              call wgesl(a,lda,n,ipvt,c(1,j),0)
c        10 continue
c
c!originator
c     linpack. this version dated 07/01/79 .
c     cleve moler, university of new mexico, argonne national lab.
c
c!auxiliary routines
c
c     blas waxpy,wdotc
c
c!
c     internal variables
c
      double precision wdotcr,wdotci,tr,ti
      integer k,kb,l,nm1
c
      nm1 = n - 1
      if (job .ne. 0) go to 50
c
c        job = 0 , solve  a * x = b
c        first solve  l*y = b
c
         if (nm1 .lt. 1) go to 30
         do 20 k = 1, nm1
            l = ipvt(k)
            tr = br(l)
            ti = bi(l)
            if (l .eq. k) go to 10
               br(l) = br(k)
               bi(l) = bi(k)
               br(k) = tr
               bi(k) = ti
   10       continue
            call waxpy(n-k,tr,ti,ar(k+1,k),ai(k+1,k),1,br(k+1),bi(k+1),
     *                 1)
   20    continue
   30    continue
c
c        now solve  u*x = y
c
         do 40 kb = 1, n
            k = n + 1 - kb
            call wdiv(br(k),bi(k),ar(k,k),ai(k,k),br(k),bi(k))
            tr = -br(k)
            ti = -bi(k)
            call waxpy(k-1,tr,ti,ar(1,k),ai(1,k),1,br(1),bi(1),1)
   40    continue
      go to 100
   50 continue
c
c        job = nonzero, solve  ctrans(a) * x = b
c        first solve  ctrans(u)*y = b
c
         do 60 k = 1, n
            tr = br(k) - wdotcr(k-1,ar(1,k),ai(1,k),1,br(1),bi(1),1)
            ti = bi(k) - wdotci(k-1,ar(1,k),ai(1,k),1,br(1),bi(1),1)
            call wdiv(tr,ti,ar(k,k),-ai(k,k),br(k),bi(k))
   60    continue
c
c        now solve ctrans(l)*x = y
c
         if (nm1 .lt. 1) go to 90
         do 80 kb = 1, nm1
            k = n - kb
            br(k) = br(k)
     *            + wdotcr(n-k,ar(k+1,k),ai(k+1,k),1,br(k+1),bi(k+1),1)
            bi(k) = bi(k)
     *            + wdotci(n-k,ar(k+1,k),ai(k+1,k),1,br(k+1),bi(k+1),1)
            l = ipvt(k)
            if (l .eq. k) go to 70
               tr = br(l)
               ti = bi(l)
               br(l) = br(k)
               bi(l) = bi(k)
               br(k) = tr
               bi(k) = ti
   70       continue
   80    continue
   90    continue
  100 continue
      return
      end
