      subroutine dmcopy(a,na,b,nb,m,n)
c!but
c     ce sous programme effectue:b=a
c     avec a matrice m lignes et n colonnes
c     mcopy utilise un code particulier si les matrices sont
c     compactes
c!liste d'appel
c
c     subroutine dmcopy(a,na,b,nb,m,n)
c     double precision a(na,n),b(nb,m)
c     integer na,nb,m,n
c
c     a         tableau contenant la matrice a
c     na        nombre de lignes du tableau a dans le prog appelant
c     b,nb      definition similaires a :a,na
c     m         nombre de lignes des matrices a et b
c     n         nombre de colonnes des matrices a et b
c!sous programmes utilises
c     neant
c!
c     Copyright INRIA
      double precision a(*),b(*)
      integer na,nb,m,n
      integer ia,ib,i,j,mn
c
      if(na.eq.m .and. nb.eq.m) goto 20
      ia=-na
      ib=-nb
      do 10 j=1,n
      ia=ia+na
      ib=ib+nb
      do 10 i=1,m
      b(ib+i)=a(ia+i)
   10 continue
      return
   20 continue
c code pour des matrices compactes
      mn=m*n
      do 30 i=1,mn
      b(i)=a(i)
   30 continue
      return
      end
