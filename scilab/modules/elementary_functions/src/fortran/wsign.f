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
