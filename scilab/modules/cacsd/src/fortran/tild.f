C/MEMBR ADD NAME=TILD,SSI=0
c     Copyright INRIA
      subroutine tild(n,tp,tpti)
c!but
c     pour un polynome p(z)  l'operation tild aboutit a un polynome
c     ptild(z) defini par la relation suivante :
c       ptild(z)= z**n * p(1/z) .
c!liste d'appel
c     Entree : - tp . vecteur des coefficients du polynome a "tilder" .
c              -  n . degre du polynome "tp"
c
c     Sortie : - tpti . vecteur des coefficients du polynome resultant .
c
c!
      implicit double precision (a-h,o-y)
      dimension tp(0:*),tpti(0:*)
c
      do 50 j=0,n
         tpti(j)=tp(n-j)
 50   continue
      return
      end
