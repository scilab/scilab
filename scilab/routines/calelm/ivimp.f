      subroutine ivimp(i1,i2,pas,iv)
c     generate iv=i1:pas:i2
c     Copyright INRIA
      integer i1,i2,pas,iv(*)
      k=0
      do 10 i=i1,i2,pas
         k=k+1
         iv(k)=i
 10   continue
      end
