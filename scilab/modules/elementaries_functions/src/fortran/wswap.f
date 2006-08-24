C/MEMBR ADD NAME=WSWAP,SSI=0
c     Copyright INRIA
      subroutine wswap(n,xr,xi,incx,yr,yi,incy)
c!but
c
c     cette subroutine wswap echange le contenu de deux vecteurs
c     complexes x et y (dont les parties reelles de ses
c     composantes sont rangees, respectivement, dans xr et yr
c     et les parties imaginaires dans xi et yi).
c
c!liste d'appel
c
c      subroutine wswap(n,xr,xi,incx,yr,yi,incy)
c
c     n: entier, taille des vecteur x et y.
c
c     xr, xi: vecteurs double precision, parties reelles et
c     imaginaires, respectivement, des composantes du vecteur x.
c
c     incx: entier, increment entre deux elements consecutifs
c     de x.
c
c     yr, yi: vecteurs double precision, parties reelles et
c     imaginaire, respectivement, des composantes du vecteur y.
c
c     incy: entier, increment entre deux elements consecutifs
c     de x.
c
c!auteur
c
c     cleve moler.- mathlab.
c
c!
      double precision xr(*),xi(*),yr(*),yi(*),t
      if (n .le. 0) return
      ix = 1
      iy = 1
      if (incx.lt.0) ix = (-n+1)*incx + 1
      if (incy.lt.0) iy = (-n+1)*incy + 1
      do 10 i = 1, n
         t = xr(ix)
         xr(ix) = yr(iy)
         yr(iy) = t
         t = xi(ix)
         xi(ix) = yi(iy)
         yi(iy) = t
         ix = ix + incx
         iy = iy + incy
   10 continue
      return
      end
