C/MEMBR ADD NAME=WQRDC,SSI=0
c     Copyright INRIA
      subroutine wqrdc(xr,xi,ldx,n,p,qrauxr,qrauxi,jpvt,workr,worki,
     *                 job)
      integer ldx,n,p,job
      integer jpvt(*)
      double precision xr(ldx,*),xi(ldx,*),qrauxr(*),qrauxi(*),
     *                 workr(*),worki(*)
c!purpose
c
c     wqrdc uses householder transformations to compute the qr
c     factorization of an n by p matrix x.  column pivoting
c     based on the 2-norms of the reduced columns may be
c     performed at the users option.
c
c!calling sequence
c
c      subroutine wqrdc(xr,xi,ldx,n,p,qrauxr,qrauxi,jpvt,workr,worki,
c     on entry
c
c        x       double-complex(ldx,p), where ldx .ge. n.
c                x contains the matrix whose decomposition is to be
c                computed.
c
c        ldx     integer.
c                ldx is the leading dimension of the array x.
c
c        n       integer.
c                n is the number of rows of the matrix x.
c
c        p       integer.
c                p is the number of columns of the matrix x.
c
c        jpvt    integer(p).
c                jpvt contains integers that control the selection
c                of the pivot columns.  the k-th column x(k) of x
c                is placed in one of three classes according to the
c                value of jpvt(k).
c
c                   if jpvt(k) .gt. 0, then x(k) is an initial
c                                      column.
c
c                   if jpvt(k) .eq. 0, then x(k) is a free column.
c
c                   if jpvt(k) .lt. 0, then x(k) is a final column.
c
c                before the decomposition is computed, initial columns
c                are moved to the beginning of the array x and final
c                columns to the end.  both initial and final columns
c                are frozen in place during the computation and only
c                free columns are moved.  at the k-th stage of the
c                reduction, if x(k) is occupied by a free column
c                it is interchanged with the free column of largest
c                reduced norm.  jpvt is not referenced if
c                job .eq. 0.
c
c        work    double-complex(p).
c                work is a work array.  work is not referenced if
c                job .eq. 0.
c
c        job     integer.
c                job is an integer that initiates column pivoting.
c                if job .eq. 0, no pivoting is done.
c                if job .ne. 0, pivoting is done.
c
c     on return
c
c        x       x contains in its upper triangle the upper
c                triangular matrix r of the qr factorization.
c                below its diagonal x contains information from
c                which the unitary part of the decomposition
c                can be recovered.  note that if pivoting has
c                been requested, the decomposition is not that
c                of the original matrix x but that of x
c                with its columns permuted as described by jpvt.
c
c        qraux   double-complex(p).
c                qraux contains further information required to recover
c                the unitary part of the decomposition.
c
c        jpvt    jpvt(k) contains the index of the column of the
c                original matrix that has been interchanged into
c                the k-th column, if pivoting was requested.
c
c!originator
c     linpack. this version dated 07/03/79 .
c     g.w. stewart, university of maryland, argonne national lab.
c
c!auxiliary routines
c
c     blas waxpy,pythag,wdotcr,wdotci,wscal,wswap,wnrm2
c     fortran abs,dimag,max,min
c
c!
c     internal variables
c
      integer j,jp,l,lp1,lup,maxj,pl,pu
      double precision maxnrm,wnrm2,tt
      double precision pythag,wdotcr,wdotci,nrmxlr,nrmxli,tr,ti
      logical negj,swapj
c
      double precision zdumr,zdumi
      double precision cabs1
      cabs1(zdumr,zdumi) = abs(zdumr) + abs(zdumi)
c
      pl = 1
      pu = 0
      if (job .eq. 0) go to 60
c
c        pivoting has been requested.  rearrange the columns
c        according to jpvt.
c
         do 20 j = 1, p
            swapj = jpvt(j) .gt. 0
            negj = jpvt(j) .lt. 0
            jpvt(j) = j
            if (negj) jpvt(j) = -j
            if (.not.swapj) go to 10
               if (j .ne. pl)
     *            call wswap(n,xr(1,pl),xi(1,pl),1,xr(1,j),xi(1,j),1)
               jpvt(j) = jpvt(pl)
               jpvt(pl) = j
               pl = pl + 1
   10       continue
   20    continue
         pu = p
         do 50 jj = 1, p
            j = p - jj + 1
            if (jpvt(j) .ge. 0) go to 40
               jpvt(j) = -jpvt(j)
               if (j .eq. pu) go to 30
                  call wswap(n,xr(1,pu),xi(1,pu),1,xr(1,j),xi(1,j),1)
                  jp = jpvt(pu)
                  jpvt(pu) = jpvt(j)
                  jpvt(j) = jp
   30          continue
               pu = pu - 1
   40       continue
   50    continue
   60 continue
c
c     compute the norms of the free columns.
c
      if (pu .lt. pl) go to 80
      do 70 j = pl, pu
         qrauxr(j) = wnrm2(n,xr(1,j),xi(1,j),1)
         qrauxi(j) = 0.0d+0
         workr(j) = qrauxr(j)
         worki(j) = qrauxi(j)
   70 continue
   80 continue
c
c     perform the householder reduction of x.
c
      lup = min(n,p)
      do 210 l = 1, lup
         if (l .lt. pl .or. l .ge. pu) go to 120
c
c           locate the column of largest norm and bring it
c           into the pivot position.
c
            maxnrm = 0.0d+0
            maxj = l
            do 100 j = l, pu
               if (qrauxr(j) .le. maxnrm) go to 90
                  maxnrm = qrauxr(j)
                  maxj = j
   90          continue
  100       continue
            if (maxj .eq. l) go to 110
               call wswap(n,xr(1,l),xi(1,l),1,xr(1,maxj),xi(1,maxj),1)
               qrauxr(maxj) = qrauxr(l)
               qrauxi(maxj) = qrauxi(l)
               workr(maxj) = workr(l)
               worki(maxj) = worki(l)
               jp = jpvt(maxj)
               jpvt(maxj) = jpvt(l)
               jpvt(l) = jp
  110       continue
  120    continue
         qrauxr(l) = 0.0d+0
         qrauxi(l) = 0.0d+0
         if (l .eq. n) go to 200
c
c           compute the householder transformation for column l.
c
            nrmxlr = wnrm2(n-l+1,xr(l,l),xi(l,l),1)
            nrmxli = 0.0d+0
            if (cabs1(nrmxlr,nrmxli) .eq. 0.0d+0) go to 190
               if (cabs1(xr(l,l),xi(l,l)) .eq. 0.0d+0) go to 130
                 call wsign(nrmxlr,nrmxli,xr(l,l),xi(l,l),nrmxlr,nrmxli)
  130          continue
               call wdiv(1.0d+0,0.0d+0,nrmxlr,nrmxli,tr,ti)
               call wscal(n-l+1,tr,ti,xr(l,l),xi(l,l),1)
               xr(l,l) = 1.0d+0 + xr(l,l)
c
c              apply the transformation to the remaining columns,
c              updating the norms.
c
               lp1 = l + 1
               if (p .lt. lp1) go to 180
               do 170 j = lp1, p
                  tr = -wdotcr(n-l+1,xr(l,l),xi(l,l),1,xr(l,j),
     *                         xi(l,j),1)
                  ti = -wdotci(n-l+1,xr(l,l),xi(l,l),1,xr(l,j),
     *                         xi(l,j),1)
                  call wdiv(tr,ti,xr(l,l),xi(l,l),tr,ti)
                  call waxpy(n-l+1,tr,ti,xr(l,l),xi(l,l),1,xr(l,j),
     *                       xi(l,j),1)
                  if (j .lt. pl .or. j .gt. pu) go to 160
                  if (cabs1(qrauxr(j),qrauxi(j)) .eq. 0.0d+0)
     *               go to 160
                     tt = 1.0d+0-(pythag(xr(l,j),xi(l,j))/qrauxr(j))**2
                     tt = max(tt,0.0d+0)
                     tr = tt
                     tt = 1.0d+0+0.050d+0*tt*(qrauxr(j)/workr(j))**2
                     if (tt .eq. 1.0d+0) go to 140
                        qrauxr(j) = qrauxr(j)*sqrt(tr)
                        qrauxi(j) = qrauxi(j)*sqrt(tr)
                     go to 150
  140                continue
                        qrauxr(j) = wnrm2(n-l,xr(l+1,j),xi(l+1,j),1)
                        qrauxi(j) = 0.0d+0
                        workr(j) = qrauxr(j)
                        worki(j) = qrauxi(j)
  150                continue
  160             continue
  170          continue
  180          continue
c
c              save the transformation.
c
               qrauxr(l) = xr(l,l)
               qrauxi(l) = xi(l,l)
               xr(l,l) = -nrmxlr
               xi(l,l) = -nrmxli
  190       continue
  200    continue
  210 continue
      return
      end
