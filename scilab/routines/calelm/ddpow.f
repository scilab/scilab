      subroutine ddpow(n,vr,vi,iv,dpow,ierr,iscmpl)
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
      if(dble(int(dpow)).ne.dpow) goto 01
c puissance entiere
      call dipow(n,vr,iv,int(dpow),ierr)
      return
c
   01 continue
      ii=1
      do 20 i=1,n
         if(vr(ii).gt.0.0d+0) then
            vr(ii)=vr(ii)**dpow
            vi(ii)=0.0d0
         elseif(vr(ii).lt.0.0d+0) then
            call wlog(vr(ii),0.0d0,sr,si)
            sr=exp(sr*dpow)
            si=si*dpow
            vr(ii)=sr*cos(si)
            vi(ii)=sr*sin(si)
            iscmpl=1
         else
            if(dpow.lt.0.0d+0) then
               ierr=2
               return
            elseif(dpow.eq.0.0d+0) then
               ierr=1
               return
            else
               vr(ii)=0.0d0
               vi(ii)=0.0d0
            endif
         endif
         ii=ii+iv
 20   continue
c
      return
      end
