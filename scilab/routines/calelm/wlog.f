      subroutine wlog(xr,xi,yr,yi)
c!but
c     calcule une determination du log du complexe x
c!liste d'appel
c     subroutine wlog(xr,xi,yr,yi)
c     double precision xr,xi,yr,yi
c     xr,xi: parties reelles et imaginaire de l'argument
c     yr,yi:parties reelles et imaginaires du resultat
c!sous programmes appeles
c     pythag (blas.extension)
c     atan2,log (fortran)
c!
c     Copyright INRIA
      double precision xr,xi,yr,yi,t,r,pythag
      r = pythag(xr,xi)
      t = atan2(xi,xr)
      if (xi.eq.0.0d+0 .and. xr.lt.0.0d+0) t = abs(t)
      yr = log(r)
      yi = t
      return
      end
