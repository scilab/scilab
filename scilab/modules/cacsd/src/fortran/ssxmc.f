      subroutine ssxmc(n,m,a,na,b,ncont,indcon,nblk,z,
     1                 wrka,wrk1,wrk2,iwrk,tol,mode)
c! calling sequence
c        subroutine ssxmc(n,m,a,na,b,ncont,indcon,nblk,z,
c    1                    wrka,wrk1,wrk2,iwrk,tol,mode)
c
c        integer n,m,na,ncont,indcon,nblk(n),iwrk(m),mode
c
c        real*8 a(na,n),b(na,m),z(na,n),wrka(n,m)
c     real*8 wrk1(m),wrk2(m),tol
c
c arguments in
c
c       n        integer
c                -the order of original state-space representation;
c                declared first dimension of nblk,wrka; declared
c                second dimension of a (and z, if mode .ne. 0)
c
c       m        integer
c                -the number of system inputs; declared first dimension
c                of iwrk,wrk1,wrk2; declared second dimension of b,wrka
c
c       a        double precision(n,n)
c                -the original state dynamics matrix.  note that this
c                matrix is overwritten here
c
c       na       integer
c                -the declared first dimension of a,b (and z, if
c                mode .ne. 0).  note that na .ge. n
c
c       b        double precision(n,m)
c                -the original input/state matrix.  note that this
c                matrix is overwritten here
c
c       tol      double precision
c                -if greater than the machine precision, tol is used
c                as zero tolerance in rank determination when trans-
c                forming (a,b,c): otherwise (eg tol = 0.0d+0), the
c                machine precision is used
c
c       mode     integer
c                -mode = 0 if accumulation of the orthogonal trans-
c                formation z is not required, and non-zero if this
c                matrix is required
c
c arguments out
c
c       a        double precision(ncont,ncont)
c                -the upper block hessenberg state dynamics matrix of
c                a controllable realization for the original system
c
c       b        double precision(ncont,m)
c                -the transformed input/state matrix
c
c       ncont    integer
c                -the order of controllable state-space representation
c
c       indcon   integer
c                -the controllability index of transformed
c                system representation
c
c       nblk     integer(indcon)
c                -the dimensions of the diagonal blocks of the trans-
c                formed a
c
c       z        double precision(n,n)
c                -the orthogonal similarity transformation which
c                reduces the given system to orthogonal canonical
c                form.  note that, if mode .eq. 0, z is not referenced
c                and so can be a scalar dummy variable
c
c!working space
c
c       wrka     double precision(n,m)
c
c       wrk1     double precision(m)
c
c       wrk2     double precision(m)
c
c       iwrk     integer(m)
c
c!purpose
c
c        to reduce the linear time-invariant multi-input system
c
c             dx/dt = a * x + b * u,
c
c        where a and b are (n x n) and (n x m) matrices respectively,
c        to orthogonal canonical form using (and optionally accum-
c        ulating) orthogonal similarity transformations.
c
c!method
c
c        b is first qr-decomposed and the appropriate orthogonal
c        similarity transformation applied to a.  leaving the first
c        rank(b) states unchanged, the resulting lower left block
c        of a is now itself qr-decomposed and this new orthogonal
c        similarity transformation applied.  continuing in this
c        manner, a completely controllable state-space pair (acont,
c        bcont) is found for the given (a,b), where acont is upper
c        block hessenberg with each sub-diagonal block of full row
c        rank, and bcont is zero apart from its (independent) first
c        rank(b) rows.  note finally that the system controllability
c        indices are easily calculable from the dimensions of the
c        blocks of acont.
c
c!reference
c
c        konstantinov, m.m., petkov, p.hr. and christov, n.d.
c        "orthogonal invariants and canonical forms for linear
c        controllable systems"
c        proc. ifac 8th world congress, 1981.
c
c!auxiliary routines
c
c        dqrdc (linpack)
c
c!originator
c
c                p.hr.petkov, higher institute of mechanical and
c                electrical engineering, sofia, bulgaria, april 1981
C     Copyright SLICOT
c
c!comments
c
c                none
c
c!user-supplied routines
c
c                none
c!
c*******************************************************************
c
c
      integer nblk(n),iwrk(m)
c
      double precision a(na,n),b(na,m),z(na,n),tol
      double precision wrka(n,m),wrk1(m),wrk2(m)
c
c     local variables:
c
c
      double precision abnorm,temp,thrtol
c
c      common /smprec/eps
c
c     common block smprec is shared with routine ddata which provides
c     a value for eps, a machine-dependent parameter which specifies
c     the relative precision of drealing-point arithmetic
c
c
c      call ddata
c
      abnorm = 0.0d+0
      ist = 0
      ncont = 0
      indcon = 0
      ni = 0
      nb = n
      mb = m
c
c     use the larger of tol, eps in rank determination
c
c      toleps = dble(n * n) * max(tol,eps)
c
      if (mode .eq. 0) go to 30
c
c     initialize  z  to identity matrix
c
      do 20 i = 1, n
c
         do 10 j = 1, n
   10    z(i,j) = 0.0d+0
c
         z(i,i) = 1.0d+0
   20 continue
c
   30 do 50 i = 1, n
c
         do 40 j = 1, m
            wrka(i,j) = b(i,j)
            b(i,j) = 0.0d+0
   40    continue
c
   50 continue
c
   60 ist = ist + 1
c
c     qr decomposition with column pivoting
c
      do 70 j = 1, mb
   70 iwrk(j) = 0
c
      call dqrdc(wrka,n,nb,mb,wrk1,iwrk,wrk2,1)
c
      irnk = 0
      mm = min(nb,mb)
      if (abs(wrka(1,1)) .gt. abnorm) abnorm = abs(wrka(1,1))
c      thresh = toleps * abnorm
c
c     rank determination
c
      thrtol=tol*abnorm*dble(n*n)
      do 100 i = 1,mm
          temp=abs(wrka(i,i))
      if(temp.gt.thrtol.and.1.0d+0+temp.gt.1.0d+0) irnk = i
  100 continue
c
      if (irnk .eq. 0) go to 360
      nj = ni
      ni = ncont
      ncont = ncont + irnk
      indcon = indcon + 1
      nblk(indcon) = irnk
      lu = min(irnk,nb-1)
      if (lu .eq. 0) go to 200
c
c     premultiply appropriate row block of a by qtrans
c
      call hhdml(lu,n,n,ni,ni,nb,nb,wrka,n,wrk1,a,na,11,ierr)
c
c     postmultiply appropriate column block of a by q
c
      call hhdml(lu,n,n,0,ni,n,nb,wrka,n,wrk1,a,na,00,ierr)
c
c     if required, accumulate transformations
c
      if (mode .ne. 0) call hhdml(lu,n,n,0,ni,n,nb,wrka,n,wrk1,z,na,
     1                            00,ierr)
c
  200 if (irnk .lt. 2) go to 230
c
      do 220 i = 2, irnk
         im1 = i - 1
c
         do 210 j = 1, im1
  210    wrka(i,j) = 0.0d+0
c
  220 continue
c
c     backward permutation of the columns
c
  230 do 270 j = 1, mb
         if (iwrk(j) .lt. 0) go to 270
         k = iwrk(j)
         iwrk(j) = -k
  240    continue
            if (k .eq. j) go to 260
c
            do 250 i = 1, irnk
               temp = wrka(i,k)
               wrka(i,k) = wrka(i,j)
               wrka(i,j) = temp
  250       continue
c
            iwrk(k) = -iwrk(k)
            k = -iwrk(k)
         go to 240
  260    continue
  270 continue
c
      if (ist .gt. 1) go to 300
c
c     form  b
c
      do 290 i = 1, irnk
c
         do 280 j = 1, m
  280    b(i,j) = wrka(i,j)
c
  290 continue
c
      go to 330
c
c     form  a
c
  300 do 320 i = 1, irnk
         ia = ni + i
c
         do 310 j = 1, mb
            ja = nj + j
  310    a(ia,ja) = wrka(i,j)
c
  320 continue
c
  330 if (irnk .eq. nb) go to 360
c
      mb = irnk
      nb = nb - irnk
c
      do 350 i = 1, nb
         ia = ncont + i
c
         do 340 j = 1, mb
            ja = ni + j
            wrka(i,j) = a(ia,ja)
            a(ia,ja) = 0.0d+0
  340    continue
c
  350 continue
      go to 60
c
  360 continue
c
      return
      end
