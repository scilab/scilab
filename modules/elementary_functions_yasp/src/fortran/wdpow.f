c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1989 - INRIA - Serge STEER
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
      subroutine wdpow(n,vr,vi,iv,dpow,ierr)
c!but
c     eleve les elements d'un vecteur complexe a une puissance reelle
c!liste d'appel
c     subroutine wdpow(n,vr,vi,iv,dpow,ierr)
c     integer n,iv,ierr
c     double precision vr(n*iv),vi(n*iw),dpow
c
c     n : nombre d'element du vecteur
c     vr : tableau contenant les parties reelles des elements du vecteur
c     vi : tableau contenant les parties imaginaires des elements du vecteur
c     iv : increment entre deux element consecutif du vecteur dans le
c          tableau v
c     dpow :  la puissance a la quelle doivent etre
c            eleves les elements du vecteur
c     ierr : indicateur d'erreur
c            ierr=0 si ok
c            ierr=1 si 0**0
c            ierr=2 si 0**k avec k<0
c!origine
c Serge Steer INRIA 1989
c
      integer n,iv,ierr
      double precision vr(*),vi(*),dpow,sr,si
c
      ierr=0
c
      if(dble(int(dpow)).ne.dpow) goto 01
c     puissance entieres
      call wipow(n,vr,vi,iv,int(dpow),ierr)
      return
c     
 01   continue
c     puissance reelles
      ii=1
      do 20 i=1,n
         if(abs(vr(ii))+abs(vi(ii)).ne.0.0d+0) then
            call wlog(vr(ii),vi(ii),sr,si)
            sr=exp(sr*dpow)
            si=si*dpow
            vr(ii)=sr*cos(si)
            vi(ii)=sr*sin(si)
            ii=ii+iv
         else
            if(dpow.gt.0.0d+0) then
               vr(ii)=0.0d+0
               vi(ii)=0.0d+0
               ii=ii+iv
            else
               ierr=2
            endif
            return
         endif
 20   continue
c     
      return
      end
