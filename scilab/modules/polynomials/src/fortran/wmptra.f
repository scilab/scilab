c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA - Serge STEER
c
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
C/MEMBR ADD NAME=WMPTRA,SSI=0
      subroutine wmptra(pm1r,pm1i,d1,ld1,pm2r,pm2i,d2,m,n)
c!but
c     cette subroutine transpose une matrice dont les coefficients
c     sont des polynomes a coefficients complexes
c     pm2=transpose(pm1)
c!liste d'appel
c
c    subroutine wmptra(pm1r,pm1i,d1,ld1,pm2r,pm2i,d2,m,n)
c    double precision pm1r(*),pm1i(*),pm2r(*),pm2i(*)
c    integer d1(*),d2(n,m),m,n
c
c     pm1 : tableau  contenant les coefficients des polynomes,
c           le coefficient de degre k du polynome pm1(i,j) est range
c           dans pm1( d1(i + (j-1)*ld1 + k) )
c           pm1 doit etre de taille au moins d1(ld1*n+1)-d1(1)
c     d1 : tableau entier de taille ld1*n+1,  si k=i+(j-1)*ld1 alors
c          d1(k)) contient  l'adresse dans pm1 du coeff de degre 0
c          du polynome pm1(i,j). Le degre du polynome pm1(i,j) vaut:
c          d1(k+1)-d1(k) -1
c     ld1 : entier definissant le rangement dans d1
c
c     pm2,d2 : definitions similaires a celles de pm1,d1, ld2
c                 est suppose  egal a n
c     m : nombre de ligne de la matrice pm1
c     n : nombre de colonne de matrice pm1
c!
      double precision pm1r(*),pm1i(*),pm2r(*),pm2i(*)
      integer d1(*),d2(*),m,n
      d2(1)=1
      i2=1
      do 20 i=1,m
      i1=i
      do 10 j=1,n
      l1=d1(i1)
      n1=d1(i1+1)-l1
      l2=d2(i2)
      call dcopy(n1,pm1r(l1),1,pm2r(l2),1)
      call dcopy(n1,pm1i(l1),1,pm2i(l2),1)
      i1=i1+ld1
      i2=i2+1
      d2(i2)=l2+n1
   10 continue
   20 continue
c      call dscal(d2(1+m*n)-1,-1.0d+0,pm2i,1)
c
      return
      end
