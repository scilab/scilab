C/MEMBR ADD NAME=WNRM2,SSI=0
c     Copyright INRIA
      double precision function wnrm2(n,xr,xi,incx)
c!purpose
c
c     cette fonction wnrm2 determine la norme l2 d'un vecteur
c     complexe double precision x, dont les parties reelles
c     sont rangees dans le vecteur double precision xr et
c     les parties imaginaires sont rangees dans le vecteur
c     double precision xi.
c
c!calling sequence
c
c      double precision function wnrm2(n,xr,xi,incx)
c
c      n: entier, taille du vecteur traite.
c
c      xr, xi: vecteurs double precision, parties reelles et
c     imaginaires, respectivement du vecteur x.
c
c      incx: increment entre deux composantes consecutives du
c     vecteur x.
c
c!auxiliary routines
c
c     pythag
c
c!author
c
c     cleve moler.- mathlab.
c
c!
      double precision xr(*),xi(*),pythag,s
c     norm2(x)
      s = 0.0d+0
      if (n .le. 0) go to 20
      ix = 1
      do 10 i = 1, n
         s = pythag(s,xr(ix))
         s = pythag(s,xi(ix))
         ix = ix + incx
   10 continue
   20 wnrm2 = s
      return
      end
