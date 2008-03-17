      subroutine wij2sp(m,n,nel,ij,vr,vi,ind,nind,iw,ierr)
c     Copyright INRIA
      double precision vr(nel),vi(nel)
      integer ij(nel,2),ind(nind),iw(nel)
c
      ierr=0
      if(nel.eq.0) then
         call iset(m,0,ind,1)
         return
      endif
      call spsort(ij,nel,iw)
      call wperm(vr,vi,nel,iw)
c
      mm=ij(nel,1)
      nm=ij(1,2)
      if(nel.ge.2) then
         do 01 k=2,nel
            nm=max(nm,ij(k,2))
 01      continue     
      endif   

c     eliminate zero entries, check for doubly defined entries
c
c     eliminate leading zero entries
      k0=0
 05   k0=k0+1
      if (vr(k0).eq.0.0d0.and.vi(k0).eq.0.0d0.and.k0.lt.nel) goto 05
      if (vr(k0).eq.0.0d0.and.vi(k0).eq.0.0d0) then
         nel1=0
         goto 10
      endif

      k1=1
      ij(k1,1)=ij(k0,1)
      ij(k1,2)=ij(k0,2)
      vr(k1)=vr(k0)
      vi(k1)=vi(k0)
      if(nel.gt.k0) then
      do 08 k=k0+1,nel
         if (vr(k).ne.0.0d0.or.vi(k).ne.0.0d0) then
            if(ij(k,1).ne.ij(k1,1).or.ij(k,2).ne.ij(k1,2)) then
               k1=k1+1
               ij(k1,1)=ij(k,1)
               ij(k1,2)=ij(k,2)
               vr(k1)=vr(k)
               vi(k1)=vi(k)
            else
               vr(k1) = vr(k1) + vr(k)
               vi(k1) = vi(k1) + vi(k)
c               if(vr(k1).ne.vr(k).or.vi(k1).ne.vi(k)) then
c                  ierr=3
c                  return
c               endif
            endif
         endif
 08      continue
      endif
      nel1=k1
      
 10   continue

      if(n.gt.0) then
         if(n.lt.nm.or.m.lt.mm) then
            ierr=1
            return
         endif
      else
         n=nm
         m=mm
      endif
      if(nind.lt.m+nel1) then
         ierr=2
         return
      endif
c     calcul du nombre d'element non nul par lignes et tri par colonne
c     
      i0=1
      do 20 lp=1,m
         i=i0-1
 21      i=i+1
         if(i.le.nel1) then
            if(ij(i,1).eq.lp) goto 21
         endif
         nl=i-i0
c     nl est le nombre d'element dans la ligne
         ind(lp)=nl
         i0=i
 20   continue
      call icopy(nel1,ij(1,2),1,ind(m+1),1)
      nel=nel1
      end
      
