      subroutine lful2sp(ma,na,a,nel,indr)
c     converts a full matrix to a sparse representation
c     Copyright INRIA
      integer a(ma,na)
      integer indr(*)
c
      nel=0
      do 5 i=1,ma
         ni=0
         do 4 j=1,na
            if(a(i,j).ne.0) then
               nel=nel+1
               indr(ma+nel)=j
               ni=ni+1
            endif
 4       continue
         indr(i)=ni
 5    continue
      end
