C/MEMBR ADD NAME=WDOTCR,SSI=0
c     Copyright INRIA
      double precision function wdotcr(n,xr,xi,incx,yr,yi,incy)
      double precision xr(*),xi(*),yr(*),yi(*),s
      s = 0.0d+0
      if (n .le. 0) go to 20
      ix = 1
      iy = 1
      if (incx.lt.0) ix = (-n+1)*incx + 1
      if (incy.lt.0) iy = (-n+1)*incy + 1
      do 10 i = 1, n
         s = s + xr(ix)*yr(iy) + xi(ix)*yi(iy)
         ix = ix + incx
         iy = iy + incy
   10 continue
   20 wdotcr = s
      return
      end
