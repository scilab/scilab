      subroutine lybsc(n,a,na,c,x,u,eps,wrk,mode,ierr)
      integer n,na,mode,ierr
      double precision a(na,n),c(na,n),x(na,n),u(na,n),wrk(n)
      double precision eps
C
C! calling sequence
C        subroutine lybsc(n,a,na,c,x,u,wrk,mode,ierr)
C
C        integer n,na,mode,ierr
C        double precision a(na,n),c(na,n),x(na,n),u(na,n),wrk(n)
C
C arguments in
C
C
C       n        integer
C                -the dimension of a
C
C       a        double precision(n,n)
C                -coefficient matrix of the equation
C                *** n.b. in this routine  a  is overwritten with
C                its transformed upper triangular form (see comments)
C
C       c        double precision(n,n)
C                -coefficient matrix of the equation
C
C       na       integer
C                -the declared first dimension of a,c,x and u
C
C       mode     integer
C                - mode = 0  if  a  has not already been reduced to
C                                upper triangular form
C                - mode = 1  if  a  has been reduced to triangular form
C                                by (e.g) a previous call to lybsc
C
C arguments out
C
C       a        double precision(n,n)
C                -on exit, a  contains the transformed upper triangular
C                form of a
C
C       x        double precision(n,n)
C                -the solution matrix
C
C       u        double precision(n,n)
C                - u  contains the orthogonal matrix which was used
C                to reduce  a  to upper triangular form
C
C       ierr     integer
C                -error indicator
C
C                ierr = 0     successful return
C
C                ierr = 1     a  has a degenerate pair of eigenvalues
C
C                ierr = 2     a  cannot be reduced to triangular form
C
C working space
C
C        wrk     double precision(n)
C
C!purpose
C
C        to solve the double precision matrix equation
C
C               trans(a)*x + x*a = c
C
C        where  c  is symmetric, and  trans(a)  denotes
C        the transpose of  a .
C
C!method
C
C        this routine is a modification of the subroutine  atxxac,
C        written and discussed by  r.h.bartels & g.w.stewart.
C
C!reference
C
C         r.h. bartels & g.w. stewart
C            "solution of the matrix equation  a'x + xb = c  ",
C            commun. a.c.m., vol 15, 1972, pp. 820-826 .
C
C!auxiliary routines
C
C       orthes,ortran (eispack)
C       sgefa,sgesl   (linpack)
C       lycsr  (slice)
C
C!origin: adapted from
C
C                control systems research group, dept eecs, kingston
C                polytechnic, penrhyn rd.,kingston-upon-thames, england.
C
C!
C******************************************************************
C       local variables:
C
      integer i,j,k
      double precision dprec,tt(1)
C
      if (mode .eq. 1) goto 10
      call orthes(na,n,1,n,a,wrk)
      call ortran(na,n,1,n,a,wrk,u)
      call hqror2(na,n,1,n,a,tt,tt,u,ierr,11)
      if (ierr .ne. 0) goto 140
 10   continue
      do 20 i = 1,n
        do 15 j = 1,n
          x(i,j) = c(i,j)
 15     continue
        x(i,i) = x(i,i) * 0.50d+0
 20   continue
C
      do 40 i = 1,n
        do 30 j = 1,n
          dprec = 0.0d+0
          do 25 k = i,n
            dprec = dprec + x(i,k)*u(k,j)
 25       continue
          wrk(j) = dprec
 30     continue
        do 40 j = 1,n
          x(i,j) = wrk(j)
 40   continue
      do 60 j = 1,n
        do 50 i = 1,n
          dprec = 0.0d+0
          do 45 k = 1,n
            dprec = dprec + u(k,i)*x(k,j)
 45       continue
          wrk(i) = dprec
 50     continue
        do 60 i = 1,n
          x(i,j) = wrk(i)
 60   continue
C
      do 70 i = 1,n
        do 70 j = i,n
          x(i,j) = x(i,j) + x(j,i)
          x(j,i) = x(i,j)
 70   continue
C
C     call shrslv (c,a,x,n,n,na,na,na,0.0d+0,1.0d+20,fail)
      call lycsr(n,a,na,x,ierr)
      if (ierr .ne. 0) return
C
      do 80 i = 1,n
        x(i,i) = x(i,i) * 0.50d+0
 80   continue
C
      do 100 i = 1,n
        do 90 j = 1,n
          dprec = 0.0d+0
          do 85 k = i,n
            dprec = dprec + x(i,k)*u(j,k)
 85       continue
          wrk(j) = dprec
 90     continue
        do 100 j = 1,n
          x(i,j) = wrk(j)
 100  continue
C
      do 120 j = 1,n
        do 110 i = 1,n
          dprec = 0.0d+0
          do 105 k = 1,n
            dprec = dprec + u(i,k)*x(k,j)
 105      continue
          wrk(i) = dprec
 110    continue
        do 120 i = 1,n
          x(i,j) = wrk(i)
 120  continue
C
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
      subroutine lycsr(n,a,na,c,ierr)
c%calling sequence
c      subroutine lycsr(n,a,na,c,ierr)
c     integer n,na,ierr
c     double precision a(na,n),c(na,n)
c%purpose
c
c
c     this routine solves the continuous lyapunov equation where
c     the matrix  a  has been transformed to quasi-triangular form.
c
c     this routine is intended to be called only from
c            slice   routine  lybsc .
c%
      integer i,j,k,l,n,dk,dl,ia,kk,ll,na,job,km1,ldl,ierr,info,
     x        ldim,nsys,ipvt(4)
      double precision a(na,n),c(na,n)
      double precision t(4,4),p(4)
      double precision dprec
      ierr = 0
      ldim = 4
      job = 0
      l = 1
   10    dl = 1
         if (l .eq. n) go to 20
         if (a(l+1,l) .ne. 0.0d+0) dl = 2
   20    ll = l + dl - 1
         k = l
   30       km1 = k - 1
            dk = 1
            if (k .eq. n) go to 34
            if (a(k+1,k) .ne. 0.0d+0) dk = 2
   34       kk = k + dk - 1
            if (k .eq. l) go to 45
c
            do 40 i = k, kk
c
               do 40 j = l, ll
                dprec = 0.0d+0
                  do 35 ia = l, km1
                     dprec = dprec + a(ia,i) * c(ia,j)
   35             continue
                c(i,j) = c(i,j) - dprec
   40       continue
c
   45       if (dl .eq. 2) go to 60
            if (dk .eq. 2) go to 50
            t(1,1) = a(k,k) + a(l,l)
            if (t(1,1) .eq. 0.0d+0) go to 130
            c(k,l) = c(k,l) / t(1,1)
            go to 90
   50       t(1,1) = a(k,k) + a(l,l)
            t(1,2) = a(kk,k)
            t(2,1) = a(k,kk)
            t(2,2) = a(kk,kk) + a(l,l)
            p(1) = c(k,l)
            p(2) = c(kk,l)
            nsys = 2
            call dgefa(t,ldim,nsys,ipvt,info)
            if (info .ne. 0) go to 130
            call dgesl(t,ldim,nsys,ipvt,p,job)
            c(k,l) = p(1)
            c(kk,l) = p(2)
            go to 90
   60       if (dk .eq. 2) go to 70
            t(1,1) = a(k,k) + a(l,l)
            t(1,2) = a(ll,l)
            t(2,1) = a(l,ll)
            t(2,2) = a(k,k) + a(ll,ll)
            p(1) = c(k,l)
            p(2) = c(k,ll)
            nsys = 2
            call dgefa(t,ldim,nsys,ipvt,info)
            if (info .ne. 0) go to 130
            call dgesl(t,ldim,nsys,ipvt,p,job)
            c(k,l) = p(1)
            c(k,ll) = p(2)
            go to 90
   70       if (k .ne. l) go to 80
            t(1,1) = a(l,l)
            t(1,2) = a(ll,l)
            t(1,3) = 0.0d+0
            t(2,1) = a(l,ll)
            t(2,2) = a(l,l) + a(ll,ll)
            t(2,3) = t(1,2)
            t(3,1) = 0.0d+0
            t(3,2) = t(2,1)
            t(3,3) = a(ll,ll)
            p(1) = c(l,l) * 0.50d+0
            p(2) = c(ll,l)
            p(3) = c(ll,ll) * 0.50d+0
            nsys = 3
            call dgefa(t,ldim,nsys,ipvt,info)
            if (info .ne. 0) go to 130
            call dgesl(t,ldim,nsys,ipvt,p,job)
            c(l,l) = p(1)
            c(ll,l) = p(2)
            c(l,ll) = p(2)
            c(ll,ll) = p(3)
            go to 90
   80       t(1,1) = a(k,k) + a(l,l)
            t(1,2) = a(kk,k)
            t(1,3) = a(ll,l)
            t(1,4) = 0.0d+0
            t(2,1) = a(k,kk)
            t(2,2) = a(kk,kk) + a(l,l)
            t(2,3) = 0.0d+0
            t(2,4) = t(1,3)
            t(3,1) = a(l,ll)
            t(3,2) = 0.0d+0
            t(3,3) = a(k,k) + a(ll,ll)
            t(3,4) = t(1,2)
            t(4,1) = 0.0d+0
            t(4,2) = t(3,1)
            t(4,3) = t(2,1)
            t(4,4) = a(kk,kk) + a(ll,ll)
            p(1) = c(k,l)
            p(2) = c(kk,l)
            p(3) = c(k,ll)
            p(4) = c(kk,ll)
            nsys = 4
            call dgefa(t,ldim,nsys,ipvt,info)
            if (info .ne. 0) go to 130
            call dgesl(t,ldim,nsys,ipvt,p,job)
            c(k,l) = p(1)
            c(kk,l) = p(2)
            c(k,ll) = p(3)
            c(kk,ll) = p(4)
   90    k = k + dk
         if (k .le. n) go to 30
         ldl = l + dl
         if (ldl .gt. n) return
c
         do 120 j = ldl, n
c
            do 100 i = l, ll
               c(i,j) = c(j,i)
  100       continue
c
            do 120 i = j, n
                dprec = 0.0d+0
               do 110 k = l, ll
                dprec = dprec + c(i,k) * a(k,j)
                dprec = dprec + a(k,i) * c(k,j)
  110          continue
                c(i,j) = c(i,j) - dprec
               c(j,i) = c(i,j)
  120    continue
c
      l = ldl
      go to 10
c
  130 ierr = 1
      return
      end
