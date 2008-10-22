C/MEMBR ADD NAME=WASUM,SSI=0
c     Copyright INRIA
      double precision function wasum(n,xr,xi,incx)
c!but
c
c     cette fonction determine la addition des normes l1 des
c     composantes d'un vecteur complexe dont les parties reelles
c     sont rangees dans le vecteur double precision xr et les
c     parties imaginaires dans le vecteur double precision xi.
c
c!liste d'appel
c
c      double precision function wasum(n,xr,xi,incx)
c
c      n: entier, taille du vecteur traite
c
c      xr, xi: vecteurs double precision contenant,
c     respectivement,  les parties reelles et imaginaires du
c     vecteur traite.
c
c      incx: increment entre deux composantes consecutives des
c     vecteurs xr ou xi.
c
c!auteur
c
c     cleve moler.- mathlab.
c
c!
      double precision xr(*),xi(*),s
c     norm1(x)
      s = 0.0d+0
      if (n .le. 0) go to 20
      ix = 1
      do 10 i = 1, n
         s = s + abs(xr(ix)) + abs(xi(ix))
         ix = ix + incx
   10 continue
   20 wasum = s
      return
      end
