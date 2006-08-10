      integer function findl(l,i,ni)
c     Copyright INRIA
      integer i(ni)
      ii=0
      if(ni.gt.0) then
         do 11 k=1,ni
            if(i(k).ne.l) goto 11
            ii=k
 11      continue
      endif
      findl=ii
      end
