      subroutine drdiv(a,ia,b,ib,r,ir,n,ierr)
c!    purpose
c     computes r=a./b with a and b real   
c     
c     ia,ib,ir : increment between two consecutive element of vectors a
c                b and r
c     n        : vectors length
c     ierr     : returned error flag:
c                o   : ok
c                <>0 : b(ierr)=0
c!
c     Copyright INRIA
      double precision a(*),b(*),r(*)
      integer ia,ib,ir,n
      jr=1
      jb=1
      ja=1
      k=0
      ierr=0
      if (ia.eq.0) then
         do 10 k=1,n
            if(b(jb).eq.0.0d0) then
               ierr=k
               return
            endif
            r(jr)=a(ja)/b(jb)
            jr=jr+ir
            jb=jb+ib
 10      continue
      elseif(ib.eq.0) then
         if(b(jb).eq.0.0d0) then
            ierr=1
            return
         endif
         do 11 k=1,n
            r(jr)=a(ja)/b(jb)
            jr=jr+ir
            ja=ja+ia
 11      continue
      else
         do 12 k=1,n
            if(b(jb).eq.0.0d0) then
               ierr=k
               return
            endif
            r(jr)=a(ja)/b(jb)
            jr=jr+ir
            jb=jb+ib
            ja=ja+ia
 12      continue
      endif
      end
