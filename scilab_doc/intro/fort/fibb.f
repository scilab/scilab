      subroutine fibb(fn,n)

c     Copyright INRIA
      n0=0
      n1=1
      if(n.ge.3)then
      do 10 k=1,n-2
         fn=n0+n1
         n0=n1
         n1=fn
 10   continue
      else
         fn=n-1
      endif
      return
      end
