      subroutine wqrsl(xr,xi,ldx,n,k,qrauxr,qrauxi,yr,yi,qyr,qyi,qtyr,
     *     qtyi,br,bi,rsdr,rsdi,xbr,xbi,job,info)
      integer ldx,n,k,job,info
      double precision xr(ldx,*),xi(ldx,*),qrauxr(*),qrauxi(*),yr(*),
     *                 yi(*),qyr(*),qyi(*),qtyr(*),qtyi(*),br(*),bi(*),
     *                 rsdr(*),rsdi(*),xbr(*),xbi(*)
c!purpose
c
c     wqrsl applies the output of wqrdc to compute coordinate
c     transformations, projections, and least squares solutions.
c     for k .le. min(n,p), let xk be the matrix
c
c            xk = (x(jpvt(1)),x(jpvt(2)), ... ,x(jpvt(k)))
c
c     formed from columnns jpvt(1), ... ,jpvt(k) of the original
c     n x p matrix x that was input to wqrdc (if no pivoting was
c     done, xk consists of the first k columns of x in their
c     original order).  wqrdc produces a factored unitary matrix q
c     and an upper triangular matrix r such that
c
c              xk = q * (r)
c                       (0)
c
c     this information is contained in coded form in the arrays
c     x and qraux.
c
c!calling sequence
c
c      subroutine wqrsl(xr,xi,ldx,n,k,qrauxr,qrauxi,yr,yi,qyr,qyi,qtyr,
c     on entry
c
c        x      double-complex(ldx,p).
c               x contains the output of wqrdc.
c
c        ldx    integer.
c               ldx is the leading dimension of the array x.
c
c        n      integer.
c               n is the number of rows of the matrix xk.  it must
c               have the same value as n in wqrdc.
c
c        k      integer.
c               k is the number of columns of the matrix xk.  k
c               must nnot be greater than min(n,p), where p is the
c               same as in the calling sequence to wqrdc.
c
c        qraux  double-complex(p).
c               qraux contains the auxiliary output from wqrdc.
c
c        y      double-complex(n)
c               y contains an n-vector that is to be manipulated
c               by wqrsl.
c
c        job    integer.
c               job specifies what is to be computed.  job has
c               the decimal expansion abcde, with the following
c               meaning.
c
c                    if a.ne.0, compute qy.
c                    if b,c,d, or e .ne. 0, compute qty.
c                    if c.ne.0, compute b.
c                    if d.ne.0, compute rsd.
c                    if e.ne.0, compute xb.
c
c               note that a request to compute b, rsd, or xb
c               automatically triggers the computation of qty, for
c               which an array must be provided in the calling
c               sequence.
c
c     on return
c
c        qy     double-complex(n).
c               qy conntains q*y, if its computation has been
c               requested.
c
c        qty    double-complex(n).
c               qty contains ctrans(q)*y, if its computation has
c               been requested.  here ctrans(q) is the conjugate
c               transpose of the matrix q.
c
c        b      double-complex(k)
c               b contains the solution of the least squares problem
c
c                    minimize norm2(y - xk*b),
c
c               if its computation has been requested.  (note that
c               if pivoting was requested in wqrdc, the j-th
c               component of b will be associated with column jpvt(j)
c               of the original matrix x that was input into wqrdc.)
c
c        rsd    double-complex(n).
c               rsd contains the least squares residual y - xk*b,
c               if its computation has been requested.  rsd is
c               also the orthogonal projection of y onto the
c               orthogonal complement of the column space of xk.
c
c        xb     double-complex(n).
c               xb contains the least squares approximation xk*b,
c               if its computation has been requested.  xb is also
c               the orthogonal projection of y onto the column space
c               of x.
c
c        info   integer.
c               info is zero unless the computation of b has
c               been requested and r is exactly singular.  in
c               this case, info is the index of the first zero
c               diagonal element of r and b is left unaltered.
c
c     the parameters qy, qty, b, rsd, and xb are not referenced
c     if their computation is not requested and in this case
c     can be replaced by dummy variables in the calling program.
c     to save storage, the user may in some cases use the same
c     array for different parameters in the calling sequence.  a
c     frequently occuring example is when one wishes to compute
c     any of b, rsd, or xb and does not need y or qty.  in this
c     case one may identify y, qty, and one of b, rsd, or xb, while
c     providing separate arrays for anything else that is to be
c     computed.  thus the calling sequence
c
c          call wqrsl(x,ldx,n,k,qraux,y,dum,y,b,y,dum,110,info)
c
c     will result in the computation of b and rsd, with rsd
c     overwriting y.  more generally, each item in the following
c     list contains groups of permissible identifications for
c     a single callinng sequence.
c
c          1. (y,qty,b) (rsd) (xb) (qy)
c
c          2. (y,qty,rsd) (b) (xb) (qy)
c
c          3. (y,qty,xb) (b) (rsd) (qy)
c
c          4. (y,qy) (qty,b) (rsd) (xb)
c
c          5. (y,qy) (qty,rsd) (b) (xb)
c
c          6. (y,qy) (qty,xb) (b) (rsd)
c
c     in any group the value returned in the array allocated to
c     the group corresponds to the last member of the group.
c
c!originator
c     linpack. this version dated 07/03/79 .
c     g.w. stewart, university of maryland, argonne national lab.
c
c!auxiliary routines
c
c     blas waxpy,wcopy,wdotcr,wdotci
c     fortran abs,dimag,min,mod
c
c     Copyright INRIA

c!
c     internal variables
c
      integer i,j,jj,ju,kp1
      double precision wdotcr,wdotci,tr,ti,tempr,tempi
      logical cb,cqy,cqty,cr,cxb
c
      double precision zdumr,zdumi
      double precision cabs1
      cabs1(zdumr,zdumi) = abs(zdumr) + abs(zdumi)
c
c     set info flag.
c
      info = 0
c
c     determine what is to be computed.
c
      cqy = job/10000 .ne. 0
      cqty = mod(job,10000) .ne. 0
      cb = mod(job,1000)/100 .ne. 0
      cr = mod(job,100)/10 .ne. 0
      cxb = mod(job,10) .ne. 0
      ju = min(k,n-1)
c
c     special action when n=1.
c
      if (ju .ne. 0) go to 80
         if (.not.cqy) go to 10
            qyr(1) = yr(1)
            qyi(1) = yi(1)
   10    continue
         if (.not.cqty) go to 20
            qtyr(1) = yr(1)
            qtyi(1) = yi(1)
   20    continue
         if (.not.cxb) go to 30
            xbr(1) = yr(1)
            xbi(1) = yi(1)
   30    continue
         if (.not.cb) go to 60
            if (cabs1(xr(1,1),xi(1,1)) .ne. 0.0d+0) go to 40
               info = 1
            go to 50
   40       continue
               call wdiv(yr(1),yi(1),xr(1,1),xi(1,1),br(1),bi(1))
   50       continue
   60    continue
         if (.not.cr) go to 70
            rsdr(1) = 0.0d+0
            rsdi(1) = 0.0d+0
   70    continue
      go to 290
   80 continue
c
c        set up to compute qy or qty.
c
         if (cqy) call wcopy(n,yr,yi,1,qyr,qyi,1)
         if (cqty) call wcopy(n,yr,yi,1,qtyr,qtyi,1)
         if (.not.cqy) go to 110
c
c           compute qy.
c
            do 100 jj = 1, ju
               j = ju - jj + 1
               if (cabs1(qrauxr(j),qrauxi(j)) .eq. 0.0d+0)
     *            go to 90
                  tempr = xr(j,j)
                  tempi = xi(j,j)
                  xr(j,j) = qrauxr(j)
                  xi(j,j) = qrauxi(j)
                  tr = -wdotcr(n-j+1,xr(j,j),xi(j,j),1,qyr(j),qyi(j),1)
                  ti = -wdotci(n-j+1,xr(j,j),xi(j,j),1,qyr(j),qyi(j),1)
                  call wdiv(tr,ti,xr(j,j),xi(j,j),tr,ti)
                  call waxpy(n-j+1,tr,ti,xr(j,j),xi(j,j),1,qyr(j),
     *                       qyi(j),1)
                  xr(j,j) = tempr
                  xi(j,j) = tempi
   90          continue
  100       continue
  110    continue
         if (.not.cqty) go to 140
c
c           compute ctrans(q)*y.
c
            do 130 j = 1, ju
               if (cabs1(qrauxr(j),qrauxi(j)) .eq. 0.0d+0)
     *            go to 120
                  tempr = xr(j,j)
                  tempi = xi(j,j)
                  xr(j,j) = qrauxr(j)
                  xi(j,j) = qrauxi(j)
                  tr = -wdotcr(n-j+1,xr(j,j),xi(j,j),1,qtyr(j),
     *                         qtyi(j),1)
                  ti = -wdotci(n-j+1,xr(j,j),xi(j,j),1,qtyr(j),
     *                         qtyi(j),1)
                  call wdiv(tr,ti,xr(j,j),xi(j,j),tr,ti)
                  call waxpy(n-j+1,tr,ti,xr(j,j),xi(j,j),1,qtyr(j),
     *                       qtyi(j),1)
                  xr(j,j) = tempr
                  xi(j,j) = tempi
  120          continue
  130       continue
  140    continue
c
c        set up to compute b, rsd, or xb.
c
         if (cb) call wcopy(k,qtyr,qtyi,1,br,bi,1)
         kp1 = k + 1
         if (cxb) call wcopy(k,qtyr,qtyi,1,xbr,xbi,1)
         if (cr .and. k .lt. n)
     *      call wcopy(n-k,qtyr(kp1),qtyi(kp1),1,rsdr(kp1),rsdi(kp1),1)
         if (.not.cxb .or. kp1 .gt. n) go to 160
            do 150 i = kp1, n
               xbr(i) = 0.0d+0
               xbi(i) = 0.0d+0
  150       continue
  160    continue
         if (.not.cr) go to 180
            do 170 i = 1, k
               rsdr(i) = 0.0d+0
               rsdi(i) = 0.0d+0
  170       continue
  180    continue
         if (.not.cb) go to 230
c
c           compute b.
c
            do 210 jj = 1, k
               j = k - jj + 1
               if (cabs1(xr(j,j),xi(j,j)) .ne. 0.0d+0) go to 190
                  info = j
c                 ......exit
c           ......exit
                  go to 220
  190          continue
               call wdiv(br(j),bi(j),xr(j,j),xi(j,j),br(j),bi(j))
               if (j .eq. 1) go to 200
                  tr = -br(j)
                  ti = -bi(j)
                  call waxpy(j-1,tr,ti,xr(1,j),xi(1,j),1,br,bi,1)
  200          continue
  210       continue
  220       continue
  230    continue
         if (.not.cr .and. .not.cxb) go to 280
c
c           compute rsd or xb as required.
c
            do 270 jj = 1, ju
               j = ju - jj + 1
               if (cabs1(qrauxr(j),qrauxi(j)) .eq. 0.0d+0)
     *            go to 260
                  tempr = xr(j,j)
                  tempi = xi(j,j)
                  xr(j,j) = qrauxr(j)
                  xi(j,j) = qrauxi(j)
                  if (.not.cr) go to 240
                     tr = -wdotcr(n-j+1,xr(j,j),xi(j,j),1,rsdr(j),
     *                            rsdi(j),1)
                     ti = -wdotci(n-j+1,xr(j,j),xi(j,j),1,rsdr(j),
     *                            rsdi(j),1)
                     call wdiv(tr,ti,xr(j,j),xi(j,j),tr,ti)
                     call waxpy(n-j+1,tr,ti,xr(j,j),xi(j,j),1,rsdr(j),
     *                          rsdi(j),1)
  240             continue
                  if (.not.cxb) go to 250
                     tr = -wdotcr(n-j+1,xr(j,j),xi(j,j),1,xbr(j),
     *                            xbi(j),1)
                     ti = -wdotci(n-j+1,xr(j,j),xi(j,j),1,xbr(j),
     *                            xbi(j),1)
                     call wdiv(tr,ti,xr(j,j),xi(j,j),tr,ti)
                     call waxpy(n-j+1,tr,ti,xr(j,j),xi(j,j),1,xbr(j),
     *                          xbi(j),1)
  250             continue
                  xr(j,j) = tempr
                  xi(j,j) = tempi
  260          continue
  270       continue
  280    continue
  290 continue
      return
      end
