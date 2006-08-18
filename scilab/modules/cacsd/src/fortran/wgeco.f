      subroutine wgeco(ar,ai,lda,n,ipvt,rcond,zr,zi)
c     Copyright INRIA
      integer lda,n,ipvt(*)
      double precision ar(lda,*),ai(lda,*),zr(*),zi(*)
      double precision rcond
c!purpose
c
c     wgeco factors a double-complex matrix by gaussian elimination
c     and estimates the condition of the matrix.
c
c     if  rcond  is not needed, wgefa is slightly faster.
c     to solve  a*x = b , follow wgeco by wgesl.
c     to compute  inverse(a)*c , follow wgeco by wgesl.
c     to compute  determinant(a) , follow wgeco by wgedi.
c     to compute  inverse(a) , follow wgeco by wgedi.
c
c!calling sequence
c
c      subroutine wgeco(ar,ai,lda,n,ipvt,rcond,zr,zi)
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
c        rcond   double precision
c                an estimate of the reciprocal condition of  a .
c                for the system  a*x = b , relative perturbations
c                in  a  and  b  of size  epsilon  may cause
c                relative perturbations in  x  of size  epsilon/rcond .
c                if  rcond  is so small that the logical expression
c                           1.0 + rcond .eq. 1.0
c                is true, then  a  may be singular to working
c                precision.  in particular,  rcond  is zero  if
c                exact singularity is detected or the estimate
c                underflows.
c
c        z       double-complex(n)
c                a work vector whose contents are usually unimportant.
c                if  a  is close to a singular matrix, then  z  is
c                an approximate null vector in the sense that
c                norm(a*z) = rcond*norm(a)*norm(z) .
c
c!originator
c     linpack. this version dated 07/01/79 .
c     cleve moler, university of new mexico, argonne national lab.
c
c!auxiliary routines
c
c     linpack wgefa
c     blas waxpy,wdotc,wasum
c     fortran abs,max
c
c!
c     internal variables
c
      double precision wdotcr,wdotci,ekr,eki,tr,ti,wkr,wki,wkmr,wkmi
      double precision anorm,s,wasum,sm,ynorm
      integer info,j,k,kb,kp1,l
c
      double precision zdumr,zdumi
      double precision cabs1
      cabs1(zdumr,zdumi) = abs(zdumr) + abs(zdumi)
c
c     compute 1-norm of a
c
      anorm = 0.0d+0
      do 10 j = 1, n
         anorm = max(anorm,wasum(n,ar(1,j),ai(1,j),1))
   10 continue
c
c     factor
c
      call wgefa(ar,ai,lda,n,ipvt,info)
c
c     rcond = 1/(norm(a)*(estimate of norm(inverse(a)))) .
c     estimate = norm(z)/norm(y) where  a*z = y  and  ctrans(a)*y = e .
c     ctrans(a)  is the conjugate transpose of a .
c     the components of  e  are chosen to cause maximum local
c     growth in the elements of w  where  ctrans(u)*w = e .
c     the vectors are frequently rescaled to avoid overflow.
c
c     solve ctrans(u)*w = e
c
      ekr = 1.0d+0
      eki = 0.0d+0
      do 20 j = 1, n
         zr(j) = 0.0d+0
         zi(j) = 0.0d+0
   20 continue
      do 110 k = 1, n
         call wsign(ekr,eki,-zr(k),-zi(k),ekr,eki)
         if (cabs1(ekr-zr(k),eki-zi(k))
     *       .le. cabs1(ar(k,k),ai(k,k))) go to 40
            s = cabs1(ar(k,k),ai(k,k))
     *          /cabs1(ekr-zr(k),eki-zi(k))
            call wrscal(n,s,zr,zi,1)
            ekr = s*ekr
            eki = s*eki
   40    continue
         wkr = ekr - zr(k)
         wki = eki - zi(k)
         wkmr = -ekr - zr(k)
         wkmi = -eki - zi(k)
         s = cabs1(wkr,wki)
         sm = cabs1(wkmr,wkmi)
         if (cabs1(ar(k,k),ai(k,k)) .eq. 0.0d+0) go to 50
            call wdiv(wkr,wki,ar(k,k),-ai(k,k),wkr,wki)
            call wdiv(wkmr,wkmi,ar(k,k),-ai(k,k),wkmr,wkmi)
         go to 60
   50    continue
            wkr = 1.0d+0
            wki = 0.0d+0
            wkmr = 1.0d+0
            wkmi = 0.0d+0
   60    continue
         kp1 = k + 1
         if (kp1 .gt. n) go to 100
            do 70 j = kp1, n
               call wmul(wkmr,wkmi,ar(k,j),-ai(k,j),tr,ti)
               sm = sm + cabs1(zr(j)+tr,zi(j)+ti)
               call waxpy(1,wkr,wki,ar(k,j),-ai(k,j),1,
     $                    zr(j),zi(j),1)
               s = s + cabs1(zr(j),zi(j))
   70       continue
            if (s .ge. sm) go to 90
               tr = wkmr - wkr
               ti = wkmi - wki
               wkr = wkmr
               wki = wkmi
               do 80 j = kp1, n
                  call waxpy(1,tr,ti,ar(k,j),-ai(k,j),1,
     $                       zr(j),zi(j),1)
   80          continue
   90       continue
  100    continue
         zr(k) = wkr
         zi(k) = wki
  110 continue
      s = 1.0d+0/wasum(n,zr,zi,1)
      call wrscal(n,s,zr,zi,1)
c
c     solve ctrans(l)*y = w
c
      do 140 kb = 1, n
         k = n + 1 - kb
         if (k .ge. n) go to 120
            zr(k) = zr(k)
     *            + wdotcr(n-k,ar(k+1,k),ai(k+1,k),1,zr(k+1),zi(k+1),1)
            zi(k) = zi(k)
     *            + wdotci(n-k,ar(k+1,k),ai(k+1,k),1,zr(k+1),zi(k+1),1)
  120    continue
         if (cabs1(zr(k),zi(k)) .le. 1.0d+0) go to 130
            s = 1.0d+0/cabs1(zr(k),zi(k))
            call wrscal(n,s,zr,zi,1)
  130    continue
         l = ipvt(k)
         tr = zr(l)
         ti = zi(l)
         zr(l) = zr(k)
         zi(l) = zi(k)
         zr(k) = tr
         zi(k) = ti
  140 continue
      s = 1.0d+0/wasum(n,zr,zi,1)
      call wrscal(n,s,zr,zi,1)
c
      ynorm = 1.0d+0
c
c     solve l*v = y
c
      do 160 k = 1, n
         l = ipvt(k)
         tr = zr(l)
         ti = zi(l)
         zr(l) = zr(k)
         zi(l) = zi(k)
         zr(k) = tr
         zi(k) = ti
         if (k .lt. n)
     *      call waxpy(n-k,tr,ti,ar(k+1,k),ai(k+1,k),1,zr(k+1),zi(k+1),
     *                 1)
         if (cabs1(zr(k),zi(k)) .le. 1.0d+0) go to 150
            s = 1.0d+0/cabs1(zr(k),zi(k))
            call wrscal(n,s,zr,zi,1)
            ynorm = s*ynorm
  150    continue
  160 continue
      s = 1.0d+0/wasum(n,zr,zi,1)
      call wrscal(n,s,zr,zi,1)
      ynorm = s*ynorm
c
c     solve  u*z = v
c
      do 200 kb = 1, n
         k = n + 1 - kb
         if (cabs1(zr(k),zi(k))
     *       .le. cabs1(ar(k,k),ai(k,k))) go to 170
            s = cabs1(ar(k,k),ai(k,k))
     *          /cabs1(zr(k),zi(k))
            call wrscal(n,s,zr,zi,1)
            ynorm = s*ynorm
  170    continue
         if (cabs1(ar(k,k),ai(k,k)) .eq. 0.0d+0) go to 180
            call wdiv(zr(k),zi(k),ar(k,k),ai(k,k),zr(k),zi(k))
  180    continue
         if (cabs1(ar(k,k),ai(k,k)) .ne. 0.0d+0) go to 190
            zr(k) = 1.0d+0
            zi(k) = 0.0d+0
  190    continue
         tr = -zr(k)
         ti = -zi(k)
         call waxpy(k-1,tr,ti,ar(1,k),ai(1,k),1,zr(1),zi(1),1)
  200 continue
c     make znorm = 1.0
      s = 1.0d+0/wasum(n,zr,zi,1)
      call wrscal(n,s,zr,zi,1)
      ynorm = s*ynorm
c
      if (anorm .ne. 0.0d+0) rcond = ynorm/anorm
      if (anorm .eq. 0.0d+0) rcond = 0.0d+0
      return
      end
