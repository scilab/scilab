      subroutine dwpow(n,vr,vi,iv,powr,powi,ierr)
c!but
c     eleve les elements d'un vecteur reel a une puissance complexe
c!liste d'appel
c     subroutine dwpow(n,vr,vi,iv,powr,powi,ierr)
c     integer n,iv,ierr
c     double precision vr(n*iv),vi(n*iv),powr,powi
c
c     n : nombre d'elements du vecteur
c     vr : tableau contenant les  elements du vecteur
c     vi : tableau contenant en retour les parties imaginaires du resultat
c     iv : increment entre deux element consecutif du vecteur dans le
c          tableau v
c     powr : partie reelle de la puissance a la quelle doivent etre
c            eleves les elements du vecteur
c     powi : partie imaginaire de la puissance a la quelle doivent etre
c            eleves les elements du vecteur
c     ierr : indicateur d'erreur
c            ierr=0 si ok
c            ierr=1 si 0**0
c            ierr=2 si 0**k avec k<0
c!origine
c Serge Steer INRIA 1989
c!
c     Copyright INRIA
      integer n,iv,ierr
      double precision vr(*),vi(*),powr,powi,sr,si
c     
      ierr=0
c     
      if(powi.ne.0.0d+0) goto 01
c     puissance reelle
      call ddpow(n,vr,vi,iv,powr,ierr,iscmpl)
      return
c     
 01   continue
c     puissance complexes
      ii=1
      do 20 i=1,n
         if(vr(ii).ne.0.0d+0) then
            sr=vr(ii)**powr
            si=log(vr(ii))*powi
            vr(ii)=sr*cos(si)
            vi(ii)=sr*sin(si)
            ii=ii+iv
         else
            if(powr.gt.0.0d+0) then
               vr(ii)=0.0d+0
               vi(ii)=0.0d+0
               ii=ii+iv
            else
               ierr=2
               return
            endif
         endif
 20   continue
c     
      return
      end
