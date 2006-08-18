      subroutine wesidu(pr,pi,np,ar,ai,na,br,bi,nb,vr,vi,tol,ierr)
c     calcul de la somme des residus de p/(a.b)
c     aux zeros de a
c     p=pr+i*pi=polynome de degre np a coefficients complexes
c     a=ar+i*ai                   na
c     b=br+i*bi                   nb
c     les zeros de b sont supposes tous differents des
c     zeros de a....
c     a,b et p dimensionnes au moins a leur degre+1 dans le pgm
c     appelant.
c     rangement par degres croissants.
c     v=vr+i*vi=resultat
c     principe du calcul:si a (ou b) est une constante on a
c     v=p(nb)/b(nb+1)/a(1)
c     sinon on remplace p et a par le reste de la division
c     euclidienne de p et a par b,puis on inverse les roles
c     de a et b en changeant le signe de v.
c     on itere jusqu a trouver degre de a ou degre de b=0.
c     F.D.
c        
c     Copyright INRIA
      dimension ar(*),br(*),pr(*),ai(*),bi(*),pi(*)
      double precision ar,br,pr,vr,rr,ai,bi,pi,vi,ri,tol,b1
      vr=0.0d+0
      vi=0.0d+0
      npp=np
      call wdegre(ar,ai,na,na)
      call wdegre(br,bi,nb,nb)
      if(na.eq.0) return
      if (nb.eq.0) then
         b1=abs(br(1)+bi(1))
         if(b1.eq.0.0d+0) then
            ierr=0
            return
         endif
         if(npp.ge.na-1) then
            call wdiv(pr(na),pi(na),ar(na+1),ai(na+1),vr,vi)
            call wdiv(vr,vi,br(1),bi(1),vr,vi)
            return
         else
            vr=0.0d+0
            vi=0.0d+0
            return
         endif
      endif
      if(na.gt.np) goto 11
c     p=p/a  (reste de la division euclidienne...)
      call wpodiv(pr,pi,ar,ai,np,na,ierr)
      if(ierr.ne.0) then
         return
      endif
      call wdegre(pr,pi,na-1,np)
 11   continue
      if(na.gt.nb) goto 31
c     b=b/a  (reste de la div euclidienne...)
      call wpodiv(br,bi,ar,ai,nb,na,ierr)
      if(ierr.ne.0) then
         return
      endif
      call wdegre(br,bi,na-1,nb)
 31   continue
      if(na.eq.1) then
c     v=p(na)/a(na+1)/b(1)
         b1=abs(br(1))+abs(bi(1))
         if(b1.le.tol) then
            ierr=1
            return
         endif
         call wdiv(pr(na),pi(na),ar(na+1),ai(na+1),vr,vi)
         call wdiv(vr,vi,br(1),bi(1),vr,vi)
         return
      endif
      call wdegre(br,bi,min(na-1,nb),nb)
      if(nb.gt.0) goto 32
      b1=abs(br(1))+abs(bi(1))
      if(b1.le.tol) then
         ierr=1
         return
      endif
      if(npp.ge.na-1) then
c     v=p(na)/a(na+1)/b(1)
         call wdiv(pr(na),pi(na),ar(na+1),ai(na+1),vr,vi)
         call wdiv(vr,vi,br(1),bi(1),vr,vi)
         return
      else
         vr=0.0d+0
         vi=0.0d+0
      endif
 32   continue
      nit=0
 20   continue
      if(nit.ge.1) na=nbb
      nit=nit+1
      nbb=nb
      call wpodiv(ar,ai,br,bi,na,nb,ierr)
      if(ierr.ne.0) then
         return
      endif
      call wdegre(ar,ai,nb-1,na)
      call wpodiv(pr,pi,br,bi,np,nb,ierr)
      if(ierr.ne.0) then
         return
      endif
      call wdegre(pr,pi,nb-1,np)
      do 30 k=1,nb+1
         rr=br(k)
         ri=bi(k)
         br(k)=-ar(k)
         bi(k)=-ai(k)
         ar(k)=rr
         ai(k)=ri
 30   continue
      call wdegre(br,bi,na,nb)
      if(nb.eq.0) goto 99
      goto 20
 99   continue
c     v=p(nbb)/a(nbb+1)/b(1)
      b1=abs(br(1))+abs(bi(1))
      if(b1.le.tol) then
         ierr=1
         return
      endif
      call wdiv(pr(nbb),pi(nbb),ar(nbb+1),ai(nbb+1),vr,vi)
      call wdiv(vr,vi,br(1),bi(1),vr,vi)
      return
      end
