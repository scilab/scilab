      subroutine fcalc(str,a)
      character*(*) str
      integer a 
c
      if (str.eq.'one') then
         a=1
      elseif (str.eq.'two') then
         a=2
      else
         a=-1
      endif
      end

