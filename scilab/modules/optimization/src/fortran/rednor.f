c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
