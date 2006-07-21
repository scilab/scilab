c     Exemple for corr
c     ----------------
c     Copyright INRIA
      
      subroutine corexx(x, incr, istart)
      integer incr,istart
      double precision x(incr)
      do 10 i = istart,istart+incr-1
         x(i-istart+1)= sin(dble(i))
 10   continue
      return
      end

      
      subroutine corexy(y, incr, istart)
      integer incr,istart
      double precision y(incr)
      do 10 i = istart,istart+incr-1
         y(i-istart+1)= cos(dble(i))
 10   continue
      return
      end

