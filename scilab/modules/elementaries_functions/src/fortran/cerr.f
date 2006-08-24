      subroutine cerr(a,w,ia,n,ndng,m,maxc)
c!purpose
c     cerr evaluate the error introduced by pade
c     approximant and normalise the  matrix a accordingly
c!calling sequence
c
c     subroutine cerr(a,w,ia,n,ndng,m,maxc)
c
c     a        : array containing the matrix a
c
c     w        : work space array of size 2*n*n + n
c
c     ia       : leading dimension of array a
c
c     n        : size of matrix a
c
c     ndng     : degree of pade approximant
c
c     m        :  the factor of normalisation is 2**(-m)
c
c     maxc     : maximum admissible for m
c
c!auxiliary routines
c     dmmul dmcopy  gdcp2i (blas.extension)
c     dset dcopy ddot (blas)
c     abs real dble (fortran)
c!
c
      integer ia,n,ndng,m,maxc
      double precision a,w
      dimension a(ia,n),w(*)
c
c internal variables
      integer k,mm,i,j,mt
      double precision norm,alpha,zero,two,norm1,one,ddot
      logical itab(15)
c
      data zero, one, two /0.0d+0,1.0d+0,2.0d+0/
c
c
      norm=0.0d+0
      n2=n*n
      k1=1
      ke=k1+n2
      kw=ke+n2
      k = 2*ndng
      call dmmul(a,ia,a,ia,w(ke),n,n,n,n)
      call gdcp2i(k, itab, mt)
      if (.not.itab(1)) go to 30
      norm = zero
      do 20 i=1,n
         alpha = zero
         do 10 j=1,n
            alpha = alpha + abs(a(i,j))
   10    continue
         if (alpha.gt.norm) norm = alpha
   20 continue
      call dmcopy(a,ia,w(k1),n,n,n)
      go to 40
   30 call dset(n2,0.0d+0,w(k1),1)
      call dset(n,1.0d+0,w(k1),n+1)
   40 if (mt.eq.1) go to 110
      do 100 i1=2,mt
         do 70 j=1,n
            l = 0
            do 50 i=1,n
               w(kw-1+i) = ddot(n,w(k1-1+j),n,w(ke+l),1)
               l = l + n
   50       continue
            call dcopy(n,w(kw),1,w(k1-1+j),n)
   70    continue
         if (.not.itab(i1)) go to 100
         norm1 = zero
         do 90 i=1,n
            alpha = zero
            l = i - 1
            do 80 j=1,n
               alpha = alpha + abs(w(k1+l))
               l = l + n
   80       continue
            if (alpha.gt.norm1) norm1 = alpha
   90    continue
         norm = norm*norm1
  100 continue
  110 continue
      norm = norm/dble(real(k+1))
      do 120 i=1,ndng
         norm = norm/dble(real((k-i+1)**2))
  120 continue
      norm = 8.0d+0*norm
      mm = 0
  130 if (norm+one .le. one) go to 140
      mm = mm + 1
      alpha = two**mm
      norm = norm/alpha
      if ((mm+m).gt.maxc) go to 140
      go to 130
  140 continue
      alpha = (two**mm)
      do 160 i=1,n
         do 150 j=1,n
            a(i,j) = a(i,j)/alpha
  150    continue
  160 continue
      m = m + mm
      return
      end
