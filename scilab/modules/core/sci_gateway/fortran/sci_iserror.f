c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt 
      subroutine intiserror
      include 'stack.h'
      logical checkrhs,checklhs,getscalar,cremat
c
      rhs=max(0,rhs)

      if(.not.checklhs('iserror',1,1)) return
      if(.not.checkrhs('iserror',0,1)) return

      if(rhs.eq.1) then
         if(.not.getscalar('iserror',top,top,l)) return
         num=nint(stk(l))
      else
         num=0
         top=top+1
      endif
      if(.not.cremat('iserror',top,0,1,1,l,lc)) return
      if(num.ge.1) then
         if(err2.eq.num) then
            stk(l)=1.0d+0
         else
            stk(l)=0.0d+0
         endif
      else
         if(err2.ne.0) then
            stk(l)=1.0d+0
         else
            stk(l)=0.0d+0
         endif
      endif
      return
      end
