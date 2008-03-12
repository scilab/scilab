c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
C/MEMBR ADD NAME=WDOTCI,SSI=0
c
      double precision function wdotci(n,xr,xi,incx,yr,yi,incy)
      double precision xr(*),xi(*),yr(*),yi(*),s
      s = 0.0d+0
      if (n .le. 0) go to 20
      ix = 1
      iy = 1
      if (incx.lt.0) ix = (-n+1)*incx + 1
      if (incy.lt.0) iy = (-n+1)*incy + 1
      do 10 i = 1, n
         s = s + xr(ix)*yi(iy) - xi(ix)*yr(iy)
         ix = ix + incx
         iy = iy + incy
   10 continue
   20 wdotci = s
      return
      end
