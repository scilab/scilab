      subroutine rilac(n,nn,a,na,c,d,rcond,x,w,nnw,z,eps,iwrk,wrk1,wrk2,
     &                 ierr)
C!purpose
C
C        to solve the continuous time algebraic equation
C
C                trans(a)*x + x*a + c - x*d*x  =  0
C
C        where  trans(a)  denotes the transpose of  a .
C
C!method
C
C        the method used is laub's variant of the hamiltonian -
C        eigenvector approach (schur method).
C
C!reference
C
C        a.j. laub
C        a schur method for solving algebraic riccati equations
C        ieee trans. automat. control, vol. ac-25, 1980.
C
C! auxiliary routines
C
C       orthes,ortran,balanc,balbak (eispack)
C       dgeco,dgesl (linpack)
C       hqror2,inva,exchgn,qrstep 
C
C! calling sequence
C        subroutine rilac(n,nn,a,na,c,d,rcond,x,w,nnw,z,
C    +                iwrk,wrk1,wrk2,ierr)
C
C        integer n,nn,na,nnw,iwrk(nn),ierr
C        double precision a(na,n),c(na,n),d(na,n)
C        double precision rcond,x(na,n),w(nnw,nn),z(nnw,nn)
C        double precision wrk1(nn),wrk2(nn)
C
C arguments in
C
C       n       integer
C               -the order of a,c,d and x
C
C       na      integer
C               -the declared first dimension of a,c,d and x
C
C       nn      integer
C               -the order of w and z
C                    nn = n + n
C
C       nnw     integer
C               -the declared first dimension of w and z
C
C
C       a       double precision(n,n)
C
C       c       double precision(n,n)
C
C       d       double precision(n,n)
C
C arguments out
C
C       x       double precision(n,n)
C               - x  contains the solution matrix
C
C       w       double precision(nn,nn)
C               - w  contains the ordered real upper-triangular
C               form of the hamiltonian matrix
C
C       z       double precision(nn,nn)
C               - z  contains the transformation matrix which
C               reduces the hamiltonian matrix to the ordered
C               real upper-triangular form
C
C       rcond   double precision
C               - rcond  contains an estimate of the reciprocal
C               condition of the  n-th order system of algebraic
C               equations from which the solution matrix is obtained
C
C       ierr    integer
C               -error indicator set on exit
C
C               ierr  =  0       successful return
C
C               ierr  =  1       the real upper triangular form of
C                                the hamiltonian matrix cannot be
C                                appropriately ordered
C
C               ierr  =  2       the hamiltonian matrix has less than n
C                                eigenvalues with negative real parts
C
C               ierr  =  3       the  n-th order system of linear
C                                algebraic equations, from which the
C                                solution matrix would be obtained, is
C                                singular to working precision
C
C               ierr  =  4       the hamiltonian matrix cannot be
C                                reduced to upper-triangular form
C
C working space
C
C       iwrk    integer(nn)
C
C       wrk1    double precision(nn)
C
C       wrk2    double precision(nn)
C
C!originator
C
C                control systems research group, dept. eecs, kingston
C                polytechnic, penrhyn rd.,kingston-upon-thames, england.
C
C! comments
C                if there is a shortage of storage space, then the
C                matrices  c  and  x  can share the same locations,
C                but this will, of course, result in the loss of  c.
C
C*******************************************************************
C
      integer n,nn,na,nnw,iwrk(nn),ierr
      double precision a(na,n),c(na,n),d(na,n)
      double precision rcond,x(na,n),w(nnw,nn),z(nnw,nn)
      double precision wrk1(nn),wrk2(nn)
C
C        local declarations:
C
      integer i,j,low,igh,ni,nj
      double precision eps,t(1)
      integer folhp
      external folhp
      logical fail
C
C
C         eps is a machine dependent parameter specifying
C        the relative precision of realing point arithmetic.
C
C        initialise the hamiltonian matrix associated with the problem
C
      do 10 j = 1,n
        nj = n + j
        do 10 i = 1,n
          ni = n + i
          w(i,j) = a(i,j)
          w(ni,j) = -c(i,j)
          w(i,nj) = -d(i,j)
          w(ni,nj) = -a(j,i)
 10   continue
C
      call balanc(nnw,nn,w,low,igh,wrk1)
C
      call orthes(nn,nn,1,nn,w,wrk2)
      call ortran(nn,nn,1,nn,w,wrk2,z)
      call hqror2(nn,nn,1,nn,w,t,t,z,ierr,11)
      if (ierr .ne. 0) goto 70
      call inva(nn,nn,w,z,folhp,eps,ndim,fail,iwrk)
C
      if (ierr .ne. 0) goto 40
      if (ndim .ne. n) goto 50
C
      call balbak(nnw,nn,low,igh,wrk1,nn,z)
C
C
      call dgeco(z,nnw,n,iwrk,rcond,wrk1)
      if (rcond .lt. eps) goto 60
C
      do 30 j = 1,n
        nj = n + j
        do 20 i = 1,n
          x(i,j) = z(nj,i)
 20     continue
        call dgesl(z,nnw,n,iwrk,x(1,j),1)
 30   continue
      goto 100
C
 40   ierr = 1
      goto 100
C
 50   ierr = 2
      goto 100
C
 60   ierr = 3
      goto 100
C
 70   ierr = 4
      goto 100
C
 100  return
      end

