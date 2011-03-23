c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt 
      subroutine intdebug()
      include 'stack.h'
      logical checkrhs,checklhs,getscalar,cremat
c
      rhs=max(0,rhs)
      if(.not.checklhs('debug',1,1)) return
      if(.not.checkrhs('debug',0,1)) return
      if (rhs.eq.1) then
c     .  set debug mode
         if(.not.getscalar('debug',top,top,l)) return
         ddt = int(stk(l))
         write(buf(1:4),'(i4)') ddt
         call basout(io,wte,' debug '//buf(1:4))
         call objvide('debug',top)
      else
c     .  get debug mode
         top=top+1
         if(.not.cremat('debug',top,0,1,1,lr,lc)) return
         stk(lr)=ddt
      endif 
      return
      end
