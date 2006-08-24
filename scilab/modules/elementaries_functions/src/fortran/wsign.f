      subroutine wsign(xr,xi,yr,yi,zr,zi)
c     Copyright INRIA
      double precision xr,xi,yr,yi,zr,zi,pythag,t
c     if y .ne. 0, z = x*y/abs(y)
c     if y .eq. 0, z = x
      t = pythag(yr,yi)
      zr = xr
      zi = xi
      if (t .ne. 0.0d+0) call wmul(yr/t,yi/t,zr,zi,zr,zi)
      return
      end
