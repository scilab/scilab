      subroutine dad(a, na, i1, i2, j1, j2, r, isw)
c
c!purpose
c
c     dad compute the matrix product a=d*a or a=a*d
c     where d is the matrix with ones on the anti-
c     diagonal and a the input matrix. it also
c     multiplies each element of the product with
c     the constant r
c!calling sequence
c
c     subroutine dad(a, na, i1, i2, j1, j2, r, isw)
c     integer i1, i2, j1, j2, na, isw
c     double precision a, r
c     dimension a(na,*)
c
c     a : input matrix
c
c     na: leading dimension of a
c
c     i1,i2 : the first and the last rows of a to be considered
c
c     j1,j2 : the first and the last columns of a to be considered
c
c     r: constant factor
c
c      isw: parameter specifying the product to be done
c          isw=0 : d*a
c          isw=1 : a*d
c
c!auxiliary routines
c     ifix real mod (fortran)
c!
c
c     Copyright INRIA
      integer i1, i2, j1, j2, na, isw
      double precision a, r
      dimension a(na,*)
c internal variables
c
      integer i1i ,i2i,j1j,j2j,nrd2,ip1,i,j,ncd2
      double precision temp
c
      if (isw.eq.1) go to 60
      if (i1.eq.i2) go to 40
c
      nrd2 = ifix(real((i2-i1+1)/2))
      do 20 j=j1,j2
         do 10 ip1=1,nrd2
            i = ip1 - 1
            i1i = i1 + i
            i2i = i2 - i
            temp = a(i1i,j)
            a(i1i,j) = a(i2i,j)*r
            a(i2i,j) = temp*r
   10    continue
   20 continue
      if (mod(i2-i1,2).eq.1) return
      i = i1 + nrd2
      do 30 j=j1,j2
         a(i,j) = a(i,j)*r
   30 continue
      return
   40 continue
      do 50 j=j1,j2
         a(i1,j) = a(i1,j)*r
   50 continue
      return
c
c
c         computes the product ad where d is as above.
c
c
c
   60 continue
      if (j1.eq.j2) go to 100
      ncd2 = ifix(real((j2-j1+1)/2))
      do 80 jp1=1,ncd2
         j = jp1 - 1
         do 70 i=i1,i2
            j1j = j1 + j
            j2j = j2 - j
            temp = a(i,j1j)
            a(i,j1j) = a(i,j2j)*r
            a(i,j2j) = temp*r
   70    continue
   80 continue
      if (mod(j2-j1,2).eq.1) return
      j = j1 + ncd2
      do 90 i=i1,i2
         a(i,j) = a(i,j)*r
   90 continue
      return
  100 continue
      do 110 i=i1,i2
         a(i,j1) = a(i,j1)*r
  110 continue
      return
      end
