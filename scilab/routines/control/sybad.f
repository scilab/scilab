      subroutine sybad(n,m,a,na,b,nb,c,nc,u,v,eps,wrk,mode,ierr)
C
C
C!purpose
C
C        to solve the double precision matrix equation
C               a*x*b - x = c
C
C! calling sequence
C       subroutine sybad(n,m,a,na,b,nb,c,nc,u,v,eps,wrk,mode,ierr)
C
C        integer n,na,mode,ierr
C        double precision a(na,n),c(nc,m),u(na,n),wrk(max(n,m))
C        double precision b(nb,m),v(nb,m)
C
C arguments in
C
C       n        integer
C                -the dimension of a.
C
C       m        imteger
C                -the dimension of b.
C
C       a        double precision(n,n)
C                -the coefficient matrix  a  of the equation. on
C                exit, a  is overwritten, but see  comments  below.
C
C       na       integer
C                -the declared first dimension of  a and  u
C
C       b        double precision(m,m)
C                -the coefficient matrix  b  of the equation. on
C                exit, b  is overwritten, but see  comments  below.
C
C       nb       integer
C                -the declared first dimension of  b and  v
C
C       c        double precision(n,n)
C                -the coefficient matrix  c  of the equation.
C
C       nc       integer
C                -the declared first dimension of  c
C
C       mode     integer
C
C                - mode = 0  if  a  has not already been reduced to
C                                upper triangular form
C
C                - mode = 1  if  a  has been reduced to triangular form
C                         by (e.g.) a previous call to this routine
C
C arguments out
C
C       a        double precision(n,n)
C                -on exit, a  contains the transformed upper
C                triangular form of a.   (see comments below)
C
C       b        double precision(n,n)
C                -on exit, b  contains the transformed lower
C                triangular form of b.   (see comments below)
C
C       c        double precision(n,m)
C                -the solution matrix
C
C       u        double precision(n,n)
C                - u  contains the orthogonal matrix which was
C                used to reduce  a  to upper triangular form
C
C       v        double precision(m,m)
C                - v  contains the orthogonal matrix which was
C                used to reduce  b  to lower triangular form
C
C       ierr    integer
C               -error indicator
C
C               ierr = 0        successful return
C
C               ierr = 1        a  has reciprocal eigenvalues
C
C               ierr = 2        a  cannot be reduced to triangular form
C
C working space
C
C        wrk        double precision (max(n,m))
C
C!originator
C
C     Serge Steer Inria 1987
C     Copyright SLICOT
C!comments
C                note that the contents of  a  are overwritten by
C                this routine by the triangularised form of  a.
C                if required, a  can be re-formed from the matrix
C                product u' * a * u. this is not done by the routine
C                because the factored form of  a  may be required by
C                further routines.
C
C!method
C
C        this routine is a modification of the subroutine d2lyb,
C        written and discussed by a.y.barraud (see reference).
C
C!reference
C
C        a.y.barraud
C        "a numerical algorithm to solve  a' * x * a  -  x  =  q  ",
C        ieee trans. automat. control, vol. ac-22, 1977, pp 883-885
C
C!auxiliary routines
C
C       ddot (blas)
C       orthes,ortran (eispack)
C       sgefa,sgesl   (linpack)
C
C!
C
      integer n,na,mode,ierr
      double precision a(na,n),c(nc,m),u(na,n),wrk(*)
      double precision b(nb,m),v(nb,m)
C
C        internal variables:
C
      integer i,j
      double precision ddot,t,eps,tt(1)
C
      if (mode .eq. 1) goto 30
      do 10 i = 1,n
        do 10 j = 1,i
          t = a(i,j)
          a(i,j) = a(j,i)
          a(j,i) = t
 10   continue
      call orthes(na,n,1,n,a,wrk)
      call ortran(na,n,1,n,a,wrk,u)
      call hqror2(na,n,1,n,a,tt,tt,u,ierr,11)
      if (ierr .ne. 0) goto 140
      call orthes(nb,m,1,m,b,wrk)
      call ortran(nb,m,1,m,b,wrk,v)
      call hqror2(nb,m,1,m,b,tt,tt,v,ierr,11)
      if (ierr .ne. 0) goto 140
C
 30   do 40 i = 1,n
        do 35 j = 1,m
          wrk(j) = ddot(m,c(i,1),nc,v(1,j),1)
 35     continue
        do 40 j = 1,m
          c(i,j) = wrk(j)
 40   continue
      do 60 j = 1,m
        do 55 i = 1,n
          wrk(i) = ddot(n,u(1,i),1,c(1,j),1)
 55     continue
        do 60 i = 1,n
          c(i,j) = wrk(i)
 60   continue
C
      call sydsr(n,m,a,na,b,nb,c,nc,ierr)
      if (ierr .ne. 0) return
C
      do 100 i = 1,n
        do 95 j = 1,m
          wrk(j) = ddot(m,c(i,1),nc,v(j,1),nb)
 95     continue
        do 100 j = 1,m
          c(i,j) = wrk(j)
 100  continue
      do 120 j = 1,m
        do 115 i = 1,n
          wrk(i) = ddot(n,u(i,1),na,c(1,j),1)
 115    continue
        do 120 i = 1,n
          c(i,j) = wrk(i)
 120  continue
C
      goto 150
 140  ierr = 2
 150  return
      end

