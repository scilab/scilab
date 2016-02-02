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
c
      subroutine proj(n,binf,bsup,x)
      implicit double precision (a-h,o-z)
      dimension binf(n),bsup(n),x(n)
      do 1 i=1,n
1     x(i)=max(binf(i),min(x(i),bsup(i)))
      return
      end
