      integer function spord(r11,r12,r21,r22)
c!purpose
c     test function for dspsort wspsort et lspsort
c!
c     Copyright INRIA
      integer r11,r12,r21,r22,r
c     returns
c                1 :if record r1 is greater than r2
c               -1 :if record r1 is less than r2
c                0 :if record r1 is equal to r2
      if(r11.gt.r21) then
         r=1
      elseif(r11.lt.r21) then
         r=-1
      elseif(r12.gt.r22) then
         r=1
      elseif(r12.lt.r22) then
         r=-1
      else
         r=0
      endif
      spord=-r
      return
      end
