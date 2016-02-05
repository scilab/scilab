c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
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
