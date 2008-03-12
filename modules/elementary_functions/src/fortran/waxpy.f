C/MEMBR ADD NAME=WAXPY,SSI=0
c     Copyright INRIA
      subroutine waxpy(n,sr,si,xr,xi,incx,yr,yi,incy)
c!but
c
c     cette subroutine multiplie la constante complexe double
c     precision s (dont la partie reelle est sr et la partie
c     imaginaire si) par le vecteur complexe double precision
c     (dont les parties reelles de ses composantes sont dans
c     le vecteur double precision xr). le produit ainsi
c     obtenu est additionne au vecteur complexe y (dont les
c     parties reelles de ses composantes sont rangees dans le
c     vecteur double precision yr et les parties imaginaires
c     dans le vecteur double precision yr). le resultat de
c     l'addition reste dans y.
c
c!liste d'appel
c
c      subroutine waxpy(n,sr,si,xr,xi,incx,yr,yi,incy)
c
c     n: entier, taille des vecteurs traites
c
c     sr, si: double precision, parties reel et imaginaire de s
c
c     xr, xi: vecteurs double precision, parties rellees et
c     imaginaires, respectivement du vecteur complexe x.
c
c     yr, yi: vecteurs double precision, parties rellees et
c     imaginaires, respectivement du vecteur complexe y.
c
c     incx, incy: entiers, increments entre deux composantes
c     successives des vecteurs x et y.
c
c!auteur
c
c     cleve moler.- mathlab.
c
c!
      double precision sr,si,xr(*),xi(*),yr(*),yi(*)
      if (n .le. 0) return
      if (sr .eq. 0.0d+0 .and. si .eq. 0.0d+0) return
      ix = 1
      iy = 1
      if (incx.lt.0) ix = (-n+1)*incx + 1
      if (incy.lt.0) iy = (-n+1)*incy + 1
      do 10 i = 1, n
         yr(iy) = yr(iy) + sr*xr(ix) - si*xi(ix)
         yi(iy) = yi(iy) + sr*xi(ix) + si*xr(ix)
         ix = ix + incx
         iy = iy + incy
   10 continue
      return
      end
