C/MEMBR ADD NAME=SCAPOL,SSI=0
c     Copyright INRIA
      subroutine scapol(na,a,nb,b,y)
c!but
c     cette subroutine a pour but de calculer le produit
c     scalaire de deux polynomes
c!liste d'appel
c     subroutine scapol(na,a,nb,b,y)
c     Entree :
c      a. est le premier polynome de degre na
c      b. est le second polynome du produit, et est de degre nb
c
c     Sortie :
c      y. est le resultat du produit scalaire <a,b>
c!
      implicit double precision (a-h,o-y)
      dimension a(0:*),b(0:*)
c
      if (na.ge.nb) then
         nmax=nb
      else
         nmax=na
      endif
c
      aux=0.0d+0
      do 20 k=0,nmax
         aux=aux + a(k)*b(k)
 20   continue
      y=aux
c
      end
