C/MEMBR ADD NAME=DDIF,SSI=0
c     Copyright INRIA
      subroutine ddif(n,a,na,b,nb)
c!but
c     ddif effectue l'operation vectorielle b=b-a
c!liste d'appel
c     subroutine ddif(n,a,na,b,nb)
c     double precision a(*),b(*)
c     integer n,na,nb
c
c     n : nombre d'elements des vecteurs a et b
c     a : tableau contenant a
c     na : increment entre deux elements consecutifs de a
c          na > 0
c     b,nb : definitions similaires a celles de a et na
c!
      double precision a(*),b(*)
      ia=1
      ib=1
      do 10 i=1,n
      b(ib)=b(ib)-a(ia)
      ia=ia+na
      ib=ib+nb
   10 continue
      return
      end
