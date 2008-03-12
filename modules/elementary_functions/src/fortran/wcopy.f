C/MEMBR ADD NAME=WCOPY,SSI=0
c     Copyright INRIA
      subroutine  wcopy(n,dxr,dxi,incx,dyr,dyi,incy)
c!calling sequence
c      subroutine  dcopy(n,dxr,dxi,incx,dyr,dyi,incy)
c
c!purpose
c     copies a vector, x, to a vector, y.
c     uses unrolled loops for increments equal to one.
c!originator
c     jack dongarra, linpack, 3/11/78.
c!
c
       double precision dxr(*),dxi(*),dyr(*),dyi(*)
      integer i,incx,incy,ix,iy,n
c
      if(n.le.0)return
      if(incx.eq.1.and.incy.eq.1)go to 20
c
c code for unequal increments or equal increments not equal to 1
c
      ix = 1
      iy = 1
      if(incx.lt.0)ix = (-n+1)*incx + 1
      if(incy.lt.0)iy = (-n+1)*incy + 1
      do 10 i = 1,n
        dyr(iy) = dxr(ix)
        dyi(iy) = dxi(ix)
        ix = ix + incx
        iy = iy + incy
   10 continue
      return
c
c code for both increments equal to 1
c
   20 continue
      do 30 i = 1,n
        dyr(i) = dxr(i)
        dyi(i) = dxi(i)
   30 continue
c
      end
