      subroutine fcalc(str,a,n)
      character*(*) str
      integer a 
c
c     
      if (str(1:n).eq.'one') then
         a=1
      elseif (str(1:n).eq.'two') then
         a=2
      else
         a=-1
      endif
      end
