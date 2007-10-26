      subroutine wipow(n,vr,vi,iv,ipow,ierr)
c!but
c     eleve a une puissance entiere les elements d'un vecteur de flottants
c     complexes
c!liste d'appel
c     subroutine wipow(n,vr,vi,iv,ipow,ierr)
c     integer n,iv,ipow ,ierr
c     double precision vr(n*iv),vi(n*iw)
c
c     n : nombre d'element du vecteur
c     vr : tableau contenant les parties reelles des elements du vecteur
c     vi : tableau contenant les parties imaginaires des elements du vecteur
c     iv : increment entre deux element consecutif du vecteur dans le
c          tableau v
c     ipow : puissance a la quelle doivent etre eleves les elements du
c            vecteur
c     ierr : indicateur d'erreur
c            ierr=0 si ok
c            ierr=1 si 0**0
c            ierr=2 si 0**k avec k<0
c!origine
c Serge Steer INRIA 1989
c!
c     Copyright INRIA
      integer n,iv,ipow ,ierr
      double precision vr(*),vi(*),xr,xi
c
      ierr=0
c
      if(ipow.eq.1) return
      if(ipow.eq.0) then
c puissance 0
      ii=1
      do 10 i=1,n
        if(abs(vr(ii))+abs(vi(ii)).ne.0.0d+0) then
                          vr(ii)=1.0d+0
                          vi(ii)=0.0d+0
                          ii=ii+iv
                      else
                          ierr=1
                          return
        endif
   10 continue
      return
c
      else if(ipow.lt.0) then
c puissance negative
      ii=1
      do 20 i=1,n
        if(abs(vr(ii))+abs(vi(ii)).ne.0.0d+0) then
                   call wdiv(1.0d+0,0.0d+0,vr(ii),vi(ii),vr(ii),vi(ii))
                   ii=ii+iv
                                             else
                   ierr=2
                    return
        endif
   20 continue
      if(ipow.eq.-1) return
      endif
c
c puissance  positive et fin puissance negatives
      ii=1
      do 30 i=1,n
         xr=vr(ii)
         xi=vi(ii)
         do 31 k=2,abs(ipow)
              call wmul(xr,xi,vr(ii),vi(ii),vr(ii),vi(ii))
   31    continue
         ii=ii+iv
   30 continue
c
      return
      end
