C/MEMBR ADD NAME=IWAMAX,SSI=0
c     Copyright INRIA
      integer function iwamax(n,xr,xi,incx)
c!but
c
c     la fonction iwamax determine l'indice de la composante
c     de plus grande norme l1 d'un vecteur complexe dont les
c     parties reelles des composantes sont rangees dans le
c     vecteur double precision xr et les parties imaginaires
c     dans le vecteur xi.
c
c!liste d'appel
c
c      integer function iwamax(n,xr,xi,incx)
c
c     n: taille du vecteur
c
c     xr, xi: vecteurs double precision qui contiennent,
c     respectivement, les parties reelles et imaginaires
c     des composantes du vecteur a traiter.
c
c     incx: increment entre deux elements consecitifs des
c     vecteurs xr ou xi.
c
c!auteur
c
c     cleve moler.- mathlab.
c
c!
      double precision xr(*),xi(*),s,p
c     index of norminf(x)
      k = 0
      if (n .le. 0) go to 20
      k = 1
      s = 0.0d+0
      ix = 1
      do 10 i = 1, n
         p = abs(xr(ix)) + abs(xi(ix))
         if (p .gt. s) k = i
         if (p .gt. s) s = p
         ix = ix + incx
   10 continue
   20 iwamax = k
      return
      end
