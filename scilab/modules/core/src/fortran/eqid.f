c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      logical function eqid(x,y)
c
c     check for equality of two names
c
      include 'stack.h'
c
      integer x(nsiz),y(nsiz)
c
      eqid = .false.
      do 10 i = 1, nsiz
   10 if(x(i).ne.y(i)) return
      eqid=.true.
      return
      end
