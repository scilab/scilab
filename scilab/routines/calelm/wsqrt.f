      subroutine wsqrt(xr,xi,yr,yi)
c     Copyright INRIA
      double precision xr,xi,yr,yi,s,tr,ti,pythag
c     y = sqrt(x) with yr .ge. 0.0 and sign(yi) .eq. sign(xi)
c
      tr = xr
      ti = xi
      s = sqrt(0.50d+0*(pythag(tr,ti) + abs(tr)))
      if (tr .ge. 0.0d+0) yr = s
      if (ti .lt. 0.0d+0) s = -s
      if (tr .le. 0.0d+0) yi = s
      if (tr .lt. 0.0d+0) yr = 0.50d+0*(ti/yi)
      if (tr .gt. 0.0d+0) yi = 0.50d+0*(ti/yr)
      return
      end
