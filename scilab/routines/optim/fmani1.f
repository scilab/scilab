      subroutine fmani1 (mode,n,d,w,indi)
c     Copyright INRIA
      implicit double precision (a-h,o-z)
      dimension d(n),w(n),indi(n)
c
      if(mode.eq.-1) go to 20
      do 10 i=1,n
   10 w(indi(i))=d(i)
      return
   20 do 30 i=1,n
   30 w(i)=d(indi(i))
      return
      end
