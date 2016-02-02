c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
C/MEMBR ADD NAME=WDOTCR,SSI=0
c
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
