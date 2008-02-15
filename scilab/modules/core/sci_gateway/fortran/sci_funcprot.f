c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
  
      subroutine intfuncprot
      include 'stack.h'
      logical checkrhs,checklhs,cremat,getscalar
      integer topk
      common /mprot/ macprt
c
      rhs=max(rhs,0)
      if(.not.checklhs('funcprot',1,1)) return
      if(.not.checkrhs('funcprot',0,1)) return

      if (rhs .eq. 0) then
c     .  get value
         top=top+1
         if(.not.cremat('funcprot',top,0,1,1,l,lc)) return
         stk(l)=macprt
         return
      else
c     .  set value
         if(.not.getscalar('funcprot',top,top,l)) return
         m=int(stk(l))
         if(m.lt.0.or.m.gt.2) then
            err=1
            call error(116)
            return
         endif
         macprt=m
         call objvide('funcprot',top)
      endif
      return
      end
