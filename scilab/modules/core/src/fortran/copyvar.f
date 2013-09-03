c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine copyvar(il,sz)
c =============================================================
c     copy a variable  at the top of the stack
c =============================================================
c
      include 'stack.h'
c
c     il : pointer on the beginning of the variable
c     sz : memory size used by the variable (in stk words)
c     on return top is incremented
      integer il,sz
c
      integer sadr
c
c
      sadr(l)=(l/2)+1
c
      top=top+1
      err=lstk(top)+sz-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if(top.ge.bot) then
         call error(18)
         return
      endif
      call unsfdcopy(sz,stk(sadr(il)),1,stk(lstk(top)),1)
      lstk(top+1)=lstk(top)+sz
      return
      end
