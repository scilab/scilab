C/MEMBR ADD NAME=ENTIER,SSI=0
c     Copyright INRIA
      subroutine entier(n,d,s)
c!
      double precision d(*)
      integer s(*)
      do 10 i=1,n
      s(i)=int(d(i))
   10 continue
      return
      end
