c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
      
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

