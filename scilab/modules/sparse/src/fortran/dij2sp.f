      subroutine dij2sp(m,n,nel,ij,v,ind,nind,iw,ierr)
c!purpose 
c     generates a sparse matrix from (i,j), v description
c!parameters
c     ierr=0  : ok
c     ierr=1  : almost on index in ij is greater than corresponding
c               dimension m or n 
c     ierr=2  : not enough memory for ind
c     ierr=3  : doubly defined entry with different value
c!
c     Copyright INRIA
      double precision v(nel)
      integer ij(nel,2),ind(nind),iw(nel)
c
      ierr=0

      if(nel.eq.0) then
         call iset(m,0,ind,1)
         return
      endif
c     sort indexes 
      call spsort(ij,nel,iw)
      call dperm(v,nel,iw)
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
      if (v(k0).eq.0.0d0.and.k0.lt.nel)  goto 05
      if (v(k0).eq.0.0d0) then
         nel1=0
         goto 10
      endif

      k1=1
      ij(k1,1)=ij(k0,1)
      ij(k1,2)=ij(k0,2)
      v(k1)=v(k0)
      if(nel.gt.k0) then
      do 08 k=k0+1,nel
         if (v(k).ne.0.0d0) then
            if(ij(k,1).ne.ij(k1,1).or.ij(k,2).ne.ij(k1,2)) then
               k1=k1+1
               ij(k1,1)=ij(k,1)
               ij(k1,2)=ij(k,2)
               v(k1)=v(k)
            else
               v(k1) = v(k1) + v(k)
c               if(v(k1).ne.v(k)) then
c                  ierr=3
c                  return
c               endif
            endif
         endif
 08      continue
      endif
      nel1=k1
      

c     check dimensions
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
c     check check memory
      if(nind.lt.m+nel1) then
         ierr=2
         return
      endif
c     compute nl the number of non zero entries for each row
      i0=1
      do 20 lp=1,m
         i=i0-1
 21      i=i+1
         if(i.le.nel1) then
            if(ij(i,1).eq.lp) goto 21
         endif
         nl=i-i0
         ind(lp)=nl
         i0=i
 20   continue
      call icopy(nel1,ij(1,2),1,ind(m+1),1)
      nel=nel1
      end
      

