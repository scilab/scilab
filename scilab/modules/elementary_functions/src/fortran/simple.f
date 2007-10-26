      subroutine simple(n,d,s)
c!
c     Copyright INRIA
      double precision d(*)
      real s(*),rmax,slamch
c
      rmax=slamch('o')
c
      do 10 i=1,n
      if(abs(d(i)).gt.rmax) then
        s(i)=real(sign(dble(rmax),d(i)))
      else
        s(i)=real(d(i))
      endif
   10 continue
      return
      end
