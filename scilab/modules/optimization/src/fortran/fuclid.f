      subroutine fuclid (n,x,y,ps,izs,rzs,dzs)
c     Copyright INRIA
      implicit double precision (a-h,o-z)
      dimension x(n),y(n),izs(*),dzs(*)
      real rzs(*)
      ps=0.d0
      do 10 i=1,n
   10 ps=ps+x(i)*y(i)
      return
      end
