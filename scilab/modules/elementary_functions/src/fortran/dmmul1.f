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
      subroutine dmmul1(a,na,b,nb,c,nc,l,m,n)
c!but
c     ce sous programme effectue le produit matriciel:
c     c=c+a*b .
c!liste d'appel
c
c     subroutine dmmul1(a,na,b,nb,c,nc,l,m,n)
c     double precision a(na,m),b(nb,n),c(nc,n)
c     integer na,nb,nc,l,m,n
c
c     a            tableau de taille na*m contenant la matrice a
c     na           nombre de lignes du tableau a dans le programme appel
c     b,nb,c,nc    definitions similaires a celles de a,na
c     l            nombre de ligne des matrices a et c
c     m            nombre de colonnes de a et de lignes de b
c     n            nombre de colonnes de b et c
c!sous programmes utilises
c     neant
c
      double precision a(*),b(*),c(*)
      double precision ddot
      integer na,nb,nc,l,m,n
      integer i,j,ib,ic
c
      ib=1
      ic=0
      do 30 j=1,n
         do 20 i=1,l
   20    c(ic+i)=c(ic+i)+ddot(m,a(i),na,b(ib),1)
      ic=ic+nc
      ib=ib+nb
   30 continue
      return
      end
