      subroutine wdrdiv(ar,ai,ia,br,ib,rr,ri,ir,n,ierr)
c!    purpose
c     computes r=a./b with a real vector and b complex vector
c     
c     ia,ib,ir : increment between two consecutive element of vectors a
c                b and r
c     ar,ai    : arrays containing a real and imaginary parts
c     br       : array  containing b vector
c     rr,ri    : arrays containing r real and imaginary parts
c     n        : vectors length
c     ierr     : returned error flag:
c                o   : ok
c                <>0 : b(ierr)=0
c!
c     Copyright INRIA
      double precision ar(*),ai(*),br(*),rr(*),ri(*)
      integer ia,ib,ir,n
c     wr, wi used because rr, ri may share same mem as ar,ai or br,bi
      double precision wr,wi
      jr=1
      jb=1
      ja=1
      ierr=0
      if (ia.eq.0) then
         do 10 k=1,n
            call wddiv(ar(ja),ai(ja),br(jb),wr,wi,ierr1)
            rr(jr)=wr
            ri(jr)=wi
            if(ierr1.ne.0) then
               ierr=k
c               return
            endif
            jr=jr+ir
            jb=jb+ib
 10      continue
      elseif(ib.eq.0) then
         if(br(jb).eq.0.0d0) then
            ierr=1
c            return
         endif
         do 11 k=1,n
            call wddiv(ar(ja),ai(ja),br(jb),wr,wi,ierr1)
            rr(jr)=wr
            ri(jr)=wi
            jr=jr+ir
            ja=ja+ia
 11      continue
      else
         do 12 k=1,n
            call wddiv(ar(ja),ai(ja),br(jb),wr,wi,ierr1)
            rr(jr)=wr
            ri(jr)=wi
            if(ierr1.ne.0) then
               ierr=k
c               return
            endif
            jr=jr+ir
            jb=jb+ib
            ja=ja+ia
 12      continue
      endif
      end
