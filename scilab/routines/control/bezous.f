       subroutine bezous(a,n,c,w,ierr)
c!but
c  ce sous programme calcule le second coefficient de bezout du
c  couple (a,at) si at=(z**n)*a(1/z) et a est un polynome de degre
c  n ayant tous ces poles dans le disque unite.;a et at sont donc
c  premiers
c!liste d'appel
c
c       subroutine bezous(a,n,c,w,ierr)
c
c a:tableau de taille n+1 contenant les coefficients du polynome a
c    ranges par puissances croissantes.
c n:degre de a
c c:tableau de taille n contenant apres execution le facteur de bezout
c    les coefficients etant ranges par puissances croissantes
c w:tableau de travail de taille n*n+(n+1)/2
c ierr:indicateur d'erreur:
c      si ierr=0 ok
c      si ierr=1 a est de degre inferieur a n
c      si ierr=2 a et at non premiers ou calcul numeriquement faux.
c
c!methode:
c la methode utilisee ici est de resoudre le systeme lineaire associe
c a la relation de bezout: a*b+at*c=1 c'est a dire:
c      [x' y'] [e1]  [b]
c      [     ].[  ] =[ ]
c      [y  x ] [ 0]  [c]
c ou x et y sont des matrice n*n toeplitz triangulaires superieures
c la premiere ligne de x est formee des n premiers coefficient de a
c la premiere ligne de y  des n derniers ranges en ordre inverse
c c est alors solution du systeme : (y'-x'*(y**-1)*x)*c=e1
c!auteur
c     serge Steer Inria 1983
c     Copyright INRIA
c!sous programmes appeles
c     invtpl
c     ddot (blas)
c     dlslv (linpack.extension)
c!
      implicit double precision (a-h,o-z)
      dimension a(*),c(*),w(*)
c
c calcul de la premiere ligne de y**-1
      call invtpl(a(2),n,n,c,ierr)
      if (ierr.ne.0) goto 70
      ierr=0
c c contient les coeff de la premiere ligne de y**-1 dans l'ordre invers
c
c calcul de la premiere ligne de la matrice de toeplitz:-(y**-1)*x
      j=n+1
      do 10 jj=1,n
      j=j-1
      c(jj)=-ddot(j,a,-1,c(jj),-1)
   10 continue
c c contient la premiere ligne du produit ranges dans l'ordre inverse
c
c calcul de x'*(-(y**-1)*x)
      i2=0
      do 20 i=1,n
      w(i2+i)=a(i)*c(n)
   20 continue
      if(n.eq.1) goto 45
       do 40 j=2,n
       i1=i2
       i2=i2+n
       w(i2+1)=a(1)*c(n+1-j)
       do 30 i=2,n
       w(i2+i)=w(i1+i-1)+a(i)*c(n+1-j)
   30 continue
   40 continue
c calcul de y'+w
   45 i1=-n
      do 60 j=1,n
      c(j)=0.0d+0
      i1=i1+n
      do 50 i=j,n
      w(i1+i)=w(i1+i)+a(n+1+j-i)
   50 continue
   60 continue
      c(1)=1.0d+0
c w contient la matrice du systeme lineaire et c le second menbre
c
c resolution
      call dlslv(w,n,n,c,n,1,w(n*n+1),rcond,ierr,1)
      if(ierr.ne.0) goto 80
      return
   70 ierr=1
      return
   80 ierr=2
      return
      end
