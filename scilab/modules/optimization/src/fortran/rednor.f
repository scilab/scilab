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
      function rednor(n,binf,bsup,x,epsx,g)
c
      implicit double precision (a-h,o-z)
      dimension binf(n),bsup(n),x(n),epsx(n),g(n)
      rednor=0.0d+0
      do 1 i=1,n
      aa=g(i)
      if(x(i)-binf(i).le.epsx(i))aa=min(0.0d+0,aa)
      if(bsup(i)-x(i).le.epsx(i))aa=max(0.0d+0,aa)
1     rednor=rednor + aa**2
      rednor=sqrt(rednor)
      end
