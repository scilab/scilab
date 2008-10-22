c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      integer function spord(r11,r12,r21,r22)
c!purpose
c     test function for dspsort wspsort et lspsort
c!
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
