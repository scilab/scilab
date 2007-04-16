c     ==================================== 
      subroutine arset (aa1)
c     ==================================== 
      double precision aa,aa1(6)
      common / aac / aa(6)
      do 10 i=1,6
         aa(i)=aa1(i)
 10      continue
      return 
      end 
c     ==================================== 