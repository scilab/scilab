      subroutine dqrsm(x,ldx,n,p,y,ldy,nc,b,ldb,k,jpvt,qraux,work)
      integer ldx,n,p,ldy,nc,ldb,k,jpvt(*)
      double precision x(ldx,*),y(ldy,*),b(ldb,*),qraux(*),work(*)
C
C!purpose
C     sqrsm is a subroutine to compute least squares solutions
C     to the system
C
C     (1)               x * b = y,
C
C     which may be either under-determined or over-determined.
C     the relative machine precision eps is used as a tolerance
C     to limit the columns of x used in computing the solution.
C     in effect, a set of columns with a condition number
C     approximately rounded by 1/eps is used, the other
C     components of b being set to zero
C     if n.eq.1 and nc.gt.1 the elements in the nc-th column of b
C     are set to one).
C!calling sequence
C
C     subroutine dqrsm(x,ldx,n,p,y,ldy,nc,b,ldb,k,jpvt,qraux,work)
C
C     on entry
C
C        x     real(ldx,p), where ldx.ge.n
C              x contains the nxp coefficient matrix of
C              the system (1), x is destroyed by sqrsm.
C
C        ldx   integer
C              ldx is the leading dimension of the array x.
C
C        n     integer
C              n is the number of rows of the matrix x.
C
C        p     integer
C              p is the number of columns of the matrix x.
C
C        y     real(ldy,nc)
C              y contains the right hand side of the system(1).
C
C        ldy   integer
C              ldy is the leading dimension of the array y.
C
C        nc    integer
C              nc is the number of columns of the matrix y.
C
C        jpvt  integer(p)
C              jpvt is an integer array used by sqrdc.
C
C        qraux real(p)
C              qraux is an array used by sqrdc and sqrsl
C
C        work  real(p)
C              work is an array used by sqrdc.
C
C     on return
C
C        x     x contains the output array from sqrdc.
C
C        b     real(ldb,nc)
C              b contains the solution matrix. components
C              corresponding io columns not used are set to zero
C              (if n.eq.1 and nc.gt.1 the elements in the nc-th
C              column of b are set to one).
C
C        ldb   integer
C
C        k     integer
C              k contains the number of columns used in the
C              solutions.
C
C        jpvt  contains the pivot information from sqrdc.
C
C        qraux contains the array output by sqrdc.
C
C     on return the arrays x, jpvt and qraux contain the
C     usual output from dqrdc, so that the qr decomposition
C     of x with pivoting is fully available to the user.
C     in particular, columns jpvt(1), jpvt(2),...,jpvt(k)
C     were used in the solution, and the condition number
C     associated with those columns is estimated by
C     abs(x(1,1)/x(k,k)).
C!auxiliary routines
C     dqrdc dqrsl (linpack)
C!originator
C     this subroutine is a modification of the example program sqrst,
C     given in the linpack users' guide:
C     dongarra j.j., j.r.bunch, c.b.moler and g.w.stewart.
C     linpack users' guide. siam, philadelphia, 1979.
C!
C     internal variables
C
      integer info,j,kk,l,m
      double precision t,tt(1)
C
C     initialize jpvt so that all columns are free.
C
      do 10 j = 1,p
        jpvt(j) = 0
 10   continue
C
C     reduce x.
C
      call dqrdc(x,ldx,n,p,qraux,jpvt,work,1)
C
C     determine which columns to use.
C
      k = 0
      m = min(n,p)
      do 20 kk = 1,m
        t = abs(x(1,1)) + abs(x(kk,kk))
        if (t .eq. abs(x(1,1))) goto 30
        k = kk
 20   continue
 30   continue
C
C     solve the least squares problem.
C
      if (k .eq. 0) goto 160
      if (n.ge.p .or. n.gt.1 .or. nc.eq.1) goto 60
      np1 = n + 1
      do 50 j = 1,n
        do 40 kk = np1,p
          y(j,nc) = y(j,nc) - x(j,kk)
 40     continue
 50   continue
 60   do 70 l = 1,nc
        call dqrsl(x,ldx,n,k,qraux,y(1,l),tt,y(1,l),b(1,l),tt,tt,100,
     &             info)
 70   continue
C
C    set the unused components of b to zero and initialize jpvt
C    for unscrambling.
C
      do 90 j = 1,p
        jpvt(j) = -jpvt(j)
        if (j .le. k) goto 90
        do 80 l = 1,nc
          b(j,l) = 0.0d+0
 80     continue
 90   continue
      if (n.ne.1 .or. nc.le.1 .or. p.le.n) goto 110
C
C    if n.eq.1 and nc.gt.1 set the elements in the nc-th
C    column of b to one.
C
      do 100 j = np1,p
        b(j,nc) = 1.0d+0
 100  continue
 110  continue
C
C    unscramble the solution.
C
      do 150 j = 1,p
        if (jpvt(j) .gt. 0) goto 150
        kk = -jpvt(j)
        jpvt(j) = kk
 120    continue
        if (kk .eq. j) goto 140
        do 130 l = 1,nc
          t = b(j,l)
          b(j,l) = b(kk,l)
          b(kk,l) = t
 130    continue
        jpvt(kk) = -jpvt(kk)
        kk = jpvt(kk)
        goto 120
 140    continue
 150  continue
 160  continue
      return
      end

