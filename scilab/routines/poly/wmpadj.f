      subroutine wmpadj(pm1r,pm1i,d1,m,n)
c!but
c     cette subroutine determine les degres effectifs (en eliminant les
c     monomes nuls de plus haut degre ) des polynomes d'une  matrice de
c     polynome a coefficinets compexes
c!liste d'appel
c
c     subroutine wmpadj(pm1r,pm1i,d1,m,n)
c
c     pm1(r,i) : tableaux reel contenant les parties reelle et complexes
c                des coefficients des polynomes,
c                le coefficient de degre k du polynome pm1(i,j) est range
c                dans pm1( d1(i + (j-1)*l1 + k) )
c                pm1 doit etre de taille au moins d1(l1*n+1)-d1(1)
c     d1 : tableau entier de taille l1*n+1,  si k=i+(j-1)*l1 alors
c        d1(k)) contient  l'adresse dans pm1 du coeff de degre 0
c        du polynome pm1(i,j). Le degre du polynome pm1(i,j) vaut:
c        d1(k+1)-d1(k) -1
c     Pm1 et d1 sont modifies par l'execution
c
c     m : nombre de ligne des matrices pm
c     n : nombre de colonnes des matrices pm
c!origine
c     s Steer INRIA
c!
c     Copyright INRIA
      double precision pm1r(*),pm1i(*)
      integer d1(*),m,n,dj
c
      integer n1
c
      kk=1
      dj=1
c     boucle sur les polynomes
      do 40 j=1,m*n
         k1=dj-1
         n1=d1(j+1)-dj+1
 10      n1=n1-1
         if(abs(pm1r(k1+n1))+abs(pm1i(k1+n1)).eq.0.0d+0
     $        .and.n1.gt.1) goto 10
         if(kk.ne.k1+1.and.n1.gt.0) then
            do 11 i=1,n1
               pm1r(kk-1+i)=pm1r(k1+i)
               pm1i(kk-1+i)=pm1i(k1+i)
 11         continue
         endif
         kk=kk+n1
         dj=d1(j+1)
         d1(j+1)=kk
 40   continue
c
      return
      end
