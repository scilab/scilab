c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
      subroutine wsign(xr,xi,yr,yi,zr,zi)
c
      double precision xr,xi,yr,yi,zr,zi,pythag,t
c     if y .ne. 0, z = x*y/abs(y)
c     if y .eq. 0, z = x
      t = pythag(yr,yi)
      zr = xr
      zi = xi
      if (t .ne. 0.0d+0) call wmul(yr/t,yi/t,zr,zi,zr,zi)
      return
      end
