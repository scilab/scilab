c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c ...
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
c     Example for int3d
c     ---------------------
c      
      subroutine int3dex(x,numfun,funvls)
      integer numfun
      double precision x(3),funvls(numfun)
      funvls(1) = exp(x(1)*x(1)+x(2)*x(2)+x(3)*x(3))
      return
      end




