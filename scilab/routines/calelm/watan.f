      subroutine watan(xr,xi,yr,yi)
c!but
c     watan calcule l'arctangente d'un complexe
c!liste d'appel
c     subroutine watan(xr,xi,yr,yi)
c     double precision xr,xi,yr,yi
c
c     xr,xi : parties reelles et imaginaires du nombre
c     yr,yi : parties reelles et imaginaires du resultat
c             yr,yi peuvent avoir la meme implantation memoire que
c              xr et xi
c!sous programmes appeles
c     wdiv wlog (blas.extensions)
c!
c     y = atan(x) = (i/2)*log((i+x)/(i-x))
c     Copyright INRIA
      double precision xr,xi,yr,yi,tr,ti
      if(xi.ne.0.0d0) goto 20
      yr = atan2(xr,1.0d+0)
      yi = 0.0d+0
      return
   20 call wdiv(xr,1.0d+0+xi,-xr,1.0d+0-xi,tr,ti)
      call wlog(tr,ti,tr,ti)
      yr = -ti/2.0d+0
      yi = tr/2.0d+0
      return
      end
