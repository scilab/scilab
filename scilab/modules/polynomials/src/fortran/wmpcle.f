      subroutine wmpcle(pm1r,pm1i,d1,m,n,d2,epsr,epsa)
c!but
c     Dans une matrice polynomiale pm1 cette routine met (pour 
c     chaque polynome) a zero les coefficients de polynomes 
c     de module inferieur a epsa ou epsr*(norme un du vecteur
c     des coefficients du polynome)
c
c!liste d'appel
c
c    subroutine wmpclea(pm1r,pm1i,d1,m,n,d2,epsr,epsa)
c    double precision pm1r(*),pm1i(*)
c    integer d1(*),m,n
c
c     pm1r : tableau reel contenant les coefficients des polynomes,
c           parties relles
c           le coefficient de degre k du polynome pm1(i,j) est range
c           dans pm1( d1(i + (j-1)*ld1 + k) )
c           pm1 doit etre de taille au moins d1(ld1*n+1)-d1(1)
c     pm1i : tableau reel contenant les coefficients des polynomes,
c           parties imaginaires
c           le coefficient de degre k du polynome pm1(i,j) est range
c           dans pm1( d1(i + (j-1)*ld1 + k) )
c           pm1 doit etre de taille au moins d1(ld1*n+1)-d1(1)
c     d1 : tableau entier de taille ld1*n+1,  si k=i+(j-1)*ld1 alors
c          d1(k)) contient  l'adresse dans pm1 du coeff de degre 0
c          du polynome pm1(i,j). Le degre du polynome pm1(i,j) vaut:
c          d1(k+1)-d1(k) -1
c     m : nombre de ligne de la matrice pm1
c     n : nombre de colonne de matrice pm1
c     d2 : cf d1
c!origine
c    s Steer INRIA
c!
c     Copyright INRIA
      double precision pm1r(*),pm1i(*),norm,normr,normi,epsr,epsa,eps
      integer d1(*),d2(*),m,n
c
c
c
      mn=m*n
      do 1 k=1,mn
         lmin=d1(k)
         lmax=d1(k+1)-1
         normr=0.d0
         normi=0.d0
         do 2 l=lmin,lmax
            normr=normr+abs(pm1r(l))
            normi=normi+abs(pm1i(l))
            norm=normr+normi
 2       continue
         eps=max(epsa,epsr*norm)
         do 3 l=lmin,lmax
            if (abs(pm1r(l)).le.eps) then
               pm1r(l)=0.0d0
            endif
            if (abs(pm1i(l)).le.eps) then
               pm1i(l)=0.0d0
            endif
 3       continue
 1    continue
      return
      end
