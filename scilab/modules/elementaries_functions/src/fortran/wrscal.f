C/MEMBR ADD NAME=WRSCAL,SSI=0
c     Copyright INRIA
      subroutine wrscal(n,s,xr,xi,incx)
c!but
c
c     cette subroutine calcule le produit d'une constante reelle
c     double precision s par un vecteur complexe x, dont les
c     reelles de ses composantes sont rangees dans xr et les
c     parties imaginaires dans xi.
c
c!liste d'appel
c
c      subroutine wrscal(n,s,xr,xi,incx)
c
c     n: entier, longueur du vecteur x.
c
c     s: double precision. the real factor
c
c     xr, xi: doubles precision, parties reelles et imaginaires,
c     respectivement, des composantes du vecteur x.
c
c     incx: increment entre deux composantes consecutives de x.
c
c!auteur
c
c     cleve moler.- mathlab.
c
c!
      double precision s,xr(*),xi(*)
      if (n .le. 0) return
      ix = 1
      do 10 i = 1, n
         xr(ix) = s*xr(ix)
         xi(ix) = s*xi(ix)
         ix = ix + incx
   10 continue
      return
      end
