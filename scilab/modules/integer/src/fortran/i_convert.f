c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine i_convert
      include 'stack.h'
c
      logical checkrhs,checklhs
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(.not.checkrhs('iconvert',2,2)) return
      if(.not.checklhs('iconvert',1,1)) return

      top0=top
      il=iadr(lstk(top))
      if(istk(il).lt.0) il=istk(il+1)
      if(istk(il).ne.1) then
         err=2
         call error(53)
         return
      endif
      is=stk(sadr(il+4))
      top=top-1
      rhs=rhs-1
      call stack_convert(is)
      return
      end

