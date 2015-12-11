c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine  vpythag(n,xr,xi)
c
c     xr(i) = pythag(xr(i),xi(i))
      double precision xr(*),xi(*),pythag
      integer n
      do 10 i=1,n
         xr(i) = pythag(xr(i),xi(i))
 10   continue
      return
      end


