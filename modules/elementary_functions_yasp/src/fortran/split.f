      subroutine split(a, v, n, l, e1, e2, na, nv)
c
c!purpose
c
c     given the upper hessenberg matrix a with a 2x2 block
c     starting at a(l,l), split determines if the
c     corresponding eigenvalues are real or complex, if they
c     are real, a rotation is determined that reduces the
c     block to upper triangular form with the eigenvalue
c     of largest absolute value appearing first.  the
c     rotation is accumulated in v. the eigenvalues (real
c     or complex) are returned in e1 and e2.
c!calling sequence
c
c     subroutine split(a, v, n, l, e1, e2, na, nv)
c
c     double precision a,v,e1,e2
c     integer n,l,na,nv
c     dimension a(na,n),v(nv,n)
c
c     starred parameters are  altered by the subroutine
c
c        *a        the upper hessenberg matrix whose 2x2
c                  block is to be dsplit.
c        *v        the array in which the dsplitting trans-
c                  formation is to be accumulated.
c         n        the order of the matrix a.
c         l        the position of the 2x2 block.
c        *e1       on return if the eigenvalues are complex
c        *e2       e1 contains their common real part and
c                  e2 contains the positive imaginary part.
c                  if the eigenvalues are real. e1 contains
c                  the one largest in absolute value and f2
c                  contains the other one.
c        na        the first dimension of the array a.
c        nv        the first dimension of the array v.
c!auxiliary routines
c     abs sqrt (fortran)
c!
c originator
c
      double precision a,v,e1,e2
      integer n,l,na,nv
      dimension a(na,n),v(nv,n)
c     internal variables
c
c     internal variables
      double precision p,q,r,t,u,w,x,y,z,zero,two
      integer i,j,l1
      data zero, two /0.0d+0,2.0d+0/
      l1 = l + 1
c
      x = a(l1,l1)
      y = a(l,l)
      w = a(l,l1)*a(l1,l)
      p = (y-x)/two
      q = p**2 + w
      if (q.ge.zero) go to 10
c
c       complex eigenvalue.
c
      e1 = p + x
      e2 = sqrt(-q)
      return
   10 continue
c
c     two real eigenvalues. set up transformation.
c
      z = sqrt(q)
      if (p.lt.zero) go to 20
      z = p + z
      go to 30
   20 continue
      z = p - z
   30 continue
      if (z.eq.zero) go to 40
      r = -w/z
      go to 50
   40 continue
      r = zero
   50 continue
      if (abs(x+z).ge.abs(x+r)) z = r
      y = y - x - z
      x = -z
      t = a(l,l1)
      u = a(l1,l)
      if (abs(y)+abs(u).le.abs(t)+abs(x)) go to 60
      q = u
      p = y
      go to 70
   60 continue
      q = x
      p = t
   70 continue
      r = sqrt(p**2+q**2)
      if (r.gt.zero) go to 80
      e1 = a(l,l)
      e2 = a(l1,l1)
      a(l1,l) = zero
      return
   80 continue
      p = p/r
      q = q/r
c
c     premultiply.
c
      do 90 j=l,n
         z = a(l,j)
         a(l,j) = p*z + q*a(l1,j)
         a(l1,j) = p*a(l1,j) - q*z
   90 continue
c
c     postmultiply.
c
      do 100 i=1,l1
         z = a(i,l)
         a(i,l) = p*z + q*a(i,l1)
         a(i,l1) = p*a(i,l1) - q*z
  100 continue
c
c     accumulate the transformation in v.
c
      do 110 i=1,n
         z = v(i,l)
         v(i,l) = p*z + q*v(i,l1)
         v(i,l1) = p*v(i,l1) - q*z
  110 continue
      a(l1,l) = zero
      e1 = a(l,l)
      e2 = a(l1,l1)
      return
      end

