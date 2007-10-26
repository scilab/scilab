      subroutine  vpythag(n,xr,xi)
c
c     xr(i) = pythag(xr(i),xi(i))
c     Copyright INRIA
      double precision xr(*),xi(*),pythag
      integer n
      do 10 i=1,n
         xr(i) = pythag(xr(i),xi(i))
 10   continue
      return
      end


