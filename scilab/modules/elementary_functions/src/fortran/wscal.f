      subroutine wscal(n,sr,si,xr,xi,incx)
c!but
c
c     cette subroutine wscal multiplie une contante complexe s
c     (dont la partie reelle est rangee dans sr et la partie
c     imaginaire dans si) par un vecteur x (dont les parties
c     reelles de ses composantes sont rangees dans xr et les
c     parties imaginaires dans xi). le resultat reste dans x.
c
c!liste d'appel
c
c      subroutine wscal(n,sr,si,xr,xi,incx)
c
c     n: entier, taille du vecteur x.
c
c     sr, si: double precision, parties reelle et imaginaire
c     de s.
c
c     xr, xi: vecteurs double precision, contiennent,
c     respectivement, les parties reelles et imaginaires des
c     composants du vecteur x.
c
c     incx: entier, increment entre deux composantes consecutives
c     de x.
c
c!routines auxilieres
c
c     wmul
c
c!auteur
c
c     cleve moler.
c
      double precision sr,si,xr(*),xi(*)
      if (n .le. 0) return
      ix = 1
      do 10 i = 1, n
         call wmul(sr,si,xr(ix),xi(ix),xr(ix),xi(ix))
         ix = ix + incx
   10 continue
      return
      end
