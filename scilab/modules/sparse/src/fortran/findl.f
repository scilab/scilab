c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      integer function findl(l,i,ni)

      integer i(ni)
      ii=0
      if(ni.gt.0) then
         do 11 k=1,ni
            if(i(k).ne.l) goto 11
            ii=k
 11      continue
      endif
      findl=ii
      end
