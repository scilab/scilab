      subroutine strang(prosca,n,m,depl,jmin,jmax,precon,alpha,ybar,
     /                 sbar,izs,rzs,dzs)
c----
c
c     Calcule le produit H g ou
c         . H est une matrice construite par la formule de bfgs inverse
c           a m memoires a partir de precon fois la matrice unite dans
c           un espace hilbertien dont le produit scalaire est donne par
c           prosca
c           (cf. J. Nocedal, math. of comp. 35/151 (1980) 773-782)
c         . g est un vecteur de dimension n (en general le gradient)
c
c     Le facteur precon apparait donc comme un preconditionneur
c     scalaire.
c
c     delp = g (en entree), = H g (en sortie)
c
c     La matrice H est memorisee par les vecteurs des tableaux
c     ybar, sbar et les pointeurs jmin, jmax.
c
c     alpha(m) est une zone de travail.
c
c     izs(1),rzs(1),dzs(1) sont des zones de travail pour prosca
c
c----
c
c         arguments
c
      integer n,m,jmin,jmax,izs(*)
      real rzs(*)
      double precision depl(n),precon,alpha(m),ybar(n,m),sbar(n,m)
      double precision dzs(*)
      external prosca
c
c         variables locales
c
      integer jfin,i,j,jp
      double precision r
      double precision ps
c
      jfin=jmax
      if (jfin.lt.jmin) jfin=jmax+m
c
c         phase de descente
c
      do 100 j=jfin,jmin,-1
          jp=j
          if (jp.gt.m) jp=jp-m
          call prosca (n,depl,sbar(1,jp),ps,izs,rzs,dzs)
          alpha(jp)=ps
          do 20 i=1,n
              depl(i)=depl(i)-ps*ybar(i,jp)
20        continue
100   continue
c
c         preconditionnement
c
      do 150 i=1,n
          depl(i)=depl(i)*precon
150   continue
c
c         remontee
c
      do 200 j=jmin,jfin
          jp=j
          if (jp.gt.m) jp=jp-m
          call prosca (n,depl,ybar(1,jp),ps,izs,rzs,dzs)
          r=alpha(jp)-ps
          do 120 i=1,n
              depl(i)=depl(i)+r*sbar(i,jp)
120       continue
200   continue
      return
      end
