c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine ctcab (n,u,v,izs,rzs,dzs)
c

      integer n,izs(1)
      real rzs(1)
      double precision u(1),v(1),dzs(1)
      do 1 i=1,n
          v(i)=u(i)
 1    continue
      return
      end

