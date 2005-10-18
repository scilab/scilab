      double precision function mnddot(n,dx,incx,dy,incy)
c
c     forms the dot product of two vectors.
c     uses unrolled loops for increments equal to one.
c     jack dongarra, linpack, 3/11/78.
c     modified 12/3/93, array(1) declarations changed to array(*)
c
      double precision dx(*),dy(*),dtemp
      double precision mpadd
      integer i,incx,incy,ix,iy,m,mp1,n
c
      if(n.le.0)return
      if(incx.eq.1.and.incy.eq.1)go to 20
c
c        code for unequal increments or equal increments
c          not equal to 1
c
      ix = 1
      iy = 1
      if(incx.lt.0)ix = (-n+1)*incx + 1
      if(incy.lt.0)iy = (-n+1)*incy + 1
      dtemp = mpadd(dx(ix),dy(iy))
c       write(*,*)dx(ix)
c       write(*,*)dy(iy)
        ix = ix + incx
        iy = iy + incy
      do 10 i = 2,n
        dtemp = min(dtemp , mpadd(dx(ix),dy(iy)))
c       write(*,*)dx(ix)
c       write(*,*)dy(iy)
c       write(*,*)'mn'
        ix = ix + incx
        iy = iy + incy
   10 continue
      mnddot = dtemp
c     write(*,*)mnddot
      return
c
c        code for both increments equal to 1
c
c
c        clean-up loop
c
   20 dtemp = mpadd(dx(1),dy(1))
      do 50 i = 2,n
        dtemp = min(dtemp , mpadd(dx(i),dy(i)))
c       write(*,*)dx(i)
c       write(*,*)dy(i)
c       write(*,*)'mn'
   50 continue
   60 mnddot = dtemp
      return
      end
