c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
C/MEMBR ADD NAME=MTRAN,SSI=0
c
      subroutine mtran(a,na,b,nb,m,n)
c!but
c     mtran transpose la matrice a dans le tableau b
c     a et b n'ayant pas la meme implantation memoire
c
c!liste d'appel
c     subroutine mtran(a,na,b,nb,m,n)
c     double precision a(na,n),b(nb,m)
c     integer na,nb,m,n
c
c     a        tableau contenant la matrice a
c     na       nombre de ligne du tableau a dans le prog appelant
c     b,nb     definition similaire a celle de a,na
c     m        nombre de lignes de a et de colonnes de b
c     n        nombre de colonnes de a et de lignes de b
c!sous programmes utilises
c     neant
c!
      double precision a(*),b(*)
      integer na,nb,m,n
      integer i,j,ia,ib
c
      ia=0
      do 20 j=1,n
      ib=j
         do 10 i=1,m
         b(ib)=a(ia+i)
         ib=ib+nb
   10    continue
      ia=ia+na
   20 continue
      return
      end
