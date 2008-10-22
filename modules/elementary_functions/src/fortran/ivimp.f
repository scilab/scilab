c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
      subroutine ivimp(i1,i2,pas,iv)
c     generate iv=i1:pas:i2
c
      integer i1,i2,pas,iv(*)
      k=0
      do 10 i=i1,i2,pas
         k=k+1
         iv(k)=i
 10   continue
      end
