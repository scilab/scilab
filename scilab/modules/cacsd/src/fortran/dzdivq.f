      subroutine dzdivq(ichoix,nv,tv,nq,tq)
c!but
c     calcule ici les quotient et reste de la division
c       par q d'un polynome p, a partir des quotient et reste
c       de la division par q du produit de ce polynome par z.
c!liste d'appel
c     subroutine dzdivq(ichoix,nv,tv,nq,tq)
c     Entree :
c     - ichoix. prend la valeur 1 si l'on ne desire que
c       calculer le nouveau quotient (puisqu'il ne se calcule
c       qu'a partir du precedent. 2 sinon.
c     - nv. est le degre du quotient entrant tv.
c     - tv. est le tableau contenant les coeff. du quotient.
c     - tr. est le tableau contenant les coeff. du reste de
c       degre nq-1.
c     - nq. est le degre du polynome tq.
c     - tq. est le tableau contenant les coeff. du pol. tq.
c
c     sortie :
c     - nv. est le degre du nouveau quotient.
c     - tv. contient les coeff. du nouveau quotient.
c     - tr. ceux du nouveau reste de degre toujours nq-1.
c
c     --------------------------
c     Copyright INRIA
      implicit double precision (a-h,o-y)
      dimension tv(0:*),tq(0:*)
c
      vaux=tv(nq)
c
c     -- Calcul du nouveau quotient ---------
c
      do 20 i=nq,nq+nv-1
         tv(i)=tv(i+1)
 20   continue
c
      tv(nq+nv)=0.0d+0
      nv =nv-1
c
      if (ichoix.eq.1) return
c
c     -- calcul du nouveau reste ------------
c
      do 30 i=0,nq-2
         tv(i)= vaux*tq(i+1) +tv(i+1)
 30   continue
c
      tv(nq-1)=vaux
c
      return
      end
