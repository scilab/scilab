      subroutine ddmpev(mp1,d1,nl1,x,v,iv,m,n)
c!purpose
c  evaluates a real polynomail matrix at a real point
c! calling sequence
c
c     mp1 : tableau contenant les coefficients des polynomes,
c           le coefficient de degre k du polynome mp1(i,j) est range
c           dans mp1( d1(i + (j-1)*nl1 + k) )
c           mp1 doit etre de taille au moins d1(nl1*n+1)-d1(1)
c     d1 : tableau entier de taille nl1*n+1,  si k=i+(j-1)*nl1 alors
c          d1(k)) contient  l'adresse dans mp1 du coeff de degre 0
c          du polynome mp1(i,j). Le degre du polynome mp1(i,j) vaut:
c          d1(k+1)-d1(k) -1
c     nl1 : entier definissant le rangement dans d1
c     x   : valeur du point pour lequel on de amde l'evaluation
c     v   : tableau resultat
c     iv  : entier definissant le rangement dans v
c     m   : nombre de ligne de la matrice de polynome
c     n   : nombre de colonne de la matrice de polynome
c!
c     Copyright INRIA
      double precision mp1(*),x,v(iv,*)
      integer d1(*),nl1,iv,m,n,dp
c

      do 20 j=1,n
         do 10 i=1,m
            k=i+(j-1)*nl1
            dp=d1(k+1)-d1(k)-1
            lp=d1(k)
            ip=lp+dp
            v(i,j)=mp1(ip)
            if(dp.eq.0) goto 10
            do 01 l=1,dp
               v(i,j)=v(i,j)*x+mp1(ip-l)
 01         continue
 10      continue
 20   continue
      return
      end
