      subroutine mpddpow(n,vr,vi,iv,dpow,ierr,iscmpl)
c!but
c     eleve a une puissance reelle les elements d'un vecteur de flottants
c     double precision
c!liste d'appel
c     subroutine ddpow(n,v,iv,dpow,ierr)
c     integer n,iv,ierr
c     double precision v(n*iv),dpow
c
c     n : nombre d'element du vecteur
c     vr : tableau contenant en entree les elements du vecteur et en 
c          sortie les parties reelles du resultat
c     vi : tableau contenant en sortie les parties imaginaire (eventuelles)
c          du resultat
c     iv : increment entre deux element consecutif du vecteur dans le
c          tableau v
c     dpow : puissance a la quelle doivent etre eleves les elements du
c            vecteur
c     ierr : indicateur d'erreur
c            ierr=0 si ok
c            ierr=1 si 0**0
c            ierr=2 si 0**k avec k<0
c     iscmpl :
c            iscmpl=0 resultat reel
c            iscmpl=1 resultat complexe
c!origine
c Serge Steer INRIA 1989
c!
c     Copyright INRIA
      integer n,iv,ierr
      double precision vr(*),vi(*),dpow,sr,si
c
      ierr=0
      iscmpl=0
c
c      if(dble(int(dpow)).ne.dpow) goto 01
c puissance entiere
      call mpdipow(n,vr,iv,dpow,ierr)
      return
c
      end
