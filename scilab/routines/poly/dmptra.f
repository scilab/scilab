C/MEMBR ADD NAME=DMPTRA,SSI=0
c     Copyright INRIA
      subroutine dmptra(pm1,d1,ld1,pm2,d2,m,n)
c!but
c     cette subroutine transpose une matrice dont les coefficients sont
c     des polynomes.
c!liste d'appel
c
c    subroutine dmptra(pm1,d1,ld1,pm2,d2,m,n)
c    double precision pm1(*),pm2(*)
c    integer d1(*),d2(n,m),m,n,iw(n)
c
c     pm1 : tableau reel contenant les coefficients des polynomes,
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
c!origine
c    s Steer INRIA
c!
      double precision pm1(*),pm2(*)
      integer d1(*),d2(*),m,n
      d2(1)=1
      i2=1
      do 20 i=1,m
      i1=i
      do 10 j=1,n
      l1=d1(i1)
      n1=d1(i1+1)-l1
      l2=d2(i2)
      call dcopy(n1,pm1(l1),1,pm2(l2),1)
      i1=i1+ld1
      i2=i2+1
      d2(i2)=l2+n1
   10 continue
   20 continue
c
      return
      end
