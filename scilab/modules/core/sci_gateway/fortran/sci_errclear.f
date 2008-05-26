c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine interrclear
      include 'stack.h'
      logical checkrhs,checklhs
c
      rhs=max(0,rhs)
      if(.not.checklhs('errclear',1,1)) return
      if(.not.checkrhs('errclear',0,1)) return

      err2=0
      call freemsgtable()
      if(rhs.eq.1) top=top-1
      top=top+1
      call objvide('errclear',top)
      return
      end
