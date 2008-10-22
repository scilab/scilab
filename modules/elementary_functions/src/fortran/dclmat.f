      subroutine dclmat(ia, n, a, b, ib, w, c, ndng)
c
c%purpose
c      computes a matrix polynomial representated in a chebyshev
c      base by the clenshaw method.
c
c%calling sequence
c
c     subroutine dclmat(ia, n, a, b, ib,w , c, ndng)
c
c     integer ia,n,ib,ndng
c     double precision a,b,w,c
c     dimension a(ia,n), b(ib,n), c(*), w(*)
c
c      ia: the leading dimension of array a.
c      n: the order of the matrices a,b.
c      a: the  array that contains the n*n matrix a
c      b: the  array that contains the n*n matrix
c         pol(a).
c      ib:the leading dimension of array b.
c      w : work-space array of size n+n
c      c:  vectors which contains the coefficients
c      of the polynome.
c      ndng: the polynomial order.
c
c%auxiliary routines
c     dmmul (blas.extension)
c%
c
      integer ia,n,ib,ndng
      double precision a,b,w,c
      dimension a(ia,n), b(ib,n), c(*), w(*)
c internal variables
c
      integer i1,i,im1,j,ndng1,ndng2
      double precision two,zero,rc,wd,w1,half
      data zero, two, half /0.0d+0,2.0d+0,0.50d+0/
c
      ndng1 = ndng + 2
      ndng2 = ndng - 1
      rc = c(ndng1-1)
      wd = c(1)
      do 60 j=1,n
         do 10 i=1,n
            w(n+i) = zero
            w(i) = zero
   10    continue
         do 30 i1=1,ndng
            im1 = ndng1 - i1
            call dmmul(a,ia,w,n,b(1,j),ib,n,n,1)
            do 20 i=1,n
               w1 = two*b(i,j) - w(n+i)
               w(n+i) = w(i)
               w(i) = w1
   20       continue
            w(j) = w(j) + c(im1)
   30    continue
      call dmmul(a,ia,w,n,b(1,j),ib,n,n,1)
         do 40 i=1,n
            w(i) = two*b(i,j) - w(n+i)
   40    continue
         w(j) = w(j) + wd
         do 50 i=1,n
            b(i,j) = (w(i)-w(n+i))*half
   50    continue
         b(j,j) = b(j,j) + half*wd
   60 continue
      return
      end
