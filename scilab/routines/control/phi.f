      double precision function phi(tq,nq,tg,ng,w)
C%but
C calcule la fonction phi
c%liste d'appel
c     Entree :
c        tg . tableau des coefficients de la fonction g .
c        ng . degre du polynome g
c        tq . tableau des coefficients du polynome q
c        nq . degre du polynome q
c        w  . tableau de travail de taille nq+ng+1
c     Sortie :
c        phi
c%
c     Copyright INRIA
      implicit double precision (a-h,o-y)

      dimension tq(nq+1),tg(ng+1),w(nq+ng+1)
c
      ltr=1
      lfree=ltr+nq+ng+1
      call lq(nq,tq,w(ltr),tg,ng)
C
      ltlq=ltr
      call calsca(nq,tq,w(ltlq),y0,tg,ng)
C
      phi = 1.0d+0 - y0
C
      return
      end


