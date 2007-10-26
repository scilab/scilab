C/MEMBR ADD NAME=WSHRSL,SSI=0
c     Copyright INRIA
      subroutine wshrsl(ar,ai,br,bi,cr,ci,m,n,na,nb,nc,eps,rmax,fail)
c
c!purpose
c   wshrsl is a fortran iv subroutine to solve the complex matrix
c   equation ax + xb = c, where a is in lower triangular form
c   and b is in upper triangular form,
c
c!calling sequence
c
c      subroutine wshrsl(ar,ai,br,bi,cr,ci,m,n,na,nb,nc,eps,rmax,fail)
c   ar,ai  a doubly subscripted array containg the matrix a in
c          lower triangular form
c
c   br,bi  a doubly subscripted array containing tbe matrix br,bi
c          in upper triangular form
c
c   cr,ci  a doubly subscripted array containing the matrix c.
c
c   m      the order of the matrix a
c
c   n      the order of the matrix b
c
c   na     the first dimension of the array a
c
c   nb     the first dimension of the array b
c
c   nc     the first dimension of the array c
c
c   eps    tolerance on a(k,k)+b(l,l)
c          if |a(k,k)+b(l,l)|<eps algorithm suppose that |a(k,k)+b(l,l)|=eps
c
c   rmax   maximum allowed size of any element of the transformation
c
c   fail   indicates if wshrsl failed
c
c!auxiliary routines
c     ddot (blas)
c     abs sqrt (fortran)
c!originator
c     Steer Serge  I.N.R.I.A from shrslv (Bartels and Steward)
c!
c
      integer m, n, na, nb, nc
      double precision ar,ai, br,bi, cr,ci, eps,rmax
      dimension ar(na,m),ai(na,m),br(nb,n),bi(nb,n),cr(nc,n),ci(nc,n)
      logical fail
c internal variables
c
      integer k,km1,l,lm1,i
      double precision t,tr,ti,ddot
c
      fail = .true.
c
      l = 1
   10 lm1 = l - 1
      if (l.eq.1) go to 30
         do 20 i=1,m
         cr(i,l)=cr(i,l)-ddot(lm1,cr(i,1),nc,br(1,l),1)
     1                  +ddot(lm1,ci(i,1),nc,bi(1,l),1)
         ci(i,l)=ci(i,l)-ddot(lm1,cr(i,1),nc,bi(1,l),1)
     1                  -ddot(lm1,ci(i,1),nc,br(1,l),1)
   20    continue
c
   30 k = 1
   40 km1 = k - 1
      if (k.eq.1) go to 50
      cr(k,l) = cr(k,l) - ddot(km1,ar(k,1),na,cr(1,l),1)
     1                  + ddot(km1,ai(k,1),na,ci(1,l),1)
      ci(k,l) = ci(k,l) - ddot(km1,ar(k,1),na,ci(1,l),1)
     1                  - ddot(km1,ai(k,1),na,cr(1,l),1)
c
   50 tr = ar(k,k) + br(l,l)
      ti = ai(k,k) + bi(l,l)
      t=tr*tr+ti*ti
      if(t.lt.eps*eps) then
         tr=1.0d0/eps
      else
         tr=tr/t
         ti=ti/t
      endif
c
      t=cr(k,l)*tr+ci(k,l)*ti
      ci(k,l)=-cr(k,l)*ti+ci(k,l)*tr
      cr(k,l)=t
      t=sqrt(t*t+ci(k,l)*ci(k,l))
      if (t.ge.rmax) return
      k = k + 1
      if (k.le.m) go to 40
      l = l + 1
      if (l.le.n) go to 10
      fail = .false.
      return
      end
