c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine proj(n,binf,bsup,x)
      implicit double precision (a-h,o-z)
      dimension binf(n),bsup(n),x(n)
      do 1 i=1,n
1     x(i)=max(binf(i),min(x(i),bsup(i)))
      return
      end
