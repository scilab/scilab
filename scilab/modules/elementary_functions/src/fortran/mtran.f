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
