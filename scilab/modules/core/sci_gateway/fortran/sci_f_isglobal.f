c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine intisglobal(fname)
      include 'stack.h'
      logical checkrhs,checklhs,crebmat
      character*(*) fname
c
      if(rhs.le.0) then
         call error(39)
         return
      endif
      if(.not.checklhs('isglobal',1,1)) return
      if(.not.checkrhs('isglobal',1,1)) return

      if(.not.crebmat(fname,top,1,1,ilr)) return
      if(infstk(top).eq.2) then
         istk(ilr)=1
      else
         istk(ilr)=0
      endif
      return
      end
