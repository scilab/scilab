      subroutine calsca(ns,ts,tr,y0,tg,ng)
c!but
c     Calcule le produit scalaire entre une fonction de Hardi donnee
c     par ses coefficients de fourrier et une fonction rationnelle r/s
c!liste d'appel
c     subroutine calsca(ns,ts,tr,y0)
c     Entrees :
c     ng. est le plus grand indice (compte negativement) des
c         coefficients de fourrier de la fonction de Hardi u
c     tg. vecteur des coefficients de fourrier
c     ns. est le degre du denominateur (polynome monique)
c     ts. est le tableau des coefficients du denominateur
c     tr. est le tableau des coefficients du numerateur dont
c         le degre est inferieur a ns
c
c     sortie  : y0. contient la valeur du produit scalaire recherche.
c!
c     Copyright INRIA
      implicit double precision (a-h,o-z)
      dimension ts(0:ns),tr(0:ns),x(0:40)
      dimension tg(0:ng)
c
      nu=ng+1
      do 20 i=0,ns-1
         x(i)=0.0d+0
 20   continue
      aux= x(ns-1)
      do 30 k=nu,1,-1
         do 29 i=ns-1,1,-1
            x(i)= x(i-1) - ts(i)*aux + tr(i)*tg(k-1)
 29      continue
         x(0)= -ts(0)*aux + tr(0)*tg(k-1)
         aux=x(ns-1)
 30   continue
      y0= x(ns-1)
      return
      end
