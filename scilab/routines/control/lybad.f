      subroutine lybad(n,a,na,c,x,u,eps,wrk,mode,ierr)
C******************************************************************
C
C name                lybad
C
C        subroutine lybad(n,a,na,c,x,u,wrk,mode,ierr)
C
C        integer n,na,mode,ierr
C        double precision a(na,n),c(na,n),x(na,n),u(na,n),wrk(n)
C
C
C!purpose
C
C        to solve the double precision matrix equation
C               trans(a)*x*a - x = c
C        where  c  is symmetric, and  trans(a)  denotes
C        the transpose of  a.
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
C! calling sequence
C arguments in
C
C       n        integer
C                -the dimension of a.
C
C       a        double precision(n,n)
C                -the coefficient matrix  a  of the equation. on
C                exit, a  is overwritten, but see  comments  below.
C
C       c        double precision(n,n)
C                -the coefficient matrix  c  of the equation.
C
C       na       integer
C                -the declared first dimension of  a, c, x  and  u
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
C       x        double precision(n,n)
C                -the solution matrix
C
C       u        double precision(n,n)
C                - u  contains the orthogonal matrix which was
C                used to reduce  a  to upper triangular form
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
C        wrk        double precision(n)
C
C!origin: adapted from
C
C                control systems research group, dept eecs, kingston
C                polytechnic, penrhyn road, kingston-upon-thames, u.k.
C
C!comments
C                note that the contents of  a  are overwritten by
C                this routine by the triangularised form of  a.
C                if required, a  can be re-formed from the matrix
C                product u' * a * u. this is not done by the routine
C                because the factored form of  a  may be required by
C                further routines.
C
C!
C
      integer n,na,mode,ierr
      double precision a(na,n),c(na,n),x(na,n),u(na,n),wrk(n)
C
C        internal variables:
C
      integer i,j
      double precision ddot,t(1),eps
C
      if (mode .eq. 1) goto 10
      call orthes(na,n,1,n,a,wrk)
      call ortran(na,n,1,n,a,wrk,u)
      call hqror2(na,n,1,n,a,t,t,u,ierr,11)
      if (ierr .ne. 0) goto 140
C
 10   do 20 j = 1,n
        do 15 i = 1,n
          x(i,j) = c(i,j)
 15     continue
        x(j,j) = x(j,j) * 0.50d+0
 20   continue
      do 40 i = 1,n
        do 35 j = 1,n
          wrk(j) = ddot(n-i+1,x(i,i),na,u(i,j),1)
 35     continue
        do 40 j = 1,n
          x(i,j) = wrk(j)
 40   continue
      do 60 j = 1,n
        do 55 i = 1,n
          wrk(i) = ddot(n,u(1,i),1,x(1,j),1)
 55     continue
        do 60 i = 1,n
          x(i,j) = wrk(i)
 60   continue
      do 70 i = 1,n
        do 70 j = i,n
          x(i,j) = x(i,j) + x(j,i)
          x(j,i) = x(i,j)
 70   continue
      call lydsr(n,a,na,x,ierr)
      if (ierr .ne. 0) return
      do 80 i = 1,n
        x(i,i) = x(i,i) * 0.50d+0
 80   continue
      do 100 i = 1,n
        do 95 j = 1,n
          wrk(j) = ddot(n-i+1,x(i,i),na,u(j,i),na)
 95     continue
        do 100 j = 1,n
          x(i,j) = wrk(j)
 100  continue
      do 120 j = 1,n
        do 115 i = 1,n
          wrk(i) = ddot(n,u(i,1),na,x(1,j),1)
 115    continue
        do 120 i = 1,n
          x(i,j) = wrk(i)
 120  continue
      do 130 i = 1,n
        do 130 j = i,n
          x(i,j) = x(i,j) + x(j,i)
          x(j,i) = x(i,j)
 130  continue
C
      goto 150
 140  ierr = 2
 150  return
      end

