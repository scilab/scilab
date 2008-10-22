c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      
      subroutine corexx(x, incr, istart)
      integer incr,istart
      double precision x(incr)
      do 10 i = istart,istart+incr-1
         x(i-istart+1)= sin(dble(i))
 10   continue
      return
      end

      
      subroutine corexy(y, incr, istart)
      integer incr,istart
      double precision y(incr)
      do 10 i = istart,istart+incr-1
         y(i-istart+1)= cos(dble(i))
 10   continue
      return
      end

