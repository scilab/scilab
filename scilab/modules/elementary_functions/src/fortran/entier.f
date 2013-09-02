c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
C/MEMBR ADD NAME=ENTIER,SSI=0
c
      subroutine entier(n,d,s)
c
      double precision d(*)
      integer s(*)
      do 10 i=1,n
      s(i)=int(d(i))
   10 continue
      return
      end
