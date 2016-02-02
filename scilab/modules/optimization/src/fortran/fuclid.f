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
      subroutine fuclid (n,x,y,ps,izs,rzs,dzs)
c
      implicit double precision (a-h,o-z)
      dimension x(n),y(n),izs(*),dzs(*)
      real rzs(*)
      ps=0.d0
      do 10 i=1,n
   10 ps=ps+x(i)*y(i)
      return
      end
