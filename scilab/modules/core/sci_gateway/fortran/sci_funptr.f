c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


      subroutine intfunptr
      include 'stack.h'
      integer id(nsiz)
      logical checkrhs,checklhs,cremat,getsmat,checkval
      integer topk,fptr
c
      topk=top

      if(.not.checklhs('funptr',1,1)) return
      if(.not.checkrhs('funptr',1,1)) return

      if(.not.getsmat('funptr',topk,top,m,n,1,1,l,n1)) return
      if(.not.checkval('funptr',m*n,1) ) return
      call namstr(id,istk(l),n1,0)
      call funtab(id,fptr,1,'NULL_NAME',0)
      if(.not.cremat('funptr',top,0,1,1,l,lc)) return
      stk(l)=dble(fptr)
      return
      end

 
