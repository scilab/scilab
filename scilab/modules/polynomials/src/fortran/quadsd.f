c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
C/MEMBR ADD NAME=QUADSD,SSI=0
      subroutine quadsd(nn, u, v, p, q, a, b)
c divides p by the quadratic  1,u,v placing the
c quotient in q and the remainder in a,b
      double precision p(nn), q(nn), u, v, a, b, c
      integer i
      b = p(1)
      q(1) = b
      a = p(2) - u*b
      q(2) =a
      do 10 i=3,nn
        c = p(i) - u*a - v*b
        q(i) =c
        b = a
        a = c
   10 continue
      return
      end
