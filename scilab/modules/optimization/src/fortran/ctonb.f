      subroutine ctonb (n,u,v,izs,rzs,dzs)
c     Copyright INRIA
      integer n,izs(1)
      real rzs(1)
      double precision u(1),v(1),dzs(1)
      do 1 i=1,n
          v(i)=u(i)
 1    continue
      return
      end
