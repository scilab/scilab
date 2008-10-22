c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt 
      subroutine intmode
      include 'stack.h'
      integer cmode,topk
      logical checkrhs,checklhs,getscalar,cremat
      integer iadr,sadr
c    
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      rhs=max(rhs,0)
      if(.not.checklhs('mode',1,1)) return
      if(.not.checkrhs('mode',0,1)) return

      if (rhs.eq.1) then
c     .  set  line mode value
         if(.not.getscalar('mode',top,top,l)) return
         lct(4)=int(stk(l))
         if(lct(4).eq.7.or.lct(4).eq.4) call msgs(26,0)
         call objvide('mode',top)
      else
c     .  get line  mode value
         top=top+1
         if(.not.cremat('mode',top,0,1,1,lr,lc)) return
         stk(lr)=lct(4)
      endif
      return
      end
      
