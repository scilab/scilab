c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
      subroutine ddd2 (prosca,ctonb,ctcab,n,nm,depl,aux,jmin,jmax,diag,
     /                alpha,ybar,sbar,izs,rzs,dzs)
c
c     calcule le produit h g ou
c         . h est une matrice construite par la formule de bfgs inverse
c           a nm memoires a partir de la matrice diagonale diag
c           dans un espace hilbertien dont le produit scalaire
c           est donne par prosca
c           (cf. J. Nocedal, Math. of Comp. 35/151 (1980) 773-782)
c         . g est un vecteur de dimension n (en general le gradient)
c
c     la matrice diag apparait donc comme un preconditionneur diagonal
c
c     depl = g (en entree), = h g (en sortie)
c
c     la matrice h est memorisee par les vecteurs des tableaux
c     ybar, sbar et les pointeurs jmin, jmax
c
c     alpha(nm) est une zone de travail
c
c     izs(1),rzs(1),dzs(1) sont des zones de travail pour prosca
c
c----
c
c         arguments
c
      integer n,nm,jmin,jmax,izs(1)
      real rzs(1)
      double precision depl(n),diag(n),alpha(nm),ybar(n,nm),sbar(n,nm),
     /    aux(n),dzs(1)
      external prosca,ctonb,ctcab
c
c         variables locales
c
      integer jfin,i,j,jp
      double precision r,ps
c
      jfin=jmax
      if (jfin.lt.jmin) jfin=jmax+nm
c
c         phase de descente
c
      do 100 j=jfin,jmin,-1
          jp=j
          if (jp.gt.nm) jp=jp-nm
          call prosca (n,depl,sbar(1,jp),ps,izs,rzs,dzs)
          r=ps
          alpha(jp)=r
          do 20 i=1,n
              depl(i)=depl(i)-r*ybar(i,jp)
20        continue
100   continue
c
c         preconditionnement
c
      call ctonb (n,depl,aux,izs,rzs,dzs)
      do 150 i=1,n
          aux(i)=aux(i)*diag(i)
150   continue
      call ctcab (n,aux,depl,izs,rzs,dzs)
c
c         remontee
c
      do 200 j=jmin,jfin
          jp=j
          if (jp.gt.nm) jp=jp-nm
          call prosca (n,depl,ybar(1,jp),ps,izs,rzs,dzs)
          r=alpha(jp)-ps
          do 120 i=1,n
              depl(i)=depl(i)+r*sbar(i,jp)
120       continue
200   continue
      return
      end
