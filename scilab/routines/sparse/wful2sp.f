      subroutine wful2sp(ma,na,ar,ai,nel,indr,rr,ri,zeror,zeroi)
c     converts a full matrix to a sparse representation
c     Copyright INRIA
      double precision ar(ma,na),ai(ma,na),rr(*),ri(*)
      double precision zeror,zeroi
      integer indr(*)
c
      nel=0
      do 5 i=1,ma
         ni=0
         do 4 j=1,na
            if(ar(i,j).ne.zeror.or.ai(i,j).ne.zeroi) then
               nel=nel+1
               indr(ma+nel)=j
               rr(nel)=ar(i,j)
               ri(nel)=ai(i,j)
               ni=ni+1
            endif
 4       continue
         indr(i)=ni
 5    continue
      end
